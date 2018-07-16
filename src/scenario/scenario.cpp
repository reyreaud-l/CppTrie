#include "scenario.hpp"

std::string word_modify(const std::string& s, std::mt19937& gen)
{
  std::string res;
  std::uniform_real_distribution<> rg;

  std::size_t pos = 0;
  std::size_t n = s.size();
  while (pos < n)
  {
    auto x = rg(gen);
    if (x < 0.8)
      res.push_back((char)s[pos++]);
    else if (x < 0.85) // deletion
      pos++;
    else if (x < 0.9) // insertion
      res.push_back((char)('a' + 26 * ((x - 0.85) / 0.05)));
    else if (x < 0.95) // mutation
    {
      res.push_back((char)('a' + 26 * ((x - 0.9) / 0.05)));
      pos++;
    }
    else if (pos + 1 < n) //
    {
      res.append({s[pos + 1], s[pos]});
      pos += 2;
    }
  }

  return res;
}

Scenario::Scenario()
  : m_impl(std::make_unique<scenario_impl_t>())
{
}

Scenario::~Scenario()
{
}

Scenario::Scenario(const std::vector<std::string>& word_list,
                   std::size_t nqueries)
  : m_impl(std::make_unique<scenario_impl_t>())
{
  std::random_device rd;
  std::mt19937 gen(rd());

  this->m_impl->queries.reserve(nqueries);

  // Create queries with these distributions
  // 70% search
  // 15% insert
  // 15% erase

  const std::size_t n_searches = nqueries * 70 / 100;
  const std::size_t n_indels = nqueries * 15 / 100;

  const std::size_t m = word_list.size();
  std::size_t search_list_size = m * 70 / 100;
  std::size_t indels_list_size = m * 15 / 100;
  auto search_list = word_list.begin();
  auto erase_list = word_list.begin() + search_list_size;
  auto insert_list = word_list.begin() + search_list_size + indels_list_size;

  // Init dictionary with 85%
  this->m_impl->init_word_list =
    std::vector<std::string>(search_list, insert_list);

  // Generate search queries
  {
    std::uniform_int_distribution<> rand(0, search_list_size);
    for (std::size_t i = 0; i < n_searches; ++i)
    {
      const auto& x = search_list[rand(gen)];
      this->m_impl->queries.push_back({query::search, word_modify(x, gen)});
    }
  }

  // Generate insert/erase queries
  {
    std::uniform_int_distribution<> rand(0, indels_list_size);
    for (std::size_t i = 0; i < n_indels; ++i)
    {
      this->m_impl->queries.push_back({query::insert, insert_list[rand(gen)]});
      this->m_impl->queries.push_back({query::erase, erase_list[rand(gen)]});
    }
  }

  // Shuffle everything
  std::shuffle(this->m_impl->queries.begin(), this->m_impl->queries.end(), gen);
}

void Scenario::prepare(IDictionary& dic) const
{
  dic.init(m_impl->init_word_list);
}

void Scenario::prepare(IAsyncDictionary& dic) const
{
  dic.init(m_impl->init_word_list);
}

std::vector<result_t> Scenario::execute(IDictionary& dic) const
{
  std::size_t n = this->m_impl->queries.size();
  std::vector<result_t> results;

  results.reserve(n * 7 / 10);
  for (auto&& q : this->m_impl->queries)
  {
    switch (q.op)
    {
    case query::search:
      results.push_back(dic.search(q.arg));
      break;
    case query::insert:
      dic.insert(q.arg);
      break;
    case query::erase:
      dic.erase(q.arg);
      break;
    }
  }

  return results;
}

std::vector<result_t> Scenario::execute_verbose(IDictionary& dic) const
{
  std::size_t n = this->m_impl->queries.size();
  std::vector<result_t> results;

  results.reserve(n * 7 / 10);

  std::cout << "I: {\n";
  for (const auto& w : m_impl->init_word_list)
    std::cout << w << ", ";
  std::cout << "}\n";

  for (auto&& q : this->m_impl->queries)
  {
    switch (q.op)
    {
    case query::search:
    {
      auto r = dic.search(q.arg);
      std::cout << "S: " << q.arg << " -> "
                << "(" << r.first << ", " << r.second << ")\n";
      results.push_back(r);
      break;
    }
    case query::insert:
      std::cout << "A: " << q.arg << "\n";
      dic.insert(q.arg);
      break;
    case query::erase:
      std::cout << "D: " << q.arg << "\n";
      dic.erase(q.arg);
      break;
    }
  }
  return results;
}

std::vector<result_t> Scenario::execute(IAsyncDictionary& dic) const
{
  std::size_t n = this->m_impl->queries.size();

  std::vector<result_t> results;
  results.reserve(n * 7 / 10);

  constexpr int SEARCH_BUFFER_SIZE = 50;
  constexpr int INDEL_BUFFER_SIZE = 50;

  std::future<result_t> search_buffer[SEARCH_BUFFER_SIZE];
  std::future<void> indel_buffer[INDEL_BUFFER_SIZE];

  unsigned search_buffer_pos = 0;
  unsigned indel_buffer_pos = 0;

  for (auto&& q : this->m_impl->queries)
  {
    if (q.op == query::search)
    {
      if (search_buffer[search_buffer_pos].valid())
        results.push_back(search_buffer[search_buffer_pos].get());
    }
    else
    {
      if (indel_buffer[indel_buffer_pos].valid())
        indel_buffer[indel_buffer_pos].get();
    }

    switch (q.op)
    {
    case query::search:
      search_buffer[search_buffer_pos] = dic.search(q.arg); // non-blocking
      search_buffer_pos = (search_buffer_pos + 1) % SEARCH_BUFFER_SIZE;
      break;
    case query::insert:
      indel_buffer[indel_buffer_pos] = dic.insert(q.arg);
      indel_buffer_pos = (indel_buffer_pos + 1) % INDEL_BUFFER_SIZE;
      break;
    case query::erase:
      indel_buffer[indel_buffer_pos] = dic.erase(q.arg);
      indel_buffer_pos = (indel_buffer_pos + 1) % INDEL_BUFFER_SIZE;
      break;
    }
  }

  for (int i = 0; i < SEARCH_BUFFER_SIZE; ++i)
  {
    if (search_buffer[search_buffer_pos].valid())
      results.push_back(search_buffer[search_buffer_pos].get());
    search_buffer_pos = (search_buffer_pos + 1) % SEARCH_BUFFER_SIZE;
  }
  return results;
}

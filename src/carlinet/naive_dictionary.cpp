#include "naive_dictionary.hpp"
#include "tools.hpp"



naive_dictionary::naive_dictionary(const std::initializer_list<std::string>& init)
  : m_set(init.begin(), init.end())
{
}


void naive_dictionary::init(const std::vector<std::string>& word_list)
{
  m_set = std::set<std::string>(word_list.begin(), word_list.end());
}


result_t naive_dictionary::search(const std::string& query) const
{
  std::lock_guard l(m);

  std::string best;
  int distance = std::numeric_limits<int>::max();
  for (const auto& word : m_set)
  {
    int d = levenshtein(query, word);
    if (d < distance)
    {
      best = word;
      distance = d;
    }
  }

  return {best, distance};
}


void naive_dictionary::insert(const std::string& w)
{
  std::lock_guard l(m);
  m_set.insert(w);
}

void naive_dictionary::erase(const std::string& w)
{
  std::lock_guard l(m);
  m_set.erase(w);
}

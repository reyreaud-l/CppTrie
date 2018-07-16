#include "scenario_insert.hpp"

ScenarioInsert::~ScenarioInsert()
{
}

ScenarioInsert::ScenarioInsert(const std::vector<std::string>& word_list,
                               std::size_t nqueries)
  : Scenario()
{
  std::random_device rd;
  std::mt19937 gen(rd());

  this->m_impl->queries.reserve(nqueries);

  this->m_impl->init_word_list = std::vector<std::string>();

  std::uniform_int_distribution<> rand(0, nqueries);
  for (std::size_t i = 0; i < nqueries; ++i)
    this->m_impl->queries.push_back({query::insert, word_list[rand(gen)]});

  // Shuffle everything
  std::shuffle(this->m_impl->queries.begin(), this->m_impl->queries.end(), gen);
}

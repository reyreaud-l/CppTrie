#include "scenario.hpp"

class ScenarioSearch : public Scenario
{
public:
  virtual ~ScenarioSearch();

  ScenarioSearch(const std::vector<std::string>& word_list,
                 std::size_t nqueries);
};

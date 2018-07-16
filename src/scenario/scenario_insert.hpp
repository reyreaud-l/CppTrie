#include "scenario.hpp"

class ScenarioInsert : public Scenario
{
public:
  virtual ~ScenarioInsert();

  ScenarioInsert(const std::vector<std::string>& word_list,
                 std::size_t nqueries);
};

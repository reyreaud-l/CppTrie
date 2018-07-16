#include "scenario.hpp"

class ScenarioErase : public Scenario
{
public:
  virtual ~ScenarioErase();

  ScenarioErase(const std::vector<std::string>& word_list,
                std::size_t nqueries);
};

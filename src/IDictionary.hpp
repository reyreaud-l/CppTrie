#pragma once

#include <string>
#include <utility>
#include <vector>

// Result = (word, distance)
using result_t = std::pair<std::string, int>;

class IDictionaryBase
{
public:
  // Reset the dictionary with this set of words
  virtual void init(const std::vector<std::string>& word_list) = 0;
};

class IDictionary : public IDictionaryBase
{
public:
  IDictionary() = default;

  virtual result_t search(const std::string& w) const = 0;
  virtual void insert(const std::string& w) = 0;
  virtual void erase(const std::string& w) = 0;
};

#pragma once
#include "IDictionary.hpp"
#include <set>
#include <mutex>
#include <vector>

class naive_dictionary : public IDictionary
{
public:
  naive_dictionary() = default;
  naive_dictionary(const std::initializer_list<std::string>& init);

  template <class Iterator>
  naive_dictionary(Iterator begin, Iterator end);

  void init(const std::vector<std::string>& word_list) final;


  result_t      search(const std::string& w) const final;
  void          insert(const std::string& w) final;
  void          erase(const std::string& w) final;

private:
  std::set<std::string> m_set;
  mutable std::mutex m;
};



template <class Iterator>
naive_dictionary::naive_dictionary(Iterator begin, Iterator end)
  : m_set(begin, end)
{
}

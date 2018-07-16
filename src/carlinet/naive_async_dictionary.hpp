#pragma once

#include "IAsyncDictionary.hpp"
#include "naive_dictionary.hpp"

/// The naive implementation is blocking to ensure Sequential Consistency
class naive_async_dictionary : public IAsyncDictionary
{
public:
  naive_async_dictionary() = default;

  template <class Iterator>
  naive_async_dictionary(Iterator begin, Iterator end);
  naive_async_dictionary(const std::initializer_list<std::string>& init);
  void init(const std::vector<std::string>& word_list) final;

  std::future<result_t> search(const std::string& w) const final;
  std::future<void> insert(const std::string& w) final;
  std::future<void> erase(const std::string& w) final;

private:
  naive_dictionary m_dic;
};

template <class Iterator>
naive_async_dictionary::naive_async_dictionary(Iterator begin, Iterator end)
  : m_dic(begin, end)
{
}

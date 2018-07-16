#include "naive_async_dictionary.hpp"



naive_async_dictionary::naive_async_dictionary(const std::initializer_list<std::string>& init)
  : m_dic(init)
{
}

void naive_async_dictionary::init(const std::vector<std::string>& word_list)
{
  m_dic.init(word_list);
}


std::future<result_t> naive_async_dictionary::search(const std::string& query) const
{
  std::promise<result_t> p;
  p.set_value(m_dic.search(query));
  return p.get_future();
}


std::future<void> naive_async_dictionary::insert(const std::string& w)
{
  std::promise<void> p;
  m_dic.insert(w);
  p.set_value();
  return p.get_future();
}

std::future<void> naive_async_dictionary::erase(const std::string& w)
{
  std::promise<void> p;
  m_dic.erase(w);
  p.set_value();
  return p.get_future();
}

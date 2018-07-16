#pragma once
#include "IDictionary.hpp"
#include <future>

class IAsyncDictionary : public IDictionaryBase
{
public:
  IAsyncDictionary() = default;

  virtual std::future<result_t> search(const std::string& w) const = 0;
  virtual std::future<void> insert(const std::string& w) = 0;
  virtual std::future<void> erase(const std::string& w) = 0;
};

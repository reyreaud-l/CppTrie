#include "trie_async_global.hpp"

AsyncGlobalTrie::AsyncGlobalTrie()
{
}

void AsyncGlobalTrie::init(const std::vector<std::string>& words)
{
  for (const auto& w : words)
    this->insert(w);
}

std::future<result_t> AsyncGlobalTrie::search(const std::string& query) const
{
  FUNCTION(__FUNCTION__);
  return std::async(std::launch::async, [&]() -> result_t {
    SHARED_LOCK(m);
    auto res = this->trie_.search(query);
    return res;
  });
}

std::future<void> AsyncGlobalTrie::insert(const std::string& w)
{
  FUNCTION(__FUNCTION__);
  return std::async(std::launch::async, [&]() -> void {
    UNIQUE_LOCK(m);
    this->trie_.insert(w);
  });
}

std::future<void> AsyncGlobalTrie::erase(const std::string& w)
{
  FUNCTION(__FUNCTION__);
  return std::async(std::launch::async, [&]() -> void {
    UNIQUE_LOCK(m);
    this->trie_.erase(w);
  });
}

void AsyncGlobalTrie::print(std::string prefix)
{
  this->trie_.print(prefix);
}

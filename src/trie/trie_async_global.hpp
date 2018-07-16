#pragma once

#include <algorithm>
#include <atomic>
#include <exception>
#include <functional>
#include <future>
#include <iomanip>
#include <iostream>
#include <memory>
#include <shared_mutex>
#include <unordered_map>

#include "IAsyncDictionary.hpp"
#include "tools.hpp"
#include "trie.hpp"

#define SIZE 26

#define SHARED_LOCK(X) std::shared_lock<decltype(X)> lk(X)
#define UNIQUE_LOCK(X) std::lock_guard<decltype(X)> lk(X)

class AsyncGlobalTrie : public IAsyncDictionary
{
public:
  AsyncGlobalTrie();

  template <class Iterator>
  AsyncGlobalTrie(Iterator begin, Iterator end);

  void init(const std::vector<std::string>&) final;

  std::future<result_t> search(const std::string&) const final;
  std::future<void> insert(const std::string&) final;
  std::future<void> erase(const std::string&) final;

  void print(std::string = std::string(""));

private:
  Trie trie_;
  mutable std::shared_mutex m;
};

template <class Iterator>
AsyncGlobalTrie::AsyncGlobalTrie(Iterator begin, Iterator end)
  : AsyncGlobalTrie()
{
  for (; begin != end; begin++)
    this->insert(*begin).get();
}

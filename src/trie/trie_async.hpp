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

#define SIZE 26

class AsyncNode
{
public:
  AsyncNode();
  ~AsyncNode();

  std::atomic<bool> leaf_ = false;
  std::atomic<bool> is_deleted_ = false;
  std::atomic<AsyncNode*> children_[SIZE];
};

class AsyncTrie : public IAsyncDictionary
{
public:
  AsyncTrie();

  template <class Iterator>
  AsyncTrie(Iterator begin, Iterator end);

  void init(const std::vector<std::string>&) final;

  std::future<result_t> search(const std::string&) const final;
  std::future<void> insert(const std::string&) final;
  std::future<void> erase(const std::string&) final;

  int erase_rec(AsyncNode*, const char*);
  int have_children(AsyncNode*);

  void search_rec(std::string, AsyncNode*, const char, const char, int*, int*,
                  std::size_t, const std::string&, result_t&) const;
  void print(std::string = std::string(""));
  void print_rec(std::string, AsyncNode*);

private:
  std::unique_ptr<AsyncNode> root;
  mutable std::shared_mutex m;
};

template <class Iterator>
AsyncTrie::AsyncTrie(Iterator begin, Iterator end)
  : AsyncTrie()
{
  for (; begin != end; begin++)
    this->insert(*begin).get();
}

#pragma once

#include <algorithm>
#include <exception>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>

#include "IDictionary.hpp"
#include "tools.hpp"

#define SIZE 26

class Node
{
public:
  Node();
  ~Node();

  bool leaf_ = false;
  bool is_deleted_ = false;
  std::string value_ = std::string("");
  Node* children_[SIZE];
};

class Trie : public IDictionary
{
public:
  Trie();

  template <class Iterator>
  Trie(Iterator begin, Iterator end);

  void init(const std::vector<std::string>&) final;

  result_t search(const std::string&) const final;
  void insert(const std::string&) final;
  void erase(const std::string&) final;

  void search_rec(Node*, const char, const char, int*, int*, std::size_t,
                  const std::string&, result_t&) const;
  int erase_rec(Node**, const char*);
  int have_children(Node*);

  void print(std::string = std::string(""));
  void print_rec(std::string, Node*);

private:
  std::unique_ptr<Node> root;
};

template <class Iterator>
Trie::Trie(Iterator begin, Iterator end)
{
  for (; begin != end; begin++)
    this->insert(*begin);
}

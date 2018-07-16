#pragma once

#include "IAsyncDictionary.hpp"
#include "IDictionary.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>
#include <vector>

struct query
{
  enum op_type
  {
    search,
    insert,
    erase
  };

  op_type op;
  std::string arg;
};

struct scenario_impl_t
{
  std::vector<std::string> init_word_list;
  std::vector<query> queries;
};

std::string word_modify(const std::string&, std::mt19937&);

class Scenario
{
public:
  Scenario();
  Scenario(Scenario&&) = default;
  virtual ~Scenario();

  // Create a scenario from a word list
  Scenario(const std::vector<std::string>& word_list, std::size_t nqueries);

  // Prepare and populate the dictionary
  virtual void prepare(IDictionary& dic) const;
  virtual void prepare(IAsyncDictionary& dic) const;

  // Execute a scenario sync-way
  virtual std::vector<result_t> execute(IDictionary& dic) const;

  // Execute a scenario async-way
  virtual std::vector<result_t> execute(IAsyncDictionary& dic) const;

  virtual std::vector<result_t> execute_verbose(IDictionary& dic) const;

protected:
  std::unique_ptr<scenario_impl_t> m_impl;
};

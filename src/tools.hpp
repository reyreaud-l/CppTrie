#pragma once
#include "IAsyncDictionary.hpp"
#include "IDictionary.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

///! Compute the Optimal string alignment distance between two strings
int levenshtein(const std::string& a, const std::string& b);

///! Load the word list from a file
std::vector<std::string> load_word_list(const char* filename = nullptr,
                                        bool shuffle = true);
class ScopeLogger
{
public:
  ScopeLogger(std::string const& msg)
    : msg(msg)
  {
    std::cout << "Enter: " << msg << std::endl;
  }
  ~ScopeLogger()
  {
    std::cout << "Exit:  " << msg << std::endl;
  }
  std::string msg;
};
#ifdef DEBUG
#define FUNCTION(x) ScopeLogger l_##x##_scope(x);
#else
#define FUNCTION(x)
#endif

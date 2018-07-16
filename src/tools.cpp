#include "tools.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

int levenshtein(const std::string& a, const std::string& b)
{
  int n = a.size();
  int m = b.size();

  std::vector<char> pprev(m + 1);
  std::vector<char> prev(m + 1);
  std::vector<char> current(m + 1);

  std::iota(current.begin(), current.end(), 0);

  for (int y = 1; y <= n; ++y)
  {
    std::swap(pprev, prev);
    std::swap(prev, current);
    current[0] = y;
    for (int x = 1; x <= m; ++x)
    {
      if (a[y - 1] == b[x - 1])
        current[x] = prev[x - 1];
      else
      {
        int tmp = std::min({current[x - 1], prev[x - 1], prev[x]});
        // Test swap
        if (x >= 2 && y >= 2 && a[y - 1] == b[x - 2] && a[y - 2] == b[x - 1])
          if (pprev[x - 2] < tmp)
            tmp = pprev[x - 2];

        current[x] = 1 + tmp;
      }
    }
  }

  return current[m];
}

std::vector<std::string> load_word_list(const char* filename, bool shuffle)
{
  std::ifstream f;
  if (!filename)
    filename = WORD_LIST_FILE;
  f.open(filename);

  std::string s;
  std::vector<std::string> data;
  for (std::string s; std::getline(f, s);)
    data.push_back(s);

  if (shuffle)
  {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(data.begin(), data.end(), g);
  }

  return data;
}

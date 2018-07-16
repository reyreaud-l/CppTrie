#pragma once

#include "naive_async_dictionary.hpp"
#include "naive_dictionary.hpp"
#include "trie.hpp"
#include "trie_async.hpp"
#include "trie_async_global.hpp"

#include "tools.hpp"

#include "scenario.hpp"
#include "scenario_erase.hpp"
#include "scenario_insert.hpp"
#include "scenario_search.hpp"

#include <benchmark/benchmark.h>
#include <functional>
#include <iostream>

namespace bench
{
#define BENCH(CLASS, NAME, STRUCT)                                             \
  BENCHMARK_DEFINE_F(Bench##CLASS, NAME)(benchmark::State & st)                \
  {                                                                            \
    STRUCT dic;                                                                \
    m_scenario->prepare(dic);                                                  \
    for (auto _ : st)                                                          \
      m_scenario->execute(dic);                                                \
    st.SetItemsProcessed(st.iterations() * nb);                                \
  }                                                                            \
  BENCHMARK_REGISTER_F(Bench##CLASS, NAME)                                     \
    ->Unit(benchmark::kMillisecond)                                            \
    ->UseRealTime()

constexpr int low = 100;
constexpr int mid = 1000;
constexpr int high = 10000;

constexpr int lowop = 1000;
constexpr int midop = 10000;
constexpr int highop = 100000;

#define BENCH_CLASS(NAME, CLASS)                                               \
  class Bench##NAME : public ::benchmark::Fixture                              \
  {                                                                            \
  public:                                                                      \
    void SetUp(benchmark::State&)                                              \
    {                                                                          \
      if (!m_scenario)                                                         \
      {                                                                        \
        auto wl = load_word_list();                                            \
        m_scenario = std::make_unique<CLASS>(wl, nb);                          \
      }                                                                        \
    }                                                                          \
                                                                               \
  protected:                                                                   \
    static size_t nb;                                                          \
    static std::unique_ptr<CLASS> m_scenario;                                  \
  }

#define INIT_BENCH_CLASS(NAME, CLASS, ITER)                                    \
  std::unique_ptr<CLASS> Bench##NAME::m_scenario;                              \
  size_t Bench##NAME::nb = ITER

// Generic
BENCH_CLASS(GenericShort, Scenario);
BENCH_CLASS(GenericLong, Scenario);

// Insert
BENCH_CLASS(Insert, ScenarioInsert);

// Erase
BENCH_CLASS(Erase, ScenarioErase);

// Search
BENCH_CLASS(Search, ScenarioSearch);
} // namespace bench

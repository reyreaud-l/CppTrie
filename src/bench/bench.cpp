#include "bench.hpp"

namespace bench
{
INIT_BENCH_CLASS(GenericShort, Scenario, mid);
INIT_BENCH_CLASS(GenericLong, Scenario, high);

INIT_BENCH_CLASS(Insert, ScenarioInsert, highop);

INIT_BENCH_CLASS(Erase, ScenarioErase, highop);

INIT_BENCH_CLASS(Search, ScenarioSearch, mid);

// Naive
BENCH(GenericShort, Naive_NoAsync, naive_dictionary);
BENCH(GenericLong, Naive_NoAsync, naive_dictionary);

BENCH(GenericShort, Naive_Async, naive_async_dictionary);
BENCH(GenericLong, Naive_Async, naive_async_dictionary);

BENCH(Insert, Naive_NoAsync, naive_dictionary);
BENCH(Insert, Naive_Async, naive_async_dictionary);

BENCH(Erase, Naive_NoAsync, naive_dictionary);
BENCH(Erase, Naive_Async, naive_async_dictionary);

BENCH(Search, Naive_NoAsync, naive_dictionary);
BENCH(Search, Naive_Async, naive_async_dictionary);
// Naive

// Trie
BENCH(GenericShort, Trie_NoAsync, Trie);
BENCH(GenericLong, Trie_NoAsync, Trie);

BENCH(GenericShort, Trie_GlobalAsync, AsyncGlobalTrie);
BENCH(GenericLong, Trie_GlobalAsync, AsyncGlobalTrie);

BENCH(GenericShort, Trie_Async, AsyncTrie);
BENCH(GenericLong, Trie_Async, AsyncTrie);

BENCH(Insert, Trie_NoAsync, Trie);
BENCH(Insert, Trie_GlobalAsync, AsyncGlobalTrie);
BENCH(Insert, Trie_Async, AsyncTrie);

BENCH(Erase, Trie_NoAsync, Trie);
BENCH(Erase, Trie_GlobalAsync, AsyncGlobalTrie);
BENCH(Erase, Trie_Async, AsyncTrie);

BENCH(Search, Trie_NoAsync, Trie);
BENCH(Search, Trie_GlobalAsync, AsyncGlobalTrie);
BENCH(Search, Trie_Async, AsyncTrie);
// Trie
} // namespace bench

BENCHMARK_MAIN();

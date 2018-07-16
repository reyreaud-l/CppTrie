C++ Asynchronous Dictionary
===========================

This repository contains different implementation of a dictionary in C++.
The dictionary can either insert/erase/search words in it. When searching, it
will return a pair containing the closest word matching the query and the
Damerau-levenshtein distance of the query and the closes word.

There are multiple implementations in this repository. Some of them are
asynchronous and some are synchronous. You can measure the different
implementations performance with the bench running:

```
$ ./bench --benchmark_filter='BenchGenericShort/*'
```

Example benchmark result:
```
2018-06-21 10:16:11
Running ./bench
Run on (4 X 3500 MHz CPU s)
CPU Caches:
  L1 Data 32K (x4)
  L1 Instruction 32K (x4)
  L2 Unified 256K (x4)
  L3 Unified 6144K (x1)
-----------------------------------------------------------------------------------
Benchmark                                             Time           CPU Iterations
------------------------------------------------------------------------------------
BenchGenericShort/Naive_NoAsync/real_time        107255 ms     107137 ms          1    9.32356 items/s
BenchGenericShort/Naive_Async/real_time          107783 ms     107644 ms          1    9.27792 items/s
BenchGenericShort/Trie_NoAsync/real_time           5664 ms       5657 ms          1    176.554 items/s
BenchGenericShort/Trie_GlobalAsync/real_time       1958 ms         11 ms          1    510.754 items/s
BenchGenericShort/Trie_Async/real_time             1834 ms         10 ms          1    545.189 items/s

```

Performances
------------

The most efficient implementation is the `TrieAsync` class. It uses atomic
operations to ensure safety. Careful as it uses a logical delete. If you need to
be efficient in memory using a lot of `erase` procedure, this implementation
will fail as it does not free memory unless the `Trie` is destroyed.

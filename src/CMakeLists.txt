set_source_files_properties(tools.cpp
  PROPERTIES
  COMPILE_DEFINITIONS WORD_LIST_FILE="${CMAKE_SOURCE_DIR}/words_alpha.txt"
)

add_library(dictionary SHARED
  IDictionary.hpp
  IAsyncDictionary.hpp
  tools.hpp

  tools.cpp
)
target_include_directories(dictionary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(carlinet SHARED
  carlinet/naive_dictionary.hpp
  carlinet/naive_async_dictionary.hpp

  carlinet/naive_dictionary.cpp
  carlinet/naive_async_dictionary.cpp
)
target_include_directories(carlinet PUBLIC carlinet)
target_link_libraries(carlinet PUBLIC dictionary)

add_library(trie SHARED
  trie/trie.hpp
  trie/trie_async.hpp
  trie/trie_async_global.hpp

  trie/trie.cpp
  trie/trie_async.cpp
  trie/trie_async_global.cpp
)
target_include_directories(trie PUBLIC trie)
target_link_libraries(trie PUBLIC dictionary)

add_library(scenario SHARED
  scenario/scenario.hpp
  scenario/scenario_insert.hpp
  scenario/scenario_erase.hpp
  scenario/scenario_search.hpp

  scenario/scenario.cpp
  scenario/scenario_insert.cpp
  scenario/scenario_erase.cpp
  scenario/scenario_search.cpp
)
target_link_libraries(scenario PUBLIC dictionary)
target_include_directories(scenario PUBLIC scenario)

add_executable(bench
  "${CMAKE_CURRENT_SOURCE_DIR}/bench/bench.cpp"
)
target_link_libraries(bench PUBLIC
  trie
  carlinet
  scenario

  GTest::GTest
  GTest::Main
  Threads::Threads
  benchmark::benchmark
)

add_executable(tests
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/main.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/carlinet_test.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/trie_test.cpp"
)
target_link_libraries(tests PUBLIC
  trie
  carlinet
  scenario

  GTest::GTest
  GTest::Main
  Threads::Threads
)

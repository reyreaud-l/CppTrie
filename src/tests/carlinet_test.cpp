#include "tests.hpp"

using namespace std::string_literals;

// TODO
// Adapt/Create new tests tests with your new structures
// naive_dictionary/async_naive_dictionary can be used as references

// A basic add/remove/search test
TEST(Dictionary, Basic)
{
  naive_dictionary dic = {"massue", "lamasse", "massive"};

  ASSERT_EQ(dic.search("massive"), std::make_pair("massive"s, 0));
  ASSERT_EQ(dic.search("lessive"), std::make_pair("massive"s, 2));
  ASSERT_EQ(dic.search("limace"), std::make_pair("lamasse"s, 3));
  ASSERT_EQ(dic.search("masseur"), std::make_pair("massue"s, 2));

  dic.insert("masseur");
  ASSERT_EQ(dic.search("masseur"), std::make_pair("masseur"s, 0));

  dic.erase("masseur");
  ASSERT_EQ(dic.search("masseur"), std::make_pair("massue"s, 2));
}

TEST(Dictionary, ConcurrentOperations)
{
  using namespace std::placeholders;

  std::vector<std::string> data = load_word_list();
  std::size_t n = 1000;

  naive_async_dictionary dic(data.begin(), data.begin() + 4 * n);

  std::thread t[6];

  t[0] = std::thread([&dic, &data, n]() {
    std::for_each(data.begin() + 0 * n, data.begin() + 1 * n,
                  std::bind(&IAsyncDictionary::search, &dic, _1));
  });

  t[1] = std::thread([&dic, &data, n]() {
    std::for_each(data.begin() + 1 * n, data.begin() + 2 * n,
                  std::bind(&IAsyncDictionary::search, &dic, _1));
  });

  t[2] = std::thread([&dic, &data, n]() {
    std::for_each(data.begin() + 2 * n, data.begin() + 3 * n,
                  std::bind(&IAsyncDictionary::erase, &dic, _1));
  });
  t[3] = std::thread([&dic, &data, n]() {
    std::for_each(data.begin() + 3 * n, data.begin() + 4 * n,
                  std::bind(&IAsyncDictionary::erase, &dic, _1));
  });

  t[4] = std::thread([&dic, &data, n]() {
    std::for_each(data.begin() + 4 * n, data.begin() + 5 * n,
                  std::bind(&IAsyncDictionary::insert, &dic, _1));
  });
  t[5] = std::thread([&dic, &data, n]() {
    std::for_each(data.begin() + 5 * n, data.begin() + 6 * n,
                  std::bind(&IAsyncDictionary::insert, &dic, _1));
  });

  for (int i = 0; i < 6; ++i)
    t[i].join();
}

// A simple scenario
TEST(Dictionary, SimpleScenario)
{
  std::vector<std::string> word_list = load_word_list();
  word_list.resize(20);

  Scenario scn(word_list, 20);

  naive_dictionary dic;
  scn.prepare(dic);
  scn.execute(dic);
}

// A long scenario, check that the async dictionary as the
// same output as the blocking one
TEST(Dictionary, LongScenario)
{
  std::vector<std::string> word_list = load_word_list();
  word_list.resize(100000);

  Scenario scn(word_list, 512);

  naive_dictionary dic;
  naive_async_dictionary async_dic;
  scn.prepare(dic);
  scn.prepare(async_dic);
  auto r1 = scn.execute(async_dic);
  auto r2 = scn.execute(dic);
  ASSERT_EQ(r1, r2);
}

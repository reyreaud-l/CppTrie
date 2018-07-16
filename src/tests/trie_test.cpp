#include "tests.hpp"

using namespace std::string_literals;
// A basic add/remove/search test
TEST(TrieDico, Basic)
{
  Trie dic;

  dic.init({"massue", "lamasse", "massive"});

  ASSERT_EQ(dic.search("massive"), std::make_pair("massive"s, 0));
  ASSERT_EQ(dic.search("lessive"), std::make_pair("massive"s, 2));
  ASSERT_EQ(dic.search("limace"), std::make_pair("lamasse"s, 3));
  ASSERT_EQ(dic.search("masseur"), std::make_pair("massue"s, 2));

  dic.insert("masseur");
  ASSERT_EQ(dic.search("masseur"), std::make_pair("masseur"s, 0));
  dic.insert("untrustwhorthily");
  dic.insert("eyra");
  ASSERT_EQ(dic.search("untjlstworthvaily"),
            std::make_pair("untrustwhorthily"s, 5));

  dic.erase("masseur");
  ASSERT_EQ(dic.search("masseur"), std::make_pair("massue"s, 2));

  dic.insert("ab");
  ASSERT_EQ(dic.search("ba"), std::make_pair("ab"s, 1));
  dic.insert("holdfasts");
  ASSERT_EQ(dic.search("hodfmatss"), std::make_pair("holdfasts"s, 3));
  dic.insert("kitten");
  ASSERT_EQ(dic.search("sitting"), std::make_pair("kitten"s, 3));
}

TEST(TrieDico, BasicAsync)
{
  AsyncTrie dic;

  auto i1 = dic.insert("coucou");
  auto i2 = dic.insert("couscous");
  i1.wait();
  i2.wait();
  auto s1 = dic.search("coucou");
  s1.wait();
  ASSERT_EQ(s1.get(), std::make_pair("coucou"s, 0));
  /*ASSERT_EQ(dic.search("couscous").get(), std::make_pair("coucou"s, 2));
  ASSERT_EQ(dic.search("coucous").get(), std::make_pair("coucou"s, 1));
  ASSERT_EQ(dic.search("massue").get(), std::make_pair("massue"s, 0));
  ASSERT_EQ(dic.search("massive").get(), std::make_pair("massive"s, 0));
  ASSERT_EQ(dic.search("lessive").get(), std::make_pair("massive"s, 2));
  ASSERT_EQ(dic.search("limace").get(), std::make_pair("lamasse"s, 3));
  ASSERT_EQ(dic.search("masseur").get(), std::make_pair("massue"s, 2));*/
}

TEST(TrieDico, BasicGlobalAsync)
{
  AsyncGlobalTrie dic;

  dic.init({"massue", "lamasse", "massive"});
  auto i1 = dic.insert("coucou");
  auto i2 = dic.insert("coucous");
  auto i3 = dic.insert("couscous");
  auto d1 = dic.erase("couscous");
  auto d2 = dic.erase("coucous");
  i1.wait();
  i2.wait();
  i3.wait();
  d1.wait();
  d2.wait();
  ASSERT_EQ(dic.search("coucou").get(), std::make_pair("coucou"s, 0));
  ASSERT_EQ(dic.search("couscous").get(), std::make_pair("coucou"s, 2));
  ASSERT_EQ(dic.search("coucous").get(), std::make_pair("coucou"s, 1));
  ASSERT_EQ(dic.search("massue").get(), std::make_pair("massue"s, 0));
  ASSERT_EQ(dic.search("massive").get(), std::make_pair("massive"s, 0));
  ASSERT_EQ(dic.search("lessive").get(), std::make_pair("massive"s, 2));
  ASSERT_EQ(dic.search("limace").get(), std::make_pair("lamasse"s, 3));
  ASSERT_EQ(dic.search("masseur").get(), std::make_pair("massue"s, 2));
}

TEST(TrieDico, ConcurrentOperations)
{
  using namespace std::placeholders;

  std::vector<std::string> data = load_word_list();
  std::size_t n = 1000;

  AsyncTrie dic(data.begin(), data.begin() + 4 * n);

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

TEST(TrieDico, ConcurrentGlobalOperations)
{
  using namespace std::placeholders;

  std::vector<std::string> data = load_word_list();
  std::size_t n = 1000;

  AsyncGlobalTrie dic(data.begin(), data.begin() + 4 * n);

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

// A long scenario, check that the async dictionary as the
// same output as the blocking one
TEST(TrieDico, Compare)
{
  std::vector<std::string> word_list = load_word_list();
  word_list.resize(10000);

  Scenario scn(word_list, 512);

  Trie dic;
  AsyncTrie async_dic;
  scn.prepare(dic);
  scn.prepare(async_dic);
  auto r1 = scn.execute(async_dic);
  auto r2 = scn.execute(dic);
  ASSERT_EQ(r1, r2);
}

TEST(TrieDico, CompareLock)
{
  std::vector<std::string> word_list = load_word_list();
  word_list.resize(10000);

  Scenario scn(word_list, 512);

  Trie dic;
  AsyncGlobalTrie async_dic;
  scn.prepare(dic);
  scn.prepare(async_dic);
  auto r1 = scn.execute(async_dic);
  auto r2 = scn.execute(dic);
  ASSERT_EQ(r1, r2);
}

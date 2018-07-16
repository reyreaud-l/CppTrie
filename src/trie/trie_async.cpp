#include "trie_async.hpp"

AsyncTrie::AsyncTrie()
{
  root = std::make_unique<AsyncNode>();
}

AsyncNode::AsyncNode()
{
  for (std::size_t i = 0; i < SIZE; i++)
    children_[i] = nullptr;
}

AsyncNode::~AsyncNode()
{
  for (std::size_t i = 0; i < SIZE; i++)
    if (children_[i] != nullptr)
      delete children_[i];
}

void AsyncTrie::init(const std::vector<std::string>& words)
{
  for (const auto& w : words)
    this->insert(w);
}

std::future<result_t> AsyncTrie::search(const std::string& query) const
{
  FUNCTION(__FUNCTION__);
  return std::async(std::launch::async, [&]() -> result_t {
    AsyncNode* curr = root.get();
    if (curr == nullptr)
      return std::make_pair<>("", 0);

    result_t result{"", std::numeric_limits<int>::max()};

    std::string word = "";
    std::size_t row_size = query.size();
    int row[row_size + 1];
    for (std::size_t i = 0; i < row_size; ++i)
      row[i] = i;
    row[row_size] = row_size;

    for (std::size_t i = 0; i < SIZE; ++i)
    {
      AsyncNode* tmp = curr->children_[i].load();
      if (tmp != nullptr && !tmp->is_deleted_)
        this->search_rec(word + std::string(1, i + 'a'), tmp, '\0', i + 'a',
                         nullptr, row, row_size + 1, query, result);
    }
    return result;
  });
}

void AsyncTrie::search_rec(std::string word, AsyncNode* curr,
                           const char pprev_letter, const char prev_letter,
                           int* pprev_row, int* prev_row, std::size_t size,
                           const std::string& query, result_t& result) const
{
  int row[size];
  row[0] = prev_row[0] + 1;

  int insert_cost = std::numeric_limits<int>::max();
  int delete_cost = insert_cost;
  int replace_cost = insert_cost;

  for (std::size_t i = 1; i < size; ++i)
  {
    int cost = 0;
    if (query[i - 1] != prev_letter)
      cost = 1;

    insert_cost = row[i - 1] + 1;
    delete_cost = prev_row[i] + 1;
    replace_cost = prev_row[i - 1] + cost;

    row[i] = std::min({insert_cost, delete_cost, replace_cost});

    // Test substitution
    if (i >= 2 && pprev_row)
      if (query[i - 2] == prev_letter && query[i - 1] == pprev_letter)
        row[i] = std::min(row[i], pprev_row[i - 2] + cost);
  }

  if (row[size - 1] < result.second && curr->leaf_)
    result = {word, row[size - 1]};

  auto min_elt = std::numeric_limits<int>::max();

  for (std::size_t i = 0; i < size; i++)
    if (row[i] < min_elt)
      min_elt = row[i];

  if (min_elt < result.second)
  {
    for (std::size_t i = 0; i < SIZE; ++i)
    {
      AsyncNode* tmp = curr->children_[i];
      if (tmp != nullptr && !tmp->is_deleted_)
        this->search_rec(word + std::string(1, i + 'a'), tmp, prev_letter,
                         i + 'a', prev_row, row, size, query, result);
    }
  }
}

std::future<void> AsyncTrie::insert(const std::string& w)
{
  FUNCTION(__FUNCTION__);
  return std::async(std::launch::async, [&]() -> void {
    AsyncNode* curr = root.get();

    if (curr == nullptr)
      return;

    for (const char c : w)
    {
      auto child = curr->children_[c - 'a'].load();
      if (child == nullptr)
        curr->children_[c - 'a'].compare_exchange_strong(child,
                                                         new AsyncNode());
      else if (child->is_deleted_)
      {
        child->is_deleted_ = false;
        curr->children_[c - 'a'].store(child);
      }
      curr = curr->children_[c - 'a'].load();
    }
    curr->leaf_ = true;
  });
}

int AsyncTrie::have_children(AsyncNode* n)
{
  for (std::size_t i = 0; i < SIZE; ++i)
  {
    AsyncNode* tmp = n->children_[i].load();
    if (tmp != nullptr)
      return 1;
  }
  return 0;
}

int AsyncTrie::erase_rec(AsyncNode* curr, const char* s)
{
  FUNCTION(__FUNCTION__);
  if (curr == nullptr)
    return 0;

  if (*s && !curr->is_deleted_)
  {
    auto child = curr->children_[*s - 'a'].load();
    if (child != nullptr && !child->is_deleted_ &&
        this->erase_rec(curr->children_[*s - 'a'].load(), s + 1) &&
        curr->leaf_ == false)
    {
      if (!have_children(curr))
      {
        curr->is_deleted_ = true;
        return 1;
      }
      else
        return 0;
    }
  }

  if (*s == '\0' && curr->leaf_)
  {
    if (!have_children(curr))
    {
      curr->is_deleted_ = true;
      return 1;
    }
    else
      curr->leaf_ = false;
  }
  return 0;
}

std::future<void> AsyncTrie::erase(const std::string& w)
{
  FUNCTION(__FUNCTION__);
  return std::async(std::launch::async, [&]() -> void {
    AsyncNode* curr = root.get();

    if (curr == nullptr)
      return;

    this->erase_rec(curr, w.c_str());
  });
}

void AsyncTrie::print(std::string prefix)
{
  if (root == nullptr)
    return;
  this->print_rec(prefix, root.get());
}

void AsyncTrie::print_rec(std::string prefix, AsyncNode* curr)
{
  for (std::size_t i = 0; i < SIZE; ++i)
  {
    AsyncNode* tmp = curr->children_[i].load();
    if (tmp != nullptr && !tmp->is_deleted_)
    {
      std::cout << prefix << (char)(i + 'a') << std::endl;
      this->print_rec(prefix + " ", tmp);
    }
  }
}

#include "trie.hpp"

Trie::Trie()
{
  root = std::make_unique<Node>(Node());
}

Node::Node()
{
  for (std::size_t i = 0; i < SIZE; i++)
    children_[i] = nullptr;
}

Node::~Node()
{
  for (std::size_t i = 0; i < SIZE; i++)
    if (children_[i] != nullptr)
      delete children_[i];
}

void Trie::init(const std::vector<std::string>& words)
{
  for (const auto& w : words)
    this->insert(w);
}

result_t Trie::search(const std::string& query) const
{
  Node* curr = root.get();
  result_t result{"", std::numeric_limits<int>::max()};

  if (!curr)
    return result;

  std::size_t row_size = query.size();
  int row[row_size + 1];
  for (std::size_t i = 0; i < row_size; ++i)
    row[i] = i;
  row[row_size] = row_size;

  for (std::size_t i = 0; i < SIZE; ++i)
  {
    Node* tmp = curr->children_[i];
    if (tmp != nullptr && !tmp->is_deleted_)
      this->search_rec(tmp, '\0', i + 'a', nullptr, row, row_size + 1, query,
                       result);
  }
  return result;
}

void Trie::search_rec(Node* curr, const char pprev_letter,
                      const char prev_letter, int* pprev_row, int* prev_row,
                      std::size_t size, const std::string& query,
                      result_t& result) const
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
    result = {curr->value_, row[size - 1]};

  auto min_elt = std::numeric_limits<int>::max();

  for (std::size_t i = 0; i < size; i++)
    if (row[i] < min_elt)
      min_elt = row[i];

  if (min_elt < result.second)
  {
    for (std::size_t i = 0; i < SIZE; ++i)
    {
      Node* tmp = curr->children_[i];
      if (tmp != nullptr && !tmp->is_deleted_)
        this->search_rec(tmp, prev_letter, i + 'a', prev_row, row, size, query,
                         result);
    }
  }
}

void Trie::insert(const std::string& w)
{
  Node* current = root.get();
  if (!current)
    return;

  for (const char c : w)
  {
    auto& child = current->children_[c - 'a'];
    if (child == nullptr)
      child = new Node();
    if (child->is_deleted_)
      child->is_deleted_ = false;
    current = child;
  }
  current->value_ = w;
  current->leaf_ = true;
}

int Trie::have_children(Node* n)
{
  for (std::size_t i = 0; i < SIZE; ++i)
  {
    Node* tmp = n->children_[i];
    if (tmp != nullptr)
      return 1;
  }
  return 0;
}

int Trie::erase_rec(Node** curr, const char* s)
{
  if (curr == nullptr)
    return 0;

  if (*s && *curr != nullptr && !(*curr)->is_deleted_)
  {
    auto child = (*curr)->children_[*s - 'a'];
    if (child != nullptr && !child->is_deleted_ &&
        this->erase_rec(&((*curr)->children_[*s - 'a']), s + 1) &&
        (*curr)->leaf_ == false)
    {
      if (!have_children(*curr))
      {
        (*curr)->is_deleted_ = true;
        return 1;
      }
      else
        return 0;
    }
  }

  if (*s == '\0' && (*curr)->leaf_)
  {
    if (!have_children(*curr))
    {
      (*curr)->is_deleted_ = true;
      return 1;
    }
    else
    {
      (*curr)->leaf_ = false;
      return 0;
    }
  }

  return 0;
}

void Trie::erase(const std::string& w)
{
  Node* input = root.get();
  this->erase_rec(&input, w.c_str());
}

void Trie::print(std::string prefix)
{
  this->print_rec(prefix, root.get());
}

void Trie::print_rec(std::string prefix, Node* curr)
{
  for (std::size_t i = 0; i < SIZE; ++i)
  {
    Node* tmp = curr->children_[i];
    if (tmp != nullptr && !tmp->is_deleted_)
    {
      std::cout << prefix << (char)(i + 'a') << std::endl;
      this->print_rec(prefix + " ", tmp);
    }
  }
}

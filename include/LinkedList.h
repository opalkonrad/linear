#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class LinkedList
{

public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type *;
  using reference = Type &;
  using const_pointer = const Type *;
  using const_reference = const Type &;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private:
  struct Node
  {
    value_type value;
    Node *next, *prev;

    Node(value_type val)
        : value(val) {}

    Node(value_type val, Node *next, Node *prev)
        : value(val), next(next), prev(prev) {}
  };

  size_type size;
  Node *sentinel;

public:
  LinkedList()
      : size(0)
  {
    sentinel = new Node(value_type());
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
  }

  LinkedList(std::initializer_list<Type> l)
      : LinkedList()
  {
    for (auto item : l)
    {
      append(item);
    }
  }

  LinkedList(const LinkedList &other) : LinkedList()
  {
    *this = other;
  }

  LinkedList(LinkedList &&other)
  {
    sentinel = other.sentinel;
    size = other.size;

    other.sentinel = nullptr;
    other.size = 0;
  }

  ~LinkedList()
  {
    if (size)
      erase(begin(), end());
    if (sentinel)
      delete sentinel;
  }

  LinkedList &operator=(const LinkedList &other)
  {
    if (this == &other)
    {
      return *this;
    }

    erase(begin(), end());

    for (auto item : other)
    {
      append(item);
    }

    return *this;
  }

  LinkedList &operator=(LinkedList &&other)
  {
    if (this == &other)
    {
      return *this;
    }

    Node *tmpSentinel = sentinel;
    sentinel = other.sentinel;
    other.sentinel = tmpSentinel;

    size_type tmpSize = size;
    size = other.size;
    other.size = tmpSize;

    other.erase(other.begin(), other.end());

    return *this;
  }

  bool isEmpty() const
  {
    return !size;
  }

  size_type getSize() const
  {
    return size;
  }

  void append(const Type &item)
  {
    insert(cend(), item);
  }

  void prepend(const Type &item)
  {
    insert(cbegin(), item);
  }

  void insert(const const_iterator &insertPosition, const Type &item)
  {
    Node *after = insertPosition.actualNode;
    Node *before = after->prev;

    Node *newNode = new Node(item, after, before);

    before->next = newNode;
    after->prev = newNode;

    ++size;
  }

  Type popFirst()
  {
    if (isEmpty())
    {
      throw std::out_of_range("Popping first element from empty list!");
    }

    Node *toPop = sentinel->next;
    value_type tmp = toPop->value;

    erase(const_iterator(sentinel, toPop));

    return tmp;
  }

  Type popLast()
  {
    if (isEmpty())
    {
      throw std::out_of_range("Popping last element from empty list!");
    }

    Node *toPop = sentinel->prev;
    value_type tmp = toPop->value;

    erase(const_iterator(sentinel, toPop));

    return tmp;
  }

  void erase(const const_iterator &possition)
  {
    if (possition == end())
    {
      throw std::out_of_range("Erasing end!");
    }

    Node *before = possition.actualNode->prev;
    Node *after = possition.actualNode->next;

    before->next = after;
    after->prev = before;

    Node *tmp = possition.actualNode;
    delete tmp;

    --size;
  }

  void erase(const const_iterator &firstIncluded, const const_iterator &lastExcluded)
  {
    Node *before = firstIncluded.actualNode->prev;
    Node *after = lastExcluded.actualNode;

    before->next = after;
    after->prev = before;

    for (auto it = firstIncluded; it != lastExcluded; ++it)
    {
      delete it.actualNode;
      --size;
    }
  }

  iterator begin()
  {
    if (isEmpty())
    {
      return end();
    }
    return iterator(cbegin());
  }

  iterator end()
  {
    return iterator(cend());
  }

  const_iterator cbegin() const
  {
    if (isEmpty())
    {
      return cend();
    }
    return const_iterator(sentinel, sentinel->next);
  }

  const_iterator cend() const
  {
    return const_iterator(sentinel, sentinel);
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename Type>
class LinkedList<Type>::ConstIterator
{

public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;

private:
  friend void LinkedList::erase(const const_iterator &firstIncluded, const const_iterator &lastExcluded);
  friend void LinkedList::insert(const const_iterator &insertPosition, const Type &item);
  friend void LinkedList::erase(const const_iterator &possition);

  Node *sentinel, *actualNode;

public:
  explicit ConstIterator(Node *sentinel, Node *actualNode) : sentinel(sentinel), actualNode(actualNode) {}

  reference operator*() const
  {
    if (actualNode == sentinel)
    {
      throw std::out_of_range("Dereferencing end!");
    }

    return actualNode->value;
  }

  ConstIterator &operator++()
  {
    if (actualNode == sentinel)
    {
      throw std::out_of_range("Incrementing end!");
    }

    actualNode = actualNode->next;
    return *this;
  }

  ConstIterator operator++(int)
  {
    ConstIterator tmp = *this;
    ++*this;
    return tmp;
  }

  ConstIterator &operator--()
  {
    if (actualNode == sentinel->next)
    {
      throw std::out_of_range("Decrementing begin!");
    }

    actualNode = actualNode->prev;
    return *this;
  }

  ConstIterator operator--(int)
  {
    ConstIterator tmp = *this;
    --*this;
    return tmp;
  }

  ConstIterator operator+(difference_type d) const
  {
    ConstIterator tmp = *this;
    for (int i = 0; i < d; ++i, ++tmp)
      ;
    return tmp;
  }

  ConstIterator operator-(difference_type d) const
  {
    ConstIterator tmp = *this;
    for (int i = 0; i < d; ++i, --tmp)
      ;
    return tmp;
  }

  bool operator==(const ConstIterator &other) const
  {
    return actualNode == other.actualNode;
  }

  bool operator!=(const ConstIterator &other) const
  {
    return actualNode != other.actualNode;
  }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;

  explicit Iterator()
  {
  }

  Iterator(const ConstIterator &other)
      : ConstIterator(other)
  {
  }

  Iterator &operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator &operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  Iterator operator+(difference_type d) const
  {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const
  {
    return ConstIterator::operator-(d);
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

} // namespace aisdi

#endif // AISDI_LINEAR_LINKEDLIST_H
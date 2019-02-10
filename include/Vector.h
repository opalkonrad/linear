#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

#define CAPACITY 4
#define CAPACITYPLUS 1000

namespace aisdi
{

template <typename Type>
class Vector
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
  size_type dataSize;
  size_type buffSize;
  pointer buff;

  void reallocate()
  {
    buffSize += CAPACITYPLUS;
    pointer newBuff = new value_type[buffSize];

    for (size_type i = 0; i < dataSize; i++)
    {
      newBuff[i] = buff[i];
    }

    delete[] buff;
    buff = newBuff;
  }

public:
  Vector() : dataSize(0), buffSize(CAPACITY)
  {
    buff = new value_type[buffSize];
  }

  Vector(std::initializer_list<Type> l) : Vector()
  {
    for (auto item : l)
    {
      append(item);
    }
  }

  Vector(const Vector &other) : dataSize(other.dataSize), buffSize(other.buffSize)
  {
    buff = new value_type[buffSize];

    for (size_type i = 0; i < dataSize; i++)
    {
      buff[i] = other.buff[i];
    }
  }

  Vector(Vector &&other) : dataSize(other.dataSize), buffSize(other.buffSize), buff(other.buff)
  {
    other.buff = nullptr;
  }

  ~Vector()
  {
    delete[] buff;
  }

  Vector &operator=(const Vector &other)
  {
    if (this == &other)
    {
      return *this;
    }

    dataSize = other.dataSize;
    buffSize = other.buffSize;
    delete[] buff;
    buff = new value_type[buffSize];

    for (size_type i = 0; i < dataSize; i++)
    {
      buff[i] = other.buff[i];
    }

    return *this;
  }

  Vector &operator=(Vector &&other)
  {
    if (this == &other)
    {
      return *this;
    }

    dataSize = other.dataSize;
    buffSize = other.buffSize;

    delete[] buff;

    buff = other.buff;
    other.buff = nullptr;

    return *this;
  }

  bool isEmpty() const
  {
    return !dataSize;
  }

  size_type getSize() const
  {
    return dataSize;
  }

  void append(const Type &item)
  {
    if (dataSize == buffSize)
    {
      reallocate();
    }

    buff[dataSize++] = item;
  }

  void prepend(const Type &item)
  {
    if (dataSize == buffSize)
    {
      reallocate();
    }

    for (size_type i = dataSize; i > 0; i--)
    {
      buff[i] = buff[i - 1];
    }

    buff[0] = item;
    ++dataSize;
  }

  void insert(const const_iterator &insertPosition, const Type &item)
  {
    if (insertPosition == begin())
    {
      prepend(item);
      return;
    }

    if (insertPosition == end())
    {
      append(item);
      return;
    }

    if (dataSize == buffSize)
    {
      reallocate();
    }

    ++dataSize;

    for (auto i = end() - 1; i != insertPosition; i--)
    {
      *(i) = *(i - 1);
    }

    *iterator(insertPosition) = item;
  }

  Type popFirst()
  {
    if (isEmpty())
    {
      throw std::out_of_range("Out of range!");
    }

    value_type tmp = buff[0];

    for (size_type i = 0; i < dataSize - 1; i++)
    {
      buff[i] = buff[i + 1];
    }

    --dataSize;
    return tmp;
  }

  Type popLast()
  {
    if (isEmpty())
    {
      throw std::out_of_range("Out of range!");
    }

    return buff[--dataSize];
  }

  void erase(const const_iterator &possition)
  {
    if (possition == end())
    {
      throw std::out_of_range("Out of range!");
    }

    for (auto tmp = iterator(possition); tmp != end() - 1; tmp++)
    {
      *tmp = *(tmp + 1);
    }

    --dataSize;
  }

  void erase(const const_iterator &firstIncluded, const const_iterator &lastExcluded)
  {
    if (firstIncluded == begin() && lastExcluded == end())
    {
      dataSize = 0;
    }
    else if (firstIncluded == lastExcluded)
    {
      return;
    }
    else
    {
      auto first = iterator(firstIncluded);
      size_type counter = 0;

      for (auto last = iterator(lastExcluded); last != end(); last++, first++)
      {
        *first = *last;
      }

      for (auto it = firstIncluded; it != lastExcluded; it++)
      {
        counter++;
      }

      dataSize -= counter;
    }
  }

  iterator begin()
  {
    return iterator(cbegin());
  }

  iterator end()
  {
    return iterator(cend());
  }

  const_iterator cbegin() const
  {
    return ConstIterator(&buff[0], this);
  }

  const_iterator cend() const
  {
    return ConstIterator(&buff[dataSize], this);
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
class Vector<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Vector::value_type;
  using difference_type = typename Vector::difference_type;
  using pointer = typename Vector::const_pointer;
  using reference = typename Vector::const_reference;

private:
  pointer elemPtr;
  const Vector<value_type> *vectPtr;

public:
  explicit ConstIterator(pointer elem, const Vector<value_type> *vect) : elemPtr(elem), vectPtr(vect) {}

  reference operator*() const
  {
    if (*this == vectPtr->end())
    {
      throw std::out_of_range("Out of range!");
    }

    return *elemPtr;
  }

  ConstIterator &operator++()
  {
    if (*this == vectPtr->end())
    {
      throw std::out_of_range("Out of range!");
    }

    ++elemPtr;
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
    if (*this == vectPtr->begin())
    {
      throw std::out_of_range("Out of range!");
    }

    --elemPtr;
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
    return elemPtr == other.elemPtr;
  }

  bool operator!=(const ConstIterator &other) const
  {
    return elemPtr != other.elemPtr;
  }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
  using pointer = typename Vector::pointer;
  using reference = typename Vector::reference;

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

#endif // AISDI_LINEAR_VECTOR_H
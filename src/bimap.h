#pragma once

#include <cstddef>

template <typename Left, typename Right, typename CompareLeft, typename CompareRight>
class bimap {
public:
  using left_t = void;
  using right_t = void;

  using node_t = void;

  class left_iterator;
  class right_iterator;

  class left_iterator {
  public:
    const left_t& operator*() const;
    const left_t* operator->() const;

    left_iterator& operator++();
    left_iterator operator++(int);

    left_iterator& operator--();
    left_iterator operator--(int);

    right_iterator flip() const;
  };

public:
  bimap(CompareLeft compare_left = CompareLeft(), CompareRight compare_right = CompareRight());

  bimap(const bimap& other);
  bimap(bimap&& other) noexcept;

  bimap& operator=(const bimap& other);
  bimap& operator=(bimap&& other) noexcept;

  ~bimap();

  friend void swap(bimap& lhs, bimap& rhs);

  left_iterator insert(const left_t& left, const right_t& right);
  left_iterator insert(const left_t& left, right_t&& right);
  left_iterator insert(left_t&& left, const right_t& right);
  left_iterator insert(left_t&& left, right_t&& right);

  left_iterator erase_left(left_iterator it);
  right_iterator erase_right(right_iterator it);

  bool erase_left(const left_t& left);
  bool erase_right(const right_t& right);

  left_iterator erase_left(left_iterator first, left_iterator last);
  right_iterator erase_right(right_iterator first, right_iterator last);

  left_iterator find_left(const left_t& left) const;
  right_iterator find_right(const right_t& right) const;

  const right_t& at_left(const left_t& key) const;
  const left_t& at_right(const right_t& key) const;

  const right_t& at_left_or_default(const left_t& key);
  const left_t& at_right_or_default(const right_t& key);

  left_iterator lower_bound_left(const left_t& left) const;
  left_iterator upper_bound_left(const left_t& left) const;

  right_iterator lower_bound_right(const right_t& right) const;
  right_iterator upper_bound_right(const right_t& right) const;

  left_iterator begin_left() const;
  left_iterator end_left() const;

  right_iterator begin_right() const;
  right_iterator end_right() const;

  bool empty() const;

  std::size_t size() const;

  friend bool operator==(const bimap& lhs, const bimap& rhs);
  friend bool operator!=(const bimap& lhs, const bimap& rhs);
};

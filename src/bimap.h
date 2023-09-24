#pragma once

#include <cstddef>

template <typename Left, typename Right, typename CompareLeft, typename CompareRight>
struct bimap {
  using left_t = void;
  using right_t = void;

  using node_t = void;

  struct right_iterator; // По аналогии с left_iterator

  struct left_iterator {
    // Элемент на который сейчас ссылается итератор.
    // Разыменование итератора end_left() неопределено.
    // Разыменование невалидного итератора неопределено.
    const left_t& operator*() const;
    const left_t* operator->() const;

    // Переход к следующему по величине left'у.
    // Инкремент итератора end_left() неопределен.
    // Инкремент невалидного итератора неопределен.
    left_iterator& operator++();
    left_iterator operator++(int);

    // Переход к предыдущему по величине left'у.
    // Декремент итератора begin_left() неопределен.
    // Декремент невалидного итератора неопределен.
    left_iterator& operator--();
    left_iterator operator--(int);

    // left_iterator ссылается на левый элемент некоторой пары.
    // Эта функция возвращает итератор на правый элемент той же пары.
    // end_left().flip() возращает end_right().
    // end_right().flip() возвращает end_left().
    // flip() невалидного итератора неопределен.
    right_iterator flip() const;
  };

  // Создает bimap не содержащий ни одной пары.
  bimap(CompareLeft compare_left = CompareLeft(), CompareRight compare_right = CompareRight());

  // Конструкторы от других и присваивания
  bimap(const bimap& other);
  bimap(bimap&& other) noexcept;

  bimap& operator=(const bimap& other);
  bimap& operator=(bimap&& other) noexcept;

  // Деструктор. Вызывается при удалении объектов bimap.
  // Инвалидирует все итераторы ссылающиеся на элементы этого bimap
  // (включая итераторы ссылающиеся на элементы следующие за последними).
  ~bimap();

  // Вставка пары (left, right), возвращает итератор на left.
  // Если такой left или такой right уже присутствуют в bimap, вставка не
  // производится и возвращается end_left().
  left_iterator insert(const left_t& left, const right_t& right);
  left_iterator insert(const left_t& left, right_t&& right);
  left_iterator insert(left_t&& left, const right_t& right);
  left_iterator insert(left_t&& left, right_t&& right);

  // Удаляет элемент и соответствующий ему парный.
  // erase невалидного итератора неопределен.
  // erase(end_left()) и erase(end_right()) неопределены.
  // Пусть it ссылается на некоторый элемент e.
  // erase инвалидирует все итераторы ссылающиеся на e и на элемент парный к e.
  left_iterator erase_left(left_iterator it);
  // Аналогично erase, но по ключу, удаляет элемент если он присутствует, иначе
  // не делает ничего Возвращает была ли пара удалена
  bool erase_left(const left_t& left);

  right_iterator erase_right(right_iterator it);
  bool erase_right(const right_t& right);

  // erase от ренжа, удаляет [first, last), возвращает итератор на последний
  // элемент за удаленной последовательностью
  left_iterator erase_left(left_iterator first, left_iterator last);
  right_iterator erase_right(right_iterator first, right_iterator last);

  // Возвращает итератор по элементу. Если не найден - соответствующий end()
  left_iterator find_left(const left_t& left) const;
  right_iterator find_right(const right_t& right) const;

  // Возвращает противоположный элемент по элементу
  // Если элемента не существует -- бросает std::out_of_range
  const right_t& at_left(const left_t& key) const;
  const left_t& at_right(const right_t& key) const;

  // Возвращает противоположный элемент по элементу
  // Если элемента не существует, добавляет его в bimap и на противоположную
  // сторону кладет дефолтный элемент, ссылку на который и возвращает
  // Если дефолтный элемент уже лежит в противоположной паре - должен поменять
  // соответствующий ему элемент на запрашиваемый (смотри тесты)
  const right_t& at_left_or_default(const left_t& key);
  const left_t& at_right_or_default(const right_t& key);

  // lower и upper bound'ы по каждой стороне
  // Возвращают итераторы на соответствующие элементы
  // Смотри std::lower_bound, std::upper_bound.
  left_iterator lower_bound_left(const left_t& left) const;
  left_iterator upper_bound_left(const left_t& left) const;

  right_iterator lower_bound_right(const right_t& left) const;
  right_iterator upper_bound_right(const right_t& left) const;

  // Возващает итератор на минимальный по порядку left.
  left_iterator begin_left() const;
  // Возващает итератор на следующий за последним по порядку left.
  left_iterator end_left() const;

  // Возващает итератор на минимальный по порядку right.
  right_iterator begin_right() const;
  // Возващает итератор на следующий за последним по порядку right.
  right_iterator end_right() const;

  // Проверка на пустоту
  bool empty() const;

  // Возвращает размер бимапы (кол-во пар)
  std::size_t size() const;

  // операторы сравнения
  friend bool operator==(const bimap& a, const bimap& b);
  friend bool operator!=(const bimap& a, const bimap& b);
};

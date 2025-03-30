#include "bimap.h"
#include "fault-injection.h"
#include "test-classes.h"
#include <catch2/catch_test_macros.hpp>



namespace {
struct move_counter {
  bool should_count = true;
  int value{0};

  move_counter(bool should_count) : should_count(should_count) {}

  void inc() {
    if (should_count) ++value;
  }
};

struct element {
  move_counter* counter;
  int data;

  element(int data, move_counter* ptr)
  : counter(ptr)
  , data(data) {}

  element(element&& other)
  : data(other.data) {
    other.counter->inc();
  }

  element& operator=(element&& other) noexcept {
    other.counter->inc();
    data = other.data;
    return *this;
  }

  bool operator<(const element& rhs) const {
    return data < rhs.data;
  }
};

struct throwable_comparator {
  bool is_throwable = true;
  throwable_comparator() = delete;
  throwable_comparator(bool should_throw)
  :  is_throwable(should_throw) {}

  bool operator()(const element& lhs, const element& rhs) const {
    if (is_throwable) {
      throw std::runtime_error("throwable comparison");
    }
    return lhs.data < rhs.data;
  }
};
}

using map_type = bimap<element, element>;

using map_throw_comp = bimap<element, element, throwable_comparator, throwable_comparator>;

TEST_CASE("some test test") {
  move_counter counter(true);
  map_type mp;
  mp.insert(element(1, &counter), element(2, &counter));
  REQUIRE(counter.value == 2);
}

TEST_CASE("insert with move semantics provides strong exception guarantee") {
  move_counter counter(false);
  throwable_comparator cmp(true);
  map_throw_comp mp(cmp, cmp);

  mp.insert(element(1, &counter), element(2, &counter));

  counter.should_count = true;
  REQUIRE_THROWS(mp.insert(element(1, &counter), element(2, &counter)));
  REQUIRE(counter.value == 0);
}
#include "bimap.h"
#include "fault-injection.h"
#include "test-classes.h"

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <random>

namespace {

class element {
public:
  element(int data)
      : value(data) {
    fault_injection_point();
  }

  element(const element& other)
      : value(-1) {
    fault_injection_point();
    value = other.value;
  }

  element(element&& other) noexcept
      : value(std::exchange(other.value, -1)) {}

  element& operator=(const element& other) {
    fault_injection_point();
    value = other.value;
    return *this;
  }

  element& operator=(element&& other) noexcept {
    value = std::exchange(other.value, -1);
    return *this;
  }

  ~element() = default;

  friend bool operator==(const element& lhs, const element& rhs) {
    fault_injection_point();
    return lhs.value == rhs.value;
  }

  friend auto operator<=>(const element& lhs, const element& rhs) {
    fault_injection_point();
    return lhs.value <=> rhs.value;
  }

  friend std::ostream& operator<<(std::ostream& out, const element& e) {
    return out << e.value;
  }

private:
  int value;
};

} // namespace

TEST_CASE("Default constructor does not throw") {
  assert_nothrow([] { [[maybe_unused]] bimap<element, element> b; });
}

TEST_CASE("Insert is exception-safe") {
  faulty_run([] {
    bimap<element, element> a;
    a.insert(1, 2);
    a.insert(3, 4);
    a.insert(5, 6);
    a.insert(7, 8);
    a.insert(9, 10);
  });
}

TEST_CASE("Copy constructor is exception-safe") {
  faulty_run([] {
    bimap<element, element> a;
    {
      fault_injection_disable dg;
      a.insert(1, 2);
      a.insert(3, 4);
      a.insert(5, 6);
      a.insert(7, 8);
      a.insert(9, 10);
    }

    bimap<element, element> b = a;
  });
}

TEST_CASE("Copy assignment is exception-safe") {
  faulty_run([] {
    bimap<element, element> a;
    {
      fault_injection_disable dg;
      a.insert(1, 2);
      a.insert(3, 4);
      a.insert(5, 6);
      a.insert(7, 8);
      a.insert(9, 10);
    }

    bimap<element, element> b;
    b = a;
  });
}

TEST_CASE("Erase is exception-safe") {
  faulty_run([] {
    bimap<element, element> a;
    {
      fault_injection_disable dg;
      a.insert(1, 2);
      a.insert(3, 4);
      a.insert(5, 6);
      a.insert(7, 8);
      a.insert(9, 10);
    }

    a.erase_left(3);
    a.erase_right(a.find_right(8));
    a.erase_left(a.begin_left(), a.end_left());
  });
}

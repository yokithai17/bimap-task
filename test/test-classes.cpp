#include "test-classes.h"

#include <catch2/catch_test_macros.hpp>

#include <stdexcept>

std::unordered_set<const address_checking_object*> address_checking_object::addresses;

void address_checking_object::add_instance() const {
  auto [it, was_inserted] = addresses.insert(this);
  if (!was_inserted) {
    // clang-format off
    FAIL(
        "New object is created at the address " << static_cast<const void*>(this)
        << " while the previous object at this address was not destroyed"
    );
    // clang-format on
  }
}

void address_checking_object::remove_instance(bool nothrow) const {
  size_t erased_count = addresses.erase(this);
  if (!nothrow && erased_count != 1) {
    FAIL("Destroying non-existing object at the address " << static_cast<const void*>(this));
  }
}

void address_checking_object::assert_exists() const {
  if (!addresses.contains(this)) {
    FAIL("Accessing an non-existing object at address " << static_cast<const void*>(this));
  }
}

void address_checking_object::expect_no_instances() {
  if (!addresses.empty()) {
    addresses.clear();
    FAIL("Not all instances are destroyed");
  }
}

size_t address_checking_object::copy_throw_countdown = 0;

void address_checking_object::process_copying() {
  if (copy_throw_countdown != 0) {
    if (--copy_throw_countdown == 0) {
      throw std::runtime_error("address_checking_object copying failed");
    }
  }
}

void address_checking_object::set_copy_throw_countdown(size_t new_countdown) {
  copy_throw_countdown = new_countdown;
}

address_checking_object::operator int() const {
  assert_exists();
  return value;
}

address_checking_object::address_checking_object() {
  add_instance();
}

address_checking_object::address_checking_object(int value)
    : value(value) {
  add_instance();
}

address_checking_object::address_checking_object(const address_checking_object& other)
    : value(other.value) {
  process_copying();
  add_instance();
}

address_checking_object& address_checking_object::operator=(const address_checking_object& other) {
  assert_exists();
  other.assert_exists();
  process_copying();
  value = other.value;
  return *this;
}

address_checking_object::~address_checking_object() noexcept(false) {
  remove_instance(std::uncaught_exceptions() > 0);
}

void counter_moved::inc_count() noexcept {
  if (counter != nullptr && enable_counter) {
    ++(*counter);
  }
}

void counter_moved::enable_count() noexcept {
  enable_counter = true;
}

void counter_moved::disable_count() noexcept {
  enable_counter = false;
}

bool counter_moved::valid_data() const noexcept {
  return data != MOVED_DATA;
}

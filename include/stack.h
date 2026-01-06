#pragma once

#include <vector>
#include <stdexcept>

template <typename T>
class Stack {
 public:
  void push(const T& item) { items_.push_back(item); }
  bool empty() const { return items_.empty(); }
  size_t size() const { return items_.size(); }
  T pop() {
    if (items_.empty()) throw std::runtime_error("Stack is empty");
    T v = items_.back();
    items_.pop_back();
    return v;
  }
  const T& peek() const {
    if (items_.empty()) throw std::runtime_error("Stack is empty");
    return items_.back();
  }
  std::vector<T> toVector() const { return items_; }
  void clear() { items_.clear(); }

 private:
  std::vector<T> items_;
};

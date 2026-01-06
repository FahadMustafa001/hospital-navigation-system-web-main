#pragma once

#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm>

class VisitedSet {
 public:
  void insert(const std::string& locationName);
  void clear();
  bool empty() const;
  std::vector<std::string> toSortedVector() const;

 private:
  std::unordered_set<std::string> visited_;
};

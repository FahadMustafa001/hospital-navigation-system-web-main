#include "visited.h"

void VisitedSet::insert(const std::string& locationName) {
  if (locationName.empty()) return;
  visited_.insert(locationName);
}

void VisitedSet::clear() { visited_.clear(); }

bool VisitedSet::empty() const { return visited_.empty(); }

std::vector<std::string> VisitedSet::toSortedVector() const {
  std::vector<std::string> v(visited_.begin(), visited_.end());
  std::sort(v.begin(), v.end());
  return v;
}

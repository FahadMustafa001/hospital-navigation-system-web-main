#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <utility>

namespace Json {
std::string escapeString(const std::string& s);
std::string quote(const std::string& s);
std::string makeObject(const std::vector<std::pair<std::string, std::string>>& fields);
std::string makeArray(const std::vector<std::string>& elements);
std::string number(double n);
}  // namespace Json

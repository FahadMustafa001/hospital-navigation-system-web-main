#include "json.h"

namespace Json {
std::string escapeString(const std::string& s) {
  std::string out;
  for (size_t i = 0; i < s.size(); ++i) {
    char c = s[i];
    if (c == '"' || c == '\\') out.push_back('\\');
    out.push_back(c);
  }
  return out;
}
std::string quote(const std::string& s) { return "\"" + escapeString(s) + "\""; }
std::string makeObject(const std::vector<std::pair<std::string, std::string>>& fields) {
  std::ostringstream oss;
  oss << "{";
  for (size_t i = 0; i < fields.size(); ++i) {
    if (i > 0) oss << ",";
    oss << quote(fields[i].first) << ":" << fields[i].second;
  }
  oss << "}";
  return oss.str();
}
std::string makeArray(const std::vector<std::string>& elements) {
  std::ostringstream oss;
  oss << "[";
  for (size_t i = 0; i < elements.size(); ++i) {
    if (i > 0) oss << ",";
    oss << elements[i];
  }
  oss << "]";
  return oss.str();
}
std::string number(double n) {
  std::ostringstream oss;
  oss << n;
  return oss.str();
}
}  // namespace Json

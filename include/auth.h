#pragma once

#include <string>
#include <unordered_map>

class AuthService {
 public:
  AuthService();
  bool login(const std::string& username, const std::string& password) const;

 private:
  std::unordered_map<std::string, std::string> users_;
};

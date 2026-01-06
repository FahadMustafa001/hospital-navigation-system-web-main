#include "auth.h"

AuthService::AuthService() {
  users_["Fahad Mustafa"] = "12341234";
  users_["muz"] = "muzpass";
  users_["ali"] = "alipass";
  users_["safi"] = "safipass";
}

bool AuthService::login(const std::string& username, const std::string& password) const {
  std::unordered_map<std::string, std::string>::const_iterator it = users_.find(username);
  return it != users_.end() && it->second == password;
}

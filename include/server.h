#pragma once

#include <functional>
#include <map>
#include <string>
#include <memory>

namespace httplib {
class Request {
 public:
  std::string method;
  std::string path;
  std::string body;
  std::map<std::string, std::string> headers;
};
class Response {
 public:
  void set_content(const std::string& content, const std::string& content_type);
  void set_header(const std::string& key, const std::string& value);
  std::string body_;
  std::map<std::string, std::string> headers_;
};
using Handler = std::function<void(const Request&, Response&)>;
class Server {
 public:
  Server();
  ~Server();
  void Get(const std::string& pattern, Handler handler);
  void Post(const std::string& pattern, Handler handler);
  void Options(const std::string& pattern, Handler handler);
  bool listen(const std::string& host, int port);
  void stop();

 private:
  struct Impl;
  std::unique_ptr<Impl> pImpl;
};
}  // namespace httplib

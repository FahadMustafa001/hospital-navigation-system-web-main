#include "server.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using SOCKET = int;
const int INVALID_SOCKET = -1;
const int SOCKET_ERROR = -1;
#define closesocket close
#endif

namespace httplib {
struct Server::Impl {
  std::map<std::string, std::map<std::string, Handler>> handlers_;
  bool initialized;
  Impl() : initialized(false) {
#ifdef _WIN32
    WSADATA wsaData;
    initialized = (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0);
#else
    initialized = true;
#endif
  }
  ~Impl() {
    if (initialized) {
#ifdef _WIN32
      WSACleanup();
#endif
    }
  }
};

Server::Server() : pImpl(new Impl) {}
Server::~Server() = default;

void Response::set_content(const std::string& content, const std::string& content_type) {
  body_ = content;
  headers_["Content-Type"] = content_type;
}
void Response::set_header(const std::string& key, const std::string& value) {
  headers_[key] = value;
}

void Server::Get(const std::string& pattern, Handler handler) {
  pImpl->handlers_["GET"][pattern] = handler;
}
void Server::Post(const std::string& pattern, Handler handler) {
  pImpl->handlers_["POST"][pattern] = handler;
}
void Server::Options(const std::string& pattern, Handler handler) {
  pImpl->handlers_["OPTIONS"][pattern] = handler;
}
void Server::stop() {}

bool Server::listen(const std::string& host, int port) {
  SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (listen_sock == INVALID_SOCKET) return false;
  int opt = 1;
  setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;
  if (bind(listen_sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
    closesocket(listen_sock);
    return false;
  }
  if (::listen(listen_sock, SOMAXCONN) == SOCKET_ERROR) {
    closesocket(listen_sock);
    return false;
  }
  std::cout << "Server listening on http://" << host << ":" << port << "\n";
  while (true) {
    SOCKET client_sock = accept(listen_sock, nullptr, nullptr);
    if (client_sock == INVALID_SOCKET) continue;
    char buffer[4096];
    int bytes = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes > 0) {
      buffer[bytes] = '\0';
      std::string raw(buffer, buffer + bytes);

      // Ensure we have full headers
      size_t header_end = raw.find("\r\n\r\n");
      while (header_end == std::string::npos) {
        int more = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
        if (more <= 0) break;
        buffer[more] = '\0';
        raw.append(buffer, buffer + more);
        header_end = raw.find("\r\n\r\n");
      }
      if (header_end == std::string::npos) {
        closesocket(client_sock);
        continue;
      }

      std::string header_part = raw.substr(0, header_end);
      std::string body_part = raw.substr(header_end + 4);

      std::istringstream iss(header_part);
      std::string line;
      std::getline(iss, line);
      std::istringstream req_line(line);
      std::string method, path, version;
      req_line >> method >> path >> version;

      Request req;
      req.method = method;
      req.path = path;

      int content_length = 0;
      while (std::getline(iss, line)) {
        size_t pos = line.find(':');
        if (pos != std::string::npos) {
          std::string key = line.substr(0, pos);
          std::string value;
          if (pos + 1 < line.size() && line[pos + 1] == ' ') {
            value = line.substr(pos + 2);
          } else {
            value = line.substr(pos + 1);
          }
          while (!value.empty() && (value.back() == '\r' || value.back() == '\n')) value.pop_back();
          req.headers[key] = value;
          if (key == "Content-Length") {
            try {
              content_length = std::stoi(value);
            } catch (...) {
              content_length = 0;
            }
          }
        }
      }

      if (content_length > 0) {
        while (static_cast<int>(body_part.size()) < content_length) {
          int more = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
          if (more <= 0) break;
          buffer[more] = '\0';
          body_part.append(buffer, buffer + more);
        }
        if (static_cast<int>(body_part.size()) > content_length) {
          body_part.resize(static_cast<size_t>(content_length));
        }
      }

      while (!body_part.empty() && (body_part.back() == '\r' || body_part.back() == '\n')) body_part.pop_back();
      req.body = body_part;

      Response res;
      auto& method_map = pImpl->handlers_[method];
      bool handled = false;
      for (const auto& kv : method_map) {
        if (kv.first == path) {
          kv.second(req, res);
          handled = true;
          break;
        }
      }
      if (!handled) {
        res.set_content("Not Found", "text/plain");
        res.set_header("Status", "404");
      }
      std::ostringstream oss;
      oss << "HTTP/1.1 200 OK\r\n";
      for (const auto& h : res.headers_) {
        oss << h.first << ": " << h.second << "\r\n";
      }
      oss << "Content-Length: " << res.body_.size() << "\r\n";
      oss << "\r\n" << res.body_;
      std::string resp_str = oss.str();
      send(client_sock, resp_str.c_str(), static_cast<int>(resp_str.size()), 0);
    }
    closesocket(client_sock);
  }
  closesocket(listen_sock);
  return true;
}
}  // namespace httplib

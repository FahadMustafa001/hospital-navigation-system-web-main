#include "include/auth.h"
#include "include/dijkstra.h"
#include "include/geo.h"
#include "include/data.h"
#include "include/stack.h"
#include "include/visited.h"
#include "include/json.h"
#include "include/server.h"

#include <iostream>
#include <sstream>
#include <vector>

int main()
{
  AuthService auth;
  std::vector<Point> points = getDefaultHospitalPoints();
  std::vector<std::vector<std::pair<int, double>>> graph;
  for (int i = 0; i < static_cast<int>(points.size()); ++i)
  {
    std::vector<std::pair<int, double>> neighbors;
    for (int j = 0; j < static_cast<int>(points.size()); ++j)
    {
      if (i == j)
        continue;
      double d = distanceMeters(points[i].lat, points[i].lon, points[j].lat, points[j].lon);
      neighbors.emplace_back(j, d);
    }
    graph.push_back(neighbors);
  }
  static Stack<std::string> history;
  static VisitedSet visited;
  httplib::Server svr;
  svr.Options(".*", [](const httplib::Request &, httplib::Response &res)
              {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type"); });
  svr.Post("/api/login", [&](const httplib::Request &req, httplib::Response &res)
           {
    res.set_header("Access-Control-Allow-Origin", "*");
    std::map<std::string, std::string> kv;
    std::istringstream iss(req.body);
    std::string line;
    while (std::getline(iss, line)) {
      size_t pos = line.find(':');
      if (pos != std::string::npos) {
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        if (!key.empty() && key.front() == '"') key.erase(key.begin());
        if (!key.empty() && key.back() == '"') key.pop_back();
        if (!value.empty() && value.front() == '"') value.erase(value.begin());
        if (!value.empty() && value.back() == '"') value.pop_back();
        while (!value.empty() && (value.back() == '\r' || value.back() == '\n')) value.pop_back();
        kv[key] = value;
      }
    }
    std::string user = kv["user"];
    std::string pass = kv["pass"];
    bool ok = auth.login(user, pass);
    std::string json = Json::makeObject({{"success", ok ? "true" : "false"}});
    res.set_content(json, "application/json"); });
  svr.Get("/api/points", [&](const httplib::Request &, httplib::Response &res)
          {
    res.set_header("Access-Control-Allow-Origin", "*");
    std::vector<std::string> arr;
    for (size_t i = 0; i < points.size(); ++i) {
      std::string ptJson = Json::makeObject({
          {"id", Json::number(static_cast<double>(i))},
          {"name", Json::quote(points[i].name)},
          {"lat", Json::number(points[i].lat)},
          {"lon", Json::number(points[i].lon)}
      });
      arr.push_back(ptJson);
    }
    std::string json = Json::makeArray(arr);
    res.set_content(json, "application/json"); });
  svr.Post("/api/route", [&](const httplib::Request &req, httplib::Response &res)
           {
    res.set_header("Access-Control-Allow-Origin", "*");
    std::map<std::string, std::string> kv;
    std::istringstream iss(req.body);
    std::string line;
    while (std::getline(iss, line)) {
      size_t pos = line.find(':');
      if (pos != std::string::npos) {
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        if (!key.empty() && key.front() == '"') key.erase(key.begin());
        if (!key.empty() && key.back() == '"') key.pop_back();
        if (!value.empty() && value.front() == '"') value.erase(value.begin());
        if (!value.empty() && value.back() == '"') value.pop_back();
        while (!value.empty() && (value.back() == '\r' || value.back() == '\n')) value.pop_back();
        kv[key] = value;
      }
    }
    int start = -1, end = -1;
    try {
      start = static_cast<int>(std::stod(kv["start"]));
      end = static_cast<int>(std::stod(kv["end"]));
    } catch (...) {
      std::string err = Json::makeObject({{"error", "Invalid start/end indices"}});
      res.set_content(err, "application/json");
      return;
    }
    if (start < 0 || end < 0 || start >= static_cast<int>(points.size()) || end >= static_cast<int>(points.size())) {
      std::string err = Json::makeObject({{"error", "Indices out of range"}});
      res.set_content(err, "application/json");
      return;
    }
    DijkstraResult result = dijkstra(graph, start, end);
    if (result.path.empty() || !(result.distanceMeters < std::numeric_limits<double>::infinity())) {
      std::string err = Json::makeObject({{"error", "No route found"}});
      res.set_content(err, "application/json");
      return;
    }
    std::vector<std::string> pathArr;
    for (size_t i = 0; i < result.path.size(); ++i) {
      int idx = result.path[i];
      std::string pt = Json::makeObject({
          {"id", Json::number(static_cast<double>(idx))},
          {"name", Json::quote(points[idx].name)},
          {"lat", Json::number(points[idx].lat)},
          {"lon", Json::number(points[idx].lon)}
      });
      pathArr.push_back(pt);
    }
    std::string json = Json::makeObject({
        {"distance", Json::number(result.distanceMeters)},
        {"path", Json::makeArray(pathArr)}
    });
    res.set_content(json, "application/json");
    {
      std::stringstream ss;
      ss << "From " << points[start].name << " -> To " << points[end].name;
      history.push(ss.str());
    }
    visited.insert(points[end].name); });
  svr.Get("/api/history", [&](const httplib::Request &, httplib::Response &res)
          {
    res.set_header("Access-Control-Allow-Origin", "*");
    std::vector<std::string> arr = history.toVector();
    std::reverse(arr.begin(), arr.end());
    std::vector<std::string> jsonArr;
    for (const std::string& entry : arr) {
      jsonArr.push_back(Json::quote(entry));
    }
    std::string json = Json::makeArray(jsonArr);
    res.set_content(json, "application/json"); });
  svr.Get("/api/visited", [&](const httplib::Request &, httplib::Response &res)
          {
    res.set_header("Access-Control-Allow-Origin", "*");
    std::vector<std::string> sorted = visited.toSortedVector();
    std::vector<std::string> jsonArr;
    for (const std::string& name : sorted) {
      jsonArr.push_back(Json::quote(name));
    }
    std::string json = Json::makeArray(jsonArr);
    res.set_content(json, "application/json"); });
  std::cout << "Starting C++ backend API server...\n";
  svr.listen("127.0.0.1", 8081);
  return 0;
}

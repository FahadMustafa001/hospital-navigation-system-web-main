#include "dijkstra.h"
#include <limits>
#include <queue>
#include <vector>
#include <utility>
#include <cstddef>
#include <algorithm>

DijkstraResult dijkstra(const std::vector<std::vector<std::pair<int, double>>>& graph,
                        int start,
                        int goal) {
  const int n = static_cast<int>(graph.size());
  const double INF = std::numeric_limits<double>::infinity();
  std::vector<double> dist(n, INF);
  std::vector<int> prev(n, -1);
  std::vector<char> visited(n, 0);
  typedef std::pair<double, int> Node;
  std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
  dist[start] = 0.0;
  pq.push(Node(0.0, start));
  while (!pq.empty()) {
    const Node top = pq.top();
    pq.pop();
    const double d = top.first;
    const int u = top.second;
    if (visited[u]) continue;
    visited[u] = 1;
    if (u == goal) break;
    for (size_t i = 0; i < graph[u].size(); ++i) {
      const int v = graph[u][i].first;
      const double w = graph[u][i].second;
      if (visited[v]) continue;
      const double nd = d + w;
      if (nd < dist[v]) {
        dist[v] = nd;
        prev[v] = u;
        pq.push(Node(nd, v));
      }
    }
  }
  DijkstraResult res;
  res.distanceMeters = dist[goal];
  if (!(res.distanceMeters < INF)) return res;
  int cur = goal;
  while (cur != -1) {
    res.path.push_back(cur);
    cur = prev[cur];
  }
  std::reverse(res.path.begin(), res.path.end());
  return res;
}

DijkstraTraceResult dijkstraTrace(
    const std::vector<std::vector<std::pair<int, double>>>& graph,
    int start,
    int goal) {
  const int n = static_cast<int>(graph.size());
  const double INF = std::numeric_limits<double>::infinity();
  std::vector<double> dist(n, INF);
  std::vector<int> prev(n, -1);
  std::vector<char> visited(n, 0);
  typedef std::pair<double, int> Node;
  std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;

  DijkstraTraceResult trace;

  dist[start] = 0.0;
  pq.push(Node(0.0, start));
  while (!pq.empty()) {
    const Node top = pq.top();
    pq.pop();
    const double d = top.first;
    const int u = top.second;
    if (visited[u]) continue;
    visited[u] = 1;

    DijkstraStep step;
    step.pickedNode = u;
    step.pickedDistance = d;

    if (u == goal) {
      trace.steps.push_back(step);
      break;
    }

    for (size_t i = 0; i < graph[u].size(); ++i) {
      const int v = graph[u][i].first;
      const double w = graph[u][i].second;
      if (visited[v]) continue;
      const double nd = d + w;
      if (nd < dist[v]) {
        dist[v] = nd;
        prev[v] = u;
        pq.push(Node(nd, v));
        DijkstraRelaxation r;
        r.from = u;
        r.to = v;
        r.newDistance = nd;
        step.relaxations.push_back(r);
      }
    }

    trace.steps.push_back(step);
  }

  trace.result.distanceMeters = dist[goal];
  if (trace.result.distanceMeters < INF) {
    int cur = goal;
    while (cur != -1) {
      trace.result.path.push_back(cur);
      cur = prev[cur];
    }
    std::reverse(trace.result.path.begin(), trace.result.path.end());
  }
  return trace;
}

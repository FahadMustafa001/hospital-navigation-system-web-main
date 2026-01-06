#pragma once

#include <vector>
#include <utility>
#include <cstddef>

struct DijkstraResult {
  std::vector<int> path;
  double distanceMeters;
};

struct DijkstraRelaxation {
  int from;
  int to;
  double newDistance;
};

struct DijkstraStep {
  int pickedNode;
  double pickedDistance;
  std::vector<DijkstraRelaxation> relaxations;
};

struct DijkstraTraceResult {
  DijkstraResult result;
  std::vector<DijkstraStep> steps;
};

DijkstraResult dijkstra(const std::vector<std::vector<std::pair<int, double>>>& graph,
                        int start,
                        int goal);

DijkstraTraceResult dijkstraTrace(const std::vector<std::vector<std::pair<int, double>>>& graph,
                                  int start,
                                  int goal);

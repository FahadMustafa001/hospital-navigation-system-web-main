#pragma once

#include <string>
#include <vector>

struct Point {
  std::string name;
  double lat;
  double lon;
};

std::vector<Point> getDefaultHospitalPoints();

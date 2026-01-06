#include "data.h"
#include <vector>
#include <string>

std::vector<Point> getDefaultHospitalPoints() {
  std::vector<Point> points;
  points.push_back(Point{"Regional Blood Center", 31.451428, 73.080936});
  points.push_back(Point{"Masjid", 31.448060, 73.080848});
  points.push_back(Point{"Bike Parking", 31.448706, 73.081745});
  points.push_back(Point{"Main Gate", 31.448894, 73.081969});
  points.push_back(Point{"Emergency", 31.449165, 73.081763});
  points.push_back(Point{"Burn Unit", 31.451195, 73.081453});
  points.push_back(Point{"CCU", 31.450568, 73.081388});
  points.push_back(Point{"Residence", 31.450435, 73.079088});
  points.push_back(Point{"FMU Auditorium", 31.452001, 73.084886});
  points.push_back(Point{"Postmortem Unit", 31.448483, 73.079840});
  points.push_back(Point{"Oncology Ward", 31.450269, 73.082591});
  points.push_back(Point{"Eye Care Center", 31.450192, 73.081491});
  points.push_back(Point{"Day Care Center", 31.449905, 73.080400});
  points.push_back(Point{"Dept. of Pathology", 31.452177, 73.083784});
  points.push_back(Point{"Dept. of BioChemistry", 31.452554, 73.083538});
  points.push_back(Point{"FMU Examination", 31.452119, 73.082794});
  return points;
}

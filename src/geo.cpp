#include "geo.h"
#include <cmath>

namespace {
constexpr double kEarthRadiusMeters = 6371000.0;
constexpr double kPi = 3.141592653589793238462643383279502884;
double deg2rad(double deg) { return deg * (kPi / 180.0); }
}

double distanceMeters(double lat1, double lon1, double lat2, double lon2) {
  const double phi1 = deg2rad(lat1);
  const double phi2 = deg2rad(lat2);
  const double dphi = deg2rad(lat2 - lat1);
  const double dlambda = deg2rad(lon2 - lon1);
  const double a = std::sin(dphi / 2) * std::sin(dphi / 2) +
                   std::cos(phi1) * std::cos(phi2) * std::sin(dlambda / 2) *
                       std::sin(dlambda / 2);
  const double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
  return kEarthRadiusMeters * c;
}

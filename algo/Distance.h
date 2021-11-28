#pragma once

#include <cmath>
#include <vector>

class Distance {
 public:
  double operator()(const std::vector<int> &a, const std::vector<int> &b);
  double operator()(const std::vector<int> &a, const std::vector<double> &b);
  double operator()(const std::vector<double> &a, const std::vector<int> &b);
};
#include "Distance.h"
#include <iostream>

double Distance::operator()(const std::vector<int> &a, const std::vector<int> &b) {
  double d = 0;
  for (int i = 1; i < a.size(); i++) {
    if (i == 5)
      d += (a[i] - b[i]) * (a[i] - b[i]);
    else
      d += (a[i] - b[i]) * (a[i] - b[i]);
  }
//  d /= 1000;
  return std::sqrt(d);
}
double Distance::operator()(const std::vector<int> &a, const std::vector<double> &b) {
  double d = 0;
  for (int i = 1; i < a.size(); i++) {
    if (i == 5)
      d += (a[i] - b[i]) * (a[i] - b[i]);
    else
      d += (a[i] - b[i]) * (a[i] - b[i]);
  }
//  d /= 1000;
//  if (d > 1000)
//    std::cout << "dsf" << std::endl;
  return std::sqrt(d);
}
double Distance::operator()(const std::vector<double> &a, const std::vector<int> &b) {
  double d = 0;
  for (int i = 1; i < a.size(); i++) {
    if (i == 5)
      d += (a[i] - b[i]) * (a[i] - b[i]);
    else
      d += (a[i] - b[i]) * (a[i] - b[i]);
  }
//  d /= 1000;
//  if (d > 1000)
//    std::cout << "dsf" << std::endl;
  return std::sqrt(d);
}

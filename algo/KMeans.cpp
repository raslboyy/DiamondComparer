#include "KMeans.h"

#include <set>
#include <random>
#include <utility>
#include <iostream>

void KMeans::Fit(const DataFrame &data) {
  Initialization(data);
  int n = data.size();
  int per = n / _nClusters;
  _affiliation.clear();
  _affiliation.assign(n, -1);
  double e1 = 1e9;
  double e2 = 0;
//  && std::abs(e1 - e2) > eStop
  for (int nIter = 0; nIter < _maxIterations; nIter++) {
    CalculatePriority(data);

    for (int i = 0; i < n; i++) {
      int point = _priority[i].second;
      int cluster = _bestCluster[point];
      if (_clusters[cluster].size() < per) {
        _clusters[cluster].Add(data[point], point);
        _affiliation[point] = cluster;
      }
      else {
        double a = std::pow(_clusters[cluster].Last().distance, 2);
        double b = std::pow(Distance().operator()(_clusters[cluster].center, data[point]), 2);
        if (b < a) {
          int clusterA = cluster;
          int clusterB = _affiliation[point];

          if (clusterB != -1) {
            auto pointA = _clusters[clusterA].Pop();
            _clusters[clusterA].Add(data[point], point);
            _clusters[clusterB].Pop(point);
            _clusters[clusterB].Add(pointA.point, pointA.id);
            _affiliation[point] = clusterA;
            _affiliation[pointA.id] = clusterB;
          }

        }


      }
    }

    for (auto &cur : _clusters) {
      cur.Update();
      std::cout << cur.size() << ' ';
    }
    std::cout << std::endl;

    e2 = e1;
    e1 = E();
  }
}

void KMeans::Initialization(const DataFrame &data) {
  if (_initializationMethod == "random")
    RandomInitialization(data);
  else
    SmartInitialization(data);
}

void KMeans::RandomInitialization(const DataFrame &data) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<size_t> dist(0, data.size() - 1);

  std::set<int> s;
  while (s.size() < _nClusters)
    s.emplace(dist(mt));

  for (auto i: s) {
    try {
      _clusters.emplace_back(data[i]);
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }

  }
}

void KMeans::SmartInitialization(const DataFrame &data) {
}

void KMeans::CalculatePriority(const DataFrame &data) {
  _bestCluster.clear();
  _bestCluster.resize(data.size());
  _priority.clear();
  _priority.resize(data.size());
  for (int i = 0; i < data.size(); i++) {
    std::vector<std::pair<double, int>> a(_nClusters);
    for (int j = 0; j < _nClusters; j++)
      a[j].first = (Distance().operator()(_clusters[j].center, data[i])), a[j].second = j;
    std::sort(a.begin(), a.end());
    _priority[i].first = a.begin()->first - a.rbegin()->first;
    _priority[i].second = i;
    _bestCluster[i] = a.begin()->second;
  }
  std::sort(_priority.begin(), _priority.end());
}

KMeans::DistancePoint::DistancePoint(const std::vector<int>& point, std::vector<double> center, int id) :
    point(point),
    distance(),
    id(id) {

  try {
    distance = (new Distance())->operator()(point, center);
  }
  catch (...){
    std::cout << "e.what()" << std::endl;
  }
}

void KMeans::Cluster::Add(std::vector<int> point, int id) {
  points[id] = DistancePoint(point, center, id);
  _pointsLasts.emplace(DistancePoint(point, center, id));
}
KMeans::DistancePoint KMeans::Cluster::Last() {
  if (_pointsLasts.empty())
    return DistancePoint{};
  while (!_pointsLasts.empty() && points.find(_pointsLasts.top().id) == points.end())
    _pointsLasts.pop();

//  if (_pointsLasts.size() != points.size())
//    std::cout << _pointsLasts.size() << " " << points.size() << std::endl;
  return _pointsLasts.top();
}
KMeans::DistancePoint KMeans::Cluster::Pop() {
  Last();
  int id = _pointsLasts.top().id;
  auto res = _pointsLasts.top();
  _pointsLasts.pop();
  if (points.find(id) != points.end())
    points.erase(points.find(id));

  return res;
}
void KMeans::Cluster::Pop(int idx) {
  if (points.find(idx) != points.end())
    points.erase(points.find(idx));

}
double KMeans::Cluster::operator()() {
  double sum = 0;
  for (auto &i : points)
    sum += std::pow(i.second.distance, 2);
  return sum;
}
void KMeans::Cluster::Update() {
  std::vector<std::vector<int>> t;
  for (auto &i : points)
    t.emplace_back(i.second.point);

  for (int i = 0; i < center.size(); i++) {
    double sum = 0;
    for (int j = 0; j < size(); j++)
      sum += t[j][i];
    center[i] = sum / (double)size();
  }
}

double KMeans::E() {
  double sum = 0;
  for (auto &i : _clusters)
    sum += i();
  return sum;
}
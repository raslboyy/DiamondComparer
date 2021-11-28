#pragma once

#include "../data/DataFrame.h"
#include "Distance.h"

#include <unordered_map>
#include <utility>
#include <queue>

class KMeans {
 public:
  void SetMaxIterations(int value) { _maxIterations = value; }

  void Fit(const DataFrame&);
//  void Predict(const DataFrame&);

  class Cluster;
  std::vector<Cluster> _clusters;
  std::vector<std::pair<double, int>> _priority;
  std::vector<int> _bestCluster;
  std::vector<int> _affiliation;
  int _maxIterations = 100;
  int _nClusters = 10;
  double eStop = 1e-4;
  std::string _initializationMethod = "random";

  void Initialization(const DataFrame &data);
  void RandomInitialization(const DataFrame &data);
  void SmartInitialization(const DataFrame &data);

  void CalculatePriority(const DataFrame &data);

  double E();

  struct DistancePoint {
    DistancePoint() : distance(1e9), point(), id() {}
    DistancePoint(const std::vector<int>& point, std::vector<double> center, int id);
    DistancePoint(const DistancePoint& other) = default;
    DistancePoint &operator=(const DistancePoint& other) = default;
    int id;
    double distance;
    std::vector<int> point;
  };

  class Cluster {
   public:
    explicit Cluster(std::vector<int> center1) : points(), center(center1.size(), 0) {
      std::copy(center1.begin(), center1.end(), this->center.begin());
    }
    void Add(std::vector<int> point, int id);
    size_t size() const { return points.size(); }
    DistancePoint Last();
    DistancePoint Pop();
    void Pop(int idx);
    std::vector<double> center;

    double operator()();
    void Update();
    std::unordered_map<int, DistancePoint> points;
   private:

    static constexpr auto cmp = [](const DistancePoint &a, const DistancePoint &b) {
      return a.distance < b.distance;
    };

    std::priority_queue<DistancePoint, std::vector<DistancePoint>, decltype(cmp)> _pointsLasts;
  };
};
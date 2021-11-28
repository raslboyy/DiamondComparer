#include "DataFrame.h"
#include "../algo/KMeans.h"

#include <fstream>
#include <sstream>
#include <algorithm>

DataFrame::DataFrame(std::string path) {
  std::ifstream csv(path);

  if (!csv)
    throw std::exception();

  std::string line, word, t;
  {
    getline(csv, line);
    std::stringstream s(line);
    while (getline(s, word, ','))
      _columns.emplace_back(word);
    _columns.back().erase(_columns.back().find('\r'));
  }

  while (getline(csv, line)) {
    std::stringstream s(line);
    _rows.emplace_back(std::vector<std::variant<std::string, int>>());

    while (getline(s, word, ',')) {
      try {
        int value = std::stoi(word);
        _rows.back().emplace_back(value);
      }
      catch (std::exception &) {
        _rows.back().emplace_back(word);
      }
    }
  }
}

void DataFrame::Save(std::string path, KMeans& k_means) {
  std::ofstream csv(path);

  for (const auto &column: _columns)
    csv << column << ',';
  csv << '\n';

  for (int i = 0; i < k_means._nClusters; i++) {
    csv << "Коробка №" << std::to_string(i + 1) << '\n';
    for (auto &c: k_means._clusters)
      for (auto &k : c.points) {
        for (auto &gf: k.second.point)
          csv << gf << ',';
        csv << '\n';
      }
  }

  for (const auto &row: _rows) {
    for (const auto &element: row)
      csv << shell{element} << ',';
    csv << '\n';
  }
}

std::unordered_map<std::string, int> DataFrame::UniqueColumnValueDictionary(int column) {
  std::unordered_map<std::string, int> dict;
  for (auto row: _rows)
    if (dict.find(std::get<std::string>(row[column])) == dict.end())
      dict[std::get<std::string>(row[column])] = (int) dict.size();
  return dict;
}

int DataFrame::FindColumn(const std::string &column) {
  int at = std::find(_columns.begin(), _columns.end(), column) - _columns.begin();
  if (at == _columns.size())
    throw std::exception();
  return at;
}

std::vector<int> DataFrame::operator[](size_t i) const {
  std::vector<int> row;
  for (auto element : _rows[i]) {
    try{
      row.emplace_back(std::get<int>(element));
    }
    catch (std::exception& e) {
      row.emplace_back(std::stoi(std::get<std::string>(element)));
    }
  }
  return row;
}
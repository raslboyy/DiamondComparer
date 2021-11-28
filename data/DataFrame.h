#pragma once

#include <unordered_map>
#include <vector>
#include <variant>
#include <memory>

class KMeans;

class DataFrame {
 public:
  explicit DataFrame(std::string path);
  DataFrame(const DataFrame &other) = default;

  template<class... Ts>
  std::shared_ptr<DataFrame> ToCategoricalFeature(const std::string& column, const Ts &... args);

  template<class... Ts>
  std::shared_ptr<DataFrame> DropColumns(const std::string& column, const Ts &... args);

  void Save(std::string path, KMeans& k_means);

  std::vector<int> operator[](size_t i) const;
  [[nodiscard]] size_t size() const { return _rows.size(); }
 private:
  std::vector<std::string> _columns;
  std::vector<std::vector<std::variant<std::string, int>>> _rows;

  int FindColumn(const std::string& column);
  std::shared_ptr<DataFrame> ToCategoricalFeature() { return std::make_shared<DataFrame>(*this); }
  std::shared_ptr<DataFrame> DropColumns() { return std::make_shared<DataFrame>(*this); }
  std::unordered_map<std::string, int> UniqueColumnValueDictionary(int column);
};

template<class T>
struct shell {
  const T &val;
};
template<class T> shell(T) -> shell<T>;

template<class T>
std::ostream &operator<<(std::ostream &os, shell<T> s) {
  os << s.val;
  return os;
}

template<class... Ts>
std::ostream &operator<<(std::ostream &os, shell<std::variant<Ts...>> sv) {
  std::visit([&os](const auto &v) { os << shell{v}; }, sv.val);
  return os;
}

template<class... Ts>
std::shared_ptr<DataFrame> DataFrame::ToCategoricalFeature(const std::string &column, const Ts &... args) {
  {
    std::shared_ptr<DataFrame> pData = ToCategoricalFeature(args...);
    int at = FindColumn(column);

    auto dict = UniqueColumnValueDictionary(at);
    for (auto &row: pData->_rows)
      row[at] = dict[std::get<std::string>(row[at])];
    return pData;
  }
}

template<class... Ts>
std::shared_ptr<DataFrame> DataFrame::DropColumns(const std::string &column, const Ts &... args) {
  auto pData = DropColumns(args...);
  int at = FindColumn(column);
  pData->_columns.erase(pData->_columns.begin() + at);
  for (auto &row: pData->_rows)
    row.erase(row.begin() + at);
  return pData;
}

#include <iostream>
#include "data/DataFrame.h"
#include "algo/KMeans.h"

int main() {
  DataFrame data("data.csv");
  auto newData = data.ToCategoricalFeature("Форма", "Цвет", "Флуоресценция", "Размер");
  newData = newData->DropColumns("Стоимость");

  KMeans algo;
  algo.Fit(*newData);
  newData->Save("save.csv", algo);

//  data.Save("save.csv");
  return 0;
}

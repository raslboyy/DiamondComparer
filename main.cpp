#include <iostream>
#include "data/DataFrame.h"

int main() {
  DataFrame data("data.csv");
  auto newData = data.ToCategoricalFeature("Форма", "Цвет");
  newData = newData->DropColumns("Стоимость");
  newData->Save("save.csv");
//  data.Save("save.csv");
  return 0;
}

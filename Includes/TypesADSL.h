#pragma once

#include <vector>

namespace adsl {

union DataEntry {
  int intU;
  double doubleU;
  char* strU;
};

enum DataType{
  INT,
  DBL,
  STR
};

typedef std::vector<DataEntry> vde;
typedef std::vector<vde> vvde;

typedef std::vector<double> vd;
typedef std::vector<vd> vvd;

}
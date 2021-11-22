// STL includes
//#include "Includes/ADSL.h"
#include "Includes/DataFrame.h"
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_math.h>
#include <numeric>

using namespace std;
using adsl::DataList;
using adsl::DataType;
// using adsl::DataFrame;
//using adsl::DataFrameList;

int main() {

  std::vector<int> a = {1, 2, 3, 4};
  DataList col1(&a, DataType::INT,"");
  std::cout << col1.str() << std::endl;

  std::vector<std::string> b = {"1", "2", "3", "4"};
  DataList col2(&b, DataType::STR,"");
  std::cout << col2.str() << std::endl;

    return 0;
}
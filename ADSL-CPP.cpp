// STL includes
#include "Includes/ADSL.h"
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_math.h>
#include <numeric>

using namespace std;
using namespace adsl;

int main() {

    std::vector<int> a = {1, 2, 3, 4};
    DataList col1(&a, DataType::INT,"");
    std::cout << col1.str() << std::endl;

    cout << "Press Enter to exit... ";
    cin.get();
}






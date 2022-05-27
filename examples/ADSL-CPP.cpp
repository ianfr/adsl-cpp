// STL includes
#include "../include/ADSL.h"
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_math.h>
#include <numeric>

using namespace std;
using namespace adsl;

int main() {

    std::vector<int> a = {1, 2, 3, 4};
    DataList col1(&a, DataType::INT,"");
    // std::cout << col1.str() << std::endl;

    // test ta-lib sma
    vd testPricesVec;
    for (int i=0; i < 100; i++) {
        testPricesVec.push_back(i * i);
    }
    DataList testPrices(&testPricesVec, DataType::DBL, "some test prices");
    // cout << testPrices + dl_MA("SMA", 30) + dl_str << endl;
    // cout << testPrices + dl_MA("EMA", 30) + dl_str << endl;

    double x = testPrices.at_dbl(3);

    cout << x << endl;

    cout << "Press Enter to exit... ";
    cin.get();
}






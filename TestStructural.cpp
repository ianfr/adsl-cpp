// STL includes
//#include "Includes/ADSL.h"
#include "Includes/DataFrame.h"
#include "Includes/TempOps.h"
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_math.h>
#include <numeric>

using namespace std;
using namespace adsl;

int main() {

    std::vector<int> a = {1, 2, 3, 4};
    DataList col1(&a, DataType::INT,"");
    std::cout << col1.str() << std::endl;

    std::vector<std::string> b = {"1", "2", "3", "40"};
    DataList col2(&b, DataType::STR,"");
    std::cout << col2.str() << std::endl;

    std::string testStr = col1 + dl_str;
    std::cout << testStr << std::endl;

    auto x = col1 + dl_toVec_int;
    auto y = col2 + dl_toVec_str;

    for (auto i : x) cout << i << " ";
    cout << endl;
    for (auto i : y) cout << i << " ";
    cout << endl;

    cout << col1 + dl_getVal_int(1) << endl;
    cout << col2 + dl_getVal_str(2) << endl;

    auto col3 = col1 + dl_append(col1);
    cout << col3.str() << endl;

    auto cond = [](int x) {
        if (x > 2)
            return true;
        return false;
    };
    auto col4 = col1 + dl_filter_int(cond);
    cout << col4.str() << endl;

    auto cond1 = [](std::string x) {
        if (x.size() == 1)
            return true;
        return false;
    };
    auto col5 = col2 + dl_filter_str(cond1);
    cout << col5.str() << endl;

    auto col6 = col1 + dl_filterLT_int(3);
    cout << col6.str() << endl;

    auto squareIt = [](int x) {
        return x*x;
    };
    auto col7 = col1 + dl_map_int(squareIt);
    cout << col7.str() << endl;

    auto strMod = [](std::string x) {
        std::replace(x.begin(), x.end(), '1', 'X');
        return x;
    };
    auto col8 = col2 + dl_map_str(strMod);
    cout << col8.str() << endl;

  return 0;
}
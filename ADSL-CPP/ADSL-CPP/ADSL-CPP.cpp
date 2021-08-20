// STL includes
#include <iostream>
#include <cassert>
// adsl includes
#include "DataFrame.h"
#include "Util.h"

using namespace std;
using adsl::DataFrame;
using adsl::DataList;

//auto tryFun = [](adsl::DataFrame& df) {
//    adsl::DataFrame ret = df;
//    adsl::vvd tmp = { {1,1,1,1}, {1,1,1,1} };
//    ret.setDbl(tmp);
//    return ret;
//};

int main() {

    // Create a DataFrame with one column
    adsl::vd someData = { 1, 2, 3, 4 };
    DataList someList(someData, "hi");
    DataFrame df;
    df.addCol(someList);

    // Increment a DataList twice with chaining
    DataList res = df.getData()[0] + adsl::inc + adsl::inc;
    cout << res.vals[0] << endl;

    // Test a DataList <- DataFrame
    DataList first = df + adsl::getFirst;
    cout << first.vals[0] << endl;
    
    // Test a DataList <- DataList <- double
    DataList less = first + adsl::filterLess(3);
    cout << less.vals.size() << endl;

    // Use a custom predicate
    // Test a DataList <- DataList <- bool <- double
    auto cond = [](double x) {
        return x > 2;
    };
    DataList filtered = first + adsl::filter(cond);
    cout << filtered.str();

    cout << df.str();

    // Use a custom predicate
    // Test a DataList <- DataFrame <- bool <- string
    /*
    auto cond = [](double x) {
        return x > 2;
    };
    DataList filteredStr = first + adsl::filter(cond);
    */
}






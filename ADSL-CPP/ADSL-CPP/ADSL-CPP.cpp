// STL includes
#include <iostream>
#include <cassert>
// adsl includes
#include "DataFrame.h"
#include "Ops.h"

using namespace std;
using adsl::DataFrame;
using adsl::DataList;

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

}






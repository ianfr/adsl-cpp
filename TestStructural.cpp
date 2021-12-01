// STL includes
//#include "Includes/ADSL.h"
#include "Includes/DataFrame.h"
#include "Includes/Ops.h"
#include "Includes/Plot.h"
#include "Includes/GSL.h"
#include "Includes/EvalFit.h"
#include "Includes/TSA.h"
#include "Includes/Dlib.h"
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_math.h>
#include <numeric>

using namespace std;
using namespace adsl;

int main() {

    // DataList
    /*
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
    

    // DataFrame

    // Create a DataFrame with one column
    vd someData = { 1, 2, 3, 4 };
    DataList someList(&someData, DataType::DBL, "hi");
    DataFrame df;
    df.addCol(someList);
    cout << df.str() << endl;

    // Test double <- DataList <- DataFrame
    cout << df + df_getData(0) + dl_getVal_dbl(0) << endl;

    // Test selection based on name
    vd vec0 = {3,4,5};
    vd vec1 = {6,7,8};
    vd vec2 = {9,10,11};
    DataList dl0(&vec0, DataType::DBL, "dogs");
    DataList dl1(&vec1, DataType::DBL, "cats");
    DataList dl2(&vec2, DataType::DBL, "birds");
    DataFrame df1;
    df1.addCol(dl0);
    df1.addCol(dl1);
    df1.addCol(dl2);
    cout << df1 + df_select({"dogs", "birds"}) + df_str << endl;

    // Test deselect
    cout << df1 + df_deselect({"dogs"}) + df_str << endl;

    // Test combineV
    cout << df1 + df_combineV(df1) + df_str << endl;

    // Test loading DFL from directory
    DataFrameList dflist;
    dflist.loadFramesFromDir("Datasets/Stocks", ".us.txt", ",", -1);
    cout << dflist.size() << endl;

    // Test filtering a DataFrameList
    auto getAA = [](DataFrame df) {
        cout << df.getDesc() << endl;
        if (df.getDesc().compare("aa") == 0) {
            return true;
        }
        return false;
    };
    auto filteredDfl = dflist + dfl_filter(getAA);
    // cout << filteredDfl.getFrame(0) + df_str << endl;

    // Test DataFrameList select
    auto selectedDfl = dflist + dfl_select({"a"});
    //cout << selectedDfl.getFrame(0) + df_str << endl;

    // Test reading in a CSV file
    DataFrame dfRead = loadFromCSV_dbl("Datasets/testMat.csv", ",", true);
    cout << dfRead.str();

    // Write to a CSV
    writeToCSV_dbl(df1, "Outputs/df1.csv");

    */

    // Plot 2D
    vd tmp0 = { 1, 2, 3, 4 };
    vd tmp1 = { 1, 2, 3, 4 };
    DataList lst0 (&tmp0, DataType::DBL, "DL 0");
    #undef lst1
    DataList lst1 (&tmp1, DataType::DBL, "DL 1");
    DataFrame df2;
    df2.addCol(lst0);
    df2.addCol(lst1);
    df2.setDesc("testing plotting 2d");
    //df2 + scatter2D({"testPlot.png", "800,600"});

    // Least-squares
    tmp0 = { 1, 2, 3, 4 };
    tmp1 = { 1, 2.2, 3, 3.9 };
    DataList lst_0(&tmp0, DataType::DBL, "DL 0");
    DataList lst_1(&tmp1, DataType::DBL, "DL 1");
    DataFrame df4;
    df4.addCol(lst_0);
    df4.addCol(lst_1);
    df4.setDesc("testing linear fit");
    DataFrame linFit = df4 + fitLinear;
    cout << linFit.str();
    cout << "prediction at x=5: " << linFit + evalFit({5}) << endl;

    // Test ARIMA
    vd arimaTestVec = {16.9969,16.5969,16.802,17.1,17.3995,17.3975,17.5008,17.3998,17.6,17.4,17.3,17,17.8};
    DataList arimaDL = DataList(&arimaTestVec, DataType::DBL, "Test Input");
    DataFrame arimaDF;
    arimaDF.addCol(arimaDL);
    DataFrame arimaResults = arimaDF + autoARIMA(5);
    cout << "ARIMA results:\n" << arimaResults.str();

    // Test Dlib classifier
    DataFrame irisData = loadFromCSV_dbl("Datasets/iris.csv", ",", true);
    vector<double> labels = irisData + df_select({ "class" }) + df_getData(0) + dl_toVec_dbl;
    DataFrame feats = irisData + df_deselect({ "class" });
    DataFrame classResult = feats + binaryClassifier( labels ); // empty for now


    return 0;
}
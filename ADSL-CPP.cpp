// STL includes
#include "Includes/ADSL.h"
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_math.h>
#include <numeric>

using namespace std;
using adsl::DataFrame;
using adsl::DataList;
using adsl::DataFrameList;

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

    // Test reading in a CSV file
    DataFrame df1 = adsl::loadFromCSV("Datasets/testMat.csv", ",", true);
    cout << df1.str();

    // Sum a DataList
    cout << first + adsl::sum;

    // Print a DataList (in chain)
    cout << first + adsl::filterLess(3) + adsl::str;

    // Test transform
    auto squareX = [](double x) {
        return x * x;
    };
    cout << first + adsl::transform(squareX) + adsl::str;

    // Mapreduce!
    cout << first + adsl::transform(squareX) + adsl::sum;

    // Write to a CSV
    adsl::writeToCSV(df1, "Outputs/df1.csv");

    // Plot 2D
    adsl::vd tmp0 = { 1, 2, 3, 4 };
    adsl::vd tmp1 = { 1, 2, 3, 4 };
    DataList lst0 (tmp0, "DL 0");
#undef lst1
    DataList lst1 (tmp1, "DL 1");
    DataFrame df2;
    df2.addCol(lst0);
    df2.addCol(lst1);
    df2.setDesc("testing plotting 2d");
    df2 + adsl::scatter2D({"testPlot.png", "800,600"});

    // Select specific columns
    auto df3 = df2 + adsl::select({ "DL 1" });
    cout << df3.str();

    // Least-squares
    tmp0 = { 1, 2, 3, 4 };
    tmp1 = { 1, 2.2, 3, 3.9 };
    DataList lst_0(tmp0, "DL 0");
    DataList lst_1(tmp1, "DL 1");
    DataFrame df4;
    df4.addCol(lst_0);
    df4.addCol(lst_1);
    df4.setDesc("testing linear fit");
    auto linFit = df4 + adsl::fitLinear;
    cout << linFit.str();
    cout << "prediction at x=5: " << linFit + adsl::evalFit({5});

    // Combine DataFrames vertically 
    DataFrame df5 = df4 + adsl::combineV(df2);
    cout << "Combining DataFrames verically:\n" << df5.str();

    // Test Dlib
    //cout << "Testing Dlib" << endl;
    //adsl::dlib_run_svm_c_trainer();

    // Test Dlib classifier
    DataFrame irisData = adsl::loadFromCSV("Datasets/iris.csv", ",", true);
    vector<double> labels = irisData + adsl::select({ "class" }) + adsl::getFirst + adsl::toVec;
    DataFrame feats = irisData + adsl::deselect({ "class" });
    //DataFrame classResult = feats + adsl::binaryClassifier( labels );

    // Test ARIMA
    adsl::mimicAutoArimaTest1(); // slightly modified version of the autoarimatest1 from ctsa
    std::vector<double> arimaTestVec = {16.9969,16.5969,16.802,17.1,17.3995,17.3995,17.5008,17.3998,17.6,17.4,17.3,17,17.8};
    DataList arimaDL = DataList(arimaTestVec, "Test Input");
    DataFrame arimaDF;
    arimaDF.addCol(arimaDL);
    DataFrame arimaResults = arimaDF + adsl::autoARIMA(5);
    cout << "ARIMA results:\n" << arimaResults.str();

    // Test reading in time-series and plot 1 col of vals
    DataFrame timeSeries = adsl::loadFromCSV_wDate("Datasets/finMini.csv", ",", true, 0, true);
    cout << "Time series results:\n" << timeSeries.str();
    timeSeries + adsl::select({"date", "val1"}) + adsl::scatter2D({"testFin.png", "800,600"});
    adsl::writeToCSV(timeSeries, "TimeSeriesOut.csv");

    // test the KRLS regression on fitting a function f(x1,x1)
    // sqrt x1^2 + x2^2
    auto fTest = [](double x1, double x2) {
        return gsl_hypot(x1, x2);
    };
    int numPts = 100;
    adsl::vd x1Vec(numPts), x2Vec(numPts), yVec(numPts);
    std::iota(x1Vec.begin(), x1Vec.end(), 1);
    std::iota(x2Vec.begin(), x2Vec.end(), 1);
    for (int i=0; i < x1Vec.size(); i++) {
        yVec[i] = fTest(x1Vec[i], x2Vec[i]);
    }
    DataList kx1(x1Vec, "x1"), kx2(x2Vec, "x2"), ky(yVec, "f(x1,x2)");
    DataFrame testKrls;
    testKrls.setDesc("Testing KRLS Regression");
    for (auto kcol : {kx1, kx2, ky})
        testKrls.addCol(kcol);
    cout << testKrls.str() << endl;

    adsl::vd krlsLabels = testKrls + adsl::select({"f(x1,x2)"}) + adsl::getFirst + adsl::toVec;
    auto krlsFit = testKrls + adsl::deselect({"f(x1,x2)"}) + adsl::krlsReg(krlsLabels, "krlsModel.dat");
    double sqrt3_3 = krlsFit + adsl::evalFit({3,3});
    cout << "Sqrt of 3^2 + 3^2: " << sqrt3_3 << endl;

    // Test DataFrameList
    DataFrameList dflist;
    dflist.loadFramesFromDir("Datasets", ".us.txt", ",", false);
    cout << dflist.size() << endl;


    cout << "Press Enter to exit... ";
    cin.get();
}






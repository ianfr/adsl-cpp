// STL includes
#include "../include/ADSL.h"
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_math.h>
#include <numeric>

using namespace std;
using namespace adsl;

int main() {

    // test the KRLS regression on fitting a function f(x1,x1)
    // sqrt x1^2 + x2^2
    auto fTest = [](double x1, double x2) {
        return gsl_hypot(x1, x2);
    };
    int numPts = 100;
    vd x1Vec(numPts), x2Vec(numPts), yVec(numPts);
    iota(x1Vec.begin(), x1Vec.end(), 1);
    iota(x2Vec.begin(), x2Vec.end(), 1);
    for (int i=0; i < x1Vec.size(); i++) {
        yVec[i] = fTest(x1Vec[i], x2Vec[i]);
    }

    DataList kx1(&x1Vec, DataType::DBL, "x1"), 
             kx2(&x2Vec, DataType::DBL, "x2"), 
             ky(&yVec, DataType::DBL, "f(x1,x2)");
    DataFrame testKrls;
    testKrls.setDesc("Testing KRLS Regression");
    for (auto kcol : {kx1, kx2, ky})
        testKrls.addCol(kcol);
    cout << testKrls.str() << endl;

    vd krlsLabels = testKrls + df_select({"f(x1,x2)"}) + df_getData(0) + dl_toVec_dbl;
    auto krlsFit = testKrls + df_deselect({"f(x1,x2)"}) + krlsReg(krlsLabels, "krlsModel.dat");
    double sqrt3_3 = krlsFit + adsl::evalFit({3,3});
    cout << "Sqrt of 3^2 + 3^2: " << sqrt3_3 << endl;

    auto rowTest = [](void* x) {
        double a = *(double*)x;
        if (a > 50) {
            return true;
        }
        return false;
    };
    // DataFrame tmp = testKrls.filter_rows(0, rowTest);

    // std::cout << tmp.str() << std::endl;

    std::cout << testKrls + df_filter_rows(0, rowTest) + df_str << std::endl;

    cout << "Press Enter to exit... ";
    cin.get();
}






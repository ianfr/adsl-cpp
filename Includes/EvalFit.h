#include "DataFrame.h"
#include "Ops.h"
#include "DlibConst.h"
#include <dlib/svm.h>

namespace adsl {

// Evaluate the result of a fit at a given point based on the DataFrame description
// supports: OLS, KRLS, ARIMA (TODO)
// double <- DataFrame
auto evalFit = [](std::vector<double> t) {
    auto retFunc = [t](DataFrame& df) {
        if (df.getDesc() == "gsl_fit_linear") {
            double m = df + select({ "slope" }) + getFirst + single();
            double b = df + select({ "intercept" }) + getFirst + single();
            return (m * t[0]) + b;
        }
        else if (df.getDesc() == "KRLS") {
            std::string modelFileName = df.getData()[0].name;
            double param = df.getData()[0].vals[0];
            using namespace dlib;
            typedef matrix<double,MAX_KRLS_F,1> sample_type;
            typedef radial_basis_kernel<sample_type> kernel_type;
            krls<kernel_type> krlsObj(kernel_type(param), KRLS_EPSILON);
            deserialize(modelFileName) >> krlsObj;
            sample_type samp;
            for (int i=0; i < t.size(); i++) {
                samp(i) = t[i];
            }
            return krlsObj(samp);
        }
        else {
            return (double)NAN;
        }
    };
    return retFunc;
};

}
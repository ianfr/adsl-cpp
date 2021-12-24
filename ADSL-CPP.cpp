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

    // test compile.sh & ta-lib
    TA_Real    closePrice[400];
    TA_Real    out[400];
    TA_Integer outBeg;
    TA_Integer outNbElement;

    for (int i=0; i < 400; i++) {
        closePrice[i] = (double)i;
    }

    TA_RetCode retCode = TA_MA( 0, 399,
        &closePrice[0],
        30,TA_MAType_SMA,
        &outBeg, &outNbElement, &out[0] );
    
    for( int i=0; i < outNbElement; i++ )
        printf( "Day %d = %f\n", outBeg+i, out[i] );


    cout << "Press Enter to exit... ";
    cin.get();
}






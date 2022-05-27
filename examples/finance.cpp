//
#include "../include/ADSL.h"
using namespace adsl;

int main() {

    // load in the financial dataset from disk
    // contains historical data scraped from Nasdaq for stocks with prices from $5 - $9.90
    DataFrameList allHist;
    allHist.loadFramesFromDir("Datasets/stocks_5_9-90", ".csv", ",", 1);
    std::cout << allHist.size() << std::endl;

}
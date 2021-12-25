#pragma once

#include "TA_includes.h"
#include "DataFrame.h"

namespace adsl {
    
    // Moving Average
    // maType can be "SMA" for simple moving average
        // or "EMA" for exponential moving average
    auto dl_MA = [](std::string maType, int period) {
        auto retFunc = [maType, period](DataList& dl) {
            return dl.calcMA(maType, period);
        };
        return retFunc;
    };

}
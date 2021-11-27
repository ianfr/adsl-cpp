// Temporary stand-in for Ops.h while I work on the new chainable things

#pragma once

#include "DataFrame.h"
#include <functional>

namespace adsl {

    // DataList ops

    // Chainable form of .str()
	// string <- DataList
	auto dl_str = [](DataList& dl) {
		return dl.str();
	};

    // Convert DataList to vector
	auto dl_toVec_int = [](DataList& dl) {
		return dl.toVec_int();
    };
    auto dl_toVec_dbl = [](DataList& dl) {
		return dl.toVec_dbl();
    };
    auto dl_toVec_str = [](DataList& dl) {
		return dl.toVec_str();
    };

    // Get a single element, first by default
    auto dl_getVal_int = [](int i = 0) {
        auto retFunc = [i](DataList& dl) {
            return dl.getVal_int(i);
        };
        return retFunc;
    };
    auto dl_getVal_dbl = [](int i = 0) {
        auto retFunc = [i](DataList& dl) {
            return dl.getVal_dbl(i);
        };
        return retFunc;
    };
    auto dl_getVal_str = [](int i = 0) {
        auto retFunc = [i](DataList& dl) {
            return dl.getVal_str(i);
        };
        return retFunc;
    };

    // Append one DataList to another
    auto dl_append = [](DataList& dl2) {
		auto retFunc = [&dl2](DataList& dl1) {
            return DataList::combineDLs(dl1, dl2);
		};
		return retFunc;
	};

    // Filters (generic)
    // Each _type suffix represents the datatype the predicate acts upon
    auto dl_filter_int = [](std::function<bool(int)> predicate) {
		auto retFunc = [predicate](DataList& dl) {
			return dl.filter_int(predicate);
		};
		return retFunc;
	};
    auto dl_filter_dbl = [](std::function<bool(double)> predicate) {
		auto retFunc = [predicate](DataList& dl) {
			return dl.filter_dbl(predicate);
		};
		return retFunc;
	};
    auto dl_filter_str = [](std::function<bool(std::string)> predicate) {
		auto retFunc = [predicate](DataList& dl) {
			return dl.filter_str(predicate);
		};
		return retFunc;
	};

    // Filters (convenience)
    // LT="less than", GT="greater than", LTE="less than or equal to", etc.
    auto dl_filterLT_int = [](int y) {
        auto cond = [y](int x) {
            if (x < y)
                return true;
            return false;
        };
        auto retFunc = [cond](DataList& dl) {
            return dl + dl_filter_int(cond);
        };
        return retFunc;
    };
    auto dl_filterLTE_int = [](int y) {
        auto cond = [y](int x) {
            if (x <= y)
                return true;
            return false;
        };
        auto retFunc = [cond](DataList& dl) {
            return dl + dl_filter_int(cond);
        };
        return retFunc;
    };
    auto dl_filterGT_int = [](int y) {
        auto cond = [y](int x) {
            if (x > y)
                return true;
            return false;
        };
        auto retFunc = [cond](DataList& dl) {
            return dl + dl_filter_int(cond);
        };
        return retFunc;
    };
    auto dl_filterGTE_int = [](int y) {
        auto cond = [y](int x) {
            if (x >= y)
                return true;
            return false;
        };
        auto retFunc = [cond](DataList& dl) {
            return dl + dl_filter_int(cond);
        };
        return retFunc;
    };
    auto dl_filterLT_dbl = [](double y) {
        auto cond = [y](double x) {
            if (x < y)
                return true;
            return false;
        };
        auto retFunc = [cond](DataList& dl) {
            return dl + dl_filter_dbl(cond);
        };
        return retFunc;
    };
    auto dl_filterLTE_dbl = [](double y) {
        auto cond = [y](double x) {
            if (x <= y)
                return true;
            return false;
        };
        auto retFunc = [cond](DataList& dl) {
            return dl + dl_filter_dbl(cond);
        };
        return retFunc;
    };
    auto dl_filterGT_dbl = [](double y) {
        auto cond = [y](double x) {
            if (x > y)
                return true;
            return false;
        };
        auto retFunc = [cond](DataList& dl) {
            return dl + dl_filter_dbl(cond);
        };
        return retFunc;
    };
    auto dl_filterGTE_dbl = [](double y) {
        auto cond = [y](double x) {
            if (x >= y)
                return true;
            return false;
        };
        auto retFunc = [cond](DataList& dl) {
            return dl + dl_filter_dbl(cond);
        };
        return retFunc;
    };

    // Map (i.e. "transform")
    auto dl_map_int = [](std::function<int(int)> action) {
        auto retFunc = [action](DataList& dl) {
            return dl.map_int(action);
        };
        return retFunc;
    };
    auto dl_map_dbl = [](std::function<double(double)> action) {
        auto retFunc = [action](DataList& dl) {
            return dl.map_dbl(action);
        };
        return retFunc;
    };
    auto dl_map_str = [](std::function<std::string(std::string)> action) {
        auto retFunc = [action](DataList& dl) {
            return dl.map_str(action);
        };
        return retFunc;
    };

    // DataFrame Ops

    auto df_str = [](DataFrame& df) {
        return df.str();
    };

    auto df_getData = [](int col) {
        auto retFunc = [col](DataFrame& df) {
            return df.getData(col);
        };
        return retFunc;
    };

    // Select relevant columns in a DataFrame
	// DataFrame <- DataFrame
	auto df_select = [](std::vector<std::string> nameVec) {
		auto retFunc = [nameVec](DataFrame& df) {
			return df.select(nameVec);
		};
		return retFunc;
	};

    // Select all columns in a DataFrame except those listed in the input
	// DataFrame <- DataFrame
	auto df_deselect = [](std::vector<std::string> nameVec) {
		auto retFunc = [nameVec](DataFrame& df) {
			return df.deselect(nameVec);
		};
		return retFunc;
	};

    // Vertically combine two DataFrames (i.e. same # cols, add rows)
	// The col names and description of df_1 will not be preserved
	// DataFrame <- DataFrame <- DataFrame
	auto df_combineV = [](DataFrame& df_2) {
		auto retFunc = [&df_2](DataFrame& df_1) {
			return df_1.combineV(df_2);
		};
		return retFunc;
	};



}
#pragma once
// Basic operations on DataLists and DataFrames

#include <vector>
#include <string>
#include <numeric>

#include "DataFrame.h"

namespace adsl {

	// Operations on DataLists

	// Increment a DataList
	// DataList <- DataList
	auto inc = [](DataList& dl) {
		DataList ret = dl;
		for (double& val : ret.vals) {
			val += 1;
		}
		return ret;
	};

	// Less than filter
	// function<DataList> <- double
	auto filterLess = [](double d) {
		auto retFunc = [d](DataList& dl) {
			DataList ret;
			ret.name = dl.name;
			for (double val : dl.vals) {
				if (val < d)
					ret.vals.emplace_back(val);
			}
			return ret;
		};
		return retFunc;
	};

	// Filter values in a DataList based on a custom predicate
	// function<DataList> <- bool <- double
	auto filter = [](std::function<bool(double)> predicate) {
		auto retFunc = [predicate](DataList& dl) {
			DataList ret;
			ret.name = dl.name;
			for (double val : dl.vals) {
				if (predicate(val))
					ret.vals.emplace_back(val);
			}
			return ret;
		};
		return retFunc;
	};

	// Basic operations on DataFrames

	// Get the first DataList in a DataFrame
	// DataList <- DataFrame
	auto getFirst = [](DataFrame& df) {
		return df.getData()[0];
	};

}
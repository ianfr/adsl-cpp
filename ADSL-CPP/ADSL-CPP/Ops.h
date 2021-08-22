#pragma once
// Basic operations on DataLists and DataFrames

#include <vector>
#include <string>
#include <numeric>

#include "DataFrame.h"

namespace adsl {

	// Operations on DataLists

	// Chainable form of .str()
	// string <- DataList
	auto str = [](DataList& dl) {
		return dl.str();
	};

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

	// Select relevant columns in a DataFrame
	// DataFrame <- DataFrame
	auto select = [](std::vector<std::string> nameVec) {
		auto retFunc = [nameVec](DataFrame& df) {
			DataFrame ret;
			ret.setDesc(df.getDesc());
			for (DataList dl : df.getData()) {
				if (std::find(nameVec.begin(), nameVec.end(), dl.name) != nameVec.end())
					ret.addCol(dl);
			}
			return ret;
		};
		return retFunc;
	};

	// Vertically combine two DataFrames
	// DataFrame <- DataFrame <- DataFrame
	auto combineV = [](DataFrame& df_1) {
		auto retFunc = [df_1](DataFrame& df) {
			// TODO
		};
	};

}
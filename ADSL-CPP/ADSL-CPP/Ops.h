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

	auto toVec = [](DataList& dl) {
		return dl.vals;
	};

	// Get a single element of a DataList
	// First element by default
	// double <- DataList
	auto single = [](double i = 0) {
		auto retFunc = [i](DataList& dl) {
			return dl.vals[i];
		};
		return retFunc;
	};

	// Append one DataList to another
	auto append = [](DataList& dl_1) {
		auto retFunc = [&dl_1](DataList& dl) {
			vd vec = dl.vals;
			vec.insert(vec.end(), dl_1.vals.begin(), dl_1.vals.end());
			return DataList(vec, dl.name);
		};
		return retFunc;
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
	// DataList <- DataList <- double
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

	// Select all columns in a DataFrame except those listed in the input
	// DataFrame <- DataFrame
	auto deselect = [](std::vector<std::string> nameVec) {
		auto retFunc = [nameVec](DataFrame& df) {
			DataFrame ret;
			ret.setDesc(df.getDesc());
			for (DataList dl : df.getData()) {
				if (std::find(nameVec.begin(), nameVec.end(), dl.name) == nameVec.end())
					ret.addCol(dl);
			}
			return ret;
		};
		return retFunc;
	};

	// Vertically combine two DataFrames (i.e. same # cols, add rows)
	// The col names and description of df_1 will not be preserved
	// DataFrame <- DataFrame <- DataFrame
	auto combineV = [](DataFrame& df_1) {
		auto retFunc = [&df_1](DataFrame& df) {
			DataFrame ret;
			if (df.getCols() == df_1.getCols()) {
				for (int i = 0; i < df.getCols(); i++) {
					DataList theCol = df.getData()[i] + append(df_1.getData()[i]);
					ret.addCol( theCol );
				}
				if (!ret.verifyDims()) {
					std::cout << "[combineV] <<WARNING>> Dimension verification failed" << std::endl;
				}
				ret.setDesc(df.getDesc());
				return ret;
			}
			else {
				std::cout << "[combineV] <<ERROR>> Unequal column lengths" << std::endl;
				return ret;
			}
		};
		return retFunc;
	};

}
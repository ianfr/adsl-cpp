#pragma once
// Lambdas providing wrappers around common STL functions

#include <algorithm>

#include "DataFrame.h"
#include "Ops.h"

namespace adsl {

	// DataList

	// Sum a DataList
	// Double <- DataList
	auto accumulate = [](DataList& dl) {
		return std::accumulate(dl.vals.begin(), dl.vals.end(), 0.0);
	};
	auto sum = accumulate; // alias

	// Apply a function to every element of a DataList
	// DataList <- DataList <- double <- double
	auto transform = [](std::function<double(double)> fun) {
		auto retFunc = [fun](DataList& dl) {
			DataList ret;
			ret.name = dl.name;
			std::transform(dl.vals.begin(), dl.vals.end(),
				           std::back_inserter(ret.vals), fun);
			return ret;
		};
		return retFunc;
	};

	// Apply a function to every element of a DataList
	// std::vector<T> <- DataList <- T <- double
	/*template<typename T>
	auto _transform = [](std::function<T(double)> fun) {
		auto retFunc = [fun](DataList& dl) {
			std::vector<T> ret;
			std::transform(dl.vals.begin(), dl.vals.end(),
				std::back_inserter(ret), fun);
			return ret;
		};
		return retFunc;
	};*/

}
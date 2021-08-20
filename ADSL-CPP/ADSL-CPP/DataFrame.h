#pragma once

// maybe make friend classes that operate on a DataFrame with a lambda and return a DataFrame

#include <string>
#include <vector>
#include <functional>
#include <numeric>
#include <iostream>

namespace adsl {

	// typedefs
	typedef std::vector<double> vd;
	typedef std::vector<std::vector<double>> vvd;

	class DataList {
	public:
		std::string name;
		vd vals;
		DataList() {
			name = "";
		}
		DataList(vd vec, std::string vecName) {
			name = vecName;
			vals = vec;
		}

		// Printing method
		std::string str();

		// Function chaining operator
		// Functions are assumed to operate on a DataList and return a DataList
		DataList operator+ (std::function<DataList(DataList&)> f) {
			return f(*this);
		}

		// Friends
		//friend class Ops;
	};
	
	typedef std::vector<DataList> vDL;

	class DataFrame { // Proper private/public
	private:
		int m_rows;
		int m_columns;
		std::string m_description;
		vDL m_data;

	public:
		// Getters
		int getRows();
		int getCols();
		std::string getDesc();
		vDL getData();

		// Setters
		void setDesc(std::string s);
		void setData(vDL& refMat);

		// Other
		std::string str();
		void addCol(DataList& refCol);
		//void addCol(vd vec&, std::string vecName);

		// Function chaining operators

		// DataFrame <- DataFrame
		DataFrame operator+ (std::function<DataFrame(DataFrame&)> f) {
			return f(*this);
		}

		// DataList <- DataFrame
		DataList operator+ (std::function<DataList(DataFrame&)> f) {
			return f(*this);
		}

		// DataList <- DataFrame & double
		/*DataList operator+ (std::function<DataList(DataFrame&, double)> f, double x) {
			return f(*this, x);
		}*/

		// Friends
		//friend class Ops;
	};

	/*class Ops {
	public:
		DataList inc(DataList& dl) {
			DataList ret = dl;
			for (double& val : ret.vals) {
				val += 1;
			}
			return ret;
		}
	};*/
	// std::function<DataList(DataList&)> inc = Ops.inc;

	// Basic operations on DataLists

	// Increment a DataList
	auto inc = [](DataList& dl) {
		DataList ret = dl;
		for (double& val : ret.vals) {
			val += 1;
		}
		return ret;
	};

	//TODO: try to allow a second (double) input for things like comparing
	//auto filterLess = [](DataList& dl, double d)
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

	// Filter values in a DataList
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
	auto getFirst = [](DataFrame& df) {
		return df.getData()[0];
	};


}
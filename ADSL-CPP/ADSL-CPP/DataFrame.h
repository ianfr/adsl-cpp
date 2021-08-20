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

		// String representation
		std::string str();

		// Function chaining operators

		// DataList <- DataList
		DataList operator+ (std::function<DataList(DataList&)> f) {
			return f(*this);
		}

		// double <- DataList
		double operator+ (std::function<double(DataList&)> f) {
			return f(*this);
		}

		// string <- DataList
		std::string operator+ (std::function<std::string(DataList&)> f) {
			return f(*this);
		}

	};
	
	typedef std::vector<DataList> vDL;

	class DataFrame {
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
		void appendToCol(int index, double value);
		void changeColName(int index, std::string value);

		// Function chaining operators

		// DataFrame <- DataFrame
		DataFrame operator+ (std::function<DataFrame(DataFrame&)> f) {
			return f(*this);
		}

		// DataList <- DataFrame
		DataList operator+ (std::function<DataList(DataFrame&)> f) {
			return f(*this);
		}
	};

}
#pragma once

// maybe make friend classes that operate on a DataFrame with a lambda and return a DataFrame

#include <string>
#include <vector>
#include <functional>
#include <numeric>
#include <iostream>
#include <sstream>

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
		int m_rows = 0;
		int m_columns = 0;
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

	// DataList

	// String representation
	std::string DataList::str() {
		using namespace std;
		stringstream os;
		os << "[BEGIN DataList]" << "\n" << "Name: " << name << endl;
		os << "Data:" << endl;
		for (auto val : vals) {
			os << val << " ";
		}
		os << "\n" << "[END DataList]" << endl;;
		return os.str();
	}


	// DataFrame
	
	// String representation
	std::string DataFrame::str() {
		using namespace std;
		stringstream os;
		os << "[[BEGIN DataFrame]]" << "\n" << "Description: " << m_description << endl;
		os << "Data:" << endl;
		for (auto list : m_data) {
			os << list.str();
		}
		os << "[[END DataFrame]]" << endl;;
		return os.str();
	}

	// Getters
	int DataFrame::getRows() {
		return m_rows;
	}

	int DataFrame::getCols() {
		return m_columns;
	}

	std::string DataFrame::getDesc() {
		return m_description;
	}

	vDL DataFrame::getData() {
		return m_data;
	}


	// Setters
	void DataFrame::setDesc(std::string s) {
		m_description = s;
	}

	void DataFrame::setData(vDL& matRef) {
		m_data = matRef;
		m_rows = matRef[0].vals.size();
		m_columns = matRef.size();
	}

	// Other
	void DataFrame::addCol(DataList& refCol) {
		m_data.emplace_back(refCol);
	}

	void DataFrame::appendToCol(int index, double value) {
		m_data[index].vals.push_back(value);
	}

	void DataFrame::changeColName(int index, std::string value) {
		m_data[index].name = value;
	}

}
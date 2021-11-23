#pragma once

// maybe make friend classes that operate on a DataFrame with a lambda and return a DataFrame

#include "TypesADSL.h"

#include <string>
#include <vector>
#include <functional>
#include <numeric>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <execution>
#include <iomanip>
#include <filesystem>
#include <regex>

namespace adsl {

	class DataList {
	public:
		// Data
		std::string name;
		vde vals;
		DataType type;


		// Constructors
		DataList() {
			name = "no_name";
		}
		DataList(void* input, DataType type, std::string name) {
			this->name = name;
			this->type = type;
			
			if (this->type == INT) {
				// this is a copy
				std::vector<int> vec = * (std::vector<int> *) input;
				for (auto vecVal : vec) {
					DataEntry tmpDataEntry;
					tmpDataEntry.intU = vecVal;
					vals.push_back(tmpDataEntry);
				}
			} else if (this->type == DBL) {
				// this is a copy
				std::vector<double> vec = * (std::vector<double> *) input;
				for (auto vecVal : vec) {
					DataEntry tmpDataEntry;
					tmpDataEntry.doubleU = vecVal;
					vals.push_back(tmpDataEntry);
				}
			} else if (this->type == STR) {
				// this is a copy
				std::vector<std::string> vec = * (std::vector<std::string> *) input;
				for (std::string vecVal : vec) {
					DataEntry tmpDataEntry;
					tmpDataEntry.strU = new char[vecVal.size()];
					strcpy(tmpDataEntry.strU, vecVal.c_str());
					vals.push_back(tmpDataEntry);
				}
			}
		}

		// String representation
		std::string str();

		// Vector conversion
		std::vector<int> toVec_int();
		std::vector<double> toVec_dbl();
		std::vector<std::string> toVec_str();

		// Get value at a specified index
		int getVal_int(int index) { return vals.at(index).intU; }
		double getVal_dbl(int index) { return vals.at(index).doubleU; }
		std::string getVal_str(int index) { 
			std::string tmp = std::string(vals.at(index).strU);
			return tmp; 
		}

		// Append one DataList to another
		static DataList combineDLs(DataList dl1, DataList dl2);

		// Filter
		DataList filter_int(std::function<bool(int)> predicate);
		DataList filter_dbl(std::function<bool(double)> predicate);
		DataList filter_str(std::function<bool(std::string)> predicate);

		// Map (i.e. "transform")
		DataList map_int(std::function<int(int)> action);
		DataList map_dbl(std::function<double(double)> action);
		DataList map_str(std::function<std::string(std::string)> action);

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

		// std::vector<int> <- DataList
		std::vector<int> operator+ (std::function<std::vector<int>(DataList&)> f) {
			return f(*this);
		}
		// std::vector<double> <- DataList
		std::vector<double> operator+ (std::function<std::vector<double>(DataList&)> f) {
			return f(*this);
		}
		// std::vector<double> <- DataList
		std::vector<std::string> operator+ (std::function<std::vector<std::string>(DataList&)> f) {
			return f(*this);
		}

	};

	// DataList

	// String representation
	std::string DataList::str() {
		using namespace std;
		stringstream os;
		os << "[BEGIN DataList] " << name << endl;
		for (auto val : vals) {
			if (type==INT) {
				os << val.intU << " ";
			} else if (type == DBL) {
				os << val.doubleU << " ";
			} else if (type == STR) {
				for (int i=0; i < strlen(val.strU); i++) {
					os << val.strU[i];
				}
			os << " ";
			} else {
				os << "ERR" << " ";
			}
		}
		os << "\n" << "[END DataList] " << name << endl;
		return os.str();
	}

	// vector conversion
	std::vector<int> DataList::toVec_int() {
		std::vector<int> ret;
		for (auto i : vals) {
			ret.push_back(i.intU);
		}
		return ret;
	}
	std::vector<double> DataList::toVec_dbl() {
		std::vector<double> ret;
		for (auto i : vals) {
			ret.push_back(i.doubleU);
		}
		return ret;
	}
	std::vector<std::string> DataList::toVec_str() {
		std::vector<std::string> ret;
		for (auto i : vals) {
			std::string tmp = std::string(i.strU); // convert from char*
			ret.push_back(tmp);
		}
		return ret;
	}

	// Append
	// Automatically assume the type and name of the first DataList
	DataList DataList::combineDLs(DataList dl1, DataList dl2) {
		if (dl1.type != dl2.type) {
			std::cerr << "<<ERROR>> [combineDLs] dl1.type != dl2.type" << std::endl;
			exit(1);
		}

		if (dl1.type == INT) {
			auto vec1 = dl1.toVec_int();
			auto vec2 = dl2.toVec_int();
			vec1.insert(vec1.end(), vec2.begin(), vec2.end());
			return DataList(&vec1, dl1.type, dl1.name);
		} else if (dl1.type == DBL) {
			auto vec1 = dl1.toVec_dbl();
			auto vec2 = dl2.toVec_dbl();
			vec1.insert(vec1.end(), vec2.begin(), vec2.end());
			return DataList(&vec1, dl1.type, dl1.name);
		} else if (dl1.type == STR) {
			auto vec1 = dl1.toVec_str();
			auto vec2 = dl2.toVec_str();
			vec1.insert(vec1.end(), vec2.begin(), vec2.end());
			return DataList(&vec1, dl1.type, dl1.name);
		} else {
			std::cerr << "<<ERROR>> [combineDLs] control should not have reached here" << std::endl;
			exit(1);
		}
	}

	// Filter values based on a predicate
	DataList DataList::filter_int(std::function<bool(int)> predicate) {
		DataList ret;
		ret.name = name;
		ret.type = type;
		for (auto val : vals) {
			if (predicate(val.intU))
				ret.vals.emplace_back(val.intU);
		}
		return ret;
	}
	DataList DataList::filter_dbl(std::function<bool(double)> predicate) {
		DataList ret;
		ret.name = name;
		ret.type = type;
		for (auto val : vals) {
			if (predicate(val.doubleU))
				ret.vals.emplace_back(val.doubleU);
		}
		return ret;
	}
	DataList DataList::filter_str(std::function<bool(std::string)> predicate) {
		DataList ret;
		ret.name = name;
		ret.type = type;
		for (auto val : vals) {
			std::string tmp = std::string(val.strU);
			if (predicate(tmp))
				ret.vals.push_back(val);
		}
		return ret;
	}

	// Map (i.e. "transform")
	DataList DataList::map_int(std::function<int(int)> action) {
		DataList ret;
		ret.type = type;
		ret.name = name;
		for (auto val : vals) {
			ret.vals.emplace_back(action(val.intU));
		}
		return ret;
	}
	DataList DataList::map_dbl(std::function<double(double)> action) {
		DataList ret;
		ret.type = type;
		ret.name = name;
		for (auto val : vals) {
			ret.vals.emplace_back(action(val.doubleU));
		}
		return ret;
	}
	DataList DataList::map_str(std::function<std::string(std::string)> action) {
		DataList ret;
		ret.type = type;
		ret.name = name;
		for (auto val : vals) {
			std::string tmp = std::string(val.strU);
			tmp = action(tmp);
			DataEntry tmpDataEntry;
			tmpDataEntry.strU = new char[tmp.size()];
			strcpy(tmpDataEntry.strU, tmp.c_str());
			ret.vals.emplace_back(tmpDataEntry);
		}
		return ret;
	}


	/*
	
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
		DataList getData(int col);
		double getData(int col, int row);

		// Setters
		void setDesc(std::string s);
		void setData(vDL& refMat);

		// Other
		std::string str();
		void addCol(DataList& refCol);
		void appendToCol(int index, double value);
		void changeColName(int index, std::string value);
		bool verifyDims();
		void setNames(std::vector<std::string> names);
		void init(int numCols);
		void addRow(vd row);
		int getColIndex(std::string colName);
		void replaceRow(int rowInd, vd theRow);
		std::vector<int> getSortedIndices(std::string colName, bool parFlag);

		// Function chaining operators

		// DataFrame <- DataFrame
		DataFrame operator+ (std::function<DataFrame(DataFrame&)> f) {
			return f(*this);
		}

		// DataList <- DataFrame
		DataList operator+ (std::function<DataList(DataFrame&)> f) {
			return f(*this);
		}

		// double <- DataFrame
		double operator+ (std::function<double(DataFrame&)> f) {
			return f(*this);
		}

	};
	*/
/*

	// DataFrame
	
	// String representation
	std::string DataFrame::str() {
		using namespace std;
		stringstream os;
		os << std::setprecision(5) << "[[BEGIN DataFrame]]" << "\n" << "Description: " << m_description << endl;
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

	// For now, assume that i is a safe index
	DataList DataFrame::getData(int col) {
		return m_data[col];
	}

	// For now, assume that i and j are safe indices
	double DataFrame::getData(int col, int row) {
		return m_data[col].vals[row];
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
	bool DataFrame::verifyDims(){
		auto fetchSize = [](DataList dl) {
			return dl.vals.size();
		};
		std::vector<size_t> sizeVec;
		std::transform(m_data.begin(), m_data.end(), std::back_inserter(sizeVec), fetchSize); // fetch all sizes
		if (std::adjacent_find(sizeVec.begin(), sizeVec.end(), std::not_equal_to<>()) == sizeVec.end())
			return true; // if all sizes are equal
		return false;
	}

	void DataFrame::addCol(DataList& refCol) {
		if (m_rows == 0 && m_columns == 0) { // if new
			m_columns = 1;
			m_rows = refCol.vals.size();
			m_data.emplace_back(refCol);
		}
		else if (refCol.vals.size() == m_rows) {
			m_data.emplace_back(refCol);
			m_columns += 1;
		}
		else {
			std::cout << "[DataFrame::addCol] <<ERROR>> Unable to add column to DataFrame" << std::endl;
		}
	}

	void DataFrame::appendToCol(int index, double value) {
		m_data[index].vals.push_back(value);
	}

	void DataFrame::changeColName(int index, std::string value) {
		m_data[index].name = value;
	}

	void DataFrame::setNames(std::vector<std::string> names) {
		assert(names.size() == m_columns);
		for (int i = 0; i < names.size(); i++) {
			m_data[i].name = names[i];
		}
	}

	void DataFrame::init(int numCols) {
		assert(m_columns == 0);
		for (int i = 0; i < numCols; i++) {
			vd tmp;
			DataList tmpDL(tmp, "");
			this->addCol(tmpDL);
		}
		m_columns = numCols;
	}

	void DataFrame::addRow(vd theRow) {
		assert(m_columns == theRow.size());
		for (int i = 0; i < m_columns; i++) {
			m_data[i].vals.push_back(theRow[i]);
		}
		m_rows += 1;
	}

	void DataFrame::replaceRow(int rowInd, vd theRow) {
		assert(m_columns == theRow.size());
		for (int i = 0; i < m_columns; i++) {
			m_data[i].vals[rowInd] = theRow[i];
		}
	}

	int DataFrame::getColIndex(std::string colName) {
		int ind;
		for (ind = 0; ind < m_columns; ind++) {
			if (m_data[ind].name == colName) {
				break;
			}
		}
		return ind;
	}

	// Sort the indices of a DataFrame by a certain Column
	std::vector<int> DataFrame::getSortedIndices(std::string colName, bool parFlag) {
		// create a list of indices
		std::vector<int> indices(m_rows);
		std::iota(indices.begin(), indices.end(), 0);
		auto sortLambda = [&](const int& a, const int& b)-> bool {
			// find the correct column index
			int ind = this->getColIndex(colName);
			return this->getData(ind, a) > this->getData(ind, b);
		};
		if (parFlag) {
			std::sort(std::execution::par, indices.begin(), indices.end(), sortLambda);
		}
		else {
			std::sort(indices.begin(), indices.end(), sortLambda);
		}
		return indices;
	}
*/

}
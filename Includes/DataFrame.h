#pragma once

// maybe make friend classes that operate on a DataFrame with a lambda and return a DataFrame

#include <string>
#include <vector>
#include <functional>
#include <numeric>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <execution>
#include <iomanip>

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

		// std::vector<double> <- DataList
		std::vector<double> operator+ (std::function<std::vector<double>(DataList&)> f) {
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

	// Class for working with a group of DataFrames
	class DataFrameList {
	private:	
		std::vector<DataFrame> frames;

	public:
		// Getters
		int size() { return frames.size(); }
		DataFrame getFrame(int frameIndex);
		DataFrame getFrame(std::string frameDesc);

		// Other
		void addFrame(DataFrame &df);
		void loadFramesFromDir(std::string path, 
			std::string exclude, std::string delim);

		// Operator overloads

		// DataFrameList <- DataFrameList
		DataFrameList operator+ (std::function<DataFrameList(DataFrameList&)> f) {
			return f(*this);
		}

	};

	DataFrame DataFrameList::getFrame(int frameIndex) {
		return frames.at(frameIndex);
	}

	DataFrame DataFrameList::getFrame(std::string frameDesc) {
		for(auto df : frames) {
			if(frameDesc.compare(df.getDesc()) == 0) {
				return df;
			}
		}
		std::cout << "getFrame() returning empty DF" << std::endl;
		DataFrame empty;
		return empty;
	}


}
#pragma once

// maybe make friend classes that operate on a DataFrame with a lambda and return a DataFrame

#include "TypesADSL.h"
#include "TA_includes.h"

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
#include <cassert>
#include <variant>
#include <type_traits>

namespace adsl {

	// for alternative typing system
	int TINT = 0;
	double TDBL = 0.0;
	std::string TSTR = "";

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

		// Length
		int length() { return vals.size(); }

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

		// Moving Average
		DataList calcMA(std::string maType, int period);

		// Basic STL vector methods

		// .at() method
		// **EXPERIMENTAL FUNCTION**
		// This function experiments with a new typing mechanism
		// someDataList.at(2, TDBL), etc
		template<typename T1, typename T2>
		T2 at(T1 index, T2 dummyVal) {
			if(std::is_same_v<T2, int>) {
				if (this->type != INT) {
					std::cout << "<<ERROR>> [at] mismatched types" << std::endl;
					exit(1);
				}
				int ret = this->vals.at(index).intU;
				return *(T2*)(void*)(&ret);
			}
			else if (std::is_same_v<T2, double>) {
				if (this->type != DBL) {
					std::cout << "<<ERROR>> [at] mismatched types" << std::endl;
					exit(1);
				}
				double ret = this->vals.at(index).doubleU;
				return *(T2*)(void*)(&ret);
			} else { // STR
				if (this->type != STR) {
					std::cout << "<<ERROR>> [at] mismatched types" << std::endl;
					exit(1);
				}
				std::string ret = this->vals.at(index).strU;
				return *(T2*)(void*)(&ret);
			}
		}

		// .at() methods
		// Cover up the experimental typing above
		int at_int(int i) {return at(i, TINT);}
		double at_dbl(int i) {return at(i, TDBL);}
		std::string at_str(int i) {return at(i, TSTR);}

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

		// Basic STL vector methods
		// To make development easy, implementation is here instead of outside
		// auto at(int index) -> std::variant<int, double, std::string> {
		// 	if (this->type == DataType::INT) {
		// 		return this->vals.at(index).intU;
		// 	} else if (this->type == DataType::DBL) {
		// 		return this->vals.at(index).doubleU;
		// 	} else { // DataType::STR
		// 		return this->vals.at(index).strU;
		// 	}
		// }

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

	// Moving Average
	// The DataList must have double values
	DataList DataList::calcMA(std::string maType, int period) {
		if (this->type != DataType::DBL) {
			std::cout << "<<ERROR>> [calcMA] The DataList.type field must be DataType::DBL" << std::endl;;
			exit(1);
		}
		double *dataVals = (double*) malloc(sizeof(double) * this->vals.size());
		for (int i=0; i < this->vals.size(); i++) {
			dataVals[i] = this->vals.at(i).doubleU;
		}
		double *out = (double*) malloc(sizeof(double) * this->vals.size());
		int outBeg;
		int outNbElement;

		if (maType.compare("SMA") == 0) {
			TA_RetCode retCode = TA_MA( 0, this->vals.size() - 1,
				dataVals,
				period, TA_MAType_SMA,
				&outBeg, &outNbElement, &out[0] );
		} else if (maType.compare("EMA") == 0) {
			TA_RetCode retCode = TA_MA( 0, this->vals.size() - 1,
				dataVals,
				period, TA_MAType_EMA,
				&outBeg, &outNbElement, &out[0] );
		} else {
			std::cout << "[calcMA] <<WARNING>> unrecognized moving average type, defaulting to TA_MAType_SMA" << std::endl;
			TA_RetCode retCode = TA_MA( 0, this->vals.size() - 1,
				dataVals,
				period, TA_MAType_SMA,
				&outBeg, &outNbElement, &out[0] );
			maType = "SMA";
		}
		
		vd outVec;
		for( int i=0; i < outNbElement; i++ ) {
			//printf( "Day %d = %f\n", outBeg+i, out[i] );
			outVec.push_back(out[i]);
		}

		DataList ret(&outVec, DataType::DBL, maType + " " + (this->name));

		return ret;
		
	}

	// STL vector .at()
	// int DataList::at_int(int index) {
	// 	return this->vals.at(index).intU;
	// }
	// double DataList::at_dbl(int index) {
	// 	return this->vals.at(index).doubleU;
	// }
	// std::string DataList::at_str(int index) {
	// 	return this->vals.at(index).strU;
	// }
	// auto DataList::at(int index) {
	// 	if (this->type == DataType::INT) {
	// 		return this->vals.at(index).intU;
	// 	} else if (this->type == DataType::DBL) {
	// 		return this->vals.at(index).doubleU;
	// 	} else if (this->type == DataType::STR) {
	// 		return this->vals.at(index).intU;
	// 	} else {
	// 		std::cout << "<<ERROR>> [at] DataList does not have type INT, DBL, or STR" << std::endl;
	// 		exit(1);
	// 	}
	// }


	// DataFrame
	
	
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
		int getData_int(int col, int row);
		double getData_dbl(int col, int row);
		std::string getData_str(int col, int row);

		// Setters
		void setDesc(std::string s);
		void setData_dbl(vDL& refMat);

		// Other
		std::string str();
		void addCol(DataList& refCol);
		//void appendToCol(int index, double value);
		void appendToCol(int index, void* value, DataType type);
		void changeColName(int index, std::string value);
		bool verifyDims();
		void setNames(std::vector<std::string> names);
		void init(int numCols, DataType type);
		void addRow_dbl(vd row);
		int getColIndex(std::string colName);
		void replaceRow_dbl(int rowInd, vd theRow);
		std::vector<int> getSortedIndices_dbl(std::string colName, bool parFlag);

		DataFrame select(std::vector<std::string> nameVec);
		DataFrame deselect(std::vector<std::string> nameVec);
		DataFrame combineV(DataFrame& df); 

		// Filter
		// Up to the user to make sure the predicate casts appropriately inside
			// ex: [](void* x) {double a = *(double*)x; ...
		DataFrame filter_rows(int col_ind, std::function<bool(void*)> predicate);

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

		// string <- DataFrame
		std::string operator+ (std::function<std::string(DataFrame&)> f) {
			return f(*this);
		}

	};
	
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
		if (col > m_data.size()-1) {
			std::cout << "<<ERROR>> [getData] Column index out of bounds" << std::endl;
			exit(1);
		}
		return m_data[col];
	}

	int DataFrame::getData_int(int col, int row) {
		if (col > m_data.size()-1) {
			std::cout << "<<ERROR>> [getData_int] Column index out of bounds" << std::endl;
			exit(1);
		}
		DataType colType = m_data[col].type;
		if (colType != INT) {
			std::cout << "<<ERROR>> [getData_int] Column type should be INT" << std::endl;
			exit(1);
		}
		//return m_data[col].vals[row];
		return m_data[col].getVal_int(row);	
	}
	double DataFrame::getData_dbl(int col, int row) {
		if (col > m_data.size()-1) {
			std::cout << "<<ERROR>> [getData_dbl] Column index out of bounds" << std::endl;
			exit(1);
		}
		DataType colType = m_data[col].type;
		if (colType != DBL) {
			std::cout << "<<ERROR>> [getData_dbl] Column type should be DBL" << std::endl;
			exit(1);
		}
		//return m_data[col].vals[row];
		return m_data[col].getVal_dbl(row);	
	}
	std::string DataFrame::getData_str(int col, int row) {
		if (col > m_data.size()-1) {
			std::cout << "<<ERROR>> [getData_str] Column index out of bounds" << std::endl;
			exit(1);
		}
		DataType colType = m_data[col].type;
		if (colType != STR) {
			std::cout << "<<ERROR>> [getData_str] Column type should be STR" << std::endl;
			exit(1);
		}
		//return m_data[col].vals[row];
		return m_data[col].getVal_str(row);	
	}


	// Setters
	void DataFrame::setDesc(std::string s) {
		m_description = s;
	}

	void DataFrame::setData_dbl(vDL& matRef) {
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

	// void DataFrame::appendToCol(int index, double value) {
	// 	m_data[index].vals.push_back(value);
	// }
	void DataFrame::appendToCol(int index, void* value, DataType type) {
		if (index > m_data.size()-1) {
			std::cout << "<<ERROR>> [appendToCol] Column index out of bounds" << std::endl;
			exit(1);
		}

		if (type == INT && type == m_data[index].type) {
			DataEntry tmp;
			tmp.intU = * (int *) value;
			m_data[index].vals.push_back(tmp);
		} else if (type == DBL && type == m_data[index].type) {
			DataEntry tmp;
			tmp.doubleU = * (double *) value;
			m_data[index].vals.push_back(tmp);
		} else if (type == STR && type == m_data[index].type) {
			std::string tmpStr = * (std::string *) value;
			DataEntry tmp;
			tmp.strU = (char *) tmpStr.c_str();
			m_data[index].vals.push_back(tmp);
		} else {
			std::cout << "<<ERROR>> [appendToCol] Typing mismatch" << std::endl;
			exit(1);
		}
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

	void DataFrame::init(int numCols, DataType type) {
		assert(m_columns == 0);
		for (int i = 0; i < numCols; i++) {
			if (type == INT) {
				std::vector<int> tmp;
				DataList tmpDL(&tmp, DataType::INT, "");
				this->addCol(tmpDL);
			} else if (type == DBL) {
				std::vector<double> tmp;
				DataList tmpDL(&tmp, DataType::DBL, "");
				this->addCol(tmpDL);
			} else { //str
				std::vector<std::string> tmp;
				DataList tmpDL(&tmp, DataType::STR, "");
				this->addCol(tmpDL);
			}
		}
		m_columns = numCols;
	}

	void DataFrame::addRow_dbl(vd theRow) {
		assert(m_columns == theRow.size());
		for (int i = 0; i < m_columns; i++) {
			DataEntry tmpDataEntry;
			tmpDataEntry.doubleU = theRow[i];
			m_data[i].vals.push_back(tmpDataEntry);
		}
		m_rows += 1;
	}

	void DataFrame::replaceRow_dbl(int rowInd, vd theRow) {
		assert(m_columns == theRow.size());
		for (int i = 0; i < m_columns; i++) {
			m_data[i].vals[rowInd].doubleU = theRow[i];
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
	std::vector<int> DataFrame::getSortedIndices_dbl(std::string colName, bool parFlag) {
		// create a list of indices
		std::vector<int> indices(m_rows);
		std::iota(indices.begin(), indices.end(), 0);
		auto sortLambda = [&](const int& a, const int& b)-> bool {
			// find the correct column index
			int ind = this->getColIndex(colName);
			return this->getData_dbl(ind, a) > this->getData_dbl(ind, b);
		};
		if (parFlag) {
			std::sort(std::execution::par, indices.begin(), indices.end(), sortLambda);
		}
		else {
			std::sort(indices.begin(), indices.end(), sortLambda);
		}
		return indices;
	}

	DataFrame DataFrame::select(std::vector<std::string> nameVec) {
		DataFrame ret;
		ret.m_description = m_description;
		for (DataList dl : m_data) {
			if (std::find(nameVec.begin(), nameVec.end(), dl.name) != nameVec.end())
				ret.addCol(dl);
		}
		return ret;
	}

	DataFrame DataFrame::deselect(std::vector<std::string> nameVec) {
		DataFrame ret;
		ret.m_description = m_description;
		for (DataList dl : m_data) {
			if (std::find(nameVec.begin(), nameVec.end(), dl.name) == nameVec.end())
				ret.addCol(dl);
		}
		return ret;
	}

	DataFrame DataFrame::combineV(DataFrame& df) {
		DataFrame ret;
		if (m_columns == df.m_columns) {
			for (int i = 0; i < m_columns; i++) {
				DataList theCol = DataList::combineDLs(m_data[i], df.m_data[i]);
				ret.addCol( theCol );
			}
			if (!ret.verifyDims()) {
				std::cout << "[combineV] <<WARNING>> Dimension verification failed" << std::endl;
			}
			ret.setDesc(m_description);
			return ret;
		}
		else {
			std::cout << "[combineV] <<ERROR>> Unequal column lengths" << std::endl;
			exit(1);
			return ret;
		}
	}

    DataFrame DataFrame::filter_rows(int col_ind, std::function<bool(void*)> predicate)
    {

		DataFrame ret; // append row to this df if predicate is true for the given column

		ret = *this; //copy

		for (auto &col : ret.m_data) {
			col.vals.clear();
		}

		auto col_type = m_data[col_ind].type;

		if (col_type == adsl::DBL) {
			for (int i=0; i < m_data[col_ind].length(); i++) {
				double tmp = m_data[col_ind].getVal_dbl(i);
				if (predicate(&tmp)) { // good row
					// loop over columns in this row
					for (int j=0; j < m_data.size(); j++) {
						// append appropriate values
						ret.m_data[j].vals.push_back(m_data[j].vals[i]); // DataEntry
					}
				}
			}
		}

		if (col_type == adsl::INT) {
			for (int i=0; i < m_data[col_ind].length(); i++) {
				int tmp = m_data[col_ind].getVal_dbl(i);
				if (predicate(&tmp)) { // good row
					// loop over columns in this row
					for (int j=0; j < m_data.size(); j++) {
						// append appropriate values
						ret.m_data[j].vals.push_back(m_data[j].vals[i]); // DataEntry
					}
				}
			}
		}

		if (col_type == adsl::STR) {
			for (int i=0; i < m_data[col_ind].length(); i++) {
				std::string tmp = m_data[col_ind].getVal_str(i);
				if (predicate(&tmp)) { // good row
					// loop over columns in this row
					for (int j=0; j < m_data.size(); j++) {
						// append appropriate values
						ret.m_data[j].vals.push_back(m_data[j].vals[i]); // DataEntry
					}
				}
			}
		}

        return ret;
    }

}
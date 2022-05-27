#pragma once

#include "DataFrame.h"
#include <sstream>
#include <fstream>
#include <iomanip>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace boost::gregorian;

namespace adsl {

	// adapted from https://www.techiedelight.com/split-string-cpp-using-delimiter/
	// changed const char to const char* and delim to *delim
	void tokenize(std::string const& str, const char* delim,
		std::vector<std::string>& out)
	{
		// construct a stream from the string
		std::stringstream ss(str);

		std::string s;
		while (std::getline(ss, s, *delim)) {
			out.push_back(s);
		}
	}

	// Loads a DataFrame from a CSV file with a SINGLE time-series column
	// TODO: Fix breaking on processing empty lines
	// TODO: Read in a description after a # symbol
	// Adapted from https://stackoverflow.com/questions/19936483/c-reading-csv-file
	DataFrame loadFromCSV_wDate_dbl(std::string filename, std::string delim, 
	bool header, int dateColInd, int dateFormat) 
	{
		std::ifstream ifs;
		ifs.open(filename, std::ifstream::in);

		DataFrame df;

		bool gotNumCols = false;
		int numCols = 0;
		std::string line;
		int lineCounter = 0;
		while (std::getline(ifs, line)) {
			std::vector<std::string> lineVec;
			tokenize(line, delim.c_str(), lineVec);

			// Get the number of columns and initialize
			if (!gotNumCols) {
				numCols = lineVec.size();
				gotNumCols = true;
				for (int i = 0; i < numCols; i++) {
					vd tmpvd;
					DataList tmpDL(&tmpvd, DataType::DBL, "");
					df.addCol(tmpDL);
				}
			}

			if (header && lineCounter == 0) {
				for (int i = 0; i < numCols; i++) {
					df.changeColName(i, lineVec[i]);
				}
			}
			else {
				for (int i = 0; i < numCols; i++) {
					if (i == dateColInd) {
						if (dateFormat == 0) { // uk
							date boostDate = from_uk_string(lineVec[i]);
							double dblDayNum = (double) boostDate.day_number();
							df.appendToCol(i, &dblDayNum, DataType::DBL);
						} else if (dateFormat == 1) { // us
							date boostDate = from_us_string(lineVec[i]);
							double dblDayNum = (double) boostDate.day_number();
							df.appendToCol(i, &dblDayNum, DataType::DBL);
						} else {
							date boostDate = from_string(lineVec[i]);
							double dblDayNum = (double) boostDate.day_number();
							df.appendToCol(i, &dblDayNum, DataType::DBL);
						}
					}
					else { 
						std::string tmpLineItem = lineVec[i];
						try {
							double tmpVal = std::stod(lineVec[i]);
							df.appendToCol(i, &tmpVal, DataType::DBL);
						} catch(...) {
							std::cout << "error with stod: " << tmpLineItem << std::endl;
						}
					}
				}
			}

			lineCounter += 1;
		}
		if (df.verifyDims()) {
			return df;
		}
		else {
			std::cout << "[loadFromCSV] <<ERROR>> DataFrame has mismatched dimensions." << std::endl;
			DataFrame empty_df;
			return empty_df;
		}
	}

	// Loads a DataFrame from a CSV file
	// TODO: Fix breaking on processing empty lines
	// TODO: Read in a description after a # symbol
	// Adapted from https://stackoverflow.com/questions/19936483/c-reading-csv-file
	DataFrame loadFromCSV_dbl(std::string filename, std::string delim, bool header) {
		std::ifstream ifs;
		ifs.open(filename, std::ifstream::in);

		DataFrame df;

		bool gotNumCols = false;
		int numCols = 0;
		std::string line;
		int lineCounter = 0;
		while (std::getline(ifs, line)) {
			std::vector<std::string> lineVec;
			tokenize(line, delim.c_str(), lineVec);

			// Get the number of columns and initialize
			if (!gotNumCols) {
				numCols = lineVec.size();
				gotNumCols = true;
				for (int i = 0; i < numCols; i++) {
					vd tmpvd;
					DataList tmpDL(&tmpvd, DataType::DBL, "");
					df.addCol(tmpDL);
				}
			}

			if (header && lineCounter == 0) {
				for (int i = 0; i < numCols; i++) {
					df.changeColName(i, lineVec[i]);
				}
			}
			else {
				for (int i = 0; i < numCols; i++) {
					double tmpVal = std::stod(lineVec[i]);
					df.appendToCol(i, &tmpVal, DataType::DBL);
				}
			}

			lineCounter += 1;
		}
		if (df.verifyDims()) {
			return df;
		}
		else {
			std::cout << "[loadFromCSV] <<ERROR>> DataFrame has mismatched dimensions." << std::endl;
			DataFrame empty_df;
			return empty_df;
		}
	}

	// Write a DataFrame to a CSV
	// Use # for the description so gnuplot skips it
	void writeToCSV_dbl(DataFrame& df, std::string filename) {
		std::ofstream handle;
		handle.open(filename);
		auto theData = df.getData();
		handle << "# " << df.getDesc() << "\n";
		for (size_t i = 0; i < theData[0].vals.size(); i++) { // rows
			for (size_t j = 0; j < theData.size() - 1; j++) { // columns - 1
				//handle << theData[j].vals[i] << ",";
				handle << std::fixed << std::setprecision(15) << theData[j].getVal_dbl(i) << " ";
			}
			// last entry on line needs a \n not a ,
			handle << theData[theData.size() - 1].getVal_dbl(i) << "\n";
		}
		handle.close();
	}



	/*template <class T>
	bool verifyDims(T& matRef) {
		if (matRef.size() == 0) {
			return false;
		}
		else {
			int theLen = matRef[0].size();
			for (auto col : matRef) {
				if (col.size() != theLen) {
					return false;
				}
			}
		}
		return true;
	}*/
}
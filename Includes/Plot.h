#pragma once
// Set of lambdas for plotting dfs with Gnuplot
// Create gnuplot script as a string, then run string
// See https://alvinalexander.com/technology/gnuplot-charts-graphs-examples/ for more

#include <iostream>
#include <sstream>
#include <cstdlib>
#include "Util.h"

namespace adsl {

	// Plot a 2-column DataFrame
	// DataFrame (empty) <- DataFrame <- vector<string>
	// options should be of the form: {outFileName, resolution};
	auto scatter2D = [](std::vector<std::string> options) {
		auto retFunc = [options](DataFrame& df) {
			DataFrame tmp;
			if (df.getCols() != 2) {
				std::cout << "<<ERROR>>[scatter2D] This function expects 2 columns, but recieved " << df.getCols() << " columns instead" << std::endl;
				return tmp;
			}
			
			// Write to csv file
			writeToCSV(df, "Outputs/scatter2D.dat");

			// create a temporary gnuplot script and then run it
			std::stringstream plotStream;
			plotStream << "set terminal png size " << options[1] << "\n"; 
			plotStream << "set output '" << options[0] << "'\n";
			plotStream << "set title " << "'" << df.getDesc() << "' \n";
			plotStream << "set xlabel " << "'" << df.getData()[0].name << "' \n";
			plotStream << "set ylabel " << "'" << df.getData()[1].name << "' \n";
			plotStream << "set title " << "'" << df.getDesc() << "' \n";
			plotStream << "plot 'Outputs/scatter2D.dat' using 1:2 notitle pt 7 ps 2";

			std::stringstream cmdStream;
			cmdStream << "echo \"" << plotStream.str() << "\" > Outputs/scatter2d.p && gnuplot Outputs/scatter2d.p";

			std::cout << "Executing: " << cmdStream.str().c_str() << std::endl;

			std::system(cmdStream.str().c_str());

			// temporary fix; void wasn't working for some reason
			return tmp;
		};
		return retFunc;
	};

	// TODO
	// Plot a 2D scatterplot from a file using the scatter2D lambda internally
	void _scatter2D(std::string filename) {

	}

	// TODO
	// Plot a 2D scatterplot from a file using the scatter2D lambda internally, specifying column names
	void _scatter2D(std::string filename, std::vector<std::string> colNames) {

	}

}
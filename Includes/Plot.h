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
	// EMPTY DataFrame <- DataFrame
	// TODO: Fix extra point being added??
	// TODO: Fix scaling problem for 4k screen
	auto scatter2D = [](DataFrame& df) {
		// TODO: Verify 2 columns
		
		// Write to csv file
		writeToCSV(df, "scatter2D.dat");

		// plot from csv file
		std::stringstream cmdStream;
		cmdStream << "START /B gnuplot -e " << "\"" << "plot 'scatter2D.dat' using 1:2; pause -1" << "\"";
		std::system(cmdStream.str().c_str());

		// temporary fix; void wasn't working for some reason
		DataFrame tmp;
		return tmp;
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
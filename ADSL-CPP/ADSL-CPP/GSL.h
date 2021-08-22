#pragma once
#include <gsl/gsl_fit.h>
#include <gsl/gsl_statistics.h>
#include <cstdlib>
#include "DataFrame.h"

namespace adsl {

	// Evaluate the result of a linear fit at a given point based on the DataFrame description
	// TODO

	// Perform a linear fit using gsl_fit_linear
	// df must have only 2 columns
	// DataFrame <- DataFrame
	auto fitLinear = [](DataFrame& df) {
		if (df.getCols() == 2) {
			DataFrame ret;
			auto xData = df.getData()[0].vals;
			auto yData = df.getData()[1].vals;
			size_t length = df.getData()[0].vals.size();
			//double* x = &df.getData()[0].vals[0]; // https://stackoverflow.com/questions/2923272/how-to-convert-vector-to-array
			//double* y = &df.getData()[1].vals[0];
			double* x;
			double* y;
			x = (double*)malloc(length * sizeof(double));
			y = (double*)malloc(length * sizeof(double));
			std::copy(xData.begin(), xData.end(), x);
			std::copy(yData.begin(), yData.end(), y);
			double c0, c1, cov00, cov01, cov11, sumsq;
			gsl_fit_linear(x, 1, y, 1, length, &c0, &c1, &cov00, &cov01, &cov11, &sumsq);
			DataList slope({ c1 }, "slope");
			DataList intercept({ c0 }, "intercept");
			DataList rsqrd({ gsl_stats_correlation(x, 1, y, 1, length) }, "r-squared");
			free(x);
			free(y);
			ret.addCol(slope);
			ret.addCol(intercept);
			ret.addCol(rsqrd);
			ret.setDesc("gsl_fit_linear");
			return ret;
		}
		else {
			std::cout << "[fitLinear] <<ERROR>> DataFrame doesn't have two columns; has " << df.getCols() << std::endl;
			DataFrame empty_df;
			return empty_df;
		}
	};

 }
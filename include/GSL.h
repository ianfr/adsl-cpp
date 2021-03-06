#pragma once
#include <gsl/gsl_fit.h>
#include <gsl/gsl_statistics.h>
#include <gsl/gsl_movstat.h>
#include <gsl/gsl_vector.h>
#include <cstdlib>
#include "DataFrame.h"
#include "Ops.h"

namespace adsl {

	// Evaluate the result of a fit at a given point based on the DataFrame description
	// double <- DataFrame
	/*
	auto evalFit = [](double t) {
		auto retFunc = [t](DataFrame& df) {
			if (df.getDesc() == "gsl_fit_linear") {
				double m = df + select({ "slope" }) + getFirst + single();
				double b = df + select({ "intercept" }) + getFirst + single();
				return (m * t) + b;
			}
			else {
				return (double)NAN;
			}
		};
		return retFunc;
	};
	*/

	// Perform a linear fit using gsl_fit_linear
	// df must have only 2 columns
	// DataFrame <- DataFrame
	auto fitLinear = [](DataFrame& df) {
		if (df.getCols() == 2) {
			if (df.getData()[0].type != DBL || df.getData()[1].type != DBL) {
				std::cout << "[fitLinear] <<ERROR>> At least one of the columns is not of type DBL " << std::endl;
				exit(1);
				DataFrame empty_df;
				return empty_df;
			}
			// Setup
			DataFrame ret;
			auto xData = df.getData()[0].toVec_dbl();
			auto yData = df.getData()[1].toVec_dbl();
			size_t length = xData.size();
			double* x;
			double* y;
			x = (double*)malloc(length * sizeof(double));
			y = (double*)malloc(length * sizeof(double));
			std::copy(xData.begin(), xData.end(), x);
			std::copy(yData.begin(), yData.end(), y);

			// Fitting & r-squared
			double c0, c1, cov00, cov01, cov11, sumsq;
			gsl_fit_linear(x, 1, y, 1, length, &c0, &c1, &cov00, &cov01, &cov11, &sumsq);
			vd tmpSlope = {c1};
			DataList slope(&tmpSlope, DataType::DBL, "slope");
			vd tmpIntercept = {c0};
			DataList intercept(&tmpIntercept, DataType::DBL, "intercept");
			vd tmpRsqrd = { gsl_stats_correlation(x, 1, y, 1, length) };
			DataList rsqrd(&tmpRsqrd, DataType::DBL, "r-squared");

			// Clean up
			free(x);
			free(y);

			// Return
			ret.addCol(slope);
			ret.addCol(intercept);
			ret.addCol(rsqrd);
			ret.setDesc("gsl_fit_linear");
			return ret;
		}
		else {
			std::cout << "[fitLinear] <<ERROR>> DataFrame doesn't have two columns; has " << df.getCols() << std::endl;
			exit(1);
			DataFrame empty_df;
			return empty_df;
		}
	};

	// TODO: add more fit types

	// Calculate the moving average of a DataList
	// DataList <- DataList <- double
	/*
	auto SMA = [](double period) {
		auto retFunc = [period](DataList& dl) {
			DataFrame ret;
			double* input;
			double* output;
			// TODO
		};
		return retFunc;
	};
	*/


 }
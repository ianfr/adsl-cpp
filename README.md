# adsl-cpp: A Data Science Library for C++
![logo](logo.png)
## Description
The ADSL C++ project is a library for performant and straightforward data science. ADSL includes:

* A library for manipulating data
* Machine learning routines _(via Dlib)_
* Time-series forecasting _(via ctsa)_
* Statistical routines _(via GSL)_
* Scatterplots and other data visualizations _(via Gnuplot)_

At ADSL's core is the `DataFrame` class to work with data sets. `DataFrame` is inspired by R's & Pandas' dataframes and can import data from a CSV file.

A cornerstone of the project is the ability to "chain together" operations on `DataFrame`s (and the `DataList`s within them) with the `+` pipe operator. Data is passed along the chain of commands from left to right, enabling incredibly expressive software development. This is similar to R's pipe operator `%>%` or the UNIX-style pipe `|`.

Please note that ADSL is for datasets of floating-point values. When ML is added, appropriate conversion functions for other 'discrete' types like categorical labels will be supplied as necessary.

Also, **this library is in an early stage of development.**

## Files
* ADSL.h - includes everything else
* DataFrame.h - class definitions for `DataFrame` and `DataList`
* Ops.h - basic operations on `DataLists` and `DataFrames`
* STL.h - wrappers around STL functions like `accumulate` and `transform`
* GSL.h - wrappers around GSL functions like `gsl_fit_linear`
* Plot.h - functions for plotting with Gnuplot
* Util.h - utility functions like those for reading in data from a file

## Examples

Gather and print all values in a `DataList` greater than 2 using a custom filter and print it:
```c
// assume 'dL' is a DataList defined previously...
auto cond = [](double x) {
  return x > 2;
};
DataList filtered = dL + adsl::filter(cond);
cout << filtered.str();
```
Perform a map-reduce to display the sum of squares of a `DataList`:
```c
// assume 'dL' is a DataList defined previously...
auto squareX = [](double x) {
        return x * x;
};
cout << dL + adsl::transform(squareX) + adsl::sum;
```
Display a scatterplot using Gnuplot behind the scenes:
```c
adsl::vd tmp0 = { 1, 2, 3, 4 };
adsl::vd tmp1 = { 1, 2, 3, 4 };
DataList lst0 (tmp0, "DL 0"); // (data, name)
DataList lst1 (tmp1, "DL 1");
DataFrame df2;
df2.addCol(lst0);
df2.addCol(lst1);
df2.setDesc("Testing plotting 2D"); // DataFrame description
df2 + adsl::scatter2D; // Plot
```
Perform linear regression and print the value of the fitted function at an arbitrary point:
```cpp
// Assume 'df' is a 2-column DataFrame defined previously...
DataFrame linFit = df + adsl::fitLinear;
// linFit contains coeffs & r-squared
cout << "Prediction at x=5: " << linFit + adsl::evalFit(5);
```
Train a binary classifier to predict flower species based on 4 physical measurements [WIP]:
```cpp
DataFrame irisData = adsl::loadFromCSV("Datasets/iris.csv", ",", true);
vector<double> labels = irisData + adsl::select({ "class" }) + adsl::getFirst + adsl::toVec;
DataFrame feats = irisData + adsl::deselect({ "class" });
DataFrame classResult = feats + adsl::binaryClassifier( labels );
```

## Writing your own ADSL chainable functions

Writing a chainable function is incredibly simple: any function returning a `DataList` or `DataFrame` and taking as input a `DataList` or `DataFrame` is chainable. (It's also possible to return `double` or `std::string`, but these kinds of functions can only be used at the end of a chain). 

This might seem limited, but with the power of C++ lambdas you can actually _pass in any kind of input of any type_ you want using a trick called **currying**.

To see an example of currying, take a look at the source for the function `adsl::filterLess(d)` (a function that filters out any values in a `DataList` not less than the input `d`):
```cpp
// Less than filter
// DataList <- DataList <- double
auto filterLess = [](double d) {
  auto retFunc = [d](DataList& dl) {
    DataList ret;
    ret.name = dl.name;
    for (double val : dl.vals) {
      if (val < d)
        ret.vals.emplace_back(val);
    }
    return ret;
  };
  return retFunc;
};
```

In the example above, we "wrap" a chainable lambda function operating on a `DataList` and returning a `DataList` with another lambda function that takes in a double. The variable `d` is _captured_ by the inner lambda inside of the brackets `[ ]`.

The trick here is that the outer function `filterLess` actually creates and returns the inner function `retFunc` on the spot when called. That's right, a function returning another function!

Thus, the user can supply a custom input `d` and the `+` pipe operator will only be exposed to a valid chainable function. 

In your extensions, the outer lambda can take in any number of arguments - just make sure to capture them inside the inner lambda's brackets `[ ]`. Outer arguments that are `DataFrame`s should be captured by reference in the inner lambda with a `&` like `[&df]`.

## Installation

Development and testing is happening in Ubuntu 20.04 using the Windows Subsystem for Linux and `g++` version 9.

The following dependencies are required to be installed (I used the awesome [vcpkg](https://vcpkg.io/en/getting-started.html) package manager, and assume that the vcpkg repo is located in `$HOME/DEV/`):
* The GNU Scientific Library (GSL): `./vcpkg install gsl`. [GSL](https://www.gnu.org/software/gsl/#subjects) is a C library with a large collection of numerical routines for everything from least-squares to simulated annealing.
* Dlib: `./vcpkg install dlib`. [Dlib](http://dlib.net/ml.html) is a C++ library containing various numerical routines including many for ML.
* Boost (date-time): `./vcpkg install boost-date-time`. Boost's Date & Time library provides functionality for working with dates, times, and strings.

The following dependencies are required to be installed manually:
* Gnuplot with `sudo apt install gnuplot`
* Gfortran with `sudo apt install gfortran`
* Intel TBB with `sudo apt install libtbb-dev`

Note that a compressed copy of the ctsa repo is included within ADSL and has to be uncompressed, placed in a folder called *ctsa*, and then built so that the *compile* Bash script can link against ctsa. To build ctsa, `cd` into *ctsa* and do `cmake .` followed by running `make`.

## Usage

To get started with ADSL, download the repo and stick it inside of your project. Then add `#include "adsl-cpp/Includes/ADSL.h"`to your code and install the dependencies as described above.

The example program *ADSL-CPP.cpp* can be compiled with the _compile_ Bash script. Any program you write can also be compiled by setting the appropriate environment variable in the *compile* script.

Since data is loaded from disk every time you compile your code, for larger data sets it is  reccomended that you take advantage of the *ramdisk* Bash script, which essentially creates a folder in RAM that you should move your dataset(s) to with `cp FILE_NAME /mnt/adsl_ramdisk/FILE_NAME`. This option only makes sense if you have a large amount of RAM, however, because the dataset is still copied by ADSL into a DataFrame object.


## Work in Progress:
* An in-memory database like what R has
* Adding basic plotting capability built around Gnuplot
* Provide chainable wrappers around STL functions like accumulate, sort, etc
* Porting statistical functions from GSL
* Incorporate Machine Learning with Dlib

## Future Work:
* `DataFrameList` class for working with lists of DataFrames?

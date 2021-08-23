# adsl-cpp: A Data Science Library for C++
![logo](logo_bigger.png)
## Description
The ADSL C++17 project is a library intended for fast, accurate, idiomatic data processing. Integration with well-established visualization, numerical, and ML libraries is being implemented.

At ADSL's core is the `DataFrame` class to work with data sets. `DataFrame` is inspired by R's & Pandas' dataframes and can import data from a CSV file.

A cornerstone of the project is the ability to "chain together" operations on `DataFrame`s (and the `DataList`s within them) with the `+` pipe operator. This is similar to R's pipe operator `%>%` or the UNIX-style pipe `|`.

**Note:** ADSL is for datasets of floating-point values. When ML is added, appropriate conversion functions for other 'discrete' types will be supplied.

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
Increment the first column of a `DataFrame` twice:
```c
// assume 'df' is a DataFrame defined previously...
df = df + adsl::getFirst + adsl::inc + adsl::inc
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

Right now development is happening in Visual Studio. The library _is_ header-only, so it should compile with any modern C++ compiler on Windows, MacOS, Linux, or UNIX. 

All you need to do to get started with ADSL is `#include "ADSL.h"` and install the dependencies below:

The following dependencies are required to be installed with the free [vcpkg](https://vcpkg.io/en/getting-started.html) package manager:
* The GNU Scientific Library (GSL): `.\vcpkg install gsl gsl:x64-windows`. [GSL](https://www.gnu.org/software/gsl/#subjects) is a C library with a large collection of numerical routines for everything from least-squares to simulated annealing.
* [NOT YET] Dlib: `.\vcpkg install dlib dlib:x64-windows`. [Dlib](http://dlib.net/ml.html) is a C++ library containing various numerical routines including many for ML.
* [NOT YET] TensorFlow: `.\vcpkg install tensorflow-cc tensorflow-cc:x64-windows`. TensorFlow is the famous ML library from Google.

The following dependencies are required to be installed manually:
* [Gnuplot (mingw version)](https://sourceforge.net/projects/gnuplot/files/gnuplot/5.4.2/). NOTE: make sure to check the box to add gnuplot to the PATH during installation.

## Planned Work:
* [STARTED] Add basic plotting capability built around Gnuplot
* [DONE] Create a function for loading in data from a CSV file
* Create a class DataFrameList for working with a vector of DataFrames
* [STARTED] Include a library of functions which are wrappers around STL functions like accumulate, sort, etc
* [STARTED] Include a library of basic statistical functions
* Include a library for working with financial data

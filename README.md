# adsl-cpp: A Data Science Library for C++
## Description
The **adsl** C++17 project is a library intended for fast, accurate, idiomatic data processing. Integration with well-established visualization, numerical, and ML libraries is being implemented.

**adsl** is built specifically to work with datasets of floating-point values (so things like dates should be mapped to `double`s by the user beforehand).

There are two main classes in **adsl**:
* DataList - A thin wrapper around std::vector\<double\>
* DataFrame - A thin wrapper around std::vector\<DataList\>

A cornerstone of the project is the ability to chain together operations on DataFrames and DataLists using operator overloading and currying with lambdas. Anyone familiar with the R pipe operator %>% or UNIX-style pipe operator | will feel at home. An example is below to gather and print all values in a DataList greater than 2:
```c
// assume 'first' is a DataList defined previously...
auto cond = [](double x) {
  return x > 2;
};
DataList filtered = first + adsl::filter(cond);
cout << filtered.str();
/*
[BEGIN DataList]
Name: hi
Data:
3 4
[END DataList]
*/
```

Right now it is a Visual Studio project (and hence uses the MSVC compiler). In the future, support will be extended to GCC or Clang (for \*NIX-like platforms including MacOS and the Windows Subsystem for Linux).

The following dependencies are required to be installed with the free [vcpkg](https://vcpkg.io/en/getting-started.html) package manager:
* The GNU Scientific Library (GSL): `.\vcpkg install gsl gsl:x64-windows`. [GSL](https://www.gnu.org/software/gsl/#subjects) is a C library with a large collection of numerical routines for everything from least-squares to simulated annealing.
* Dlib: `.\vcpkg install dlib dlib:x64-windows`. [Dlib](http://dlib.net/ml.html) is a C++ library containing various numerical routines, and **adsl** is using it for lightweight ML.
* [NOT YET] TensorFlow: `.\vcpkg install tensorflow-cc tensorflow-cc:x64-windows`. TensorFlow is the famous ML library from Google.

The following dependencies are required to be installed manually:
* [Gnuplot (mingw version)](https://sourceforge.net/projects/gnuplot/files/gnuplot/5.4.2/). NOTE: make sure to check the box to add gnuplot to the PATH during installation.

## Planned Work:
* [STARTED] Add basic plotting capability built around Gnuplot
* [DONE] Create a function for loading in data from a CSV file
* Create a class DataFrameList for working with a vector of DataFrames
* [STARTED] Include a library of functions which are wrappers around STL functions like accumulate, sort, etc
* Include a library of basic statistical functions
* Include a library for working with financial data

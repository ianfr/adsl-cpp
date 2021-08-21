# adsl-cpp: A Data Science Library for C++
## Description
This C++17 project is a library intended for fast, accurate, idiomatic data processing with no dependencies aside from [Gnuplot (mingw version)](https://sourceforge.net/projects/gnuplot/files/gnuplot/5.4.2/).

**adsl** is built specifically to work with datasets of floating-point values (so things like dates should be mapped to *double*s by the user beforehand).

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

## Planned Work:
* [STARTED] Add basic plotting capability built around Gnuplot
* [DONE] Create a function for loading in data from a CSV file
* Create a class DataFrameList for working with a vector of DataFrames
* [STARTED] Include a library of functions which are wrappers around STL functions like accumulate, sort, etc
* Include a library of basic statistical functions
* Include a library for working with financial data

# adsl-cpp: A Data Science Library for C++
## Description
This C++20 project is a 'header-only' library intended for fast, accurate, idiomatic data processing with no dependencies (aside from the STL)

Right now it is a Visual Studio project (and hence uses the MSVC compiler). In the future, support will be extended to GCC 10 (for \*NIX-like platforms including the Windows Subsystem for Linux).

A unique feature of the library is that all data frames (inspired by R) actually are actually just matrices of doubles behind the scenes to simplify implementation (plus a string to allow for storage of a data set description and the like). Data such as dates (strings) should be mapped to to doubles via a dictionary. (THIS MAY CHANGE IN THE FUTURE).

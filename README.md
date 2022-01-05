# **A** **D**ata **S**cience **L**ibrary for C++
![logo](logo.png)
## Introduction

The ADSL C++ project is a library for performant and straightforward data science. ADSL is a framework for:

* Data manipulation
* Machine learning _(via Dlib)_
* Time-series forecasting _(via ctsa)_
* Statistics _(via GSL)_
* Data visualization _(via Gnuplot)_

At ADSL's core is the `DataFrame` class to work with data sets. `DataFrame` is inspired by R's & Pandas' dataframes and can import data from a CSV file.

A cornerstone of the project is the ability to "chain together" operations on `DataFrame`s (and the `DataList`s within them) with the `+` pipe operator. Data is passed along the chain of commands from left to right, enabling incredibly expressive software development. This is similar to R's pipe operator `%>%` or the UNIX-style pipe `|`.

ADSL uses a prefix & suffix system for consistent and readable code:
* `df_` prefix indicates the function is for `DataFrame`s, 
* `dl_` is for `DataList`s, and
* `dfl_` is for `DataFrameList`s.
* `_int` suffix means the function works with integers, 
* `_dbl` means floating-point, and
* `_str` means strings.

I intend to use ADSL to bolster my stock trading strategy, and that will guide the direction of the project.

## Examples

*SEE **GUIDE.md** FOR EXAMPLES ON USING ADSL*

## Behind ADSL's Chainable Functions

Writing a chainable function is incredibly simple: any function returning a `DataList` or `DataFrame` and taking as input a `DataList` or `DataFrame` is chainable. (It's also possible to return `double` or `std::string`, but these kinds of functions can only be used at the end of a chain). 

This might seem limited, but with the power of C++ lambdas you can actually _pass in any kind of input of any type_ you want using a trick called **currying**.

To see an example of currying, take a look at the source for the chainable function `adsl::dl_getVal_dbl` (a function that gets a value in a `DataList` at the input index `i`):
```cpp
auto dl_getVal_dbl = [](int i = 0) {
    auto retFunc = [i](DataList& dl) {
        return dl.getVal_dbl(i);
    };
    return retFunc;
};
```

In the example above, we "wrap" a chainable lambda function operating on a `DataList` and returning a `double` with another lambda function that takes in an integer index. The variable `i` is _captured_ by the inner lambda inside of the brackets `[ ]`.

The trick here is that the outer function `dl_getVal_dbl` actually creates and returns the inner function `retFunc` on the spot when called. That's right, a function returning another function!

Thus, the user can supply a custom input `i` and the `+` pipe operator will only be exposed to a valid chainable function. 

The outer lambda can take in any number of arguments - so long as they are captured inside the inner lambda's brackets `[ ]`. Outer arguments that are `DataFrame`s should be captured by reference for performance purposes in the inner lambda with a `&` like `[&df]`.

## Installation

Development and testing is happening in Ubuntu 21.10 with `g++` version 11.

If you'd rather use a Docker container, see DOCKER.md. Otherwise, follow the steps below on your system:

The following dependencies are required to be installed manually:
* Gnuplot with `sudo apt install gnuplot`
* Gfortran with `sudo apt install gfortran`
* Intel TBB with `sudo apt install libtbb-dev`

The following dependencies are required to be installed (I used the awesome [vcpkg](https://vcpkg.io/en/getting-started.html) package manager, and assume that the vcpkg repo is located in `$HOME/DEV/`):
* The GNU Scientific Library (GSL): `./vcpkg install gsl`. [GSL](https://www.gnu.org/software/gsl/#subjects) is a C library with a large collection of numerical routines for everything from least-squares to simulated annealing.
* Dlib: `./vcpkg install dlib`. [Dlib](http://dlib.net/ml.html) is a C++ library containing various numerical routines including many for ML.
* Boost (date-time): `./vcpkg install boost-date-time`. Boost's Date & Time library provides functionality for working with dates, times, and strings.

Installing ctsa:

A compressed copy of the ctsa repo is included within ADSL and has to be uncompressed, placed in a folder called *ctsa*, and then built so that the *compile* Bash script can link against ctsa. To build ctsa, `cd` into *ctsa* and do `cmake .` followed by running `make`.

Installing TA-Lib:

A compressed copy of the TA-Lib repo is included within ADSL and has to be uncompressed, placed in a folder called *ta-lib-0.4.0-msvc*, and then built so that the *compile* Bash script can link against TA-Lib. To build TA-lib, cd into *ta-lib-0.4.0-msvc/ta-lib/c/make/csr/linux/g++/* and do `cmake .` followed by running `make`.

## Usage

To get started with ADSL, download the repo and stick it inside of your project. Then add `#include "adsl-cpp/Includes/ADSL.h"`to your code and install the dependencies as described above.

The example program *ADSL-CPP.cpp* can be compiled with the _compile.sh_ Bash script. Any program you write can also be compiled by setting the appropriate environment variable in the *compile.sh* script. You can also modify the Bash script to suit your own projects.

Since data is loaded from disk every time you compile your code, for larger data sets it is  reccomended that you take advantage of the *ramdisk.sh* Bash script, which essentially creates a folder in RAM that you should move your dataset(s) to with `cp FILE_NAME /mnt/adsl_ramdisk/FILE_NAME`. This option only makes sense if you have a large amount of RAM, however, because the dataset is still copied by ADSL into a DataFrame object.


## Work in Progress:
* Improving plotting capability built around Gnuplot
* Porting statistical functions from GSL
* Improve ML with Dlib (specifically, re-implement KRLS regression)

## Future Work:
* Incorporate financial time-series analysis functions from [TA-Lib](https://www.ta-lib.org/d_api/d_api.html)

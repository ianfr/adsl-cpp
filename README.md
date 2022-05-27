# **A** **D**ata **S**cience **L**ibrary for C++
![logo](logo.png)
## Introduction

The ADSL C++ project is a library for performant and straightforward data science. ADSL is a framework for:

* Data manipulation
* Machine learning _(via Dlib)_
* Time-series forecasting _(via ctsa)_
* Time-series analysis _(via TA-Lib)_
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

## Examples

*SEE **GUIDE.md** FOR EXAMPLES ON USING ADSL*

## Installation

Development and testing is happening in Ubuntu 22.04 under WSL2 with `g++` version 11.

If you'd like to use a Docker container, see DOCKER.md. Otherwise, use the commands below:

`cd; mkdir coding; cd coding && git clone https://github.com/ianfr/adsl-cpp && cd adsl-cpp`

`bash tools/dependencies.sh`

## Usage

To get started with ADSL, download the repo and install dependencies as shown above. Then add `#include "~/coding/adsl-cpp/include/ADSL.h"` to your code.

The example program *ADSL-CPP.cpp* (in *examples*) can be compiled with the _compile.sh_ Bash script. Any program you write can also be compiled by setting the appropriate `ADSL_MAIN_SRC` environment variable in the *compile.sh* script.

For your own projects, I reccomend copying *compile.sh* and modifying it (since the location of ADSL is hard-coded). It would also be possible to use something like CMake based on *compile.sh*.

Since data is loaded from disk every time you compile your code, for larger data sets it is  reccomended that you take advantage of the *ramdisk.sh* bash script, which essentially creates a folder in RAM that you should move your dataset(s) to with `cp FILE_NAME /mnt/adsl_ramdisk/FILE_NAME`. This option only makes sense if you have a large amount of RAM, however, because the dataset is still copied by ADSL into a DataFrame object. *In effect, using a ramdisk with ADSL results in **twice as much memory usage** for storing a dataset.*

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

## Work in Progress:
* Improving plotting capability built around Gnuplot
* Porting statistical functions from GSL
* Improve ML with Dlib (specifically, re-implement KRLS regression)
* More time-series analysis functions from [TA-Lib](https://www.ta-lib.org/d_api/d_api.html)

## Future Work:

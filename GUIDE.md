*(Note: these examples assume `using namespace std` and `using namespace adsl`)*

# Introduction
The rest of this section is composed of a list of all chainable functions available in ADSL. The sections afterwards are examples of how to use ADSL.

Basic DataList operations (Ops.h):
* dl_str
* dl_ToVec_*type*
* dl_getVal_*type*
* dl_append
* dl_filter_*type*
* dl_filterLT_*type*
* dl_filterLTE_*type*
* dl_filterGT_*type*
* dl_filterGTE_*type*
* dl_map_*type*

Basic DataFrame operations (Ops.h):
* df_str
* df_getData
* df_select
* df_deselect
* df_combineV

Basic DataFrameList operations (Ops.h):
* dfl_filter
* dfl_select

Machine learning (Dlib.h):
* binaryClassifier
* krlsReg

Model evaluation (EvalFit.h):
* evalFit

Statistics (GSL.h):
* fitLinear

Plotting (Plot.h):
* scatter2D

Time series analysis (TSA.h):
* autoARIMA


# Basic Examples with `DataList`
## Create a `DataList` and print it
```cpp
vector<int> a = {1, 2, 3, 4};
DataList col1(&a, DataType::INT, "An int list");
cout << col1.str() << endl;
```
Note that the first argument to the `DataList` constructor is an address, the second argument is a `DataType` (`INT`, `DBL`, or `STR`) and the third argument is the `DataList` description.

## Print with a chainable operation
We also can use the `dl_str` chainable operation from Ops.h to facilitate printing the list in the above example:
```cpp
cout << col1 + dl_str << endl;
```
With ADSL chainable operations, data is passed from left to right, with the `+` operator linking together the operations.

## Go from a string `vector` to a `DataList` back to a `vector`
```cpp
vector<string> b = {"1", "2", "3", "40"};
DataList col1(&b, DataType::STR, "");
auto stringVector = col1 + dl_toVec_str;
```
Note the `dl_` prefix and the `_str` suffix.

## Filter a `DataList` based on a custom condition
(Click [here](https://en.cppreference.com/w/cpp/language/lambda) for a review of C++ lambdas).

The snippet below keeps only the elements in a list greater than 2.
```cpp
// assume col1 was previously defined DataList...
auto cond = [](int x) {
    if (x > 2)
        return true;
    return false;
};
auto colFiltered = col1 + dl_filter_int(cond);
cout << colFiltered + dl_str << endl;
```

## Apply a function to every element in a `DataList` (i.e. perform "map" or "transform")
The snippet below squares every element in a list by mapping a function to every element in the list.
```cpp
// assume col1 was previously defined DataList...
auto squareIt = [](int x) {
    return x*x;
};
auto colMapped = col1 + dl_map_int(squareIt);
cout << colMapped + dl_str << endl;
```

# Basic Examples with `DataFrame`
## Create a `DataFrame` with 1 column
```cpp
vd someData = { 1, 2, 3, 4 };
DataList someList(&someData, DataType::DBL, "hi");
DataFrame df;
df.addCol(someList);
cout << df.str() << endl;
```
Note the `addCol` method used to add a `DataList` to a `DataFrame` - every column in a `DataFrame` is actually a `DataList`.

## Select Columns of a `DataFrame` Based on Name
In the example below, we create a `DataFrame` with 3 distinct columns named "dogs", "cats", and "birds" and then select the only the "dogs" and "birds" column with `df_select`. Then, we print the result with `df_str` to call the `.str()` method on the resulting `DataFrame`. Note how easy this process is with `+` chaining.
```cpp
vd vec0 = {3,4,5};
vd vec1 = {6,7,8};
vd vec2 = {9,10,11};
DataList dl0(&vec0, DataType::DBL, "dogs");
DataList dl1(&vec1, DataType::DBL, "cats");
DataList dl2(&vec2, DataType::DBL, "birds");
DataFrame df1;
df1.addCol(dl0);
df1.addCol(dl1);
df1.addCol(dl2);
cout << df1 + df_select({"dogs", "birds"}) + df_str << endl
```

## Combine 2 `DataFrame`s (Vertically)
Combining vertically means the two `DataFrame`s must have the same number of columns, and the data in each column of the second `DataFrame` gets added to each column of the first `DataFrame`.
```cpp
// assume df1 and df2 are arbitrary DataFrames with a matching # of columns
auto df3 = df1 + df_combineV(df2);
```

## Access a DataFrame Column Element
```cpp
// assume df1 is some DataFrame previously defined...
auto elem = df1 
    + df_select({"colA"}) // select a single column called "colA"
    + df_getData(0) // convert the 1st and only column to DataList
    + dl_getVal_dbl(3); // get the 4th value in the DataList
cout << elem << endl;
```

# Plotting
## 2D Scatterplot
```cpp
vd tmp0 = { 1, 2, 3, 4 };
vd tmp1 = { 1, 2, 3, 4 };
DataList lst_0 (&tmp0, DataType::DBL, "DL 0");
DataList lst_1 (&tmp1, DataType::DBL, "DL 1");
DataFrame df1;
df1.addCol(lst0);
df1.addCol(lst1);
df1.setDesc("testing plotting 2d");
df1 + scatter2D({"testPlot.png", "800,600"});
```

# Regression
## Perform Linear Regression
The `fitLinear` function from GSL.h expects as input a single `DataFrame` with the x-values in the first column and y-values in the second column. The `evalFit` function from EvalFit.h expects two things as input: first, the chained result of `fitLinear` (or other fitting functions) and second, the x-value at which to evaluate the fit.
```cpp
tmp0 = { 1, 2, 3, 4 };
tmp1 = { 1, 2.2, 3, 3.9 };
DataList lst_0(&tmp0, DataType::DBL, "DL 0");
DataList lst_1(&tmp1, DataType::DBL, "DL 1");
DataFrame df1;
df1.addCol(lst_0);
df1.addCol(lst_1);
df1.setDesc("testing linear fit");
DataFrame linFit = df4 + fitLinear;
cout << linFit.str();
cout << "prediction at x=5: " << linFit + evalFit({5}) << endl;
```
## Multivariate KRLS Regression
According to RDocumentation, Kernel-Based Regularized Least Squares (KRLS) is "a machine learning method described in Hainmueller and Hazlett (2014) that allows users to solve regression and classification problems..."

In the example below, a KRLS model is trained to predict the hypotenuse of a triangle. The hypotenuse function is simply sqrt(x1^2 + x2^2), so it is a function of two variables. (Most of the code is for generating test data).

Note the use of `evalFit` just as with linear regression.
```cpp
// test the KRLS regression on fitting a function f(x1,x1)
// sqrt x1^2 + x2^2
auto fTest = [](double x1, double x2) {
    return gsl_hypot(x1, x2);
};
int numPts = 100;
vd x1Vec(numPts), x2Vec(numPts), yVec(numPts);
iota(x1Vec.begin(), x1Vec.end(), 1);
iota(x2Vec.begin(), x2Vec.end(), 1);
for (int i=0; i < x1Vec.size(); i++) {
    yVec[i] = fTest(x1Vec[i], x2Vec[i]);
}
DataList kx1(&x1Vec, DataType::DBL, "x1"), kx2(&x2Vec, DataType::DBL, "x2"), 
    ky(&yVec, DataType::DBL, "f(x1,x2)");
DataFrame testKrls;
testKrls.setDesc("Testing KRLS Regression");
for (auto kcol : {kx1, kx2, ky})
    testKrls.addCol(kcol);
cout << testKrls.str() << endl;

vd krlsLabels = testKrls + df_select({"f(x1,x2)"}) + df_getData(0) + dl_toVec_dbl;
auto krlsFit = testKrls + df_deselect({"f(x1,x2)"}) + krlsReg(krlsLabels, "krlsModel.dat");
double sqrt3_3 = krlsFit + adsl::evalFit({3,3});
cout << "Sqrt of 3^2 + 3^2: " << sqrt3_3 << endl;
```

# Time-Series Analysis
## ARIMA Forecasting
ARIMA stands for Autoregressive Integrated Moving Average - it's a powerful tool for making predictions with complicated time-series data.

The example below represents the result of using the `autoARIMA` function from TSA.h. `arimaDF` contains a series of floating-point values, with each entry corresponding to a single time period. The argument 5 to `autoARIMA` indicates the number of time periods in the future to forecast the value for.
```cpp
vd arimaTestVec = {16.9969,16.5969,16.802,17.1,17.3995,17.3975,17.5008,17.3998,17.6,17.4,17.3,17,17.8};
DataList arimaDL = DataList(&arimaTestVec, DataType::DBL, "Test Input");
DataFrame arimaDF;
arimaDF.addCol(arimaDL);
DataFrame arimaResults = arimaDF + autoARIMA(5);
cout << "ARIMA results:\n" << arimaResults.str();
```

# Machine Learning
**Please note that ML in ADSL is a work in progress.**

## Train a Binary Classifier
In the example below, we first load in the classic Iris dataset from a csv file. The, we split the csv file into vector of labels and a DataFrame of features. Note that the features are passed in to `binaryClassifier` via `+` while the labels are passed in normally.
```cpp
DataFrame irisData = loadFromCSV_dbl("Datasets/iris.csv", ",", true);
vector<double> labels = irisData 
    + df_select({ "class" }) 
    + df_getData(0) 
    + dl_toVec_dbl;
DataFrame feats = irisData + df_deselect({ "class" });
DataFrame classResult = feats + binaryClassifier( labels );
// As of right now, binaryClassifier returns an empty DataFrame
```

# Basic DataFrameList Examples
Since the `DataFrameList` class was created primarily to work with scraped Nasdaq financial data, it likely will be rapidly expanded in the future as I focus more on finance-oriented functionality.

As such, I will include a snippet of code below demonstrating the usage of `DataFrameList` but will not provide explanations - the format of `DataFrameList` functions is the same as for `DataFrame`s and `DataList`s.

```cpp
// Test loading DFL from directory
DataFrameList dflist;
dflist.loadFramesFromDir("Datasets/Stocks", ".us.txt", ",", -1);
cout << dflist.size() << endl;

// Test filtering a DataFrameList
auto getAA = [](DataFrame df) {
    cout << df.getDesc() << endl;
    if (df.getDesc().compare("aa") == 0) {
        return true;
    }
    return false;
};
auto filteredDfl = dflist + dfl_filter(getAA);
cout << filteredDfl.getFrame(0) + df_str << endl;

// Test DataFrameList select
auto selectedDfl = dflist + dfl_select({"a"});
cout << selectedDfl.getFrame(0) + df_str << endl
```
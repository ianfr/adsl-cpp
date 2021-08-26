#pragma once
#include <execution>

/* Binary Classifier*/
// Constants
constexpr int MAX_BIN_F = 4; // number of features for binary classifier
constexpr double GRID_GAMMA_MIN = 0.00001; // minimum gamma for the initial grid search
constexpr double GRID_GAMMA_MAX = 1.0; // maximum gamma for the initial grid search
constexpr double GRID_C_MIN = 1.0; // minimum C for the initial grid search
constexpr double GRID_C_MAX = 100000; // maximum C for the initial grid search
constexpr long long int POP_SIZE = 100; // population size for the GA
constexpr double MAX_GENS = 10; // cap on generations for the GA
constexpr double THRESH_ACC = 98.0; // if GA reaches this accuracy, terminate early
constexpr double PERC_START = 7.5; // percentage variation on grid search results for initializing population
constexpr double PERC_TOP = 2; // percent variation for top 49.999..%
constexpr double PERC_MIDDLE = 90; // percent variation for next-to-bottom 25%
constexpr double GA_GAMMA_MIN = GRID_GAMMA_MIN; // minimum gamma for the bottom 25%
constexpr double GA_GAMMA_MAX = GRID_GAMMA_MAX; // maximum gamma for the bottom 25%
constexpr double GA_C_MIN = GRID_C_MIN; // minimum C for the bottom 25%
constexpr double GA_C_MAX = GRID_C_MAX; // maximum C for the bottom 25%
// Flags
constexpr bool PAR_SORT_FLAG = false; // only worth it for very large GA populations
constexpr auto LOOP_POL = std::execution::seq; // 'par' for parallel loops, 'seq' for non-parallel





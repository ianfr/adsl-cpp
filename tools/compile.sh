#!/bin/bash

# the ADSL installation directory
export ADSL_DIR=$HOME/coding/adsl-cpp

# the vcpkg installation directory
export VCPKG_DIR=$HOME/DEV/vcpkg

# the name of the main ADSL cpp source file
export ADSL_MAIN_SRC=examples/ADSL-CPP.cpp

# the desired name of the main executable
export ADSL_MAIN_EX=$ADSL_DIR/main

rm $ADSL_MAIN_EX

# compile the main script
g++-11 -std=c++2a \
  -I$VCPKG_DIR/installed/x64-linux/include \
  $ADSL_DIR/$ADSL_MAIN_SRC \
  -L$VCPKG_DIR/installed/x64-linux/lib -llapack -lopenblas -lgsl -ldlib -pthread -ltbb \
  -L$ADSL_DIR/include_lib/ctsa/Bin -lctsalib \
  -I$ADSL_DIR/include_lib/ta-lib/c/include -L$ADSL_DIR/include_lib/ta-lib/c/lib -lta_abstract_csr -lta_common_csr -lta_func_csr -lta_libc_csr \
  -o $ADSL_MAIN_EX \
  -lgfortran

# compile the boost in-memory script
# TODO
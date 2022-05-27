#!/bin/bash

# the name of the main ADSL cpp source file
export ADSL_MAIN_SRC=ADSL-CPP

# the desired name of the main executable
export ADSL_MAIN_EX=main

rm $ADSL_MAIN_EX

# compile the main script
g++ -std=c++2a \
  -I$HOME/DEV/vcpkg/installed/x64-linux/include \
  $ADSL_MAIN_SRC.cpp \
  -L$HOME/DEV/vcpkg/installed/x64-linux/lib -llapack -lopenblas -lgsl -ldlib -pthread -ltbb \
  -L./ctsa/Bin -lctsalib \
  -Ita-lib-0.4.0-msvc/ta-lib/c/include -Lta-lib-0.4.0-msvc/ta-lib/c/lib -lta_abstract_csr -lta_common_csr -lta_func_csr -lta_libc_csr \
  -o $ADSL_MAIN_EX \
  -lgfortran

# compile the boost in-memory script
# TODO
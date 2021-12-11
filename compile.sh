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
  -o $ADSL_MAIN_EX \
  -lgfortran

# compile the boost in-memory script
# TODO
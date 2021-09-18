#!/bin/bash

# the name of the in-memory database cpp source file
export ADSL_DATA_SRC=LoadData

# the desired name of the main executable
export ADSL_DATA_EX=db

# compile the main script
g++ -std=c++17 \
  -I$HOME/DEV/vcpkg/installed/x64-linux/include \
  MemDB/$ADSL_DATA_SRC.cpp \
  -L$HOME/DEV/vcpkg/installed/x64-linux/lib -llapack -lopenblas -lgsl -ldlib -pthread -ltbb \
  -o $ADSL_DATA_EX \
  -lgfortran -lrt
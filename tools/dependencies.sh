#!/bin/bash

# NOTE: Assumes ADSL is cloned to ~/coding/adsl-cpp

echo "Installing required Ubuntu packages..."

sudo apt install cmake git g++ g++-11 gnuplot gfortran libtbb-dev curl zip unzip tar pkg-config -y

echo "Installing vcpkg c++ package manager in ~/DEV/vcpkg..."

cd

mkdir DEV

cd DEV

git clone https://github.com/Microsoft/vcpkg.git

cd vcpkg

bash bootstrap-vcpkg.sh

echo "Installing required vcpkg c++ packages..."

./vcpkg install gsl dlib boost-date-time

echo "Installing included archived libraries for time-series analysis..."

cd ~/coding/adsl-cpp

unzip files/ctsa-master.zip

mv ctsa-master include_lib/ctsa

unzip files/ta-lib-0.4.0-msvc.zip

mv ta-lib include_lib/ta-lib

cd include_lib/ctsa

cmake .

make

cd ../ta-lib/c/make/csr/linux/g++

make

cd ~/coding/adsl-cpp

echo "Done installing.\n"
echo "See README.md for information about ADSL"
echo "See DOCKER.md for how to grab a prebuilt, containerized version of ADSL"
echo "See GUIDE.md for a wide range of ADSL examples\n"
echo "See compile.sh for how to link against ADSL in your own application"
echo "Happy coding!"

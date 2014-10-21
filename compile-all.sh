#!/bin/bash
find . -name "CMakeCache.txt" -exec rm -rf {} \;
cd ParadisEO
mkdir build
cd build
cmake ..
make
cd ../../
cmake .
make
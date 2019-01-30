#!/usr/bin/env bash

mkdir release
cd release
cmake ..
make
mkdir github
cd ..
zip -r release/github/include.zip include/
cp release/libLLV.a release/github/
# include before src as to get all our definitions at the top
python3 ssc.py "release/github/llv.h" LLV include src

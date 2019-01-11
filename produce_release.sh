#!/usr/bin/env bash

mkdir release
cd release
cmake ..
make
mkdir github
zip -r github/include.zip ../include
cp libLLV.a github/
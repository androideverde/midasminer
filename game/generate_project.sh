#!/bin/sh
# generate project

rm -rf build
mkdir build
cd build
cmake -G Xcode ..

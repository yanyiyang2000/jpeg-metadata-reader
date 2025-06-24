#!/bin/bash

BUILD_TYPE=Debug

if [ -d "build" ]; then
    make -C build install
else
    cmake -D CMAKE_BUILD_TYPE=$BUILD_TYPE -B build .
    cmake --build ./build
    make -C build install
fi

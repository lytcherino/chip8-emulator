#!/bin/bash

dir="build"

if [ -d "$dir" ]; then
    rm -rf "$dir"
fi

mkdir build && cd ./build
cmake ../
make

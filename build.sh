#!/bin/bash

target=$1
echo ${target}
old_dir=$(pwd)
cmake -Bbuild && cd build && make -j32
cd ${old_dir}
./build/${target}

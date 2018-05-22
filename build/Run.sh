#!/bin/bash

[[ ! -n $1 ]] && exit 1

echo Compile $1 to a.out

cd ../cmake-build-debug/
make
cd -
./bin/compile ./test.tc out.s
./PostRun.sh
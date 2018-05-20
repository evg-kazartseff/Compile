#!/bin/bash

[[ ! -n $1 ]] && exit 1

echo Compile $1 to a.out

./bin/compile $1 out.s
./PostRun.sh
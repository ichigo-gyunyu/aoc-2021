#!/bin/bash

cd ./src
n=$(expr $(ls -l | grep -c ^d) + 1)
dir="day-$(printf "%02d" $n)"
mkdir $dir

cd ./$dir
touch in.txt
cp ../../template.cpp main.cpp

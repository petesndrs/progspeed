#!/bin/bash

gcc --version
python3 --version

date

pushd c/pythagorean_triples

gcc -std=c99 -o pyth.exe pyth.c

time ./pyth.exe > pyth.out

popd

date

pushd python/pythagorean_triples

time python pyth.py > pyth.out

popd

date


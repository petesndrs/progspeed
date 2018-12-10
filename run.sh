#!/bin/bash

gcc --version
python3 --version
go version
javac -version
java -version

date

pushd c/pythagorean_triples

gcc -std=c99 -o pyth.exe pyth.c

time ./pyth.exe > pyth.out

popd

date

pushd go/pythagorean_triples

go build -o pyth.exe pyth.go

time ./pyth.exe > pyth.out

popd

date

pushd java/linear_congruential_gen

javac gen.java

time java gen > gen.out

popd

date

pushd python/pythagorean_triples

time python pyth.py > pyth.out

popd

date


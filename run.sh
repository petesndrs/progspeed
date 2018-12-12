#!/bin/bash

gcc --version
python3 --version
go version
javac -version
java -version

DIR=("pythagorean_triples" "linear_congruential_gen" "xor_shift_gen")
BIN=("pyth" "gen" "xor")

date

for i in ${!DIR[@]};
do

  echo ${DIR[$i]}

  pushd c/${DIR[$i]} >> /dev/null

  gcc -std=c99 -o ${BIN[$i]}.exe ${BIN[$i]}.c

  time ./${BIN[$i]}.exe > ${BIN[$i]}.out

  popd

done

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


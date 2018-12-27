#!/bin/bash

gcc --version
python3 --version
go version
javac -version
java -version

DIR=("pythagorean_triples" "linear_congruential_gen" "xor_shift_gen" "fibonacci_twoways" "kaprekars_process")
BIN=("pyth" "gen" "xor" "fib" "kap")

date

for i in ${!DIR[@]};
do

  echo ${DIR[$i]}

  pushd c/${DIR[$i]} >> /dev/null

  gcc -std=c99 -o ${BIN[$i]}.exe ${BIN[$i]}.c

  time -p ./${BIN[$i]}.exe > ${BIN[$i]}.out

  popd >> /dev/null

  pushd go/${DIR[$i]} >> /dev/null

  go build -o ${BIN[$i]}.exe ${BIN[$i]}.go

  time -p ./${BIN[$i]}.exe > ${BIN[$i]}.out

  popd >> /dev/null

  pushd java/${DIR[$i]} >> /dev/null

  javac ${BIN[$i]}.java

  time -p java ${BIN[$i]} > ${BIN[$i]}.out

  popd >> /dev/null

  pushd python/${DIR[$i]} >> /dev/null

  time -p python ${BIN[$i]}.py > ${BIN[$i]}.out

  popd >> /dev/null

done

date

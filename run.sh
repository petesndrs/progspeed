#!/bin/bash

gcc --version
pypy3 --version
go version
javac -version
java -version

DIR=("pythagorean_triples" "linear_congruential_gen" "xor_shift_gen" "fibonacci_twoways"
     "kaprekars_process" "naughts_crosses" "triangle_enclosure" "unique_anagram"
     "goldbach_conjecture" "sieve_eratosthenes" "pi_from_random" "min_spanning_tree")

BIN=("pyth" "gen" "xor" "fib" "kap" "nac" "tri" "ana"
     "gold" "erat" "pirn" "mst")

date

for i in ${!DIR[@]};
do

  echo ${DIR[$i]}

  pushd c/${DIR[$i]} >> /dev/null
  echo "C"

  gcc -std=c99 -o ${BIN[$i]}.exe ${BIN[$i]}.c

  time ./${BIN[$i]}.exe > ${BIN[$i]}.out
  diff ${BIN[$i]}.out ../../samples/${DIR[$i]}/${BIN[$i]}.out

  popd >> /dev/null

  pushd go/${DIR[$i]} >> /dev/null
  echo "go"

  go build -o ${BIN[$i]}.exe ${BIN[$i]}.go

  time ./${BIN[$i]}.exe > ${BIN[$i]}.out
  diff ${BIN[$i]}.out ../../samples/${DIR[$i]}/${BIN[$i]}.out

  popd >> /dev/null

  pushd java/${DIR[$i]} >> /dev/null
  echo "java"

  javac ${BIN[$i]}.java

  time java ${BIN[$i]} > ${BIN[$i]}.out
  diff ${BIN[$i]}.out ../../samples/${DIR[$i]}/${BIN[$i]}.out

  popd >> /dev/null

  pushd python/${DIR[$i]} >> /dev/null
  echo "python"

  time pypy3 ${BIN[$i]}.py > ${BIN[$i]}.out
  diff ${BIN[$i]}.out ../../samples/${DIR[$i]}/${BIN[$i]}.out

  popd >> /dev/null

done

date

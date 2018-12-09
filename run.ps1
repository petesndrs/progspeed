

$Env:path = $env:path + ";c:\MinGW\bin"
$Env:path = $env:path + ";c:\Python34\"
$Env:path = $env:path + ";c:\Go\bin\"
$Env:path = $env:path + ";c:\Program Files\Java\jre1.8.0_161\bin;c:\Program Files\Java\jdk1.7.0_60"

cc --version
python --version
go version
javac -version
java -version

date

pushd c\pythagorean_triples

cc -std=c99 -o pyth.exe pyth.c

Measure-Command {.\pyth.exe > pyth.out}

popd

pushd c\linear_congruential_gen

cc -std=c99 -o gen.exe gen.c

Measure-Command {.\gen.exe > gen.out}

popd

date

pushd go\pythagorean_triples

go build -o pyth.exe pyth.go

Measure-Command {.\pyth.exe > pyth.out}

popd

pushd go\linear_congruential_gen

go build -o gen.exe gen.go

Measure-Command {.\gen.exe > gen.out}

popd

date

pushd java\linear_congruential_gen

javac gen.java

Measure-Command {java gen > gen.out}

popd

date

pushd python\pythagorean_triples

Measure-Command {python pyth.py > pyth.out}

popd

pushd python\linear_congruential_gen

Measure-Command {python gen.py > gen.out}

popd

date




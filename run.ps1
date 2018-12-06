

$Env:path = $env:path + ";c:\MinGW\bin"
$Env:path = $env:path + ";c:\Python34\"
$Env:path = $env:path + ";c:\Go\bin\"

cc --version
python --version
go version

date

pushd c\pythagorean_triples

cc -std=c99 -o pyth.exe pyth.c

Measure-Command {.\pyth.exe > pyth.out}

popd

date

pushd go\pythagorean_triples

go build -o pyth.exe pyth.go

Measure-Command {.\pyth.exe > pyth.out}

popd

date

pushd python\pythagorean_triples

Measure-Command {python pyth.py > pyth.out}

popd

date




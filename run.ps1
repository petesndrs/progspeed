

$Env:path = $env:path + ";c:\MinGW\bin"
$Env:path = $env:path + ";c:\Python34\"

cc --version
python --version

date

pushd c\pythagorean_triples

cc -std=c99 -o pyth.exe pyth.c

Measure-Command {.\pyth.exe > pyth.out}

popd

date

pushd python\pythagorean_triples

Measure-Command {python pyth.py > pyth.out}

popd

date






$Env:path = $env:path + ";c:\MinGW\bin"
$Env:path = $env:path + ";c:\Python34\"
$Env:path = $env:path + ";c:\Go\bin\"
$Env:path = $env:path + ";c:\Program Files\Java\jre1.8.0_161\bin;c:\Program Files\Java\jdk1.7.0_60"

cc --version
python --version
go version
javac -version
java -version

$DIR = @("pythagorean_triples","linear_congruential_gen","xor_shift_gen")
$BIN = @("pyth","gen","xor")

date

for($i=0; $i -le 2; $i++){

    echo "$($DIR[$i])"

    pushd "c\$($DIR[$i])"

    cc -std=c99 -o "$($BIN[$i]).exe" "$($BIN[$i]).c"

    Measure-Command { Invoke-Expression ".\$($BIN[$i]).exe" > "$($BIN[$i]).out" }

    popd

}

date

for($i=0; $i -le 2; $i++){

    echo "$($DIR[$i])"

    pushd "go\$($DIR[$i])"

    go build -o "$($BIN[$i]).exe" "$($BIN[$i]).go"

    Measure-Command  {Invoke-Expression ".\$($BIN[$i]).exe" > "$($BIN[$i]).out"}

    popd

}

date

for($i=0; $i -le 2; $i++){

    echo "$($DIR[$i])"

    pushd "java\$($DIR[$i])"

    javac "$($BIN[$i]).java"

    Measure-Command { java "$($BIN[$i])" > "$($BIN[$i]).out"}

    popd

}

date

pushd python\pythagorean_triples

Measure-Command {python pyth.py > pyth.out}

popd

pushd python\linear_congruential_gen

Measure-Command {python gen.py > gen.out}

popd

date




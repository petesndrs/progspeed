

$Env:path = $env:path + ";c:\MinGW\bin"
#$Env:path = $env:path + ";c:\Python34\"
$Env:path = $env:path + ";c:\PyPy\pypy3.6-v7.1.1-win32\pypy3.6-v7.1.1-win32\"
$Env:path = $env:path + ";c:\Go\bin\"
$Env:path = $env:path + ";c:\Program Files\Java\jre1.8.0_161\bin;c:\Program Files\Java\jdk1.7.0_60"

cc --version
pypy3.exe --version
go version
javac -version
java -version

$FIRST=0
$LAST=5
$DIR = @("pythagorean_triples","linear_congruential_gen","xor_shift_gen","fibonacci_twoways","kaprekars_process","naughts_crosses","triangle_enclosure")
$BIN = @("pyth","gen","xor","fib","kap","nac","tri")

date

for($i=$FIRST; $i -le $LAST; $i++){

    echo "$($DIR[$i])"

    pushd "c\$($DIR[$i])"

    echo "C"
    cc -std=c99 -o "$($BIN[$i]).exe" "$($BIN[$i]).c"

    Measure-Command { Invoke-Expression ".\$($BIN[$i]).exe" > "$($BIN[$i]).out" }

    if (Test-Path "..\..\samples\$($DIR[$i])\$($BIN[$i]).out" -PathType Leaf){

        Compare-Object -ReferenceObject (Get-Content "$($BIN[$i]).out") -DifferenceObject (Get-Content "..\..\samples\$($DIR[$i])\$($BIN[$i]).out")

    } else {
        echo "No sample output file exists"
    }

    popd

    pushd "go\$($DIR[$i])"

    echo "go"
    go build -o "$($BIN[$i]).exe" "$($BIN[$i]).go"

    Measure-Command  {Invoke-Expression ".\$($BIN[$i]).exe" > "$($BIN[$i]).out"}

    if (Test-Path "..\..\samples\$($DIR[$i])\$($BIN[$i]).out" -PathType Leaf){

        Compare-Object -ReferenceObject (Get-Content "$($BIN[$i]).out") -DifferenceObject (Get-Content "..\..\samples\$($DIR[$i])\$($BIN[$i]).out")

    } else {
        echo "No sample output file exists"
    }

    popd

    pushd "java\$($DIR[$i])"

    echo "java"
    javac "$($BIN[$i]).java"

    Measure-Command { java "$($BIN[$i])" > "$($BIN[$i]).out"}

    if (Test-Path "..\..\samples\$($DIR[$i])\$($BIN[$i]).out" -PathType Leaf){

        Compare-Object -ReferenceObject (Get-Content "$($BIN[$i]).out") -DifferenceObject (Get-Content "..\..\samples\$($DIR[$i])\$($BIN[$i]).out")

    } else {
        echo "No sample output file exists"
    }

    popd

    echo "python"
    pushd "python\$($DIR[$i])"

#    Measure-Command {python "$($BIN[$i]).py" > "$($BIN[$i]).out"}
    Measure-Command {pypy3.exe "$($BIN[$i]).py" > "$($BIN[$i]).out"}
	
    if (Test-Path "..\..\samples\$($DIR[$i])\$($BIN[$i]).out" -PathType Leaf){

        Compare-Object -ReferenceObject (Get-Content "$($BIN[$i]).out") -DifferenceObject (Get-Content "..\..\samples\$($DIR[$i])\$($BIN[$i]).out")

    } else {
        echo "No sample output file exists"
    }

    popd

}

date

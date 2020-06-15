
git rev-parse HEAD

$Env:path = $env:path + ";c:\MinGW\bin"
#$Env:path = $env:path + ";c:\Python34\"
$Env:path = $env:path + ";c:\PyPy\pypy3.6-v7.3.1-win32\"
#$Env:path = $env:path + ";c:\PyPy\pypy3.6-v7.1.1-win32\pypy3.6-v7.1.1-win32\"
$Env:path = $env:path + ";c:\Go\bin\"
$Env:path = $env:path + ";c:\Program Files\Java\jre1.8.0_161\bin;c:\Program Files\Java\jdk1.7.0_60"

$Env:INCLUDE = "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.25.28610\include;C:\Program Files (x86)\Windows Kits\10\include\10.0.18362.0\ucrt"
$Env:LIB = "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.25.28610\lib\x64;C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.25.28610\lib\x86\store\references;C:\Program Files (x86)\Windows Kits\10\UnionMetadata\10.0.18362.0;C:\Program Files (x86)\Windows Kits\10\References\10.0.18362.0;C:\Windows\Microsoft.NET\Framework64\v4.0.30319;C:\Program Files (x86)\Windows Kits\10\Lib\10.0.18362.0\um\x64;C:\Program Files (x86)\Windows Kits\10\Lib\10.0.18362.0\ucrt\x64"
$Env:path = $env:path + ";c:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.25.28610\bin\Hostx64\x64"

cc --version
cl
pypy3.exe --version
go version
javac -version
java -version

$FIRST=0
$LAST=5
$DIR = @("pythagorean_triples","linear_congruential_gen","xor_shift_gen","fibonacci_twoways",
         "kaprekars_process","naughts_crosses","triangle_enclosure","unique_anagram",
         "goldbach_conjecture","sieve_eratosthenes")
$BIN = @("pyth","gen","xor","fib","kap","nac","tri","ana",
         "gold","erat")

date

for($i=$FIRST; $i -le $LAST; $i++){

    echo "$($DIR[$i])"

    pushd "c\$($DIR[$i])"

    echo "C/gcc"
    cc -std=c99 -o "$($BIN[$i]).cc.exe" "$($BIN[$i]).c"

    Measure-Command { Invoke-Expression ".\$($BIN[$i]).cc.exe" | Out-File -Encoding ASCII -FilePath "$($BIN[$i]).out" }

    if (Test-Path "..\..\samples\$($DIR[$i])\$($BIN[$i]).out" -PathType Leaf){

        Compare-Object -ReferenceObject (Get-Content "$($BIN[$i]).out") -DifferenceObject (Get-Content "..\..\samples\$($DIR[$i])\$($BIN[$i]).out")

    } else {
        echo "No sample output file exists"
    }

    echo "C/MSVC"
    cl /nologo /Fe:"$($BIN[$i]).cl.exe" "$($BIN[$i]).c"

    Measure-Command { Invoke-Expression ".\$($BIN[$i]).cl.exe" | Out-File -Encoding ASCII -FilePath "$($BIN[$i]).out" }

    if (Test-Path "..\..\samples\$($DIR[$i])\$($BIN[$i]).out" -PathType Leaf){

        Compare-Object -ReferenceObject (Get-Content "$($BIN[$i]).out") -DifferenceObject (Get-Content "..\..\samples\$($DIR[$i])\$($BIN[$i]).out")

    } else {
        echo "No sample output file exists"
    }


    popd

    pushd "go\$($DIR[$i])"

    echo "go"
    go build -o "$($BIN[$i]).exe" "$($BIN[$i]).go"

    Measure-Command  {Invoke-Expression ".\$($BIN[$i]).exe" | Out-File -Encoding ASCII -FilePath "$($BIN[$i]).out"}

    if (Test-Path "..\..\samples\$($DIR[$i])\$($BIN[$i]).out" -PathType Leaf){

        Compare-Object -ReferenceObject (Get-Content "$($BIN[$i]).out") -DifferenceObject (Get-Content "..\..\samples\$($DIR[$i])\$($BIN[$i]).out")

    } else {
        echo "No sample output file exists"
    }

    popd

    pushd "java\$($DIR[$i])"

    echo "java"
    javac "$($BIN[$i]).java"

    Measure-Command { java "$($BIN[$i])" | Out-File -Encoding ASCII -FilePath "$($BIN[$i]).out"}

    if (Test-Path "..\..\samples\$($DIR[$i])\$($BIN[$i]).out" -PathType Leaf){

        Compare-Object -ReferenceObject (Get-Content "$($BIN[$i]).out") -DifferenceObject (Get-Content "..\..\samples\$($DIR[$i])\$($BIN[$i]).out")

    } else {
        echo "No sample output file exists"
    }

    popd

    echo "python"
    pushd "python\$($DIR[$i])"

#    Measure-Command {python "$($BIN[$i]).py" > "$($BIN[$i]).out"}
    Measure-Command {pypy3.exe "$($BIN[$i]).py" | Out-File -Encoding ASCII -FilePath "$($BIN[$i]).out"}
	
    if (Test-Path "..\..\samples\$($DIR[$i])\$($BIN[$i]).out" -PathType Leaf){

        Compare-Object -ReferenceObject (Get-Content "$($BIN[$i]).out") -DifferenceObject (Get-Content "..\..\samples\$($DIR[$i])\$($BIN[$i]).out")

    } else {
        echo "No sample output file exists"
    }

    popd

}

date

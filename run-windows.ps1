$ErrorActionPreference = "Stop"
Set-Location $PSScriptRoot
$msysBin = "C:\msys64\ucrt64\bin"
if (!(Test-Path "$msysBin\cmake.exe")) { throw "Install MSYS2 UCRT64 dependencies; see README.md" }
$env:Path = "$msysBin;$env:Path"
& "$msysBin\cmake.exe" -S . -B build-windows -G Ninja "-DCMAKE_CXX_COMPILER=$msysBin/g++.exe" -DCMAKE_BUILD_TYPE=Debug
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& "$msysBin\cmake.exe" --build build-windows
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& .\build-windows\CompGeomGraphLab2.exe
exit $LASTEXITCODE

# ============================================================================
#  build.ps1  -  compiles the whole project with MinGW g++ (C++14).
#  Usage:  .\build.ps1
# ============================================================================
$ErrorActionPreference = 'Stop'

$root     = $PSScriptRoot
$include  = Join-Path $root 'include'
$srcDir   = Join-Path $root 'src'
$buildDir = Join-Path $root 'build'

if (-not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

$sources = Get-ChildItem -Path $srcDir -Recurse -Filter *.cpp | ForEach-Object { $_.FullName }
Write-Host "Compiling $($sources.Count) source files with g++ -std=c++14 ..." -ForegroundColor Cyan

$exe = Join-Path $buildDir 'restaurant.exe'
$gppArgs = @('-std=c++14', '-I', $include) + $sources + @('-o', $exe)

& g++ @gppArgs
if ($LASTEXITCODE -ne 0) {
    throw "Build failed (g++ returned $LASTEXITCODE)"
}

Write-Host "Build OK  ->  $exe" -ForegroundColor Green

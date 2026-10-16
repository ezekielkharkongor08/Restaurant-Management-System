# ============================================================================
#  run.ps1  -  builds the project and then runs it.
#  Usage:  .\run.ps1
# ============================================================================
$ErrorActionPreference = 'Stop'

$root = $PSScriptRoot
& (Join-Path $root 'build.ps1')

$exe = Join-Path $root 'build\restaurant.exe'
Push-Location $root
try {
    & $exe
} finally {
    Pop-Location
}

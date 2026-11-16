$ErrorActionPreference = 'Stop'

$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
if (-not (Test-Path -LiteralPath $vswhere)) {
    throw 'Install Visual Studio Build Tools with the Desktop development with C++ workload.'
}
$installation = & $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (-not $installation) {
    throw 'Visual C++ build tools were not found. Install the Desktop development with C++ workload.'
}
$vcvars = Join-Path $installation 'VC\Auxiliary\Build\vcvars64.bat'
$build = Join-Path $PSScriptRoot 'build'
New-Item -ItemType Directory -Force -Path $build | Out-Null
$command = 'call "{0}" >nul && cd /d "{1}" && cl /nologo /TC /std:c17 /W4 /WX /D_CRT_SECURE_NO_WARNINGS /Fe:AlertProcessing.exe "..\main.c"' -f $vcvars, $build
& $env:ComSpec /d /c $command
if ($LASTEXITCODE -ne 0) {
    throw "C build failed with exit code $LASTEXITCODE."
}
Copy-Item -LiteralPath (Join-Path $PSScriptRoot 'aaJsonDemo.json') -Destination $build
Write-Host "Built $(Join-Path $build 'AlertProcessing.exe')"

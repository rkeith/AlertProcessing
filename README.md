# AlertProcessing

C17 console application that reads `aaJsonDemo.json` into memory on startup,
then waits for terminal commands. The file is loaded as raw bytes; JSON parsing
and alert-processing rules are not implemented yet. The original file is not modified.

## Build and run on Windows

Requires Visual Studio or Visual Studio Build Tools with the **Desktop development
with C++** workload. From this folder in PowerShell:

```powershell
.\build.ps1
.\build\AlertProcessing.exe
```

The build script locates the installed Visual C++ compiler automatically and
also copies the demo file into `build`.

The default input path is relative to the terminal's current directory.
Optionally supply a different file:

```powershell
.\build\AlertProcessing.exe 'C:\data\another-demo.json'
```

Startup fails with a nonzero exit code if the input cannot be loaded.

## Commands

| Command | Action |
| --- | --- |
| `help` | List commands |
| `info` | Show the file path and number of bytes loaded |
| `reload` | Read the file again; retain the old data if loading fails |
| `quit`, `exit` | Exit |

Commands are case-sensitive; surrounding whitespace is ignored.
End-of-input also exits cleanly.

## CMake alternative

A CMake project is included for Visual Studio, VS Code with CMake Tools, or
other C17 toolchains:

```powershell
cmake -S . -B build
cmake --build build --config Debug
.\build\Debug\AlertProcessing.exe
```

With a single-configuration generator, the executable is directly in `build`.

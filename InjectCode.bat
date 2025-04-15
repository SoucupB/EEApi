@echo off
setlocal enabledelayedexpansion
set "pathTo=%cd%"

cmd /c "InjectorBuild.bat"
cmd /c "LibraryBuild.bat"

set binaryPath=LibraryBinary

set toExecute=InjectorBinaries\DllInjector32BitsExec.exe !pathTo!\!binaryPath!\Bots.dll
echo !toExecute!
cmd /c "!toExecute!"
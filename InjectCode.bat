@echo off
setlocal enabledelayedexpansion
set "pathTo=%cd%"
set binaryPath=LibraryBinary

set toExecute=InjectorBinaries\DllInjector32BitsExec.exe !pathTo!\!binaryPath!\Bots.dll
echo !toExecute!
cmd /c "!toExecute!"
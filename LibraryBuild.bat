@echo off
setlocal enabledelayedexpansion
set "pathTo=%cd%"

set binaryPath=LibraryBinary
set sourcePath=LibrarySources
set commonPath=Common
set filesPath=

for /r "%pathTo%\%commonPath%" %%f in (*.cpp) do (
  set crp=!pathTo!/!binaryPath!
  set "currentPathFile=!crp:\=/!"
  set comm=g++ -ILibrarySources/BotLogic -ICommon/ -ILibrarySources/ -ILibrarySources/Geometry/ -c !commonPath!/%%~nxf -std=gnu++11 -o !currentPathFile!/%%~nf.o
  echo !comm!
  cmd /c "!comm!"
)

for /r "%pathTo%\%sourcePath%" %%f in (*.cpp) do (
  set crp=!pathTo!/!binaryPath!
  set "currentPathFile=!crp:\=/!"
  set comm=g++ -ILibrarySources/BotLogic -ICommon/ -ILibrarySources/ -ILibrarySources/Geometry/ -c %%f -std=gnu++11 -o !currentPathFile!/%%~nf.o
  echo !comm!
  cmd /c "!comm!"
)

for /r %%f in (!binaryPath!/*.o) do (
  set filesPath=!filesPath! !binaryPath!/%%~nxf
)

set toExecute=g++ -ILibrarySources/BotLogic -ILibrarySources/ -ILibrarySources/Geometry/ -shared -std=gnu++11 !filesPath! -o !binaryPath!/Bots.dll -O9 -lPsapi -lgdi32 -luser32 -lmsvcrt

echo !toExecute!
cmd /c "!toExecute!"
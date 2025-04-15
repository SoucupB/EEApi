@REM g++ -ILibrarySources/BotLogic -ICommon/ -ILibrarySources/ -ILibrarySources/Geometry/ -c LibrarySources/EmpireEarth.cpp -std=gnu++11 -o LibraryBinary/Sources.o
@REM g++ -ILibrarySources/BotLogic -ICommon/ -ILibrarySources/ -ILibrarySources/Geometry/ -c Common/InjectUtilities32bits.cpp -std=gnu++11 -o LibraryBinary/InjectUtilities32bits.o
@REM g++ -ILibrarySources/BotLogic -ICommon/ -ILibrarySources/ -ILibrarySources/Geometry/ -c LibrarySources/MethodsDefinitions.cpp -std=gnu++11 -o LibraryBinary/MethodsDefinitions.o
@REM g++ -ILibrarySources/BotLogic -ICommon/ -ILibrarySources/ -ILibrarySources/Geometry/ -c LibrarySources/EETwa.cpp -std=gnu++11 -o LibraryBinary/EETwa.o
@REM g++ -ILibrarySources/BotLogic -ICommon/ -ILibrarySources/ -ILibrarySources/Geometry/ -c LibrarySources/BotLogic/BotLogic.cpp -std=gnu++11 -o LibraryBinary/BotLogic.o
@REM g++ -ILibrarySources/BotLogic -ICommon/ -ILibrarySources/ -ILibrarySources/Geometry/ -c LibrarySources/Helpers.cpp -std=gnu++11 -o LibraryBinary/Helpers.o
@REM g++ -ILibrarySources/BotLogic -ICommon/ -ILibrarySources/ -ILibrarySources/Geometry/ -c LibrarySources/EETwaTypes.cpp -std=gnu++11 -o LibraryBinary/EETwaTypes.o
@REM g++ -ILibrarySources/BotLogic -ICommon/ -ILibrarySources/ -ILibrarySources/Geometry/ -c LibrarySources/BotLogic/TimerHelper.cpp -std=gnu++11 -o LibraryBinary/TimerHelper.o
@REM g++ -ILibrarySources/BotLogic -ICommon/ -ILibrarySources/ -ILibrarySources/Geometry/ -c LibrarySources/Geometry/Geometry.cpp -std=gnu++11 -o LibraryBinary/Geometry.o
@REM g++ -ILibrarySources/BotLogic -ICommon/ -ILibrarySources/ -ILibrarySources/Geometry/ -c LibrarySources/BotLogic/AttackDecision.cpp -std=gnu++11 -o LibraryBinary/AttackDecision.o
@REM g++ -ILibrarySources/BotLogic -ICommon/ -ILibrarySources/ -ILibrarySources/Geometry/ -c LibrarySources/BotLogic/PlayerState.cpp -std=gnu++11 -o LibraryBinary/PlayerState.o
@REM g++ -ILibrarySources/BotLogic -ICommon/ -ILibrarySources/ -ILibrarySources/Geometry/ -c LibrarySources/BotLogic/ResourceManager.cpp -std=gnu++11 -o LibraryBinary/ResourceManager.o

@REM g++ -ILibrarySources/BotLogic -ILibrarySources/ -ILibrarySources/Geometry/ -shared -std=gnu++11 LibraryBinary/ResourceManager.o LibraryBinary/AttackDecision.o LibraryBinary/PlayerState.o LibraryBinary/Geometry.o LibraryBinary/TimerHelper.o LibraryBinary/EETwaTypes.o LibraryBinary/Helpers.o LibraryBinary/BotLogic.o LibraryBinary/EETwa.o LibraryBinary/MethodsDefinitions.o LibraryBinary/InjectUtilities32bits.o LibraryBinary/Sources.o -o LibraryBinary/Bots.dll -O9 -lPsapi -lgdi32 -luser32 -lmsvcrt

@echo off
setlocal enabledelayedexpansion
set "pathTo=%cd%"

set binaryPath=LibraryBinary/
set sourcePath=LibrarySources/
set commonPath=Common/
set filesPath=

for /r "%pathTo%\%commonPath%" %%f in (*.cpp) do (
  set comm=g++ -ILibrarySources/BotLogic -ICommon/ -ILibrarySources/ -ILibrarySources/Geometry/ -c !commonPath!%%~nxf -std=gnu++11 -o !binaryPath!%%~nf.o
  echo !comm!
  cmd /c "!comm!"
)

for /r "%pathTo%\%sourcePath%" %%f in (*.cpp) do (
  set comm=g++ -ILibrarySources/BotLogic -ICommon/ -ILibrarySources/ -ILibrarySources/Geometry/ -c !sourcePath!%%~nxf -std=gnu++11 -o !binaryPath!%%~nf.o
  echo !comm!
  cmd /c "!comm!"
)

for /r %%f in (!binaryPath!/*.o) do (
  set filesPath=!filesPath! !binaryPath!%%~nxf
)

set toExecute=g++ -ILibrarySources/BotLogic -ILibrarySources/ -ILibrarySources/Geometry/ -shared -std=gnu++11 !filesPath! -o !binaryPath!Bots.dll -O9 -lPsapi -lgdi32 -luser32 -lmsvcrt

@REM echo !toExecute!
@REM cmd /c "!toExecute!"
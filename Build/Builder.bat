@REM set "CHECK=if %%errorlevel%% neq 0 exit /b %%errorlevel%%"

@REM g++ -std=gnu++11 -IHeaders/ -c Sources/EETwa.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/EETwa.o
@REM g++ -std=gnu++11 -IHeaders/ -c Sources/Resource.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/Resource.o
@REM g++ -std=gnu++11 -IHeaders/ -c Sources/EmpireEarth.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/EmpireEarth.o -DREPLACE_MMU
@REM g++ -std=gnu++11 -IHeaders/ -c Sources/Player.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/Player.o
@REM g++ -std=gnu++11 -IHeaders/ -c Sources/Geometry.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/Geometry.o
@REM g++ -std=gnu++11 -IHeaders/ -c Sources/Helpers.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/Helpers.o
@REM g++ -std=gnu++11 -IHeaders/ -c Sources/PlayerState.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/PlayerState.o
@REM g++ -std=gnu++11 -IHeaders/ -c Sources/InjectUtilities32bits.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/InjectUtilities32bits.o
@REM g++ -std=gnu++11 -IHeaders/ -c Sources/TimerHelper.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/TimerHelper.o
@REM g++ -std=gnu++11 -IHeaders/ -c Sources/MethodsDefinitions.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/MethodsDefinitions.o
@REM g++ -std=gnu++11 -IHeaders/ -c Sources/MapData.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/MapData.o
@REM g++ -std=gnu++11 -IHeaders/ -c Sources/LibManager.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/LibManager.o
@REM g++ -std=gnu++11 -IHeaders/ -c Sources/Unit.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/Unit.o
@REM g++ -std=gnu++11 -IHeaders/ -c Sources/EETypes.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/EETypes.o
@REM g++ -std=gnu++11 -IHeaders/ -c Sources/Game.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/Game.o
@REM ar rcs lib/Engine.a lib/*.o
mingw32-make -f MakefileLib.mk
if errorlevel 1 exit /b 1
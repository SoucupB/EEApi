set "CHECK=if %%errorlevel%% neq 0 exit /b %%errorlevel%%"

g++ -std=gnu++11 -IHeaders/ -c Sources/EETwa.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/EETwa.o
g++ -std=gnu++11 -IHeaders/ -c Sources/EETwaTypes.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/EETwaTypes.o
g++ -std=gnu++11 -IHeaders/ -c Sources/EmpireEarth.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/EmpireEarth.o
g++ -std=gnu++11 -IHeaders/ -c Sources/Geometry.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/Geometry.o
g++ -std=gnu++11 -IHeaders/ -c Sources/Helpers.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/Helpers.o
g++ -std=gnu++11 -IHeaders/ -c Sources/PlayerState.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/PlayerState.o
g++ -std=gnu++11 -IHeaders/ -c Sources/InjectUtilities32bits.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/InjectUtilities32bits.o
g++ -std=gnu++11 -IHeaders/ -c Sources/TimerHelper.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/TimerHelper.o
g++ -std=gnu++11 -IHeaders/ -c Sources/MethodsDefinitions.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/MethodsDefinitions.o
g++ -std=gnu++11 -IHeaders/ -c Sources/MapData.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/MapData.o
g++ -std=gnu++11 -IHeaders/ -c Sources/LibManager.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/LibManager.o
ar rcs lib/Engine.a lib/*.o
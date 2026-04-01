g++ -std=gnu++11 -IHeaders/ -c Sources/EETwa.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/EETwa.o
g++ -std=gnu++11 -IHeaders/ -c Sources/EETwaTypes.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/EETwaTypes.o
g++ -std=gnu++11 -IHeaders/ -c Sources/EmpireEarth.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/EmpireEarth.o
g++ -std=gnu++11 -IHeaders/ -c Sources/Geometry.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/Geometry.o
g++ -std=gnu++11 -IHeaders/ -c Sources/Helpers.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/Helpers.o
g++ -std=gnu++11 -IHeaders/ -c Sources/PlayerState.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/PlayerState.o
g++ -std=gnu++11 -IHeaders/ -c Sources/InjectUtilities32bits.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/InjectUtilities32bits.o
g++ -std=gnu++11 -IHeaders/ -c Sources/TimerHelper.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/TimerHelper.o
g++ -std=gnu++11 -IHeaders/ -c Sources/MethodsDefinitions.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/MethodsDefinitions.o

g++ -c -std=gnu++11 -IHeaders/ -ILocal/Sources/ Local/Sources/ResourceManager.cpp -o lib/ResourceManager.o
g++ -c -std=gnu++11 -IHeaders/ -ILocal/Sources/ Local/Sources/AttackDecision.cpp -o lib/AttackDecision.o
g++ -c -std=gnu++11 -IHeaders/ -ILocal/Sources/ Local/Runner.cpp -o lib/Runner.o

g++ -IHeaders/ -ILocal/Sources/ lib/*.o -shared -std=gnu++11 -o lib/Bots.dll -O9 -lPsapi -lgdi32 -luser32 -lmsvcrt
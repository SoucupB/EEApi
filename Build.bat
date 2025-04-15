g++ -ISources/BotLogic -ISources/ -IGeometry/ -c Sources/EmpireEarth.cpp -std=gnu++11 -o Sources/Sources.o
g++ -ISources/BotLogic -ISources/ -IGeometry/ -c Sources/InjectUtilities32bits.cpp -std=gnu++11 -o Sources/InjectUtilities32bits.o
g++ -ISources/BotLogic -ISources/ -IGeometry/ -c Sources/MethodsDefinitions.cpp -std=gnu++11 -o Sources/MethodsDefinitions.o
g++ -ISources/BotLogic -ISources/ -IGeometry/ -c Sources/EETwa.cpp -std=gnu++11 -o Sources/EETwa.o
g++ -ISources/BotLogic -ISources/ -IGeometry/ -c Sources/BotLogic/BotLogic.cpp -std=gnu++11 -o Sources/BotLogic/BotLogic.o
g++ -ISources/BotLogic -ISources/ -IGeometry/ -c Sources/Helpers.cpp -std=gnu++11 -o Sources/Helpers.o
g++ -ISources/BotLogic -ISources/ -IGeometry/ -c Sources/EETwaTypes.cpp -std=gnu++11 -o Sources/EETwaTypes.o
g++ -ISources/BotLogic -ISources/ -IGeometry/ -c Sources/BotLogic/TimerHelper.cpp -std=gnu++11 -o Sources/BotLogic/TimerHelper.o
g++ -ISources/BotLogic -ISources/ -IGeometry/ -c Sources/Geometry/Geometry.cpp -std=gnu++11 -o Sources/Geometry/Geometry.o
g++ -ISources/BotLogic -ISources/ -IGeometry/ -c Sources/BotLogic/AttackDecision.cpp -std=gnu++11 -o Sources/BotLogic/AttackDecision.o
g++ -ISources/BotLogic -ISources/ -IGeometry/ -c Sources/BotLogic/PlayerState.cpp -std=gnu++11 -o Sources/BotLogic/PlayerState.o
g++ -ISources/BotLogic -ISources/ -IGeometry/ -c Sources/BotLogic/ResourceManager.cpp -std=gnu++11 -o Sources/BotLogic/ResourceManager.o

g++ -ISources/BotLogic -ISources/ -IGeometry/ -shared -std=gnu++11 Sources/BotLogic/ResourceManager.o Sources/BotLogic/AttackDecision.o Sources/BotLogic/PlayerState.o Sources/Geometry/Geometry.o Sources/BotLogic/TimerHelper.o Sources/EETwaTypes.o Sources/Helpers.o Sources/BotLogic/BotLogic.o Sources/EETwa.o Sources/MethodsDefinitions.o Sources/InjectUtilities32bits.o Sources/Sources.o -o Sources/Bots.dll -O9 -lPsapi -lgdi32 -luser32 -lmsvcrt

g++ -ISources/BotLogic -ISources/ -IGeometry/ Sources/InjectUtilities32bits.o Sources/InjectAOCEarth.cpp -std=gnu++11 -o Sources/DllInjector32BitsExec -O9 -lPsapi -lgdi32 -luser32
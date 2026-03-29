g++ -c -std=gnu++11 -IHeaders/ -ILocal/Sources/ Local/Sources/ResourceManager.cpp -o Local/Bins/ResourceManager.o -O9
g++ -c -std=gnu++11 -IHeaders/ -ILocal/Sources/ Local/Sources/AttackDecision.cpp -o Local/Bins/AttackDecision.o -O9
g++ -c -std=gnu++11 -IHeaders/ -ILocal/Sources/ Local/Runner.cpp -o Local/Bins/Runner.o -O9

g++ -std=gnu++11 -IHeaders/ -ILocal/Sources/ Local/Bins/Runner.o Local/Bins/ResourceManager.o Local/Bins/AttackDecision.o  lib/Engine.a -shared -o lib/Bots.dll -O9  -lPsapi -lgdi32 -luser32 
g++ -c -std=gnu++11 -IHeaders/ -ILocal/Sources/ Playground/Playground.cpp -o Playground/Bins/Playground.o

g++ -std=gnu++11 -IHeaders/ -ILocal/Sources/ Playground/Bins/Playground.o -Wl,--whole-archive lib/Engine.a -Wl,--no-whole-archive -shared -o lib/PlaygroundBots.dll -O9  -lPsapi -lgdi32 -luser32 -lmsvcrt
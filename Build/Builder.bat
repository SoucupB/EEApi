g++ -std=gnu++11 -IHeaders/ -c Sources/EETwa.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/EETwa.o
g++ -std=gnu++11 -IHeaders/ -c Sources/EETwaTypes.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/EETwaTypes.o
g++ -std=gnu++11 -IHeaders/ -c Sources/EmpireEarth.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/EmpireEarth.o
g++ -std=gnu++11 -IHeaders/ -c Sources/Geometry.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/Geometry.o
g++ -std=gnu++11 -IHeaders/ -c Sources/Helpers.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/Helpers.o
g++ -std=gnu++11 -IHeaders/ -c Sources/PlayerState.cpp -O9 -lPsapi -lgdi32 -luser32 -o lib/PlayerState.o
ar rcs lib/Engine.a lib/*.o
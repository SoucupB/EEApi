g++ -std=gnu++11 -IHeaders/ -c Sources/InjectUtilities32bits.cpp -O9 -lPsapi -lgdi32 -luser32 -o Injector/InjectUtilities32bits.o
g++ -std=gnu++11 -IHeaders/ -c InjectorSources/PathReader.cpp -O9 -lPsapi -lgdi32 -luser32 -o Injector/PathReader.o

g++ Injector/PathReader.o Injector/InjectUtilities32bits.o -o Injector/DllInjector32BitsExec -O9 -lPsapi -lgdi32 -luser32
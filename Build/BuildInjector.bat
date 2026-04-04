set "CHECK=if %%errorlevel%% neq 0 exit /b %%errorlevel%%"

g++ -c -std=gnu++11 -IHeaders/ Sources/InjectUtilities32bits.cpp -o Injector/InjectUtilities32bits.o -O9
g++ -c -std=gnu++11 -IHeaders/ InjectorSources/PathReader.cpp -o Injector/PathReader.o -O9
g++ -c -std=gnu++11 -IHeaders/ InjectorSources/InjectAOCEarth.cpp -o Injector/InjectAOCEarth.o -O9

g++ -std=gnu++11 -IHeaders/ Injector/InjectAOCEarth.o Injector/PathReader.o Injector/InjectUtilities32bits.o -o Injector/DllInjector32BitsExec -O9 -lPsapi -lgdi32 -luser32
cd Injector && DllInjector32BitsExec.exe
set "CHECK=if %%errorlevel%% neq 0 exit /b %%errorlevel%%"

call Builder.bat
if errorlevel 1 exit /b 1
call BuildInjector.bat
if errorlevel 1 exit /b 1

copy .\lib\Engine.a .\Release\lib\
if errorlevel 1 exit /b 1
copy .\Injector\DllInjector32BitsExec.exe .\Release\GameInjector.exe
if errorlevel 1 exit /b 1
copy .\Injector\Config.txt .\Release\Config.txt
if errorlevel 1 exit /b 1
copy .\Headers\*.h .\Release\EngineHeaders\
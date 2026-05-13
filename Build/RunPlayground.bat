set "CHECK=if %%errorlevel%% neq 0 exit /b %%errorlevel%%"

call Builder.bat
if errorlevel 1 exit /b 1
call BuildInjector.bat
if errorlevel 1 exit /b 1
call BuildPlayground.bat
if errorlevel 1 exit /b 1

cd Injector && DllInjector32BitsExec.exe
set "CHECK=if %%errorlevel%% neq 0 exit /b %%errorlevel%%"

call BuildDLLGame.bat
cd Injector && DllInjector32BitsExec.exe
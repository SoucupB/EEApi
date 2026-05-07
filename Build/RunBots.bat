set "CHECK=if %%errorlevel%% neq 0 exit /b %%errorlevel%%"

call BuilderRelease.bat
call BuildDLLGame.bat
cd Injector && DllInjector32BitsExec.exe
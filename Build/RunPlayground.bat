set "CHECK=if %%errorlevel%% neq 0 exit /b %%errorlevel%%"

call Builder.bat
call BuildPlayground.bat
call BuildInjector.bat

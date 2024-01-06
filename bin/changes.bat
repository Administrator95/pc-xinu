@echo off
IF NOT EXIST %1\src\makefile goto usage
goto changes

:usage 
echo Usage: changes L3X-altdir-dir 
echo Finds chenged files wrt %L3X%
goto end

:changes
echo *copy /s /u /n %1 %L3X% > %L3X%\changes
*copy /s /u /n /[!*.obj xmodules makefile xbldlib x*.lib *.$$$ *.BAK *.{??] %1 %L3X% >> %L3X%\changes
echo. >> %L3X%\changes
echo *copy /s /u /n %L3X% %1 >> %L3X%\changes
*copy /s /u /n /[!*.obj xmodules makefile xbldlib x*.lib *.$$$ *.BAK *.{??] %L3X% %1 >> %L3X%\changes

:end


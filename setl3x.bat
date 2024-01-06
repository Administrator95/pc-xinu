@echo off
if NOT "%1" == "" set L3X=%1
if NOT "%L3X%" == "" goto chkl3x
if "%1" == "" goto usage

:chkl3x
if not exist %L3X%\SETL3X.BAT goto usage

:setrest
if NOT "%WINDIR%" == "" set PATH=%WINDIR%\command;%WINDIR%
if "%WINDIR%" == "" set PATH=c:\dos
set PATH=%L3X%\bin;%PATH%
echo -L%L3X%\bin >%L3X%\bin\tlink.cfg
cd %L3X%\main
goto end

:usage
echo Usage: setl3x [L3X-base-directory] (where SETL3X.BAT resides)
echo E.g. setl3x c:\L3X
echo Default argument L3X-base-directory is environment var %L3X
echo.
echo L3X-base-directory must exist and hold L3X, or this message is displayed

:end

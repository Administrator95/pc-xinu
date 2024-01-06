@echo off
if "%1"=="" goto usage
goto doit

:usage
echo Usage: ddir DIR
echo list sub directories in DIR
goto done

:doit
command /C dir /A:d /o:N /B %1 

:done

@echo off
echo.
echo.
echo Login to Xinu 7.9 as user "dc" (no password)
echo Logout with the command "dos"
echo.
echo.

pause
@echo on
lh slip8250 0x65 1 > NUL
xinu79 -n 

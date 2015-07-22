@echo off
mode co80,102

rem - check if the flag file exists
if exist @debug@ del @debug@ > nul
rem - touch all sources so new object files are generated
for %%1 in (*.c) do touch %%1

goto build

:tryagain
echo.
echo Prememere un tasto qualsiasi per rieseguire NMAKE
echo Premerere Ctrl-C per terminare.
pause > nul

:build
cls
nmake -nologo
if errorlevel 1 goto tryagain
lxlite pmbench.exe
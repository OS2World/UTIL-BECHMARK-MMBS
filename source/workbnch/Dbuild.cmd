@echo off
set debug=1
mode co80,102

: if the flag file doesn't exist create it and touch all the sources
if exist @debug@ goto build

echo . > @debug@
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
pause


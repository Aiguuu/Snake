@echo off
rem Compile les tests et le jeu sans make (Windows, gcc MinGW dans le PATH).
rem A lancer depuis le dossier du projet.

set MODULES=src\a_faire.c src\alea.c src\types.c src\carte.c src\serpent.c src\partie.c src\scores.c
set OPTIONS=-std=c11 -Wall -Wextra -g

echo Compilation des tests...
gcc %OPTIONS% -o tests.exe src\tests.c %MODULES%
if errorlevel 1 goto erreur

echo Compilation du jeu...
gcc %OPTIONS% -Iraylib\include -o serpent.exe src\interface.c %MODULES% -Lraylib\lib -lraylib -lopengl32 -lgdi32 -lwinmm
if errorlevel 1 goto erreur

echo.
echo Compilation reussie : tests.exe et serpent.exe
exit /b 0

:erreur
echo.
echo La compilation a echoue : lisez le PREMIER message d'erreur ci-dessus.
exit /b 1

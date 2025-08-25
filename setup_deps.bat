@echo off
setlocal

REM Percorso dove installare vcpkg (nella cartella del progetto)
set VCPKG_ROOT=%~dp0\vcpkg

REM Se vcpkg non esiste, clonalo
if not exist "%VCPKG_ROOT%" (
    echo Clonazione di vcpkg...
    git clone https://github.com/microsoft/vcpkg "%VCPKG_ROOT%"
    call "%VCPKG_ROOT%\bootstrap-vcpkg.bat"
)

REM Installazione pacchetti necessari
echo Installazione dipendenze: SDL2, SDL2_ttf, glm
"%VCPKG_ROOT%\vcpkg.exe" install sdl2 sdl2-ttf glm --triplet x64-windows

REM Aggiunta integrazione automatica per CMake (user-wide)
"%VCPKG_ROOT%\vcpkg.exe" integrate install

REM Scrive su file il comando CMake con toolchain già corretto
echo.
echo Per configurare CMake usa questo comando:
echo   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake"
echo.
echo Se sei da Powershell usa:
echo   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake"
echo.
echo Oppure configuralo dentro CLion/Visual Studio aggiungendo:
echo   -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake"
echo.

pause

@echo off
setlocal

REM Percorso dove si trova vcpkg (nella cartella del progetto)
set VCPKG_ROOT=%~dp0\vcpkg

REM Verifica che vcpkg esista
if not exist "%VCPKG_ROOT%\vcpkg.exe" (
    echo ERRORE: vcpkg non trovato in "%VCPKG_ROOT%"
    pause
    exit /b 1
)

REM Disinstallazione pacchetti (SDL2, SDL2_ttf, glm)
echo Rimozione pacchetti installati...
"%VCPKG_ROOT%\vcpkg.exe" remove sdl2 sdl2-ttf glm --triplet x64-windows

REM Rimozione di vcpkg stesso
echo Eliminazione cartella vcpkg...
rmdir /s /q "%VCPKG_ROOT%"

echo.
echo Tutti i pacchetti e vcpkg sono stati rimossi.
echo.

pause

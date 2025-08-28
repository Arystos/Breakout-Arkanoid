@echo off
setlocal

REM Percorso dove si trova vcpkg (nella cartella del progetto)
set VCPKG_ROOT=%~dp0\vcpkg

REM Verifica che vcpkg esista
if not exist "%VCPKG_ROOT%\vcpkg.exe" (
    echo ERROR: vcpkg not found: "%VCPKG_ROOT%"
    pause
    exit /b 1
)

REM Disinstallazione pacchetti (SDL2, SDL2_ttf, glm)
echo Removing packages: SDL2, SDL2_ttf, glm
"%VCPKG_ROOT%\vcpkg.exe" remove sdl2 sdl2-ttf glm --triplet x64-windows

REM NOTA: vcpkg rimane installato, vengono rimossi solo i pacchetti

echo.
echo Packages unistalled succesfully, vcpkg is still avaiable per future use.
echo Want to remove other unused packages? Run:
echo   "%VCPKG_ROOT%\vcpkg.exe" remove --outdated --recurse
echo.

pause

@echo off
REM Build script for the C++ Winsock chat application (MSVC / Visual Studio 2022)
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 (
    echo Failed to initialize MSVC environment.
    exit /b 1
)

cl /nologo /EHsc /std:c++17 /I server server\*.cpp /Fe:main_server.exe ws2_32.lib
if errorlevel 1 exit /b 1

cl /nologo /EHsc /std:c++17 /I client client\*.cpp /Fe:main_client.exe ws2_32.lib
if errorlevel 1 exit /b 1

del /q *.obj 2>nul
echo.
echo Build succeeded: main_server.exe and main_client.exe

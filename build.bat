@echo off

REM Must run this batch file as it creates the environment that will be used by MSVC's cl.exe compiler
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

set GENERATOR="Ninja"
set BUILD_DIR=build

cmake -S . -B %BUILD_DIR% -G %GENERATOR%
@REM   -DCMAKE_C_COMPILER=cl.exe ^
@REM   -DCMAKE_CXX_COMPILER=cl.exe

cmake --build %BUILD_DIR%

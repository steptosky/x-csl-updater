@echo off

set QT_PATH="C:/Qt/Qt5/5.11.2/msvc2015_64"
set SLN_VERSION="Visual Studio 14 Win64"
set PROJ_DIR=build

if not exist %PROJ_DIR% mkdir %PROJ_DIR%
cd %PROJ_DIR%
rem configure
cmake -G %SLN_VERSION% ../ -DCMAKE_PREFIX_PATH=%QT_PATH% -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../Package

rem build
cmake --build . --clean-first --config Release

rem install
cmake --build . --target INSTALL --config Release

cd ../
pause
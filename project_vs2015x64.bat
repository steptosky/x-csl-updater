@echo off

set QT_PATH="C:/Qt/Qt/5.6/msvc2015_64"

set SLN_VERSION="Visual Studio 14 Win64"

set PROJ_DIR=Project_vs2015x64

if not exist %PROJ_DIR% mkdir %PROJ_DIR%
cd %PROJ_DIR%
cmake -G %SLN_VERSION% ../ -DCMAKE_PREFIX_PATH=%QT_PATH%
cd ../

pause
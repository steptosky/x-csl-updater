@echo off

set QT_PATH="C:/Qt/5.15.2/msvc2019_64"

set SLN_VERSION="Visual Studio 16 2019"

set PROJ_DIR=build

if not exist %PROJ_DIR% mkdir %PROJ_DIR%
cd %PROJ_DIR%
rem configure
cmake -G %SLN_VERSION% ../ ^
        -DCMAKE_PREFIX_PATH=%QT_PATH% ^
        -DCMAKE_BUILD_TYPE=Release ^
        -DCMAKE_INSTALL_PREFIX=../Package ^
        -DCLEANUP_INSTALL_FIRST=ON

rem build
cmake --build . --clean-first --config Release

rem install
cmake --build . --target INSTALL --config Release

cd ../
pause
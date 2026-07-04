@echo off

set "SCRIPT_DIR=%~dp0"
set "SCRIPT_DIR=%SCRIPT_DIR:\=/%"

call "%SCRIPT_DIR%setup-qt-env.bat" || goto :eof
echo QT_PATH=%QT_PATH%

set SLN_VERSION="Visual Studio 16 2019"

set PROJ_DIR=Project_vs2019x64

if not exist %PROJ_DIR% mkdir %PROJ_DIR%
cd %PROJ_DIR%
cmake -G %SLN_VERSION% ../ ^
        "-DQT_PATH=%QT_PATH%" ^
        "-DCMAKE_PREFIX_PATH=%QT_PATH%" ^
        -DCMAKE_INSTALL_PREFIX=../Package ^
        -DCLEANUP_INSTALL_FIRST=ON
cd ../

pause

@echo off

set "SCRIPT_DIR=%~dp0"
set "SCRIPT_DIR=%SCRIPT_DIR:\=/%"

call "%SCRIPT_DIR%../setup-qt-env.bat" || goto :eof
echo QT_PATH=%QT_PATH%
set "QT_BIN_PATH=%QT_PATH%/bin"
set "PATH=%PATH%;%QT_BIN_PATH%"

call "%QT_BIN_PATH%/lupdate.exe" . -ts x-csl-updater_ru.ts

pause

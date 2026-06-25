@echo off

if not defined QT_BIN_PATH set "QT_BIN_PATH=C:\Qt\5.15.2\msvc2019_64\bin"
set "PATH=%PATH%;%QT_BIN_PATH%"

echo %PATH%

call "%QT_BIN_PATH%\lupdate.exe" . -ts x-csl-updater_ru.ts

pause

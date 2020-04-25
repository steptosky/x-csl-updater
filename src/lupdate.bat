@echo off

set QT_BIN_PATH="C:\Qt\5.11.2\msvc2015_64\bin"
set PATH=%PATH%;%QT_BIN_PATH%;

echo %PATH%

call lupdate.exe . -ts x-csl-updater_ru.ts

pause
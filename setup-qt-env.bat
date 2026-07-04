@echo off

if defined QT_PATH exit /b 0

setlocal
set "SCRIPT_DIR=%~dp0"
set "SCRIPT_DIR=%SCRIPT_DIR:\=/%"
set "RESOLVED_QT_PATH="

for /f "usebackq delims=" %%I in (`cmake -P "%SCRIPT_DIR%resolve-qt-path.cmake"`) do (
    set "RESOLVED_QT_PATH=%%I"
)

if not defined RESOLVED_QT_PATH (
    endlocal
    exit /b 1
)

endlocal & set "QT_PATH=%RESOLVED_QT_PATH%"
exit /b 0

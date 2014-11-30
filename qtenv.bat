
set QT_DIR=C:\Qt\4.8.5_msvs2010

set PATH=%PATH%;%QT_DIR%\bin
set QMAKESPEC=%QT_DIR%\mkspecs\win32-msvc2010

call copy_qtlibs.bat

:exit

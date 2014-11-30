mkdir bin\debug
mkdir bin\release

copy %QT_DIR%\bin\QtCore4.dll bin\release\QtCore4.dll
copy %QT_DIR%\bin\QtGui4.dll bin\release\QtGui4.dll
copy %QT_DIR%\bin\QtNetwork4.dll bin\release\QtNetwork4.dll

copy %QT_DIR%\bin\QtCored4.dll bin\debug\QtCored4.dll
copy %QT_DIR%\bin\QtGuid4.dll bin\debug\QtGuid4.dll
copy %QT_DIR%\bin\QtNetworkd4.dll bin\debug\QtNetworkd4.dll
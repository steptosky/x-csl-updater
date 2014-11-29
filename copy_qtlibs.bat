mkdir bin\debug
mkdir bin\release

copy %QTDIR%\bin\QtCore4.dll bin\release\QtCore4.dll
copy %QTDIR%\bin\QtGui4.dll bin\release\QtGui4.dll
copy %QTDIR%\bin\QtWidgets4.dll bin\release\QtWidgets4.dll

copy %QTDIR%\bin\QtCored4.dll bin\debug\QtCored4.dll
copy %QTDIR%\bin\QtGuid4.dll bin\debug\QtGuid4.dll
copy %QTDIR%\bin\QtWidgetsd4.dll bin\debug\QtWidgetsd4.dll
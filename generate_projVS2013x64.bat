@echo off

cd /d %~dp0
call "c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
call qtenv.bat

del X-CSL-Updater.sln
del X-CSL-Updater.sdf

call sts_get_hg_version.bat

cd src/
call make_file_list.py
cd ../

qmake -spec win32-msvc2010 -r -tp vc X-CSL-Updater.pro

pause


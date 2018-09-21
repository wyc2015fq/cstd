@echo off
call vs2015_x64.bat
::del aa.exe
::%cc%  aa.cpp
del hello_ext.pyd
%cc%  /LD  hello_ext.cpp
ren hello_ext.dll hello_ext.pyd




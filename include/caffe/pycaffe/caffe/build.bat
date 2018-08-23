@echo off
call vs2015_x64.bat
::del aa.exe
::%cc%  aa.cpp
del _caffe.dll
del _caffe.pyd
del _caffe.obj
%cc%  /LD  _caffe.cpp /out:D:\code\bin\_caffe.dll
::%cc%  /LD  _caffe.cpp /out:D:\code\bin\_caffe.pyd


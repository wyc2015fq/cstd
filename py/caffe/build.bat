@echo off
call vs2015_x64.bat
::del aa.exe
::%cc%  aa.cpp
::dumpbin /exports C:\ProgramData\Anaconda3\DLLs\_bz2.pyd
del _caffe.dll
del _caffe.pyd
del _caffe.obj
%cc%  /LD  _caffe.cpp
copy /y _caffe.dll _caffe.pyd
dumpbin /exports _caffe.dll
goto end
::copy ../../../../bin1
::%cc%  /LD  _caffe.cpp /out:D:\code\bin\_caffe.pyd

:end


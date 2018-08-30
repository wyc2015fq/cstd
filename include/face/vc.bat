@echo off
call msvcdef.bat
del *.dll *.obj ut *.rar *.lib
call %EXPORTS_H% libCPMDev.def CPMDev.h

echo on
echo #include "face_recog.inl">libface_recog.c

%_CC%  /c libface_recog.c
echo.EXPORTS>libface_recog.def
echo.face_recog @1>>libface_recog.def
echo.load_image @2>>libface_recog.def
echo.save_image @3>>libface_recog.def
echo.free_image @4>>libface_recog.def

%_DLL% /def:libface_recog.def libface_recog.obj /out:libface_recog.dll

copy /y libface_recog.lib E:\code\cstd\lib\
copy /y libface_recog.dll C:\Windows\

::%rar% a libCPMDev.rar test_CPMDev.exe test_CPMDev.cpp aaa.inl *.lib *.dll *.h
::%RMTMP%
::%rar% a CPMDev.rar -r *.h *.cpp *.inl

:end

@echo off
call msvcdef.bat
del *.dll *.obj ut *.rar *.lib
call %EXPORTS_H% libCPMDev.def CPMDev.h

echo on
echo #include "lib\openssl-1.0.2d\libopenssl.inl">libopenssl.c
::%_CC%  /c libopenssl.c
%_CC%  /c libCPMDev.cpp
%_DLL% /def:libCPMDev.def libCPMDev.obj libeay32.lib ssleay32.lib /out:libCPMDev.dll

::echo #define test_CPMDev main>test_CPMDev.cpp
::echo #include "test_CPMDev.inl">>test_CPMDev.cpp
::%_CC% test_CPMDev.cpp libopenssl.obj  libeay32.lib ssleay32.lib /out:test_CPMDev.exe /link %LIBPATH%
%rar% a libCPMDev.rar test_CPMDev.exe test_CPMDev.cpp aaa.inl *.lib *.dll *.h
%RMTMP%  test_CPMDev.cpp  *.lib *.dll *.exe ut
%rar% a CPMDev.rar -r *.h *.cpp *.inl

:end

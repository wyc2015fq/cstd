@echo off

del *.dll *.lib
set _R=E:\soft\VC2008PE\VC
set _R2=E:\soft\VC2008PE\VC\PlatformSDK
set _RI=E:\pub\cstd
set rar="C:\Program Files\WinRAR\Rar.exe"

::set INCPATH=%_RI%\include
::set LIBPATH=%_RI%\lib
set BIN=%_R%\bin

set INCLUDEPATH= /I"%_R2%\include" /I%_R%\include
set INCLUDEPATH= %INCLUDEPATH% /I".\openssl-1.0.2d"
set LIBPATH=  /LIBPATH:".\openssl-1.0.2d"  /LIBPATH:"%_R%\lib" /LIBPATH:"%_R2%\lib" /LIBPATH:"%_R%\MFC\lib"

set CFLAGS=%INCLUDEPATH%  /MD /W3 /O2  /FD /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS"
set CPPFLAGS=%CFLAGS%

set NEEDLIB=user32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib crypt32.lib

set _CC="%BIN%\cl" /nologo %CFLAGS%
set _CPP="%BIN%\cl" /nologo %CFLAGS%

set RC=@"%BIN%\rc"
set _LINK="%BIN%\link" /nologo /OPT:REF %LIBPATH% %NEEDLIB%
set _DLL="%BIN%\link" /nologo /OPT:REF %LIBPATH% /dll  %NEEDLIB%
set _LIB="%BIN%\lib" /nologo %LIBPATH%
::CFLAGS=%INCLUDE% /D "WIN32"
set RFLAGS=%INCLUDEPATH%
set MAKEVERSION="E:\code\cstd\src\makeVersion.bat"
set EXPORTS_H="E:\code\cstd\bat\EXPORTS_h.bat"
set EXPORTS="E:\code\cstd\bat\EXPORTS.bat"
set COPY=copy /y

set outpath=%_RI%\lib
set toolpath=E:\code\bin
set toolbak=%rar% a %toolpath%\bin.rar

set RM=del
set RMTMP=@del /s .xdcenv.mak .dep .dlls .executables .interfaces cscope.out .libraries *.x64P *~ *64P *470MV .source_list *.bak *.o  *.exp *.pcf *.orig *.positions *.ncb *.opt *.plg *.bsc *.obj *.ilk *.pdb *.vpj *.vpw *.vpwhist *.vtg *.aps *.pch *.res *.sbr *.idb *.001 *.paf2


del  *.obj ut *.rar
if not exist libSLGDev.def (
  call %EXPORTS_H% libSLGDev.def SLGDev.h
)

echo on
::echo #include "lib\openssl-1.0.2d\libopenssl.inl">libopenssl.c
::%_CC%  /c libopenssl.c
%_CC%  /c libSLGDev.cpp
rc.exe /n /fo libSLGDev.res libSLGDev.rc
%_DLL% /def:libSLGDev.def libSLGDev.obj libSLGDev.res libeay32.lib ssleay32.lib /out:libSLGDev.dll
::%_DLL% /def:libSLGDev.def libSLGDev.obj libSLGDev.res libcrypto.lib libssl.lib /out:libSLGDev.dll
goto end

::echo #define test_SLGDev main>test_SLGDev.cpp
::echo #include "test_SLGDev.inl">>test_SLGDev.cpp
::%_CC% test_SLGDev.cpp libopenssl.obj  libeay32.lib ssleay32.lib /out:test_SLGDev.exe /link %LIBPATH%
%rar% a libSLGDev.rar test_SLGDev.exe test_SLGDev.cpp SLGDev.h aaa.inl *.lib *.dll *.doc
%RMTMP%  test_SLGDev.cpp  *.exe ut *.res
%rar% a SLGDev.rar -r *.h *.cpp *.inl *.lib *.def *.bat

:end

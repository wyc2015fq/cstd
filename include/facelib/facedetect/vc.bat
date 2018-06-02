@echo off

set _R=D:\soft\VC6Pro\VC98
set _R2=D:\soft\VC6Pro\Microsoft SDK
set _RI=E:\pub\cstd
set rar="C:\Program Files\WinRAR\Rar.exe"

::set INCPATH=%_RI%\include
::set LIBPATH=%_RI%\lib
set BIN=%_R%\bin

set INCLUDEPATH= /I"%_R2%\include" /I%_R%\include /I"E:\code\cstd\include"
set INCLUDEPATH= %INCLUDEPATH% /I".\openssl-1.0.2d"
set LIBPATH=  /LIBPATH:"%_R%\lib" /LIBPATH:"%_R2%\lib" /LIBPATH:"%_R%\MFC\lib"

set CFLAGS=%INCLUDEPATH% /Zm1000 /ML /W3 /O2  /FD /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS"
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
if not exist libfacedetect.def (
  call %EXPORTS_H% libfacedetect.def facedetect.h
)

echo on
::echo #include "lib\openssl-1.0.2d\libopenssl.inl">libopenssl.c
::%_CC%  /c libopenssl.c
echo #include "facedetect.inl">libfacedetect.c
echo #include "lib.inl">>libfacedetect.c
%_CC%  /c libfacedetect.c
::rc.exe /n /fo libfacedetect.res libfacedetect.rc
%_DLL% /def:libfacedetect.def libfacedetect.obj /out:libfacedetect.dll
del libfacedetect.cpp
::%_DLL% /def:libfacedetect.def libfacedetect.obj libfacedetect.res libcrypto.lib libssl.lib /out:libfacedetect.dll


::echo #define test_slgdev main>test_facedetect.cpp
::echo #include "test_facedetect.inl">>test_facedetect.cpp
::%_CC% test_facedetect.cpp libopenssl.obj  libeay32.lib ssleay32.lib /out:test_facedetect.exe /link %LIBPATH%
copy /y Z:\yiwei\git\rongyi_term_driver\android\cstd\facelib\facedetect\*.so  .\
%rar% a libfacedetect.rar *.c facedetect.h aaa.inl *.lib *.dll *.doc *.so *.bmp *.md
%RMTMP%  test_facedetect.cpp  *.exe ut *.res RC*
::%rar% a facedetect.rar -r *.h *.cpp *.inl *.lib *.def *.bat *.dll *.lib *.dsp *.dsw

:end

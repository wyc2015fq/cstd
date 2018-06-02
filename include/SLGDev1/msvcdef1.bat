@echo off
set _R=E:\soft\VC6Pro\VC98
set _RI=E:\pub\cstd

::set INCPATH=%_RI%\include
::set LIBPATH=%_RI%\lib
set BIN=%_R%\bin

set INCLUDEPATH= /I"E:\soft\VC6Pro\Microsoft SDK\include" /I%_R%\sdk_2k3\include /I%_R%\include
set INCLUDEPATH= %INCLUDEPATH% /I"E:\code\cstd\lib\openssl-1.0.2d\include"
set LIBPATH= /LIBPATH:"E:\code\cstd\lib" /LIBPATH:"E:\code\lib\DXSDK\lib" /LIBPATH:"E:\soft\VC6Pro\Microsoft SDK\lib" /LIBPATH:"%_R%\sdk_2k3\lib" /LIBPATH:"%_R%\lib" /LIBPATH:"%_R%\MFC\lib"

set CFLAGS=%INCLUDEPATH%  /MD /W3 /O2  /FD /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS"
set CPPFLAGS=%CFLAGS%

set NEEDLIB=user32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib crypt32.lib


set _CC=%BIN%\cl /nologo %CFLAGS%
set _CPP=%BIN%\cl /nologo %CFLAGS%

set RC=@%BIN%\rc
set _LINK=%BIN%\link /nologo /OPT:REF %LIBPATH% %NEEDLIB%
set _DLL=%BIN%\link /nologo /OPT:REF %LIBPATH% /dll  %NEEDLIB%
set _LIB=%BIN%\lib /nologo %LIBPATH%
::CFLAGS=%INCLUDE% /D "WIN32"
set RFLAGS=%INCLUDEPATH%
set MAKEVERSION="E:\code\cstd\src\makeVersion.bat"
set EXPORTS_H="E:\code\cstd\bat\EXPORTS_h.bat"
set EXPORTS="E:\code\cstd\bat\EXPORTS.bat"
set COPY=copy /y

set outpath=%_RI%\lib
set toolpath=E:\code\bin
set rar="C:\Program Files\WinRAR\Rar.exe"
set toolbak=%rar% a %toolpath%\bin.rar

set RM=del
set RMTMP=@del /s .xdcenv.mak .dep .dlls .executables .interfaces cscope.out .libraries *.x64P *~ *64P *470MV .source_list *.bak *.o *.lib *.exp *.pcf *.orig *.positions *.ncb *.opt *.plg *.bsc *.obj *.ilk *.pdb *.vpj *.vpw *.vpwhist *.vtg *.aps *.pch *.res *.sbr *.idb *.001 *.paf2

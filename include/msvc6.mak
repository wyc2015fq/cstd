
_R=E:\soft\VC6Pro\VC98
_RI=E:\pub\cstd
INCPATH=$(_RI)\include
LIBPATH=$(_RI)\lib
BIN=$(_R)\bin
INCLUDE= -IE:\code\lib\DXSDK\Include -I$(_R)\sdk_2k3\include -I$(_R)\include -I$(_R)\MFC\Include -I$(_R)\ATL\Include -I..\include -I..\..\include -IE:\code\cstd\include -IE:\code\cstd
SETLIB=(set lib=E:\code\lib\DXSDK\lib;$(_R)\sdk_2k3\lib;$(_R)\lib;$(_R)\MFC\lib;$(LIBPATH))
CC=@$(SETLIB)&@$(BIN)\cl /nologo
CPP=@$(SETLIB)&@$(BIN)\cl /nologo
RC=@$(BIN)\rc
LINK=@$(SETLIB)&$(BIN)\link /nologo /OPT:REF
LIB=@$(BIN)\lib /nologo
CFLAGS=$(INCLUDE) /MT /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD
CPPFLAGS=$(INCLUDE) /MT /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD
#CFLAGS=$(INCLUDE) /D "WIN32"
RFLAGS=$(INCLUDE)
MAKEVERSION="E:\code\cstd\src\makeVersion.bat"
EXPORTS_H="E:\code\cstd\src\EXPORTS_h.bat"
EXPORTS="E:\code\cstd\src\EXPORTS.bat"
COPY=copy /y


incpath=$(_RI)\include
libpath=$(_RI)\lib
binpath=$(_RI)\bin
toolpath=E:\code\bin
rar="C:\Program Files\WinRAR\Rar.exe"
toolbak=$(rar) a $(toolpath)\bin.rar


.obj.dll:
	$(LINK) /dll $< /out:$@

.obj.exe:
	$(LINK) $< /out:$@

RM=del
RMTMP=@(del /s .xdcenv.mak .dep .dlls .executables .interfaces cscope.out .libraries *.x64P *~ *64P *470MV .source_list *.bak *.o *.lib *.exp *.pcf *.orig *.positions *.ncb *.opt *.plg *.bsc *.obj *.ilk *.pdb *.vpj *.vpw *.vpwhist *.vtg *.aps *.pch *.res *.sbr *.idb *.001 *.paf2 *.def 2>nul)>nul

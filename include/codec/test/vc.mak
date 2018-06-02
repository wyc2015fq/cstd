# 作者：易伟

include ..\msvc6.mak

PROJNAME=文本工具集

NEEDLIB=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib

lib_obj=ecfile.obj

tags1=\
test_comp.exe \
test_codec.exe \
test_ecfile.exe \
minixmllib.lib \
minixmldll.dll \
test_minixml.exe \
codecvt.exe \
strrep.exe \
iniset.exe \
renx.exe \
bld_uni_table.exe \
test_fmtcode.exe \
clean


tags=\
htm2txt.exe \
codecvt.exe \
ren1line.exe \
cliptxt.exe


all: $(tags) install clean
	$(RMTMP)
	@$(RM) *.exe

install:
	@cp -f *.exe $(toolpath)

OBJS=ecfile.obj

codec.def: ../../include/codec.h
	$(EXPORTS_H) codec.def ../../include/codec.h

codeclib.lib: $(lib_obj)
	$(LIB) $(lib_obj) /out:$@

codecdll.dll: $(lib_obj)
	$(LINK) /dll $(lib_obj) /out:$@

test_ecfile.exe: test_ecfile.obj $(lib_obj)
	$(LINK) test_ecfile.obj $(lib_obj) /out:$@

test_comp.exe: test_comp.obj $(lib_obj)
	$(LINK) test_comp.obj $(lib_obj) /out:$@

test_codec.exe: test_codec.obj $(lib_obj)
	$(LINK) test_codec.obj $(lib_obj) /out:$@

bld_uni_table.exe: bld_uni_table.obj
	$(LINK) bld_uni_table /out:$@
	
UnWebCompiler.exe: UnWebCompiler.obj Lh5.obj
	$(LINK) UnWebCompiler.obj Lh5.obj /out:$@

strrep.exe: strrep.obj stropt.obj
	$(LINK) strrep.obj stropt.obj /out:$@

ren1line.exe: ren1line.obj
	$(LINK) ren1line.obj /out:$@

$(toolpath)\htm2txt.exe: htm2txt.obj $(lib_obj)
	-$(LINK) htm2txt.obj $(lib_obj) /out:$@

iniset.exe: iniset.obj inifile.obj
	$(LINK) iniset.obj inifile.obj /out:$@
	
codecvt.exe: codecvt.obj $(lib_obj)
	$(LINK) codecvt.obj $(lib_obj) /out:$@

renx.exe: renx.obj strex.obj $(lib_obj)
	$(LINK) renx.obj strex.obj $(lib_obj) /out:$@
	
cliptxt.exe: cliptxt.obj
	$(LINK) cliptxt.obj $(NEEDLIB) /out:$@

minixml.def: minixml.h
	$(EXPORTS_H) minixml.def minixml.h

minixmllib.lib: minixml.obj list.obj
	$(LIB) minixml.obj list.obj /out:$@

minixmldll.dll: minixml.obj list.obj minixml.def
	$(LINK) /dll /def:minixml.def minixml.obj list.obj /out:$@

test_minixml.exe: test_minixml.obj minixml.obj list.obj
	$(LINK) test_minixml.obj minixml.obj list.obj /out:$@

test_fmtcode.exe: test_fmtcode.obj fmtcode.obj
	$(LINK) test_fmtcode.obj fmtcode.obj /out:$@

clean:
	$(RMTMP)
	@$(RM) *.exe

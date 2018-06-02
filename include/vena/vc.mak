
include ..\msvc6.mak

lib_ext_hdrs=libvena.h libvena.txt

NEEDLIB=user32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib 

lib_objs=libvena.obj

tags=\
libvenalib.lib \
libvenadll.dll \
libvenadll.lib

all: clean $(tags) install clean

libvenadll.dll: $(lib_objs)
	$(EXPORTS) libvena.def vena_recog_bf cap_open
	$(LINK) /dll /def:libvena.def $(lib_objs) $(NEEDLIB) /out:$@
	
libvenalib.lib: $(lib_objs)
	$(LIB) $(lib_objs) /out:$@

install:
	-@cp $(lib_ext_hdrs) $(tags) $(libpath)
	-@cp $(lib_ext_hdrs) $(tags) ..\cstd.h ..\sys_*.h .\test_venaMFC
	@pushd $(libpath) & ($(rar) a libvena.rar $(lib_ext_hdrs) libvenalib.lib libvenadll.dll libvenadll.lib >nul) & popd
	@echo ok

clean:
	$(RMTMP)
	

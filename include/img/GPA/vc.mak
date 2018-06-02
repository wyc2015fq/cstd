# 作者：易伟
#echo cmake.bat

include ..\msvc6.mak

lib_ext_hdrs="GPA.h"
CFLAGS = $(CFLAGS) /D "SUPPORT_ALL" /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG"
#/D "_UNICODE" /D "UNICODE"

NEEDLIB=comctl32.lib kernel32.lib user32.lib gdi32.lib advapi32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib

lib_obj=GPA.obj
#~ fmttif.obj

tags = $(libpath)\GPAlib.lib \
$(libpath)\GPAdll.dll \
$(binpath)\test_GPA.exe \
$(binpath)\test_reg.exe

all: version $(tags) install clean

$(libpath)\GPAlib.lib: $(lib_obj)
	-$(LIB) $(lib_obj) /out:$@

$(libpath)\GPAdll.dll: $(lib_obj) GPA.res
	$(EXPORTS) GPA.def GPA_new GPA_update GPA_free
	-$(LINK) /dll /def:GPA.def $(lib_obj) $(NEEDLIB) GPA.res /out:$@

$(binpath)\test_GPA.exe: test_GPA.obj $(lib_obj) ..\imgproc\window.obj ..\imgproc\cap.obj ..\fmtimg\fmtimg.obj
	-$(LINK) test_GPA.obj $(lib_obj) $(NEEDLIB) window.obj cap.obj fmtimg.obj /out:$@
	
$(binpath)\test_reg.exe: test_reg.obj
	-$(LINK) test_reg.obj $(NEEDLIB) /out:$@

install:
	cp $(lib_ext_hdrs) $(incpath)
	cp $(libpath)\GPAdll.dll .
	cp $(libpath)\GPAdll.lib .
	cp $(binpath)\test_GPA.exe .
	cp $(binpath)\test_reg.exe .
	del $(libpath)\GPA.rar
	rar a $(libpath)\GPA.rar $(lib_ext_hdrs) test_GPA_opencv.c 说明文档.txt GPAdll.dll GPAdll.lib $(lib_ext_hdrs)
	cp $(libpath)\GPA.rar .

clean:
	$(RMTMP)

version:
	$(MAKEVERSION) GPA.rc 2 0 0 2 视频去抖动算法库
 
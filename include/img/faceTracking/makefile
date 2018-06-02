# ×÷Õß£ºÒ×Î°

include F:\code\cstd\src\msvc6.mak

PROJNAME=faceTracking
PROJPATH=$(PROJPATH)

SVNPATH=O:\code\svn\chrep\rep\algo\$(PROJNAME)\bin
BINPATH=F:\public\bin

SRC=*.c
OBJECTS=*.obj

CFLAGS = $(INCLUDE) /MD /GX /O2 /D "_USRDLL" /D "_LIB" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD

OBJ = faceTracking.obj camshift.obj tables.obj imcolor.obj

all: version $(OBJ) test_canshift.obj faceTracking.res
	$(EXPORTS_H) faceTracking.def faceTracking.h
	$(EXPORTS) faceTracking.def new_tracker free_tracker camtrack
	$(LINK) /dll /def:faceTracking.def $(OBJ) faceTracking.res FaceDetectlib.lib /out:faceTracking.dll
	$(LIB) *.obj /out:faceTrackinglib.lib
	$(LINK) test_canshift.obj faceTrackinglib.lib FaceDetectlib.lib highguilib.lib /out:test_canshift.exe
	-@mkdir $(PROJPATH) 2>nul
	-@mkdir $(SVNPATH) 2>nul
	-$(COPYTOLIB)
	-$(COPYTOPROJ)
	-$(COPYEXE)
	-$(COPY) /y $(PROJNAME).txt    $(PROJPATH) >nul
	-$(COPY) /y test_canshift.c    $(BINPATH) >nul
	-$(CLEAN)

version:
	$(MAKEVERSION) faceTracking.rc 1 0 0 1 003

tables.obj:
	$(CC) $(CFLAGS) /c ..\highgui\tables.c

imcolor.obj:
	$(CC) $(CFLAGS) /c ..\highgui\imcolor.c

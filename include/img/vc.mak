# ×÷Õß£ºÒ×Î°

include ..\msvc6.mak

OBJ = window.obj cap.obj imdraw.obj color.obj
OBJ_F = fractal.obj fractalart.obj fractal_IFS_system.obj \
fractal_Julia.obj fractal_King.obj fractal_King2.obj \
fractal_L_system.obj fractal_mand.obj fractal_Mira.obj \
fractal_moun.obj fractal_Newton.obj fractal_sierpinski.obj \
fractal_SpotFormat.obj ColorMover.obj fractal_Fire.obj \
fractal_Water.obj fractal_Plasma.obj fractal_Pool.obj \
fractal_Cloud.obj fractal_Newton2.obj ColorMover.obj

NEEDLIB=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib 

EXPORTSNAME=cvNamedWindow cvWaitKey cvShowImage cvDestroyWindow cvCreateTrackbar \
  cvSetMouseCallback cam_start \
	Line Line2 LineAA ThickLine PolyLine PolyLineM FillPolyM \
	DrawEllipse Ellipse2Poly ClipLine DrawRectangle Circle \
	DrawLine DrawCircle DrawHistHsv ScalarToRawData FillConvexPoly \
	DrawLeaf cvPutText cvInitFont cvGetTextSize xDrawText xDrawDigi

lib_ext_hdrs="..\..\include\chalg.h"

tags=$(libpath)\imgproclib.lib \
$(libpath)\imgprocdll.dll \
$(binpath)\test_clickpoint.exe

all: $(tags) install clean

lib_objs2=fmtimg.obj

$(lib_objs2):
	$(CC) /c $(CFLAGS) /D "SUPPORT_ALL" ..\fmtimg\fmtimg.c

$(libpath)\imgproclib.lib: $(OBJ)
	-$(LIB) $(OBJ) /out:$@

$(libpath)\imgprocdll.dll: $(OBJ)
	$(EXPORTS) imgproc.def $(EXPORTSNAME)
	-$(LINK) /dll /def:imgproc.def $(OBJ) $(NEEDLIB) /out:$@

$(binpath)\test_clickpoint.exe: test_clickpoint.obj $(OBJ) $(lib_objs2)
	-$(LINK) test_clickpoint.obj $(lib_objs2) $(OBJ) $(NEEDLIB) /out:$@

install:
	cp $(lib_ext_hdrs) $(incpath)

clean:
	$(RMTMP)

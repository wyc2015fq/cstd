# ×÷Õß£ºÒ×Î°

include ..\msvc6.mak

PROJNAME=imgproc

SRC=*.c

OBJ = window_w32.obj cap.obj xdraw.obj tables.obj outtext.obj imcolor.obj xDrawText.obj xDrawDigi.obj yuv.obj
OBJ_F = fractal.obj fractalart.obj fractal_IFS_system.obj \
fractal_Julia.obj fractal_King.obj fractal_King2.obj \
fractal_L_system.obj fractal_mand.obj fractal_Mira.obj \
fractal_moun.obj fractal_Newton.obj fractal_sierpinski.obj \
fractal_SpotFormat.obj ColorMover.obj fractal_Fire.obj \
fractal_Water.obj fractal_Plasma.obj fractal_Pool.obj \
fractal_Cloud.obj fractal_Newton2.obj

EXPORTSNAME=cvNamedWindow cvWaitKey cvShowImage cvDestroyWindow cvCreateTrackbar \
  cvSetMouseCallback NewCAM_VFW NewAVI_VFW \
	Line Line2 LineAA ThickLine PolyLine PolyLineM FillPolyM \
	DrawEllipse Ellipse2Poly ClipLine DrawRectangle Circle \
	DrawLine DrawCircle DrawHistHsv ScalarToRawData FillConvexPoly \
	DrawLeaf cvPutText cvInitFont cvGetTextSize xDrawText xDrawDigi imcolorcvt

all: test_cam.obj test_drawing.obj $(OBJ) $(OBJ_F) test_gui.obj gui.obj test_paint.obj
	$(EXPORTS) imgproc.def $(EXPORTSNAME)
	$(LIB) $(OBJ) /out:imgproclib.lib
	$(LINK) /dll /def:imgproc.def $(OBJ) /out:imgprocdll.dll
	$(LINK) test_cam.obj imgproclib.lib fmtimglib.lib /out:test_cam.exe
	$(LINK) test_drawing.obj $(OBJ_F) imgproclib.lib fmtimglib.lib /out:test_drawing.exe
	$(LINK) test_paint.obj imgproclib.lib fmtimglib.lib /out:test_paint.exe
	-$(COPYEXE)
	-$(COPYTOLIB)
	-$(COPYTOPROJ)
	-$(CLEAN)
	
# ×÷Õß£ºÒ×Î°

include F:\code\cstd\src\msvc6.mak

PROJNAME=MILTrack
OBJ = candidateWeakClf.obj compute_reult.obj GetOneFeatureValue_2.obj ImageToIntg.obj \
 my_imadjust.obj randomMFeatures.obj randomSelectSample.obj unidrnd.obj MILTrack.obj

CFLAGS = $(INCLUDE) /MD /openmp /GX /O2 /D "_USRDLL" /D "_LIB" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD

all: version test_MILTrack.obj imgseq.obj $(OBJ) MILTrack.res
	$(EXPORTS_H) MILTrack.def MILTrack.h
	$(LINK) /dll /def:MILTrack.def $(OBJ) MILTrack.res /out:MILTrack.dll
	$(LIB) $(OBJ) /out:MILTracklib.lib
	$(LINK) test_MILTrack.obj imgseq.obj $(OBJ) fmtimglib.lib highguilib.lib /out:test_MILTrack.exe
	-@mkdir $(PROJPATH) 2>nul
	-$(COPYEXE)
	-$(COPYTOLIB)
	-$(COPYTOPROJ)
	-$(COPY) /y $(PROJNAME).txt    $(PROJPATH) >nul
	-$(CLEAN)
	-@(del /s *.log>nul) 2>nul

version:
	$(MAKEVERSION) MILTrack.rc 1 0 0 2 001

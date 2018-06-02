# ×÷Õß£ºÒ×Î°

include ..\msvc6.mak
PROJNAME=ml

lib_ext_hdrs="..\..\include\chalg.h"

NEEDLIB = user32.lib highguilib.lib fmtimglib.lib

lib_obj=util.obj matmul.obj matrix.obj svd.obj knearest.obj Clustering.obj kdtree.obj

tags = $(binpath)\test_ml.exe \
$(binpath)\test_Clustering.exe \
$(binpath)\test_kdtree.exe

all: $(tags) install clean

$(binpath)\test_ml.exe: test_ml.obj $(lib_obj)
	$(LINK) test_ml.obj $(lib_obj) $(NEEDLIB) /out:$@
	
$(binpath)\test_Clustering.exe: test_Clustering.obj $(lib_obj)
	$(LINK) test_Clustering.obj $(lib_obj) $(NEEDLIB) /out:$@
	
$(binpath)\test_kdtree.exe: test_kdtree.obj $(lib_obj)
	$(LINK) test_kdtree.obj $(lib_obj) $(NEEDLIB) /out:$@

$(binpath)\test_smo.exe: test_smo.obj $(lib_obj) smo.obj
	$(LINK) test_smo.obj $(lib_obj) smo.obj $(NEEDLIB) /out:$@

install:
	cp $(lib_ext_hdrs) $(incpath)

clean:
	$(RMTMP)

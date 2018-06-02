
include ..\msvc6.mak

lib_ext_hdrs2=../../include/*.h cascadearr.h expression.txt
lib_ext_hdrs=

NEEDLIB=user32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib 

lib_objs=faceRecog.obj
lib_objs2=CM_facerecog.obj

tags1=\
$(libpath)\libfacefeature.dll \
$(libpath)\CM_facerecogdll.dll \
$(libpath)\faceRecoglib.lib \
$(libpath)\faceRecogdll.dll

tags=\
$(libpath)\libfacefeature.dll $(libpath)\test_libfacefeature.exe

all: $(tags) install clean

$(libpath)\libfacefeature.dll: libfacefeature.obj test_libfacefeature.obj
	$(EXPORTS) libfacefeature.def face_detect_defaut test_face_detect_defaut
	$(LINK) /dll /def:libfacefeature.def libfacefeature.obj $(NEEDLIB) /out:$@
	
$(libpath)\test_libfacefeature.exe: $(libpath)\libfacefeature.dll
	$(LINK)  test_libfacefeature.obj libfacefeature.lib $(NEEDLIB) /out:$@
	-@cp test_libfacefeature.c $(libpath)
	cd $(libpath) & $(rar) a libfacefeature.rar *libfacefeature*.* adfasdf.bmp >nul

$(libpath)\CM_facerecogdll.dll: $(lib_objs2)
	$(EXPORTS) CM_facerecog.def CM_StartCam CM_CloseCam CM_CreateFaceLib CM_DeleteFaceLib CM_GetFaceInfo CM_AddFacePack CM_DeleteFacePack CM_GetMaxFacePackLen CM_GetFacePack CM_SetLong HUFF_Decode HUFF_Encode
	$(LINK) /dll /def:CM_facerecog.def $(lib_objs2) $(NEEDLIB) /out:$@
	
$(libpath)\faceRecoglib.lib: $(lib_objs)
	$(LIB) $(lib_objs) /out:$@

$(libpath)\faceRecogdll.dll: $(lib_objs)
	$(EXPORTS) faceRecog.def facerecog_memsize facerecog_process CPM_GetCascade CPM_FaceDetect CPM_EyesDetect CPM_FaceStd CPM_PpLight CPM_Distance CPM_GetFeature CPM_GetRecoAlgoInfo rect_partition
	$(EXPORTS) CM_facerecog.def CM_StartCam CM_CloseCam CM_CreateFaceLib CM_DeleteFaceLib CM_AddFacePack CM_GetMaxFacePackLen CM_GetFacePack CM_SetLong
	$(EXPORTS) faceRecog.def facerecog_memsize facerecog_process
	$(LINK) /dll /def:faceRecog.def $(lib_objs) /out:$@
	
lib_objs2=expression.obj

$(lib_objs2):
	$(CC) /c $(CFLAGS) /D "SUPPORT_ALL" expression.c

expression.res: expression.rc

$(libpath)\expressionlib.lib: $(lib_objs2) expression.res
	$(LIB) $(lib_objs2) expression.res /out:$@

$(libpath)\expressiondll.dll: $(lib_objs2) expression.res
	$(EXPORTS_H) expression.def ..\..\include\expression.h
	-$(LINK) /dll /def:expression.def $(lib_objs2) $(NEEDLIB) expression.res /out:$@

$(binpath)\test_expression.exe: test_expression.obj
	-$(LINK) test_expression.obj $(NEEDLIB) /out:$@
	
$(binpath)\test_ASM.exe: test_ASM.obj
	-$(LINK) test_ASM.obj $(NEEDLIB) /out:$@

install:
	-@cp $(lib_ext_hdrs) $(incpath)
	@cp $(tags) .
	
install2:
	@$(rar) a $(libpath)\expression.rar $(incpath)\expression.h $(libpath)\expressionlib.lib $(libpath)\expressiondll.dll $(libpath)\expressiondll.lib expression.txt test_expression.c test_ASM.c >nul
	@$(rar) a $(libpath)\faceRecog.rar $(incpath)\libfaceRecog.h $(libpath)\faceRecoglib.lib $(libpath)\faceRecogdll.dll $(libpath)\faceRecogdll.lib >nul

clean:
	$(RMTMP)
	$(RM) *.def *.exe *.dll *.lib

expression.rc:
	$(MAKEVERSION) expression.rc 1 0 0 2 004

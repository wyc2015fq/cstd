# Microsoft Developer Studio Project File - Name="CBIRLab" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=CBIRLab - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CBIRLab.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CBIRLab.mak" CFG="CBIRLab - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CBIRLab - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "CBIRLab - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CBIRLab - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_WINDOWS" /D "_DEBUG" /D "_AFXDLL" /D "_MBCS" /YX /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_WINDOWS" /D "_DEBUG" /D "_AFXDLL" /D "_MBCS" /YX /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /robust /win32
# ADD MTL /nologo /D "_DEBUG" /robust /win32
# ADD BASE RSC /l 0x804 /i "$(IntDir)" /d "_DEBUG"
# ADD RSC /l 0x804 /i "$(IntDir)" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib cxcore210.lib cv210.lib highgui210.lib /nologo /subsystem:windows /debug /machine:IX86 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:IX86 /pdbtype:sept

!ELSEIF  "$(CFG)" == "CBIRLab - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Oi /Gy /D "WIN32" /D "_WINDOWS" /D "NDEBUG" /D "_AFXDLL" /D "_UNICODE" /YX /c
# ADD CPP /nologo /MD /W3 /GX /Zi /Oi /Gy /D "WIN32" /D "_WINDOWS" /D "NDEBUG" /D "_AFXDLL" /D "_UNICODE" /YX /c
# ADD BASE MTL /nologo /D "NDEBUG" /robust /win32
# ADD MTL /nologo /D "NDEBUG" /robust /win32
# ADD BASE RSC /l 0x804 /i "$(IntDir)" /d "NDEBUG"
# ADD RSC /l 0x804 /i "$(IntDir)" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:IX86 /pdbtype:sept /opt:ref /opt:icf
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:IX86 /pdbtype:sept /opt:ref /opt:icf

!ENDIF 

# Begin Target

# Name "CBIRLab - Win32 Debug"
# Name "CBIRLab - Win32 Release"
# Begin Group "源"

# PROP Default_Filter "cpp;c;cc;cxx;def;odl;idl;hpj;bat;asm;asmx"
# Begin Source File

SOURCE=.\CBIRLab.cpp
DEP_CPP_CBIRL=\
	".\CBIRLab.h"\
	".\CBIRLabDlg.h"\
	".\stdafx.h"\
	".\targetver.h"\
	{$(INCLUDE)}"cv.h"\
	{$(INCLUDE)}"cv.hpp"\
	{$(INCLUDE)}"cvcompat.h"\
	{$(INCLUDE)}"cvtypes.h"\
	{$(INCLUDE)}"cvver.h"\
	{$(INCLUDE)}"cxcore.h"\
	{$(INCLUDE)}"cxcore.hpp"\
	{$(INCLUDE)}"cxerror.h"\
	{$(INCLUDE)}"cxtypes.h"\
	{$(INCLUDE)}"emmintrin.h"\
	{$(INCLUDE)}"highgui.h"\
	{$(INCLUDE)}"mmintrin.h"\
	{$(INCLUDE)}"xmmintrin.h"\
	
NODEP_CPP_CBIRL=\
	"..\..\..\..\..\..\..\soft\vc6sp6fix\vc98\sdk_2k3\include\emm_func.h"\
	"..\..\..\..\..\..\..\soft\vc6sp6fix\vc98\sdk_2k3\include\xmm_func.h"\
	"..\..\..\..\math\OpenCV1\include\ipl.h"\
	"..\..\..\..\math\OpenCV1\include\ipl\ipl.h"\
	
# End Source File
# Begin Source File

SOURCE=.\CBIRLabDlg.cpp
DEP_CPP_CBIRLA=\
	".\CBIRLab.h"\
	".\CBIRLabDlg.h"\
	".\MatchSIFT.h"\
	".\MySIFT.h"\
	".\RetrievalDlg.h"\
	".\RetrievalResult.h"\
	".\stdafx.h"\
	".\targetver.h"\
	{$(INCLUDE)}"cv.h"\
	{$(INCLUDE)}"cv.hpp"\
	{$(INCLUDE)}"cvcompat.h"\
	{$(INCLUDE)}"cvtypes.h"\
	{$(INCLUDE)}"cvver.h"\
	{$(INCLUDE)}"cxcore.h"\
	{$(INCLUDE)}"cxcore.hpp"\
	{$(INCLUDE)}"cxerror.h"\
	{$(INCLUDE)}"cxtypes.h"\
	{$(INCLUDE)}"emmintrin.h"\
	{$(INCLUDE)}"highgui.h"\
	{$(INCLUDE)}"mmintrin.h"\
	{$(INCLUDE)}"xmmintrin.h"\
	
NODEP_CPP_CBIRLA=\
	"..\..\..\..\..\..\..\soft\vc6sp6fix\vc98\sdk_2k3\include\emm_func.h"\
	"..\..\..\..\..\..\..\soft\vc6sp6fix\vc98\sdk_2k3\include\xmm_func.h"\
	"..\..\..\..\math\OpenCV1\include\ipl.h"\
	"..\..\..\..\math\OpenCV1\include\ipl\ipl.h"\
	
# End Source File
# Begin Source File

SOURCE=.\MatchSIFT.cpp
DEP_CPP_MATCH=\
	".\imgfeatures.h"\
	".\kdtree.h"\
	".\MatchSIFT.h"\
	".\MySIFT.h"\
	".\RetrievalResult.h"\
	".\sift.h"\
	".\stdafx.h"\
	".\targetver.h"\
	".\utils.h"\
	".\xform.h"\
	{$(INCLUDE)}"cv.h"\
	{$(INCLUDE)}"cv.hpp"\
	{$(INCLUDE)}"cvcompat.h"\
	{$(INCLUDE)}"cvtypes.h"\
	{$(INCLUDE)}"cvver.h"\
	{$(INCLUDE)}"cxcore.h"\
	{$(INCLUDE)}"cxcore.hpp"\
	{$(INCLUDE)}"cxerror.h"\
	{$(INCLUDE)}"cxtypes.h"\
	{$(INCLUDE)}"emmintrin.h"\
	{$(INCLUDE)}"highgui.h"\
	{$(INCLUDE)}"mmintrin.h"\
	{$(INCLUDE)}"xmmintrin.h"\
	
NODEP_CPP_MATCH=\
	"..\..\..\..\..\..\..\soft\vc6sp6fix\vc98\sdk_2k3\include\emm_func.h"\
	"..\..\..\..\..\..\..\soft\vc6sp6fix\vc98\sdk_2k3\include\xmm_func.h"\
	"..\..\..\..\math\OpenCV1\include\ipl.h"\
	"..\..\..\..\math\OpenCV1\include\ipl\ipl.h"\
	
# End Source File
# Begin Source File

SOURCE=.\MySIFT.cpp
DEP_CPP_MYSIF=\
	".\imgfeatures.h"\
	".\kdtree.h"\
	".\MySIFT.h"\
	".\sift.h"\
	".\stdafx.h"\
	".\targetver.h"\
	".\utils.h"\
	".\xform.h"\
	{$(INCLUDE)}"cv.h"\
	{$(INCLUDE)}"cv.hpp"\
	{$(INCLUDE)}"cvcompat.h"\
	{$(INCLUDE)}"cvtypes.h"\
	{$(INCLUDE)}"cvver.h"\
	{$(INCLUDE)}"cxcore.h"\
	{$(INCLUDE)}"cxcore.hpp"\
	{$(INCLUDE)}"cxerror.h"\
	{$(INCLUDE)}"cxtypes.h"\
	{$(INCLUDE)}"emmintrin.h"\
	{$(INCLUDE)}"highgui.h"\
	{$(INCLUDE)}"mmintrin.h"\
	{$(INCLUDE)}"xmmintrin.h"\
	
NODEP_CPP_MYSIF=\
	"..\..\..\..\..\..\..\soft\vc6sp6fix\vc98\sdk_2k3\include\emm_func.h"\
	"..\..\..\..\..\..\..\soft\vc6sp6fix\vc98\sdk_2k3\include\xmm_func.h"\
	"..\..\..\..\math\OpenCV1\include\ipl.h"\
	"..\..\..\..\math\OpenCV1\include\ipl\ipl.h"\
	
# End Source File
# Begin Source File

SOURCE=.\RetrievalDlg.cpp
DEP_CPP_RETRI=\
	".\CBIRLab.h"\
	".\CBIRLabDlg.h"\
	".\MatchSIFT.h"\
	".\MySIFT.h"\
	".\RetrievalDlg.h"\
	".\RetrievalResult.h"\
	".\stdafx.h"\
	".\targetver.h"\
	{$(INCLUDE)}"cv.h"\
	{$(INCLUDE)}"cv.hpp"\
	{$(INCLUDE)}"cvcompat.h"\
	{$(INCLUDE)}"cvtypes.h"\
	{$(INCLUDE)}"cvver.h"\
	{$(INCLUDE)}"cxcore.h"\
	{$(INCLUDE)}"cxcore.hpp"\
	{$(INCLUDE)}"cxerror.h"\
	{$(INCLUDE)}"cxtypes.h"\
	{$(INCLUDE)}"emmintrin.h"\
	{$(INCLUDE)}"highgui.h"\
	{$(INCLUDE)}"mmintrin.h"\
	{$(INCLUDE)}"xmmintrin.h"\
	
NODEP_CPP_RETRI=\
	"..\..\..\..\..\..\..\soft\vc6sp6fix\vc98\sdk_2k3\include\emm_func.h"\
	"..\..\..\..\..\..\..\soft\vc6sp6fix\vc98\sdk_2k3\include\xmm_func.h"\
	"..\..\..\..\math\OpenCV1\include\ipl.h"\
	"..\..\..\..\math\OpenCV1\include\ipl\ipl.h"\
	
# End Source File
# Begin Source File

SOURCE=.\RetrievalResult.cpp
DEP_CPP_RETRIE=\
	".\RetrievalResult.h"\
	".\stdafx.h"\
	".\targetver.h"\
	{$(INCLUDE)}"cv.h"\
	{$(INCLUDE)}"cv.hpp"\
	{$(INCLUDE)}"cvcompat.h"\
	{$(INCLUDE)}"cvtypes.h"\
	{$(INCLUDE)}"cvver.h"\
	{$(INCLUDE)}"cxcore.h"\
	{$(INCLUDE)}"cxcore.hpp"\
	{$(INCLUDE)}"cxerror.h"\
	{$(INCLUDE)}"cxtypes.h"\
	{$(INCLUDE)}"emmintrin.h"\
	{$(INCLUDE)}"highgui.h"\
	{$(INCLUDE)}"mmintrin.h"\
	{$(INCLUDE)}"xmmintrin.h"\
	
NODEP_CPP_RETRIE=\
	"..\..\..\..\..\..\..\soft\vc6sp6fix\vc98\sdk_2k3\include\emm_func.h"\
	"..\..\..\..\..\..\..\soft\vc6sp6fix\vc98\sdk_2k3\include\xmm_func.h"\
	"..\..\..\..\math\OpenCV1\include\ipl.h"\
	"..\..\..\..\math\OpenCV1\include\ipl\ipl.h"\
	
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
DEP_CPP_STDAF=\
	".\stdafx.h"\
	".\targetver.h"\
	{$(INCLUDE)}"cv.h"\
	{$(INCLUDE)}"cv.hpp"\
	{$(INCLUDE)}"cvcompat.h"\
	{$(INCLUDE)}"cvtypes.h"\
	{$(INCLUDE)}"cvver.h"\
	{$(INCLUDE)}"cxcore.h"\
	{$(INCLUDE)}"cxcore.hpp"\
	{$(INCLUDE)}"cxerror.h"\
	{$(INCLUDE)}"cxtypes.h"\
	{$(INCLUDE)}"emmintrin.h"\
	{$(INCLUDE)}"highgui.h"\
	{$(INCLUDE)}"mmintrin.h"\
	{$(INCLUDE)}"xmmintrin.h"\
	
NODEP_CPP_STDAF=\
	"..\..\..\..\..\..\..\soft\vc6sp6fix\vc98\sdk_2k3\include\emm_func.h"\
	"..\..\..\..\..\..\..\soft\vc6sp6fix\vc98\sdk_2k3\include\xmm_func.h"\
	"..\..\..\..\math\OpenCV1\include\ipl.h"\
	"..\..\..\..\math\OpenCV1\include\ipl\ipl.h"\
	

!IF  "$(CFG)" == "CBIRLab - Win32 Debug"

# ADD CPP /nologo /GX /Yc"stdafx.h" /GZ

!ELSEIF  "$(CFG)" == "CBIRLab - Win32 Release"

# ADD CPP /nologo /GX /Yc"stdafx.h"

!ENDIF 

# End Source File
# End Group
# Begin Group "头"

# PROP Default_Filter "h;hpp;hxx;hm;inl;inc;xsd"
# Begin Source File

SOURCE=.\CBIRLab.h
# End Source File
# Begin Source File

SOURCE=.\CBIRLabDlg.h
# End Source File
# Begin Source File

SOURCE=.\imgfeatures.h
# End Source File
# Begin Source File

SOURCE=.\kdtree.h
# End Source File
# Begin Source File

SOURCE=.\MatchSIFT.h
# End Source File
# Begin Source File

SOURCE=.\minpq.h
# End Source File
# Begin Source File

SOURCE=.\MySIFT.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RetrievalDlg.h
# End Source File
# Begin Source File

SOURCE=.\RetrievalResult.h
# End Source File
# Begin Source File

SOURCE=.\sift.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\targetver.h
# End Source File
# Begin Source File

SOURCE=.\utils.h
# End Source File
# Begin Source File

SOURCE=.\xform.h
# End Source File
# End Group
# Begin Group "资源"

# PROP Default_Filter "rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;resx;tiff;tif;png;wav"
# Begin Source File

SOURCE=.\res\CBIRLab.ico
# End Source File
# Begin Source File

SOURCE=.\CBIRLab.rc
# End Source File
# Begin Source File

SOURCE=.\res\CBIRLab.rc2
# End Source File
# Begin Source File

SOURCE=".\res\image retrieval.bmp"
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

# Microsoft Developer Studio Project File - Name="SIFTLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SIFTLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SIFTLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SIFTLib.mak" CFG="SIFTLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SIFTLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "SIFTLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SIFTLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_UNICODE" /Gm PRECOMP_VC7_TOBEREMOVED /GZ /c /GX 
# ADD CPP /nologo /MDd /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_UNICODE" /Gm PRECOMP_VC7_TOBEREMOVED /GZ /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 
# ADD RSC /l 1033 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo 
# ADD LIB32 /nologo 

!ELSEIF  "$(CFG)" == "SIFTLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /Zi /W3 /O2 /Oi /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_UNICODE" /Gy PRECOMP_VC7_TOBEREMOVED /c /GX 
# ADD CPP /nologo /MD /Zi /W3 /O2 /Oi /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_UNICODE" /Gy PRECOMP_VC7_TOBEREMOVED /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 
# ADD RSC /l 1033 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo 
# ADD LIB32 /nologo 

!ENDIF

# Begin Target

# Name "SIFTLib - Win32 Debug"
# Name "SIFTLib - Win32 Release"
# Begin Group "源"

# PROP Default_Filter "cpp;c;cc;cxx;def;odl;idl;hpj;bat;asm;asmx"
# Begin Source File

SOURCE=.\imgfeatures.c
# End Source File
# Begin Source File

SOURCE=.\kdtree.c
# End Source File
# Begin Source File

SOURCE=.\minpq.c
# End Source File
# Begin Source File

SOURCE=.\sift.c
# End Source File
# Begin Source File

SOURCE=.\utils.c
# End Source File
# Begin Source File

SOURCE=.\xform.c
# End Source File
# End Group
# Begin Group "头"

# PROP Default_Filter "h;hpp;hxx;hm;inl;inc;xsd"
# Begin Source File

SOURCE=.\imgfeatures.h
# End Source File
# Begin Source File

SOURCE=.\kdtree.h
# End Source File
# Begin Source File

SOURCE=.\minpq.h
# End Source File
# Begin Source File

SOURCE=.\sift.h
# End Source File
# Begin Source File

SOURCE=.\utils.h
# End Source File
# End Group
# Begin Group "资源"

# PROP Default_Filter "rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;resx;tiff;tif;png;wav"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project


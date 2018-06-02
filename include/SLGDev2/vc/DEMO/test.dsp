# Microsoft Developer Studio Project File - Name="test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=test - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "test.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "test.mak" CFG="test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "test - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "test - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "test - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"test.exe"
# SUBTRACT LINK32 /incremental:yes /debug /nodefaultlib

!ELSEIF  "$(CFG)" == "test - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"test.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "test - Win32 Release"
# Name "test - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddUserDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AuthDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateDynamicDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DebugAssistant.cpp
# End Source File
# Begin Source File

SOURCE=.\DevSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DevStatisDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupTimeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuEx.cpp
# End Source File
# Begin Source File

SOURCE=.\PasswordDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Picture.cpp
# End Source File
# Begin Source File

SOURCE=.\RightDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinList.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\test.cpp
# End Source File
# Begin Source File

SOURCE=.\test.rc
# End Source File
# Begin Source File

SOURCE=.\testDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UDDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UPLCaDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UseGDIPlus.cpp
# End Source File
# Begin Source File

SOURCE=.\UserGroupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VideoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WebServiceMrg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddUserDlg.h
# End Source File
# Begin Source File

SOURCE=.\AuthDlg.h
# End Source File
# Begin Source File

SOURCE=.\CreateDynamicDlg.h
# End Source File
# Begin Source File

SOURCE=.\DebugAssistant.h
# End Source File
# Begin Source File

SOURCE=.\DevSet.h
# End Source File
# Begin Source File

SOURCE=.\DevStatisDlg.h
# End Source File
# Begin Source File

SOURCE=.\dynamicPassword.h
# End Source File
# Begin Source File

SOURCE=.\GroupTimeDlg.h
# End Source File
# Begin Source File

SOURCE=.\MenuEx.h
# End Source File
# Begin Source File

SOURCE=.\PasswordDlg.h
# End Source File
# Begin Source File

SOURCE=.\Picture.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RightDlg.h
# End Source File
# Begin Source File

SOURCE=.\SkinList.h
# End Source File
# Begin Source File

SOURCE=.\SkinTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\test.h
# End Source File
# Begin Source File

SOURCE=.\skin\test_CSuperGridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\testDlg.h
# End Source File
# Begin Source File

SOURCE=.\UDDlg.h
# End Source File
# Begin Source File

SOURCE=.\UPLCaDlg.h
# End Source File
# Begin Source File

SOURCE=.\UseGDIPlus.h
# End Source File
# Begin Source File

SOURCE=.\UserGroupDlg.h
# End Source File
# Begin Source File

SOURCE=.\VideoDlg.h
# End Source File
# Begin Source File

SOURCE=.\WebServiceMrg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\device.ico
# End Source File
# Begin Source File

SOURCE=.\res\deviceselect.ico
# End Source File
# Begin Source File

SOURCE=.\res\folders.bmp
# End Source File
# Begin Source File

SOURCE=.\res\right.ico
# End Source File
# Begin Source File

SOURCE=.\res\rightselect.ico
# End Source File
# Begin Source File

SOURCE=.\res\test.ico
# End Source File
# Begin Source File

SOURCE=.\res\test.rc2
# End Source File
# Begin Source File

SOURCE=.\res\week.ico
# End Source File
# Begin Source File

SOURCE=.\res\weekselect.ico
# End Source File
# End Group
# Begin Group "database"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\database\CHCADO.h
# End Source File
# Begin Source File

SOURCE=.\database\CHCADOConnection.cpp
# End Source File
# Begin Source File

SOURCE=.\database\CHCADOConnection.h
# End Source File
# Begin Source File

SOURCE=.\database\CHCADOException.cpp
# End Source File
# Begin Source File

SOURCE=.\database\CHCADOException.h
# End Source File
# Begin Source File

SOURCE=.\database\CHCADORecordset.cpp
# End Source File
# Begin Source File

SOURCE=.\database\CHCADORecordset.h
# End Source File
# Begin Source File

SOURCE=.\database\CHCAutoPtr.h
# End Source File
# Begin Source File

SOURCE=.\database\CHCConnection.cpp
# End Source File
# Begin Source File

SOURCE=.\database\CHCConnection.h
# End Source File
# Begin Source File

SOURCE=.\database\CHCException.cpp
# End Source File
# Begin Source File

SOURCE=.\database\CHCException.h
# End Source File
# Begin Source File

SOURCE=.\database\CHCRecordset.cpp
# End Source File
# Begin Source File

SOURCE=.\database\CHCRecordset.h
# End Source File
# Begin Source File

SOURCE=.\database\CHCType.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

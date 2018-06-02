# Microsoft Developer Studio Project File - Name="Eagleeye2_Viewer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Eagleeye2_Viewer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Eagleeye2_Viewer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Eagleeye2_Viewer.mak" CFG="Eagleeye2_Viewer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Eagleeye2_Viewer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Eagleeye2_Viewer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Eagleeye2_Viewer - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ws2_32.lib FLib.lib /nologo /subsystem:windows /machine:I386 /out:"Release/Eagleeye2_RMVA210SC_Viewer.exe" /libpath:"Lib/"

!ELSEIF  "$(CFG)" == "Eagleeye2_Viewer - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib FLib.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/Eagleeye2_RMVA210SC_Viewer.exe" /pdbtype:sept /libpath:"Lib/"

!ENDIF 

# Begin Target

# Name "Eagleeye2_Viewer - Win32 Release"
# Name "Eagleeye2_Viewer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AfeConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Eagleeye2_Viewer.cpp
# End Source File
# Begin Source File

SOURCE=.\Eagleeye2_Viewer.rc
# End Source File
# Begin Source File

SOURCE=.\Eagleeye2_ViewerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\Eagleeye2_ViewerView.cpp
# End Source File
# Begin Source File

SOURCE=.\GeneralIODlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HFRBINModeCfgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HFRModeCfgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MACCfgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MJPEGModeCfgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewIPMACDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RTCConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TargetIPAddressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TriggerModeCfgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WDTDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WhiteBalanceDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AfeConfigDlg.h
# End Source File
# Begin Source File

SOURCE=.\Eagleeye2_Viewer.h
# End Source File
# Begin Source File

SOURCE=.\Eagleeye2_ViewerDoc.h
# End Source File
# Begin Source File

SOURCE=.\Eagleeye2_ViewerView.h
# End Source File
# Begin Source File

SOURCE=.\GeneralIODlg.h
# End Source File
# Begin Source File

SOURCE=.\HFRBINModeCfgDlg.h
# End Source File
# Begin Source File

SOURCE=.\HFRModeCfgDlg.h
# End Source File
# Begin Source File

SOURCE=.\MACCfgDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MJPEGModeCfgDlg.h
# End Source File
# Begin Source File

SOURCE=.\NewIPMACDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RTCConfigDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelectFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TargetIPAddressDlg.h
# End Source File
# Begin Source File

SOURCE=.\TriggerModeCfgDlg.h
# End Source File
# Begin Source File

SOURCE=.\WDTDlg.h
# End Source File
# Begin Source File

SOURCE=.\WhiteBalanceDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Eagleeye2_Viewer.ico
# End Source File
# Begin Source File

SOURCE=.\res\Eagleeye2_Viewer.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Eagleeye2_ViewerDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar_Disable_new.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar_Enable_new.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

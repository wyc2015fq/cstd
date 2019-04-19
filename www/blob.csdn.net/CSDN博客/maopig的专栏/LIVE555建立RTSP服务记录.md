# LIVE555建立RTSP服务记录 - maopig的专栏 - CSDN博客
2011年10月21日 16:21:06[maopig](https://me.csdn.net/maopig)阅读数：3033
                
 在官网上面
[http://www.live555.com/liveMedia/#config-unix](http://www.live555.com/liveMedia/#config-unix)下载最新源码，并进行编译，同时官网上面告诉了你怎么样编译已经不同平台对应需要修改的内容
一、arm_linux_g++下面编译视频文件LIVE555   【config.armlinux】
CROSS_COMPILE=         arm-none-linux-gnueabi-  ////编译器名称
COMPILE_OPTS =          $(INCLUDES) -I. -O2 -DSOCKLEN_T=socklen_t -DNO_SSTREAM=1 -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64
C =                     c
C_COMPILER =            $(CROSS_COMPILE)gcc
C_FLAGS =               $(COMPILE_OPTS)
CPP =                   cpp
CPLUSPLUS_COMPILER =    $(CROSS_COMPILE)gcc
CPLUSPLUS_FLAGS =       $(COMPILE_OPTS) -Wall -DBSD=1
OBJ =                   o
LINK =                  $(CROSS_COMPILE)g++ -o ////原来是gcc，修为g++就可以解决以上问题。
LINK_OPTS =             
CONSOLE_LINK_OPTS =     $(LINK_OPTS)
LIBRARY_LINK =          $(CROSS_COMPILE)ar cr 
LIBRARY_LINK_OPTS =     $(LINK_OPTS)
LIB_SUFFIX =                    a
LIBS_FOR_CONSOLE_APPLICATION =
LIBS_FOR_GUI_APPLICATION =
EXE =
//=============================================================
#./genMakefiles armlinux
再make或gmake
//=============================================================
若出现collect2: ld returned 1 exit status
make[1]: *** [testMP3Streamer] Error 1
make[1]: Leaving directory `/home/live/testProgs'
make: *** [testProgs/testMP3Streamer] Error 2等错误  make clean之后在重新make,这个时候就可以在mediaServer下面
 看到live555MediaServer服务，这个时候./live555MediaServer就可以建立RTSP服务
二、、linux下面编译视频文件LIVE555 【config.linux】
COMPILE_OPTS =          $(INCLUDES) -I. -O2 -DSOCKLEN_T=socklen_t -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64
C =                     c
C_COMPILER =            cc
C_FLAGS =               $(COMPILE_OPTS)
CPP =                   cpp
CPLUSPLUS_COMPILER =    g++ ////编译器名称 ,c++为g++
CPLUSPLUS_FLAGS =       $(COMPILE_OPTS) -Wall -Wno-deprecated   -DBSD=1
OBJ =                   o
LINK =                  g++ -o
LINK_OPTS =             -L.
CONSOLE_LINK_OPTS =     $(LINK_OPTS)
LIBRARY_LINK =          ar cr
LIBRARY_LINK_OPTS =
LIB_SUFFIX =                    a
LIBS_FOR_CONSOLE_APPLICATION =
LIBS_FOR_GUI_APPLICATION =
EXE =
//=============================================================
#./genMakefiles linux
再make或gmake
//=============================================================
若出现collect2: ld returned 1 exit status
make[1]: *** [testMP3Streamer] Error 1
make[1]: Leaving directory `/home/live/testProgs'
make: *** [testProgs/testMP3Streamer] Error 2等错误  make clean之后在重新make,这个时候就可以在mediaServer下面
 看到live555MediaServer服务，这个时候./live555MediaServer就可以建立RTSP服务
 这个时候会见到下列的一些信息
 ----------------------------------------------------------------
  version 0.73 (LIVE555 Streaming Media library version 2011.10.18).
Play streams from this server using the URL
        rtsp://10.4.1.76:8554/<filename>
where <filename> is a file present in the current directory.
Each file's type is inferred from its name suffix:
        ".264" => a H.264 Video Elementary Stream file
        ".aac" => an AAC Audio (ADTS format) file
        ".ac3" => an AC-3 Audio file
        ".amr" => an AMR Audio file
        ".dv" => a DV Video file
        ".m4e" => a MPEG-4 Video Elementary Stream file
        ".mkv" => a Matroska audio+video+(optional)subtitles file
        ".mp3" => a MPEG-1 or 2 Audio file
        ".mpg" => a MPEG-1 or 2 Program Stream (audio+video) file
        ".ts" => a MPEG Transport Stream file
                (a ".tsx" index file - if present - provides server 'trick play' support)
        ".wav" => a WAV Audio file
See [http://www.live555.com/mediaServer/](http://www.live555.com/mediaServer/) for additional documentation.
(We use port 8000 for optional RTSP-over-HTTP tunneling, or for HTTP live streaming (for indexed Transport Stream files only).)
 ----------------------------------------------------------------
 这个时候打开VLC的rtsp://10.4.1.76:8000/test.264，即可播放相应视频文件
 ---------------------------------------------------------------------
三、window下面编译视频文件LIVE555 【win32config】
# Comment out the following line to produce Makefiles that generate debuggable code:
NODEBUG=1
# The following definition ensures that we are properly matching
# the WinSock2 library file with the correct header files.
# (will link with "ws2_32.lib" and include "winsock2.h" & "Ws2tcpip.h")
TARGETOS = WINNT
# If for some reason you wish to use WinSock1 instead, uncomment the
# following two definitions.
# (will link with "wsock32.lib" and include "winsock.h")
#TARGETOS = WIN95
#APPVER = 4.0
!include    <ntwin32.mak>
UI_OPTS =  $(guilflags) $(guilibsdll)
# Use the following to get a console (e.g., for debugging):
CONSOLE_UI_OPTS =  $(conlflags) $(conlibsdll)
CPU=i386
TOOLS32 =  C:\Program Files\Microsoft Visual Studio\VC98   
//修改相对应的C++的位置信息
COMPILE_OPTS =  $(INCLUDES) $(cdebug) $(cflags) $(cvarsdll) -I. -I"$(TOOLS32)\include"
C =   c
C_COMPILER =  "$(TOOLS32)\bin\cl"
C_FLAGS =  $(COMPILE_OPTS)
CPP =   cpp
CPLUSPLUS_COMPILER = $(C_COMPILER)
CPLUSPLUS_FLAGS = $(COMPILE_OPTS)
OBJ =   obj
LINK =   $(link) -out:
LIBRARY_LINK =  lib -out:
LINK_OPTS_0 =  $(linkdebug) msvcirt.lib
LIBRARY_LINK_OPTS = 
LINK_OPTS =  $(LINK_OPTS_0) $(UI_OPTS)
CONSOLE_LINK_OPTS = $(LINK_OPTS_0) $(CONSOLE_UI_OPTS)
SERVICE_LINK_OPTS =     kernel32.lib advapi32.lib shell32.lib -subsystem:console,$(APPVER)
LIB_SUFFIX =  lib
LIBS_FOR_CONSOLE_APPLICATION =
LIBS_FOR_GUI_APPLICATION =
MULTIMEDIA_LIBS = winmm.lib
EXE =   .exe
PLATFORM = Windows
rc32 = "$(TOOLS32)\bin\rc"
.rc.res:
 $(rc32) $<
//====================================================
genWindowsMakefiles//在命令提示符下面，运行此命令，这个时候会在各个相应的目录下面都会形成.MAK文件
在To use these Makefiles from within Visual Studio, use the "Open Workspace" menu command, then (in the file selection dialog)
 for "Files of type", choose "Makefiles (.mak)". Visual Studio should then prompt you, asking if you want to use this Makefile
 to set up a new project. Say "OK". 
 这个时候需要每个模块都要单独进行编译，最后mediaServer会形成live555MediaServer.exe文件，双击即运行服务
//====================================================

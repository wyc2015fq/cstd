# OpenGL ES 2.0 Programming Guide这本书相关资源下载及编译环境建立说明 - xqhrs232的专栏 - CSDN博客
2010年03月01日 15:00:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：7138
这本书中文名为::OpenGLES2.0编程指导.pdf
pdf格式电子书下载地址：：[http://download.csdn.net/source/1891928](http://download.csdn.net/source/1891928)
原文地址：：[http://www.opengles-book.com/downloads.html](http://www.opengles-book.com/downloads.html)
OpenGL® ES 2.0 Programming Guide - Book Website
Authors: Aaftab Munshi, Dan Ginsburg, Dave Shreiner
Publisher: Addison-Wesley Professional
ISBN: 0321502795 Home Downloads Errata  
Sample Code
The sample code from the book is now available for Windows and iPhone SDK 3.0. Please choose which you would like to download:
iPhone 3G S 
Windows 
--------------------------------------------------------------------------------
iPhone SDK 3.0 - Prerequisites
In order to be able to build and run the code samples, you will need:
Mac OS X 10.5.7 or later 
iPhone SDK 3.0 
The iPhone sample code is available for download from this link: OpenGL_ES_Programming_Guide_iPhone_v1.0.0.tar.gz.
Instructions for building and using the samples are in an e-Chapter on the iPhone 3G S available for download in PDF format from this link: OpenGL_ES_20_Programming_Guide_iPhone_eChapter.pdf
--------------------------------------------------------------------------------
Windows - Prerequisites
In order to be able to build and run the code samples, you will need:
Microsoft Windows XP or Windows Vista 
An OpenGL 2.0-capable Graphics Card 
Microsoft Visual Studio 2005 or Microsoft Visual Studio 2008 (you can also use the free Express Edition available from Microsoft at
[http://www.microsoft.com/express/download/](http://www.microsoft.com/express/download/)).
You will need to download and install the following:
In order to build and run the code samples, you will need AMD's OpenGL ES 2.0 Emulator which can be downloaded from
[http://developer.amd.com/gpu/opengl/Pages/default.aspx](http://developer.amd.com/gpu/opengl/Pages/default.aspx).
In order to view the RenderMonkey example workspaces, you will need to download and install RenderMonkey v1.81. This tool can be downloaded from AMD Developer Central at
[http://developer.amd.com/gpu/rendermonkey/Pages/default.aspx](http://developer.amd.com/gpu/rendermonkey/Pages/default.aspx).
In order to build and run the OpenKODE sample in Chapter 15, you will need Acrodea's OpenKODE 1.0 Implementation for Windows.  This can be downloaded from
[http://www.acrodea.co.jp/en/openkode/](http://www.acrodea.co.jp/en/openkode/) (NOTE: if you do not care to build/run the OpenKODE sample in Chapter 15, you can skip this step).
Examples
All of the code samples and RenderMonkey workspaces can be downloaded from the following link:
[http://www.opengles-book.com/OpenGL_ES_Programming_Guide_v1.0.2.zip](http://www.opengles-book.com/OpenGL_ES_Programming_Guide_v1.0.2.zip)
(Updated 4/19/09 - v1.0.2) Paul Bennett reported two bugs in esGenSphere() in esShapes.c.  The new update includes his fixes.
(Updated 8/24/08 - v1.0.1) We have received reports that some users are having difficulty running the samples on Nvidia GPUs when using the AMD OpenGL ES 2.0 Emulator. To workaround this issue, we updated the sample framework to be compatible with the Imagination
 Technologies PowerVR SDK. For instructions on using the PowerVR SDK, jump here. 
For those users running on an Nvidia GPU and still wanting to use the RenderMonkey samples, Till Rathmann posted a workaround over at the AMD Developer Forums at the following link (7/26/09).
Setup Instructions
Unzip OpenGL_ES_Programming_Guide_v1.0.1.zip to its own folder. 
If you have not done so already, install AMD's OpenGL ES 2.0 Emulator. 
Copy the following files from C:/program files/AMD/OpenGL ES 2.0 Emulator v1.1/bin to the /Bin folder:
libEGL.dll 
libGLESv2.dll 
Copy the following files from C:/program files/AMD/OpenGL ES 2.0 Emulator v1.1/lib to the /Lib folder:
libEGL.lib 
libGLESv2.lib 
For the OpenKODE sample in Chapter 15, place the following file from Acrodea's OpenKODE Implementation to the /Lib folder:
libKD.lib 
Alternative: Setup Instructions Using the PowerVR SDK
Unzip OpenGL_ES_Programming_Guide_v1.0.1.zip to its own folder. 
Download Imagination Technologies Khronos OpenGL ES 2.0 SDK from [http://www.imgtec.com/powervr/insider/sdkdownloads/index.asp](http://www.imgtec.com/powervr/insider/sdkdownloads/index.asp). 
Copy the following files from C:/Imagination Technologies/PowerVR SDK/OGLES2_WINDOWS_PCEMULATION_2.02.22.0756/Builds/OGLES2/WindowsPC/Lib to the /Bin folder:
libEGL.dll 
libGLESv2.dll 
Copy the following files from C:/Imagination Technologies/PowerVR SDK/OGLES2_WINDOWS_PCEMULATION_2.02.22.0756/Builds/OGLES2/WindowsPC/Lib to the /Lib folder:
libEGL.lib 
libGLESv2.lib 
Copy all of the files from C:/Imagination Technologies/PowerVR SDK/OGLES2_WINDOWS_PCEMULATION_2.02.22.0756/Builds/OGLES2/Include to the /Common/Include folder (overwriting the existing headers).
For the OpenKODE sample in Chapter 15, place the following file from Acrodea's OpenKODE Implementation to the /Lib folder:
libKD.lib 

# matlab编译含opencv函数cpp文件 - 三少GG - CSDN博客
2013年05月21日 01:41:59[三少GG](https://me.csdn.net/scut1135)阅读数：1718
个人分类：[PKU Research](https://blog.csdn.net/scut1135/article/category/801457)
cpp裡面有用到opencv的lib, 所以在編譯mex的時候, 需要將opencv的lib也一起加到matlab的環境中.
系統環境: win 7
matalb: matalb 2011a
c++ compiler: visual studio 2008
首先設定matlab所選定的compiler:
**1. matlab command: mex -setup**
(C:\Users\xxx\AppData\Roaming\MathWorks\MATLAB\R2010a\mexopts.bat 為預設的mex option file)
Please choose your compiler for building external interface (MEX) files: 
Would you like mex to locate installed compilers [y]/n? y 
Select a compiler: 
[1] Microsoft Visual C++ 2008 SP1 in C:\Program Files (x86)\Microsoft Visual Studio 9.0 
[0] None 
Compiler: 1
Please verify your choices: 
Compiler: Microsoft Visual C++ 2008 SP1 
Location: C:\Program Files (x86)\Microsoft Visual Studio 9.0 
Are these correct [y]/n? y
*************************************************************************** 
Warning: MEX-files generated using Microsoft Visual C++ 2008 require 
that Microsoft Visual Studio 2008 run-time libraries be 
available on the computer they are run on. 
If you plan to redistribute your MEX-files to other MATLAB 
users, be sure that they have the run-time libraries. 
*************************************************************************** 
Trying to update options file: C:\Users\xxx\AppData\Roaming\MathWorks\MATLAB\R2010a\mexopts.bat 
From template: C:\PROGRA~1\MATLAB\R2010a\bin\win64\mexopts\msvc90opts.bat 
Done . . . 
************************************************************************** 
**2. 修改mexopt.bat**
mex -v 修改mexopts.bat檔：C:\Users\xxx\AppData\Roaming\MathWorks\MATLAB\R2011a\**mexopts.bat**
**主要修改如红色所示:**
**set OPENCVDIR=D:\xxx\OpenCV2.4**
set MATLAB=%MATLAB%
set VSINSTALLDIR=C:\Program Files (x86)\Microsoft Visual Studio 9.0
set VCINSTALLDIR=%VSINSTALLDIR%\VC
rem In this case, LINKERDIR is being used to specify the location of the SDK
set LINKERDIR=C:\Program Files\Microsoft SDKs\Windows\v6.0A\
set PATH=%VCINSTALLDIR%\bin\amd64;%LINKERDIR%\bin\x64;%VCINSTALLDIR%\BIN;%VSINSTALLDIR%\Common7\Tools;%VSINSTALLDIR%\Common7\Tools\bin;%MATLAB_BIN%;**%OPENCVDIR%\build\x64\vc9\bin;**%PATH%
set INCLUDE=%LINKERDIR%\include;%VCINSTALLDIR%\ATLMFC\INCLUDE;%VCINSTALLDIR%\INCLUDE;%VCINSTALLDIR%\VCPackages;%INCLUDE%**;%OPENCVDIR%\build\include**
set LIB=%LINKERDIR%\LIB\x64;%VCINSTALLDIR%\LIB\amd64;%VCINSTALLDIR%\ATLMFC\LIB\amd64;%MATLAB%\extern\lib\win64;%LIB%**;%OPENCVDIR%\build\x64\vc9\lib**
set MW_TARGET_ARCH=win64
rem ********************************************************************
rem Compiler parameters
rem ********************************************************************
set COMPILER=cl
set COMPFLAGS=/c /Zp8 /GR /W3 /EHs /D_CRT_SECURE_NO_DEPRECATE /D_SCL_SECURE_NO_DEPRECATE /D_SECURE_SCL=0 /DMATLAB_MEX_FILE /nologo /MD
set OPTIMFLAGS=/O2 /Oy- /DNDEBUG
set DEBUGFLAGS=/Z7
set NAME_OBJECT=/Fo
rem ********************************************************************
rem Linker parameters
rem ********************************************************************
set LIBLOC=%MATLAB%\extern\lib\win64\microsoft
set LINKER=link
set LINKFLAGS=/dll /export:%ENTRYPOINT% /LIBPATH:"%LIBLOC%" libmx.lib libmex.lib libmat.lib /MACHINE:X64 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /incremental:NO
 /implib:"%LIB_NAME%.x" /MAP:"%OUTDIR%%MEX_NAME%%MEX_EXT%.map" **opencv_core243.lib opencv_highgui243.lib opencv_legacy243.lib opencv_ml243.lib opencv_objdetect243.lib opencv_imgproc243.lib opencv_video243.lib**
set LINKOPTIMFLAGS=
set LINKDEBUGFLAGS=/DEBUG /PDB:"%OUTDIR%%MEX_NAME%%MEX_EXT%.pdb"
set LINK_FILE=
set LINK_LIB=
set NAME_OUTPUT=/out:"%OUTDIR%%MEX_NAME%%MEX_EXT%"
set RSP_FILE_INDICATOR=@
**3. build mex: mex -c xxx.cpp**
===============================
若不想配置上述路径，也可参考下述方法：
### [Matlab与C/C++混合编程调用OpenCV](http://blog.csdn.net/raocong2010/article/details/8785358)
编译代码时仍然是在Matlab命令行下使用mex命令进行编译，不过不同的是需要在编译时指明OpenCV的头文件、静态库文件所在的目录，以及需要链接的OpenCV库的名称。这个部分与在Linux下面使用gcc或者g++进行代码编译是类似的，下面以目前最新的OpenCV 2.4.4库为例子，展示一下如何编写编译选项的参数，假设上面的代码存放在名为OpenCVShowImage.cpp的源文件中。在Matlab的Command
 Line窗口中，我们可以输入以下参数来进行上面代码的编译。
```
mex OpenCVShowImage.cpp -IF:\3rdlibs\OpenCV\include -LF:\3rdlibs\OpenCV\lib 
-lopencv_core244 -lopencv_imgproc244 -lopencv_highgui244
```
 其中-IF:\3rdlibs\OpenCV\include，告诉编译器可以在F:\3rdlibs\OpenCV\include这个目录进行头文件的查找；-LF:\3rdlibs\OpenCV\lib，告诉链接器可以在F:\3rdlibs\OpenCV\lib这个目录进行库文件的查找；最后三个参数表面我们写的源代码需要链接opencv_core244、opencv_imgproc244和opencv_highgui244这三个OpenCV的静态库。
      在执行完毕上面的命令之后，**当前目录下面就会生成一个OpenCVShowImage.mexw64(或32)的文件**，具体的文件名由操作系统是64位还是32位来决定。需要注意的是，如果当前的操作系统是64位的，则在环境变量F:\3rdlibs\OpenCV\bin中以及库文件目录F:\3rdlibs\OpenCV\lib中，必须放置的也是64位版本的OpenCV动态库和静态库。最后便可以使用OpenCVShowImage这个函数了，下面是运行这个函数的结果。
 最后也顺便提及一下，在日本有个学生也自己封装一个Matlab版本的OpenCV库，相关的资料可以在[http://www.cs.stonybrook.edu/~kyamagu/mexopencv/](http://www.cs.stonybrook.edu/~kyamagu/mexopencv/)这个地方看到。

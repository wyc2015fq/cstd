# wince -- 用SYSGEN_CAPTURE实现WINCE PUBLIC文件夹中新增驱动DM9000的本地化（BSP化） - xqhrs232的专栏 - CSDN博客
2014年04月04日 11:24:28[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：672
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://www.cnblogs.com/zym0805/archive/2011/08/04/2127279.html](http://www.cnblogs.com/zym0805/archive/2011/08/04/2127279.html)
- From a command prompt, in your OS design directory, create two subdirectories for the driver. One is for a library corresponding to your driver and the other is for your driver's binary file, which can be a DLL or EXE. **（建立src文件夹的copy，改名为DLL,一个生成lib，另一个生成DLL,在ep93xx\src\Drivers下面建立目录dm9isa,然后在下面建立子目录LIB和DLL）**
For example, to customize Com16550.dll, create a subdirectory named %_TARGETPLATROOT%\Drivers\Com16550\Src for the library and %_TARGETPLATROOT%\Drivers\Com16550\DLL for the binary file.
> 
**Note**   If you were customizing an executable such as Etcha.exe, then you might create %_TARGETPLATROOT%\Apps\Etcha\Src and %_TARGETPLATROOT%\Apps\Etcha\EXE.
- Verify that the library name for your driver module is different from the name for your driver's binary file. **（TARGETNAME从dm9isa要更名为dm9isa_lib）**
In this example, Com16550.dll is built by linking Com16550_lib.lib. Most DLL Sysgen targets use the *_lib.lib naming convention because if you do not differentiate the library file and binary file names, the driver's import library has the same name as the
 static library containing its code. This may result in build problems that are difficult to diagnose accurately.
- Copy the Public directory that contains code for the module that you want to build into the Src directory that you created. In this example, you would copy the files from Public\Common\OAK\Drivers\Serial\Com16550\*.*. **(COPY DM9ISA的源码到ep93xx\src\Drivers\dm9isa\LIB和ep93xx\src\Drivers\dm9isa\DLL)**
- Navigate to the %_TARGETPLATROOT%\Drivers\Com16550\Src directory. Use a text editor to open the Sources file, and then add the following line.
RELEASETYPE=PLATFORM**(在SOURCES文件中增加RELEASETYPE=PLATFORM，这样BUILD之后的LIB文件就会生成到PLATFORM\EP93XX\LIB下面****不然原本会生成到PROJECT目录的\OAK\LIB下面下面)**
If a RELEASETYPE line already exists in the Sources file, then change its value to PLATFORM. This tells the build system that output files should be placed in your OS design's Lib or target directories.
- Verify that the following line appears in the Sources file, and then save the file and close the text editor.
WINCEOEM=1**(Sources文件中添加WINCEOEM=1,去掉WINCETARGETFILE0=$(_COMMONOAKROOT)\lib\$(_CPUINDPATH)\dm9isa.def)**
This tells the build system to include portions of header files that are only available to OEMs who are building run-time images.
- Build the library in %_TARGETPLATROOT%\Drivers\Com16550\Src. If there are any errors, resolve them before proceeding. A successful build creates and places the file Com16550_lib.lib in the appropriate subdirectory
 of %_TARGETPLATROOT%\lib. **（BUILD –C生成LIB到PLATFORM\EP93XX\LIB\dm9isa_lib.lib）**
- Navigate to the directory for your driver's binary file. For example, navigate to %_TARGETPLATROOT%\Drivers\Com16550\DLL. **(切换目录到ep93xx\src\Drivers\dm9isa\DLL)**
- Run Sysgen_capture with the usual Sysgen parameters. For example, type the following line to execute Sysgen_capture with Com16550 as a target.
Sysgen_capture com16550**（执行SYSGEN_CAPTURE DM9ISA，生成souces.DM9ISA）系统会去抓系统MAKEFILE.DEF中模块的一些信息**
This creates one or more files, one of which is a Sources file with the name of the driver as the file extension. For example, a file created using Com16550 as a target would be named Sources.com16550. The Sysgen_capture.bat utility creates one such file
 for each .dll or .exe target created while running your Sysgen command.
- Rename the sources file for your driver as Sources. For example, remove the extension .com16550 from the file name Sources.com16550.**(把souces.DM9ISA更名为sources)**
- Copy one of the selected module's sample Makefiles into your Src directory. At a minimum, you need a Sources file and a Makefile to run Build.exe. **(把LIB中的Makefiles拷贝到DLL中)**
For example, copy the Makefile located in %_TARGETPLATROOT%\Drivers\Com16550\Src into %_TARGETPLATROOT%\Drivers\Com16550\DLL.
- Run Build.exe. This creates the same binary as the standard Sysgen command, and places it in your OS design's target directory, which is a subdirectory of %_TARGETPLATROOT%\target. Examine Build.log to determine where the binary was created.**（在ep93xx\src\Drivers\dm9isa\DLL中，BUILD -C）**
**Note**   This version of the driver uses the standard version of Com16550_lib.lib, not the version that you created in %_TARGETPLATROOT%\Drivers\Com16550\src.
- In %_TARGETPLATROOT%\Drivers\Com16550\Src, open the sources file in a text editor. Remove the TARGETLIBS, or SOURCELIBS, entry for Com16550_lib.lib, and move it to SOURCELIBS, changing the path as appropriate.
 You may need to replace occurrences of %_PUBLICROOT%\common\oak with %_TARGETPLATROOT%. **(把$(_PUBLICROOT)\common\oak\lib\$(_CPUINDPATH)\dm9isa.lib \改为$(_TARGETPLATROOT)\lib\$(_CPUINDPATH)\dm9isa_lib.lib，即链接本地的LIB)**
- Navigate to the directory for your driver's binary file, for example, %_TARGETPLATROOT%\Drivers\Com16550\DLL, and then run Build.exe. Build.exe builds the target binary using your version of Com16550_lib.lib, which you can confirm by examining Build.log.** (链接你自己生成的LIB之后（ep93xx\src\drivers\lib\ dm9isa_lib.lib,DM9ISA的DLL编译，完全本地化）**
**Note**   In order to delete all the object files, you may need to run Build.exe with the **–c** parameter.
- In the %_TARGETPLATROOT%\Drivers\Com16550 directory, create a Dirs file that lists, in order, the Src and DLL, or EXE, directories. This enables you to build your modified version of %_TARGETPLATROOT%\Drivers\Com16550\Src and link an executable with one
 build command. （在**ep93xx\src\drivers\dm9isa目录下创建dirs文件，添加LIB和DLL，注意顺序，先编译LIB,然后生成DLL**）
- Run Build.exe with the **–c** option in the %_PROJECTROOT%\CustomDriver directory and confirm that the contents of both the Src and DLL, or EXE, subdirectories are built, and that the driver links successfully. (在**ep93xx\src\drivers\dm9isa运行BUILD –C即可在本地BSP编译链接，生成DM9ISA.DLL文件，不用SYSGEN整个系统，且避免影响其他平台。**)

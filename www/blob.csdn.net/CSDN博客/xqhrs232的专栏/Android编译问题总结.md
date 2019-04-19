# Android编译问题总结 - xqhrs232的专栏 - CSDN博客
2012年04月26日 15:38:55[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：812标签：[android																[jdk																[command																[flex																[makefile																[file](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=makefile&t=blog)](https://so.csdn.net/so/search/s.do?q=flex&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=jdk&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/yyhzpk/article/details/7078385](http://blog.csdn.net/yyhzpk/article/details/7078385)
**编译Linux 内核需要的工具**
sudo apt-get install ncurses-dev
sudo apt-get update
sudo apt-get install openssh-server openssh-client
**编译Android必备工具，以Ubutu9.04版本为例，安装所需要软件步骤如下:**
1. sudo apt-get install bison
2. sudo apt-get install build-essential
3. sudo apt-get install zlib1g-dev
4. sudo apt-get install flex
5. sudo apt-get install libncurses-dev
6. sudo apt-get install libx11-dev
7. sudo apt-get install gperf
8. 安装jdk1.5.0_21
  1) 拷贝jdk-1_5_0_21-linux-i586.bin到安装目录下面,以根目录为例
  2). 运行jdk-1_5_0_21-linux-i586.bin来安装jdk1.5
     sudo ./jdk-1_5_0_21-linux-i586.bin
  3)安装完之后，会在根目录下面生成一个jdk1.5.0_21目录,这就是我们安装的jdk1.5的目录路径 
  4）在系统根目录/bin/下面创建java程序的链接(注：原理上安装好jdk后，只需要在PATH环境变量里面添加jdk安装路径即可，但实际发现这样做不行，makefile依然只会在/bin/目录下面查找java程序，所以暂时先用此方法)
   sudo ln -s /jdk1.5.0_21/bin/jar /bin/jar
   sudo ln -s /jdk1.5.0_21/bin/java /bin/java
   sudo ln -s /jdk1.5.0_21/bin/javac /bin/javac
   sudo ln -s /jdk1.5.0_21/bin/javah /bin/javah
   sudo ln -s /jdk1.5.0_21/bin/javadoc /bin/javadoc
**常见问题及解决:**
**(1) **/bin/bash: bison: command not found
解决：sudo apt-get install bison
**(2) **make: *** No rule to make target `Please-install-JDK-5.0,-update-12-or-higher,-which-you-can-download-from-java.sun.com',
needed by `out/target/common/docs/api-stubs-timestamp'.  Stop.
解决：安装JDK 5.0, 可参考附录，或者运行以下命令:
       sudo apt-get install sun-java5-jdk
**(3) **/bin/bash: g++: command not found
 make:***[out/host/linuxx86/obj/STATIC_LIBRARIES/libhost_intermediates/pseudolocalize.o] Error 127
解决：sudo apt-get install build-essential
**(4)** external/clearsilver/cgi/cgi.c:22:18: error: zlib.h: No such file or directory
external/clearsilver/cgi/cgi.c: In function ‘cgi_compress’:
......
解决：sudo apt-get install zlib1g-dev
**(5)** /bin/bash: flex: command not found
       make: *** [out/host/linux-       x86/obj/EXECUTABLES/aidl_intermediates/aidl_language_l.cpp] Error 127
解决： sudo apt-get install flex
**(6) **/usr/bin/ld: cannot find -lncurses
collect2: ld returned 1 exit status
make: *** [out/host/linux-x86/obj/EXECUTABLES/adb_intermediates/adb] Error 1
解决：sudo apt-get install libncurses-dev
**(7) **prebuilt/linux-x86/sdl/include/SDL/SDL_syswm.h:55:22: error: X11/Xlib.h: No       such file or directory
       prebuilt/linux-x86/sdl/include/SDL/SDL_syswm.h:56:23: error: X11/Xatom.h:       No such file or directory
    .........
解决：sudo apt-get install libx11-dev
**(8) **sh: gperf: not found
    calling gperf failed: 32512 at ./makeprop.pl line 96.   
    ......................
解决：sudo apt-get install gperf


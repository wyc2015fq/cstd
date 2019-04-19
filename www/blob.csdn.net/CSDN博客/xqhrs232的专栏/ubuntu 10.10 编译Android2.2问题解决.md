# ubuntu 10.10 编译Android2.2问题解决 - xqhrs232的专栏 - CSDN博客
2012年04月26日 15:08:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：889标签：[ubuntu																[android																[command																[file																[flex																[jdk](https://so.csdn.net/so/search/s.do?q=jdk&t=blog)](https://so.csdn.net/so/search/s.do?q=flex&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.163.com/yuyuer_007/blog/static/69133549201181474456812/](http://blog.163.com/yuyuer_007/blog/static/69133549201181474456812/)
参见：[http://blog.csdn.net/izard999/article/details/6699962](http://blog.csdn.net/izard999/article/details/6699962)；
　Ubuntu10.10默认安装的纯净系统，没装任何其他软件。
　　1.下载源码android-2.2，直接make
　　2.出错及解决如下：
　　(1)/bin /bash: bison: command not found
　　解决：sudo apt-get install bison
　　(2)make: *** No rule to make target `Please-install-[jdk](http://dev.10086.cn/cmdn/wiki/index.php?doc-view-1361)-5.0',
　　我的ubuntu 默认的安装的为java6;
　　解决：安装JDK 5.0
　　参见 How to install sun-java5-jdk on Ubuntu 9.10 or Ubuntu 10.10
　　4): 执行 sudo apt-get update 更新源.
　　5): 配置编译环境并下载JDK5：sudo apt-get install sun-java5-jdk
　　(3)/bin/bash: g++: command not found
　　在应用程序->Ubuntu软件中心，选择安装g++；
　　(4)external/clearsilver/cgi/cgi.c:22:18: error: zlib.h: No such file or directory
　　解决：sudo apt-get install zlib1g-dev 
　　(5)/bin/bash: flex: command not found
　　解决：[](mailto:a@ubuntu:~/work/android-2.1_r2$)sudo apt-get install flex
　　(6)/usr/bin/ld: cannot find -lncurses
　　collect2: ld returned 1 exit status
　　解 决： sudo apt-get install libncurses-dev 
　　(7)prebuilt/linux-x86/sdl/include/SDL /SDL_syswm.h:55:22: error: X11/Xlib.h: No such file or directory
　　prebuilt/linux-x86/sdl/include/SDL/SDL_syswm.h:56:23: error: X11/Xatom.h: No such file or directory
　　In file included from external/qemu/android/skin/window.c:19:
　　prebuilt/linux-x86/sdl/include/SDL/SDL_syswm.h:73: error: expected specifier-qualifier-list before ‘XEvent’
　　prebuilt/linux-x86/sdl/include/SDL/SDL_syswm.h:86: error: expected specifier-qualifier-list before ‘Display’
　　make: *** [out/host/linux-x86/obj/EXECUTABLES/emulator_intermediates/android/skin/window.o] Error 1
　　解决： sudo apt-get install libx11-dev
　　(8)sh: gperf: not found
　　calling gperf failed: 32512 at ./makeprop.pl line 96.
　　make: *** [out/target/product/generic/obj/STATIC_LIBRARIES/libwebcore_intermediates/WebCore/css/CSSPropertyNames.h] Error 25
　　make: *** Deleting file `out/target/product/generic/obj/STATIC_LIBRARIES/libwebcore_intermediates/WebCore/css/CSSPropertyNames.h'
　　解 决： sudo apt-get install gperf


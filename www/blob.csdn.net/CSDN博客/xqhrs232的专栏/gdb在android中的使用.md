# gdb在android中的使用 - xqhrs232的专栏 - CSDN博客
2012年05月03日 14:10:12[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1100
原文地址::[http://blog.163.com/bjtornado@yeah/blog/static/69510484201081181657449/](http://blog.163.com/bjtornado@yeah/blog/static/69510484201081181657449/)
相关网帖
1.android中使用gdbserver调试c程序----[http://hi.baidu.com/tmy13/blog/item/c05751a7bfad638fd0435848.html](http://hi.baidu.com/tmy13/blog/item/c05751a7bfad638fd0435848.html)
2.[移动开发:Android中用gdb调试一启动就崩溃的程序](http://www.xue5.com/Mobile/Mobile/Index.html)----[http://www.xue5.com/Mobile/Mobile/603114.html](http://www.xue5.com/Mobile/Mobile/603114.html)
#### A**ndroid环境下使用GDB**
**1) 编译包含Debug 信息设置如下环境变量:**
TARGET_BUILD_TYPE=debug
TARGET_STRIP_MODULE=false (发现即使设置编译为Debug模式, 最后生成的文件系统中的程序和库还是会被strip. 设置此选项可以避免被strip)不过还要需要修改一下build/core/combo/linux-arm.mk, 否者即使设置TARGET_STRIP_MODULE=false也无效
修改
113:  TARGET_STRIP_MODULE:=true
为
ifeq ( $(strip $(TARGET_STRIP_MODULE)),,)
TARGET_STRIP_MODULE := true
endif
然后 make
2) 在板子上启动gdb server
gdb-server IP-address:Port 可执行程序
或者
gdb-server IP-address:Port --attach 程序的PID
(Note: IP-Address是目标板自己的IP地址, 此选项可以省略)
3) 在主机端
(前提: 进入 android 目录, 执行过 source build/envsetup.sh)
#gdbclient 在板子上执行的程序名称
(默认已经设置好如下信息, 无需自己设置
set solib-absolute-prefix /absolute-source-path/out/debug/target/product/product-name/symbols
set solib-search-path /absolute-source-path/out/debug/target/product/product-name/symbols/system/lib)
(gdb) target remote 板子的ip-address:port
(gdb) b main
(gdb) c
程序会停在main函数
补充:
如果板子上找不到gdb-server, 可以到如下地址下载已经做好的: [http://ortegaalfredo.googlepages.com/android](http://ortegaalfredo.googlepages.com/android)
gdb在android开发中的使用相信很多开发人员都很想掌握，今天我们就用gdb在Android Emulator中进行调试，让大家学习gdb调试。在Android手机开发平台的emulator里，有两种方式可以做Gdb调试:直接使用GdbServer或利用Gdb调试进行调试。 
　　1、利用Gdb Server进行调试： 
　　A、从下面的链接中下载针对Android的Gdb Server   http://www.billrocks.org/android_libs/bin/ 
　　B、把下载的Gdb Server安装到你的emulator 中
　　1）、 启动emulator 
　　2）、下载Gdb Server到你的emulator 
　　打开一个终端,运行：
    $ adb shell
    #cd data
    #mkdir bin
　　在另外一个终端里运行：
    $ adb push gdbserver /data/bin 
　　这样通过Android自己的adb，把gdbserver 下载到emulator 中了。 
　　C、运行gdbserver :
     # /data/bin/gdbserver 10.0.2.2:1234 [args...] 
　　D、回到另一终端，映射端口：
       $telnet localhost 5554
      telnet到emulator 中，运行下面的命令：
       redir add tcp:1234:1234 
　　E、在第三个终端中运行 gdb：
       $gdb test
       test 是你要调试的程序，然后在gdb命令行运行：
       (gdb) target remote localhost:1234 
       然后你就可以像以前用gdb那样进行程序调试了。 
　　2、直接用本地Gdb进行调试 
　　A、从下面的链接中下载Android调试包，它是用Android的toolchain编译好的二进制文件： 
      http://ortegaalfredo.googlepages.com/android-debug.tbz 
　　B、解压调试包，然后把gdb下载到你的emulator 中，当然如果你有真的手机，也可以这样调试：
   $adb push gdb /data/bin 
　　C、利用adb连接到emulator 
   $adb shell 
　　运行gdb，会出现下面的信息： 
   # /data/bin/gdb 
dlopen failed on 'libthread_db.so.1' - libthread_db.so.1: cannot open shared object file: No such file or directory
GDB will not be able to debug pthreads. 
GNU gdb 6.7
Copyright (C) 2007 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "--host=arm-none-linux-gnueabi --target=".
(gdb) 
　　D、    然后你就可以像以前用gdb那样对程序进行调试了。

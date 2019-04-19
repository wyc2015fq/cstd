# 深入理解LINUX下动态库链接器/加载器ld-linux.so.2 - xqhrs232的专栏 - CSDN博客
2017年02月17日 14:28:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1119
原文地址::[http://blog.csdn.net/elfprincexu/article/details/51701242](http://blog.csdn.net/elfprincexu/article/details/51701242)
相关文章
1、
# [ld-linux.so.2](http://blog.csdn.net/rdc2008/article/details/21228863)----[http://blog.csdn.net/rdc2008/article/details/21228863](http://blog.csdn.net/rdc2008/article/details/21228863)

最近在[Linux](http://lib.csdn.net/base/linux) 环境下开发，搞了好几天 Compiler 和 linker，觉得有必要来写一篇关于Linux环境下 ld.so的文章了，google上搜索了很多相关介绍性的文档，发现国内百度上却很少有相关类文档，觉得有必要来梳理一下：
ld-linux.so.2 是linux下的动态库加载器/链接器，这篇文章主要来讲一下 ld-linux.so.2 是如何和Linux 以及相关应用打交道的。
# 1. 什么是 ld.linux.so ? 
很多现代应用都是通过动态编译链接的，当一个 需要动态链接 的应用被[操作系统](http://lib.csdn.net/base/operatingsystem)加载时，系统必须要 定位 然后 加载它所需要的所有动态库文件。
 在Linux环境下，这项工作是由ld-linux.so.2来负责完成的，我们可以通过 ldd 命令来查看一个 应用需要哪些依赖的动态库:
$ ldd `which ls`
      linux-gate.so.1 =>  (0xb7fff000)
      librt.so.1 => /lib/librt.so.1 (0x00b98000)
      libacl.so.1 => /lib/libacl.so.1 (0x00769000)
      libselinux.so.1 => /lib/libselinux.so.1 (0x00642000)
      libc.so.6 => /lib/libc.so.6 (0x007b2000)
      libpthread.so.0 => /lib/libpthread.so.0 (0x00920000)
      /lib/ld-linux.so.2 (0x00795000)
      libattr.so.1 => /lib/libattr.so.1 (0x00762000)
      libdl.so.2 => /lib/libdl.so.2 (0x0091a000)
      libsepol.so.1 => /lib/libsepol.so.1 (0x0065b000)
当最常见的ls小程序加载时，操作系统会将 控制权 交给 ld-linux.so 而不是 交给程序正常的进入地址。 ld-linux.so.2 会寻找然后加载所有需要的库文件，然后再将控制权交给应用的起始入口。
**上面的ls在启动时，就需要ld-linux.so加载器将所有的动态库加载后然后再将控制权移交给ls程序的入口。**
ld-linux.so.2 man page给我们更高一层的全局介绍， 它是在 链接器（通常是ld）在运行状态下的部件，用来定位和加载动态库到应用的运行地址（或者是运行内存）当中去。通常，动态链接是 在连接阶段当中 隐式指定的。 gcc -W1 options -L/path/included -lxxx 会将 options 传递到ld 然后指定相应的动态库加载。 ELF 文件提供了相应的加载信息， GCC包含了一个特殊的 ELF 头： INTERP， 这个 INTERP指定了 加载器的路径，我们可以用readelf
 来查看相应的程序
$ readelf -l a.out
Elf file type is EXEC (Executable file)
Entry point 0x8048310
There are 9 program headers, starting at offset 52
Program Headers:
  Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align
  PHDR           0x000034 0x08048034 0x08048034 0x00120 0x00120 R E 0x4
  INTERP         0x000154 0x08048154 0x08048154 0x00013 0x00013 R   0x1
      [Requesting program interpreter: /lib/ld-linux.so.2]
  LOAD           0x000000 0x08048000 0x08048000 0x004cc 0x004cc R E 0x1000
  LOAD           0x000f0c 0x08049f0c 0x08049f0c 0x0010c 0x00110 RW  0x1000
. . .
ELF 规格要求，假如 PT_INTERP 存在的话，操作系统必须创建这个 interpreter文件的运行映射，而不是这个程序本身， 控制权会交给这个interpreter，用来定位和加载所有的动态库，

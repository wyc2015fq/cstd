# WINCE下BIN文件转换成NB0文件 - xqhrs232的专栏 - CSDN博客
2010年04月15日 19:49:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2064
原文地址：：[http://blog.csdn.net/orangeman1982112/archive/2008/10/22/3122542.aspx](http://blog.csdn.net/orangeman1982112/archive/2008/10/22/3122542.aspx)
关于cvrtbin的用途，最常用的就是将NK.bin转换出NK.nb0。用法如下：
cvrtbin [options] [filename]
-s 从bin文件中产生sre文件
-r 从bin文件中产生rom文件
-a rom文件的起始地址
-w 总线的宽度
-l rom文件的大小
这个工具可以和viewbin工具一起使用，将NK.bin文件转换成NK.nb0。
首先通过：viewbin -nk.bin命令获得NK.bin的起始地址和大小，然后通过下面的命令：
cvrtbin -a C02C0000 -l 00D5E380 -w 32 -r NK.bin
将NK.bin转换成NK.nb0。起始地址为0xC02C0000，大小是0x00D5E380，总线宽度为32bit，最后输出NK.nb0. 
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/orangeman1982112/archive/2008/10/22/3122542.aspx](http://blog.csdn.net/orangeman1982112/archive/2008/10/22/3122542.aspx)
//===========================
提取NB0/BIN文件里面的DLL/EXE用------M8解包工具220++++rom超级工具!-----dumprom.exe
反编译DLL/EXE文件用-------IDA这个IDE来做!
//=========================
三个常用工具-------viewbin.exe++++cvrtbin.exe++++dumprom.exe!

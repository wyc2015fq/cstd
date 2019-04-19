# WinCE中的Data/ Prefetch Abort等异常定位----通过RA的地址定位 - xqhrs232的专栏 - CSDN博客
2013年04月16日 10:59:05[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：828
原文地址::[http://www.cnblogs.com/xFreedom/archive/2011/05/14/2046191.html](http://www.cnblogs.com/xFreedom/archive/2011/05/14/2046191.html)
在调试WinCE程序的时候，有时候会碰到Data/Prefetch Abort的异常，相信从事过WinCE开发的人对这种异常信息应该都不会陌生，系统会在调试控制台输出如下类似信息：
Exception 'Prefetch Abort' (3): Thread-Id=05870016(pth=9970c000), Proc-Id=057c0016(pprc=9973cdd4)‘TCPClient.exe’,VM-active=057c0016(pprc=9973cdd4) 'TCPClient.exe'PC=00000004(???+0x00000004) RA=00011254(TCPClient.exe+0x00001254)
 SP=0011f954, BVA=00000004
如下图：
![](http://pic002.cnblogs.com/images/2011/300228/2011051412503986.jpg)
Prefetch Abort和Data Abort的定位方法一样。
其中RA=00011254就表示产生异常的地址，也可以通过TCPClient.exe+0x00001254中的
0x00001254 + 0x00010000 = RA=00011254来计算。为什么是加上0x00010000呢？
打开map文件，找到Preferred load address is 00010000的一段信息，你就明白要加上多少了。
在map文件中找到比0x000011254小且最接近的一个值，本程序中为0x00011108，如下图：
![](http://pic002.cnblogs.com/images/2011/300228/2011051412505894.jpg)
就可以判断是该函数中出了问题，要想找到具体出错在那一行，还得利用.cod文件进行定位。
SocketThreadFunc函数的起始地址为0x00011108，错误处的偏移地址为：
0x﻿14C﻿ = 000011254 - 0x00011108(可以用windows计算器计算)。计算出偏移地址后，打开cod文件，寻找0x14C﻿出现的位置，可以用记事本打开cod文件，用Ctrl+F查找14C，会定位到如下信息：
![](http://pic002.cnblogs.com/images/2011/300228/2011051412513760.jpg)
前边的;127就表示错误发生在代码中的第127行，分号应该是注释吧。
VS2005默认不会生成.map文件和.cod文件，修改以下两处就可以生成这两个文件了：
1.(.map)文件：在工程目录上右键->Properties->Configuration Properties->Linker->Debugging->Generate Map File(选择Yes(/MAP));
2.(.cod)文件：在工程目录上右键->Properties->Configuration Properties->C/C++->Output Files->Assembler Output(选择Assembly，Machine Code and Source(/FAcs))。
//================================================================
备注：：
1》这篇文章提到了VS2005 APP下怎么产生MAP跟COD文件，很方便调试！！！
2》其实通过PC指针的值也是可以定位到异常的函数名及OBJ模块的！！！

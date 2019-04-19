# Wince 的编译选项 (WinCE6.0 build options) - xqhrs232的专栏 - CSDN博客
2013年09月16日 14:28:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1028
原文地址::[http://blog.csdn.net/linuxandroidwince/article/details/6197842](http://blog.csdn.net/linuxandroidwince/article/details/6197842)
相关网帖
1、[WinCE6.0
 build options](http://www.cnblogs.com/xilentz/archive/2010/05/27/1745500.html)----[http://www.cnblogs.com/xilentz/archive/2010/05/27/1745500.html](http://www.cnblogs.com/xilentz/archive/2010/05/27/1745500.html)
2、WinCE BSP中打印信息的实现介绍----[http://tech.ddvip.com/2008-09/122060987460906.html](http://tech.ddvip.com/2008-09/122060987460906.html)
3、
### [wince debug信息实现：DEBUGMSG，RETAILMSG，NKDbgPrintfW的实现](http://blog.csdn.net/bbw2008/article/details/6087738)----[http://blog.csdn.net/bbw2008/article/details/6087738](http://blog.csdn.net/bbw2008/article/details/6087738)
4、
platform builder build option介绍 
 ----[http://hgh123.blog.163.com/blog/static/59804221200941474421418](http://hgh123.blog.163.com/blog/static/59804221200941474421418/)[/](http://hgh123.blog.163.com/blog/static/59804221200941474421418/)
5、wince串口打印信息的屏蔽与打开 ----[http://motion.chinaitlab.com/WINCE/879558.html](http://motion.chinaitlab.com/WINCE/879558.html)
**Buffer Tracked Events In RAM**
选取这个, 将会在image中多出一个OSCapture.exe, 可以记录系统日志到文件
**Enable Eboot Space In Memory**
选这个,在os启动阶段, Eboot可以传输数据给os
**Enable Event Tracking During Boot**
选择这个, 系统日志跟踪启动早于内核和文件系统初始化前.
**Enable Hardware-Assisted Debugging Support**
新增选项.说是第三方硬件调试工具需要的(JTAG probes compiant with exdi2).管它去死.
**Enable Kernel Debugger**
需要kitl支持, 内核调试器. 可以step through code. 内核可以单步断点?不行吧,,,暂时还不想去调试内核.
**Enable KITL**
把kitl加入image.
**Enable Profiling**
在image增加profiler功能, 性能测试功能.
**Enable Ship Build**
以前看源码, 这个SHIP_BUILD宏, 会把所有的输出, RETAILMSG, DEBUGMSG都给mask掉了.
**Flush Tracked Events To Release Directory**
往image中加入CeLogFlush.exe, 自动把OSCapture.exe手机到的日子数据写入Celog.clg文件中.注意写入的是你的电脑release目录的文件哦?! 不知道怎么实现的.
**Run-Time Image Can Be Larger Than 32MB**
自己翻译
**Use Xcopy Instead Of Links To Populate Release Directory**
用xcopy命令替代copylink. 就是真的拷贝还是只是弄个link的区别.这个link和快捷方式应该不完全一样. 可能是linux的那样的, windows什么时候也支持link了?不知道…
**Write Run-Time Image To Flash Memory**
指示Eboot下载image到flash…这…到底要干吗?(更正,download后的kernel在RAM中,选中该项则会下载到flash中)
**对比WinCE5.0的, 少了**
**Enable CE Target control Support.**
这个功能还是有用的啊, 没有kitl和activesync时候, 还是可以gi all一下.不过, 我在ce6上面试过了.这个功能并没有取消.可能是默认支持了.所以去掉这个
**Enable Full Kernel Mode**
嗯哼~. WinCE6.0的内核都重写了. 地址空间也发生天翻地覆的变化.这个肯定要去掉了.
微软的 ：
**Enable CE Target Control Support (SYSGEN_SHELL=1)**
为 OS 设计启用 CE 目标控制支持会为您的 OS 设计启用目标控制支持，并且启用内核独立传输层 (KITL)。有关详细信息，请参阅[目标控制
 (CESH) 服务](http://msdn.microsoft.com/library/en-us/wcedebug5/html/wce50conTargetControlPPFSCESHService.asp)。
**Enable Eboot Space in Memory (IMGEBOOT=1)**
选择该选项可以在 OS 设计的 Config.bib 文件中保留空间，从而使启动加载程序能够保存数据以供 OS 在启动时读取。
**Enable Full Kernel Mode (no IMGNOTALLKMODE=1)**
为 OS 设计启用完全内核模式可以提高运行库映像中的系统性能。有关详细信息，请参阅[完全内核模式](http://msdn.microsoft.com/library/en-us/wcecoreos5/html/wce50conFullKernelMode.asp)。
**Enable KITL (no IMGNOKITL=1)**
为 OS 设计启用完全内核模式可以提高运行库映像中的系统性能。有关详细信息，请参阅[完全内核模式](http://msdn.microsoft.com/library/en-us/wcecoreos5/html/wce50conFullKernelMode.asp)。
**Run-Time Image Can be Larger than 32 MB (IMGRAM64=1)**
通过使运行库映像能够大于 32 MB，您可以具有更大的运行库映像。如果最终的运行库映像需要 32 MB 以上的空间，则生成过程可能无法成功完成。
**Enable Kernel Debugger (no IMGNODEBUGGER=1)**
通过启用对内核调试器的支持，您可以调试基于您的 OS 设计创建的运行库映像。

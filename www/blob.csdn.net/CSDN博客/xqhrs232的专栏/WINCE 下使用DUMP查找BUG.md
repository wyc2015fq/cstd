# WINCE 下使用DUMP查找BUG - xqhrs232的专栏 - CSDN博客
2014年11月06日 14:55:47[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：806
原文地址::[http://blog.sina.com.cn/s/blog_7ffab91701016fbj.html](http://blog.sina.com.cn/s/blog_7ffab91701016fbj.html)
相关文章
1、如何用WinDbg或Waston Dump Viewer分析WinCE机台上抓取的Dump File----[http://bbs.csdn.net/topics/280071399](http://bbs.csdn.net/topics/280071399)
2、[如何诊断Windows CE的应用程序崩溃](http://blog.csdn.net/singlerace/article/details/2655154)----[http://blog.csdn.net/singlerace/article/details/2655154](http://blog.csdn.net/singlerace/article/details/2655154)
3、WinCE下异常捕获方法----[http://wenku.baidu.com/link?url=FBH-vati4frRuHz_pzO9m-_QJSYzNF9CC_4_G7Pngsx3HtByQs7Rq0MHjJ4BPVH7a6ZQXHRLTBm0_80mqzCkFTKx-vbJojEf2s_BEyzCbHe](http://wenku.baidu.com/link?url=FBH-vati4frRuHz_pzO9m-_QJSYzNF9CC_4_G7Pngsx3HtByQs7Rq0MHjJ4BPVH7a6ZQXHRLTBm0_80mqzCkFTKx-vbJojEf2s_BEyzCbHe)
4、[SEH异常处理学习总结](http://www.cppblog.com/guojingjia2006/archive/2013/03/21/198679.html)----[http://www.cppblog.com/guojingjia2006/archive/2013/03/21/198679.html](http://www.cppblog.com/guojingjia2006/archive/2013/03/21/198679.html)
5、[SEH异常处理](http://blog.csdn.net/liwei8703/article/details/4773501)----[http://blog.csdn.net/liwei8703/article/details/4773501](http://blog.csdn.net/liwei8703/article/details/4773501)
CE系统有WER（Windows Error Reporting）模块，可以帮助我们找到系统crash的状态。任何应用程序，不管是本地的还是托管的，都可以使用。举例如下：
这个程序运行后，会产生除0的错误。设备加入了WER模块，就会弹出一个消息框，询问是否发送给微软。
消息中有2个链接，其中第二个链接（To View technical information contained in this error report）打开后：
提示上传2个文件，打开这2文件, 第一个是报告,第二个是内存数据。
打开My Device/Windows/System/DumpFiles的目录，这是一个隐藏目录，需要设置文件夹属性为show all才能看到。
文件夹中还有一个是CE打头的目录，文件夹名字是根据crash时候的时间建立。如果上述操作中，选择发送报告给微软，那么这个目录就会被删除。
打开报告文件，需要使用WinDbg工具，它包含在微软提供的debugging tools for windows包中。
启动WinDbg，打开报告：
这里可以看到，发生了一个除0异常
[](http://www.windowsfordevices.com/files/article022/798a52de.jpg)
下面窗口指出错误产生的位置和文件
打开stack frame，可以看到对应源代码的内容：
[](http://www.windowsfordevices.com/files/article022/m44b34424)
最后，locals窗口可以看到变量i的值是0
[](http://www.windowsfordevices.com/files/article022/123bdd0b.jpg)
在CE5中，已经加入了这个功能的支持。可在catalog中选择，并生成新的镜像。
使用错误报告后，当程序crash了，设备可以自动把设备当时的状态记录下来。错误报告会生成一个dump文件，里面包含了许多有用的信息，能帮助我们调试和测试程序。
错误报告生成器负责以上工作，它可以在注册表中进行参数设置。其中dump文件的格式和微软Watson网站上的一致的。
要生成错误报告的dump文件，需要至少有128K的内存。OAL层初始化函数OEMInit中，变量dwNKDrWatsonSize就是记录这个内存的大小。内核会从主内存区域的最后，保留出这一块内存。设置编译环境变量SYSGEN_WATSON_DMPGEN，让镜像包含有错误报告生成器的功能。
注册表中，有保存错误报告生成器的设置，如下：
[](http://www.windowsfordevices.com/files/article022/2beb0c62.jpg)
错误报告传输驱动（Error Report Transfer Driver），会把这些设置放到上述的保留内存中。那么错误报告生成器就能创建正确的内容。这些信息告诉它那里生成报告和生成何种报告。上述注册表设置是使用system dump，使用4倍保留内存的磁盘空间去存放报告。
Dump文件的各个类型，有统一的格式：
1、 context dump
4K到64K大小
crash系统的信息
发生错误线程的context记录
发生错误线程的模块列表
发生错误线程的线程列表
发生错误线程的调用栈
发生错误线程中，发生错误位置上下64字节的内存内容
发生错误线程的堆栈，最大保存64K
2、 system dump
64K以上
Context dump的所有信息
所有线程的context记录和调用栈
所有模块、进程、线程的列表
发生错误线程中，发生错误位置上下2K字节的内存内容
Crash时刻中的全局变量
3、 complete dump
所有使用中内存的内容，至少64K大小
Context dump的所有信息
错误报告生成的文件中，开头是一个MINIDUMP_HEADER结构体。后面是一些MINIDUMP_DIRECTORY的入口，描述的是数据类型，如系统信息和异常信息等。数据的大小也会记录下来，还有数据存放的偏移地址。详细参考$(_COMMONOAKROOT)/INC/DwCeDump.h中内容。
错误报告传输驱动，负责把设置信息从注册表中，放到保留内存中。同时把生成的文件，复制到可持续保存的介质中（nandflash、SD）。接着它还要通过注册表，启动上传报告的客户端程序。
镜像要有错误报告传输驱动，需要设置编译环境变量SYSGEN_WATSON_XFER。在注册表中，还要设置相关属性。如下，错误报告传输驱动设置为：每5分钟唤醒一次，优先级是249。
[](http://www.windowsfordevices.com/files/article022/m6a08c83b.jpg)
错误报告控制面板，也可设置相关属性，包括：
1、 打开、关闭错误报告（有显示的设备中，错误报告默认是打开的。无显示设备中，模式是关闭）
2、 设置dump文件的可用空间大小
3、 打开用户通知对话框
[](http://www.windowsfordevices.com/files/article022/3856119f.jpg)
需要使用错误报告控制面板，编译时候要设置SYSGEN_WATSON_CTLPNL。
错误报告控制面板的注册表内容，存放在HKLM/System/ErrorReporting/DumpSettings和路径HKLM/System/ErrorReporting/UploadSettings中。
报告上传的客户端，负责把错误报告发送到watson.microsoft.com网站上。也可以发送到其他网站，但是需要修改代码,在(_PUBLICROOT) /WCESHELLFE/OAK/WATSON/DWUI/ DWUIDLGS.CPP。另外，(_PUBLICROOT) /COMMON/OAK/INC/DWPUBLIC.H中，需要定义一个有效的服务器VALID_RESPONSE_SERVER。
当应用程序crash后，Dr.Watson开始工作，调用GenerateDumpFileContent函数。这个函数完成大部分的工作，它首先通过设置高优先级，来确保Dr.Watson不被抢占。然后搜集系统、模块、异常、进程和线程等信息，放到文件的CRASH_DATA结构体中。当信息收集完毕，它会恢复到原来状态，写入crash信息，生成dump文件。
在Dr.Watson的帮助下，福尔摩斯们就能通过错误报告工具，去分析和调试程序了。
还可以通过以下方式进行调试：
**一、使用PB**
1、开启错误报告, 设备上的“开始”-〉“设置”-〉“系统”-〉“错误报告”-〉“启用错误报告”。
2、日常使用（不连接KITL）时，程序发生崩溃，会提示是否把错误报告给微软，此时选择“发送”（注：如果选择“不发送”下面的步骤就不要看了）。这样会生成 “ce+日期+序号.kdmp”文件。
3、连接USB ActiveSync，在设备windows/system/dumpfiles下会多一个“ce月日年-序号”的目录（例如：“Ce032406-1” ）其下可以发现 ce+日期+序号.kdmp文件 和 cedevice.log 文件。
4、用 Remote File Viewer （PB或VS2005自带的远程工具）将这两个文件download回到电脑上。注意用ActiveSync是无法直接拷贝的,Copy会被告知 AccessDenied。
5、用PB-〉File Open打开 xxx.kdmp 文件，之后点击 Target -〉 Attach 程序就可以恢复到崩溃时候的状态了。可以看到程序崩溃时候的 call stack/ memory / process 调试了。（至少能定位到时哪个程序调用的时候崩溃了）
**二、使用UIDumpViewer.exe**
微软还提供了一个调试工具，可以在PC上直接查看dump文件，链接地址：
http://www.microsoft.com/downloads/details.aspx?FamilyID=76B18828-09E4-4A87-A8E4-A06F2352B754&displaylang=en&displaylang=en
**三、根据输出的提示信息，使用CrashFinder**
这个方式需要有源代码。
编译工程后，按照出错的提示信息，add一个出错的文件（exe、dll）。
输入crash地址，工具就会列出具体信息。
下载地址：
http://www.wintellect.com/CS/blogs/jrobbins/archive/2006/04/19/crashfinder-returns.aspx
[http://www.microsoft.com/msj/0498/bugslayer0498.aspx](http://www.microsoft.com/msj/0498/bugslayer0498.aspx)

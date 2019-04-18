# WINDBG分析DMP方法 - gauss的专栏 - CSDN博客
2013年01月04日 00:18:45[gauss](https://me.csdn.net/mathlmx)阅读数：386
个人分类：[开发工具/调试技巧](https://blog.csdn.net/mathlmx/article/category/1317877)
上次发了2100蓝屏抓DMP分析案例这个帖子后，好多人想学怎么分析DMP，那我也是刚刚学的，简单的说下。
[http://bbs.icafe8.com/forum.php?mod=viewthread&tid=399075&fromuid=30123](http://bbs.icafe8.com/forum.php?mod=viewthread&tid=399075&fromuid=30123)
1：先下载附件WinDbg，解压出来后，直接运行里面的windbg.exe。（或者可以自已去网上下的）
![](http://bbs.icafe8.com/static/image/filetype/rar.gif)[WinDbg.part11.rar](http://bbs.icafe8.com/forum.php?mod=attachment&aid=NTk5ODl8YjgyNGE2YTJ8MTM0NDU1NTQ5NHwwfDQwMDEwNA%3D%3D)*(465.13 KB, 下载次数: 84)*
![](http://bbs.icafe8.com/static/image/filetype/rar.gif)[WinDbg.part10.rar](http://bbs.icafe8.com/forum.php?mod=attachment&aid=NTk5ODh8MTNmOTZiMGZ8MTM0NDU1NTQ5NHwwfDQwMDEwNA%3D%3D)*(1.39 MB, 下载次数: 97)*
![](http://bbs.icafe8.com/static/image/filetype/rar.gif)[WinDbg.part09.rar](http://bbs.icafe8.com/forum.php?mod=attachment&aid=NTk5ODd8YmE1OGQyN2J8MTM0NDU1NTQ5NHwwfDQwMDEwNA%3D%3D)*(1.39 MB, 下载次数: 80)*
![](http://bbs.icafe8.com/static/image/filetype/rar.gif)[WinDbg.part08.rar](http://bbs.icafe8.com/forum.php?mod=attachment&aid=NTk5ODZ8MTI0YThkMTB8MTM0NDU1NTQ5NHwwfDQwMDEwNA%3D%3D)*(1.39 MB, 下载次数: 86)*
![](http://bbs.icafe8.com/static/image/filetype/rar.gif)[WinDbg.part07.rar](http://bbs.icafe8.com/forum.php?mod=attachment&aid=NTk5ODV8NDA4OWFiNDJ8MTM0NDU1NTQ5NHwwfDQwMDEwNA%3D%3D)*(1.39 MB, 下载次数: 79)*
![](http://bbs.icafe8.com/static/image/filetype/rar.gif)[WinDbg.part06.rar](http://bbs.icafe8.com/forum.php?mod=attachment&aid=NTk5ODR8OTU5NzY1NDR8MTM0NDU1NTQ5NHwwfDQwMDEwNA%3D%3D)*(1.39 MB, 下载次数: 98)*
![](http://bbs.icafe8.com/static/image/filetype/rar.gif)[WinDbg.part05.rar](http://bbs.icafe8.com/forum.php?mod=attachment&aid=NTk5ODN8ZDZlODA3Yzl8MTM0NDU1NTQ5NHwwfDQwMDEwNA%3D%3D)*(1.39 MB, 下载次数: 86)*
![](http://bbs.icafe8.com/static/image/filetype/rar.gif)[WinDbg.part04.rar](http://bbs.icafe8.com/forum.php?mod=attachment&aid=NTk5ODJ8OTY5NzZjNzB8MTM0NDU1NTQ5NHwwfDQwMDEwNA%3D%3D)*(1.39 MB, 下载次数: 94)*
![](http://bbs.icafe8.com/static/image/filetype/rar.gif)[WinDbg.part03.rar](http://bbs.icafe8.com/forum.php?mod=attachment&aid=NTk5ODF8ZmNlYjI2ZGR8MTM0NDU1NTQ5NHwwfDQwMDEwNA%3D%3D)*(1.39 MB, 下载次数: 82)*
![](http://bbs.icafe8.com/static/image/filetype/rar.gif)[WinDbg.part02.rar](http://bbs.icafe8.com/forum.php?mod=attachment&aid=NTk5ODB8ZWE0ZWIwOGJ8MTM0NDU1NTQ5NHwwfDQwMDEwNA%3D%3D)*(1.39 MB, 下载次数: 93)*
![](http://bbs.icafe8.com/static/image/filetype/rar.gif)[WinDbg.part01.rar](http://bbs.icafe8.com/forum.php?mod=attachment&aid=NTk5Nzl8OTQzNmM4NDl8MTM0NDU1NTQ5NHwwfDQwMDEwNA%3D%3D)*(1.39 MB, 下载次数: 82)*
2：第一次打开界面操作如下图：
![1.jpg](http://bbs.icafe8.com/data/attachment/forum/201208/03/221722a26r05g6c0o0az2x.jpg)
3：然后会出现如下图的，另外把SRV*e:\symbols*  [http://msdl.microsoft.com/download/symbols](http://msdl.microsoft.com/download/symbols)
复制进去。
[http://msdl.microsoft.com/download/symbols](http://msdl.microsoft.com/download/symbols)为微软符号表服务器地址
![2.jpg](http://bbs.icafe8.com/data/attachment/forum/201208/03/222416o7wgjmrj1n72j1m7.jpg)
然后点OK，然后关闭windbg一次。不然这个设置不会被保存下来。
然后下次需要看DUP的时候，打开windbg.exe，直接可以把抓来的DMP文件拖到打开的界面里面就可以分析了。
4：自动分析命令 !analyze -v
以网吧抓到的一个F4蓝屏为例
![3.jpg](http://bbs.icafe8.com/data/attachment/forum/201208/03/224111i52ir54ko5ehbi1k.jpg)
上图比较迷惑的地方
从这个栈回溯看，会以为是vdiskbus+0xda6c引起的蓝屏。
实际上这里是[无盘](http://bbs.icafe8.com/thread-378606-1-1.html)实现的抓DUMP的机制，这里可以看作是对系统函数KebugCheckEx的展开。这个是这样理解，KebugCheckEx调用地址
 0x89fb41ca,地址0x89fb41ca会调用vdiskbus+0xda6c,也就是无盘的DMP机制。所以看到这样的栈回溯，说明并不是vdiskbus引起蓝屏，只是调用到了无盘的DMP机制。
从另一个角度来说，调用KebugCheckEx就是蓝屏，KebugCheckEx调用的不是引起蓝屏的。所以要向前推，看谁在调用KebugCheckEx。
这个实例中看到杀进程的函数ZwTerminateProcess原型为：
ZwTerminateProcess(IN HANDLE ProcessHandle OPTIONAL,IN NTSTATUS ExitStatus)；
看堆栈中第一个参数:
![5.jpg](http://bbs.icafe8.com/data/attachment/forum/201208/03/2248168b8714meeeq8bn4q.jpg)
为0xffffffff即-1，表示杀掉了自己。然后用命令!process 看下当前进程是谁。
![6.jpg](http://bbs.icafe8.com/data/attachment/forum/201208/03/225009c0uyr4j3mo0oys43.jpg)
这样从栈信息，看到是csrss.exe进程出现了异常，会导致进程自 杀。而引起F4蓝屏。具体是哪里引起的，因为小DMP没有用户态信息。得不到进一步的问题确认（有可能有人注入csrss引起等原因）。
其实自动分析已经把结果列出来了，实际上不需要我们做上面的分析。
![7.jpg](http://bbs.icafe8.com/data/attachment/forum/201208/03/2251335sw0gw6a3gi63vn4.jpg)
F4蓝屏的第二个参数，就是被杀掉的进程的进程对象。自动分析已经指出为csrss进程。做上述人工分析，是想说明人工怎么定位问题。
5：常用命令：
查看栈的命令：k,kb,kn,kd,kl
查看内存的命令：db,dw,dd,da,du
用一个网吧抓到的DMP为EA蓝屏的做为例子。
![8.jpg](http://bbs.icafe8.com/data/attachment/forum/201208/03/2254563rfezfrodovk4kda.jpg)
自动分析的栈来看，是看门狗驱动调用的蓝屏函数kebugcheckex。这个栈来看没有什么用。从自动分析对EA蓝屏的解释来看是设备驱动变为闲置状态，一定时间内，没有喂狗信号。看门狗会调用dbgBreakPoint，而不是kebugCheckex。不过我们还是在kebugCheckex拿到了一些蓝屏的信息。
EA蓝屏的第一个参数指向一个线程对象，用命令.thread 切换到对应线程，然后用kb查看线程栈，就可以确定哪里出了问题。
![9.jpg](http://bbs.icafe8.com/data/attachment/forum/201208/03/225653z0foj860zjo6nthc.jpg)
执行提示出错，用dd命令查看一下对应的地址。
![10.jpg](http://bbs.icafe8.com/data/attachment/forum/201208/03/230228c1zjjhndcjqlqhj6.jpg)
对应地址为？？？？？？，很遗憾，表示这个地址的内容，并没有被我们DMP下来。
同样第二个参数中的地址，也没有包含在我们的小DMP中。
![11.jpg](http://bbs.icafe8.com/data/attachment/forum/201208/03/230353fc3itz92fc4iel43.jpg)
第三个参数的解释，是指向出错驱动的名称。用dd命令来查看一下对应地址的内容是否被我们DMP出来了。
![12.jpg](http://bbs.icafe8.com/data/attachment/forum/201208/03/2303530iyop33q3ql5i91l.jpg)
能看到数据内容。看样子应该是UNICODE_STRING结构，显示UNICODE_STRING的命令是dS（S必须大写）。
![13.jpg](http://bbs.icafe8.com/data/attachment/forum/201208/03/230353kiks53y88g4dhqv5.jpg)
显示驱动名称为：“nv4_disp”.看来应该是N卡的显示驱动。
用lm 可以显示驱动模块。
![14.jpg](http://bbs.icafe8.com/data/attachment/forum/201208/03/230353hfw43fohxtxowm16.jpg)
6：需要切换环境手动生成了一个DMP，7E蓝屏。
![15.jpg](http://bbs.icafe8.com/data/attachment/forum/201208/03/230353ar2enstwnywas925.jpg)
这时用KB看栈
![16.jpg](http://bbs.icafe8.com/data/attachment/forum/201208/03/230353xcexbxcye8tpcztj.jpg)
从上图看明显不是出错的线程栈。是因为需要切换一下环境。蓝屏的第四个参数存储的是对应的环境地址。用命令.cxr切换一下，然后再用kb查看。
![17.jpg](http://bbs.icafe8.com/data/attachment/forum/201208/03/230353y44ph9eihkrmylmo.jpg)
切换环境后，再看栈回溯就能定位到出错的地方了。

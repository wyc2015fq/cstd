# WINCE6.0+S3C2443的RTC初始化及存在的问题 - xqhrs232的专栏 - CSDN博客
2014年05月26日 15:41:02[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：698
原文地址::[http://cky0612.blog.163.com/blog/static/27478916201072111512454/](http://cky0612.blog.163.com/blog/static/27478916201072111512454/)
1.系统启动过程中RTC初始化
在WINCE6.0中，我们知道是通过OALIoCtlHalInitRTC()函数来设置RTC的时间的，但是如何调用到这个函数的呢？我们就从NKStartup()函数开始，但系统从nboot开始是如何执行到NKStartup()函数的呢？我后面会抽空写关于eboot和nk的启动过程中有涉及。
![WINCE6.0+S3C2443的RTC初始化及存在的问题 - 男儿当自强 - 男儿当自强的博客](http://hi.csdn.net/attachment/201008/21/0_1282365503knt5.gif)
在SystemStartupFunc函数中，通过下面语句来创建一个内核线程RunApps：
hTh = CreateKernelThread (RunApps,0,THREAD_RT_PRIORITY_NORMAL,0);
接着就执行线程RunApps。在线程RunApps中，通过下面语句：
 hFilesys = (HMODULE) NKLoadLibraryEx (L"filesys.dll", MAKELONG (LOAD_LIBRARY_IN_KERNEL, LLIB_NO_PAGING), NULL);
来加载filesys.dll，接着通过下面语句：
hTh = CreateKernelThread ((LPTHREAD_START_ROUTINE)pfnMain, hFilesys, THREAD_RT_PRIORITY_NORMAL, 0);
来创建filesys.dll的线程WinMain()，这个线程在\WINCE600\PRIVATE\TEST\BASEOS\FILESYS\GENFILE\genfile.cpp中定义，这样就接着执行这个线程。
Filesys.dll的主要工作是初始化文件系统、对象存储、注册表、CEDB数据库、设备通知以及其它一些工作：
⑴Filesys.dll检测是冷启动还是热启动
如果是冷启动，对象储存内存将被初始化并映射给Filesys.dll；对于热启动，不用初始化而直接映射给Filesys.dll。
⑵Filesys.dll从nandflash中加载OEM的certification DLL。
⑶如果必须他能够必须是冷启动，Filesys.dll调用OAL函数pNotifyForceCleanboot。
⑷对于冷启动，Filesys.dll调用OEMIoControl函数，I/O控制代码为IOCTL_HAL_INIT_RTC，也就是初始化RT。
Filesys.dll还有很多的动作，再这里就不介绍了，我们接下来看看OEMIoControl函数是如何一层层调用到OALIoCtlHalInitRTC的呢？接着往下看。
OEMIoControl()：在\WINCE600\PLATFORM\COMMON\SRC\COMMON\IOCTL\ioctl.c下面定义
![WINCE6.0+S3C2443的RTC初始化及存在的问题 - 男儿当自强 - 男儿当自强的博客](http://hi.csdn.net/attachment/201008/21/0_12823655413y9T.gif)
从上图可以知道OEMIoControl函数通过传递进来的code来和数组g_oalIoCtlTable中的code成员比较，找到吻合的code后便跳出这个for循环，然后往下执行。而数组g_oalIoCtlTable是在\Src\Oal\Oallib\ioctl.c下定义，如下图
![WINCE6.0+S3C2443的RTC初始化及存在的问题 - 男儿当自强 - 男儿当自强的博客](http://hi.csdn.net/attachment/201008/21/0_128236558135aA.gif)
接着看ioctl_tab.h的内容
![WINCE6.0+S3C2443的RTC初始化及存在的问题 - 男儿当自强 - 男儿当自强的博客](http://hi.csdn.net/attachment/201008/21/0_12823656113t6p.gif)
到这里我们还是没有直观看到IOCTL_HAL_INIT_RTC，接着往下看oal_ioctl_tab.h的内容：
![WINCE6.0+S3C2443的RTC初始化及存在的问题 - 男儿当自强 - 男儿当自强的博客](http://hi.csdn.net/attachment/201008/21/0_1282365629vK3K.gif)
在上图的第31行可以看到IOCTL_HAL_INIT_RTC，那么对应于IOCTL_HAL_INIT_RTC的处理函数是如何得到执行的呢？上面提到，OEMIoControl函数通过传递进来的code找到数组g_oalIoCtlTable中吻合的code后，通过下面的处理
![WINCE6.0+S3C2443的RTC初始化及存在的问题 - 男儿当自强 - 男儿当自强的博客](http://hi.csdn.net/attachment/201008/21/0_12823655413y9T.gif)
会记下i的值，也就是IOCTL_HAL_INIT_RTC所在数组g_oalIoCtlTable的索引值，接着看OEMIoControl函数下面的处理
![WINCE6.0+S3C2443的RTC初始化及存在的问题 - 男儿当自强 - 男儿当自强的博客](http://hi.csdn.net/attachment/201008/21/0_12823675386see.gif)
这段代码就是根据上面确定的i的值，来执行对应的handler，在这里就是调用OALIoCtlHalInitRTC()函数
2.RTC初始化存在的问题
从上面可知，系统启动过程是通过调用函数OALIoCtlHalInitRTC()来初始化RTC的，这个函数体如下
![WINCE6.0+S3C2443的RTC初始化及存在的问题 - 男儿当自强 - 男儿当自强的博客](http://hi.csdn.net/attachment/201008/21/0_12823675855a42.gif)
从这个函数体可知是通过调用OEMSetRealTime()来设置RTC的，g_oalRtcResetTime的定义如下：
SYSTEMTIME g_oalRtcResetTime = {2010, 8, 5, 27, 12, 0, 0, 0};
因为我们的系统每次关机后重启都是冷启动，所以每次启动之后都会调用到OALIoCtlHalInitRTC函数，从而不管你之前设置的时间是多少，都会重新初始化为g_oalRtcResetTime中的时间。我把上图的第98行替换为下面的函数就可以解决了这个问题：
![WINCE6.0+S3C2443的RTC初始化及存在的问题 - 男儿当自强 - 男儿当自强的博客](http://hi.csdn.net/attachment/201008/21/0_1282367617sSQ7.gif)
3.BOOL OEMSetRealTime(LPSYSTEMTIME pTime) 
OEMGetRealTime()用来获得当前的时间。WinCE启动以后，默认情况下，WinCE会每隔一段时间调用OEMGetRealTime()函数来获得系统的时间，这种方式被称为hardware mode。WinCE还有另一种获得系统时间的方法，被称为software mode，就是通过调用GetTickCount()函数跟踪系统的timetick的变化来累加时间。如果要用software mode，那么需要在注册表中做如下的设置：
HKEY_LOCAL_MACHINE\Platform\"SoftRTC" = 1
我来谈谈我的看法，一般都要使用hardware mode，这样获得的系统时间比较准。software mode获得系统时间不会很准的。
4.BOOL OEMGetRealTime(SYSTEMTIME *pTime)
OEMGetRealTime()用来获得当前的时间。WinCE启动以后，默认情况下，WinCE会每隔一段时间调用OEMGetRealTime()函数来获得系统的时间，这种方式被称为hardware mode。WinCE还有另一种获得系统时间的方法，被称为software mode，就是通过调用GetTickCount()函数跟踪系统的timetick的变化来累加时间。如果要用software mode，那么需要在注册表中做如下的设置：
HKEY_LOCAL_MACHINE\Platform\"SoftRTC" = 1
我来谈谈我的看法，一般都要使用hardware mode，这样获得的系统时间比较准。software mode获得系统时间不会很准的。
5.BOOL OEMSetRealTime(LPSYSTEMTIME pTime) 
OEMSetRealTime()用来设置当前的时间。当WinCE启动以后，我们会在界面的右下角看到时间显示，我们可以直接在WinCE的界面里面设置时间，这个时候，系统就会调用OEMSetRealTime()把你设置的时间写到RTC模块里面。
6.LPSYSTEMTIME
LPSYSTEMTIME实际上是一个指向SYSTEMTIME结构的指针，关于SYSTEMTIME，定义如下：
typedef struct _SYSTEMTIME
    {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
    } SYSTEMTIME;

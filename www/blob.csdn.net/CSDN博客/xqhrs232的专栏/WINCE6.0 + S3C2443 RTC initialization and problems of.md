# WINCE6.0 + S3C2443 RTC initialization and problems of - xqhrs232的专栏 - CSDN博客
2014年05月26日 15:27:01[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：804
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://www.databaseskill.com/2227393/](http://www.databaseskill.com/2227393/)
相关文章
1、Troubled by OALIoCtlHalInitRTC----[http://www.tech-archive.net/Archive/WindowsCE/microsoft.public.windowsce.platbuilder/2007-01/msg00854.html](http://www.tech-archive.net/Archive/WindowsCE/microsoft.public.windowsce.platbuilder/2007-01/msg00854.html)
2、WINCE6.0+S3C2443的RTC初始化及存在的问题 
 ----[http://cky0612.blog.163.com/blog/static/27478916201072111512454/](http://cky0612.blog.163.com/blog/static/27478916201072111512454/)
3、WinCE中RTC驱动开发介绍----[http://www.xuebuyuan.com/1179995.html](http://www.xuebuyuan.com/1179995.html)
RTC initialization. During system startup
In the WINCE6.0 in, we know through OALIoCtlHalInitRTC () function to set the RTC time, but how to call to this function it?From NKStartup () function, but the system from nboot
 how to perform NKStartup () function?Behind me for taking the time to write about the the eboot and nk startup process involved.
In the SystemStartupFunc function, the following statement to create a kernel thread RunApps:
hTh = CreateKernelThread (RunApps, 0, THREAD_RT_PRIORITY_NORMAL, 0);
Then on the implementation of thread RunApps.In the thread RunApps the following statement:
hFilesys = (HMODULE) NKLoadLibraryEx (L "filesys.dll", MAKELONG (LOAD_LIBRARY_IN_KERNEL, LLIB_NO_PAGING), NULL);
To to load filesys.dll, followed by the following statement:
hTh = CreateKernelThread ((LPTHREAD_START_ROUTINE) pfnMain, hFilesys, THREAD_RT_PRIORITY_NORMAL, 0);
To create filesys.dll thread WinMain (), the thread / WINCE600/PRIVATE/TEST/BASEOS/FILESYS/GENFILE/genfile.cpp defined, so that then executes this thread.
The main work of Filesys.dll initialization file system, object storage, registry, CEDB database, device notifications, as well as some other work:
The ? Filesys.dll detection is cold start or hot start
If cold start to The object stored memory will be initialized and mapped to the Filesys.dll; for hot start, do not initialize directly mapped to Filesys.dll of.
(2) Filesys.dll from the nandflash loaded the OEM Certification DLLs.
? must he must be a cold start, Filesys.dll call OAL the function pNotifyForceCleanboot.
(4) For the cold start the Filesys.dll call OEMIoControl of function, I / O control code for IOCTL_HAL_INIT_RTC, which is initialized RT.
Filesys.dll there are a lot of action here is not introduced, we next look at how OEMIoControl function call layers to OALIoCtlHalInitRTC it?Then read on.
OEMIoControl (): / WINCE600/PLATFORM/COMMON/SRC/COMMON/IOCTL/ioctl.c the following definition of
From the graph you can know OEMIoControl function by passing the incoming code to array g_oalIoCtlTable of the code member to compare, find the consistent code after jumping out of the for loop, and then down the implementation.The
 array g_oalIoCtlTable / Src / Oal / Oallib / ioctl.c define, as shown
Then see ioctl_tab.h content
Here we still did not visually see IOCTL_HAL_INIT_RTC, then look down the contents of the oal_ioctl_tab.h:
Line on Figure 31 the can see IOCTL_HAL_INIT_RTC, then the corresponding processing function is how to get it performed IOCTL_HAL_INIT_RTC?The mentioned above, OEMIoControl
 function is passed through the code to find the array g_oalIoCtlTable anastomosis code, by the following process
Will write down the value of i is IOCTL_HAL_INIT_RTC the array g_oalIoCtlTable where the index value, then look OEMIoControl function following treatment
This code is based on the above-determined value of i to perform the corresponding handler in the here is calling OALIoCtlHalInitRTC to () function
2.RTC initialization Problems
We can see from the above, the system startup process by calling function OALIoCtlHalInitRTC () to initialize the RTC, this function is as follows
We can see from this function to set the RTC by calling OEMSetRealTime (), g_oalRtcResetTime defined as follows:
SYSTEMTIME g_oalRtcResetTime = {2010, 8, 5, 27, 12, 0, 0, 0};
Because our system every shutdown and reboot are cold start, it will be called after each start to OALIoCtlHalInitRTC function, so no matter what you set before time is re-initialized to time g_oalRtcResetTime.I
 put on the line 98 replaced by the following function can solve this problem:
3.BOOL OEMSetRealTime (LPSYSTEMTIME pTime)
OEMGetRealTime () is used to get the current time.WinCE startup default the WinCE will the regular intervals the call OEMGetRealTime () function to get the system time, this
 approach is known as the Hardware Mode.WinCE Another way to get the system time, system referred to as a software mode, calling GetTickCount () function tracking system timetick of changes to the accumulation time.If
 the system to use a software mode, you will need to do the following settings in the registry:
HKEY_LOCAL_MACHINE / Platform / "SoftRTC" = 1
I'll talk about my views are generally required to use the Hardware Mode, so that more accurate system time.Software mode to get the system time will not be accurate.
4.BOOL OEMGetRealTime (SYSTEMTIME * pTime)
OEMGetRealTime () is used to get the current time.WinCE startup default the WinCE will the regular intervals the call OEMGetRealTime () function to get the system time, this
 approach is known as the Hardware Mode.WinCE Another way to get the system time, system referred to as a software mode, calling GetTickCount () function tracking system timetick of changes to the accumulation time.If
 the system to use a software mode, you will need to do the following settings in the registry:
HKEY_LOCAL_MACHINE / Platform / "SoftRTC" = 1
I'll talk about my views are generally required to use the Hardware Mode, so that more accurate system time.Software mode to get the system time will not be accurate.
5.BOOL OEMSetRealTime (LPSYSTEMTIME pTime)
OEMSetRealTime () is used to set the current time.WinCE startup, we will see the time in the lower right corner of the interface, we can directly set inside the WinCE interface,
 this time, the system will to call OEMSetRealTime () inside of the time you set is written to the RTC module.
6.LPSYSTEMTIME
The LPSYSTEMTIME is actually a pointing the SYSTEMTIME structure of the pointer, SYSTEMTIME, defined as follows:
typedef struct _SYSTEMTIME
{
The WORD wYear;
The WORD wMonth;
The WORD wDayOfWeek;
WORD wDay;
The WORD wHour;
The WORD wMinute;
The WORD wSecond;
The WORD wMilliseconds;
} SYSTEMTIME;

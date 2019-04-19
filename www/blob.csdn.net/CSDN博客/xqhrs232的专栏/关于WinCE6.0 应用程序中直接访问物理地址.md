# 关于WinCE6.0 应用程序中直接访问物理地址 - xqhrs232的专栏 - CSDN博客
2014年03月26日 17:47:01[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：784
原文地址::[http://blog.163.com/qiu_zhi2008/blog/static/601409772010240238767/](http://blog.163.com/qiu_zhi2008/blog/static/601409772010240238767/)
相关文章
1、wince下访问物理地址0x10000000，如何实现----[http://social.msdn.microsoft.com/Forums/windowshardware/zh-CN/0b1a5620-2e8b-4e58-a648-b6c82d3fc221/wince0x10000000](http://social.msdn.microsoft.com/Forums/windowshardware/zh-CN/0b1a5620-2e8b-4e58-a648-b6c82d3fc221/wince0x10000000)
2、[WINCE
 对物理地址的访问](http://blog.csdn.net/pipi87/article/details/5840510)----[http://blog.csdn.net/pipi87/article/details/5840510](http://blog.csdn.net/pipi87/article/details/5840510)
3、[WinCE5.0
 内存管理总结](http://www.cnblogs.com/songtitan/archive/2005/07/20/196675.html)----[http://www.cnblogs.com/songtitan/archive/2005/07/20/196675.html](http://www.cnblogs.com/songtitan/archive/2005/07/20/196675.html)
4、关于访问物理地址---[-http://bbs.csdn.net/topics/340097139]()
转载一高手的帖子，有待偶去验证下，如果可用，用处大大的啊
在实际开发过程中，经常希望能在应用程序中直接读写设备的物理空间。以前在做WinCE6.0下的MEMMgr时通过秘密加载一个内核态驱动实现了这个需 求。但这种方式有一个明显的缺陷，每次读写都必须经由它才能完成。如果只是读取GPIO，那问题不算大。如果想通过这种方式实现视频播放的加速就比较困难
 了。估计非但不能加速，反而会变得更慢。
      早先曾与ZL仔细的讨论过这个问题，他当时在WinCE6.0上移植TCPMP，发现播放视频不太流畅，于是想通过直接写显存进行加速。目的很明确，在应 用中申请一段虚拟空间，通过某种方法将其映射到显存上，视频解码过程中直接往映射过的虚拟空间上写。这种方法与使用GAPI有一点类似。
      实现这个需求，需要用到函数VirtualCopyEx()。看看帮助中关于它的说明，This function dynamically maps a virtual address to a physical address by creating a new page-table entry.This function is callable in kernel mode and in user mode, when the source and destination process handles
 are the active process.This function is similar to **VirtualCopy**, except **VirtualCopyEx**requires handles to the source and destination process.
      据此基本可以确定，我们的确可以在应用中申请一段虚拟空间，然后通过这个函数将其映射到某段物理空间上。其中目标进程是我们的应用，而源进程是 NK.exe。为了实现在NK.exe中执行VirtualCopyEx()，可以加载一个内核态的驱动。更为方便的方法是移植一个OALIOCTL，并 在IOControl()中添加一个case。这样，应用程序在做内存映射时就无需打开某个流驱动，直接调用KernelIoControl()即可。
      OALIOCTL中添加的关键代码如下。 
 1typedef struct {
 2void*    pvDestMem;
 3    DWORD    dwPhysAddr;
 4    DWORD    dwSize;
 5} VIRTUAL_COPY_EX_DATA;
 6
 7#define IOCTL_VIRTUAL_COPY_EX CTL_CODE (FILE_DEVICE_UNKNOWN,3333,METHOD_BUFFERED,FILE_ANY_ACCESS)
 8
 9
10case IOCTL_VIRTUAL_COPY_EX:
11{
12    VIRTUAL_COPY_EX_DATA *p = (VIRTUAL_COPY_EX_DATA*)pInBuf;
13    HANDLE hDst = (HANDLE)GetDirectCallerProcessId();
14    HANDLE hSrc = (HANDLE)GetCurrentProcessId();
15    fRet = VirtualCopyEx(hDst,p->pvDestMem,hSrc,(LPVOID)p->dwPhysAddr,p->dwSize,
16 　　　　 PAGE_READWRITE|PAGE_PHYSICAL|PAGE_NOCACHE);
17}break;
       应用程序中进行内存映射的关键代码如下。
 1volatile LPVOID GetVirtual(DWORD dwPhyBaseAddress, DWORD dwSize)
 2{
 3volatile LPVOID pVirtual;
 4    VIRTUAL_COPY_EX_DATA vced;
 5
 6if(dwPhyBaseAddress&0xFFF)
 7    {
 8return NULL;
 9    }
10    vced.dwPhysAddr = dwPhyBaseAddress>>8;
11    pVirtual = VirtualAlloc(0,dwSize,MEM_RESERVE,PAGE_NOACCESS);
12    vced.pvDestMem = pVirtual;
13    vced.dwSize = dwSize;
14    KernelIoControl(IOCTL_VIRTUAL_COPY_EX,&vced, sizeof(vced), NULL, NULL, NULL);
15return pVirtual;
16}
17
18// WinCE6.0模拟器中应用程序直接写屏
19PBYTE pLCDBuf = (PBYTE)GetVirtual(0x33f00000,0x100000);
20memset(pLCDBuf,0,0x100000);
21
       这种方法在WinCE6.0的模拟器中测试了一下，能达到预期的效果。
really？ i will try


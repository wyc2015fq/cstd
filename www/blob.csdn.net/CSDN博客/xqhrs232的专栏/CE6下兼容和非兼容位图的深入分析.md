# CE6下兼容和非兼容位图的深入分析 - xqhrs232的专栏 - CSDN博客
2013年03月15日 17:37:02[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：819
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://blog.csdn.net/alien75/article/details/7233019](http://blog.csdn.net/alien75/article/details/7233019)
## [CreateDIBSection、BITMAPINFO、BITMAPINFOHEADER、RGBQUAD - [Daliy APIs]](http://www.cnblogs.com/Lisen/archive/2010/01/21/1653608.html)----[http://www.cnblogs.com/Lisen/archive/2010/01/21/1653608.html](http://www.cnblogs.com/Lisen/archive/2010/01/21/1653608.html)
兼容位图即DDB，由CreateCompatibleBitmap创建；非兼容位图即DIB，是由LoadImage、SHLoadBitmap、CreateDIBSection、CreateBitmap创建。
非兼容位图和兼容位图有两个本质差别
第一个区别是兼容位图的存放空间通常由GWES或显示驱动分配，处于驱动层之中，可以获取到存放数据的物理地址，而非兼容位图的存放空间通常由应用程序分配，处于应用层中，不能获取到物理地址。
第二个区别是兼容位图与显示驱动的位深和像素格式保持一致（WinCE显示驱动的位深和格式是固定的），如显示驱动采用的是16位RGB565的位深和像素格式，则兼容位图的格式也一样，而非兼容位图通常不相同，即它内部可能是32位位深ARGB的格式，也有可能是24位RGB888的格式。
以上两个区别意味着
第一个区别意味着非兼容位图无法享受2D显示模块的硬件加速，硬件加速的原理类似DMA，需要物理地址，而非兼容位图获取不到物理地址，自然无法进行硬件加速。
第二个区别意味着非兼容位图拷贝到显示界面上往往需要进行颜色转换，因为它要符合显示驱动的颜色格式才能正常地显示出来，常用的位图格式是24位RGB888，而显示驱动常用的格式是16位RGB565、32位ARGB8888，自然免不了转换，这部分工作都是在显示驱动中进行的，如果仅有一两次转换或少量像素的转换可能不会有什么，但如果是大量的绘图操作，意味着每次拷贝都会进行转换，这将把宝贵的系统资源消耗在位图的转换上。
补充说明：
1、创建兼容位图的时候，GDI会调用GPE的AllocSurface(纯虚)方法创建GPESurf(主表面也就是LCD对应的显存是第一时间创立的，剩下的也就用于GDI对象了)，GPESurf构造函数是用new来分配内存的，也就是说从0xD0000000~0xDFFFFFFF区域分配的；如果系统定制的时候预留内存(也就是视频内存)，并重载GPESurf优先从视频内存上分配则可以用于DMA实现加速，换句话说如果不重载GPESurf，为显示预留内存是没有意义的。
2、创建非兼容位图的时候，GDI未调用GPE的AllocSurface方法
3、GDI的SurfaceHeap类可用于对预留的视频内存进行管理
下面是一组测试可以证明两者的差别，测试方法是循环创建800*480的16位HBITMAP直到返回失败
CreateCompatibleBitmap
NK.exe的heap一直增加，直到耗尽物理内存。堆分配地址范围0xD0000000~0xDFFFFFFF(内核空间地址，由所有被内核加载的服务和驱动共享)
CE6模拟器执行50次，物理内存由39063552降到503808，NK.exe的heap为40406591，应用的heap为2016
CE6设备执行279次，物理内存由83456000降到815104，NK.exe的heap为83519444，应用的heap为19616
补充：CE5模拟器执行35次，物理内存由43204608降到16211968，这估计是因为CE5进程只有32MB空间
CreateDIBSection
应用的heap一直增加，直到耗尽物理内存。堆分配地址范围0x00010000~0x3FFFFFFF(进程及用户分配的虚拟地址)
CE6模拟器执行50次，物理内存由39059456降到442368，NK.exe的heap为2022079，应用的heap为38403616
CE6设备执行107次，物理内存由83439616降到872448，NK.exe的heap为1207028，应用的heap为82199040
测试代码如下
**[cpp]**[view
 plain](http://blog.csdn.net/alien75/article/details/7233019#)[copy](http://blog.csdn.net/alien75/article/details/7233019#)
- void* pucAddr;  
- struct
- {  
-     BITMAPINFOHEADER    bmiHeader;  
- int                 BitMask[3];  
- } bmi;  
- bmi.bmiHeader.biWidth         = 800 ;  
- bmi.bmiHeader.biHeight        = -480 ;  
- bmi.bmiHeader.biPlanes        = 1 ;  
- bmi.bmiHeader.biSizeImage     = 0 ;  
- bmi.bmiHeader.biXPelsPerMeter = 0 ;  
- bmi.bmiHeader.biYPelsPerMeter = 0 ;  
- bmi.bmiHeader.biClrUsed       = 0 ;  
- bmi.bmiHeader.biClrImportant  = 0 ;  
- bmi.bmiHeader.biBitCount      = 16 ;  
- bmi.bmiHeader.biSize = sizeof(bmi);  
- bmi.bmiHeader.biCompression = BI_BITFIELDS;  
- bmi.BitMask[0] = 0xF800;  
- bmi.BitMask[1] = 0x07E0;  
- bmi.BitMask[2] = 0x001F;  
- HBITMAP hBitmap;  
- for(int i = 0; ; i++)  
- {  
- #if 1
-     hBitmap = ::CreateCompatibleBitmap(::GetDC(NULL), 800, 480);  
- #else
-     hBitmap = ::CreateDIBSection( NULL  
-         , (BITMAPINFO*)&bmi  
-         , DIB_RGB_COLORS  
-         , (void**)&pucAddr  
-         , NULL  
-         , 0  
-         ) ;  
- #endif
- DWORD dwRet = ::GetLastError();  
- if(dwRet == 8) {  
-         DebugBreak();  
-     }  
- }  

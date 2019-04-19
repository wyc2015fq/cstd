# wince6.0 s5pv210 iic驱动 报错 - xqhrs232的专栏 - CSDN博客
2014年12月08日 10:05:40[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：862
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://blog.csdn.net/zmq5411/article/details/20729995#comments](http://blog.csdn.net/zmq5411/article/details/20729995#comments)
相关文章
1、wince6.0 s5pv210 iic驱动 报错----[http://bbs.csdn.net/topics/390481291?page=1#post-398640529](http://bbs.csdn.net/topics/390481291?page=1#post-398640529)
2、6410操作24C02驱动bug批改及测试----[http://www.educity.cn/wenda/98050.html](http://www.educity.cn/wenda/98050.html)
3、Re: CeAllocAsynchronousBuffer functionality and VirtualAllocCopyEx----[http://www.tech-archive.net/Archive/WindowsCE/microsoft.public.windowsce.platbuilder/2008-07/msg00305.html](http://www.tech-archive.net/Archive/WindowsCE/microsoft.public.windowsce.platbuilder/2008-07/msg00305.html)
在读iic时经常会报如下错误
Exception 'Data Abort' (4): Thread-Id=0909001a(pth=8c61a834), Proc-Id=00400002(pprc=824f9308) 'NK.EXE', VM-active=0567014e(pprc=8ccaaedc) 'edm.exe'
PC=c0bc75a0(i2c.dll+0x000075a0) RA=c0bc7370(i2c.dll+0x00007370) SP=d4fefe48, BVA=0002fc77
I2CBusDriver: Exception caught ExceptionCode:0xC0000005, flags:0x00000000, Code Address 0xC0BC75A0 
        Exception: EXCEPTION_ACCESS_VIOLATION 
        Write Access Exceptioned at VAddress : 0x0002FC77 
[31mEXCEPTION IS INVOKED(HW 2 Channel)
[0m[31mFifo Index : 4
[0m[31mbRequestComplete : 0
[0m[31mCurrnet Tx Count : 0
[0m[31mCurrnet Rx Count : 3
[0m[31mCurrent Service Obj : 0xD53B1C40
[0m[31mTx Data Addr & size : 0x0 0
[0m[31mRx Data Addr & size : 0x2FC74 14
[0m
[I2C:I] IICCON : 0xFB 0xEB
[I2C:I] IICSTAT : 0xB0 0xB0
[I2C:I] IICLC : 0x4 0x0
[I2C:I] IICDS : 0x68 0xA1
求大虾指点迷津
读的函数如下：
BYTE read_buffer[14]={0};
BYTE offset=0;
I2C_READ_DESC i2creaddesc;
//step1:set offset
i2creaddesc.IN_bStop=1; 
i2creaddesc.IN_dwData=1;
i2creaddesc.IO_pbtData=&offset;
DeviceIoControl(hfile,IOCTL_I2C_GENERAL_WRITE,&i2creaddesc,sizeof(i2creaddesc),NULL,0,NULL,0);
//step2:read data
i2creaddesc.IN_bStop=1;
i2creaddesc.IN_dwData=14;
i2creaddesc.IO_pbtData=read_buffer;
DeviceIoControl(hfile,IOCTL_I2C_GENERAL_READ,&i2creaddesc,sizeof(i2creaddesc),NULL,0,NULL,0);
故障有原因是：内嵌指针和数据传递的问题。在I2C_READ_DESC结构中有个i2creaddesc.IO_pbtData 是指针指向内存(内嵌指针)
解决见CE6 驱动: 你不得不知道的事情内嵌指针和数据传递 具体操作我还没试 等后续更新。
 因为设备管理器负责加载驱动程序DLL，这意味着当应用程序调用驱动程序接口函数的时候，WINCE内核会将调用驱动程序接口函数的线程转移到设备管理器的进程空间然后执行具体的驱动程序代码，应用程序和设备管理器处于两个进程空间，这就造成设备管理器无法访问应用程序传递的指针（虚拟地址），所以当我们在应用程序中传递指针给流驱动程序接口函数时，WINCE内核从中作了一个地址映射，例如ReadFile、WriteFile、DeviceIoControl函数的参数凡是指针都经过了映射才传递给驱动程序，所以很多驱动程序开发者并不了解其中的奥秘就可以编程了。但是如果参数是一个指向一个结构体的指针，而结构体里包括一个或多个指针，那么WINCE内核并不负责映射，所以就需要开发者在驱动程序接口函数中调用API函数MapPtrToProcess来映射地址。例如：pPointer_retval = MapPtrToProcess(pPointer, GetCallerProcess());  
经朋友指点有间接的解决办法是在写个驱动在驱动中调用I2C驱动 而应用程序不直接调用I2C 不使用内嵌指针。
//==============================================================
备注::
1》试了间接写个流接口驱动，在流接口驱动里面去调I2C的内嵌指针，似乎是可行的，就是不知道测试出稳定性会是怎么样的一个情况！！！


# wince5.0+IO操作基础----直接操作物理内存 - xqhrs232的专栏 - CSDN博客
2014年03月26日 15:02:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1270
原文地址::[http://wenku.baidu.com/link?url=rIUuCdchI2UfeJlllSzy5XZTspak93Os_Q_vRFlZYb3KvYZRGXiKljNRjz1EbI6kHjzw7ObBFpaDOt8Fe0Ed2S2IRaqhtVGDQ8OB1Je1QSG](http://wenku.baidu.com/link?url=rIUuCdchI2UfeJlllSzy5XZTspak93Os_Q_vRFlZYb3KvYZRGXiKljNRjz1EbI6kHjzw7ObBFpaDOt8Fe0Ed2S2IRaqhtVGDQ8OB1Je1QSG)
相关文章
1、**WinCE5.0设备驱动开发----[http://www.docin.com/p-105699913.html](http://www.docin.com/p-105699913.html)**
-------重点看一下第4章的设备驱动的内存访问及DMA工作原理

**[wince5.0 I/O操作基础(2)](http://blog.csdn.net/milkyway1888/archive/2007/03/23/1538654.aspx)**
对外设进行 I/O 操作实际上也就是读写外设的寄存器,而我们通常使用的X86或者ARM处理器在硬件上决定了wince系统启动MMU后,OS无法直接访问物理地址,因此需要做一些工作来实现I/O操作.
首先要理解 windows CE 下的地址映射机制。 wince有两种地址：物理地址和虚拟地址．不同架构的 CPU 硬件上的区别导致地址映射也不同。ＭＩＰＳ和ＳＨ x 处理器，不采用ＭＭＵ，直接在ＣＰＵ和内核里定义 1G 的物理地址；而Ｘ８６和ＡＲＭ带有 MMU 单元，在 OEMAddressTable 中定义物理地址到虚拟地址间的映射关系或者是ＯＳ启动后调用 CreateStaticMapping和 NKCreateStaticMapping 来实现从虚拟地址到物理地址的静态映射．经过静态映射的地址，可以由操作系统内核用于
 ISR 访问设备。如果我们要在应用程序中访问外设，必须在物理地址和虚拟地址间建立动态映射关系，我们可以使用 VirtualAlloc 和 VirtualCopy （或者直接调用 MmmapIoSpace 函数）来实现。
其次，如果是操作通过总线挂接的 I/O 或者存储器，必须先把总线地址转化成 CPU 上的系统地址，再做物理地址到虚拟地址的映射。这里需要查 CPU 的 Datasheet ，找出所要操作的I/O地址.先调用HALTranslateBusAddress( )把总线地址转化成CPU上的系统地址, 再调用 MmmapIoSpace 函数实现虚实映射；也可以使用 TransBusAddrToVirtual （）直接把总线上的地址转化成系统的虚拟地址。
第三，在一般的应用程序中访问 I/O 是访问它的缓存段虚拟地址，而驱动中必须访问无缓存段虚拟地址。简单来说无缓存段虚拟地址 = 缓存段虚拟地址 +0x20000000 。
   总结起来，如果是 wince 内核(如HAL)访问外部 I/O ，只需要在 OEMAddressTable 中定义物理地址到虚拟地址间的映射关系就可以了；如果是应用程序或者驱动要访问 I/O ，要做的工作包括： 1 。在 CPU 物理地址和虚拟地址间做一个动态映射， 2 。对虚拟地址进行操作。 
Trackback:http://tb.blog.csdn.net/TrackBack.aspx?PostId=1533948
[收藏到我的网摘]   milkyway1888发表于2007年03月19日 17:19:00 
**//===================================================================================================================================**

在X86和ARM架构的CPU中,wince访问系统内存的方法随程序所属模式层次的不同而有所区别.
  1.在系统内核模式下(kernel mode),在OAL层访问,只需要在OEMAddressTable 中做静态的虚实地址映射就可以了.例如X86架构的映射表格式如下:
   ; OEMAddressTable defines the mapping between Physical andVirtual Address  // 定义4GB的虚拟地址和512MB存储的映射关系
   ;   o MUST be in a READONLY Section
   ;   o First Entry MUST be RAM, mapping from 0x80000000-> 0x00000000
   ;   o each entry is of the format ( VA, PA, cbSize )
   ;   o cbSize must be multiple of 4M
   ;   o last entry must be (0, 0, 0)
   ;   o must have at least one non-zero entry
   ; RAM 0x80000000 -> 0x00000000, size64M       //把物理地址为0x00000000映射到虚拟地址为 0x80000000 处
   dd  80000000h,    0,   04000000h
   ; FLASH and other memory, if any
   ; dd  FlashVA,     FlashPA,    FlashSize
   ; Last entry, all zeros
   dd  0   0   0
2.在驱动或应用程序(user mode)中访问RAM,既可以通过OEMAddressTable+VirtualCopy方式,也可以直接用MmMapIoSpace函数建立物理地址到当前进程虚拟地址的映射关系.
经过OEMAddressTable,实现的只是CPU物理地址到OS内核层虚拟地址的一次映射,如果需要在普通的应用程序中访问内存,还要再用VirtuaAlloc+VirtualCopy做一个内核到当前进程的二次映射(有一种情况例外，就是你的OS被配置成Full Kernel Mode，这时任何应用程序都可以访问OS内核地址).
     简单说明几个关键函数:
     VirtualAlloc用于在当前进程的虚拟地址空间中保留或者提交空间，在保留时以64KB为单位，提交时以4KB为单位。其函数原型为 
 LPVOID VirtualAlloc( 
 LPVOID lpAddress,  // 分配虚拟地址的起始指针 
 DWORD dwSize,     // 大小，以字节为单位 
 DWORD flAllocationType, // 类型，设为MEM_RESERVE
 DWORD flProtect    // 存取保护，设为PAGE_NOACCESS
); 
 VirtualCopy 用来绑定物理地址到静态映射虚拟地址： 
 BOOL VirtualCopy( 
 LPVOID lpvDest,        // 虚拟目的地址指针，接受VirtualAlloc的返回值
 LPVOID lpvSrc,        // 源物理地址指针 
 DWORD cbSize,         // 大小必须与虚拟地址相同 
 DWORD fdwProtect  // 存取保护类型 
); 
这里需要注意的是 fdwProtect参数。如果是驱动程序访问，需要设置为 PAGE_NOCACHE ，以访问无缓存段虚拟地址。如果映射的物理地址范围在 0x1FFFFFFF 之上，必须使用 PAGE_PHYSICAL ，此时必须把lpvSrc右移八位，实现地址对齐。（这是由内核中 VirtualCopy 的实现决定的，在那个函数中会判断如果是 PAGE_PHYSICAL
 就将 PHYSADDR 左移 8 位移回来，源代码位于 private/winceos/coreos/nk/kernel 目录下的 virtmem.c中的DoVirtualCopy ）
    MmMapIoSpace 用来把物理地址直接映射到与进程无关的虚拟地址上。函数原型为 
 PVOID MmMapIoSpace( 
 PHYSICAL_ADDRESS PhysicalAddress, 
 ULONG NumberOfBytes, 
 BOOLEAN CacheEnable
);
  一个使用 VirtualAlloc+Copy 的例子：把物理地址为 0x10000000 的单元映射到虚拟地址空间中。 
#include <windows.h> 
#definePHYSADDR ((PVOID)0x10000000) 
// PHYSADDR is the physicaladdress of the peripheral 
// registers 
#define SIZE (4800*4) 
LPVOID lpv; 
BOOL bRet; 
lpv = VirtualAlloc(0, SIZE,MEM_RESERVE, PAGE_NOACCESS); 
// For a user mode driver, alwaysleave the first 
// parameter 0 and use only theflags MEM_RESERVE 
// and PAGE_NOACCESS Check thereturn value: lpv == 0 
// is an error 
printf(TEXT("VirtualAllocreservation @%8.8lx\r\n"), lpv); 
bRet = VirtualCopy(lpv,PHYSADDR>>8, SIZE, PAGE_READWRITE | PAGE_NOCACHE | PAGE_PHYSICAL);
// The lpv parameter is thevirtual address returned 
// by VirtualAlloc(). 
// Always use PAGE_NOCACHE */ 
// Check the return value: bRet==0 is an error */ 
printf(TEXT("VirtualCopyreturned: %d\r\n"), bRet); 
// At this point lpv is a virtualaddress which maps 
// the I/O registers 
// at PHYSADDR for SIZE bytes */ 
Trackback:http://tb.blog.csdn.net/TrackBack.aspx?PostId=1538654
**[wince的存储器操作与地址映射知识点滴](http://blog.csdn.net/milkyway1888/archive/2007/04/04/1551707.aspx)**
 1.如果是在bootloader中访问设备寄存器，可以直接操作物理地址。
2.wince启动后，硬件上ARM和X86体系的处理器启动了MMU，操作系统只能访问到虚拟地址，不能直接操作物理内存了。但是如果是X86的CPU，由于它的外设I/O端口和存储器空间分开编址，可以直接嵌入汇编或者使用宏read_port_xxx,write_port_xxx来读写设备寄存器的物理地址。
3.wince软件结构里对应MMU的是一个名为OEMAddressTable的数据结构（源文件oeminit.asm中），其中建立了物理地址和虚拟地址的静态映射关系，也可以在其中改动系统所能识别物理内存的大小,以支持大内存。
4.我们也可以在wince启动后调用CreateStaticMapping和NKCreateStaticMapping来实现OEMAddressTable中的这种物理地址和虚拟地址的静态映射关系。
5.建立了静态映射关系的虚拟地址只能由内核模式下的程序来操作，例如 ISR。除非你在定制系统时，选择了full kernal mode，使所有程序都运行在完全内核模式下，这将导致系统不稳定。
6.如果要在驱动程序中访问设备寄存器，必须建立动态虚拟地址映射，可以调用MmmapIoSpace函数来实现，或者通过VirtualAlloc和VirtualCopy函数来实现。其实MmmapIoSpace内部就调用了后者。
7.在驱动中访问虚拟地址时，必须是非缓存段（位于0xA0000000 到 0xBFFFFFFF ）。
8.使用VirtualCopy函数映射物理内存时，其lpvSrc参数必须右移8位，且相应的fdwProtect参数必须带page_physical。
9.如果是ARM体系的处理器，访问挂在系统总线上的设备寄存器前，必须先把总线地址转化成CPU的地址，通过HalTranslateBusAddress实现两种物理地址的变换，然后再调用MmmapIoSpace函数作虚实地址的转换。
  wince5.0下可以使用CreateBusAccessHandle(总线注册表路径）+BusTransBusAddrToVirtual来实现总线物理地址到系统虚拟地址的直接变换。
Trackback: http://tb.blog.csdn.net/TrackBack.aspx?PostId=1551707
//==========================================================================================================================
备注::
1》WINCE5下可以通过AllocPhysMem来分配实际的物理内存，然后再通过 VirtualAlloc 和 VirtualCopy这组函数，建立实际物理地址与虚拟地址之间的映射关系（这种映射关系的建立跟一般的GPIO实际物理地址跟虚拟地址的映射建立是一样的）。映射建立后，在程序里面操作虚拟地址就可以操作到实际的物理地址，AllocPhysMem分配到的物理内存也可以直接赋给DMA的数据源地址，DMA需要指向的地址是联系的物理地址。我们要通过DMA发数据，就可以先把数据写到虚拟地址，DMA从实际物理地址取到的数据，就是我们赋给虚拟地址的值。
2》VirtualAlloc 和 VirtualCopy这组函数的一般用法，注意3C2450_BASE_REG_PA_IOPORT这个就是一物理地址
    #define S3C2450_BASE_REG_PA_IOPORT
                  0x56000000
 // GPIO Virtual alloc
    g_pGPIOregs = (volatile S3C2450_IOPORT_REG *) VirtualAlloc(0,sizeof(S3C2450_IOPORT_REG), MEM_RESERVE, PAGE_NOACCESS);
    if(g_pGPIOregs == NULL)
    {
        RETAILMSG(1,(TEXT("For pGPIOregs: VirtualAlloc failed!\r\n")));
        bResult = FALSE;
        goto CleanUp;
    }
    else
    {
        if(!VirtualCopy((PVOID)g_pGPIOregs,(PVOID)(S3C2450_BASE_REG_PA_IOPORT >> 8),sizeof(S3C2450_IOPORT_REG),PAGE_PHYSICAL | PAGE_READWRITE | PAGE_NOCACHE ))
        {
            RETAILMSG(1,(TEXT("For pGPIOregs: VirtualCopy failed!\r\n")));
            bResult = FALSE;
            goto CleanUp;
        }
    }
   3》一般可以去看芯片的地址空间映射图(Memory  Map)来看用AllocPhysMem函数分配的实际物理地址是不是正确的，注意多片物理内存的情况，多个片选，多个物理内存空间
   4》还有一种实际物理地址与虚拟地址之间的映射关系通过g_oalAddressTable表来建立，S3C6410 BSP包里面这个表定义在Oemaddrtab_cfg.inc文件里面

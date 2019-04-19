# WINCE OAL层简介 - xqhrs232的专栏 - CSDN博客
2013年08月17日 22:33:12[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：591
原文地址::[http://blog.csdn.net/chenyt2007/article/details/1661632](http://blog.csdn.net/chenyt2007/article/details/1661632)
相关文章
1、[WinCE OAL架构分析](http://blog.csdn.net/nanjianhui/article/details/3830452)----[http://blog.csdn.net/nanjianhui/article/details/3830452](http://blog.csdn.net/nanjianhui/article/details/3830452)
2、[WinCE6.0
 OAL的特点](http://www.cnblogs.com/we-hjb/archive/2008/10/14/1311286.html)----[http://www.cnblogs.com/we-hjb/archive/2008/10/14/1311286.html](http://www.cnblogs.com/we-hjb/archive/2008/10/14/1311286.html)
3、WinCE OAL中的Startup.s----[http://blog.sina.com.cn/s/blog_69b384950100vtaj.html](http://blog.sina.com.cn/s/blog_69b384950100vtaj.html)
4、[WinCE OAL中的中断处理(转载 )](http://blog.csdn.net/xqhrs232/article/details/4915771)----[http://blog.csdn.net/xqhrs232/article/details/4915771](http://blog.csdn.net/xqhrs232/article/details/4915771)
5、[程序访问物理地址之MmMapIoSpace 与 OALPAtoVA](http://blog.csdn.net/zhandoushi1982/article/details/4581226)----[http://blog.csdn.net/zhandoushi1982/article/details/4581226](http://blog.csdn.net/zhandoushi1982/article/details/4581226)
6、[WinCE 6.0中断驱动程序分析](http://www.cnblogs.com/we-hjb/archive/2009/05/26/1490461.html)----[http://www.cnblogs.com/we-hjb/archive/2009/05/26/1490461.html](http://www.cnblogs.com/we-hjb/archive/2009/05/26/1490461.html)
Windows CE是微软针对嵌入式领域推出的一款全新的操作系统。之所以说它是一款全新的操作系统，是因为尽管Windows CE的UI非常接近其它的桌面版Windows操作系统，但是它的内核完全是重新写的，并不是任何一款桌面版Windows的精简版本。 Windows CE是一种支持多种CPU架构的操作系统，这其中包括ARM、x86、MIPS和SHx，极大地减轻了0EM开发过程中移植操作系统的工作量。
    操作系统移植包含两个层面上的工作：一个层面是CPU级的，另一个层面是板级的。CPU级的移植通常由微软或芯片制造商来完成；板级移植则是由OEM来完成的。0AL正是0EM完成这一系统移植的工作核心！
**1. OAL**
    OAL的全称是OEM Adaption Layer，即原始设备制造商适配层。从逻辑结构上看，它位于操作系统的内核与硬件之间，是连接系统与硬件的枢纽；从功能上看，OAL颇似桌面机上的BIOS，具有初始化设备、引导操作系统以及抽象硬件功能等作用。与B10S不同的是，0AL隶属于操作系统，是操作系统的一部分。从存在方式上，讲OAL是一组函数的集合体，这些函数体现出0AL的功能
**2 最小化的OAL**
    OAL层的首要任务是加载内核。OAL层中为内核的启动作种种铺垫的函数的集合构成最小OAL层。我们可以由此深入0AL层 
首先来看一下OS的启动顺序。 
    ①CPU执行引导向量，跳转到硬件初始化代码，即Startup函数； 
    ②在start up函数完成最小硬件环境初始化后跳转到KernelStart函数(当CPU为x86架构时为Kernel Initial-ize函数)，来对内核进行初始化； 
    ③Kernelstart函数调用OEMInitDebugSerial完成对调试串口的初始化，调用0EMInit函数来完成硬件初始化工作以及设置时钟、中断，调用OEMGetExtensionDRAM函数来判断是否还有另外一块DRAM。 
    至此，内核加载完毕。由此可见，OS启动的重中之重是Startup函数的正确加载。
**2.1 Startup **Startup阶段的特点是Kernel还没有加载起来，调试工作比较困难。StartuP函数的两大核心任务分别是把CPU初始化到一已知状态和调用内核初始化函数来初始化内核。以下是Startup函数中通常包含的内容： 
    ①把处理器置为监控模式； 
    ②禁止CPU的IRQ和FIQ输入； 
    ③禁止内存管理单元MMU和指令、数据Cache； 
    ④刷新指令和数据Cache、TLB、清空写buffr； 
    ⑤确定启动的原因一hard reset，wake from sleep， 
    GPIO reset，Watchdog reset，eboot handoff； 
    ⑥根据目标板需要配置GPIO，比如连接LED的GPIO； 
    ⑦配置内存管理器，设置刷新频率，使能时钟； 
    ⑧配置中断控制器； 
    ⑨初始化实时时钟(RTC)为0，使能实时时钟； 
    ⑩设置电源管理寄存器；
    ⑾打开所有板级时钟和片内外部时钟；
    ⑿取得OEMAddressTable的物理基地址并把它存在r0中； 
    ⒀跳转到KernelStart。 
   Bootloader和OAL中均包含Startup函数。它的功能大致相同，都是要初始化最小硬件环境。Bootloader是在为自己的执行准备硬件环境，OAL则是为kernel的执行准备硬件环境。由于这两种硬件环境要求基本相同，所以它们的代码也有很大部分可以相互借鉴。但应该明白，Bootloader与OAL在物理上是独立的，它们并不是同一段代码。而且，如果可以确定这一硬件部分Bootloader已经初始化过，则在OAL中不必重复。当然，前提是每次加载都要经过Bootloader这一环节。
     S t a r t u P执行完毕后，跳转至K e r n e 1 S t a r t/Kemellnitialize
**2.2 Kernel Start **    Kernel Start主要完成内核的最小初始化并且通过调用OEMInit函数来完成板级硬件初始化。以下是ARM内核初始化过程： 
    ① 初始化一级页表； 
    ②使能MMU和cache； 
    ③为每种工作模式使能栈(stack)； 
    ④重新定位内核； 
    ⑤执行串口调试函数； 
    ⑥调用OEMInit； 
    ⑦初始化内存； 
    ⑧执行其它初始化。 
    KernelStart中用到的三个函数OEMInit ()、OEMInitDebugSerial()和OEMGetExtensionDRAM()中，OEMInit()硬件相关性较大，也最重要。(1)OEMlnit() 0EMInit的最小任务是初始化其它硬件和注册系统时钟。通常OEMInit应该完成以下工作。 
    ①通过设置以下值来设置中断映射表一SYSINTR→IRQ和IRQ→SYSINTR。 
    ②在中断映射表中设置静态中断映射。 
    ③设置KITL，但在最小化的OAL层中通常不包括KITL。 
    ④用Init Clock配置系统定时器、实时时钟、时钟。 
    ⑤确定系统时钟的中断源。 
    ⑥初始化内核时间粒度为1ms。 
    ⑦配置中断控制器或可编程中断控制器(PICS)。 
    ⑧提供调试用LED指示灯。 
    ⑨置pWriteDedugLED=OEMWriteDedugLED。 
    ⑩调用HookInterrupt函数来注册中断服务例程ISRs，以下示例说明如何处理TIMERISR硬件中断5的中断服务例程：
void OEMInit(void)
{...
HookInterrupt(5，TIMERISR)；//注册定时器中断
…
}
    ⑾ 清除中断掩码，防止初始化内核时有中断申请。
**(2)串口调试函数 **有限的调试手段是0S移植人员经常遇到的难题。串口调试函数虽然不像以太网口调试函数那样功能强大，但仍然要比LED指示灯或数码管要直观得多，是调试OAL层代码不可或缺的一组工具。这个函数组由四个函数组成，分别是0 E M I n i t D e b u g S e r i a l()、OEMReadDebugByte()、OEMWriteDebugByte()和OEMWriteDebugString()。
    ◇OEMInitDebugSerial()用于配置串口；
    ◇OEMReadDebugByte0和OEMWriteDebugByte()用于 向串口读写一个字节；
    ◇OEMWriteDebugString()用于向串口写一个调试用字符串。 
    KernelStart中调用的是OEMInitDebugSerial()，完成串口初始化，为串口调试工作作好准备。
**(3)OEMGetExtensionDRAM() **
    在最简最小化OAL层函数中，OEMGetExtensionDRAM()并不是一个必需的函数。OEMGetExtensionDRAM()的主要功能是查询是否存在另外一片DRAM．如果目标板上只有一片DRAM，则该函数返回FALSE。但在KernelStart通常都包含此函数。 
至此，最小的OAL层已经完毕，kernel的最基本的功能可以正常使用。骨架搭起，第一阶段的任务告一段落，但是很多非常重要的功能还不完整，还不能做到物尽其用。于是需要进一步加强OAL层的功能。这种做法也是OAL层开发通常使用的方法。先完成基本功能，在基本功能确保正确无误后，逐渐加入其它功能。循序渐进，即使出错也很容易找到出错的地方，便于排查。
**3 加强OAL **
    第二阶段主要目的是充分利用板上硬件资源和加强调试手段。主要包括中断、KITL、以太网口调试函数和OEMIOControl四方面内容。我们把包含这四方面内容的OAL层称为加强OAL。
**3.1 中 断 **
    外设硬件与CPU的数据交换基本上是异步进行的、最常用的中断形式。CE的中断处理顺序如图3所示。由图3可知，CE的中断实际上是由两部分ISR和IST组成的。其中IST包含在驱动程序中，而ISR包含在OAL层中。所以，要想支持一个硬件，首先必须从0AL层为其作好准备。这个准备用两步完成。
     ①创建中断标识符。下面代码节选自SAMSUNG2410的oalintr.h。中断映射表通常位于   <CEVersion>/Platform/<platform name>/INC。
#define SYSINTR USB (SYSlNTR FIRMWARE+11) 
#define SYSINTR USBD (SYSlNTR_FIRMWARE+12) 
     ② 创建并注册ISR。ISR的主要任务是返回中断标识符。ISR代码通常位于<CE Version>/Platform/<Platformname>/KERNEL/HAL下。 
下面代码节选自SAMSUNG2410的armint.c。
if(IntPendVal==INTSRC_ADC){ 
s2410INT.>rlNTSUBMSKl=BIT_SUB_TC； 
s2410INT_>rINTMSK |=BIT_ADC； 
s2410INT_>rSRCPND |=BIT_ADC； 
s2410INT_>rINTPND =BIT_ADC； 
return(SYSINTR_TOUCH)；
} 
在中断处理中，还有三个函数也起着至关重要的作用。它是OEMInterruptEnable()、OEMInterruptDisable()和OEMInterruptDone()。 
    ◇OEMInterruptEnable()用于执行允许设备产生中断的硬件操作； 
    ◇OEMInterruptDisable()禁止设备发出中断申请； 
    ◇OEMInterruptDone()中断处理结束。
**3.2 以太网口调试函数**
以太网口调试函数与串口调试函数相比，具有更快的速度。 
    ◇OEMEthInit 初始化以太网调试口； 
    ◇OEMEthEnableInts开以太网适配器中断； 
    ◇OEMEthDisableInts关以太网适配器中断； 
    ◇OEMEthISR 以太网适配器中断服务例程； 
    ◇OEMEthGetFrame从以太网调试口收数据； 
    ◇OEMEthSendFrame从以太网调试口发数据； 
    ◇OEMEthQueryClientlnfo获取平台相关信息；
    ◇OEMEthGetSecs 返回从某一特定时间开始的计时值。本函数用于处理超时。
**3.3 KITL**
KITL全称为Kernel Independent TransportLayer。它的主要用途是提供更方便的调试手段，如图4所示。KITL出现在Windows CE．net之后，把软件传输协议与硬件传输层隔离开。KITL使得开发者不必了解硬件传输层如何与软件协议层接口。 
    以下是应该在OEMInit函数中加入的KITL初始化代码。 
    ①初始化所有PCI桥和设备，枚举它们并且给它们分配资源，然后使能，使他们能正常工作。注：此条适于有KITl网络接口卡(NIC)和NIC桥的情况。 
    ② 对相关总线进行初始化，使得CPU能够正确识别NIC。 
    ③通过调用KitlInit函数来初始化KITL。这部分代码可参照其它平台，代码文件为Halkitl.c。 
    ④执行0EMKitlInit函数，进行相关的硬件初始化工作。搜索是否存在KITL 网口、串口或并口连接。 
    ⑤执行完OEMKitlInit后，把Kitl.1ib和Kitleth.1ib包含入平台资源文件/<Platform>/Kernel/Buildexe/Kernkitl，以便把KITL打包进内核。有关KITL的其它函数请参考微软MSDN
**3.4 OEMIOControl**
OEMIOContr01在OAL层是一个非常重要的函数，应用程序是通过调用KernelIoContrOI来调用OEMl0Control的。内核对许多硬件平台信息的获得都要通过对它的调用来实现。此外，0EMl0Contr0I还是用户模式应用代码到内核模式OAL代码之间的转换入口。这就是说，用在用户模式下通过调用0EMl0Control可以获得内核模式的权力。0EMIOControl函数原型如下：
BOOL OEMIoControl(......) 
{switch(dwloControlCode) 
{caseIOCTL_HAL_SET_DEVICE_INFO： 
case10CTL_HAL_REBOOT： 
……
default： 
return FALSE； 
} 
return TRUE： 
} 
    硬件资源利用和调试手段的加强大大丰富了OAL的功能，但是**嵌入式系统**通常会面临的功耗问题和由于网络功能的日益普及而带来的安全性问题并没有涉及到。
**4 完整OAL **    完整OAL是指在加强OAL的基础上扩充了功耗和安全性验证的OAL。所以这一阶段的主要工作集中在电源管理与模块认证两部分。
**4.1 电源管理 **
    OAL层的电源管理与驱动程序的电源管理颇为不同。一种设备驱动程序仅负责某种特定的设备，如果可能，则把这种设备置为省电模式，当形势需要时再把设备置为满载荷模式。OAL层的电源管理则是负责整个系统功耗管理。例如，调度器在下一个25ms没有线程要运行时，系统将被置为省电模式。
    电源管理函数响应关闭系统和使系统空闲的系统调用。这些系统调用可能是软触发也可能是硬触发。以下两个函数是须在OAL层中实现的电源管理函数： 
    ◇0EMIdle一一把设备置为空闲状态，此时系统处于低功耗状态； 
    ◇0EMPoweroff一一把设备置为断电状态； 
    ◇OEMPowerOff和OEMIdle的程序代码可在如下目录中参照例程％_WINCER00T％/Platform/<Platform>/Kerlael/Hal。
**4.2 模块认证 **
    自从Windows CE 3.0以来，在RAM中加载和运行模块前，内核可以对其进行授权核查。对于在ROM中运行的模块则不需要此过程。模块认证实际上是在被加载的模块后添加一数字签名，只有当系统用公开密钥验证数字签名通过后，该模块才可以被加载到RAM中运行。这样系统可以阻止或限制一些模块的运行，达到系统安全的目的。 
    要达到以上目的须完成以下两个函数： 
    ◇OEMCertifyModuleInit，用于初始化验证过程，每验 证一个模块调用一次； 
    ◇OEMCertifyM0dule，用于验证数字签名。 
    为了支持这两个函数，在OEMInit函数中须分配两个全局变量pOEMLoadInit和p0EMLoadModule，用来存放这两个函数的地址。
**结语**
    Windows CE的OAL层是一个复杂的函数集。它的复杂性不但体现在包含函数数目繁多，而且体现在很多函数的硬件相关性非常大。本文并没有详细讲解每个OAL层函数，而是就一些通常会遇到的OAL层函数进行层层划分；在说明OAL层的功能和结构的同时，提出开发OAL的一种方法和思路。

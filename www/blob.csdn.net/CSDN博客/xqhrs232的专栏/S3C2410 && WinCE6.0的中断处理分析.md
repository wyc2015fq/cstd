# S3C2410 && WinCE6.0的中断处理分析 - xqhrs232的专栏 - CSDN博客
2013年11月25日 13:57:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：962
原文地址::[http://www.cnblogs.com/we-hjb/archive/2008/11/08/1329830.html](http://www.cnblogs.com/we-hjb/archive/2008/11/08/1329830.html)
 S3C2410的内核是ARM920T，所以，这里先介绍一下ARM920T的异常。ARM920T中有一个当前程序状态寄存器(CPSR)，其中BIT6和BIT7分别控制FIQ和IRQ的使能与否。大家经常说的开中断和关中断，就是指的设置这两个BIT。
       ARM体系的异常中断如下图所示：
![](http://images.cnblogs.com/cnblogs_com/we-hjb/2410EXTable.png)
可以看到，ARM920T中一共有7中异常模式，如果同一时刻有多个异常发生，系统则通过优先级顺序来决定处理其中的哪一个异常。他们之间的优先级顺序从高到低依次是：
1.Reset复位
2.Data Abort数据访问中止
3.FIQ 快速中断请求
4.IRQ 外部中断请求
5.指令预取中止
6.未定义的指令和软件中断
当系统发生异常时，PC指针将跳转到相应的异常中断处理程序处。异常中断和其处理程序之间的对应关系被称为异常向量表，就是通常所说的中断向量表。一般情况下，它存放在低地址(0x0)，但在WinCE中，该表存放在高地址(0xffff0000)。
S3C2410的中断处理过程如下图所示：
![](http://images.cnblogs.com/cnblogs_com/we-hjb/2410INTPROC.png)
可以看到，S3C2410中跟中断密切相关的寄存器主要有以下几个：
SUBSRCPND(二级源待决寄存器)：硬件产生中断后，该寄存器的相应位被置1；
SUBMASK(二级屏蔽寄存器)：若该寄存器的对应位为1，则屏蔽该中断，不再往前提交；
SRCPND(源待决寄存器)：如果是二级中断源产生了中断，当SUBSRCPND和SUBMASK满足条件时，该寄存器的相应位被置1，或者由一级中断源直接引起该寄存器的对应位置1；
MASK(一级屏蔽寄存器)：如果该寄存器的对应位为1，则屏蔽该中断，不再往前提交；
MODE(中断模式寄存器)：决定中断是FIQ模式还是IRQ模式，系统中只能有一个FIQ中断。若当前中断为FIQ模式，则产生一个FIQ异常，CPU进入FIQ异常处理程序。
PRIORITY(优先级控制寄存器)：控制各中断源的优先级。当有多个中断源同时发出请求时由优先级最高的中断源最终产生IRQ。
INTPND(中断待决寄存器)：当SRCPND某一位被置1，且没有被屏蔽，则该寄存器的相应位也被置1，同时产生一个IRQ异常，CPU进入IRQ异常处理程序。
IRQ异常处理程序中，需要清除SRCPND、INTPND。清除SRCPND和INTPND的方法比较特殊，并不是往对应的位写0，相反，应该往对应的位写1。一般是将其值读取出来，再写进去，以完成清除SRCPND和INTPND的工作。
除了以上几个寄存器外，2410还有一个INTOFFSET寄存器，用来表明当前是哪一个中断请求处理。WinCE的OEMInterruptHandler()函数，就是根据其值来判断当前是哪一个中断发出请求。该寄存器在清除SRCPND和INTPND时，被自动复位。所以，代码中不必对其进行处理。
如果中断源是EINT4-23，则还需处理EXTINTn、EINTFLT、EINTMASK和EINTPEND等几个寄存器。另外，由于2410的中断引脚一般与IO复用，所以在使用特定的外部中断之前，需要设置相应的GPIO，使其工作在中断模式下。
WinCE6.0中的中断处理过程如下图所示：
![](http://images.cnblogs.com/cnblogs_com/we-hjb/WinCE6.0INTPROC.png)
     可以看到，整个处理过程分为四层，分别是硬件、内核、OAL和驱动。硬件产生一个IRQ，CPU进入中断服务程序，调用OAL中的OEMInterruptHandler()函数，根据IRQ返回一个SYSINTR，内核根据该SYSINTR，设置一个事件，驱动中捕获到该事件，执行相应的处理程序，完成处理后，调用InterruptDone()，通知CPU中断处理完成。
这里说明一下SYSINTR和IRQ的概念。IRQ一般被认为是物理中断号，由硬件决定。SYSINTR是逻辑中断号，一般跟IRQ一一对应。这种对应关系可以在OAL中静态建立，一般通过函数OALIntrStaticTranslate()实现，静态映射的IRQ一般是MCU内部的中断源，如USB
 Host。为了提高驱动的可移植性，通常采用动态映射的方式，如网卡驱动。不同的硬件平台，可能使用不同的外部中断供网卡使用，通过动态映射的方式，只需修改注册表中的相应键值就能完成驱动的移植，而无须修改代码。驱动中动态映射IRQ的方法是调用函数KernelIoControl()，第一个参数为IOCTL_HAL_REQUEST_SYSINTR，传入物理中断号IRQ，正确返回后，会产生一个SYSINTR，最终完成动态转换的是函数OALIntrRequestSysIntr()。IRQ和SYSINTR的映射关系在文件C:\WINCE600\PLATFORM\COMMON\SRC\COMMON\INTR\BASE\map.c中实现。
驱动中使用中断的典型过程如下：
1.初始化GPIO，以及相应中断寄存器，配置中断的工作模式。
2.建立一个SYSINTR与IRQ对应，调用函数KernelIoControl()。
3.创建一个事件与SYSINTR关联，调用函数CreateEvent()。
4.创建一个线程，在线程中等待创建的事件，调用函数WaitForSingleObject()。
5.完成处理后，通知内核本次中断处理结束，调用函数InterruptDone()。
OAL层跟中断相关的有如下几个函数：
OALIntrInit()：初始化中断寄存器及相应的GPIO，可建立静态的IRQ到SYSINTR的映射关系。
OALIntrRequestIrqs()：获取设备的IRQ号，如通过IO Address获取该设备对应的IRQ。
OALIntrEnableIrqs()：使能中断源，主要完成清除中断屏蔽寄存器和中断待决寄存器。
OALIntrDisableIrqs()：关闭中断源，通过设置中断屏蔽寄存器的相应位以屏蔽中断源。
OALIntrDoneIrqs()：清除中断屏蔽寄存器和中断待决寄存器使MCU能处理下一次中断。
OEMInterruptHandler()：将IRQ号转换为SYSINTR，内核的中断服务程序将根据此值设定特定的事件。
内核中跟中断相关的工作主要有以下几个部分：
1.定义异常处理函数，其实现文件为C:\WINCE600\PRIVATE\WINCEOS\COREOS\NK\KERNEL\ARM\armtrap.s。
2.创建中断向量表，其实现文件为C:\WINCE600\PRIVATE\WINCEOS\COREOS\NK\KERNEL\ARM\exvector.s
3.中断向量的初始化，其实现在文件C:\WINCE600\PRIVATE\WINCEOS\COREOS\NK\KERNEL\ARM\mdarm.c中，其中的代码表明WinCE的中断向量表在高端(0xffff0000)。

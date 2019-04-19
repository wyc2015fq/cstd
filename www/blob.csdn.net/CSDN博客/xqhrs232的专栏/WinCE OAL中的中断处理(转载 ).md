# WinCE OAL中的中断处理(转载 ) - xqhrs232的专栏 - CSDN博客
2009年12月01日 16:57:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1125
关于WinCE的中断处理，OAL中主要是实现了ISR部分，一般IST会在设备驱动中实现。推荐一篇WinCE的中断架构的文章，如下：
[http://msdn.microsoft.com/zh-cn/library/ms836807.aspx](http://msdn.microsoft.com/zh-cn/library/ms836807.aspx)
建议对WinCE中断不了解的朋友，可以先看看这片文章。架构如图：
![Interrupt Architechture](https://p-blog.csdn.net/images/p_blog_csdn_net/nanjianhui/EntryImages/20090303/Interrupt_1.JPG)
这张图想必很多人都见过，主要这张图太经典了，所以还是贴出来唠叨几句，硬件中断产生以后，会导致内核ISR的运行，然后由OAL中的ISR来处理相应的中断，最后导致相对应的IST运行完成真正的中断处理。所以在WinCE中，中断处理由ISR和IST共同完成。ISR主要完成中断源的确定，屏蔽该中断并返回给内核相对应的系统中断号，ISR应该尽量短小。IST则是完成真正的中断处理，比如数据的传输和解析等。当然不是所有的中断处理都需要ISR和IST，看需要，比如WinCE的系统Timer中断就只需要ISR完成。
在OAL中支持中断，需要实现以下几个中断处理函数：
1. BOOL OEMInterruptEnable(DWORD sysIntr, VOID* pData, DWORD dataSize)
sysIntr：要被使能的系统中断号
pData：传入的数据指针，该数据由InterruptInitialize函数传入
dataSize：传入数据的大小
该函数用于使能某一个硬件中断。在设备驱动调用InterruptInitialize来初始化一个中断的时候，内核就会调用该函数来使能相应的硬件中断。
2. VOID OEMInterruptDisable(DWORD sysIntr)
sysIntr：要被屏蔽的系统中断号
该函数用于屏蔽一个硬件中断。如果设备驱动调用InterruptDisable函数，则该函数会被调用。
3. VOID OEMInterruptDone(DWORD sysIntr)
sysIntr：要被重新使能的系统中断号
该函数标志着一个中断处理过程的完成。当设备驱动调用InterruptDone函数的时候，该函数会被调用，重新使能相应的硬件中断。
4. ULONG OEMInterruptHandler(ULONG ra)
ra：指令计数，在实际应用中，没有太大意义
当硬件中断产生的时候，该函数就会被调用完成ISR部分的中断处理。一般会在该函数中读取系统的中断标记位，确定中断源并返回相应的系统中断号。
5. void OEMInterruptHandlerFIQ(void)
针对于ARM处理器，该函数用于处理快速中断。
上面5个函数完成了中断的相关处理。这里要提到两个概念：IRQ和SYSINTR。IRQ是指物理中断或者叫硬件中断，而SYSINTR指的是系统中断，也有的地方称为虚拟中断或者逻辑中断，我个人觉得还是叫系统中断比较好。每一个IRQ会和一个系统中断SYSINTR相对应，当硬件中断产生时，ISR实际上是处理IRQ中断，然后返回相应的系统中断SYSINTR给内核，内核会根据相应的SYSINTR触发相应的IST来完成中断处理。
IRQ和SYSINTR之间的对应关系称为映射，分为静态映射和动态映射。静态映射是指在系统编译时IRQ已经和SYSINTR相对应，一般通过OALIntrStaticTranslate函数来实现。而动态映射是指WinCE系统启动后，动态关联IRQ与SYSINTR，一般通过KernelIoControl(IOCTL_HAL_REQUEST_SYSINTR…)来实现。
SYSINTR的类型在nkintr.h中定义，OEMInterruptHandler函数在处理完中断以后，会返回不同类型的SYSINTR给内核，内核会根据返回值进行下一步操作，分为以下几种类型：
SYSINTR_NOP：表示不需要进行任何处理
SYSINTR_RESCHED：表示要进行一次系统调度
SYSINTR_CHAIN：表示不是该中断源产生，在中断链中寻找下一个中断
SYSINTR_RTC_ALARM：表示RTC报警产生
SYSINTR_TIMING：用于ILTiming测试
SYSINTR_PROFILE：用于系统的profile
SYSINTR_FIRMWARE：用于用户自定义系统中断号，所有自定义的系统中断号都应该基于该值进行累加加1，这些自定义的系统中断号用于和IRQ一一对应。
在以前，如果要实现OAL中的中断部分，我们需要自己建立一个IRQ表和一个SYSINTR表，然后实现前面提到的5个函数，还要实现一个InterruptInitialize的函数，该函数用于初始化中断，会在OEMInit中被调用，就完事了。自从WinCE5.0以后，微软提出了PQOAL架构，中断的实现变得“曲折”了。要实现的函数如下：
1. BOOL OALIntrInit()
该函数为中断初始化函数，会在OEMInit中被调用，用于初始化系统的中断，以及完成一些中断的静态映射。
2. BOOL OALIntrRequestIrqs(DEVICE_LOCATION *pDevLoc, UINT32 *pCount, UINT32 *pIrqs)
pDevLoc：一个DEVICE_LOCATION结构指针，包含设备信息
pCount：作为输入表示最大的IRQ数，作为输出表示实际获得的IRQ数
pIrqs：指向一个实际获得的IRQ数组
该函数用于通过设备的物理地址来得到IRQ信息，一般用于总线设备。
3. BOOL OALIntrEnableIrqs(UINT32 count, const UINT32 *pIrqs)
count：要使能多少个IRQ
pIrqs：要被使能的IRQ数组
该函数用于使能IRQ中断，该函数会被OEMInterruptEnable函数调用。
4. VOID OALIntrDisableIrqs(UINT32 count, const UINT32 *pIrqs)
count：要禁用多少个IRQ
pIrqs：要被禁用的IRQ数组
该函数用于禁用IRQ中断，该函数会被OEMInterruptDisable函数调用。
5. VOID OALIntrDoneIrqs(UINT32 count, const UINT32 *pIrqs)
count：要重新使能多少个IRQ
pIrqs：要被重新使能的IRQ数组
该函数用于重新使能IRQ中断，会被OEMInterruptDone函数调用。
除了上述5个函数以外，还要实现的一个重要函数就是OEMInterruptHandler了，这个函数前面介绍过，这里不说了。实际上在WinCE5.0以后，在/Platform/Common/Src/Common/INTR/Base目录下有个“map.c”文件，该文件实现了中断的相关接口函数，实现了中断的动态/静态映射，还完成了SYSINTR与IRQ之间的转换，我们只需要实现对硬件中断的操作和初始化就可以了。
最后还有几个函数要说一下，如下：
BSPIntrInit：被OALIntrInit函数调用
BSPIntrEnableIrq：被OALIntrEnableIrqs函数调用
BSPIntrDisableIrq：被OALIntrDisableIrqs函数调用
BSPIntrDoneIrq：被OALIntrDoneIrqs函数调用
BSPIntrRequestIrqs：被OALIntrRequestIrqs函数调用
这些函数可以被称为板级中断处理函数，总感觉这些函数有点多余，一般实现了OALIntrInit，OALIntrEnableIrqs，OALIntrDisableIrqs，OALIntrDoneIrqs和OALIntrRequestIrqs就可以了，但这是基于处理器级的实现，对于基于同一处理器的不同的板子可能中断要做一些修改，这些修改就可以在BSPIntrInit，BSPIntrEnableIrq，BSPIntrDisableIrq，BSPIntrDoneIrq和BSPIntrRequestIrqs里面完成。
在这里，OAL中的中断处理函数基本都介绍了。我想最好的理解方法就是看代码了。一般在OAL中只是做一些开关中断和清中断标记位的操作，真正的数据处理交给IST去做。但有的时候，有些特殊设备的中断会很频繁，IST来不及响应，解决办法就是在ISR中将数据保存在一块内存中，然后根据需要，每隔多少个硬件中断返回一次系统中断，从而激活IST将数据一次性读走，这里涉及一个问题就是在ISR和IST中共享数据，在config.bib中预留一块共享内存就可以了。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/nanjianhui/archive/2009/03/03/3953466.aspx](http://blog.csdn.net/nanjianhui/archive/2009/03/03/3953466.aspx)

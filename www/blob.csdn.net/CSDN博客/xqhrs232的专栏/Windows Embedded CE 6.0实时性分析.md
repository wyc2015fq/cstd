# Windows Embedded CE 6.0实时性分析 - xqhrs232的专栏 - CSDN博客
2013年11月23日 11:34:52[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1014
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://www.cnblogs.com/we-hjb/archive/2009/11/29/1613245.html](http://www.cnblogs.com/we-hjb/archive/2009/11/29/1613245.html)
Windows Embedded CE 6.0是微软公司目前最新的嵌入式操作系统之一，支持四种体系结构的MCU（ARM、MIPS、SH4和x86）。在众多的嵌入式操作系统中，它以自己独特的优势脱颖而出，在整个嵌入式操作系统的市场份额中占有很大比重。相比其他嵌入式操作系统，Windows Embedded CE 6.0主要有以下特点：
- 图形用户界面，与桌面操作系统的图形用户界面基本一致，便于用户接受；
- 网络连接的支持，包括PAN、LAN、WAN、BlueTooth和WIFI；
- 多媒体的支持，如DirectDraw、DirectShow、Directory3D及多种音频视频的编解码
- 多语言的支持，可以在定制系统时选择一种或多种语言；
- 多进程的支持，理论上支持32K个进程，满足各种应用需求；
- 组件化的开发方式有效缩短了产品的研发周期；
- 基于VS2005的集成开发环境，让开发更方便。
      以上这些都是我们选择Windows Embedded CE 6.0的原因，但在很多嵌入式系统的应用场合，系统的实时性至关重要。而整个系统的实时性在很大程度上取决于它所采用的操作系统。那么Windows Embedded CE 6.0的实时性到底如何，能否满足我们的应用需求，或者说Windows Embedded CE 6.0是否是一个实时操作系统？本文将根据实际应用，介绍一种在目标硬件平台上测试Windows Embedded CE 6.0实时性的方法。
      在讨论Windows Embedded CE 6.0的实时性之前，先简单介绍一下实时操作系统的概念。实时操作系统除了具有一般操作系统的基本要素外，还必须能及时准确地响应所有不可预知的外部触发，即使在系统处于尖峰负荷状态时。这是一个非常苛刻的要求。根据实际应用的场景不同，实时操作系统一般分为以下两种。
- 硬实时操作系统：准时准确地响应所有的外部触发，一次失败将导致灾难性的后果。这类操作系统在航空航天和工业控制领域应用广泛。
- 软实时操作系统：在不满足响应时限时，系统性能退化，但不会导致灾难性后果。这类操作系统在交换系统中经常有应用。
     这里需要注意，实时操作系统并不是一个“快速”系统，实时系统有限定的响应时间，而并非响应越快越好。事实上，响应时间可以是皮秒级的也可以是秒级的，甚至是分钟级的。只要在系统规定的时限内正确响应所有的外部触发，我们就可以认为它是一个实时操作系统，哪怕这个时限是一小时。举个例子，顾客打电话给肯德基叫外卖，肯德基在规定的30分钟内将顾客点的汉堡送达，那这个过程就可以认为是实时性的，时限为30分钟。
     从2000年发布的WinCE3.0开始，Windows Embedded CE就是一个实时操作系统。随着新版本的不断推出，Windows Embedded CE的实时性也不断提高。毋庸置疑，Windows Embedded CE 6.0也是一个实时操作系统。
通常，对于实时操作系统来说，外部触发一般是指外部中断触发。所以，外部中断的响应时间（中断延迟）是衡量其实时性的一个重要标准。下面将从这一角度分析Windows Embedded CE 6.0的实时性。
Windows Embedded CE 6.0的中断处理过程主要分为两部分：
- 中断服务例程（ISR）：处于内核中的低级处理程序，中断发生时首先被调用。
- 中断服务线程（IST）：处于驱动或者应用中的中断处理线程，由系统调度，完成大部分的中断处理工作。
     ISR的实现在OAL（OEM适配层）中，它只处理最低级的中断响应，通常是设置MCU内部的中断控制寄存器。中断处理的主要部分在驱动或者应用的中断处理线程中。中断处理线程与其他线程一样，使用同一个线程优先级管理系统。ISR和IST之间通过事件对象进行同步。IST中创建一个事件对象，并使用函数WaitForSingleObject()等待该事件被触发。ISR中通知内核触发相应的事件对象。Windows Embedded CE 6.0的中断处理的过程如下图所示。
![clip_image001](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WindowsEmbeddedCE6.0_1155E/clip_image001_thumb.gif)
                  图-1 Windows Embedded CE 6.0的中断处理过程
     可以看到，从外部中断产生到IST运行，中间经历了几个环节，每一个环节都有时间开销，有些时间是基本确定的，有些时间随系统的状态不同而不同。但整个中断延迟主要可分成ISR延迟和IST延迟两部分，如图-2所示，t1为ISR延迟，t2为IST延迟。中断延迟的时间是衡量Windows mbedded CE 6.0实时性的一个重要指标。那么如何在我们系统的硬件平台上测得这个中断延迟的具体值，从而进一步分析Windows Embedded CE 6.0的实时性能否满足项目的实际应用需求。
![clip_image003](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WindowsEmbeddedCE6.0_1155E/clip_image003_thumb.jpg)
        图-2 Windows Embedded CE 6.0的中断延迟模型
      这是中断延迟的一个简要模型，t1是ISR延迟，t2是IST延迟，t1+t2即为中断延迟，我们需要测量和计算的就是这两个值。
Windows Embedded CE 6.0提供了一些工具软件，用来测量中断延迟，如ILTiming.exe。它需要OAL的代码支持，并且在定制系统时设置相应的编译属性，具体用法可参考Platform Builder 6.0的帮助。这里介绍一种借助硬件的测量方法，能在示波器上直观的看出中断延迟。
根据前文所述的中断处理过程以及图-2所示的中断延迟模型，可通过控制MCU的一个GPIO输出引脚来帮助测量t1和t2。具体方法如下：
1. 在OAL的OALIntrInit()函数中，设置某一GPIO为输出口，并使其输出高电平；
2. 在ISR的OEMInterruptHandler()函数中，将该GPIO设置为低电平；
3. 在IST的WaitForSingleObject()函数之后，将该GPIO设置为高电平；
4. 当外部中断触发时，GPIO会先被ISR拉低，再被IST拉高；
5. t1是外部中断下降沿触发以后，GPIO保持高电平的时间；
6. t2是GPIO保持低电平的时间；
7. t1+t2≈中断延迟时间
      实际应用的目标平台采用的MCU是S3C2410，其设置GPIO的主要代码如下。
//初始化GPIO，并设置为高
//设置GPE11为输出口
value = INREG32(&pOalPortRegs->GPECON);
OUTREG32(&pOalPortRegs->GPECON, (value & ~(3 << 22))|(1 << 22));
//设置GPE11的IO状态为高
value = INREG32(&pOalPortRegs->GPEDAT);
OUTREG32(&pOalPortRegs->GPEDAT, (value |(1 << 11)));
在ISR中设置GPIO为低的代码如下。
//将GPE11置低
mask = INREG32(&g_pPortRegs->GPEDAT);
OUTREG32(&g_pPortRegs->GPEDAT, (mask & ~(1<<11)));
驱动中的IST代码如下。
DWORD WINAPI IntrThreadProc(LPVOID ptr)
{
     while(bRun)
     {
           WaitForSingleObject(gIntrEvent, INFINITE);
           //将GPE11置高
          g_pIOPregs->GPEDAT |= (0x1 << 11);
          //RETAILMSG(1,(TEXT(“\r\nThread Run.”)));
          //Sleep(1000);
          InterruptDone(gSysIntr);
       }
       return 0;
}
      实现以上代码后，重新编译，并将操作系统下载到目标平台上。使用双通道的示波器测量中断触发引脚和GPIO的输出引脚，其中CH1测量GPIO的输出引脚，CH2测量外部中断引脚。
      在使用下降沿触发中断时，中断信号和GPIO的输出信号如图-3所示。
![clip_image005](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WindowsEmbeddedCE6.0_1155E/clip_image005_thumb.jpg)
                              图-3 下降沿触发时的实际波形
      从上图可以看出，t1基本可以忽略不计，t2约为1.8ms,t1+t2不到2ms，说明此时的中断延迟小于2ms。
在使用高电平中断，且IST中有输出调试信息时的波形如图-4所示。
![clip_image007](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WindowsEmbeddedCE6.0_1155E/clip_image007_thumb.jpg)
                    图-4 高电平触发，IST中有延时的实际波形
      从上图可以看出，t2的时间约为1.8ms，两次中断的时间间隔约为3.8ms。中断延迟小于2ms。
在使用高电平中断，且IST中没有输出调试信息时的波形如图-5所示。
![clip_image009](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WindowsEmbeddedCE6.0_1155E/clip_image009_thumb.jpg)
                   图-5 高电平触发，IST中无延时的实际波形
     从上图可以看出，t2的时间大约为18μ秒，t1可以认为小于4μ秒，两次中断之间的时间间隔为22μ秒，且能够连续稳定的正确响应。这说明Windows Embedded CE 6.0内核的中断延迟可达18μ秒。当然，我们很难根据一次测量的值来判定它的最低响应时限为多少，但微秒的数量级足以说明Windows Embedded CE 6.0的实时性基本可以满足常见的各种应用。
     需要注意的是，应用系统的实时性并不完全由操作系统的实时性决定，系统的硬件结构、软件驱动和应用程序等都会影响到整个系统的实时性。另外，Windows Embedded CE 6.0虽然是实时操作系统，但它也提供了非实时的工作环境，如果定制系统时添加了非实时的相关组件，也会影响系统的实时性。
Platform Builder 6.0提供的Remote Kernel Tracker工具，可以帮助我们观察系统线程的执行情况。图-6和图-7所示是采用高电平触发，有1秒Sleep的状态截图，第一个框表示线程执行到Sleep(1000)，第二个框表示等待事件被触发，红色的虚线表示线程处于阻塞的状态。
![clip_image011](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WindowsEmbeddedCE6.0_1155E/clip_image011_thumb.jpg)
              图-6 高电平触发，IST中有延时的状态，Sleep(1000)
![clip_image013](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WindowsEmbeddedCE6.0_1155E/clip_image013_thumb.jpg)
                   图-7 高电平触发，IST中有延时的状态，等待事件的触发
      以上通过测量中断延迟时间，验证了Windows Embedded CE 6.0的实时性。当然，实时操作系统的定义严格，判断一个操作系统是否为实时操作系统，还有其他一些因素，如系统的中断管理机制、线程管理机制、同步对象的支持、内存的管理等等。
      实际开发过程中，如何提高系统的实时性呢？在硬件平台、操作系统确定的前提下，我们还可以做哪些工作？通过前面的分析和实际测量，可以知道ISR的中断延迟时间很短，也很难再控制。IST的延迟受系统线程调度的影响，有进一步控制的可能。
      Windows Embedded CE 6.0使用基于优先级的时间片轮转算法对线程进行调度。对处于就绪态队列的线程，先分给优先级最高的线程以时间片，当时间片用完后，无论线程是否执行完毕，都退回就绪态队列中，等待下一次的时间片分配。
      所以，一方面，我们可以通过提高中断线程的优先级以获得更多的执行机会。Windows Embedded CE 6.0的线程有256个优先级(0~255)，0的优先级最高，255的优先级最低。线程默认的优先级为251。WinCE中用来设置线程优先级的函数有两个。
CeSetThreadPriority(HANDLE hThread,int nPriority)：可以设置0~255的所有优先级
SetThreadPriority(HANDLE hThread,int nPriority)：只能设置8个最低的优先级(248~255)
      为了减小中断处理延迟，应使用CeSetThreadPriority()设置更高的线程优先级。函数SetThreadPriority()通常在应用程序中用来设置工作线程的优先级。
      另一方面，我们可以修改线程的时间片大小（线程额度），线程额度只会影响同一优先级的线程调度。系统默认的线程额度是100ms。线程额度被设为0意味着线程将一直执行，直到被阻塞或者被更高优先级的打断。修改线程额度有两种方法，可以在OAL的OEMInit()函数中设置dwDefaultThreadQuantum，这将修改系统默认的线程额度。也可以使用设置线程额度的函数CeSetThreadQuantum(HANDLE hThread,DWORD dwTime)来修改某一特定线程的线程额度。为了保证系统的实时性，dwDefaultThreadQuantum应该设定为一个合适的值。对实时性要求较高的中断处理线程，可以单独将它的线程额度设为0。
      以上两种方法是比较方便的处理方法，如果采用了这两种方法后，系统的实时性仍然不能满足需求，那么需要考虑其他的解决办法，主要有以下几个：
- 将中断处理的主要部分放到ISR中，这是一种困难而又危险的做法，困难是因为ISR中能调用的系统函数有限，危险意味着处理不当会导致系统性能急剧下降。
- 改善外部中断的触发机制，从硬件着手，如增加硬件缓冲区，减少单位时间内的中断响应次数，在IST中处理硬件缓冲区的数据，这种方法能极大提高系统的实时性。
- 采用更高主频的MCU，提高总线访问速度。
      总之，实现一个实时系统，需要从硬件、操作系统、驱动和应用程序等各方面考虑，根据应用的需求选择合适的硬件、操作系统，在软件设计时也需要时刻考虑到实时性的要求。

# 《Cortex-M3权威指南》——概览 - 一世豁然的专栏 - CSDN博客





2015年05月02日 12:57:09[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1810








一、简介

  1、Cortex-M3是一个32位处理器内核。内部的数据内径是32位的，寄存器是32位的，寄存器接口也是32位的。指令总线和寄存器总线共享同一个寄存器内存。




  2、比较复杂的应用可能需要更多的存储系统功能，为此Cortex-M3提供了一个可选的MPU，而且在需要的情况下也可以使用外部的cache。另外Cortex-M3支持both的大端模式和

        小端模式。




  3、Cortex-M3内部还附赠了需要调试组件，用于在硬件水平上支持调试操作。另外，为支持更高级的调试，还有且他可选组件，包括指令跟踪和多种类型的调试接口。




  4、Cortex-M3的简化视图

![](https://img-blog.csdn.net/20150501142446084?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







二、寄存器组

  1、Cortex-M3处理器拥有R1-R5个寄存器组。其中R13作为堆栈指针，SP。堆栈指针SP有两个，但在同一时刻只能有一个可以看到，这就是所谓的banked寄存器。

![](https://img-blog.csdn.net/20150501143110357?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  2、R0-R12：通用寄存器

   1）、R0-R12都是32位通用寄存器，用于数据操作。

   2）、绝大多数Thumb指令只能访问R0-R7，二32位的Thumb寄存器可以访问所有寄存器。




  3、Banked R13：两个堆栈指针

   1）、主堆栈指针（MSP）：复位后缺省使用的堆栈指针，用于操作系统内核以及异常处理例程。

   2）、进程堆栈指针（PSP）：由用户的应用程序代码使用。

   3）、堆栈指针的最低两位永远是0，这意味着堆栈指针总是4字节对齐的。




  4、R14：链接寄存器

   1）、当呼叫一个子程序时，由R14存储访问地址。




  5、R15：程序计数寄存器

   1）、指向当前的程序地址。




  6、特殊功能寄存器

   1）、Cortex-M3在内核水平上搭载了若干个特殊功能寄存器，它们分别是：

    I、程序状态子寄存器组（PSRs）。

    II、中断屏蔽寄存器组（PRIMASK、FAULTMASK、BASEPRI）。

    III、控制寄存器（CONTROL）。

  2）、Cortex-M3中特殊功能寄存器集合

![](https://img-blog.csdn.net/20150501144639386?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   3）、特殊功能寄存器机器功能

![](https://img-blog.csdn.net/20150501144755873?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







三、操作模式和特权模式级别

  1、Cortex-M3处理器支持两种处理器的操作模式，还支持两级特权操作。

   1）、两种操作模式分别是handler模式和thread模式，引入这两种模式是为了区别普通应用程序的代码和异常服务程序的代码。

   2）、两种特权分级分别为特权集和用户集，这可以提供一个存储器访问的保护机制，使得普通的用户程序代码不能意外的甚至是恶意的执行涉及到要害的操作。




  2、Cortex-M3下的操作模式和用户级别

![](https://img-blog.csdn.net/20150501195013354?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  3、Cortex-M3合法的操作模式转换图

![](https://img-blog.csdn.net/20150501195327980?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   1）、在Cortex-M3运行主应用程序是，即可以使用特权集，也可以使用用户级，但是异常服务例程必须在特权集下执行。复位后，处理器默认进入线程模式，特权级访问。


   2）、从用户级到特权集的唯一途径是异常。




  4、通过应用特权级和用户级，就能够在硬件水平上限制某些不受信任的货还没有调试好的应用程序，不让它们随便的设置涉及要害的寄存器。进一步，如果配置了MPU，它

        还可以作为特权机制的补充——保护关键的存储区不被破坏，这些区域通常是操作系统的区域。







四、内建的嵌套中断向量控制器

  1、Cortex-M3在内核水平上搭载了一个嵌套中断向量控制器NVIC，NVIC提供如下功能。

   1）、可嵌套中断支持。

   2）、向量中断支持。

   3）、动态优先级调整支持。

   4）、中断延时大大缩短。

   5）、中断可屏蔽。







五、、存储器映射

  1、Cortex-M3存储器映射

![](https://img-blog.csdn.net/20150501201212978?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  2、Cortex-M3预先定义好了“粗线条的存储器映射，从而控制外设的工作。其内部还拥有一个总线基础设施，专用于优化对这种存储器结构的作用。在此之上，Cortex-M3这只

        还允许这些区域之间“越权使用”。处于最高地址的系统存储区域用于存放中断控制器、MPU以及各种调试组件。







六、总线接口

  1、Cortex-M3内部有若干个总线接口，以使Cortex-M3能同时取址和访问内存：

   1）、指令存储区域总线。

   2）、系统总线。

   3）、私有外设总线。




  2、指令存储区域总线：有两条代码存储区总线负责对代码存储区的访问，分别是I-Code总线和D-Code总线。前者用于取指，后者用于查表等操作。




  3、系统总线：用于访问内存和外设，覆盖的区域包括SRAM，片上外设，片外RAM，片外扩展设备以及系统存储区的部分空间。




  4、私有外设总线：负责一部分私有外设的访问，主要就是访问调试组件。







七、存储器保护单元（MPU）

  1、存储器保护单元可以对特权集访问和用户级访问分别施加不同的访问限制。当检测到犯规时，MPU就会产生一个fault异常，可以由fault的服务例程来分析该错误，并在可能

        时改正它。




  2、MPU有许多用法，最常见的就是由操作系统使用MPU，以使特权级代码的数据，包括操作系统的本身数据不被其它数据破坏。




  3、MPU会使嵌入式系统变得更加健壮，更加可靠。







八、指令集

  1、Cortex-M3只使用Thumb-2指令集，它允许32位指令和16位指令水乳交融，代码密度和处理器性能都考虑到了，且使用方便。




  2、混合使用ARM和Thumb代码段有时间和空间上的额外开销。

![](https://img-blog.csdn.net/20150502121623930?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  3、Cortex-M3使用Thumb指令集的优点：

   1）、消灭了状态切换的额外开销，节省了both执行时间和指令时间。

   2）、不再需要把源代码文件分成ARM编译的和按Thumb编译的，软甲开发的管理大大减负。

   3）、无须再反复的求证究竟在何种状态下程序的的执行最有效率。




  4、因为Cortex-M3使用Thumb-2指令集，旧的应用程序需要移植和重建。对于大多数C程序，只需简单的重新编译就能重建，汇编代码可能需要大面积的修改和重写。




  5、Cortex-M3并不支持所有的Thumb-2指令，Cortex-M3也没有实现SIMD指令集，不支持的指令还包括v6中引入的SETEND指令。







九、中断和异常

  1、Cortex-M3的所有中断机制都由NVIC实现。除了支持240条中断之外NVIC还支持16-4-1=11个内部异常源，可以实现fault管理机制。




  2、Cortex-M3有256个异常预定义的异常类型

![](https://img-blog.csdn.net/20150502123407072?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20150502123415824?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  3、Cortex-M3虽然支持240个外中断，但具体使用了多少个由芯片生产商决定.Cortex-M3还有一个NMI（不可屏蔽中断）输入脚。当它被置为有效（assert）时NMI服务例程会

        无条件执行。







十、调试支持

  1、Cortex-M3在内核水平上搭载了若干种调试相关的特性。最主要的就是程序执行控制，，包括停机（halting）、单步执行（stepping）、指令断点、数据观察点、寄存器和

         存储器访问、性能速写（profiling）以及跟中跟踪机制。




  2、Cortex-M3调试系统基于ARM最新的CoreSight架构。其内恶化本身不在含有JTAG接口，取而代之的是CPU提供的“调试访问接口（DAP）”的总线接口。通过这个接口，可

        以访问芯片的寄存器，也可以访问心痛存储器，甚至是在内核运行的时候访问。




  3、Cortex-M3还搭载所谓的“嵌入式跟踪宏单元（ETM）”和“指令跟踪单元（ITM）”。




  4、Cortex-M3的调试动作能由一系列的时间触发，包括断点、数据观察点、fault条件、或者是外部调试请求输入信号。




  5、所有的调试组件都可以由DAP总线接口来控制，Cortex-M3内恶化提供了DAP接口。此外运行中的程序也能控制它们。所有的跟踪信息都可以通过TPIU来访问得到。







十一、Cortex-M3的品行简评

  1、高性能。

  2、先进的中断处理功能。

  3、低功耗。

  4、系统特性。

  5、调试支持。





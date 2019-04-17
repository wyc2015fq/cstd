# 《Cortex-CM3权威指南》——Cortex-CM3基础 - 一世豁然的专栏 - CSDN博客





2015年05月04日 13:05:30[一世豁然](https://me.csdn.net/Explorer_day)阅读数：2259








一、寄存器组

 一）、通用寄存器


  1、Cortex-CM3拥有通用寄存器以及一些特殊功能寄存器。R0-R12是最通用目的的，但是绝大多数16位指令只能使用R0-R7，而Thumb-2则可以访问所有通用寄存器。特殊功能

        寄存器有预定义的功能，必须经过专门的经历来访问。

![](https://img-blog.csdn.net/20150504083729867?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





  2、通用目的寄存器R0-R7：R0-R7被称为低组寄存器。所有指令都能访问它们。它们的字长全是32位，复位后的初始值是不可预料的。




  3、通用目的寄存器R8-R12：R8-R12被称为高组寄存器。其只有很少的16位指令才能访问它们，32位指令则不受限制。它们的字长全是32位，复位后的初始值是不可预料

        的。




  4、堆栈指针R13

   1）、Cortex-CM3处理器内核有两个中断指针，也支持两个堆栈。当引用R13时，你引用到的是当前正在使用的哪一个，另一个必须用特殊的指令来访问。

   2）、主堆栈指针（MSP）或写作SP_main：这是缺省的堆栈指针，它由OS内核，异常服务例程以及所有需要特权访问的应用程序代码来使用。

   3）、进程堆栈指针（PSP或写作SP_process）：用于常规的应用程序代码（不处于异常服务例程中）。

   4）、注意并不是每个应用都必须用齐两个堆栈指针。简单的应用只是用MSP，而PUSH和POP指令默认使用SP。




  5、连接寄存器R14

   1）、R14是连接寄存器（LR）。在一个汇编程序中，你可以把它写作R14或LR，LR用于调用子程序时存储返回地址。




  6、程序计数器R15

   1）、R15是程序计数器，在汇编代码中可以使用PC来访问它。因为Cortex-CM3内部使用指令流水线，读PC时返回的值是当前指令的地址+4。







 二）、特殊功能寄存器

  1、Cortex-CM3中的特殊功能寄存器包括：

   1）、程序状态寄存器组（PSRs或xPSR）。

   2）、中断屏蔽寄存器组（PRIMASK，FAULTMASK，以及BASEPRI）。

   3）、控制寄存器（CONTROL）。

   注意：特殊功能寄存器只能被MSR和MRS指令访问，而且它们也没有存储器地址。




  2、程序状态寄存器组（PSRs或xPSR）

   1）、程序状态寄存器又被分为三个子状态寄存器

    I、应用程序PSR（APSR）。

    II、中断号PSR（IPSR）。

    III、执行PSR（EPSR）。

   2）、通过MRS/MSR指令这三个PSRs既可以单独访问，也可以组合访问。当使用三合一的方式访问时，应使用名字“xPSR”或“PSR”。




  3、图示

   1）、Cortex-CM3中的程序状态寄存器


![](https://img-blog.csdn.net/20150504090906630?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   2）、合体后的程序状态寄存器


![](https://img-blog.csdn.net/20150504090915460?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  4、中断屏蔽寄存器（PRIMASK，FAULTMASK，以及BASEPRI）

   1）、中断屏蔽寄存器用于控制异常的使能和除能。

   2）、Cortex-CM3屏蔽寄存器S

![](https://img-blog.csdn.net/20150504091427585?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


   3）、对于时间-关键人物而言，PRIMASK和BASEPRI对于暂时关闭中断是非常重要的。而FAULTMASK则可以被OS用于暂时关闭fault处理功能。




  5、控制寄存器（CONTROL）

   1）、控制寄存器用于控制特权级别，还用于选择当前使用那个堆栈指针。

![](https://img-blog.csdn.net/20150504092226129?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)










二、操作模式

  1、Cortex-CM3支持两个模式和两个特权等级：

![](https://img-blog.csdn.net/20150504115634951?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  2、当处理器处于线程状态下，即可以使用特权级，也可以使用用户级；另一方面，handler模式总是特权级。复位后，处理器进入线程模式+特权级。在线程模式+用户级下，

        对系统控制空间（SCS）的访问将被阻止。除此之外，还禁止使用MSR访问特殊功能寄存器——除了APSR例外。




  3、特权级的下的代码可以通过置位CONTROL[0]来进入用户级。但用户级不能通过修改CONTROL[0]来进入特权级。它必须通过一个异常handler，由那个异常handler来修改

        CONTROL[0]，才能放回线程模式后拿到特权级。

![](https://img-blog.csdn.net/20150504121118379?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  4、特权等级和堆栈指针的选择均由CONTROL负责。当CONTROL[0]=0时，在异常处理的始末，只发生了处理器模式的转换：

![](https://img-blog.csdn.net/20150504121410228?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

    但若CONTROL[0]=1（线程模式+用户级），则在中断响应的始末，both处理器模式和特权等级都要发生变化：

![](https://img-blog.csdn.net/20150504121517934?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


    CONTROL[0]只有在特权级下才能访问。








三、异常与中断

![](https://img-blog.csdn.net/20150504122136750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




四、向量表S

  1、当发生一个异常被Cortex-CM3内核接受，对应的handler就会执行。为了决定handler的入口地址，Cortex-CM3使用了“向量表查表机制”。




  2、向量表结构

![](https://img-blog.csdn.net/20150504122858552?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20150504122701168?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







五、栈内存操作

  1、在Cortex-CM3中，除了可以使用POP和PUSH来处理堆栈外，内核还会在异常处理的始末自动的执行PUSH和POP操作。




  2、笼统的讲，栈堆操作就是对内存的读写操作，但是其地址由SP给出。寄存器的数据通过PUSH操作存入堆栈，以后用POP操作从堆栈中取出。在POP和PUSH的操作

        中，SP的只会按堆栈的使用法则自动调整。




  3、堆栈的功能就是把寄存器的数据放入内存以便将来恢复。







六、Cortex-CM3堆栈的实现

  1、Cortex-CM3使用的是“向下生长满栈”模型。

   1）、PUSH操作：堆栈指针SP指向最后一个被压入堆栈的32位数值。在下一次压栈时，SP先减4，再存入新的数值。

![](https://img-blog.csdn.net/20150504124157998?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   2）、POP操作：先从SP指针处读出上一次被压入的值，再把SP指针自增4。

![](https://img-blog.csdn.net/20150504124356277?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







七、再论Cortex-CM3双堆栈机制

  1、Cortex-CM3堆栈有两个：主堆栈和进程堆栈，CONTROL[1]决定如何选择。




  2、当CONTROL[1]=1时，只使用MSP，此时用户程序和异常handler共享同一个堆栈。

![](https://img-blog.csdn.net/20150504124855096?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  3、当CONTROL[1]=0时，线程模式将不再使用PSP，而改用MSP（handler模式永远使用MSP）。

![](https://img-blog.csdn.net/20150504125158724?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  4、在特权模式下，可以指定具体的堆栈指针，而不受当前使用堆栈的限制。







八、复位序列

  1、在离开复位状态后，Cortex-CM3做的第一件事就是读取下列两个32位数值：

   1）、从地址0x00000000出取出MSP的初始值。

   2）、从地址0x00000004取出PC的初始值——这个值是复位向量，LSB必须为1。然后从这个值所对应的地址处取指。

![](https://img-blog.csdn.net/20150504130139944?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  2、在Cortex-CM3中，0地址处提供MSP的初始值，然后就是向量表。向量表中的数值是32位的地址，而不是跳转指令。向量表的第一个条目指向复位后应执行的第一条指

        令。








# 《Cortex-CM3权威指南》——异常 - 一世豁然的专栏 - CSDN博客





2015年05月31日 12:41:05[一世豁然](https://me.csdn.net/Explorer_day)阅读数：2679








一、异常类型

  1、Cortex-CM3在内核系统上搭载了一个异常响应系统，支持为数众多的系统异常和外部中断。其中，编号为1-15对应的是系统异常，大于等于16的则全是外部中断。除了个

        别异常的优先级被定死外，其他异常的优先级都是可以编程的。




  2、系统异常清单

![](https://img-blog.csdn.net/20150509134721064?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  3、外部中断清单

![](https://img-blog.csdn.net/20150509134619862?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  4、在NVIC的中断控制及状态寄存器中，有一个VECTACTIVE位段，另外，还有一个特殊功能寄存器IPSR。在他们二者里面，都记录了当前正在服务异常的编号。




  5、如果一个发生的异常不能被即刻相应，就称它被“悬起（pending）”。不过，少数fault异常是不允许被悬起的。对于每个异常源，在被悬起的情况下，都会有一个对应的“悬

        起状态寄存器”保存期异常请求，知道该异常能够被执行为止。







二、优先级的定义

  1、对Cortex-CM3来说，优先级对于异常是很关键的，它会影响一个异常是否会响应以及何时会响应。优先级的数值越小，则优先级越高。




  2、Cortex-CM3支持中断嵌套，使得高优先级异常可以抢占低优先级异常。有3个系统异常：复位，NMI以及硬fault，它们有固定的优先级，并且它们的优先级号是复数，从而

        高于所有其他异常。所有其他异常的优先级都是可以编程的（但不可编程为负数）。




  3、原则上Cortex-CM3支持3个固定的优先级和多大256个可编程的优先级，并且支持128级抢占，但实际上支持的优先级数会更少。它们在设计时会裁掉表达优先级的几个低

       端有效位，以达到减少优先数的目的。（优先级号是以MSB对齐的）

   1）、使用3位表达优先级的的情况

![](https://img-blog.csdn.net/20150514194107357?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   2）、如果使用更多的事表达优先级，则能够使用的值就越多，同时需要的门也越多，这会带来更多成本和消耗。Cortex-CM3允许的最少使用位数为3位。

   3）、3位表达的优先级和4位表达的优先级对比


![](https://img-blog.csdn.net/20150514194646334?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   4）、3位、5位、8位表达优先级时，优先级寄存器的使用情况

![](https://img-blog.csdn.net/20150514195032082?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  4、为了使抢占机能变得更可控，Cortex-CM3还把256级优先级按位分成高低两段，分别是抢占优先级和亚优先级。




  5、NVIC中的优先级组：该位段的值对每一个优先级可配置的异常都有影响，把其优先级分为两个位段：MSB所在的位段对应抢占优先级，二LSB所在的位段对应亚优先级。

![](https://img-blog.csdn.net/20150514195934991?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   1）、抢占优先级决定了抢占行为：当系统正在响应某异常L时，如果来了抢占级更高的异常H，则H可以抢占L。亚优先级则处理内务：当抢占优先级的异常有不止一个悬起

             时，就优先响应亚优先级最高的异常。

   2）、此优先级分组规定：亚优先级至少是一位，因此抢占优先级最多是7位，造成了最多只有128级抢占的现象。




  6、计算抢占优先级和亚优先级的有效位数时，必须先求出下列值：

   1）、芯片实际使用了多少位来表达优先级。

   2）、优先级组是如何划分的。




  7、虽然优先级分组的功能很强大，但是粗心的更改会使他变得很暴力，尤其是在设计硬实时系统的时候。在绝大多数情况下，优先级的分组都要预先经过计算论证，并且在开

        机初始化时一次性的设置好，以后也不再动用。







三、向量表

  1、当发生了异常并且要响应它时，Cortex-CM3需要定位其处理例程的入口地址。这些入口地址存储在所谓的“（异常）向量表”中。缺省情况下，Cortex-CM3认为该表位于零

        地址处，且各向量占用四字节。

![](https://img-blog.csdn.net/20150514202224611?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  2、为了动态重分发中断，Cortex-CM3允许向量表重定位——从其他地址处开始定位各异常向量。这些地址可以是代码区，也可以是RAM区。通过修改NVIC中的一个叫“向量

        表偏移量寄存器”中的值就能重定位向量表。但要注意：向量表的起始地址是有要求的，必须先求出系统中共有多少个向量，再把这个数字向上增加到是2的整次幂，而起始

       地址必须对齐到后者的边界上。

   1）、向量表偏移寄存器

![](https://img-blog.csdn.net/20150514203333743?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  3、如果需要动态的更改向量表，则对于任何器件来说，向量表的起始处都必须包含一下向量：

   1）、主堆栈指针（MSP）的初始值。

   2）、复位向量。

   3）、NMI。

   4）、硬fault服务例程。








四、中断输入及悬起行为

  1、中断输入及悬起行为也适用于NMI。只是NMI将会立即无条件执行，除了以下情况：

   1）、当前已经在执行NMI例程。

   2）、CPU被调试器喊停（halted）。

   3）、被一些严重的系统错误锁定

   则新的NMI请求也将被悬起。

![](https://img-blog.csdn.net/20150515184838744?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  2、当中断输入被assert后，该中断就被悬起。即使后来中断源取消了中断请求，已经被标记悬起的中断也会被记录下来。到了系统中，它的优先级最高的时候，就会得到响

        应。但是，如果在某个中断得到响应之前，其悬起状态被清除了，则中断被取消。

![](https://img-blog.csdn.net/20150515190004120?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  3、当某中断的服务例程开始执行时，就称此中断进入了活跃状态，并且其悬起位会被硬件自动清零。

![](https://img-blog.csdn.net/20150515190417694?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

     在一个中断活跃后，直到其服务例程执行完毕，并且返回。才能对该中断的新请求予以响应。




  4、如果中断源咬着请求信号不放，该中断就会在其上催服务例程返回后再次被设置为悬起状态。

![](https://img-blog.csdn.net/20150515190946344?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

     另一方面，如果某个中断在得到响应之前，其请求信号以若干的脉冲的方式呈现，则被视为只有一次中断请求，多出的中断脉冲全部错失——这是中断请求太快，以至于超

     出处理器反应限度的情况。

![](https://img-blog.csdn.net/20150515191401623?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  5、如果在服务例程中执行时，中断请求释放了，但是在服务例程返回前又重新被置为有效，则Cortex-CM3会记住此动作，重新悬起该中断。

![](https://img-blog.csdn.net/20150515191656655?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







五、Fualt异常类型

 一）、有若干个系统异常专用于fault处理。Cortex-CM3中的Fault可分为以下几类。

  1、总线faults。

  2、存储器管理faults。

  3、用法faults。

  4、硬faults。




 二）、总线Faults

  1、在AHB接口上正在传输数据时，如果回复了一个错误信号，则会产生总线fault，产生的场合是：

   1）、取指，通常被称作预流产（prefech  abort）。

   2）、数据读/写，通常被称作数据流产（data  abort）。




  2、在Cortex-CM3中执行以下动作可以触发总线异常：

   1）、中断处理起始阶段的堆栈PUSH动作。称为“入栈错误”。

   2）、中断处理收尾阶段的堆栈POOP动作。称为“出栈错误”。

   3）、在处理器启动中断处理序列后的向量读取时。这是一种罕见的特殊情况，被归类为营fault。




  3、当上述总线fault（取向量除外）发生时，只要没有同级或更高优先级的异常正在服务，且FAULTMASK=1时，就会执行总线的鼓舞例程。

   1）、如果在检测到总线fault时，还检测到了更高优先级的异常，则先处理后者，而总线fault则被标记为悬起。

   2）、如果总线fault被除能，或者总线fault发生时正在处理同级或更高优先级的异常，则总线fault被迫成为“硬伤”——上访成硬fault，使得最后执行的是硬fault例程。

   3）、如果在总线执行时又产生了总线fault，内核进入锁定状态。




  4、欲使能总线fault的服务例程需要在NVIC的“系统Hnadler控制及状态寄存器”中置位BUSFAULTENV位。要注意的是，在使能之前，总线fault服务例程的入口·地址必须已经在

        向量表中配置好。否则程序可能跑飞。




  5、如何找出fault事故原因：NVIC提供了若干个fault状态寄存器，其中一个名为“总线fault状态寄存器”。通过它，总线fault服务例程可以确定产生fault的场合：是在数据访问

       时，在取指时，还是在中断的堆栈操作时。

   1）、对于精确的总线fault，肇事的指令的地址被压在栈堆中。

   2）、如果不是精确的总线fault，则无法定位。

   3）、BFSR的程序员模型：

![](https://img-blog.csdn.net/20150517180536743?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




 三）、用法fault

  1、用法fault发生的场合是：

   1）、执行了未执行的指令。

   2）、执行了预处理器指令。

   3）、尝试进入ARN状态。

   4）、无效的中断返回。

   5）、使用多重加载/存储指令，地址没对齐。

   6）、除数为零。

   7）、任何未对齐的访问。




  2、用法fault说明

   1）、如果用法fault被使能，再发生用法fault时通常会执行其服务例程。

   2）、但是如果当时还发生了更高优先级的异常，则用法fault被悬起。

   3）、如果此时处理器正在处理同级或更高优先级异常，或者用法fault服务例程被使能，则上访成硬fault最终执行的是硬fault的服务例程。

   4）、如果硬fault服务例程或NMI服务例程的执行竟然导致了用法fault，内核又要被锁定。




  3、用法fault必须被使能才能正常响应。其在NVIC中的使能位是USGFAULTENV。如果把向量表置于RAM中，应优先建立好用法fault服务例程的入口地址。




  4、为了调查用法fault的的案发现场，NVIC中有一个“用法fault状态寄存器（UFSR）”，它指向导致用法fault的原因。在服务例程中，导致用法fault的指令被压入栈堆中。

   1）、UFSR占用2个字节可以被按半字或字访问。按字访问时的地址是0xE000_ED28,高半字有效；按半字访问时的地址是0xE000_ED2A。

   2）、和其他FAULT状态寄存器一样，里面的位可以通过写1来清零。

![](https://img-blog.csdn.net/20150531115532877?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







 四）、硬fault

  1、硬fault是总线fault、存储器管理fault，用法fault上访的结果。即这些fault例程如果无法执行，就会上访生硬fault。另外，在取向量时产生的总线fault也按硬fault处理。




  2、在NVIC中有一个为硬fault状态寄存器（HFSR），它指出产生硬fault的原因。如果不是由于取向量造成的，则硬fault服务例程必须检查其他的fault状态寄存器，以最终决定

        是谁上访。




  3、HFSR寄存器

![](https://img-blog.csdn.net/20150531120832246?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20150531120850850?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




 五）、应对fault

  1、在软件开发中，我们可以根据各种fault的值来判定程序错误，并且改正将其改正。但是在一个实时系统中情况就大不同——在找出fault的原因和，软件必须决定下一步该怎

        么做。




  2、应对fault的常用方法

   1）、复位。

   2）、恢复。

   3）、中止相关任务。




  3、各个fault状态寄存器（FSRs）都保持住它们的状态，知道手工清除。Fault服务例程在处理了相应的fault后不要忘记清除这些状态。FSRs采用一种写时清除机制（写1时清

        除）。







六、SVC和PendSV

 一）、SVC

  1、SVC称为系统服务调用或系统调用，用于产生系统函数的的调用请求。




  2、SVC作为操作系统函数门户示意图

![](https://img-blog.csdn.net/20150531123243970?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




 二）PendSV

  1、PendSV称为可悬起的系统调用，它与SVC协同使用。




  2、SVC异常是必须立即等到响应的，而PendSV不同，它可以像普通的中断被悬起。悬起PendSV的方法是：手工往NVIC的PendSV悬起寄存器中写1。





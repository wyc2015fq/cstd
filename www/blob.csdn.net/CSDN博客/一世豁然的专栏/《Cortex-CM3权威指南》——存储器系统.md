# 《Cortex-CM3权威指南》——存储器系统 - 一世豁然的专栏 - CSDN博客





2015年05月07日 09:27:11[一世豁然](https://me.csdn.net/Explorer_day)阅读数：2066








一、存储器系统的功能概览

  1、Cortex-CM3存储器系统功能

   1）、存储器映射是预定义的，并且还规定好了那个位置使用那条总线。

   2）、Cortex-CM3的存储器系统支持“位带”操作。

   3）、Cortex-CM3存储器系统支持非对齐访问和互斥访问。

   4）、Cortex-CM3的存储器系统支持both大端配置和小端配置。







二、存储器映射

  1、Cortex-CM3只有一个单一固定的存储器映射。这极大方便了软件在各种Cortex-CM3单片机间的移植。




  2、存储器的一些位置用于调试组件等私有外设，这个地址被称为“私有外设区”。私有外设区的组件包括：

   1）、闪存地址重载及断点单元（FPB）。

   2）、数据观察点单元（DWT）。

   3）、指令跟踪宏单元（ITM）。

   4）、嵌入式跟踪宏单元（ETM）。

   5）、跟踪端口接口单元（TPIU）。

   6）、ROM表。




  3、Cortex-CM3的地址空间是4GB，程序可以在代码区，内部SRAM区以及RAM区执行。4GB粗线条划分：

![](https://img-blog.csdn.net/20150506093327781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   1）、内部SRAM区的大小是512MB，用于让芯片制造商连接片上的SRAM，这个区通过系统总线来访问。在此区的下部，有一个1MB的位带区，该位带区还有一个对应的

             32MB的“位带别名区”，容纳了8M个“位变量”。位带区对应的是最低的1MB地址范围，而位带别名区里面的每个字对应位带区的一个比特。位带操作只适用于数据访问，

             不适用与取指操作。

   2）、地址空间另一个512范围由片上外设（的寄存器）使用。这个区也有一条32MB的位带别名，以便于快捷的访问外设寄存器。

   3）、还有两个1GB的范围，分别用于连接外部RAM和外部设备，它们之间没有位带。两者的区别在于外部RAM区允许执行指令，而外设设备区则不允许。

   4）、最后剩下0.5GB的地带是Cortex-CM3内核所在区域，包括系统级组件，内部私有外部总线S，外部私有外部总线S，以及由提供者定义的系统外设。

   5）、私有外部总线有两条

    I、AHB外设总线，只用于Cortex-CM3内部的AHB设备，它们是：NVIC,FPB，DWT和ITM。

    II、APB外设总线，即用于Cortex-CM3内部的APB设备，也用于外部设备

   6）、NVIC所处的区域叫做“系统控制空间（SCS）”在SCS里面还有SysTick、MPU以及代码调试控制所用的寄存器：

![](https://img-blog.csdn.net/20150506100015484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







三、存储器访问属性S

  1、Cortex-CM3为存储器做了映射之外，还为存储器的访问规定了4中属性：

   1）、可否缓冲（Bufferable）

   2）、可否缓存（Cacheable）

   3）、可否执行（Executable）

   4）、可否共享（Shareable）




  2、如果配置了MPU，则可以通过它配置不同的存储区，并且覆盖缺省的访问属性。







四、存储器的缺省访问许可

  1、Cortex-CM3有一个缺省的访问许可，它能防止使用户代码访问系统控制存储空间，保护NVIC,MPU等关键组件。缺省访问许可在以下条件时生效：

   1）、没有配置MPU。

   2）、配置了MPU，但是MOPU被除能。




  2、存储器的缺省访问许可

![](https://img-blog.csdn.net/20150506101303352?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




四、位带操作

 一）、简介

  1、支持位带操作后，可以使用普通的加载/存储指令来对单一的比特进行读写。在Cortex-CM3中，有两个区实现了位带。其中一个是SRAM区的最低1MB，第二个则是片内外

        设区的最低1MB范围。这两个区的地址除了可以像普通的RAM使用外，它也都有自己的“位带别名区”，位带别名区把每个比特膨胀成一个32位的字。

1）、位带区与别名区的膨胀对应关系图A


![](https://img-blog.csdn.net/20150506202525384?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   2）、位带区与别名区的膨胀对应关系图B

![](https://img-blog.csdn.net/20150506202715906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  2、Cortex-CM3用如下术语来表示位带存储的地址区

   1）、位带区：支持位带操作的地址区。

   2）、位带别名：对别名地址的访问最总作用到位带区的访问上。（注意：这中途有一个地址映射过程）




  3、在位带区中每个比特都都映射到别名地址区的一个字——这是只有LSB有效的字。当一个别名地址被访问时，会先把改地址变换成位带地址。

   1）、对于读操作，读取位带地址中的一个字，再把需要的位右移到LSB，并把LSB返回。

   2）、对于写操作，把需要写的位左移到对应的位序号处，然后执行一个原子的“读-改-写”过程。




  4、支持位带操作的两个内存区的范围是：

   1）、0x2000_0000-0x000F_FFFF（SRAM区中的最低1MB）和0x4000_0000-0x400F_FFFF（片上外设区中的最低1MB）。

   2）、对于SRAM位带区的某个比特，记它所在字节地址为A，位序号为n，则该比特在别名区的地址是：

![](https://img-blog.csdn.net/20150506204707955?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   3）、对于片上外设位带区的某个比特，记它所在字节地址为A，位序号为n，则该比特在别名区的地址是：

![](https://img-blog.csdn.net/20150506204804848?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  5、位带地址映射

   1）、SRAM区中的位带地址映射

![](https://img-blog.csdn.net/20150506205104732?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   2）、片上外设区中的位带地址映射

![](https://img-blog.csdn.net/20150506204948460?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







 二）、位带操作的优越性

   1、位带操作对硬件I/O密集型的底层程序提供了很大方便。



   2、位带操作可用来化简跳转的判断。是代码更整洁。




  3、在多任务中用于实现共享资源在任务间的“互锁”访问。




 三）、其他数据长度上的位带操作

  1、位带操作并不限于以字为单位的传送。亦可以按半子节和字节为单位传送。











五、在C语言中使用位带操作

  1、C编译器中并没有直接支持位带操作。欲在在C中使用位带操作，最简单的做法就是#define一个位带别名区的地址。为了简化位带操作，也可以定义一些宏。




  2、当使用位带功能时，要访问的变量必须用vollatile来定义。







六、非对齐数据传送

  1、Cortex-CM3支持在单一的访问中使用非（地址）对齐的传送，数据存储器的访问无需对齐。




  2、非对齐传送实例

![](https://img-blog.csdn.net/20150507084237355?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

  3、在Cortex-CM3中，非对齐的数据传送只发生在常规的数据传送指令中，其他的指令则不支持：

   1）、多个数据的加载存储（LDM/STM）。

   2）、栈堆操作（PUSH/POP）。

   3）、互斥访问（LDREX/STREX）。

   4）、位带操作。




  4、应该养成好习惯，总是保持地址对齐。为此可以变成NVIC，使之监督地址对齐。







七、互斥访问

  1、在Cortex-CM3中，用互斥体访问取代了ARM处理器中的SWP指令。




  2、互斥访问的理念同SWP非常相似，不同点在于：在互斥访问操作下，允许互斥体所在的总线被其他master访问，也允许被其他运行在本机上的任务访问，但是Cortex-CM3


        能驳回有可能导致竟态条件的互斥访问。




  3、互斥访问分为加载/存储，相应的指令为LDREX/STREX，LDREXH/STREXH，LDREXB/STREXB，分别对应于字/半字/字节




  4、使用方式（以LDREX/STREX为例）

   1）、语法格式

![](https://img-blog.csdn.net/20150507090035457?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   2）、LDREX指令与LDR相同。而STREX不同，STREX指令的执行是可以被驳回的。

    I、当处理器同意执行STREX，Rxf的值会被存储到（Rn+Offset）处，并且把Rd的值更新。

    II、若处理器驳回了STREX的执行，则不会发生存储动作，并且把Rd的值更新为1。

   3）、驳回规则：只有在LDREX执行后最近的一条STREX才能成功执行。其他情况下，驳回此STREX

    I、中途有其他STR指令执行。

    II、中途有其他的STREX执行。

   4）、当时用互斥访问时，LDREX/STREX指令必须成对使用。







八、端模式

  1、Cortex-CM3支持both小端模式和大端模式。在绝大多数情况下，Cortex-CM3都是用小端模式——为了避免不必要的麻烦，推荐使用小端模式。




  2、Cortex-CM3中对大端模式的定义与ARM7的不同（小端定义都相同）。在ARM7中大端模式称为“字不变大端”，而在Cortex-CM3中，使用的是“字节不变大端”。

   1）、Cortex-CM3的字节不变大端：存储器视图

![](https://img-blog.csdn.net/20150507091953934?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   2）、Cortex-CM3的字节不变大端：在ABH上的数据

![](https://img-blog.csdn.net/20150507092117738?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   3）、ARM7字节不变大端：在ABH上的数据

![](https://img-blog.csdn.net/20150507092111510?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  3、在Cortex-CM3,是在复位时确定使用那种端模式的，且运行时不得改变。指令预取永远使用小端模式，在配置控制存储空间的访问也永远小端模式（包括NVIX，FPB之

        流）。另外外部私有总线地址区0xE0000000至0xE00FFFFF也永远使用小端模式。





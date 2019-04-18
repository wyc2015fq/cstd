# 《Cortex-CM3权威指南》——实现Cortex-CM3的整体风景 - 一世豁然的专栏 - CSDN博客





2015年05月08日 15:18:25[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1336








一、流水线

  1、Cortex-CM3处理器使用一个3级流水线，流水线的3级分别是：取指，解码和执行：

![](https://img-blog.csdn.net/20150508080445588?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  2、当运行的指令大多数是16位时，处理器会每隔一个周期做一次取指。当执行到跳转指令时需要清洗流水线，处理器会不得不跳转目的地重新取指。为了改善这种情

        况，Cortex-CM3支持一定数量的v7M指令可以避免很多短程跳转。




  3、由于流水线的存在，以及出于对Thumb代码兼容的考虑，读取PC会返回当前指令地址+4的值。




  4、在处理器内核的预取单元中也有一个指令缓冲区，它允许后续的指令在执行前现在里面排队，也能在执行未对齐的32位指令时，避免流水线断流。







二、详细框图

  1、Cortex-CM3处理器里面除了处理核心外，还有其他好多组件，以用于系统管理和调试支持：

![](https://img-blog.csdn.net/20150508082249247?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   1）、MPUI和ETM是可选组件，不一定会包含在每一个Cortex-CM3的MCU中。

   2）、方框图中的缩写及定义

![](https://img-blog.csdn.net/20150508082347807?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20150508082600332?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  2、Cortex-CM3处理器是以一个处理器子系统呈现的，其CPU内核本身与NVIC和一系列调试块都紧密耦合：

   1）、CM3core：Cortex-CM3处理器的中央处理核心。

   2）、嵌套向量中断控制器NVIC：NVIC是一个在Cortex-CM3中内建的中断控制器。

   3）、SysTick定时器：系统滴答定时器是一个非常基本的到时器定时器，用于在每隔一定的时间产生一个中断，即使是系统在睡眠模式下也能工作。

   4）、存储器保护单元：MPU是一个选配的单元，有些Cortex-CM3可能没有配置此组件。如果有，则它可以把存储器分成一些regions，并分别给予保护。

   5）、BusMatrix：BusMatrix是Cortex-CM3内部总线系统的核心。

   6）、AHB to APB：它是一个总线桥，用于把若干个APB设备链接到Cortex-CM3处理器的私有外设总线上。

   7）、图框中其他的组件都用于调试，通常不会再应用程序中使用它们。




  3、其他调试组件

   1）、SW-DP/SWJ-DP：串行线调试端口（SW-DP）/串口线JTAG调试端口（SWJ-DP）都与AHB访问端口（AHB-PB）协同工作，以使外部调试器可以发起AHB上的数据传

             送，从而执行调试活动。

   2）、AHB-AP：AHB访问端口通过少量的寄存器，提供了对全部Cortex-CM3存储器的访问机能。

   3）、嵌入式跟踪单元ETM：ETM用于实现实时指令跟踪，但它是一个选配件，所以不是所有的Cortex-CM3产品都具有实时指令跟踪能力。

   4）、数据观察点及跟踪单元：通过DWT，可以设置数据观察点。

   5）、指令跟踪单元ITM：软件可以通过该模块直接把消息送给TPIU；还可以让DWT匹配命中事件通过ITM产生数据跟踪包，并把它输出到一个数据跟踪流中。

   6）、跟踪端口的接口单元TPIU：TPIU用于跟外部的跟踪硬件交互。

   7）、FPB：FPB提供flash地址重载和断点功能。

   8）、ROM表：只是一个简单的查找表，提供了存储器映射信息，这些信息包括多种系统设备和调试组件。







三、Cortex-CM3的总线接口

  1、I-Code总线

   1）、I-Code总线是一条基于AHB-lite总线的32位总线，负责在0x0000_0000-0x1FFF_FFFF之间取指操作。

   2）、取指以字的长度执行，即使是对于16位指令也是如此.因此CPU内核可以一次取出两条16位指令。




  2、D-Code总线

   1）、D-Code总线是一条基于AHB-lite总线的32位总线，负责在0x0000_0000-0x1FFF_FFFF之间取指操作。

   2）、链接到D-Code总线上的任何设备都只需支持AHB-Lite的对齐访问，不需要支持非对齐访问。




  3、系统总线

  1）、系统总线是一条基于AHB-lite总线的32位总线，负责在0x2000_0000-0xDFFF-FFFF和0xE010_0000-0xFFFF_FFFF之间的所有数据传送。取指和数据访问都有。

   2）、所有的数据传输都是对齐的。




  4、外部私有外设总线

   1）、基于APBzing总线协议的32位总线。此总线负责0xE004_0000-0xE00F_FFFF之间的私有外设访问。

   2）、但是APB的一部分空间已经被TPIU、ETM以及ROM表用掉了，就只留下0xE004_2000-0xE00F_F000这个区间用于附加的（私有）外设。




  5、调试访问端口总线

   1）、调试访问端口总线接口是一条基于”增强型APB规格“的32位总线，它专用于挂接调试接口。













四、Cortex-CM3的其他接口

  1、除了总线接口之外，Cortex-CM3还有几个用于其它目的的接口，这些接口的信号都不大可能会引出到引脚上，而只用于连接SoC不同的部分，或者干脆就没有使用。




  2、杂项接口信号

![](https://img-blog.csdn.net/20150508091941594?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








五、外部私有外设总线

  1、Cortex-CM3处理器有一个外部私有外设总线（PPB）接口。外部PPB接口是基于高级外设总线（APB）协议构造的。用于非共享的系统设备。




  2、为了支持CoreSight设备，该接口又包含了称为“PADDR31”的信号，给出传送的发源地：

   1）、若该信号为0，则表示是运行在Cortex-CM3内部的软件产生的传送操作。

   2）、若为1，则表示是调试软件产生了传送操作。




  3、外部私有外设总线是专用的，不服务于普通的外设，这恶鬼规矩只能靠芯片设计者自觉遵守。




  4、外部PPB不支持非对齐访问。







六、典型的连接方式

  1、总线接口与其他设备和存储器的典型连接

![](https://img-blog.csdn.net/20150508145557143?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  2、代码存储区既可以被指令总线（I-CODE）访问，也可以被数据总线（D-CODE）访问，此时需要在中间插入一个总线开关，称为总线矩阵或者使用一个AHB总线复用器。

   1）、如果使用了总线矩阵，则闪存和附加的SRAM可以被both I-CODE和D-CODE访问。

   2）、当数据访问和指令访问同时尝试访问同一个区域时，可以赋予数据访问更高的优先级以提高性能。

   3）、通过AHB总线矩阵把取指和数据访问分开后，如果指令总线和数据总线在同一时刻访问不同的存储器设备，则两者可以并行不勃。但是若只使用了总线复用器则数据传

             送就不能同时发生。





  3、有些引脚比较多的接口会带外部总线接口（EMI）。在此情况下，需要一个外部存储器控制器。其他的AHB设备则可以简单的链接到系统总线上，而不需要额外的总线矩

        阵。







七、复位信号

  1、Cortex-CM3中各种复位信号

![](https://img-blog.csdn.net/20150508151608392?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  2、典型的Cortex-CM3芯片内部复位信号和其作用范围示意图

![](https://img-blog.csdn.net/20150508151757249?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





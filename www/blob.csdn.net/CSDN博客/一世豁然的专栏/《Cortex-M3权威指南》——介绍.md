# 《Cortex-M3权威指南》——介绍 - 一世豁然的专栏 - CSDN博客





2015年05月01日 04:16:24[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1636








一、ARM Cortex-M3处理器初探

  1、Cortex-M3的特色

   1）、性能强劲。

   2）、功耗低。

   3）、实时性好。

   4）、代码密度得到很大改善。

   5）、使用方便。

   6）、低成本的整体解决方案。

   7）、遍地开发的优秀编程工具。




二、Cortex-M3处理器内核 VS 基于Cortex-M3 CPU

  1、Cortex-M3处理器内核是单片机的中央处理单元，完整的基于Cortex-M3的MCU还需要很多其他组件。




  2、ARM以及ARM架构的背景

![](https://img-blog.csdn.net/20150501033614216?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




三、ARM的给中架构版本

  1、ARMv7架构让内核架构从单一款式变成三种款式

   1）、款式A（ARMv7-A）：设计应用高性能的“开发应用平台”。

   2）、款式R（ARMv7-R）：用于高端的嵌入式系统，特别是带实时要求的系统。

   3）、款式M（ARMv7-M）：用于深度嵌入的，单片机风格的系统。




  2、处理器命名法

   1）、T代表Thumb指令集，D是说明支持JTAG调试，M意指快速乘法器，I则对应一个嵌入式ICE模块（默许命名）。新的后缀也在不断加入，包括定义存储器接口的、定义高

             速缓存的、定义紧耦合存储器。




   2）、ARM处理器名字![](https://img-blog.csdn.net/20150501035702236?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







四、指令系统的开发

  1、ARM一直支持两种形式上相对独立的指令集：

   1）、32位ARM指令集。对应处理器状态：ARM状态。

   2）、16位的Thumb指令集。对应处理器状态：Thumb状态。




  2、ARM指令集和Thumb指令集对应了两种处理器执行状态。在程序的执行过程的，处理器可以动态的在两种执行状态中转换。




  3、指令集的演进

![](https://img-blog.csdn.net/20150501040636332?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  4、Thumb-2指令集体系体系结构（ISA）

   1）、Thumb-2指令集和Thumb指令集的关系![](https://img-blog.csdn.net/20150501041018446?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)











五、Cortex-M3处理器的舞台

  1、低成本单片机。

  2、汽车电子。

  3、数据通信。

  4、工业控制。

  5、消费类产品。





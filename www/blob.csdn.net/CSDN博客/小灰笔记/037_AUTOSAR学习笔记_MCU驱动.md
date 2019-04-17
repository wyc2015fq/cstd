# 037_AUTOSAR学习笔记_MCU驱动 - 小灰笔记 - CSDN博客





2018年12月15日 12:36:00[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：163








![](https://img-blog.csdnimg.cn/20181215123538877.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         主要功能：MCU初始化、断电功能、复位、MCAL其他模块的特殊需求。

![](https://img-blog.csdnimg.cn/20181215123539131.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         几种模式：

         外部时钟的PLL时钟模式、晶振的PLL时钟模式、晶振参考下的PLL关闭模式。

![](https://img-blog.csdnimg.cn/2018121512353985.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

内部RC电路主要是用来监控晶振以及锁相环质量的。

![](https://img-blog.csdnimg.cn/20181215123539162.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181215123539159.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)
- DMA应该有多种优先级，这个优先级跟中断是否是一个概念？
- 从下文看，有一个嵌套的说法，看起来有可能跟中断时一个概念。这样，还是得弄明白中断嵌套的用法。

![](https://img-blog.csdnimg.cn/20181215123539117.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         之前见过，这个描述方式是一种什么描述方式？应该如何解读？

![](https://img-blog.csdnimg.cn/20181215123539110.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

PB的概念原来是Post Build。

![](https://img-blog.csdnimg.cn/20181215123539120.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         容器就是参数配置页面的意思？




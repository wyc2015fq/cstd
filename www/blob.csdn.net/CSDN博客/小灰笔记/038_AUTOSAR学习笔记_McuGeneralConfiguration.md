# 038_AUTOSAR学习笔记_McuGeneralConfiguration - 小灰笔记 - CSDN博客





2018年12月15日 12:15:16[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：90








         首先需要知道的是McuGeneralConfiguration在AUTOSAR的概念中是一个容器，关于容器目前我自己的理解很可能是自己定义的一种数据结构。在EB的配置工具中，具体信息如下：

![](https://img-blog.csdnimg.cn/20181215121457545.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         图中，几个可以选择可以配置的都是这个容器中包含的信息。接下来，针对每一个做一个理解。

**McuDevErrorDetec**

![](https://img-blog.csdnimg.cn/20181215121457551.png)

         结合上面的属性表，这个量本质上是一个宏编译开关，预处理或者预编译阶段起作用。关于它的部分属性配置如上表，类型是一个布尔类型，也就是说这个可以开或者关。

**McuGetRamStateApi**

![](https://img-blog.csdnimg.cn/20181215121457570.png)

         这个是获取Ram状态的结果，从配置属性上来说跟错误检测的配置十分相似。

**McuInitClock**

![](https://img-blog.csdnimg.cn/20181215121457575.png)

结合如下AUTOSAR的文档：

![](https://img-blog.csdnimg.cn/20181215121457758.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这是一个配置参数，配置为True则MCU的驱动负责时钟的初始化。否则的话，MCU的驱动不再负责时钟的初始化。

         如果禁用，使用场景一般是：存在一次性写入的时钟寄存器或者是BootLoader就绪。前者还好理解，后者是什么概念？为什么BootLoader就绪之后就不可以改变时钟或者不能够由MCU初始化时钟了呢？退一步考虑，这个参数是用于整个驱动配置的，配置完了就不变了，如何会出现可用或者不可用两种状态呢？或许，BootLoader阶段的时钟配置不使用MCU的驱动，应该独立配置？

**McuNoPll**

![](https://img-blog.csdnimg.cn/20181215121457635.png)

         没有PLL的芯片或者PLL不受软件控制的时候，这个参数应该设置为True。如下还有部分说明：

![](https://img-blog.csdnimg.cn/20181215121457731.png)

         一般来说，我现在遇到的应该全都是配置成False了。

**McuPerformResetApi**

![](https://img-blog.csdnimg.cn/20181215121457751.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这个应该是复位相关的操作。

**McuCalloutBeforePerformReset****、****McuPerformResetCallout**

![](https://img-blog.csdnimg.cn/20181215121457723.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

复位时候的回调函数配置，两个是有相互关系的，后面配置项是这个功能使能之后设置回调函数的名字。

**McuVersionInfoApi**

![](https://img-blog.csdnimg.cn/20181215121457716.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         获取MCU的版本信息的接口配置。

**McuConfigureFlash**

这个地方多少有点不解了，为什么MCU还有一个FLS的配置？这部分不该是存储组该有的功能吗？

**McuEnterLowPowerMode**

**      -**

**McueMiosConfigureGPRENApi**

         这部分的功能也有点疑惑，唯一能够沾上边的或许是GPT的配置，这部分为什么会划分到这里来？




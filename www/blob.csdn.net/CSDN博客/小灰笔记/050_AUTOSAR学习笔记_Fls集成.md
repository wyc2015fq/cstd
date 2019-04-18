# 050_AUTOSAR学习笔记_Fls集成 - 小灰笔记 - CSDN博客





2018年12月19日 23:56:02[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：103








![](https://img-blog.csdnimg.cn/20181219235539183.png)

         这部分是自动被调用还是需要自己写代码？

![](https://img-blog.csdnimg.cn/20181219235539179.png)

         看起来，需要先初始化MCU模块。这样，看起来是需要时钟等功能的初始化。尝试在我的代码中进行了初始化。

![](https://img-blog.csdnimg.cn/20181219235539246.png)

         有一个主程序。

![](https://img-blog.csdnimg.cn/20181219235539250.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         还有部分对看门狗、Fee的支持。

![](https://img-blog.csdnimg.cn/20181219235539273.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         不同的写入模式。

         其实，Fls的集成功能不是最终目的，最终目的是用来实现Fee。暂时的集成已经Ok，接下来尝试实现Fee做一个调试测试。




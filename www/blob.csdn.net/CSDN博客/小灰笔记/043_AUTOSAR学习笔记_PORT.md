# 043_AUTOSAR学习笔记_PORT - 小灰笔记 - CSDN博客





2018年12月15日 21:28:19[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：137








         之前已经总结过了，PORT的功能主要是配置MCU所有的端口。也就是说，引脚什么功能是在这个位置配置的。至于管脚功能配置好之后如何使用，就在其他诸如DIO之类的模块了。

         手头的这个Demo中，大致的配置如下：

![](https://img-blog.csdnimg.cn/20181215212804658.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这张表在其他的IDE中见过类似的，我自己试用过EB的工具但是花的时间很短，加上对AUTOSAR的了解不够没有找到类似的功能。其实，我比较想找到的就是这个表，这个算是最初做驱动时候一般做测试用的简单的LED等功能的测试。

         正好Demo中没有CAN的功能，我自己看看尝试做一个CAN PORT的配置。在开发板中，设计如下：

![](https://img-blog.csdnimg.cn/20181215212804780.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

相应的管脚信息：

![](https://img-blog.csdnimg.cn/20181215212804762.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

工具中配置如下：

![](https://img-blog.csdnimg.cn/20181215212804730.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181215212804756.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这样，CAN的功能配置似乎确实是简单了不少。不过，接下来还有很多诸如波特率之类的问题需要继续测试。后面，先尝试把配置出来的PORT模块集成一下。




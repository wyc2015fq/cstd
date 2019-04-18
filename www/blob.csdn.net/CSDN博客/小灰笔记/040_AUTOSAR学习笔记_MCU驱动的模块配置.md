# 040_AUTOSAR学习笔记_MCU驱动的模块配置 - 小灰笔记 - CSDN博客





2018年12月15日 14:26:03[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：171








![](https://img-blog.csdnimg.cn/20181215142545662.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181215142545500.png)

         看起来，这个功能应该直接禁用，而我手头的demo也是禁用的。

![](https://img-blog.csdnimg.cn/20181215142545700.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         或许，这个不是MCU可以提供的，而是MCU驱动可以提供的。

![](https://img-blog.csdnimg.cn/20181215142546155.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         RAM的分块主要是用于什么？或许是分为标定、程序等各种不同用途？

![](https://img-blog.csdnimg.cn/20181215142545770.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         看起来，这个也是一个未实现的功能。

![](https://img-blog.csdnimg.cn/20181215142545773.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这个应该是外面晶振的参数了，我的开发板确认下是8M，已经修改过了。

![](https://img-blog.csdnimg.cn/20181215142545566.png)

![](https://img-blog.csdnimg.cn/20181215142545778.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这个参数是用来保证不出现死循环的。

![](https://img-blog.csdnimg.cn/20181215142545546.png)

![](https://img-blog.csdnimg.cn/20181215142545558.png)

         这两个参数可以选择之后，可以配置开启两个中断。

![](https://img-blog.csdnimg.cn/20181215142545669.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         看到这里，我昨天的一个疑问算是解答了。这个优先级与中断并不是一回事儿。

![](https://img-blog.csdnimg.cn/20181215142545767.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         DEM的模块难度不知道什么程度，后续肯定避不过去的一个模块。

![](https://img-blog.csdnimg.cn/20181215142545570.png)

上面这个配置本来对应的是一个复位源，但是从代码中可以看到的是一个枚举类型。但是，复位源一直没找到。

![](https://img-blog.csdnimg.cn/20181215142545696.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)




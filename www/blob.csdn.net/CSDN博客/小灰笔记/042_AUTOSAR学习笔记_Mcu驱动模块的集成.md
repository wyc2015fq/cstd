# 042_AUTOSAR学习笔记_Mcu驱动模块的集成 - 小灰笔记 - CSDN博客





2018年12月15日 20:30:54[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：89








         简单看了一下用户手册，接下来直接尝试下这个模块的集成以及测试。

![](https://img-blog.csdnimg.cn/20181215203037196.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         上面是支持的芯片的列表，看起来这个芯片并不支持5634。

         Mcu_Init是一个需要调用的函数，但是从现在的生成文件来看这个代码没有，看起来这个应该是固定的程序代码，而AUTOSAR配置工具配置出来的应该只是配置数据。

![](https://img-blog.csdnimg.cn/20181215203037224.png)

         这个后面的参数本来应该是一个Mcu_ClockType类型的，这个类型本质上是一个uint8。

![](https://img-blog.csdnimg.cn/20181215203037226.png)

         这个参数，应该配置中的几套配置的序列号。在配置中应该如下：

![](https://img-blog.csdnimg.cn/20181215203037241.png)

![](https://img-blog.csdnimg.cn/20181215203037229.png)

         整个模块的集成看起来不难，但是配置可能比较费工夫。




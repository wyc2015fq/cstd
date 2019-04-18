# 044_AUTOSAR学习笔记_PORT设计 - 小灰笔记 - CSDN博客





2018年12月15日 22:31:10[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：116








![](https://img-blog.csdnimg.cn/2018121522305662.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

Port_Init初始化所有的端口引脚，在其他的端口调用之前调用。

![](https://img-blog.csdnimg.cn/2018121522305663.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         从这里看，或许引脚功能调整是一个比较常见的功能。

![](https://img-blog.csdnimg.cn/2018121522305670.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这个是端口方向的调整函数。

![](https://img-blog.csdnimg.cn/2018121522305663.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         设置不同的模式，应该有对CAN、GPIO等类似功能的选择。

         关于配置工具的几个没有操作过的内容，选择几个之前疑惑的做一下简单的小结。

![](https://img-blog.csdnimg.cn/2018121522305643.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这个是弱上拉或者弱下拉的一个配置。

![](https://img-blog.csdnimg.cn/2018121522305668.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这个是具体确认上拉还是下拉的一个配置项。

         以上，算是PORT中相对来说比较值得再注意一点的地方。看起来，整个PORT模块的功能并不是很难，实现起来应该比较简单。






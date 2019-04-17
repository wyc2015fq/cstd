# 046_AUTOSAR学习笔记_FLS - 小灰笔记 - CSDN博客





2018年12月17日 23:19:08[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：90
个人分类：[汽车电子](https://blog.csdn.net/grey_csdn/article/category/8518809)









         正好前段时间测试了一下Flash的驱动，看看这个顺便也测试学习下。

![](https://img-blog.csdnimg.cn/20181217231845885.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这里面提到的线性的概念应该是Fls的区块大小相等的意思，而Fls物理区块的名称中带有相应的系数以及偏移量的信息。

![](https://img-blog.csdnimg.cn/20181217231846526.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181217231846494.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         以上是FLS模块中可能出现的错误，分为同步、异步等不同模式中。基本上就是擦除失败、写入失败两种，只是针对于不同的场景。

![](https://img-blog.csdnimg.cn/20181217231845826.png)

         读、写 、擦除或者比较的取消。

![](https://img-blog.csdnimg.cn/20181217231846512.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         比较功能，比较的是FLS与存储Buffer 。

![](https://img-blog.csdnimg.cn/20181217231846507.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         没有看到调用的例子，这个目标地址是不是前面看到的分块，会定义成一系列的宏？查看了一下代码，其实是一些配置数据。具体的定义如下：

![](https://img-blog.csdnimg.cn/20181217231846451.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181217231846495.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         以上是初始化的函数。

![](https://img-blog.csdnimg.cn/20181217231846496.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         有一个周期性调度的Fls主函数。

![](https://img-blog.csdnimg.cn/20181217231846523.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这里的指针应该是回读的信息？小疑问：Flash的读取不是一个地址读写就OK了？

![](https://img-blog.csdnimg.cn/20181217231846495.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         Fls写入函数，似乎也没有特殊的地方，与我之前接触的驱动类似。

![](https://img-blog.csdnimg.cn/20181217231846503.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         Fls驱动的块配置其实也是有点图表化的，这里面的分区都是以下拉栏的方式提供的，配置起来应该不是很难。

         第一感觉比较有用的信息大致如上，比较粗浅，如果使用的话应该还有一个fee的驱动需要一块儿，后面还需要再看看。




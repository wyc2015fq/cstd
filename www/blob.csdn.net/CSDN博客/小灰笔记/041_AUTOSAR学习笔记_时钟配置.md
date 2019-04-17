# 041_AUTOSAR学习笔记_时钟配置 - 小灰笔记 - CSDN博客





2018年12月15日 16:51:35[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：94








          这部分专门拿出来总结一下，时钟的配置一直是我想要了解的一块儿。能够跳过去的内容直接跳过去，不做笔记，针对几个重要点仔细理解一下。

![](https://img-blog.csdnimg.cn/20181215165112579.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这里是系统时钟的频率，150主要是因为5644的时钟最高 150。之前，针对我自己的开发板改成了80M。不过，为了准确了解整个系统，再次调整回150。系统晶振改成12M。

![](https://img-blog.csdnimg.cn/20181215165112580.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         现在的系统时钟参数已经是150M，但是报错，主要原因是各种分频器、倍频器的寄存器配置出了问题。先顺着文档走，不行后面自己尝试按我自己的理解来调试。

![](https://img-blog.csdnimg.cn/20181215165112608.png)

         对于这个参数的理解：1个工程应该可以配置多个时钟配置出来，可以通过ID来选择具体让哪个配置生效。其实，ST的配置软件中这部分做的还是可以的。

![](https://img-blog.csdnimg.cn/20181215165112681.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这个配置现在的实现中直接没用，这个我也是有点好奇的。现在的方式是什么方式呢？从具体的选择来看，应该只有增强模式了。

![](https://img-blog.csdnimg.cn/20181215165112604.png)

![](https://img-blog.csdnimg.cn/20181215165112599.png)

         这个旁通的功能一直没有弄明白是什么功能，后期再做一个专门的测试理解。下面的参数，不知道是否跟旁通有关？

![](https://img-blog.csdnimg.cn/20181215165112671.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         感觉这个讲的应该是外设的参考时钟。

![](https://img-blog.csdnimg.cn/20181215165112660.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这三个是一个城际的关系，系统时钟目前写的是150M，但是还么有最终配置完成。

![](https://img-blog.csdnimg.cn/20181215165112620.png)

         做如下配置，参考还可以选择外部，晶振不也是外部输入信号吗？这两种描述方式有什么差异？难道是正弦波和矩形波之间的差异？

![](https://img-blog.csdnimg.cn/20181215165112658.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         如上，完成了系统时钟的配置。改完其他的冲突，之后编译通过，接下来或许可以简单测试下软件执行是否可行了。

         这一次的简单配置倒是没有之前那么难，比较顺利就完成了代码生成。




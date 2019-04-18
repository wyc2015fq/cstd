# powerpc_012_ADC功能实现与测试 - 小灰笔记 - CSDN博客





2018年09月24日 21:01:26[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：93
个人分类：[PowerPC](https://blog.csdn.net/grey_csdn/article/category/7114544)









       ADC的驱动实现，其实最麻烦的不在于硬件原理，而在于管脚与通道的映射关系。几种模式的ADC我觉得都是如此。即使是背后有一个再复杂的硬件实现，也基本不会妨碍我们进行功能代码的实现。

       我用到的开发板上做了两路输入可调的AD，我暂且只用其中的一路。相应的硬件资源使用的是PIN2，AN17。进行ADC功能配置的第一步，可以先检查并把pin脚功能配置出来。尤其是在管脚功能出现复用的情况下，这部分更应该先做一下核查。比如说，管脚116如果用作ADC的功能，这个就需要在软件层面上配置。但是，相比之下的PIN2是一个功能相对独立的管脚，就是ADC。这部分工作其实就可以省略掉了。

       以下是主要的配置：

![](https://img-blog.csdn.net/20180924210055422?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180924210055454?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180924210055400?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       参考芯片手册，采用了FIFO的时候，最大数值应该是0x3FFC。

![](https://img-blog.csdn.net/20180924210055557?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       程序读取结果的设计如下：

![](https://img-blog.csdn.net/20180924210055495?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       程序编译烧写测试：

![](https://img-blog.csdn.net/20180924210055474?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180924210055751?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180924210055856?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       以上是在可调范围内的最高、中间部分、最小的ADC采集结果，跟预期基本是一致的。




# powerpc_013_数字输入 - 小灰笔记 - CSDN博客





2018年09月26日 23:23:20[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：49
个人分类：[PowerPC](https://blog.csdn.net/grey_csdn/article/category/7114544)









       前面做的ADC模块算是驱动里面比较难处理的一个驱动了，但是这部分现在也已经做得代码生成非常成熟了。看起来，整个的基础驱动现在实现起来应该没有太大的难度了。再看一个简单的驱动——数字输入。

![](https://img-blog.csdn.net/20180926232304168?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       在我手头的开发板中，有几个开关量的输入。这里简单拿其中一个来做一下测试，选择S2作为测试。为了能够看得出效果，再增加一个LED与之关联。

       这个测试不是很成功，甚至多大多数时候检测不到低电平。我觉得可能是硬件的原因，毕竟个人设计的没有官方的开发板那么可靠。大致的测试代码可以展示如下：

![](https://img-blog.csdn.net/20180926232304178?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




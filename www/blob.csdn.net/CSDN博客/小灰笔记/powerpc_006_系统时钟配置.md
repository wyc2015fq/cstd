# powerpc_006_系统时钟配置 - 小灰笔记 - CSDN博客





2018年09月10日 00:45:26[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：106








       有一段小小的回忆值得说一下，关于系统时钟乃至锁相环的配置。这个应该是接触PowerPC嵌入式软件很早就该掌握的技能，但是我的这个技能迟到了很久。工作了好几年，都是站在别人的肩膀上，这让我总觉得脚底下空空的不安稳。也算是自己给了自己一点压力，开始了对底层更加深入的一点探索。

       最初，说到系统时钟或者锁相环的时候，总觉得有点茫然。不是没有去翻查芯片手册，而是看了之后根本没看懂。加上最初的学习纯粹是看手册，没有什么其他的实践或者去参考其他的资料，入手比较难。好在最终还是迈过了那道坎。

       其实，系统时钟的配置还是比较简单的。继续以我手中的MPC5634芯片为例进行这个简单的知识梳理。

       首先，先来个一针见血的结论：系统时钟的配置其实就是对一个公式的了解然后对可能的参数进行尝试。相应的软件实现依然会在我之前的软件工程上进行扩充，算是一个临时性的扩充，后期我可能会尝试配置工具生成的代码。

       芯片手册中找到的信息如下：

![](https://img-blog.csdn.net/20180910004509936?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       上面是传统模式，还有一个增强模式：

![](https://img-blog.csdn.net/20180910004509951?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       关于什么是传统模式什么是增强模式，需要在一个寄存器中进行配置选择：

![](https://img-blog.csdn.net/2018091000451036?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       这么看，默认情况下应该是传统模式。我的开发板外部晶振是8M的，这种模式下，尝试配置寄存器如下以达到芯片的最高主频80M：

![](https://img-blog.csdn.net/20180910004509922?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       上面代码中，第一行的主要作用还是选择一个参考时钟，至于模式等设置都是一些大致相同的东西。

       运行效果如下：

![](https://img-blog.csdn.net/2018091000451042?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



       其实，上面的结果只能够说明软件已经开始执行且没有飞。至于时钟是否正确是看不出来的，至少是不可以直观看出。后面，或许下一次的小结就是如何进行一个正确性的验证了。




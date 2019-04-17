# powerpc_008_任务调度的简单实现 - 小灰笔记 - CSDN博客





2018年09月24日 12:01:25[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：56








       汽车电子实际的工程运用中用到的时间片轮转模式的调度还是居多，而SPC5Studio中的一个便于OS移植的一个抽象层模块OSAL是可以提供部分模拟功能的。这是一个必选的模块，因此配置上的难度比较低，初始就基本配置好了。

       以下是初始的状态：

![](https://img-blog.csdn.net/2018092412010357?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       从上面的信息可以看出，系统时钟80M，OS的tick 1ms。此外，任务调度的中断优先级可以理解为4，按照我的理解其实这个可以设置为最低。

       先采用这种默认的方式来使用，如果满足不了需求再进行功能修改。

       如果没有提供这部分，通常我再实现的时候采用的是一个PIT的中断来实现。在MCU的手册中其实也有说到，推荐使用一个PIT来做OS的实现。其实，看一下这部分的代码就知道，这里提供的接口也是通过PIT来实现的。以下是部分实现的接口：

![](https://img-blog.csdn.net/2018092412010388?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       一个简单的使用场景：如何实现100ms的周期性任务调度呢？一个简单的实现例子如下：

![](https://img-blog.csdn.net/2018092412010377?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



运行效果：

![](https://img-blog.csdn.net/2018092412010380?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



       从上面的结果看，计数器实现了周期100ms的自增调度。




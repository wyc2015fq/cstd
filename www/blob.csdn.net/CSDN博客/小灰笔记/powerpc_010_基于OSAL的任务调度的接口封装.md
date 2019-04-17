# powerpc_010_基于OSAL的任务调度的接口封装 - 小灰笔记 - CSDN博客





2018年09月24日 14:27:14[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：41
个人分类：[PowerPC](https://blog.csdn.net/grey_csdn/article/category/7114544)









       习惯了自己做一个裸的PIT调度，这里通过已经生成的OSAL的调度做一个封装。提供10ms、20ms、100ms的任务调度。

       设计的大致思路，写一个PIT_10ms的fake interface，在里面实现任务调度的实现。而这个接口，在main函数中调用。封装完成之后，使用LED闪烁做一个测试。

       调度相关的关键调度点修改：

![](https://img-blog.csdn.net/20180924142656594?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       几个调度接口的实现：

![](https://img-blog.csdn.net/20180924142656603?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       LED闪烁的实现：

![](https://img-blog.csdn.net/20180924142656607?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       经过测试，相应功能与预期一致。

       值得一说的是，这个任务调度很多缺点，只是大致实现了一个裸程序。后面，可以针对这个方式略作修改，让这个功能再有进一步的完善。




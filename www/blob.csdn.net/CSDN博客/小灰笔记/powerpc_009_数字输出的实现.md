# powerpc_009_数字输出的实现 - 小灰笔记 - CSDN博客





2018年09月24日 13:00:19[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：39
个人分类：[PowerPC](https://blog.csdn.net/grey_csdn/article/category/7114544)









       程序设计中的第一步是实现一个hello world的输出，嵌入式类比的一个功能是LED的闪烁。

       LED闪烁实验，一般是在各种基于中断的任务调度完成之前。不过，之前我自己使用的一个更简单的方式作为初步，那就是查看变量的变化。至于LED灯的测试，暂且往后放了。在工作学习的经历中，我喜欢有多个LED灯的开发板，这样我能够直观看到很多我想看的状态。

       LED对应的驱动的功能是GPIO，确切说是GPDO。如果在现在使用的SDK中，各种配置简单到了只需要勾选相应功能即可。

       我手头的电路板，58脚连了一个LED，接下来尝试对此LED进行驱动配置。实现一个1s的翻转。

![](https://img-blog.csdn.net/20180924125958945?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       相应的管教配置映射到宏PIN_GPIO188_LED_D7。软件配置中会有如下增加：

![](https://img-blog.csdn.net/20180924125958951?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       控制代码如下：

![](https://img-blog.csdn.net/20180924125958929?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       编译通过后烧写到板子上，现在板子上有一颗LED等按照1s的时间间隔进行翻转，证明相应的驱动功能OK。




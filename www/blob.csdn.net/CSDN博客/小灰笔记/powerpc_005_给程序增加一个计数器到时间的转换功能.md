# powerpc_005_给程序增加一个计数器到时间的转换功能 - 小灰笔记 - CSDN博客





2018年08月30日 22:32:46[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：47
个人分类：[PowerPC](https://blog.csdn.net/grey_csdn/article/category/7114544)









            这次学习之路，计划一直在现有的一个工程上不断更新、不断增加新的功能知道这个工程的功能足够完备，具备一定的可用性。

            前面几次的小结都是围绕一个时间执行的机器周期来进行的，但是实际的工作中我们需要的往往不是计数器个数来衡量一段程序的执行时间，我们需要的可能是一个确切的时间概念。比如，程序执行了多少毫秒或者多少微秒。接下来，做一个小小的转换完成这个功能。

            实际的使用中，微秒通常是一个更好的衡量方式。这次，实现的任务就定义为从计数器到微秒的一个对等时间衡量的转换。如果要能够做到这个，必须知道一个系统时钟具体执行了多长时间。而这又是一个很少提及的概念。但是，另一个与之相关联的参数却经常被提起，那就是主频。有了主频，其实做一个倒数也就是系统时钟的时间了。由此，做如下的代码设计：

![](https://img-blog.csdn.net/20180830223222640?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

            上面的SYSTEM_CLOCK是一个宏，代表系统时钟，单位是HZ。由于现在还没有接触到锁相环等功能，暂且不好知道时钟主频是多少，这里随意定义一个数值，暂且写成16M。

![](https://img-blog.csdn.net/20180830223222637?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

            这样，结合之前的程序，再做一次执行时间的测量：

![](https://img-blog.csdn.net/20180830223222644?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

            这次的计算，把执行的系统时钟数转换成了时间。也就是，执行时间大约是6us。如果后期系统时钟继续往上调，相应的执行时间还会缩短。




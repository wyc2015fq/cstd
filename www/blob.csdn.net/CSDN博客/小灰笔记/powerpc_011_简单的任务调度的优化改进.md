# powerpc_011_简单的任务调度的优化改进 - 小灰笔记 - CSDN博客





2018年09月24日 14:41:38[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：96








       下面是之前实现的一个简单的任务调度的相关代码：

![](https://img-blog.csdn.net/20180924144118343?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       其实，简单的调度倒是不会有什么大的问题。不过，一个很大的问题点是10ms、20ms、100ms在执行的时间点上会有重合。如果能够做到更好的调度，应把它们拆开更加合理一点。这种拆分，依然使用10ms的时基进行拆分能够有一点改进，但是更好的方式应该是把时基拆分的更小，而每个任务调度的时间上限应该也增加相应的限制。这样，能够实现一个更加合理的死调度。

       接下来，做个简单的拆分：

![](https://img-blog.csdn.net/20180924144118352?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       这样的改进其实很小，但是已经把20ms和100ms的调度重合点给打散了。如果要进一步优化，除了上面说的继续减小时基之外，应该把类似20ms这样的任务拆成更多个，例如上面的例子中可以把每个case都拆分成一个20ms的task，而每个task都要增加相应的执行时间限制。这样，整个任务调用散开之后，程序能够充分利用每一个中断产生的时间片，增加CPU的利用率。

       其实，说到了这里，进一步的优化方案又可以初见端倪了。接下来可以尝试进一步的修改改进。




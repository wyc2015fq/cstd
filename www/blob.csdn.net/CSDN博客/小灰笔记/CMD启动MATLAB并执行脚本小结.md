# CMD启动MATLAB并执行脚本小结 - 小灰笔记 - CSDN博客





2017年01月04日 23:33:38[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：7034











关于这部分功能实现的掌握已经是前一段时间的事情了，今天再总结一下最近使用过程中的一点其他的收获。顺便再记录一个测试的例子，就当作知识的温习了。

1，在C:\Users\grey\workspace\MATLAB\test下放一个测试脚本my_test.m，简单实现每隔一秒钟输出一个数字的功能。输入命令启动并执行脚本：matlab -sd C:\Users\grey\workspace\MATLAB\test -r my_test




![](https://img-blog.csdn.net/20170104233356276?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







2，启动时的MATLAB启动界面




![](https://img-blog.csdn.net/20170104233425346?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







3，下面截图是启动成功并且脚本执行中的截图，这里是今天值得小结的一个地方：其实，脚本启动并执行的过程并不是我们通常理解的MATLAB启动就绪后开始执行脚本的过程。这个过程实际上是MATLAB启动时初始化的一个阶段。具体的信息提示从下面截图中勾画出来的提示就可以看出了，此时的状态并不是busy而是Initializing。其实，现在的这种状态意义的划分对我现在用到的功能或者特性来说都没有影响，这一点都不会妨碍我的使用。不过，能够了解的更加详尽一些也算是一种折腾的乐趣。探究这些东西有时候确实是没有太深的目的，我也只是为了好玩。


![](https://img-blog.csdn.net/20170104233458729?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






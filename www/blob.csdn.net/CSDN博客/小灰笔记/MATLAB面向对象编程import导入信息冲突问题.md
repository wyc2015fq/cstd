# MATLAB面向对象编程import导入信息冲突问题 - 小灰笔记 - CSDN博客





2017年05月18日 23:29:47[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：256








      MATLAB中使用import导入信息的时候有时候会遇到一点混乱现象。比如，当我们尝试导入的两个包中存在同名但是内容并不相同的类时，两者肯定是会产生冲突的。那么，导入的类到底以哪个稳准呢？

      在之前搭建的包测试环境上做一下测试如下：

![](https://img-blog.csdn.net/20170518232919969?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




      从上面的结果看，最终MyClassA是按照Pack1中的类来执行的。而Pack导入要在Pack2之前。由此可见，这种导入过程中并不会存在覆盖问题，而是先导入的起作用，后导入的被忽略掉。

      这确实是个比较有意思的现象，很多时候我们的惯性思维来构建软件运行模型的时候覆盖一般是在这种情况下的一种常见方式。倒是这种“先入为主”的运行方式比较少见。在实际的使用过程中，这确实是个值得注意的细节，一定得有充足的手段做一下相关的检测，否则很可能会引入难以预见的问题而导致Bug。




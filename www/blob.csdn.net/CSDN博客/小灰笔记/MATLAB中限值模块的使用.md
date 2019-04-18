# MATLAB中限值模块的使用 - 小灰笔记 - CSDN博客





2017年10月24日 18:55:01[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：402








            限值模块的功能很简单，把输入的信号输出限制在指定的范围内。但是有一个特殊的用法，那就是限值的最大值和最小值相同的时候。如果这样设置，那么输出将会是一个恒定值。

            简单的示例子如下：

![](https://img-blog.csdn.net/20171024185509647?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


            设置的参数如下：

![](https://img-blog.csdn.net/20171024185521681?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


            输出结果如下：

![](https://img-blog.csdn.net/20171024185530358?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

            如果设置修改，限值改成相同的数值0.7，输出的结果如下：

![](https://img-blog.csdn.net/20171024185539956?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

            其实，到这里我自己倒是有一个问题。为什么需要一个上下限值一样的设置呢？如果需要这样的功能，为什么不直接使用一个常量呢？这多少有点不可理解。如果，整体看来，这个模块在异常值的过滤上应该还是很有用的。




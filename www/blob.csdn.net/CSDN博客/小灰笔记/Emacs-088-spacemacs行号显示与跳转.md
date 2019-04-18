# Emacs-088-spacemacs行号显示与跳转 - 小灰笔记 - CSDN博客





2018年01月29日 22:17:34[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：637








Emacs-088-spacemacs行号显示与跳转

       这是一个令我兴奋的功能，曾有那么一瞬间，看着的它的时候我有点恼火。看懂之后，扔不掉了！

       使用了spacemacs之后，发现不能够进行行号显示。于是，尝试配置行号显示。增加了如下的配置：

![](https://img-blog.csdn.net/20180129221714609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       从上面看，这个配置生效前行号还是不能够显示的。之后，让这个配置生效。生效之后的效果如下：

![](https://img-blog.csdn.net/20180129221722863?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       说实话，第一次看到这个状态的时候，我心想：这TMD是个什么鬼玩意儿啊？行号怎么乱七八糟的？后来看出了其中的玄机，光标所在行显示出来了行号。往上往下的行号显示都是以光标所在行为起点的增量式增加，上下数字以此行为中心轴对称。

       看明白了就够了？不够！这样有什么好处呢？当然是跳转！如果没有这个功能，我想跳转到其他行的时候，只能是看一下行号直接跳转到行号，或者是一点一点地移动。但是，有了这个功能之后，我就可以执行如下的操作：

       10j：向下跳转10行

       7k：向上跳转7行

       这个功能自然可以在没有这种显示的时候工作，但是，有了这个显示功能，相应的操作可以变成一种更加自然的方式，因为偏移量已经十分清楚。

       太赞了！这个功能真的就是专门为vim这样的模式打造的！赞！




# Emacs-034-org mode - 小灰笔记 - CSDN博客





2018年01月08日 23:50:52[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：115








                
Emacs-034-org mode
这是我最近新接触的一个概念，似乎很多地方也把这个称之为大纲。如果是设置表述，应该叫做org-mode。Emacs对这种模式的识别是通过文件的后缀名来实现的，使用这种模式的时候，文件的后缀名需要是org。目前看应该是这样，至于是否还能够在不同后缀的文件中通过命令开启这个模式暂时还真不清楚。

如果开启了这个模式，会出现如下提示：

![](https://img-blog.csdn.net/20180108235143793?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


同时，从上面的截图中也能够看到很多org中的用法。其中，第一级标题是使用一个*符号来标识的，*后面应该有一个空格。二级标题需要两个**，如果有更多的分级，那么*个数依次增加。到了内容层级，使用的标注符号则成了-。值得注意的是，-后面也有一个空格才行。

当光标位于标题行的时候，按下Tab键可以对其下面的内容进行展开或者折叠，通过这种方式可以清爽的看清整个提纲的内容结构。折叠效果如下：

![](https://img-blog.csdn.net/20180108235157405?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



折叠后的内容隐藏掉，但是行号依然占用，因此能够看到行号不连续的状态。同时，左下角会有相应的动作提示。这是一个toggle命令，就像是一个开关，如果再按一次Tab键则会展开相应的内容，同时提示动作相应。具体如下：

![](https://img-blog.csdn.net/20180108235207448?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





# VLOOKUP返回N/A的一种原因 - lyx的专栏 - CSDN博客





2016年01月19日 16:16:50[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：6092








有时候会碰到VLOOKUP函数会返回N/A真心很蛋疼。查了函数参数，无误；查了查找的数据是否在查找队列中，无误。

下面是一个满足功能的情况。

![](https://img-blog.csdn.net/20160218145036783?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

D列是用VLOOKUP函数查找来的。这个时候是很好用的，我们交换一下A列和B列的位置，结果就出现了NA。![](https://img-blog.csdn.net/20160218145008971?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

在百度解决上诉问题的时候，脑海中凭借多年码农经验，考虑到，其实参数传的并不完整，没有指明在那一列中找数值，但是参数的说明就是这样的。后来偶然才发现，原来excel这个函数默认查找的数值就必须是lookup table的第一列。

希望大家以后可以不费这个时间去找答案了。




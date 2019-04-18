# eclipse创建单文件java工程 - 小灰笔记 - CSDN博客





2017年02月20日 00:27:28[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：474
个人分类：[Java](https://blog.csdn.net/grey_csdn/article/category/6673076)









终究还是使用了clips，不知道是否还有返回非IDE的那天。

尝试试了一下使用eclips创建简单的hello world式的工程并运行，还比较顺利实现了。中间的操作没有参考什么资料，纯粹是根据经验和感觉摸索。

1.创建一个工程，选择创建java工程

![](https://img-blog.csdn.net/20170220002921552?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





2.填写工程名称，其他的暗示使用默认。

![](https://img-blog.csdn.net/20170220003004015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





3.创建类，并写入类的名字。针对但文件，其实可以选择创建主函数，只是这个功能后来才看到。

![](https://img-blog.csdn.net/20170220003051882?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





4.类的基本框架会自动生成，接下来只需要在这个框架的基础上写代码。

![](https://img-blog.csdn.net/20170220003323635?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





5.补充代码

![](https://img-blog.csdn.net/20170220003542544?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





6.编译并且运行的方法似乎有好几个，随便选择一个自己喜欢的方式即可

![](https://img-blog.csdn.net/20170220003608888?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





7.运行成功，结果如下

![](https://img-blog.csdn.net/20170220003703206?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





刚刚接触，我还不是很清楚现在的Java开发主流模式是什么。到底是使用IDE还是自己搭建一个工程，使用make实现自动化的编译？说起来，自动化的编译调试应该是比较值得掌握的一个方式，不过，我自己目前只不过是针对简单的学习以及小地方的使用。简单的IDE应该足以满足我的需要，而目前最大的障碍还是语言基础知识的掌握。如此，暂时先使用eclips，make式的自动化实现还是留作以后的能力提升吧！




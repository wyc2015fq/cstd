# Java封装初探以及Mac下的eclipse操作实现 - 小灰笔记 - CSDN博客





2017年02月21日 00:47:26[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：229
个人分类：[Java](https://blog.csdn.net/grey_csdn/article/category/6673076)









虽说没有接触过面向对象的封装，但是在我工作的过程中C语言工程中倒是接受过相应的编码规范要求。那就是参数参数的传递或者修改要经过函数来实现，这样做的方式很大程度上是为了保证程序的可靠性。

Java中的封装是将类的某些信息隐藏在类的内部，不允许外部的程序直接访问。只能够通过该类提供的方法来实现对隐藏信息的操作或者访问。一般需要写相应的方法，在方法名上其实也没有什么限制。eclipse在这方面提供了不错的自动生成机制，而且有着统一的编码风格，难怪那么多开发Java的都是用eclipse的环境。




1.先写如下代码

![](https://img-blog.csdn.net/20170221004723644?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






2. 接下来利用eclipse的功能进行封装，需要注意的是，选择菜单的时候必须首先选中相应的源码文件。
![](https://img-blog.csdn.net/20170221004744744?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






3.弹出的对话框中可以选择对哪个量封装以及封装什么特性。

![](https://img-blog.csdn.net/20170221004830167?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







4.生成了统一风格的封装代码，其中涉及了this的使用。而this关键字的作用是代表当前对象。
![](https://img-blog.csdn.net/20170221004855589?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






5.增加使用封装功能的代码


![](https://img-blog.csdn.net/20170221004922272?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







6.程序运行结果


![](https://img-blog.csdn.net/20170221004943433?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)













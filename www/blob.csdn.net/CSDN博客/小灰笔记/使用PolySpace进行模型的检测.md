# 使用PolySpace进行模型的检测 - 小灰笔记 - CSDN博客





2017年08月07日 21:11:11[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：3243
个人分类：[MATLAB](https://blog.csdn.net/grey_csdn/article/category/6639461)









       使用PolySpace进行代码检测的方法比较基础，其实这个工具也可以用于模型的检测。道理也很容易讲得通，既然可以进行模型的代码生成，那么肯定也就可以对生成的代码进行检测。

       先进性代码生成，然后把代码拷贝出来，最后进行代码的分析。这是我以前的做法，倒是也没有太大的问题。不过，后来发现其实还可以更简单一点，那就是利用产品之间已经提供的接口。

       比如有如下的模型：

![](https://img-blog.csdn.net/20170807210950031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       进行代码生成后，从以下的地方启动检测的功能：

![](https://img-blog.csdn.net/20170807210959673?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       运行之后，模型的代码自动进行检测，如下：

![](https://img-blog.csdn.net/20170807211014790?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       整体的操作并没有太大的改进，但是却不用自己搭建测试环境。代码检测的一些详细的参数可以从前面的图中的那个option中进行调整。




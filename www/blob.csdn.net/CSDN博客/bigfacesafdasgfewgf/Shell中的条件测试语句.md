# Shell中的条件测试语句 - bigfacesafdasgfewgf - CSDN博客





2015年05月17日 21:56:22[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：1150标签：[shell																[条件测试																[test																[条件判断](https://so.csdn.net/so/search/s.do?q=条件判断&t=blog)
个人分类：[Shell](https://blog.csdn.net/puqutogether/article/category/3236387)








    Shell有条件测试语句，一般用test命令或是[]命令来完成，它们是条件判断语句if~then语句的基础，特别是[]命令。下面我们讲解一些条件测试语句。




# 1. test命令实现条件测试

    对于检测系统中某些文件是否存在，或者相关属性时，test命令很好用。

    其基本语法如下：

![](https://img-blog.csdn.net/20150517214713426?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


    test命令还可以测试字符串：

![](https://img-blog.csdn.net/20150517214941627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





    test命令还可以测试整数之间的关系：

![](https://img-blog.csdn.net/20150517215000257?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





# 2. []命令来实现条件测试

    使用中括号[]命令来实现条件测试功能时，要特别注意。因为中括号在很多地方都有用到，比如通配符和正则表达式。而在用[]表示条件测试时，我们必须遵循如下几点原则：

    1）在中括号[]内的每个组件都需要有空格键来分隔，例如：[□"$HOME"□==□"$MAIL"□]


    2）在中括号内的变量，最好都以双引号括号起来；

    3）在中括号内的常量，最啊哦都以单引号或是双引号括号起来。

    []命令来实现条件测试时，一般用于if~then的条件判断语句中。




参考：

《鸟哥的Linux的私房菜》](https://so.csdn.net/so/search/s.do?q=test&t=blog)](https://so.csdn.net/so/search/s.do?q=条件测试&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)





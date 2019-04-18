# 使用MATLAB加载文本数据 - 小灰笔记 - CSDN博客





2017年05月05日 08:26:28[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：821








体验了pandas数据加载的便捷性之后，突然间发现MATLAB原来也有类似的便捷功能。比如，文本文件data.txt中有如下数据：

![](https://img-blog.csdn.net/20170505082514873?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




加载操作如下：

>> A = load('data.txt')




A =




     1     5     3     6     2     4

     1     3     5     7     9     8

     3     1     4     1     5     9




>> A




A =




     1     5     3     6     2     4

     1     3     5     7     9     8

     3     1     4     1     5     9




最终的数据会背自动处理成一个矩阵。

再测试一下数据存在缺失的情况，假设数据如下：
![](https://img-blog.csdn.net/20170505082534962?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



数据加载操作结果如下：







>> A = load('data.txt');

Error using load

Number of columns on line 2 of ASCII file data.txt must be the same as

previous lines.




由此可见，这种加载的前提是原来的文本中数据必须是一个矩阵的模式。





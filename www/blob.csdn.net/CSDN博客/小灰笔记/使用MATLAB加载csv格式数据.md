# 使用MATLAB加载csv格式数据 - 小灰笔记 - CSDN博客





2017年05月05日 08:35:34[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：3372








类似于文本数据的加载，MATLAB支持直接加载csv格式的数据。假如有以下数据：

![](https://img-blog.csdn.net/20170505083446790?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







加载过程如下：




>> B = load('data.csv')




B =




     1     5     3     6     2     4

     3     1     4     1     5     9




>> B




B =




     1     5     3     6     2     4

     3     1     4     1     5     9

数据加载成功并且被处理成了向量格式。

再看一下数据非矩阵的情况，假如有以下数据：


![](https://img-blog.csdn.net/20170505083505810?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




操作过程如下：

>> B = load('data.csv')

Error using load

Number of columns on line 3 of ASCII file data.csv must be the same as

previous lines.




如此，可以得出与文本加载类似的结论，能够加载成功的前提是数据必须是矩阵格式。





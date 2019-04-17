# 014_bash中的for循环 - 小灰笔记 - CSDN博客





2018年11月08日 22:38:26[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：37
个人分类：[bash](https://blog.csdn.net/grey_csdn/article/category/7113865)









前面解决了let命令以及等效简写方式之后，可以尝试做个简单的小东西了，自然也是程序编写学习初步的一些东西。其实，最初尝试了一次失败了，主要还是不清楚bash的具体使用细节。

尝试求解了前100个正整数的和以及5的阶乘：

![](https://img-blog.csdnimg.cn/20181108223808359.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         执行结果如下：

![](https://img-blog.csdnimg.cn/20181108223808434.png)

         增加一种等效写法代码：

![](https://img-blog.csdnimg.cn/20181108223808500.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

执行结果：

![](https://img-blog.csdnimg.cn/20181108223808464.png)

         这样，简单的算术处理使用bash的话可以求解了。总体对比来看，或许是比不上Python等简单好用。




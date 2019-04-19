# initialization of 'XXX' is skipped by 'case' label 原因及解决办法 - xqhrs232的专栏 - CSDN博客
2015年02月09日 15:23:37[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：527
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://www.cnblogs.com/alexleo/archive/2010/11/19/2146999.html](http://www.cnblogs.com/alexleo/archive/2010/11/19/2146999.html)
今天遇到这个问题,在网上看了一会资料后找到原因,即:
switch 的 case 中不能定义变量,不然就会报错.可能是变量的初始化会因为有时候case条件不被执行而跳过.
后来想到三个解决的方法:
1：用if else 代替 switch 语句;
2：在case中用{}将代码括起来,这样在{}中就能定义变量了;
3：如果变量在各个case中都要用的话,就把变量定义在switch外面吧;

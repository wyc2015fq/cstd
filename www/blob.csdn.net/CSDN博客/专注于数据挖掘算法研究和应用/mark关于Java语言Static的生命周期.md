# mark关于Java语言Static的生命周期 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年02月18日 15:12:14[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1919
个人分类：[Java](https://blog.csdn.net/fjssharpsword/article/category/6480440)









1、Java客户端开发时用static类保存静态数据是常用做法，其生命周期和程序一样；

2、类源码移植到web开发时，static定义的类型就会出现问题，容易出现多用户共享数据，如静态队列定义，多用户共享容易出现错误；

3、这个逻辑错误，调试了很久才发现，包括各类循环结构的追踪，在web开发中要考虑每个连接用户数据的独立性，少设置静态关键字，如确实必要，需要对static定义的数据进行临界区设置，互斥性读写。

Mark：web开发注意数据在各用户间的独立性，避免交叉访问，不能定义静态公用数据。







# SQL SERVER读书笔记：nolock - 左直拳的马桶_日用桶 - CSDN博客
2013年09月11日 23:59:19[左直拳](https://me.csdn.net/leftfist)阅读数：1543
个人分类：[sql server](https://blog.csdn.net/leftfist/article/category/136470)
让查询语句不去申请共享锁，从而消除死锁，效果立竿见影。
缺点：
1、脏读
2、只能解决共享锁（S锁）参与的问题
3、需要修改语句本身才能使用

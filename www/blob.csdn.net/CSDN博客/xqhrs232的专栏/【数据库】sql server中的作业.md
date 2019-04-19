# 【数据库】sql server中的作业 - xqhrs232的专栏 - CSDN博客
2017年10月19日 15:45:07[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：232
原文地址::[http://blog.csdn.net/lifen0908/article/details/51864098](http://blog.csdn.net/lifen0908/article/details/51864098)
相关文章
1、SQL server 2012 如何创建定时作业----[https://jingyan.baidu.com/article/0bc808fc7ea4d71bd585b968.html](https://jingyan.baidu.com/article/0bc808fc7ea4d71bd585b968.html)
2、Sql Server如何新建作业----[https://jingyan.baidu.com/article/49ad8bce7287315834d8fab4.html](https://jingyan.baidu.com/article/49ad8bce7287315834d8fab4.html)
3、sql server如何使用作业----[https://jingyan.baidu.com/article/36d6ed1f56fffe1bcf4883bd.html](https://jingyan.baidu.com/article/36d6ed1f56fffe1bcf4883bd.html)
4、**SQL Server--作业(Job)浅析1----[http://57388.blog.51cto.com/47388/1846689/](http://57388.blog.51cto.com/47388/1846689/)**
**5、**[使用SQL
 Server 2005作业设置定时任务](http://www.cnblogs.com/IPrograming/archive/2012/03/08/SQLServer_Timer.html)----[http://www.cnblogs.com/IPrograming/archive/2012/03/08/SQLServer_Timer.html](http://www.cnblogs.com/IPrograming/archive/2012/03/08/SQLServer_Timer.html)
6、[使用SQL
 Server Management Studio 创建数据库备份作业](http://www.cnblogs.com/January/p/3811524.html)----[http://www.cnblogs.com/January/p/3811524.html](http://www.cnblogs.com/January/p/3811524.html)
7、[S](http://www.cnblogs.com/kerrycode/p/3279559.html)[QL
 SERVER 作业浅析](http://www.cnblogs.com/kerrycode/p/3279559.html)----[http://www.cnblogs.com/kerrycode/p/3279559.html](http://www.cnblogs.com/kerrycode/p/3279559.html)
本博客讲述关于数据库作业的基本知识。
# ** 哪里有作业？**
       如下图，在数据库的sql server代理中，我们可以看到作业，当我们启动代理服务的时候，作业也就可以启动了。
![](https://img-blog.csdn.net/20160708214548559)
# ** 什么情况下用作业？**
        一般的存储过程是在用户进行触发的时候执行的，但是假如用户一直不触发这个事件，但是却想执行，这时候就可以用作业了。他广泛的用在我们的数据库备份、定时定点的更新数据库表等操作。
# ** 怎么建立作业？**
       操作非常的简单。
        1、右击作业——新建作业——填写名称——常规中——填写名称（图一）
![](https://img-blog.csdn.net/20160708222221247)

           2、步骤——常规——新建作业步骤——填写步骤名称、数据库、运行的sql语句或者是存储过程等。这里用了一个插入的操作。当我们用存储过程来操作，需要传递参数的时候，一般都是通过系统session取出来和存储过程取出来的。
![](https://img-blog.csdn.net/20160708222053062)
             3、然后点击计划——出来下面的页面，我们就可以制定我们这个作业的计划啦。如下：
![](https://img-blog.csdn.net/20160708222114672)
                   警告、通知、目标等可以写，也可以不写，最后点击确定，我们的一个作业就指定出来啦。
             4、启动我们的作业的服务，然后我们就可以看到下面的效果啦。
![](https://img-blog.csdn.net/20160708223226109)
# **  最后：**
            这个例子比较简单，实际中，写的一般都是带有很多逻辑的存储过程，同时作业不存储在具体的用户数据库下面，而是存储在我们的系统库msdb中，所以里面有关于作业的详细记录，他比一般的表要大。我们在msdb中经常会用他的系统表来查询关于作业的信息，所以，这些系统表我们还是很有必要去看看的。


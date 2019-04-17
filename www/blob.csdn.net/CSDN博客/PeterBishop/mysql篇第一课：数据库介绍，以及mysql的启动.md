# mysql篇第一课：数据库介绍，以及mysql的启动 - PeterBishop - CSDN博客





2018年11月23日 10:16:42[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：11
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









  现在我们正式进入mysql数据库篇的课程



  首先一个很重要的概念: 什么是数据库？先来百度百科看一看:



数据库(Database)是按照[数据结构](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84/1450)来组织、[存储](https://baike.baidu.com/item/%E5%AD%98%E5%82%A8/1582924)和管理数据的仓库



  百度百科给出的定义非常的直接、简单，我最喜欢这样的定义，都不用我解释第二遍



  接下来，我们要学的是mysql数据库，那么来看看mysql的百度百科？

  No，先来了解一些数据库的相关概念:

DB: 数据库，狭义的指.db文件或其他数据库文件，广义的可以直接代指DBMS

DBMS: Database Management System,数据库管理系统，来看百度百科:

[数据库](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE%E5%BA%93/103728)管理[系统](https://baike.baidu.com/item/%E7%B3%BB%E7%BB%9F)(Database Management System)是一种操纵和管理数据库的大型软件，用于建立、使用和维护[数据库](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE%E5%BA%93/103728)，简称[DBMS](https://baike.baidu.com/item/DBMS)。

依然是非常的简单、直接

mysql就是一个DBMS



下面来看看mysql的百度百科:

MySQL是一个[关系型数据库管理系统](https://baike.baidu.com/item/%E5%85%B3%E7%B3%BB%E5%9E%8B%E6%95%B0%E6%8D%AE%E5%BA%93%E7%AE%A1%E7%90%86%E7%B3%BB%E7%BB%9F/696511)，由瑞典MySQL AB 公司开发，目前属于 [Oracle](https://baike.baidu.com/item/Oracle) 旗下产品。

依然简单，直接





那么问题来了，什么叫关系型数据库?



关系数据库，是建立在关系模型基础上的[数据库](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE%E5%BA%93/103728)，借助于集合代数等数学概念和方法来处理数据库中的[数据](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE/33305)。



放心，我知道你们可能有点看不懂了。

所谓的关系，其实就是一张表(table)，说白了，就是建立在表基础上的数据库就叫关系型数据库。



说了这么多，下面来看看mysql怎么启动(一般情况下是自启动的)

方式一：计算机——右击管理——服务

方式二：通过管理员身份运行 

net start 服务名（启动服务）

net stop 服务名（停止服务）



各位放心，上面这些我都没有用过，因为我的mysql就是开机自启动的，我也不会去闲的蛋疼停了它



OK，那接下来就是登录mysql了。

各位在安装mysql的过程中一定设置过登录要用的用户名和密码，我的用户名和密码都是root，我就演示下怎么登。

两种方式:
- 在开始菜单里找Mysql5.x Command Line Client

![](https://img-blog.csdnimg.cn/20181123101215571.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)



然后打开，输入密码



登录成功:

![](https://img-blog.csdnimg.cn/20181123101215527.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)


- 找到mysql.exe所在位置，然后通过cmd进入那个位置，然后用命令:

Mysql -u用户名 -p

然后输入密码即可

  默认安装的mysql.exe的位置在C:\Program Files\MySQL\MySQL Server 5.x\bin



退出用quit或者exit




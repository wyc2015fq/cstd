# SQL四种语言形式(DML、DDL、DCL、DQL)的概念与区别 - Machine Learning with Peppa - CSDN博客





2017年12月22日 23:28:46[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：432
所属专栏：[深入浅出数据库与Mysql](https://blog.csdn.net/column/details/18665.html)









## SQL语言共分为四大类：数据查询语言DQL，数据操纵语言DML，数据定义语言DDL，数据控制语言DCL。

![](https://timgsa.baidu.com/timg?image&quality=80&size=b9999_10000&sec=1512910093090&di=1ae3acf0ae4d551e592a2cfb92c6ab4f&imgtype=0&src=http%3A%2F%2Fitfish.net%2FHome%2FModules%2FImages%2Fitfish_36115_0.jpg)





## 四种语言的性质：

1.  DQL (Data Query Language)数据查询语言：

数据查询语言DQL基本结构是由SELECT子句，FROM子句，WHERE

子句组成的查询块：

SELECT <字段名表>

FROM <表或视图名>

WHERE <查询条件>

2. DML(Data Manipulation Language)数据操纵语言：


适用范围：对数据库中的数据进行一些简单操作，如insert,delete,update,select等.

数据操纵语言DML主要有三种形式：

1) 插入：INSERT

2) 更新：UPDATE

3) 删除：DELETE

3.DDL(Data Definition Language)数据定义语言：
适用范围：对数据库中的某些对象(例如，database,table)进行管理，Create,Alter和Drop.


数据定义语言DDL用来创建数据库中的各种对象—–表、视图、 索引、同义词、聚簇等如：

CREATE TABLE/VIEW/INDEX/SYN/CLUSTER

表 视图 索引 同义词 簇

DDL操作是隐性提交的！不能rollback

4 .数据控制语言DCL

数据控制语言DCL用来授予或回收访问数据库的某种特权，并控制数据库操纵事务发生的时间及效果，对数据库实行监视等。如：


1) GRANT：授权。


2) ROLLBACK [WORK] TO [SAVEPOINT]：回退到某一点。

回滚—ROLLBACK


回滚命令使数据库状态回到上次最后提交的状态。其格式为：

SQL>ROLLBACK;


3) COMMIT [WORK]：提交。


在数据库的插入、删除和修改操作时，只有当事务在提交到数据库时才算完成。在事务提交前，只有操作数据库的这个人才能有权看到所做的事情，别人只有在最后提交完成后才可以看到。

提交数据有三种类型：显式提交、隐式提交及自动提交。下面分别说明这三种类型。


(1) 显式提交

用COMMIT命令直接完成的提交为显式提交。其格式为：

SQL>COMMIT；


(2) 隐式提交

用SQL命令间接完成的提交为隐式提交。这些命令是：

ALTER，AUDIT，COMMENT，CONNECT，CREATE，DISCONNECT，DROP，

EXIT，GRANT，NOAUDIT，QUIT，REVOKE，RENAME。


(3) 自动提交

若把AUTOCOMMIT设置为ON，则在插入、修改、删除语句执行后，

系统将自动进行提交，这就是自动提交。其格式为：

SQL>SET AUTOCOMMIT ON；




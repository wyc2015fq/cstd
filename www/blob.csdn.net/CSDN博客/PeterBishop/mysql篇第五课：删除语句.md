# mysql篇第五课：删除语句 - PeterBishop - CSDN博客





2018年11月23日 13:43:17[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：27
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









  这节课我们来学删除语句



方式1：delete语句 

单表的删除： delete from 表名 【where 筛选条件】

多表的删除： delete 别名1，别名2 from 表1 别名1，表2 别名2 where 连接条件 and 筛选条件;

方式2：truncate语句

truncate table 表名



先将方式1：

  where讲解

分类：

一、条件表达式

         示例：salary>10000

         条件运算符：

         > < >= <= = != <>



Demo 01:



原有数据:

![](https://img-blog.csdnimg.cn/20181123132416458.png)



Sql:

delete from t_user where age > 20;



结果：



![](https://img-blog.csdnimg.cn/20181123132416649.png)



age为25的那一行被删掉了



Sql :

delete FROM t_user where age IS NULL;



结果:

![](https://img-blog.csdnimg.cn/20181123132416782.png)



age为null的两行都删掉了





二、逻辑表达式

示例：salary>10000 && salary<20000



逻辑运算符：



         and（&&）:两个条件如果同时成立，结果为true，否则为false

         or(||)：两个条件只要有一个成立，结果为true，否则为false

         not(!)：如果条件成立，则not后为false，否则为true



这个和java里的&& ，|| ，  !一样的



三、模糊查询

示例：last_name like 'a%'



模糊查询这个要说一下:



模糊查询用like

‘%a%’ 表示字段值里包含a的

‘a%’  表示以a开头的

‘%a’  表示以a结尾的



Demo 02:

原有数据:

![](https://img-blog.csdnimg.cn/20181123132416791.png)



Sql:

delete from t_user where name like '%a%';



结果:

![](https://img-blog.csdnimg.cn/20181123132416706.png)



name中包含a的行全部被删掉了.



多表的以后讲。



注意:

delete from 表名;

执行后会删除表中所有数据，但不会删掉表本身



truncate table 表名

这个也是删除表中所有数据



二者的区别:

1.truncate不能加where条件，而delete可以加where条件



2.truncate的效率高一丢丢



3.truncate 删除带自增长的列的表后，如果再插入数据，数据从1开始

delete 删除带自增长列的表后，如果再插入数据，数据从上一次的断点处开始



4.truncate删除不能回滚，delete删除可以回滚



对第3点的演示:

delete:

原来的:

![](https://img-blog.csdnimg.cn/20181123132416739.png)



Sql:

insert into t_user(name,age) values ('zhangsan',null);



结果:

![](https://img-blog.csdnimg.cn/20181123132416744.png)





truncate:

![](https://img-blog.csdnimg.cn/20181123132416825.png)



Sql:

insert into t_user(name,age) values ('zhangsan',null);



结果:

![](https://img-blog.csdnimg.cn/20181123132416899.png)



关于第四点，以后讲事务的时候讲。




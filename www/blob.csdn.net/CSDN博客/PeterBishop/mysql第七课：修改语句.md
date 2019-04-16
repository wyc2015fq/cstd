# mysql第七课：修改语句 - PeterBishop - CSDN博客





2018年11月23日 13:44:55[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：12








  这节课我们来学更新语句



单表:

update 表名 set 字段=新值,字段=新值

【where 条件】



多表:

update 表1 别名1,表2 别名2

set 字段=新值，字段=新值

where 连接条件

and 筛选条件



多表以后讲，暂时只讲单表:



Demo:

原来的数据:

![](https://img-blog.csdnimg.cn/20181123134439455.png)



Sql

update t_user set name = 'lisi', age = 18 where id = 1;



结果:

![](https://img-blog.csdnimg.cn/20181123134439407.png)










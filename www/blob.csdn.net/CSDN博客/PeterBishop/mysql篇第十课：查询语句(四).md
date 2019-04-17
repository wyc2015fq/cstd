# mysql篇第十课：查询语句(四) - PeterBishop - CSDN博客





2018年11月24日 10:00:39[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：24








  这节课继续学查询



### 分组查询

语法： select 查询的字段，分组函数 from 表 group by 分组的字段



注意:

1、可以按单个字段分组

2、和分组函数一同查询的字段最好是分组后的字段

3、分组筛选

针对的表位置关键字

分组前筛选：原始表              group by的前面               where

分组后筛选：分组后的结果集       group by的后面               having



4、可以按多个字段分组，字段之间用逗号隔开

5、可以支持排序

6、having后可以支持别名





Demo 01:

-- 根据部门分组

select * from tbl_employee GROUP BY d_id;

![](https://img-blog.csdnimg.cn/20181124095356660.png)





非常奇怪，为什么每组只有一条数据？

因为group by的机制就是每组只取一条数据



如果我希望所有数据都显示怎么办？



Demo 02：

-- 根据部门分组

select * from tbl_employee GROUP BY d_id,id;

![](https://img-blog.csdnimg.cn/20181124095405815.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)





为什么加上id之后就全部出来了呢？



因为根据id分组的话，id作为主键是不可重复的，所以所有数据都会显示



因此，如果分组后想取得所有数据而不是每组取一条数据的话，分组条件要加上主键或者其他不可重复的列



下面说说where和having的区别



Where是在分组前 对所有数据进行条件筛选

Having 是在分组后 对所有数据以组为单位进行筛选



Demo 03:

-- 根据部门分组，获取组内人数大于3的组内所有成员

select * from tbl_employee GROUP BY d_id, id having count(*) > 3;

![](https://img-blog.csdnimg.cn/20181124095414649.png)





为什么会这样呢？



因为我们是根据d_id和id分组，既然你根据id分组了那么每个组其实只有一行数据



那么怎么才能实现呢？ 要用子查询，这个以后讲。



Demo 04:

-- 查看每个部门的人数

select d_id as 部门, count(*) as 部门人数 from tbl_employee GROUP BY d_id;

![](https://img-blog.csdnimg.cn/20181124095423918.png)





Demo 05:

-- 查看部门人数大于3的每个部门的人数

select d_id as 部门, count(*) as 部门人数 from tbl_employee GROUP BY d_id having count(*) > 3;

![](https://img-blog.csdnimg.cn/20181124095431805.png)








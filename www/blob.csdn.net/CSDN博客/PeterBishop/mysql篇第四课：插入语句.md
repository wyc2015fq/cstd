# mysql篇第四课：插入语句 - PeterBishop - CSDN博客





2018年11月23日 13:23:02[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：12
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









  这节课讲mysql的插入语句，特别简单



  语法:

insert into 表名(字段名，...) values(值1，...);



先把我们要用的表准备好，表结构如下:

![](https://img-blog.csdnimg.cn/2018112313194815.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)



写sql也可以叫写查询语句，所以我们需要在navicat里新建查询来写sql

![](https://img-blog.csdnimg.cn/20181123131948416.png)

在查询那里点击右键选新建查询就可以写sql了



Demo 1:

![](https://img-blog.csdnimg.cn/20181123131948633.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)



![](https://img-blog.csdnimg.cn/20181123131948329.png)



可以看到已经插入进去了



接下来来详细介绍insert语句:



1、字段类型和值类型一致或兼容，而且一一对应

2、可以为空的字段，可以不用插入值，或用null填充

3、不可以为空的字段，必须插入值

4、字段个数和值的个数必须一致

5、字段可以省略，但默认所有字段，并且顺序和表中的存储顺序一致



第1条不用多说，这是自然的

关于第2条，我们来试试

t_user表里name字段不能为空，而age字段可以为空，我们来试试



Demo 02:

insert into t_user(id,name,age) values (1002,'zhangsan',null);



结果:

![](https://img-blog.csdnimg.cn/20181123131948362.png)



Demo 03:

insert into t_user (id,name) values (1003,'lisi');

![](https://img-blog.csdnimg.cn/20181123131948461.png)

可以为空的字段不必必须插入数据，默认为null



Demo 04:

insert into t_user (id,name,age) values (1003,null, 18);



结果:

[Err] 1048 - Column 'name' cannot be null

不能为空的字段不可以插入null



Demo 05:

insert into t_user (id,age) values (1005, 18);



结果:

[Err] 1364 - Field 'name' doesn't have a default value

不能为空的字段必须插入值且值不可以是null



 4、字段个数和值的个数必须一致

这个不用说了，个数不一致直接报错

[Err] 1136 - Column count doesn't match value count at row 1


- 字段可以省略，但默认所有字段，并且顺序和表中的存储顺序一致

这个很简单，演示一下:

Demo 06:

insert into t_user values (1005,'wangwu',18);



结果；

![](https://img-blog.csdnimg.cn/20181123131948393.png)



关于插入语句，还有一点，如果主键设置了自动递增，可以不用插入数据，即使它是not null的



t_user表的主键id是自动递增的:



Demo 07:

insert into t_user (name, age)values ('zhaoliu',25);

结果

![](https://img-blog.csdnimg.cn/20181123131948620.png)



注意，如果想要省略某一列不写，则不可以省略values前指定列名



Demo 07:

insert into t_user values ('erdan',12);



结果；

[Err] 1136 - Column count doesn't match value count at row 1



虽然主键是自动递增的，但如果你不想写主键则必须在values前指定要插入的列




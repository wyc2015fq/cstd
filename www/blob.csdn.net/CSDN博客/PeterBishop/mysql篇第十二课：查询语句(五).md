# mysql篇第十二课：查询语句(五) - PeterBishop - CSDN博客





2018年11月24日 10:05:05[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：9
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









  这节课我们来讲子查询:



  先来看我们的表和数据:



tbl_dept:

![](https://img-blog.csdnimg.cn/20181124100158165.png)



tbl_employee:

![](https://img-blog.csdnimg.cn/20181124100158381.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)



接下来看看子查询的定义:

一条查询语句中又嵌套了另一条完整的select语句，其中被嵌套的select语句，称为子查询或内查询

在外面的查询语句，称为主查询或外查询



特点:
- 子查询都放在小括号内

Demo 01:

-- 查询开发部的所有员工

select * from tbl_employee where d_id in (

-- 子查询放在括号里

    select id from tbl_dept where dept_name = '开发部'

);



结果:

![](https://img-blog.csdnimg.cn/20181124100158412.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)


- 子查询可以放在from后面、select后面、where后面、having后面，但一般放在条件的右侧

说明一下，子查询会生成一张虚表，虚表就是我们查出来的那个结果，这个结果并不是真实的表，只是一个数据而已，这份数据有和表类似的结构，所以叫虚表

虚表里的数据全部存在内存里，真实表的数据在磁盘上



3、子查询优先于主查询执行，主查询使用了子查询的执行结果(一张虚表)

4、子查询根据查询结果的行数不同分为以下两类：

① 单行子查询

         结果集只有一行

         一般搭配单行操作符使用：> < = <> >= <=

         非法使用子查询的情况：

         a、子查询的结果为一组值

         b、子查询的结果为空



② 多行子查询

         结果集有多行

         一般搭配多行操作符使用：any、all、in、not in

         in： 属于子查询结果中的任意一个就行

         any和all往往可以用其他查询代替



基本上只会用到单行子查询和in、not in



Demo 02：

-- 查询不是开发部的所有员工

select * from tbl_employee where d_id not in (

-- 子查询放在括号里

    select id from tbl_dept where dept_name = '开发部'

);



接下来看看 any和all是什么含义:

any关键词可以理解为对于子查询返回的列中的任一数值，如果比较结果为true，则返回true。

all 关键词可以理解为对于子查询返回的列中的所有数值，如果比较结果为true，则返回true。



Demo 03:

-- 查询开发部和宣传部的所有员工

select * from tbl_employee where d_id = any (

    select id from tbl_dept where dept_name = '开发部' or dept_name = '宣传部'

);



结果:

![](https://img-blog.csdnimg.cn/20181124100158456.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)



any和all用的不多，因为这两货基本可以被其他语法代替,如:



  Demo 04:

-- 查询开发部和宣传部的所有员工

select * from tbl_employee where d_id in (

    select id from tbl_dept where dept_name = '开发部' or dept_name = '宣传部'

);



= any 其实就是in

!= any 其实就是not in










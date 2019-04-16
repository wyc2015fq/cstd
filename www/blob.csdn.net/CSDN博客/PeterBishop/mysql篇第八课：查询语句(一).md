# mysql篇第八课：查询语句(一) - PeterBishop - CSDN博客





2018年11月23日 16:57:55[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：9








  OK,这节课开始我们学sql增删改查里最难的查



  这节课学 基础查询



  语法:

SELECT 要查询的东西 【FROM 表名】;



说明: 这个要查的东西可以是很多东西，最常见的是列



下面来说说我们查询部分要用的表，我已经准备好了:

![](https://img-blog.csdnimg.cn/2018112316501018.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)



表名叫tnl_emploee，也就是员工表



下面解释下各个字段:

id: 员工编号

last_name: 名字 0表示女性， 1 表示男性

email: 邮箱

dender: 性别

d_id: 所在部门id(部门表还没建，这个不用管)



下面来看Demo:



Demo 01:

select id,last_name from tbl_employee;



结果:

![](https://img-blog.csdnimg.cn/2018112316501083.png)



查询出了所有的id和last_name



接下来: 条件查询



就是在查询后面加where



where在删除语句那章已经讲了，我就直接拿过来了



分类：

一、条件表达式

         示例：salary>10000

         条件运算符：

         > < >= <= = != <>



Demo 02:



-- 查询员工编号大于10的员工的所有信息

select * from tbl_employee where id > 10;

结果:

![](https://img-blog.csdnimg.cn/20181123165010105.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)



* 表示查询所有的列













二、逻辑表达式

示例：salary>10000 && salary<20000



逻辑运算符：



         and（&&）:两个条件如果同时成立，结果为true，否则为false

         or(||)：两个条件只要有一个成立，结果为true，否则为false

         not(!)：如果条件成立，则not后为false，否则为true



这个和java里的&& ，|| ，  !一样的



Demo 03:



-- 查询员工编号大于10的男员工

select * from tbl_employee where id > 10 && gender = 1; 14



结果:

![](https://img-blog.csdnimg.cn/2018112316501079.png)





三、模糊查询

示例：last_name like 'a%'



模糊查询这个要说一下:



模糊查询用like

‘%a%’ 表示字段值里包含a的

‘a%’  表示以a开头的

‘%a’  表示以a结尾的



我的数据其实不太适合模糊查询，这个就不写Demo了，大家可以自己造数据测一下










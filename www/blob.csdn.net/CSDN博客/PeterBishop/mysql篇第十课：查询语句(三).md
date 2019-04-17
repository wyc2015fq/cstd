# mysql篇第十课：查询语句(三) - PeterBishop - CSDN博客





2018年11月23日 17:26:51[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：10








  这节课讲常用函数


- 单行函数 
- 字符函数 

concat拼接 

substr截取子串 

upper转换成大写 

lower转换成小写 

trim去前后指定的空格和字符 

ltrim去左边空格 

rtrim去右边空格 

replace替换 

lpad左填充 

rpad右填充 

instr返回子串第一次出现的索引 

length 获取字节个数

以上函数除了concat基本上都不会用到



2、数学函数

         round 四舍五入

         rand 随机数

         floor向下取整

         ceil向上取整

         mod取余

         truncate截断

以上函数视情况使用，虽然还是基本不会用到，不过rand还是有机会用的



3、日期函数

         now当前系统日期+时间

         curdate当前系统日期

         curtime当前系统时间

         str_to_date 将字符转换成日期

         date_format将日期转换成字符



前三个会用到



Demo 01:

-- 用下now这个函数

select id,now() from tbl_employee where id = 1;



结果:

![](https://img-blog.csdnimg.cn/20181123165938445.png)



说明下，我们可以给列取别名

在select后要查询的东西后面加 as 加 想取的别名就行，比如:



Demo 02:

-- 用下now这个函数

select id,now() as 当前时间 from tbl_employee where id = 1;



结果:

![](https://img-blog.csdnimg.cn/20181123165938451.png)



而且，as可以省略



Demo 03:

-- 用下now这个函数

select id,now() 当前时间 from tbl_employee where id = 1;



结果:

![](https://img-blog.csdnimg.cn/20181123165938528.png)



3、其他函数

         version版本

         database当前库

         user当前连接用户


- 分组函数:

         sum 求和

         max 最大值

         min 最小值

         avg 平均值

         count 计数



Demo 04:

-- 试试分组函数

select count(id) as 记录数,sum(id) as id总和,max(id) as 最大id from tbl_employee;



结果:

![](https://img-blog.csdnimg.cn/20181123165938532.png)



特点：

         1、以上五个分组函数都忽略null值，除了count(*)

         2、sum和avg一般用于处理数值型

                  max、min、count可以处理任何数据类型

    3、都可以搭配distinct使用，用于统计去重后的结果

         4、count的参数可以支持：

                  字段、*、常量值，一般放1



            建议使用 count(*)




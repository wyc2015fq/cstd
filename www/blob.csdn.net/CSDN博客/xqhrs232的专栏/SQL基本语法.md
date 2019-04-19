# SQL基本语法 - xqhrs232的专栏 - CSDN博客
2017年10月19日 10:35:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：145
个人分类：[SQLServer/Oracle数据库技术](https://blog.csdn.net/xqhrs232/article/category/906969)
原文地址::[http://blog.csdn.net/chendong_/article/details/47864657](http://blog.csdn.net/chendong_/article/details/47864657)
相关文章
1、SQL的基本语法----[http://blog.csdn.net/vera514514/article/details/8276568](http://blog.csdn.net/vera514514/article/details/8276568)
2、**[SQL常用语法](http://blog.csdn.net/smilepasta035/article/details/51743300)----[http://blog.csdn.net/smilepasta035/article/details/51743300](http://blog.csdn.net/smilepasta035/article/details/51743300)**
3、[Sql常用语法](http://www.cnblogs.com/qq4004229/archive/2012/01/09/2317231.html)----[http://www.cnblogs.com/qq4004229/archive/2012/01/09/2317231.html](http://www.cnblogs.com/qq4004229/archive/2012/01/09/2317231.html)
4、SQL 语法----[http://www.w3school.com.cn/sql/sql_syntax.asp](http://www.w3school.com.cn/sql/sql_syntax.asp)-----------每个语句进行讲解并举例说明，非常不错
5、SQL基本语句----[http://download.csdn.net/download/lxs10903080313/3912412](http://download.csdn.net/download/lxs10903080313/3912412)
# SQL基本语法
这篇文章介绍最基本的SQL语法。
创建数据库
`create database db_name;`
删除数据库
`drop database db_name;`
创建表
create table tb_name(字段名 数据类型 [约束]，。。。);“` 
根据已经有的表创建表
```
create table tb_name1 like tb_name2;
create table tb_name1 as select 字段名...from tb_name2 definition only
```
- 1
- 2
删除表
`drop table tb_name;`
增加一个列
`alter table tb_name add 字段名 数据类型 [约束]`
删除一个列
`alter table tb_name drop 字段名；`
修改一个列
`alter table alter 旧字段名 新字段名 数据类型 [约束]`
添加主键
`alter table tb_name add primary key(col);`
删除主键
`alter table tb_name drop primary key(col);`
增删改查
查询语句
```
select (*|count(*)|函数|字段名) [as 自定义字段名] from tb_name
where 判断条件
group by 分组条件
having 筛选条件
order by 字段名 [desc],字段名 [desc]...默认升序
limit [m,]n 跳过m条显示n条，默认m=0;
```
- 1
- 2
- 3
- 4
- 5
- 6
删除语句
`delete from tb_name where 条件`
修改语句
`update tb_name set 字段名 = "新值" where 条件`
关键字
```
between and 在两者之间，包含两者
or and  或 且
not 非
in 在..中
like 模糊查询
escape 设置转义字符
```
- 1
- 2
- 3
- 4
- 5
- 6
函数 
`avg(),sum(),max(),min()`
为大家提供一个很好的关于SQL的网站 
[http://www.runoob.com/sql/sql-tutorial.html](http://www.runoob.com/sql/sql-tutorial.html)


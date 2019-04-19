# SQL.Mysql中Cast()函数的用法 - 数据之美的博客 - CSDN博客
2017年06月09日 09:42:23[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：6077
之前讲到了orcale的字符串与日期等类型的转换，现在我们来看看[MySQL](http://lib.csdn.net/base/mysql)是怎么转换的。比起orcale，MySQL相比之下就简单得多了，只需要一个Cast()函数就能搞定。其语法为：Cast(字段名
 as 转换的类型 )，其中类型可以为：
CHAR[(N)] 字符型 
DATE  日期型
DATETIME  日期和时间型
DECIMAL  float型
SIGNED  int
TIME  时间型
例如表table1
date
2015-11-03 15:31:26
select cast(date as signed) as date from  table1;
结果如下：
date
20151103153126
select cast(date as char) as date from  table1;
结果如下：
date
2015-11-03 15:31:26
select cast(date as datetime) as date from  table1;
结果如下：
date
2015-11-03 15:31:26
select cast(date as date) as date from  table1;
结果如下：
date
2015-11-03
select cast(date as time) as date from  table1;
结果如下：
date
15:31:26
这里date对应日期，time对应时间

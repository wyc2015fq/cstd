# Hive  开窗函数 - Simple 专栏 - CSDN博客
2018年10月10日 18:42:42[Simple_Zz](https://me.csdn.net/love284969214)阅读数：244
### 什么时候用开窗函数？
开窗函数常结合聚合函数使用，一般来讲聚合后的行数要少于聚合前的行数，但是有时我们既想显示聚集前的数据,
又要显示聚集后的数据,这时我们便引入了窗口函数。
### 窗口函数的语法
UDAF() over (PARTITION By col1，col2 order by col3 窗口子句（rows between .. and ..）) AS 列别名
注意：PARTITION By后可跟多个字段，order By只跟一个字段。
### over()的作用
over()决定了聚合函数的聚合范围，默认对整个窗口中的数据进行聚合，聚合函数对每一条数据调用一次。
例如：
```
select name, orderdate, cost, sum(cost) over()
from business;
```
### partition by 子句
使用Partiton by子句对数据进行分区，可以用paritition by对区内的进行聚合。
例如：
```
select name, orderdate, cost, sum(cost) over(partition by name)
from business;
```
### order by 子句
- 对分区中的数据进行排序。
- 确定聚合哪些行（默认从起点到当前行的聚合）。
例如：
```
select name, orderdate, cost, sum(cost) over(partition by name order by orderdate)
from business;
```
### 窗口子句
- CURRENT ROW  当前行
- n PRECEDING  往前n行数据
- n FOLLOWING  往后n行数据
- UNBOUNDED PRECEDING  表示从前面的起点
- UNBOUNDED FOLLOWING  表示到后面的终点
通过使用partition by子句将数据进行了分区。如果想要对窗口进行更细的动态划分，
就要引入窗口子句。
例如：
```
select name, orderdate,cost, sum(cost) 
over(partition by name order by orderdate rows between UNBOUNDED PRECEDING and CURRENT ROW)
from business;
```
### 注意
- order by 必须跟在 partition by 后;
- rows 必须跟在 Order by 后；
- (partition by .. order by ..)  可替换为  (distribute by .. sort by ..)

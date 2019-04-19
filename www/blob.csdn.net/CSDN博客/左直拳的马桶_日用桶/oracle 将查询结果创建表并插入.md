# oracle 将查询结果创建表并插入 - 左直拳的马桶_日用桶 - CSDN博客
2017年06月24日 10:53:33[左直拳](https://me.csdn.net/leftfist)阅读数：5381
如果在sql server的t-sql，是很容易的：
```
select * into 新表名
```
这个新表不存在的话，就创建之。
其实在oracle，也很容易，不过是另一种写法：
```
create table 新表名 as select ...
```

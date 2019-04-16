# 用SQL进行多值列拆分成二值列的一个实现 - 沧海一粟 —— 技术随手记 - CSDN博客





2017年09月26日 11:02:46[慢游](https://me.csdn.net/eengel)阅读数：1920








在进行数据分析的时候，往往需要把数据表格中的多值列拆分成二值列。如图1中的表A所示，fruits为一个多值列，值可以是不同的水果名字，表明某个以ID作为标识的人吃水果的记录。现在的需求是要把是否吃了什么具体水果拆成多个列，最终如表B所示。在这个例子中，吃水果的记录还有重复，所以需要把重复的记录去掉。

具体来说，可以将这个任务拆分成多个步骤，由不同的SQL select语句完成。如图2所示。这儿使用的数据库是DB2。

![](https://img-blog.csdn.net/20170926111708289?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZWVuZ2Vs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


图1

![](https://img-blog.csdn.net/20170926111714876?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZWVuZ2Vs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


图2

# 1. 拆分列

从表A到A1的过程重点是需要将每一行中的水果名字转化成相应新列的0/1值。



```
select id, INSTR(fruits, 'apple') as apple, INSTR(fruits, 'banana') as banana
  from A;
```



# 2. 去重

可以看到，id为3的记录重复出现了。因此需要用group by去掉重复的。这次的查询是基于上一次的结果，因此我们用with将上一次的查询包含进来。



```
with q1 as (
  select id, INSTR(fruits, 'apple') as apple, INSTR(fruits, 'banana') as banana
  from A
)
select *
  from q1
  group by id, apple, banana;
```



# 3. 整合行

A2中，id为2的记录出现了2次，可以把相应的列的值整合起来。同样用with将第二次的查询包含进来。



```
with q1 as (
  select id, INSTR(fruits, 'apple') as apple, INSTR(fruits, 'banana') as banana
  from A
), q2 as (
  select *
  from q1
  group by id, apple, banana
)
select id, sum(apple) as apple, sum(banana) as banana
  from q2
  group by id;
```










# [数据库] Navicat for MySQL换种思维解决插入同时更新数据 - 杨秀璋的专栏 - CSDN博客

2017年03月12日 01:46:41[Eastmount](https://me.csdn.net/Eastmount)阅读数：3157所属专栏：[HTML网站前端设计](https://blog.csdn.net/column/details/13444.html)[数据库实战开发设计与优化](https://blog.csdn.net/column/details/14842.html)



这篇文章是我的学生在实际项目中遇到的一个案例，在对某张表插入数据过程中，某些特定的字段需也要进行更新，比如说部门编号在前端插入，而部门名称在插入时应该自动更新，如果前端设置选择编号又选择部门就重复功能了，那么数据库怎么实现呢？
最早学生想通过触发器实现，设置一个插入触发器，插入的同时更新数据，但是问题来了，在同一张表中，触发器是不能同时插入又更新的。这篇文章主要讨论这个问题的解决方法。同时为了加深大家对触发器的映像，也会在给大家讲述些触发器的知识，起到讨论的作用。
希望文章对你有所帮助，尤其是我的学生和数据库基础的博友以及解决实际项目中的类似问题。如果文章中存在错误或不足之处，还请海涵~
推荐前文：
[[数据库] Navicat for MySQL触发器更新和插入操作](http://blog.csdn.net/eastmount/article/details/52344036)
[[数据库] Navicat for MySQL事件Event实现数据每日定期操作](http://blog.csdn.net/eastmount/article/details/60984798)


## 一. 触发器问题
现在存在两张表。第一张表为部门表department，核心字段为unit（学院名称）、depid（学院编号），如下图所示：

![](https://img-blog.csdn.net/20170312004854576)

第二张表为插入信息表task，depid表示部门编号、source表示部门名称，对应表department表的unit字段。还有插入时间、公告事件等字段，这里省略了。

![](https://img-blog.csdn.net/20170312005055077)
现在前端有个按钮，选择部门编号，但是想通过department部门表同时插入部门名称，怎么实现呢？首先，学生想到的是通过触发器，如下所示：

```python
DROP TRIGGER IF EXISTS `upd_info`;  
create trigger upd_info  
after insert on task for each row   
begin  
    update task,department set source = department.unit  
        where task.depid=department.depid;  
end;
```
触发器设置成功。但是当插入数据时，报错如下所示：[Err] 1442 - Can't update table 'task' in stored function/trigger because it is already used by statement which invoked this stored function/trigger.
![](https://img-blog.csdn.net/20170312005717393)
注意：该错误表示如果你在触发器里面对刚刚插入的数据进行了 insert/update, 则出现这个问题。因为会造成循环的调用。解决方法通过set设置值。
参考地址：[http://blog.csdn.net/java2000_net/article/details/3857579](http://blog.csdn.net/java2000_net/article/details/3857579)


## 二. 触发器解决
现在修改需要通过set来设置值，我修改如下所示：

```python
DROP TRIGGER IF EXISTS `test`;  
create trigger test  
before insert on task for each row   
begin  
	declare dname varchar(20);
	select department.unit into dname from department,task 
		where task.depid=department.depid and task.depid=new.depid;
	set new.source = dname; 
end;
```
其中需要注意几点：
**1.触发器Before和After的区别。**
(1) before（insert、update）可以对new进行修改；
(2) after不能对new进行修改；
(3) 两者都不能修改old数据；
(4) 对于Insert语句，只有new是合法的，对于delete语句，只有old合法，而update可以在new和old同时使用。
否则，在after insert触发器中使用old，报错如下：
[Err] 1363 - There is no OLD row in on INSERT trigger
**2.这里使用declare定义变量，并且select a into b把查询结果a赋值到b使用。**
但是运行结果是没有反应，因为查询条件中task.depid=new.depid在before insert触发器中，插入数据前进行查询，其结果是空的；而使用task.depid=old.depid又不能在insert中使用old。
比如执行下列SQL语句：
```python
insert task (depid) VALUES('1067105002');
```
输出结果如下所示：
![](https://img-blog.csdn.net/20170312011445460)
那怎么解决呢？解决方法包括：
1.后台执行插入语句后，再执行一条Update语句，更新该字段是最方便的操作；
2.通过Event实时更新单位名称，参考前文：
[[数据库] Navicat for MySQL事件Event实现数据每日定期操作](http://blog.csdn.net/eastmount/article/details/60984798)
但是如果需要通过SQL语句实现呢？那么我想到的是在使用insert插入过程中调用select查询，这就是我说的换个思维解决该问题。


## 三. 换个思维解决
SQL语句代码如下：
```python
INSERT INTO task(depid,source) 
	SELECT depid, unit FROM department 
  where department.depid='1067103002';
```
运行结果如下所示，可以看到插入数据成功，其中taskid是自增整型，单位名称添加成功了。后台把'1067103002'换成对应的Java变量即可连接前端插入。
![](https://img-blog.csdn.net/20170312013810164)

同样，有些时候需要插入条件判断，如果不存在则插入替代设置唯一性，代码为：

```python
INSERT INTO task(depid) 
SELECT '106710511' FROM DUAL WHERE NOT EXISTS(SELECT depid FROM task WHERE depid = '106710511');
```
参考文章：
[语法：MySQL中INSERT INTO SELECT的使用 - RoadGY](http://www.cnblogs.com/RoadGY/archive/2011/07/22/2114088.html)
[MySQL INSERT插入条件判断：如果不存在则插入](https://my.oschina.net/jsan/blog/270161/)同时，强烈推荐大家阅读"老紫竹"老师的数据库博客，参考：
[http://blog.csdn.net/java2000_net/article/details/4533826](http://blog.csdn.net/java2000_net/article/details/4533826)
最后希望在线笔记对您有所帮助，基础性文章，如果存在错误或不足之处，还请海涵~还是娜么开心、娜么美丽。
(By:Eastmount 2017-03-12 晚上1点http://blog.csdn.net//eastmount/)



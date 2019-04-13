
# Join连接 - 追求卓越,做到专业 - CSDN博客


2013年10月28日 13:18:07[Waldenz](https://me.csdn.net/enter89)阅读数：1062个人分类：[Oracle																](https://blog.csdn.net/enter89/article/category/1078928)[MS SqlServer																](https://blog.csdn.net/enter89/article/category/758258)[
							](https://blog.csdn.net/enter89/article/category/1078928)


连接查询，是关系数据库中最主要的查询，主要包括内连接、外连接和交叉连接等。通过连接运算符可以实现多个表查询。连接是关系数据库模型的主要特点，也是它区别于其它类型数据库管理系统的一个标志。
创建表，插入数据
```python
create table students (
 SID varchar(4) not null primary key,
 NAME varchar(30),
 AGE int,
 CLASSID varchar(3)
);
create table classes (
   CLASSID varchar(3) not null primary key,
   CLASSNAME varchar(20)
);
```

```python
insert into students(   
select 'S001','Student1','1','C01' from dual union all  
select 'S002','Student2','2','C02' from dual union all  
select 'S003','Student3','3','C04' from dual);
insert into classes(  
select 'C01','Class No.1'from dual union all  
select 'C02','Class No.2' from dual union all  
select 'C03','Class No.3' from dual);
```
Students :                                                                           Classes:
![](https://img-blog.csdn.net/20131028113646921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZW50ZXI4OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20131028113654781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZW50ZXI4OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**1. 自连接 join, inner join**

```python
--自连接 :只返回两张表连接列的匹配项。
  --以下三种查询结果一样。
  select * from students s, classes c where s.classid=c.classid; 
  select * from students s inner join classes c on s.classid=c.classid;
  select * from students s join classes c on s.classid=c.classid;
```
![](https://img-blog.csdn.net/20131028110617421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZW50ZXI4OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**2. 笛卡尔积 cross join**

```python
--笛卡儿乘积连接 ：即不加任何条件，达到 M*N 的结果集。
  --以下两种查询结果一样。
  select * from students,classes;
  select * from students s cross join classes c;
```
![](https://img-blog.csdn.net/20131028111527406)**3. 左外连接 left join**

```python
--左连接 ：列出左边表全部的，及右边表符合条件的，不符合条件的以空值代替。
  --在(+)计算时，哪个表带(+)则需要条件符合，另一个则显示全部的。即（+）放左即右连接，放右即左连接。
  --以下结果集相同。
  select * from students s left join classes c on s.classid=c.classid;
  select * from students s,classes c where s.classid=c.classid(+);
```
![](https://img-blog.csdn.net/20131028112248718)
注：（+）是oracle中的一种特殊用法，（+）表示外连接，并且总是放在非主表的一方，哪个表带（+）则需要符合where条件，否则显示空值。
**4. 右外连接 right join**

```python
--右外连接：与左连接正好相反，列出右边表全部的，及左边表符合条件的，不符合条件的用空值替代。
  --(+)一样，它的位置与连接相反。
  select * from students s right join classes c on s.classid=c.classid;
  select * from students s,classes c where s.classid(+)=c.classid;
```
![](https://img-blog.csdn.net/20131028112810687)**5. 全连接 full join**

```python
--全连接 ：产生M+N的结果集，列出两表全部的，不符合条件的，以空值代替。
  select * from students s full join classes c on s.classid=c.classid;
```
![](https://img-blog.csdn.net/20131028113258234)



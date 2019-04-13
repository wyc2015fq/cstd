
# hive的JOIN和SQL执行计划解读 - leofionn的博客 - CSDN博客


2018年01月20日 15:39:39[leofionn](https://me.csdn.net/qq_36142114)阅读数：107



## hive的join相关分析：
**hive常用的join有两大类：**
common join/reduce join/shuffle join 一般的join
mapjoin 优化器优化后的join
**hive默认使用的join：**
当 hive.auto.convert.join = true时，优化器默认将common join转化成mapjoin
当 hive.auto.convert.join = false时，默认使用 common join
**测试SQL：**
```python
select
```
```python
e.empno, e.ename, e.deptno, d.dname
```
```python
from
```
```python
emp e
```
```python
join
```
```python
dept d
```
```python
on
```
```python
e.deptno=d.deptno;
```
```python
-- shuffle：将相同的deptno分到一个reduce上去
```
```python
-- emp表所需列 ：<deptno,(empno,ename)>
```
```python
-- dept表所需列：<deptno,(dname)>
```
```python
-- 正常的操作如以上分析，分别扫表取出emp表和dept表对应的列
```
```python
-- 然后将列deptno相同的数据分配到一个reduce上去，查出数据
```

**该SQL的common join执行计划解读：**
```python
set
```
```python
hive.auto.convert.
```
```python
join
```
```python
=
```
```python
false
```
```python
;
```
```python
explain
```
```python
select
```
```python
e.empno, e.ename, e.deptno, d.dname
```
```python
from
```
```python
emp e
```
```python
join
```
```python
dept d
```
```python
on
```
```python
e.deptno=d.deptno;
```
```python
-- 设置优化器参数为false，使用explain关键字查看执行计划
```
common join 执行了两步，第一步是map+reduce，第二部是展示数据，
第一步中，map操作分别对两表进行扫描，根据deptno分组，查出需要的列数据，传递给reduce，
然后在reduce操作中进行join操作，最终得出结果数据集。
**该SQL的mapjoin执行计划解读：**
```python
set
```
```python
hive
```
```python
.auto
```
```python
.convert
```
```python
.join
```
```python
= true
```
```python
;
```
```python
explain
select e
```
```python
.empno
```
```python
, e
```
```python
.ename
```
```python
, e
```
```python
.deptno
```
```python
, d
```
```python
.dname
```
```python
from emp e join dept d on e
```
```python
.deptno
```
```python
=d
```
```python
.deptno
```
```python
;
```
mapjoin比common join多了一步，首先启动了一个本地的Map Reduce作业，读d表，
然后启动了一个非本地的Map Reduce作业，是一个真实的Map操作，读e表，
然后并没有启动真实的Reduce操作，而直接在Map端进行了join操作，最后展示数据。
使用优化器将commmon join 优化成mapjoin，省掉了Reduce操作，效率更高。
## 两种join的进一步分析：
![](https://img-blog.csdn.net/20180114154434913?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvendqenFxYg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
两表进行common join，需要对两表分别启动一组map作业，将数据根据join的条件进行排序，
经过网络shuffle后传输到同一个reduce作业，然后启动该reduce作业，进行join，然后查出数据。
这中join性能是较差的，因为两表的数据map之后需要经过shuffle进行网络传输。
![](https://img-blog.csdn.net/20180114155114361?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvendqenFxYg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
两表进行mapjoin，首先启动一个本地的MR Local Task，会去读小表（根据表的元数据中的统计信息确定），将小表的数据读入之后生成一个HashTable文件，将该文件存入hadoop的分布式缓存中；
然后启动一个Map任务，将另外一个表的数据读入之后和上一步存入到入hadoop的分布式缓存中的HashTable文件进行join操作，查出数据。
这种join是没有shuffle进行网络传输的，是性能比较高的join方法。










来自@若泽大数据



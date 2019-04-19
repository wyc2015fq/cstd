# 那条linq语句为啥这么慢 - 左直拳的马桶_日用桶 - CSDN博客
2015年09月29日 19:47:05[左直拳](https://me.csdn.net/leftfist)阅读数：2921
目前所在的项目大量使用了linq，结果有个地方出现了严重的性能问题。一个统计需要3、40秒。头头焦头烂额之际，也让我看看。
我向来喜欢性能调优，自诩编码极为注重性能。曾几何时，也动不动就把性能挂在嘴边。总之，我貌似是一个性能砖家。
不过，对于这条linq，我阴沟翻船，搞不定。一来我半路杀入，不了解情况；二来我看不懂那些linq。
我想到的切入点，是监控数据库运行的sql，看看问题出在哪里。
数据库是oracle的。如果是sql server，有图形化的界面profile侍候，但oracle的话，plsql工具有个SQL 跟踪，但好像变灰了，用不了。可以查询视图V$SQL。
用sys登录，运行如下语句：
```
select sql_text,sql_fulltext,cpu_time,elapsed_time,first_load_time,last_load_time,last_active_time
from v$sql
where last_active_time is not null
and module='w3wp.exe' --来自于IIS
and service='ORCL' --指定数据库实例名称
order by last_active_time desc --按时间倒序排
```
视图V$SQL字段极多，涉及到时间的有好几个。first_load_time,last_load_time这些不宜用来考察运行的时间点。因为看上去这里面涉及到缓存。一条SQL语句，如果没有变化，那么一次加载，可以重复使用很久。
我运行了这条查询语句，可以看到所有的SQL。问题是，调试那个linq，却看不到有啥SQL出现。真奇怪啊。
后来头头自己搞定了。原因是参与这条linq运算的对象是一个IEnumable<>，改成IQueryable<>就可以了。估计是IEnumable<>对象的话，系统会自动ToList()，将数据获取到内存里，然后参与运算；而IQueryable<>的话，则最后编译成sql到数据库运行，这时过滤条件什么的会起作用，速度当然快了。这也是为何那条LINQ不见有SQL产生的原因。
但是，IQueryable 是继承 IEnumable 的。这听上去有点诧异。

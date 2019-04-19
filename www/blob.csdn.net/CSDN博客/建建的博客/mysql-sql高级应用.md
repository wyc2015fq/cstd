# mysql-sql高级应用 - 建建的博客 - CSDN博客
2017年03月15日 11:06:38[纪建](https://me.csdn.net/u013898698)阅读数：138
个人分类：[Mysql](https://blog.csdn.net/u013898698/article/category/6790249)

sql语言进阶
典型操作
order by
```
- select * from play_list order by createtime;
- select * from play_list order by bookedcount desc,createtime asc;
```
order by  语句用于根据指定的列对结果集进行排序
order by  语句默认按照升序对记录排序，使用desc则降序排序
order by  也可以多个字段排序，而desc只作用于一个字段；
distinct
```
select distinct userid from play_list;
     select distinct userid,play_name from play_list;    
    （userid,play_named都相同时去重）
```
distinct 用于去重，可以返回多列的唯一组合；
distinct 在后台做排序，所以很消耗CUP的；
group by having
场景：统计云音乐创建歌单的用户列表和每人创建歌单的数量
```
mysql> select userid,count(*) AS play_num from play_list group by userid having count(*)>=**2**;
```
分组关键字userid，需要在查询中出现，且一般查询分组关键字后要加聚合函数；
like
```
select * from play_list where play_name like '%男孩%'；
```
|通配符|描述|
|----|----|
|%|代替一个或多个字符|
|_|替代单个字符|
|[charlist]|中括号中的任何单一字符|
|[^charlist]或者[!charlist]|不在中括号中的任何单一字符|
大表慎用like;
除了%号在最右边以外，会对表中所有记录进行查询匹配；
limit offset
场景4：查询一个月内创建的歌单（从第6行开始显示10条记录）
```
select * from play_list where (createtime between **1427701323** and **1430383307**) limit **10** offset **6**
```
注意：offset 后面的值不要太大，假设offset1000，那它会扫描前1000条记录，这样IO开销会很大
case when
case when 实现类似编程语言的 if  else 功能，可以对SQL的输出结果进行选择判断；
场景5：对于未录入的歌单（trackcount  = null）,输出结果时歌曲数返回0
```
mysql> select play_name,case when trackcount is null then **0** else trackcount end from play_list;
```
连接-join
用一个SQL 语句把多个表中相互关联的数据查询出来；
场景6：查询收藏“老男孩”歌单的用户列表
```
mysql> SELECT play_fav.userid FROM play_fav INNER JOIN play_list ON play_fav.play_id = play_list.id where play_list.play_name = '老男孩';
```
另一种写法：
```
mysql> select f.userid from play_list lst,play_fav f where lst.id = f.play_id and lst.play_name = '老男孩'
```
子查询及问题
子查询的写法：
```
select userid from play_fav where play_id=(select id from play_list where play_name = '老男孩');
```
别名 可以不用使用AS 关键字，直接空格加别名就可以了；
子查询在性能上有一定劣势，不利于mysql性能优化器进行优化；
因为内层表和驱动表用户自己定死了，而联结的驱动表和内层表 性能优化器 会根据实际情况 来定；
子查询为什么不利于优化：
联结是嵌套循环查询实现；
如select * from play_list,play_fav where play_list.id = play_fav.play_id;
play_list驱动表（where等号左边）；内层表play_fav（where等号右边）;
遍历去东北play_list.id，找到一个id后再去play_fav.play_id中找；依次循环下去；
内层表此时可以查询一次或者几次索引，便可以得到；
所以基本的优化就是将表量比较小的作为驱动表，这样减少了循环的次数；
union
作用：把不同表中相同的字段聚合在一个结果集中返回给用户
场景8：老板想看创建和收藏歌单的所有用户，查询play_list和play_fav两表中所有的userid;
```
mysql> select userid from play_list
         -> union
         -> select userid from play_fav;
```
默认的union 会对结果集进行去重处理，不想去重使用 union all；
DML进阶语法
- 多值插入：insert into table values（.....）,(.....)
- 覆盖插入：replace into  table values(...)
- 忽略插入：insert ignore into table values(...)
- 查询插入：insert into table_a select * from table_b
多值插入：减少数据库访问次数，提高效率；
覆盖插入，忽略插入：简化业务逻辑的判断；避免主键重复；
查询插入：导表结构中；
insert主键重复则update：
     insert into table tb1 values(id,col1,col2)
on duplicate key update col2=....;
```
mysql> insert into  a values(**1**,**100**) on duplicate key update age=**100**;
```
如果id = 1存在，则键age 更改为100；
注意得是主键噢，如何表中没有设置主键，则会新增加一条记录；
而给表a增加主键则是： 
```
mysql> alter table order add primary key （id）;
```
连表update
用B表中的age 更新到 A 表中的age :
```
mysql> select * from a;
+----+------+
| id | age  |
+----+------+
|  **1** |  **100** |
|  **2** |   **34** |
|  **3** |   **23** |
|  **4** |   **29** |
+----+------+
mysql> select * from b;
+------+------+------+
| id   | name | age  |
+------+------+------+
|    **1** | pw   |   **20** |
|    **2** | ljb  |   **30** |
+------+------+------+
mysql> update a,b set a.age=b.age where a.id = b.id;
mysql> select * from a;
+----+------+
| id | age  |
+----+------+
|  **1** |   **20** |
|  **2** |   **30** |
|  **3** |   **23** |
|  **4** |   **29** |
+----+------+
```
连表delete
用B表中的条件去删除A表中数据；
```
mysql> select * from a;
+----+------+
| id | age  |
+----+------+
|  **1** |   **20** |
|  **2** |   **30** |
|  **3** |   **23** |
|  **4** |   **29** |
+----+------+
mysql> select * from b;
+------+------+------+
| id   | name | age  |
+------+------+------+
|    **1** | pw   |   **20** |
|    **2** | ljb  |   **30** |
+------+------+------+
mysql> delete a from a,b where a.id=b.id and b.name='pw';
mysql> select * from a;
+----+------+
| id | age  |
+----+------+
|  **2** |   **30** |
|  **3** |   **23** |
|  **4** |   **29** |
+----+------+
```
删除语法：
```
DELETE FROM Customers
WHERE cust_id = '1000000006';
```
连表在delete之后还要将需要删除的表放在delete后面；
内置函数
- 目标：掌握常用的mysql聚合函数，预定义函数
- 在SQL查询语句中运用上述函数结构group by,order by等语法完成各种统计功能
聚合函数
- 聚合函数面向一组数据，对数据进行聚合运算后返回单一的值
- mysql聚合函数基本语法：select function（列） from 表
常用聚合函数：
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160718224608919-995877572.png)
场景：查询播放次数最多的歌曲
```
mysql> select song_name,max(playcount) from song_list; //错误查法
#select song_name,没有对应 playcount；
#注意聚合函数是对返回列来做处理的，此中放回列是所有歌曲；
 
mysql> select song_name,playcount from song_list order by playcount desc limit1；//正确
 
子查询方法：
select song_name from song_list where playcount=(select max(playcount) from song_list);
```
场景：显示每张专辑的歌曲列表。例如：
```
mysql> select album,group_concat(song_name) from song_list group by album;
+------------------+-------------------------------------------------+
| album            | group_concat(song_name)                  |
+------------------+-------------------------------------------------+
| **1701**             | 大象,定西                                        |
| Straight Shooter | Good Lovin' Gone Bad,Weep No More,Shooting Star |
| 作品李宗盛         | 风柜来的人                                       |
| 红雪莲            | 红雪莲                                           |
+------------------+-------------------------------------------------+
```
group_concat 连接的最长字符是1024，可以通过参数调整；
使用聚合函数做数据库行列转换：
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160718224746872-1562515255.png)
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160718224800357-1218703445.png)
预定义函数：
- 预定义函数面向单一值数据，返回一对一的处理结果（聚合函数可以理解成多对一）
- 预定义函数基本语法：select  function（列） from 表；select * from 表 where 列 = function（value）
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160718224813529-2082424917.png)
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160718224827951-1909983040.png)
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160718224835435-1458015000.png)

时间处理函数：
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160718224846232-2131409088.png)
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160718224902513-29605103.png)
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160718224912076-2105688142.png)
总结
- order by
- distinct
- limit offset:
- case when then else end
- 连接-join两种写法
- 子查询为什么不利于优化：优化器不能改变，驱动表，内层表；从而不能优化；驱动表一般表量较小，因其需要全表id；内层表，仅需要查找一个或几个索引；这就是jion后优化器工作
- union：把不同表中相同字段聚合在一个结果集中
- 连表update，根据B表age值更新A表age：update a,b set a.age=b.age where a.id = b.id;
- 连表delete，根据B表name删除A表的数据：delete a from a,b where a.id=b.id and b.name='pw';
- 聚合函数：AVG(),COUNT(),COUNT(DISTNCT),MAX(),MIN(),SUM()常与 group by,order by连用；
- group_concat()mysql特有
- 预定义函数

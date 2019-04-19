# MySQL 索引及查询优化 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [brucefeng](http://www.jobbole.com/members/brucefengofnju) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
### 1. MySQL索引的原理
#### 1.1 索引目的
索引的目的在于提高查询效率，可以类比字典，如果要查“mysql”这个单词，我们肯定需要定位到m字母，然后从下往下找到y字母，再找到剩下的sql。如果没有索引，那么你可能需要把所有单词看一遍才能找到你想要的，如果我想找到m开头的单词呢？或者ze开头的单词呢？
#### 1.2 索引原理
除了词典，生活中随处可见索引的例子，如火车站的车次表、图书的目录等。它们的原理都是一样的，通过不断的缩小想要获得数据的范围来筛选出最终想要的结果，同时把随机的事件变成顺序的事件，也就是我们总是通过同一种查找方式来锁定数据。
每个数据表都有一个主键（*如果没有主键，数据库会将该表中的唯一索引当作主键使用？*），MySQL会以主键的方式构造一棵树，叶子节点存放该主键对应的整行数据。
![](http://7xkbey.com1.z0.glb.clouddn.com/%E8%A1%A8.png)
自己建立的索引，一般叫做辅助索引，辅助索引的树，也自己节点存放了两个东西，一个是索引自身的值，另外一个是索引对应主键的值。
![](http://7xkbey.com1.z0.glb.clouddn.com/%E7%B4%A2%E5%BC%95.png)
如果索引是联合索引，比如UserID和AddTime索引的方式，索引叶子节点会存储UserID和AddTime之间的配对+主键的配对数据。
#### 1.3 索引的类型
##### 1.3.1 B-树索引
**B-树索引在生产环境更为广泛，这里我只针对B-树索引进行讨论**
B-树索引是一个复杂的内容，可以参见[B-tree](https://en.wikipedia.org/wiki/B-tree)。
##### 1.3.2 Hash索引
哈希索引（Hash Index）建立在哈希表的基础上，它只对使用了索引中的每一列的精确查找有用。对于每一行，存储引擎计算出了被索引的哈希码（Hash Code），它是一个较小的值，并且有可能和其他行的哈希码不同。它把哈希码保存在索引中，并且保存了一个指向哈希表中的每一行的指针。
在mysql中，只有memory存储引擎支持显式的哈希索引。
- Hash 索引仅仅能满足”=”,”IN”和”<=>”查询，不能使用范围查询。
- Hash 索引无法被用来避免数据的排序操作。
- Hash 索引不能利用部分索引键查询。
- Hash 索引在任何时候都不能避免表扫描。
- Hash 索引遇到大量Hash值相等的情况后性能并不一定就会比B-Tree索引高
##### 1.3.3 空间索引（R-树）索引
主要用于GIS中空间数据的存储，但是MySQL的空间索引支持并不好，现在多使用PostgreSQL。
##### 1.3.4 全文索引(Full-text)索引
文本字段上的普通索引只能加快对出现在字段内容最前面的字符串(也就是字段内容开头的字符)进行检索操作。如果字段里存放的是由几个、甚至是多个单词构成 的较大段文字，普通索引就没什么作用了。这种检索往往以LIKE %word%的形式出现，这对MySQL来说很复杂，如果需要处理的数据量很大，响应时间就会很长。
这类场合正是全文索引(full-text index)可以大显身手的地方。在生成这种类型的索引时，MySQL将把在文本中出现的所有单词创建为一份清单，查询操作将根据这份清单去检索有关的数 据记录。全文索引即可以随数据表一同创建，也可以等日后有必要时再使用下面这条命令添加：

MySQL
```
ALTER TABLE tablename ADD FULLTEXT(column1, column2)
```
### 2. 索引的合理使用
|字段名|数据类型|NULL|INDEX|
|----|----|----|----|
|ID|int(10)|NOT NULL|pk|
|UserID|int(10)|NOT NULL||
|Mobile|varchar(15)|NOT NULL||
|ArriveDate|DateTime|NOT NULL||
|AddDate|DateTime|NOT NULL||
|UpdateTime|timetamp|NOT NULL||
下面我们的分析都会这个数据表为例。
#### 2.1 索引在查询中的使用
**不使用索引**
![](http://7xkbey.com1.z0.glb.clouddn.com/%E6%95%B0%E6%8D%AE.png)
现在刚建立的表上没有任何索引，但是我们想通过UserID找出ArriveDate这个数据，SQL将会写成如下方式：

MySQL
```
select ArriveDate from TestSQL where UserID = 10;
```
![](http://7xkbey.com1.z0.glb.clouddn.com/%E6%97%A0%E7%B4%A2%E5%BC%95%E5%88%86%E6%9E%90.png)
这个SQL没有走索引，数据库就会根据主键（ID）扫描全表，每拿到一条数据库记录就与where条件比对，如果符合条件则将这条记录返回，重复直到全表扫描完毕。
在大数据量的情况下，不使用索引进行查询几乎是不可行的。
- 使用UserID作为索引
![](http://7xkbey.com1.z0.glb.clouddn.com/userId%E7%B4%A2%E5%BC%95.png)
这时候看到where条件是使用了UserID索引的。这时候数据库引擎会根据UserID到索引上找到ID，然后根据ID去查询对应记录，从而取出ArriveDate数据。
现在我们将UserID的索引更换为UserID,ArriveDate的联合索引。
现在再来查询：
![](http://7xkbey.com1.z0.glb.clouddn.com/useIDDate%E7%B4%A2%E5%BC%95.png)
可以发现这个Extra里面也是使用了索引的，这就意味这个SQL是完全走了索引，数据库引擎根据UserID找到对应的索引， 因为Select的字段是索引的一部分，所以找到索引之后不需要再读取表记录了。
当一个查询语句中使用设计到多个索引时，MySQL数据库引擎会计算不同索引涉及到的行数大小，选取行数最小的索引作为实际执行时使用的索引，如：
![](http://7xkbey.com1.z0.glb.clouddn.com/%E8%8C%83%E5%9B%B4%E6%9F%A5%E8%AF%A2sql%E7%B4%A2%E5%BC%95.png)
**一次查询同一张表，MySQL每次只会使用一个索引。**
#### 2.2 索引在范围查询的使用
范围查询主要是指查询字段值在某个范围内的记录，表现在where条件中为>,<,between等关键字。如，我们使用如下SQL进行查询：

MySQL
```
select UserID,ArriveDate  from TestSQL where UserID >0 and UserID <100  and ArriveDate = '2015-09-23 00:00:00';
```
作为对比，我们使用另一种SQL查询相同记录：

MySQL
```
select UserID,ArriveDate from TestSQL where UserID in(2,12) and ArriveDate = '2015-09-23 00:00:00';
```
联调SQL都会查询出相同的记录：
|UserID|ArriveDate|
|----|----|
|2|2015-09-23 00:00:00|
|12|2015-09-23 00:00:00|
第一个SQL（使用范围查询）的explain结果为：
![](http://7xkbey.com1.z0.glb.clouddn.com/%E8%8C%83%E5%9B%B4%E6%9F%A5%E8%AF%A2sql%E7%B4%A2%E5%BC%95.png)
第二个SQL（未使用范围查询）的explain结果为：
![](http://7xkbey.com1.z0.glb.clouddn.com/%E8%8C%83%E5%9B%B4%E6%9F%A5%E8%AF%A2sql%E7%B4%A2%E5%BC%95.png)
对比可见，两种sql的索引长度是不一样的。在范围查询中，索引的使用是遵循最左（leftmost）原则，例如这个表的使用的索引是IX_UserID_ArriveDate，但是因为UserID使用了范围查询(Range query)，就不再使用ArrvieDate的索引了。
#### 2.3 排序使用索引
在排序中以下情况无法使用索引：
- Order by 的字段并不是索引
- 使用了两种排序方向，但是索引都是使用升序排列的
在索引的原理里我们讲到过
> 
如果索引是联合索引，比如UserID和AddTime索引的方式，索引叶子节点会存储UserID和AddTime之间的配对+主键的配对数据。
这种情况下，索引会以UserID进行排序，当UserID相同时再以AddTime进行排序（默认为升序），以我们现有的数据库为例：

MySQL
```
select UserID,ArriveDate  from TestSQL where UserID in(2,3,4)   order by UserID asc,ArriveDate desc;
```
这种情况将只使用UserID 作为索引，而ArriveDate 将不再作为索引。
- 不符合最左（Left most）的条件

MySQL
```
select * from TestSQL where  ArriveDate =  '2015-09-23 00:00:00' order by UserID desc
```
这种情况将不再使用UserID_ArriveDate索引
- 联合查询的情况下，如果第一个筛选条件是范围查询，MySQL不再使用剩下的索引

MySQL
```
select *  from TestSQL where UserID >1 and UserID < 5 order by UserID desc,ArriveDate desc
```
这种情况下，因为UserID 为范围查询，所以就不会再使用ArriveDate 索引了。
#### 2.4 join 中使用索引
我们来新建一个TestSQL_join表，其结构与TestSQL 相同，但只有Mobile索引。首先来看一个简单的join操作

MySQL
```
select * from TestSQL join TestSQL_join on TestSQL.id = TestSQL.id
```
MySQL首先比较TestSQL和 TestSQL_join表那个行数少，如TestSQL中的记录较少，TestSQL就是一个小表，而TestSQL_join则是大表，MySQL引擎先把TestSQL中的ID全部去出来，然后根据id到TestSQL_join中查询相关的记录。
![](http://7xkbey.com1.z0.glb.clouddn.com/join%E6%97%A0where.png)
在这里，TestSQL中行数决定了循环的次数，但是TestSQL_join则决定了每次循环查询所需要查询的时间；这时如果TestSQL_join中的ID是索引则会大大减少查询时间如下SQL:

MySQL
```
select * from TestSQL a join TestSQL_join b on a.ID = b.ID
```
由于b.ID 是TestSQL_join的主键，查询使用了TestSQL_join主键索引。
![](http://7xkbey.com1.z0.glb.clouddn.com/join%20%E6%97%A0%E7%B4%A2%E5%BC%95.png)
如果此时对TestSQL增加条件筛选：

MySQL
```
select * from TestSQL a join TestSQL_join b on a.ID = b.ID
where a.UserID = 1;
```
因为a.UserID 是TestSQL的索引，所以在过滤TestSQL表的行数时，采用次索引查询对应ID，然后根据ID查询TestSQL_join的记录。
![](http://7xkbey.com1.z0.glb.clouddn.com/join%E4%BD%BF%E7%94%A8%E7%B4%A2%E5%BC%95%E8%BF%87%E6%BB%A4.png)
如果此时针对TestSQL_join 增加where条件过滤：

MySQL
```
select * from TestSQL a join TestSQL_join b on a.ID = b.ID
where a.UserID >1 and b.Mobile = "2147483647"
```
这种时候，因为TestSQL 和 TestSQL_join 根据where条件所筛选出来的行数大小可能会有变化，也就是说TestSQL_join 有可能会变成小表，这时候将会优先从TestSQL_join 查询出相关ID，然后根据ID去查询TestSQL。
join操作时，大表小表的概念，主要是按照两张表分别执行对应查询条件，哪个开销更小，哪个就是小表。
**join操作虽然在SQL层面很方便，而且在线上大流量的情况下，一旦SQL的join操作导致查询缓慢，较难即使优化。另外在服务化的系统中，容易导致业务领域不清晰，所以在互联网大流量的应用中是不推荐使用join操作的。**
### 3. 索引建立的原则
- 使用区分度高的列作为索引
区分度的公式是count(distinct col)/count(*)，表示字段不重复的比例，区分度越高，索引树的分叉也就越多，一次性找到的概率也就越高。
- 尽量使用字段长度小的列作为索引
- 使用数据类型简单的列（int 型，固定长度）
- 选用NOT NULL的列
在MySQL中，含有空值的列很难进行查询优化，因为它们使得索引、索引的统计信息以及比较运算更加复杂。你应该用0、一个特殊的值或者一个空串代替空值。
- 尽量的扩展索引，不要新建索引。比如表中已经有a的索引，现在要加(a,b)的索引，那么只需要修改原来的索引即可。这样也可避免索引重复。
### 4. 索引使用的原则
- 最左前缀匹配原则(leftmost)，mysql会一直向右匹配直到遇到范围查询(>、<、between、like)就停止匹配，=和in可以乱序，一个联合索引中，如UserID，ArriveDate的联合索引，使用ArriveDate in （）and UserID = 的任意顺序，MySQL的查询优化器会帮你优化成索引可以识别的形式
- 索引列不能参与计算

MySQL
```
select *  from TestSQL where UserID + 1 >1 and UserID < 5
```
这种方式UserID 的索引就不会再被使用，因为在进行检索时，需要把所有元素都应用函数才能比较，显然成本太大。
另外当使用<>,like通配符放置在最前面 如：like’%ddd’ ,not, in, !=等运算符都不会使用索引。
- 查询数据库记录时，查询到的条目数尽量小，当通过索引获取到的数据库记录> 数据库总揭露的1/3时，SQL将有可能直接全表扫描，索引就失去了应有的作用。
### 5. explain的使用
explain是MySQL查询优化过程的神器，详情可以查看[explain的使用](http://dev.mysql.com/doc/refman/5.0/en/explain-output.html#explain-extra-information)
### References
- High Performance MySQL-THIRD EDITION
- [explain的使用](http://dev.mysql.com/doc/refman/5.0/en/explain-output.html#explain-extra-information)
- [MySQL索引学习漫画](http://mp.weixin.qq.com/s?__biz=MzA5NTQxNjA4NQ==&mid=211982023&idx=1&sn=5bc45006475f63258f7ac78283e23781&key=dffc561732c22651884a66f1d1ebe9536538747eec9accebae9bb52c3834bb1c9eee59714db6c3261df4f7e9b538d0db&ascene=0&uin=MjE5NTY4NQ%3D%3D&devicetype=iMac+MacBookPro12%2C1+OSX+OSX+10.10.5+build(14F27)
- [MySQL索引原理及慢查询优化](http://blog.jobbole.com/86594/)
- [理解MySQL——索引与优化](http://www.cnblogs.com/hustcat/archive/2009/10/28/1591648.html)
- [mysql索引的类型和优缺点](http://www.cnblogs.com/kupig/archive/2011/10/19/2217228.html)
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/b947d0acc8cb4e3b0649a27c60646636-1.jpg)![](http://www.jobbole.com/wp-content/uploads/2016/04/f22f5454bd93395413ea7e7507db150a-1.jpg)

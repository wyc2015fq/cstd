# SQLServer的分页优化兼谈谈使用Row_Number()分页存在的问题 - 文章 - 伯乐在线
原文出处： [马非码](http://www.cnblogs.com/marvin/p/4204826.html)
最近有项目反应，在服务器CPU使用较高的时候，我们的事件查询页面非常的慢，查询几条记录竟然要4分钟甚至更长，而且在翻第二页的时候也是要这么多的时间，这肯定是不能接受的，也是让现场用`SQLServerProfiler`把语句抓取了上来。
## 用ROW_NUMBER()进行分页
我们看看现场抓上来的分页语句：

MySQL
```
select top 20 a.*,ag.Name as AgentServerName,,d.Name as MgrObjTypeName,l.UserName as userName 
from eventlog as a 
    left join mgrobj as b on a.MgrObjId=b.Id and a.AgentBm=b.AgentBm 
    left join addrnode as c on b.AddrId=c.Id 
    left join mgrobjtype as d on b.MgrObjTypeId=d.Id 
    left join eventdir as e on a.EventBm=e.Bm 
    left join agentserver as ag on a.AgentBm=ag.AgentBm 
    left join loginUser as l on a.cfmoper=l.loginGuid 
where a.OrderNo not in  (
    select top 0 OrderNo  
    from eventlog  as a 
        left join mgrobj as b on a.MgrObjId=b.Id 
        left join addrnode as c on b.AddrId=c.Id  
    where 1=1 and a.AlarmTime>='2014-12-01 00:00:00' and a.AlarmTime='2014-12-01 00:00:00' and a.AlarmTime
```
这是典型的使用两次top来进行分页的写法，原理是：先查出`pageSize*(pageIndex-1)`(T1)的记录数，然后再`Top`出`PageSize`条不在T1中的记录，就是当前页的记录。这种查询效率不高主要是使用了`not in`。参考我之前文章《[程序猿是如何解决SQLServer占CPU100%的](http://www.cnblogs.com/marvin/p/ASolutionForSQLServerCauseHighCPU.html)》提到的：**“对于不使用SARG运算符的表达式，索引是没有用的”**。
那么改为使用`ROW_NUMBER`分页：

MySQL
```
WITH cte AS(
    select a.*,ag.Name as AgentServerName,d.Name as MgrObjTypeName,l.UserName as userName,b.AddrId
            ,ROW_NUMBER() OVER(ORDER BY AlarmTime DESC) AS RowNo
        from eventlog as a WITH(FORCESEEK) 
            left join mgrobj as b on a.MgrObjId=b.Id and a.AgentBm=b.AgentBm 
            left join addrnode as c on b.AddrId=c.Id 
            left join mgrobjtype as d on b.MgrObjTypeId=d.Id 
            left join eventdir as e on a.EventBm=e.Bm 
            left join agentserver As ag on a.AgentBm=ag.AgentBm 
            left join loginUser as l on a.cfmoper=l.loginGuid 
        where a.AlarmTime>='2014-12-01 00:00:00' and a.AlarmTime
```
执行时间从14秒提升到5秒，这说明Row_Number分页还是比较高效的，而且这种写法比`top top`分页优雅很多。
## “欺骗”查询引擎让查询按你的期望去查询
但是为什么查询20条记录竟然要5秒呢，尤其在这个表是加上了时间索引的情况下——参考《[程序猿是如何解决SQLServer占CPU100%的](http://www.cnblogs.com/marvin/p/ASolutionForSQLServerCauseHighCPU.html)》中提到的索引。
我尝试去掉这句`AND b.AddrId in ('02109000',……,'02109002')`，结果不到1秒就把538条记录查询出来了，而加上地点限制这句，结果是204行。为什么结果集不大，花费的时间却相差这么多呢？查看执行计划，发现走的是另外的索引，而非时间索引。
![](http://jbcdn2.b0.upaiyun.com/2016/08/fb99f4d14e43f50aea56b5d71f62ce28.png)
把这个疑问放到了SQLServer群上，很快，高桑给了回复：要想达到跟去掉地点限制这句的效果，就使用`AdddrId+'' in`。
什么意思？一时没看明白，是高桑没看懂我的语句？很快，有人补充，要欺骗查询引擎。“**欺骗**”?还是不懂，不过我照做了，把上述cte的语句原封不动的Copy出来，然后把这句`AND b.AddrId in ('02109000',……,'02109002')`更改为了`AND b.AddrId+'' in ('02109000',……,'02109002')`，一点执行，神了！！！不到1秒就执行完了。在把执行计划一对，果然走的是时间索引：
![](http://jbcdn2.b0.upaiyun.com/2016/08/8162e5ecefcc2ad546779c08e3c19c7e.png)
后来回味了一下，记起之前看到的查询引擎优化原理，如果你的条件中带有运算符或者使用函数等，则查询引擎会放弃优化，而执行表扫描。脑袋突然转过来了，在使用`b.AddrId+''`前查询引擎尝试把mgrObj表加入一起做优化，那么两个表联查，会导致预估的记录数大大增加，而使用了`b.AddrId+''`，查询引擎则会先按时间索引把记录刷选出来，这样就达到了效果，即强制先做cte在执行`in`条件，而不是在cte中进行`in`条件刷选。原来如此！**有时候，查询引擎过度的优化，会导致相反的效果，而你如果能够知道优化的原理，那么就可以通过一些小的技巧让查询引擎按你的期望去进行优化**。
## ROW_NUMBER()分页在页数较大时的问题
事情到这里，还没完。后面同事又跟我反应，查询到后面的页数，又卡了！what?我重新执行上述语句，把时间范围放到2011-12-01到2014-12-26，记录数限制为为19981到20000，果然，查询要30秒左右，查看执行计划，都是一样的，为什么？
高桑怀疑是key lookup过多导致的，建议先分页取出rid 再做key lookup。不懂这么一句是什么意思。把执行计划和IO打印出来：
![](http://jbcdn2.b0.upaiyun.com/2016/08/59ace0dac5b0bad32839589784b89d88.png)
看看IO，很明显，主要是越到后面的页数，其他的几个关联表读取的页数就越多。我推测，**在Row_Number分页的时候，如果有表连接，则按排序一致到返回的记录数位置，前面的记录都是要参与表连接的**，这就导致了越到后面的分页，就越慢，因为要扫描的关联表就越多。
难道就没有了办法了吗？这个时候宋桑英勇的站了出来：“你给表后加一个`forceseek`提示可破”。这真是犹如天籁之音，马上进行尝试。
### 使用forceseek提示可以强制表走索引
查了下资料：
> 
SQL Server2008中引入的提示`ForceSeek`，可以用它将索引查找来替换索引扫描
那么，就在eventlog表中加上这句看看会怎样?
![](http://jbcdn2.b0.upaiyun.com/2016/08/1e05ab9fc2ff2787c838d927d4e10e9e.png)
果然，查询计划变了，开始提示，缺少了包含索引。赶紧加上，果然，按这个方式进行查询之后查询时间变为18秒，有进步！但是查看IO，跟上面一样，并没有变少。不过，总算学会了一个新的技能，而宋桑也很热心说晚上再帮忙看看。
### 把其他没参与where的表放到cte外面
根据上面的IO，很快，又有人提到，把其他`left join`的表放到cte外面。这是个办法，于是把除`eventlog`、`mgrobj`、`addrnode`的表放到外面，语句如下：

MySQL
```
WITH cte AS(
    select a*,b.AddrId,b.Name as MgrObjName,b.MgrObjTypeId          
            ,ROW_NUMBER() OVER(ORDER BY AlarmTime DESC) AS RowNo
        from eventlog as a
            left join mgrobj as b on a.MgrObjId=b.Id and a.AgentBm=b.AgentBm 
            left join addrnode as c on b.AddrId=c.Id 
        where a.AlarmTime>='2011-12-01 00:00:00' and a.AlarmTime
```
果然有效，IO大大减少了，然后速度也提升到了16秒。

MySQL
```
表 'loginuser'。扫描计数 1，逻辑读取 63 次，物理读取 0 次，预读 0 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
表 'agentserver'。扫描计数 1，逻辑读取 1617 次，物理读取 0 次，预读 0 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
表 'mgrobjtype'。扫描计数 1，逻辑读取 126 次，物理读取 0 次，预读 0 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
表 'eventdir'。扫描计数 1，逻辑读取 42 次，物理读取 0 次，预读 0 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
表 'addrnode'。扫描计数 1，逻辑读取 119997 次，物理读取 0 次，预读 0 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
表 'Worktable'。扫描计数 0，逻辑读取 0 次，物理读取 0 次，预读 0 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
表 'eventlog'。扫描计数 1，逻辑读取 5027 次，物理读取 3 次，预读 5024 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
表 'mgrobj'。扫描计数 1，逻辑读取 24 次，物理读取 0 次，预读 0 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
```
我们看到，addrNode表还是扫描计数很大。那还能不能提升，这个时候，我想到了，先把`addrNode`、`mgrobj`、`mgrobjtype`三个表联合查询，放到一个临时表，然后再和`eventlog`做`inner join`，然后查询结果再和其他表做`left join`，这样还能减少IO。
### 使用临时表存储分页记录在进行表连接减少IO

MySQL
```
IF OBJECT_ID('tmpMgrObj') IS NOT NULL DROP TABLE tmpMgrObj
SELECT m.Id,AddrId,MgrObjTypeId,AgentBM,m.Name,a.Name AS AddrName 
    INTO tmpMgrObj  
    FROM dbo.mgrobj m
        INNER JOIN dbo.addrnode a ON a.Id=m.AddrId
    WHERE AddrId IN('02109000',……,'02109002');
WITH cte AS(
    select a.*,b.AddrId,b.MgrObjTypeId          
            ,ROW_NUMBER() OVER(ORDER BY AlarmTime DESC) AS RowNo
            ,ag.Name as AgentServerName
    ,d.Name as MgrObjTypeName,l.UserName as userName
        from eventlog as a
            INNER join tmpMgrObj as b on a.MgrObjId=b.Id and a.AgentBm=b.AgentBm
            left join mgrobjtype as d on b.MgrObjTypeId=d.Id 
            left join agentserver As ag on a.AgentBm=ag.AgentBm 
            left join loginUser as l on a.cfmoper=l.loginGuid 
    WHERE AlarmTime>'2011-12-01 00:00:00' AND AlarmTime
```
这次查询仅用了10秒。我们来看看IO：

MySQL
```
表 'Worktable'。扫描计数 0，逻辑读取 0 次，物理读取 0 次，预读 0 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
表 'mgrobj'。扫描计数 1，逻辑读取 24 次，物理读取 2 次，预读 23 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
表 'addrnode'。扫描计数 1，逻辑读取 6 次，物理读取 3 次，预读 0 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
----------
表 'loginuser'。扫描计数 0，逻辑读取 24 次，物理读取 1 次，预读 0 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
表 'Worktable'。扫描计数 0，逻辑读取 0 次，物理读取 0 次，预读 0 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
表 'eventlog'。扫描计数 93，逻辑读取 32773 次，物理读取 515 次，预读 1536 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
表 'tmpMgrObj'。扫描计数 1，逻辑读取 3 次，物理读取 0 次，预读 0 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
表 'mgrobjtype'。扫描计数 1，逻辑读取 6 次，物理读取 1 次，预读 0 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
表 'agentserver'。扫描计数 1，逻辑读取 77 次，物理读取 2 次，预读 0 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
```
除了eventlog之外，其他的表的IO大大减少，有木有？
### 强制使用hash join
经网友提示，在大的页数时，可以强制使用`hash join`来减少IO，而且经过尝试，可以通过建立两个子查询来避免使用临时表。经过调整，最终优化的SQL语句如下：

MySQL
```
SELECT  *
    ,ag.Name AS AgentServerName
    , l.UserName AS userName
FROM    ( 
    SELECT    a.*,ROW_NUMBER() OVER (ORDER BY AlarmTime DESC) AS RowNo
        , b.AddrName , b.Name AS MgrObjName
    FROM
        (SELECT    * 
            FROM      eventlog
            WHERE     AlarmTime>= '2011-12-01 00:00:00' AND AlarmTime
```
在大的分页的时候，通过hash查询，不必扫描前面的页数，可以大大减少IO，但是，由于`hash join`是强制性的，所以使用的时候要注意，我这里应该是个特例。
> 
查询分析器的提示：**“警告: 由于使用了本地联接提示，联接次序得以强制实施。”**
我们来看看对应情况下的IO：

MySQL
```
表 'eventlog'。扫描计数 5，逻辑读取 5609 次，物理读取 34 次，预读 5636 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
表 'Worktable'。扫描计数 3，逻辑读取 375 次，物理读取 0 次，预读 0 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
表 'Worktable'。扫描计数 0，逻辑读取 0 次，物理读取 0 次，预读 0 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
表 'mgrobj'。扫描计数 5，逻辑读取 24 次，物理读取 8 次，预读 40 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
表 'mgrobjtype'。扫描计数 1，逻辑读取 6 次，物理读取 1 次，预读 0 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
表 'addrnode'。扫描计数 3，逻辑读取 18 次，物理读取 6 次，预读 0 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
表 'loginuser'。扫描计数 1，逻辑读取 60 次，物理读取 2 次，预读 0 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
表 'eventdir'。扫描计数 1，逻辑读取 40 次，物理读取 0 次，预读 0 次，lob 逻辑读取 30 次，lob 物理读取 0 次，lob 预读 0 次。
表 'agentserver'。扫描计数 1，逻辑读取 1540 次，物理读取 1 次，预读 0 次，lob 逻辑读取 0 次，lob 物理读取 0 次，lob 预读 0 次。
```
这次的IO表现非常的好，没有因为查询后面的页数增大而导致较大的IO，**查询时间从没有使用`hash join`的50秒提升为只需12秒**，查询时间的开销应该耗费了在`hash`查找上了。
再看看对应的查询计划，这个时候，主要是因为排序的开销较大。
![](http://jbcdn2.b0.upaiyun.com/2016/08/e394df5936b346fc38f8bd0840bac31b.png)
我们再看看他的预估的和执行的区别，为什么会让排序占如此大的开销？
![](http://jbcdn2.b0.upaiyun.com/2016/08/2ed6ae943dc57395276ca488c46dc8d8.png)
很明显，预估的时候只需对刷选的结果排序，但是实际执行是对前面所有的页数进行了排序，最终排序占了大部分的开销。那么，这种情况能破吗?请留下您的回复！
## 其他优化参考
在另外的群上讨论时，发现使用`ROW_NUMBER`分页查询到后面的页数会越来越慢的这个问题的确困扰了不少的人。
有的人提出，谁会这么无聊，把页数翻到几千页以后?一开始我也是这么想的，但是跟其他人交流之后，发现确实有这么一种场景，我们的软件提供了**最后一页**这个功能，结果……当然，一种方法就是在设计软件的时候，就去掉这个最后一页的功能；另外一种思路，就是查询页数过半之后，就反向查询，那么查询最后一页其实也就是查询第一页。
还有一些人提出，把查询出来的内容，放到一个临时表，这个临时表中的加入自增Id的索引，这样，可以通过辨别Id来进行快速刷选记录。这也是一种方法，我打算稍后尝试。但是这种方法也是存在问题的，就是无法做到通用，必须根据每个表进行临时表的构建，另外，在超大数据查询时，插入的记录过多，因为索引的存在也是会慢的，而且每次都这么做，估计CPU也挺吃紧。但是不管怎么样，这是一种思路。
你有什么好的建议？不妨把你的想法在评论中提出来，一起讨论讨论。
## 总结
现在，我们来总结下在这次优化过程中学习到什么内容：
- 在SQLServer中，`ROW_NUMBER`的分页应该是最高效的了，而且兼容SQLServer2005以后的数据库
- 通过“欺骗”查询引擎的小技巧，可以控制查询引擎部分的优化过程
- `ROW_NUMBER`分页在大页数时存在性能问题，可以通过一些小技巧进行规避
- 尽量通过cte利用索引
- 把不参与`where`条件的表放到分页的cte外面
- 如果参与`where`条件的表过多，可以考虑把不参与分页的表先做一个临时表，减少IO
- 在较大页数的时候强制使用`hash join`可以减少io，从而获得很好的性能
- 使用`with(forceseek)`可以强制查询因此进行索引查询
最后，感谢SQLServer群的高桑、宋桑、肖桑和其他群友的大力帮助，这个杜绝吹水的群非常的棒，让我这个程序猿学到了很多数据库的知识！
**注：经网友提示，2015-01-07 09:15做以下更新**：
> - 可以在记录数超过10000条，则采用`hash join`强制进行hash连接，减少IO
- 去掉最先给定的结果中采用`left join`而不是`inner join`的连接——`left join`的结果相当于没有用上`addrId in ()`的条件
## 参考文章
- [曲演杂坛–蛋疼的ROW_NUMBER函数](http://www.cnblogs.com/TeyGao/p/3851809.html)
- [为什么超长列表数据的翻页技术实现复杂](http://timyang.net/data/key-list-pagination/)

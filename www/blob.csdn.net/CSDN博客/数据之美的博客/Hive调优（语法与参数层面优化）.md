# Hive调优（语法与参数层面优化） - 数据之美的博客 - CSDN博客
2017年06月08日 21:08:54[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：271
个人分类：[大数据																[hive/sql](https://blog.csdn.net/oppo62258801/article/category/6961227)](https://blog.csdn.net/oppo62258801/article/category/6768103)
一、简介
作为企业[Hadoop](http://lib.csdn.net/base/hadoop)应用的核心产品，[Hive](http://lib.csdn.net/base/hive)承载着FaceBook、淘宝等大佬
 95%以上的离线统计，很多企业里的离线统计甚至全由Hive完成，如我所在的电商。Hive在企业[云计算](http://lib.csdn.net/base/hadoop)平台发挥的作用和影响愈来愈大，如何优化提速已经显得至关重要。
好的[架构](http://lib.csdn.net/base/architecture)胜过任何优化，好的Hql同样会效率大增，修改Hive参数，有时也能起到很好的效果。
有了瓶颈才需要优化 
1、Hadoop的主要性能瓶颈是IO负载，降IO负载是优化的重头戏。 
2、对中间结果的压缩 
3、合理设置分区，静态分区和动态分区
二、Hive Sql语法层面和Properties参数层面优化
优化方法
合并小文件 
避免数据倾斜,解决数据倾斜 
减少job数据（合并job，大job的拆分…）
优化手段
2.1 合理控制Map和Reduce数
（一）map数
1、Map数过大 
Map阶段输出文件太小，产生大量小文件（下一个阶段就需要进行小文件合并，或者到reduce阶段就会浪费很多reduce数）。 
初始化和创建map的开销很大。
2、Map数过小 
文件处理或查询并发度小，Job执行空间过长。 
大量作业时，容易堵塞集群。
通常情况下，作业会通过input文件产生一个或者多个map数 
主要的决定因素有：input文件数，input文件大小。 
举例 
a）假设input目录下有1个文件a，大小为800M，那么hadoop会将该文件a分隔成7个块（6个128M的块和1个32M的块，Block是128M)，从而产生7个map数。
b）假设input目录下有3个文件a,b,c,大小分别为30M,60M,130M， 
那么hadoop会分隔成4个块（30M,60M,128M,2M)，从而产生4个map数。
拆分是根据大文件来分的，而map数是根据文件数来生成的。
解决方法：
两种方式控制Map数：即减少map数和增加map数 
1、减少map数可以通过合并小文件来实现，这点是对文件源。
2、增加map数的可以通过控制上一个job的reduce数来控制(一个sql中join多个表会分解为多个mapreduce)
Map对应参数和默认值：
`set hive.merge.mapfiles = true;`#在Map-only的任务结束时合并小文件，map阶段Hive自动对小文件合并。
```
hive> set hive.merge.mapfiles;
hive.merge.mapfiles=true(默认)
```
- 1
- 2
- 1
- 2
`set hive.merge.mapredfiles = true;`#默认false， true时在MapReduce的任务结束时合并小文件
`set hive.merge.per.task = 256*1000*1000;`#合并文件的大小
`set mapred.max.split.size = 256000000;`#每个Map最大分割大小（hadoop）
`set mapred.min.split.size.per.node = l00000000;` #一个节点上split的最小值
`set hive.input.format =org.apache.hadoop.hive.ql.io.CombineHiveInputFormat;`#执行Map前进行小文件合并
在开启了`org.apache.hadoop.hive.ql.io.CombineHiveInputFormat`后,一个datanode节点上多个小文件会进行合并，合并文件数大小由 `mapred.max.split.size` 限制的大小决定。 
`mapred.min.split.size.per.node` 决定了 多个datanode上的文件 是否需要合并。
Hive中设置map数：参数`mapred.map.tasks`
`set mapred.map.tasks=100;` （并不是每次都是有效的，要看大小是否合理）
set的作用域是session级
案例环境如下：
```
hive.merge.mapredfiles=true （默认是false，可以在hive-site.xml里配置）
hive.merge.mapfiles=true
hive.merge.per.task=256000000
mapred.map.tasks=2（默认值）
```
- 1
- 2
- 3
- 4
- 1
- 2
- 3
- 4
因为合并小文件默认为true，而dfs.block.size与hive.merge.per.task的搭配使得合并后的绝大部分文件都在256MB左右。
Case1：
现在我们假设有3个300MB大小的文件，整个JOB**会有**6个map．其牛3个map分别处理256M的数据，还有3个map分别处理44M的数据。
那么木桶效应就来了，整个Job的map阶段的执行时间,不是看最短的1个map的执行时间，而是看最长的1个map的执行时间。虽然有3个map分别只处理44MB的数据，可以很快跑完，但它们还是要等待另外3个处理256MB的map。显然，处理256MB的3个map拖了整个JOB的后腿。
Case2：
如果我们把mapred.map.tasks设置成6，再来看一下变化： 
goalsize=min(900M/6,256M)=150M 
整个JOB同样会分配6个Map来处理，每个map处理150MB，非常均匀，谁都不会拖后腿，最合理地分配了资源，执行时间大约为case1的59%（150/256）。
（二）reduce数
1、Reduce数过大
生成了很多个小文件（最终输出文件由reduce决定，一个reduce一个文件），那么如果这些小文件作为下一个Job输入，则也会出现小文件过多需要进行合并（耗费资源）的问题。 
启动和初始化reduce也会消耗大量的时间和资源，有多少个reduce就会有多少个输出文件。
2、Reduce数过小
每个文件很大，执行耗时。 
可能出现数据领斜。
reduce个数的决定 
默认下，Hive分配reduce数基于以下参数： 
参数1：`hive.exec.reducers.bytes.per.reducer`(默认是1G) 
参数2：`hive.exec.reducers.max`(最大reduce数，默认为999)
计算reduce数的公式： 
N=min（参数2，总输入数据量/参数1）, 
即默认一个reduce处理1G数据量
什么情况下只有一个reduce？
很多时候你会发现任务中不管数据量多大，不管你有没有设置调reduce个数的参数，任务中一直都只有一个reduce任务（会产生数据倾斜）。
原因：
1、数据量小于hive.exec.reducers.bytes.per.reducer参数值（有时，通常情况下设置reduce个数会起作用） 
2、没有group by的汇总 
3、用了order by
解决：
设置reduce数 
参数mapred.reduce.tasks 默认是1 
`set mapred.reduce.tasks=10`
set的作用域是session级
设置reduce数有时对我们优化非常有帮助。 
当某个job的结果被后边job**多次引用**时，设置该参数，以便增大访问的map数。Reuduce数决定中间结果或落地文件数，文件大小和Block大小无关。
2.2 解决数据倾斜
(一) 什么是数据领斜？
hadoop框架的特性决定最怕数据倾斜。 
由于数据分布不均匀，造成数据大量的集中到一点，造成数据热点。
症状： 
map阶段快，reduce阶段非常慢； 
某些map很快，某些map很慢； 
某些reduce很快，某些reduce奇慢。
如下情况： 
A、数据在节点上分布不均匀（无法避免）。 
B、join 时 on 关键词中个别值量很大（如null值） 
C、count(distinct)，数据量大的情况下，容易数据倾斜， 
因为count(distinct)是按group by字段分组，按distinct字段排序。(有时无法避免)
![](https://img-blog.csdn.net/20150504102952778)
（二）数据倾斜的解决方案
1、参数调节
`hive.map.aggr=true`
Map 端部分聚合，相当于Combiner
`hive.groupby.skewindata=true`
有数据倾斜的时候进行负载均衡，当选项设定为true，生成的查询计划会有两个 MR Job。第一个 MR Job 中，Map 的输出结果集合会随机分布到 Reduce 中，每个 Reduce 做部分聚合操作，并输出结果，这样处理的结果是相同的 Group By Key 有可能被分发到不同的 Reduce 中，从而达到负载均衡的目的；第二个 MR Job 再根据预处理的数据结果按照 Group By Key 分布到 Reduce
 中（这个过程可以保证相同的 Group By Key 被分布到同一个 Reduce 中），最后完成最终的聚合操作。
2、SQL语句调节
Join： 
关于驱动表的选取：选用join key分布最均匀的表作为驱动表
做好列裁剪和filter操作，以达到两表做join的时候，数据量相对变小的效果。
大表与小表Join 
使用map join让小的维度表（1000条以下的记录条数）先进内存。在map端完成reduce。
大表与大表Join： 
把空值的key变成一个字符串加上随机数，把倾斜的数据分到不同的reduce上，由于null值关联不上，处理后并不影响最终结果。
count distinct大量相同特殊值： 
count distinct时，将值为空的情况单独处理，如果是计算count distinct，可以不用处理，直接过滤，在最后结果中加1。如果还有其他计算，需要进行group by，可以先将值为空的记录单独处理，再和其他计算结果进行union。
group by维度过小： 
采用sum() 与group by的方式来替换count(distinct)完成计算。
特殊情况特殊处理： 
在业务逻辑优化效果的不大情况下，有些时候是可以将倾斜的数据单独拿出来处理，最后union回去。
3、应用场景
（一）空值产生的数据倾斜 
场景： 
如日志中，常会有信息丢失的问题，比如日志中的user_id，如果取其中的user_id和用户表中的user_id 关联，会碰到数据倾斜的问题。
解决方法1： user_id为空的不参与关联
```
select * from log a
  join users b
  on a.user_id is not null
  and a.user_id = b.user_id
union all
select * from log a
  where a.user_id is null;
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 1
- 2
- 3
- 4
- 5
- 6
- 7
解决方法2 ：赋与空值新的key值
```
select * from log a left outer join users b on case when a.user_id is null then concat('hive',rand() ) 
else a.user_id end = b.user_id;
```
- 1
- 2
- 1
- 2
结论： 
方法2比方法1效率更好，不但io少了，而且作业数也少了。 
解决方法1中 log读取两次，job是2。 
解决方法2中 job数是1 。这个优化适合无效 id (比如 -99 , ”, null 等) 产生的倾斜问题。把空值的key变成一个字符串加上随机数，就能把倾斜的数据分到不同的reduce上，解决数据倾斜问题。
（二）不同数据类型关联产生数据倾斜
场景：用户表中user_id字段为int，log表中user_id字段既有string类型也有int类型。当按照user_id进行两个表的Join操作时，默认的Hash操作会按int型的id来进行分配，这样会导致所有string类型id的记录都分配到一个Reducer中。
解决方法：把数字类型转换成字符串类型
```
select * from users a
  left outer join logs b
  on a.usr_id = cast(b.user_id as string);
```
- 1
- 2
- 3
- 1
- 2
- 3
（三）小表不小不大，怎么用 map join 解决倾斜问题
使用 map join 解决小表(记录数少)关联大表的数据倾斜问题，这个方法使用的频率非常高，但如果小表很大，大到map join会出现bug或异常，这时就需要特别的处理。 
解决如下：
```
select * from log a
  left outer join users b
  on a.user_id = b.user_id;
```
- 1
- 2
- 3
- 1
- 2
- 3
users 表有 600w+ 的记录，把 users 分发到所有的 map 上也是个不小的开销，而且 map join 不支持这么大的小表。如果用普通的 join，又会碰到数据倾斜的问题。 
解决方法：
```
select /*+mapjoin(x)*/* from log a
  left outer join (
    select  /*+mapjoin(c)*/d.*
      from ( select distinct user_id from log ) c
      join users d
      on c.user_id = d.user_id
    ) x
  on a.user_id = b.user_id;
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
假如，log里user_id有上百万个，这就又回到原来map join问题。所幸，每日的会员uv不会太多，有交易的会员不会太多，有点击的会员不会太多，有佣金的会员不会太多等等。所以这个方法能解决很多场景下的数据倾斜问题。

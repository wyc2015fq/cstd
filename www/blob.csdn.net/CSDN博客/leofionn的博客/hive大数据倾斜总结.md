
# hive大数据倾斜总结 - leofionn的博客 - CSDN博客


2018年06月21日 09:49:46[leofionn](https://me.csdn.net/qq_36142114)阅读数：222


ive在跑数据时经常会出现数据倾斜的情况，使的作业经常reduce完成在99%后一直卡住，最后的１%花了几个小时都没跑完，这种情况就很可能是数据倾斜的原因，解决方法要根据具体情况来选择具体的方案
１、join的key值发生倾斜，key值包含很多空值或是异常值
这种情况可以对异常值赋一个随机值来分散key
如：
selectuserid,name
fromuser_info a
join(
selectcase  whenuseridis  null  then  cast ( rand (47)*100000as i nt )
elseuserid
fromuser_read_log
)bona.userid=b.userid
通过rand函数将为null的值分散到不同的值上，在key值比较就能解决数据倾斜的问题
注：对于异常值如果不需要的话，最好是提前过滤掉，这样计算量可以大大减少
2、当key值都是有效值时，解决办法为设置以下几个参数
set hive.exec.reducers.bytes.per.reducer = 1000000000
也就是每个节点的reduce 默认是处理1G大小的数据，如果你的join 操作也产生了数据倾斜，那么你可以在hive 中设定
set hive.optimize.skewjoin = true;
set hive.skewjoin.key = skew_key_threshold （default = 100000）
hive 在运行的时候没有办法判断哪个key 会产生多大的倾斜，所以使用这个参数控制倾斜的阈值，如果超过这个值，新的值会发送给那些还没有达到的reduce, 一般可以设置成你
（处理的总记录数/reduce个数）的2-4倍都可以接受.
倾斜是经常会存在的，一般select 的层数超过2层，翻译成执行计划多于3个以上的mapreduce job 都很容易产生倾斜，建议每次运行比较复杂的sql 之前都可以设一下这个参数. 如果你不知道设置多少，可以就按官方默认的1个reduce 只处理1G 的算法，那么  skew_key_threshold  = 1G/平均行长. 或者默认直接设成250000000 (差不多算平均行长4个字节)
3、reduce数太少
set mapred.reduce.tasks=800;
默认是先设置hive.exec.reducers.bytes.per.reducer这个参数，设置了后hive会自动计算reduce的个数，因此两个参数一般不同时使用
4、对于group by 产生倾斜的问题
set hive.map.aggr=true (开启map端combiner); //在Map端做combiner,假如map各条数据基本上不一样, 聚合没什么意义，做combiner反而画蛇添足,hive里也考虑的比较周到通过参数hive.groupby.mapaggr.checkinterval = 100000 (默认)
hive.map.aggr.hash.min.reduction=0.5(默认)
两个参数的意思是：预先取100000条数据聚合,如果聚合后的条数/100000>0.5，则不再聚合
set hive.groupby.skewindata=true；//`决定``group``by``操作是否支持倾斜的数据。注意：只能对单个字段聚合.`控制生成两个MR Job,第一个MR Job Map的输出结果随机分配到reduce做次预汇总,减少某些key值条数过多某些key条数过小造成的数据倾斜问题
5、小表与大表关联
此时，可以通过mapjoin来优化，
`set``hive.auto.``convert``.``join``=``true``; //将小表刷入内存中`
`set``hive.mapjoin.smalltable.filesize = 2500000 ;//刷入内存表的大小(字节)`


# 1数据倾斜的原因
## 1.1操作：
|关键词
|情形
|后果
|
|Join
|其中一个表较小，
|但是key集中
|分发到某一个或几个Reduce上的数据远高于平均值
|
|大表与大表，但是分桶的判断字段0值或空值过多
|这些空值都由一个reduce处理，灰常慢
|
|group by
|group by 维度过小，
|某值的数量过多
|处理某值的reduce灰常耗时
|
|Count Distinct
|某特殊值过多
|处理此特殊值的reduce耗时
|

## 1.2原因：
1)、key分布不均匀
2)、业务数据本身的特性
3)、建表时考虑不周
4)、某些SQL语句本身就有数据倾斜

## 1.3表现：
任务进度长时间维持在99%（或100%），查看任务监控页面，发现只有少量（1个或几个）reduce子任务未完成。因为其处理的数据量和其他reduce差异过大。
单一reduce的记录数与平均记录数差异过大，通常可能达到3倍甚至更多。 最长时长远大于平均时长。

# 2数据倾斜的解决方案
## 2.1参数调节：
hive.map.aggr=true
Map 端部分聚合，相当于Combiner
hive.groupby.skewindata=true
有数据倾斜的时候进行负载均衡，当选项设定为 true，生成的查询计划会有两个 MR Job。第一个 MR Job 中，Map 的输出结果集合会随机分布到 Reduce 中，每个 Reduce 做部分聚合操作，并输出结果，这样处理的结果是相同的 Group By Key 有可能被分发到不同的 Reduce 中，从而达到负载均衡的目的；第二个 MR Job 再根据预处理的数据结果按照 Group By Key 分布到 Reduce 中（这个过程可以保证相同的 Group By Key 被分布到同一个 Reduce 中），最后完成最终的聚合操作。

## 2.2 SQL语句调节：
如何Join：
关于驱动表的选取，选用join key分布最均匀的表作为驱动表
做好列裁剪和filter操作，以达到两表做join的时候，数据量相对变小的效果。
大小表Join：
使用map join让小的维度表（1000条以下的记录条数） 先进内存。在map端完成reduce.
大表Join大表：
把空值的key变成一个字符串加上随机数，把倾斜的数据分到不同的reduce上，由于null值关联不上，处理后并不影响最终结果。
count distinct大量相同特殊值
count distinct时，将值为空的情况单独处理，如果是计算count distinct，可以不用处理，直接过滤，在最后结果中加1。如果还有其他计算，需要进行group by，可以先将值为空的记录单独处理，再和其他计算结果进行union。
group by维度过小：
采用sum() group by的方式来替换count(distinct)完成计算。
特殊情况特殊处理：
在业务逻辑优化效果的不大情况下，有些时候是可以将倾斜的数据单独拿出来处理。最后union回去。

# 3典型的业务场景
## 3.1空值产生的数据倾斜
场景：如日志中，常会有信息丢失的问题，比如日志中的 user_id，如果取其中的 user_id 和 用户表中的user_id 关联，会碰到数据倾斜的问题。
解决方法1：user_id为空的不参与关联（红色字体为修改后）
[sql][view plain](https://blog.csdn.net/guohecang/article/details/51567434#)[copy](https://blog.csdn.net/guohecang/article/details/51567434#)

select*fromlog a
joinusers b
ona.user_idisnotnull
anda.user_id = b.user_id
unionall
select*fromlog a
wherea.user_idisnull;

解决方法2 ：赋与空值分新的key值
[sql][view plain](https://blog.csdn.net/guohecang/article/details/51567434#)[copy](https://blog.csdn.net/guohecang/article/details/51567434#)

select*
fromlog a
leftouterjoinusers b
oncasewhena.user_idisnullthenconcat(‘hive’,rand() )elsea.user_idend= b.user_id;

结论：方法2比方法1效率更好，不但io少了，而且作业数也少了。解决方法1中 log读取两次，jobs是2。解决方法2 job数是1 。这个优化适合无效 id (比如 -99 , ’’, null 等) 产生的倾斜问题。把空值的 key 变成一个字符串加上随机数，就能把倾斜的数据分到不同的reduce上 ,解决数据倾斜问题。

## 3.2不同数据类型关联产生数据倾斜
场景：用户表中user_id字段为int，log表中user_id字段既有string类型也有int类型。当按照user_id进行两个表的Join操作时，默认的Hash操作会按int型的id来进行分配，这样会导致所有string类型id的记录都分配到一个Reducer中。
解决方法：把数字类型转换成字符串类型
[sql][view plain](https://blog.csdn.net/guohecang/article/details/51567434#)[copy](https://blog.csdn.net/guohecang/article/details/51567434#)

select*fromusers a
leftouterjoinlogs b
ona.usr_id =cast(b.user_idasstring)

## 3.3小表不小不大，怎么用 map join 解决倾斜问题
使用 map join 解决小表(记录数少)关联大表的数据倾斜问题，这个方法使用的频率非常高，但如果小表很大，大到map join会出现bug或异常，这时就需要特别的处理。以下例子:
[sql][view plain](https://blog.csdn.net/guohecang/article/details/51567434#)[copy](https://blog.csdn.net/guohecang/article/details/51567434#)

select*fromlog a
leftouterjoinusers b
ona.user_id = b.user_id;

users 表有 600w+ 的记录，把 users 分发到所有的 map 上也是个不小的开销，而且 map join 不支持这么大的小表。如果用普通的 join，又会碰到数据倾斜的问题。
解决方法：
[sql][view plain](https://blog.csdn.net/guohecang/article/details/51567434#)[copy](https://blog.csdn.net/guohecang/article/details/51567434#)

select/*+mapjoin(x)*/*fromlog a
leftouterjoin(
select/*+mapjoin(c)*/d.*
from(selectdistinctuser_idfromlog ) c
joinusers d
onc.user_id = d.user_id
) x
ona.user_id = b.user_id;

假如，log里user_id有上百万个，这就又回到原来map join问题。所幸，每日的会员uv不会太多，有交易的会员不会太多，有点击的会员不会太多，有佣金的会员不会太多等等。所以这个方法能解决很多场景下的数据倾斜问题。

## 3.4大表join大表，业务场景如何优化
用户轨迹工程的性能瓶颈一直是etract_track_info，其中耗时大户主要在于trackinfo与pm_info进行左关联的环节，trackinfo与pm_info两张表均为GB级别，左关联代码块如下：
[sql][view plain](https://blog.csdn.net/guohecang/article/details/51567434#)[copy](https://blog.csdn.net/guohecang/article/details/51567434#)

fromtrackinfo a
leftouterjoinpm_info b
on(a.ext_field7 = b.id)

使用以上代码块需要耗时1.5小时。
优化流程
第一次优化考虑到pm_info表的id是bigint类型，trackinfo表的ext_field7是string类型，其关联时数据类型不一致，默认的hash操作会按bigint型的id进行分配，这样会导致所有string类型的ext_field7集中到一个reduce里面，因此，改为如下：
[sql][view plain](https://blog.csdn.net/guohecang/article/details/51567434#)[copy](https://blog.csdn.net/guohecang/article/details/51567434#)

fromtrackinfo a
leftouterjoinpm_info b
on(cast(a.ext_field7asbigint) = b.id)

改动为上面代码后，效果仍然不理想，耗时为1.5小时。
第二次优化
考虑到trackinfo表的ext_field7字段缺失率很高（为空、字段长度为零、字段填充了非整数）情况，做进行左关联时空字段的关联操作实际上没有意义，因此，如果左表关联字段ext_field7为无效字段，则不需要关联，因此，改为如下：
[sql][view plain](https://blog.csdn.net/guohecang/article/details/51567434#)[copy](https://blog.csdn.net/guohecang/article/details/51567434#)

fromtrackinfo a
leftouterjoinpm_info b
on(a.ext_field7isnotnull
andlength(a.ext_field7) > 0
anda.ext_field7 rlike'^[0-9]+$'
anda.ext_field7 = b.id)

上面代码块的作用是，如果左表关联字段ext_field7为无效字段时（为空、字段长度为零、字段填充了非整数），不去关联右表，由于空字段左关联以后取到的右表字段仍然为null，所以不会影响结果。
改动为上面代码后，效果仍然不理想，耗时为50分钟。
第三次优化
想了很久，第二次优化效果效果不理想的原因，其实是在左关联中，虽然设置了左表关联字段为空不去关联右表，但是这样做，左表中未关联的记录（ext_field7为空）将会全部聚集在一个reduce中进行处理，体现为reduce进度长时间处在99%。
换一种思路，解决办法的突破点就在于如何把左表的未关联记录的key尽可能打散，因此可以这么做：若左表关联字段无效（为空、字段长度为零、字段填充了非整数），则在关联前将左表关联字段设置为一个随机数，再去关联右表，这么做的目的是即使是左表的未关联记录，它的key也分布得十分均匀
[sql][view plain](https://blog.csdn.net/guohecang/article/details/51567434#)[copy](https://blog.csdn.net/guohecang/article/details/51567434#)

fromtrackinfo a
leftouterjoinpm_info b
on(
casewhen (a.ext_field7isnotnull
andlength(a.ext_field7) > 0
anda.ext_field7 rlike'^[0-9]+$')
then
cast(a.ext_field7asbigint)
else
cast(ceiling(rand() * -65535)asbigint)
end= b.id
)
第三次改动后，耗时从50分钟降为了1分钟32秒，效果显著！
# 4总结
使map的输出数据更均匀的分布到reduce中去，是我们的最终目标。由于Hash算法的局限性，按key Hash会或多或少的造成数据倾斜。大量经验表明数据倾斜的原因是人为的建表疏忽或业务逻辑可以规避的。在此给出较为通用的步骤：
1、采样log表，哪些user_id比较倾斜，得到一个结果表tmp1。由于对计算框架来说，所有的数据过来，他都是不知道数据分布情况的，所以采样是并不可少的。
2、数据的分布符合社会学统计规则，贫富不均。倾斜的key不会太多，就像一个社会的富人不多，奇特的人不多一样。所以tmp1记录数会很少。把tmp1和users做map join生成tmp2,把tmp2读到distribute file cache。这是一个map过程。
3、map读入users和log，假如记录来自log,则检查user_id是否在tmp2里，如果是，输出到本地文件a,否则生成<user_id,value>的key,value对，假如记录来自member,生成<user_id,value>的key,value对，进入reduce阶段。
4、最终把a文件，把Stage3 reduce阶段输出的文件合并起写到hdfs。
如果确认业务需要这样倾斜的逻辑，考虑以下的优化方案：
1、对于join，在判断小表不大于1G的情况下，使用map join
2、对于group by或distinct，设定 hive.groupby.skewindata=true
3、尽量使用上述的SQL语句调节进行优化



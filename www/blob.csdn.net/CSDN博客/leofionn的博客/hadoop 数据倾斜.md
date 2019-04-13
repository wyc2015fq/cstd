
# hadoop 数据倾斜 - leofionn的博客 - CSDN博客


2018年07月04日 23:25:18[leofionn](https://me.csdn.net/qq_36142114)阅读数：59


数据倾斜是指，map / reduce程序执行时，减少节点大部分执行完毕，但是有一个或者几个减少节点运行很慢，这是因为某一个键的条数比其他键多很多（有时是百倍或者千倍之多），这条关键所在的减少节点所处理的数据量比其他节点就大很多，从而导致某几个节点迟迟运行不完。
阿里的这篇比较实用，通俗易懂：倾斜数据总结http://www.tbdata.org/archives/2109
有篇分析比较详细，如果需要使用可以细读：http：//blog.csdn.net/core_cto/article/details/8644692浅析Hadoop中的数据倾斜
·症状和原因：
·操作：join，group by，count distinct
·原因：key分布不均匀，人为的建表疏忽，业务数据特点。
·症状：任务进度长时间维持在99％（或100％），查看任务监控页面，发现只有少量（1个或几个）reduce子任务未完成;查看未完成的子任务，可以看到本地读写数据量积累非常大，通常超过10GB的可以认定为发生数据倾斜。
·倾斜度：平均记录数超过50瓦特且最大记录数是超过平均记录数的4倍;最长时长比平均时长超过4分钟，且最大时长超过平均时长的2倍。
我遇到的问题：
从连接b中选择*;
1.表1000多万，b表不到2亿，用mapjoin显然不行;
2.设置参数set hive.groupby.skewindata = true，不起作用;
3.由于关连键为手机号，自认为业务数据上不存在数据倾斜;
后来通过查看每个表里面关联键的分布，才发现两个表里面都存在空串''，而且严重倾斜，大表里面的空串数量有400多万。
将两个表的空串过滤后再进行关联，job时间由原来的40多分钟减少到2分钟。
总结：
1.数据倾斜的原因就那么几种，逐一排查；
2.细心，动手，不能光凭感觉来判定；
3.判定某一个表的key是否存在数据倾斜，就是group by key,取top N来看；

附：数据倾斜常用解决方法：

1.万能膏药：hive.groupby.skewindata=true
2.大小表关联：将key相对分散，并且数据量小的表放在join的左边，这样有效减少内存溢出错误发生几率；再进一步，可用map join让小的维度表（1000条以下的记录条数）先进内存。在map端完成reduce.
3.大表和大表关联：把空值的key变成一个字符串加上随机数，把倾斜的数据分到不同的reduce上，由于null值关联不上，处理后并不影响最终结果。
4. count distinct大量相同特殊值：（空值单独处理）count distinct时，将值为空的情况单独处理，如果是计算count distinct，可以不用处理，直接过滤，在最后结果中加1。如果还有其他计算，需要进行group by，可以先将值为空的记录单独处理，再和其他计算结果进行union。

用hadoop程序进行数据关联时，常碰到数据倾斜的情况，这里提供一种解决方法。
(1)设置一个hash份数N，用来对条数众多的key进行打散。
(2)对有多条重复key的那份数据进行处理：从1到N将数字加在key后面作为新key，如果需要和另一份数据关联的话，则要重写比较类和分发类（方法如上篇《hadoop job解决大数据量关联的一种方法》）。如此实现多条key的平均分发。
int iNum = iNum % iHashNum;
String strKey = key + CTRLC + String.valueOf(iNum) + CTRLB + “B”;
（3）上一步之后，key被平均分散到很多不同的reduce节点。如果需要和其他数据关联，为了保证每个reduce节点上都有关联的key，对另一份单一key的数据进行处理：循环的从1到N将数字加在key后面作为新key
for(int i = 0; i < iHashNum; ++i){
String strKey =key + CTRLC + String.valueOf(i) ;
output.collect(new Text(strKey), new Text(strValues));}
以此解决数据倾斜的问题，经试验大大减少了程序的运行时间。但此方法会成倍的增加其中一份数据的数据量，以增加shuffle数据量为代价，所以使用此方法时，要多次试验，取一个最佳的hash份数值。
======================================
用上述的方法虽然可以解决数据倾斜，但是当关联的数据量巨大时，如果成倍的增长某份数据，会导致reduce shuffle的数据量变的巨大，得不偿失，从而无法解决运行时间慢的问题。
有一个新的办法可以解决成倍增长数据的缺陷：
在两份数据中找共同点。用重复较少的某个属性作为分布给reduce的依据。比如两份数据里除了关联的字段以外，还有另外相同含义的字段，如果这个字段在所有log中的重复率比较小，则用这个字段计算hash值，如果是数字，直接用来模hash的份数，如果是字符可以用hashcode来模hash的份数（当然数字为了避免落到同一个reduce上的数据过多，也可以用hashcode），这样如果字段值分布足够平均就可以解决上述问题。
我到过的处理的方式
1.mapjoin方式
/*+ MAPJOIN(c,d,e,f) */
其中c,d,e,f是你小表，也就是说可能会倾斜数据的表；
但是对于join，在判断小表不大于1G的情况下，使用map join，也就是要考虑c,d,e,f等表的大小，不能超过内存限制，否则会出现OOM错误；
2.控制空值分布
Java代码![复制代码](http://scholers.iteye.com/images/icon_copy.gif)![收藏代码](http://scholers.iteye.com/images/icon_star.png)![](http://scholers.iteye.com/images/spinner.gif)
select
'${date}'as thedate,
a.search_type,
a.query,
a.category,
a.cat_name,
a.brand_id,
a.brand_name,
a.dir_type,
a.rewcatid,
a.new_cat_name,
a.new_brand_id,
f.brand_name as new_brand_name,
a.pv,
a.uv,
a.ipv,
a.ipvuv,
a.trans_amt,
a.trans_num,
a.alipay_uv
from fdi_search_query_cat_qp_temp a
left outer join brand f
on
f.pt='${date}000000'
andcasewhen a.new_brand_id isnullthen concat('hive',rand() )elsea.new_brand_id end = f.brand_id;

select


'${date}'as thedate,


a.search_type,


a.query,


a.category,


a.cat_name,


a.brand_id,


a.brand_name,


a.dir_type,


a.rewcatid,


a.new_cat_name,


a.new_brand_id,


f.brand_name as new_brand_name,


a.pv,


a.uv,


a.ipv,


a.ipvuv,


a.trans_amt,


a.trans_num,


a.alipay_uv


from fdi_search_query_cat_qp_temp a


left outer join brand f


on


f.pt='${date}000000'


andcasewhen a.new_brand_id isnullthenconcat('hive',rand())elsea.new_brand_id end= f.brand_id;

这样的写法把空值的 key 变成一个字符串加上随机数，就能把倾斜的数据分到不同的reduce上 ,解决数据倾斜问题。
如果上述的方法还不能解决，比如当有多个JOIN的时候，建议建立临时表，然后拆分HIVE SQL语句；
关于数据倾斜，阿里集团数据平台上的博客文章有很好的几个方法，敢兴趣的人也可以去看一下：[http://www.tbdata.org/archives/2109](http://www.tbdata.org/archives/2109)
3.关于nonstrict
join同样一张表多次的时候，会出现这样的错误信息：
FAILED: Error in semantic analysis: In strict mode, cartesian product is not allowed. If you really want to perform the operation, set hive.mapred.mode=nonstrict
解决方式是在SQL前面加上如下：
set hive.mapred.mode=nonstrict;
strict模式在下面三种情况下有限制：
(1) partition表需要加上分区裁剪
(2) order by 只有一个reduce，需要加上limit
(3) join时，如果只有一个reduce，笛卡尔积不支持。
HIVE小技巧：
1.hive sql中：
sum(t.shop_gmvcount + t.GMVCOUNT_NEW + t.auc_shop_gmvcount + t.spu_gmv_cnt) gmv_cnt,
这样的统计结果，当t.t.shop_gmvcount为NULL时，即使后面的t.GMVCOUNT_NEW 不为null，那么总计的结果这个计算仍然是NULL;
修改的方法是：采用sum(coalesce(t.shop_gmvcount,cast(0 as bigint)) + coalesce(t.GMVCOUNT_NEW,cast(0 as bigint))
这样的方式，coalesce函数类似于ORACLE数据库里面的nvl
2。join中where的过滤，on里面才能起到表的过滤，放在where里面起不到提前过滤的情况；
3.left semi jioin的使用
LEFT SEMI JOIN 是 IN/EXISTS 子查询的一种更高效的实现。Hive 当前没有实现 IN/EXISTS 子查询，所以你可以用 LEFT SEMI JOIN 重写你的子查询语句。LEFT SEMI JOIN 的限制是， JOIN 子句中右边的表只能在 ON 子句中设置过滤条件，在 WHERE 子句、SELECT 子句或其他地方过滤都不行。


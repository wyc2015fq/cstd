# Kylin的cube模型 - Soul Joy Hub - CSDN博客

2016年08月31日 16:25:08[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：772


							http://www.cnblogs.com/en-heng/p/5239311.html

# [Kylin的cube模型](http://www.cnblogs.com/en-heng/p/5239311.html)

# 1. 数据仓库的相关概念

## OLAP

大部分数据库系统的主要任务是执行联机事务处理和查询处理，这种处理被称为OLTP（Online Transaction Processing, OLTP），面向的是顾客，诸如：办事员、DBA等。而数据仓库主要面向知识工人（如经理、主管等）提供数据分析处理，这种处理被称为OLAP（Online Analysis Processing）。OLTP管理的是**当前数据**，比较琐碎，很难用于做决策。而OLAP管理的是**大量历史数据**，提供汇总与聚集机制，并在不同的维度、不同的粒度存储和管理信息。
|特征|OLTP|OLAP|
|----|----|----|
|面向|办事员、DBA|知识工人|
|DB设计|基于ER，面向应用|星形/雪花，面向主题|
|数据|当前的、确保更新|历史的、跨时间维护|
|视图|详细、一般关系|汇总的、多维的|
|访问|读/写|大多数为读|
|度量|事务吞吐量|查询吞吐量、访问时间|

举个简单的例子：我们会用OLTP去管理app名称与app类别的映射关系；而分析某一周app（和app类别）的UV，则会使用OLAP；并且OLAP提供了数据的多维观察——比如：在某周在**华为手机**上top100用户的**APP**。

## Fact Table

事实表（Fact Table）是中心表，包含了大批数据并不冗余，其数据列可分为两类：
- 包含大量数据事实的列；
- 与维表（Dimension Table）的primary key相对应的foreign key。

## Lookup Table

Lookup Table包含对事实表的某些列进行扩充说明的字段。在Kylin的quick start中给出sample cube（**kylin_sales_cube**）——其Fact Table为购买记录，lookup table有两个：用于对购买日期PART_DT、商品的LEAF_CATEG_ID与LSTG_SITE_ID字段进行扩展说明。

## Dimension

维表（Dimension Table）是由fact table与lookup table**逻辑抽象**出来的表，包含了多个相关的列（即dimension），以提供对数据的多维观察；其中dimension的值的数目称为**cardinatily**。在**kylin_sales_cube**的事实表的`LSTG_FORMAT_NAME`被单独抽出来做一个dimension，可与其他维度组合分析数据。

## Star Schema

星形模式（Star Schema）包含一个或多个事实表、一组维表，其中维表的primary key与事实表的foreign key相对应。这种模式很像星光四射，维表显示在围绕事实表的射线上。下图是我根据某数据源所建立的星形模式：
![](http://images2015.cnblogs.com/blog/399159/201603/399159-20160303170323330-255885359.png)

## Cube

cube是所有的dimensions组合，任一dimensions的组合称为cuboid。因此，包含\(n\)个dimensions的cube有\(2^n\)个cuboid，如下图所示：

![](http://images2015.cnblogs.com/blog/399159/201603/399159-20160303170336627-705310588.png)

# 2. Kylin介绍

## Dimension

为了减少cuboid的数目，Kylin将Dimension分为四种类型：
- Normal，为最常见的类型，与所有其他的dimension组合构成cuboid。
- 
Mandatory，在每一次查询中都会用到dimension，在下图中A为Mandatory dimension，则与B、C总共构成了4个cuboid，相较于normal dimension的cuboid（\(2^3=8\))减少了一半。
![](http://images2015.cnblogs.com/blog/399159/201603/399159-20160303170405065-1445103588.png)

- 
Hierarchy，为带层级的dimension，比如说：省份->城市， 年->季度->月->周->日；以用于做drill down。

![](http://images2015.cnblogs.com/blog/399159/201603/399159-20160303170417784-1772210326.png)
- Derived，指该dimensions与维表的primary key是一一对应关系，可以更有效地减少cuboid数量，详细的解释参看[这里](http://kylin.apache.org/docs/howto/howto_optimize_cubes.html)；并且derived dimension只能由lookup table的列生成。

![](http://images2015.cnblogs.com/blog/399159/201603/399159-20160303170425393-1379274928.png)

然而，Kylin的Hierarchy dimensions并没有做**集合包含约束**，比如：kylin_sales_cube定义Hierarchy dimension为`META_CATEG_NAME->CATEG_LVL2_NAME->CATEG_LVL3_NAME`，但是同一个`CATEG_LVL2_NAME`可以对应不同`META_CATEG_NAME`。因此，hierarchy 显得非常鸡肋，以至于在Kylin后台处理时被废弃了（详见Li Yang在[mail group](http://apache-kylin.74782.x6.nabble.com/Hierarchy-Dimension-td1652.html)中所说）：

> 
@Julian, plan to refactor the underlying aggregation group in Q4. Will drop

hierarchy concept in the backend, however in the frontend for ease of

understanding, may still call it hierarchy.

## Measure

Measure为事实表的列度量，Kylin提供诸如：
- Sum
- Count
- Max
- Min
- Average
- Distinct Count (based on HyperLogLog)

等函数，一般配合`group by dimesion`使用。

# 3. 实战

下面的SQL语句是在**kylin_sales_cube** build成功后执行的。

sql命令`select * from kylin_sales`，得到fact table所缓存的列——均为dimension的主key、measure中所需计算的字段。

各个时间段内的销售额及购买量：

```
```
select part_dt, sum(price) as total_selled, count(distinct seller_id) as sellers 
from kylin_sales 
group by part_dt 
order by part_dt
```
```

查询某一时间的销售额及购买量,

```
```
select part_dt, sum(price) as total_selled, count(distinct seller_id) as sellers 
from kylin_sales
where  part_dt = '2014-01-01'
group by part_dt
```
```

发现报错：

```
Error while compiling generated Java code: 
public static class Record3_0 implements java.io.Serializable {            public java.math.BigDecimal f0; 
    public boolean f1; 
    public org.apache.kylin.common.hll.HyperLogLogPlusCounter f2;          public Record3_0(java.math.BigDecimal f0, boolean f1, ...
```

这是因为part_dt是date类型，在解析string到date的时候出问题，应将sql语句改为：

```
```
select part_dt, sum(price) as total_selled, count(distinct seller_id) as sellers 
from kylin_sales
where part_dt between '2014-01-01' and '2014-01-01'
group by part_dt

-- or
select part_dt, sum(price) as total_selled, count(distinct seller_id) as sellers 
from kylin_sales
where part_dt = date '2014-01-01'
group by part_dt
```
```

上面查询只用到了fact table，而没有用到lookup table。如果查询各个时间段所有二级商品类型的销售额，则需要fact table与lookup table做inner join：

```
```
select fact.part_dt, lookup.CATEG_LVL2_NAME, count(distinct seller_id) as sellers
from kylin_sales fact
inner join KYLIN_CATEGORY_GROUPINGS lookup 
on fact.LEAF_CATEG_ID = lookup.LEAF_CATEG_ID and fact.LSTG_SITE_ID = lookup.SITE_ID
group by fact.part_dt, lookup.CATEG_LVL2_NAME
order by fact.part_dt desc
```
```

# 4. 参考资料

[1] 韩家炜，《数据挖掘——概念与技术》.

[2] 教练_我要踢球, [OLAP引擎——Kylin介绍](http://blog.csdn.net/yu616568/article/details/48103415).

[3] Kylin, [Design Cube in Kylin](http://www.slideshare.net/YangLi43/design-cube-in-apache-kylin).








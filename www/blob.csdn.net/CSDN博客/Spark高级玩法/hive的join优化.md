# hive的join优化 - Spark高级玩法 - CSDN博客
2018年09月02日 00:16:14[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：121
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDd5tXcheeGtYr8aRQpSZTS7fncO0z5lyibEZI7v8YjVHkTXa79OYCZdqQ/640?wx_fmt=png)
“国际大学生节”又称“世界大学生节”、“世界学生日”、“国际学生日”。1946年，世界各国学生代表于布拉格召开全世界学生大会，宣布把每年的11月17日定为“世界大学生节”，以加强全世界大学生的团结和友谊。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDdxOq06gyribibH9twuLMzRTeJWxqepiawF1VTZcLYub1wnWQic3lrqRrSiaw/640?wx_fmt=png)
### hive Optimizer的改进
注意，本文讨论的hive join优化器是从hive 0.11.0版本起添加的，
本文描述了Hive查询执行计划的优化，以提高join效率并减少对用户提示的需求。
Hive自动识别各种用例并对其进行优化。Hive 0.11改进了这些情况的优化器：
- 
Join过程中加入有表可以存于内存。在新的优化中：
- 
该表会作为哈希表加载到内存中
- 
只需要扫描较大的表
- 
事实表在内存中的占用空间较小
- 
星型模型join
- 
许多情况下不再需要提示。
- 
优化程序会自动选择map join。
### 星型join优化
决策支持系统或数据仓库的简单模型是星型模型，其中事件在大型*事实表*中收集，而较小的支持表（*维度*表）用于描述数据。
**星型模型示例**
```
Select count(*) cnt
From store_sales ss
     join household_demographics hd on (ss.ss_hdemo_sk = hd.hd_demo_sk)
     join time_dim t on (ss.ss_sold_time_sk = t.t_time_sk)
     join store s on (s.s_store_sk = ss.ss_store_sk)
Where
     t.t_hour = 8
     t.t_minute >= 30
     hd.hd_dep_count = 2
order by cnt;
```
**原有的mapjoin**
hive支持map joins，该类型适合星型模型的场景，当然要求维度表要适合加载入内存。在0.11的发布版本之前，mapjoin可以使用优化器提示来激活：
```
select /*+ MAPJOIN(time_dim) */ count(*) from
store_sales join time_dim on (ss_sold_time_sk = t_time_sk)
```
或者，也可以通过自动转换：
```
set hive.auto.convert.join=true;
select count(*) from
store_sales join time_dim on (ss_sold_time_sk = t_time_sk)
```
hive 0.10版本的时候，hive.auto.convert.join的值是false，0.11改为了true。 
MAPJOIN通过将较小的表加载到内存中的hashmap中并在流传输时将key与较大的表匹配来处理。先前的实现有一下几个步骤：
- 
local work
- 
通过标准表扫描（包括过滤器和投影）从本地计算机上的源读取记录
- 
在内存中构建哈希表
- 
将哈希表写入本地磁盘
- 
将哈希表上传到dfs
- 
将哈希表添加到分布式缓存中
- 
map task
- 
从本地磁盘（分布式缓存）读取哈希表到内存中
- 
匹配记录的key和hash表的key
- 
组合匹配并写入输出
- 
没有reduce
**先前实现的局限性**
Hive 0.11之前的MAPJOIN实现具有以下限制：
- 
mapjoin运算符一次只能处理一个key; 也就是说，它可以执行多表连接，但前提是所有表都连接在同一个key上。（典型的星型模式连接不属于此类别。）
- 
提示对于用户来说正确使用太麻烦了，并且自动转换没有足够的逻辑来一致地预测内存是否适合MAPJOIN。
- 
除非将查询编写为级联序列`mapjoin(table, subquery(mapjoin(table, subquery....)`，否则MAPJOIN链不会合并为单个仅有map的job。自动转换从不生成单个仅有map的job。
- 
必须为每次查询运行生成mapjoin的哈希表，其中包括将所有数据下载到Hive客户端机器以及上载生成的哈希表文件。
**星形join的增强功能**
Hive 0.11中的优化器增强功能专注于有效处理星型模型中所需的join。最初的工作仅限于星型模型join，其中projection和filter后的所有维度表同时适合内存。现在也实现了只有一些维度表适合内存的情景（HIVE-3996）。
连接优化可以分为三个部分：
- 
在使用maphints时，在单个仅有map的job中执行mapjoins链。
- 
将优化扩展到自动转换的情况（在优化时生成适当的备份计划）。
- 
在任务端完全生成内存哈希表。（未来的工作。）
以下部分介绍了每种优化程序增强功能。
**mapjoin的优化链**
下面的查询会产生两个对立的仅有map 的job
```
select /*+ MAPJOIN(time_dim, date_dim) */ count(*) from
store_sales 
join time_dim on (ss_sold_time_sk = t_time_sk) 
join date_dim on (ss_sold_date_sk = d_date_sk)
where t_hour = 8 and d_year = 2002
```
但是，对于小尺寸表，所需的两个表有可能同时适合装入内存。这大大减少了执行此查询所需的时间，因为事实表只读取一次而不是读取两次并将其写入HDFS以在job之间进行通信。
###### 当前和未来的优化
- 
将M * -MR模式合并为单个MR。
- 
尽可能将MJ-> MJ合并为单个MJ。
- 
将MJ *模式合并为单个Map stage，作为一个MJ运算符链。（尚未实现。）
如果`hive.auto.convert.join`设置为true，则优化器不仅会将连接转换为mapjoins，还会尽可能地合并MJ *模式。
**优化自动join转换**
当自动join启动了，query中就不需要map-join的提示了。配置下面两个参数可以启自动join选项
```
set hive.auto.convert.join.noconditionaltask = true;
set hive.auto.convert.join.noconditionaltask.size = 10000000;
```
**第一个配置是指定默认值。**
在大小配置使用户能够控制哪些尺寸表可以存放在内存。此值表示可以转换为适合内存的哈希映射的表大小的总和。目前，n-1个连接表必须适合装入内存才能使map join优化生效。没有检查表是否是压缩表以及表的潜在大小。这个假设对结果的影响将在下一节中讨论。
例如，上面的例子可以简单写为：
```
select count(*) from
store_sales 
join time_dim on (ss_sold_time_sk = t_time_sk)
join date_dim on (ss_sold_date_sk = d_date_sk)
where t_hour = 8 and d_year = 2002
```
如果time_dim和date_dim符合所提供的大小配置，则相应的join将转换为map-join。如果表的大小总和可以小于配置的大小，则组合两个map join，从而产生单个map join。这减少了所需的MR作业数量，并显着提高了此查询的执行速度。此示例也可以轻松扩展为多join，并且可以按预期工作。
Outer joins 提供了更多挑战。由于map-join运算符只能流式传输一个表，因此流式表必须是所有行都需要的表。对于 left outer join，这是连接左侧的表; 对于 right outer join，右侧的表等。这意味着即使inner join可以转换为map join，outer join也无法转换为map join。在除了需要流式传输之外的表都小于配置的阈值大小的情况下，outer join可以转化为外部链接。full outer join无法转换为map-join，因为两个表都需要进行流式处理。
**自动join转换也会影响sort-merge-bucket join。**
Hive0.13.0 及更高版本引入`hive.auto.convert.join.use.nonstaged`，默认为false。
对于条件join，如果来自小别名的输入流可以直接应用于join运算符而不进行filter或projection，则无需通过MapReduce本地任务在分布式缓存中预先划分stage（pre-staged）。设置`hive.auto.convert.join.use.nonstaged`为true可避免在这些情况下进行预先分stage。
###### 当前优化
1.将尽可能多的MJ操作分组为一个MJ。
当Hive根据配置转换join为map-join时，会在这些转换结束时尽可能地将尽可能多的join组合在一起。如果参与各个map join的表的大小总和在`noConditionalTask.size`配置的限制范围内，则将这些MJ运算符组合在一起，按顺序进行。这确保了关于这些查询的更多加速。
##### 自动转换为SMB map join
Sort-Merge-Bucket（SMB）join也可以转换为SMB map join。在表进行排序和分桶的任何地方都可以使用SMB连接。join归结为仅合并已经排序的表，允许此操作比普通的map-join更快。但是，如果表是分区的，则可能会减慢速度，因为每个mapper需要获得一个具有单个key分区的非常小块。
以下配置设置可以将SMB转换为map join SMB：
```
set hive.auto.convert.sortmerge.join = true;
set hive.optimize.bucketmapjoin = true;
set hive.optimize.bucketmapjoin.sortedmerge = true;
```
可以使用以下配置设置大表选择策略：
```
hive.auto.convert.sortmerge.join.bigtable.selection.policy 
    = org.apache.hadoop.hive.ql.optimizer.TableSizeBasedBigTableSelectorForAutoSMJ;
```
默认情况下，选择策略是平均分区大小。与hash表和流式传输表相比，大表选择策略有助于确定仅选择哪个表进行流式传输。
可用的选择政策是：
```
org.apache.hadoop.hive.ql.optimizer.AvgPartitionSizeBasedBigTableSelectorForAutoSMJ（默认）
org.apache.hadoop.hive.ql.optimizer.LeftmostBigTableSelectorForAutoSMJ
org.apache.hadoop.hive.ql.optimizer.TableSizeBasedBigTableSelectorForAutoSMJ
```
类名称描述了它们的用途。这对于事实 - 事实连接（TPC DS基准中的查询82）特别有用。
###### SMB使用不同的key跨表连接
如果表具有不同数量的key，例如表A有2个SORT列，而表B有1个SORT列，那么可能会遇到索引超出范围的异常。
以下查询导致索引超出范围异常，因为emp_person让我们说例如有1个排序列，而emp_pay_history有2个排序列。
**错误Hive 0.11**
```
`SELECTp.*, py.*
FROMemp_person p ``INNERJOINemp_pay_history py
ON   p.empid = py.empid
```
下面就可以正常运行
**Hive 0.11**
```
SELECTp.*, py.*
FROMemp_pay_history py ``INNERJOINemp_person p
ON   p.empid = py.empid
```
#### 在任务端生成哈希表
未来的工作将使在任务端完全生成内存中的哈希表成为可能。
##### 客户端哈希表的优缺点
在客户端上生成哈希表（或多个join的多个哈希表）具有缺点。（*客户端机器是用于运行Hive客户端并提交作业的主机。）
- 
**数据位置：**客户端所在机器通常不是数据节点。访问的所有数据都是远程的，必须通过网络读取。
- 
**规格：**出于同样的原因，不清楚运行此处理的机器的规格是什么。它可能在任务节点没有的内存，硬盘驱动器或CPU方面存在限制。
- 
**HDFS上传：必须**将数据带回群集并通过分布式缓存进行复制以供任务节点使用。
在客户端计算机上预处理哈希表也有一些好处：
- 
存储在分布式缓存中的内容可能小于原始表（filter和projection）。
- 
相反，使用分布式缓存直接在任务节点上加载哈希表意味着缓存中的较大对象，这可能会减少使用MAPJOIN的机会。
##### 哈希表的任务端生成
当在任务端完全生成哈希表时，所有任务节点都必须访问原始数据源以生成哈希表。由于在正常情况下这将并行发生，因此不会影响延迟，但Hive具有存储处理程序的概念，并且许多任务访问相同的外部数据源（HBase，数据库等）可能会对数据源造成性能压力。
###### 进一步的优化选项
- 
增加维度表上的复制因子。
- 
使用分布式缓存来保存维度表。
**推荐阅读：**
[重磅：关于hive的join使用必须了解的事情](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484426&idx=1&sn=50613f6f4f6fda4a139b01d2f04656d6&chksm=9f38e722a84f6e34c4c8df1d9849fda384a0e7659ac383b378ea9948c522b98c8a5462250a60&scene=21#wechat_redirect)
[经验|如何设置Spark资源](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485250&idx=1&sn=f9a8a27d6b966fb53725505072f50f8f&chksm=9f38e46aa84f6d7ca50bec7c593f73ddc064af6e598cde4ea9ed85b91aef89e0f55065c4bad2&scene=21#wechat_redirect)
[你真知道如何高效用mapPartitions吗？](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485220&idx=1&sn=e4965b088e1d55914a235f84c2f01908&chksm=9f38e40ca84f6d1ae4b2e5fbac7f78612a40f0d948ca4104ac1bfbe91df021d63ee9dbeb14a6&scene=21#wechat_redirect)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWEbA0t2WgVoUbCSPAic4uAuwkHOg8iczLJQ4gpogt2Gy2FelL1iaM9jW6IPAtjuB4c3ZmfmNb1vRy2w/640?wx_fmt=jpeg)

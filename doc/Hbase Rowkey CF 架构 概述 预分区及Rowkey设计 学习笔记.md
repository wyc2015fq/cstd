# Hbase Rowkey CF 架构 概述 预分区及Rowkey设计 学习笔记

2019年01月11日 18:12:20

小鹅鹅

阅读数：71

更多

 								所属专栏： 																[Spark / Hadoop生态系列](https://blog.csdn.net/column/details/31206.html) 																 							

 									

​                   					 					版权声明：本文为博主原创文章，转载请附http://blog.csdn.net/asd136912。					https://blog.csdn.net/asd136912/article/details/86310966				

## Hbase Rowkey CF 概述 预分区及Rowkey设计 学习笔记

### 1. 概述

HBase是建立在Hadoop文件系统之上的分布式面向列的数据库。

HBase类似于数据库的存储层，HBase适用于结构化存储，并且为列式分布式数据库。

HBase是一个数据模型，类似于Google Big Table设计，可以提供快速随机访问海量结构化数据。它利用了Hadoop的文件系统（HDFS）提供的容错能力。

HBase属于Hadoop的生态系统，提供对数据的随机实时读/写访问，是Hadoop文件系统的一部分。

我们可以直接或通过HBase的存储HDFS数据。使用HBase在HDFS读取消费/随机访问数据。

HBase在Hadoop的文件系统之上，并提供了读写访问。
 ![img](https://img-blog.csdnimg.cn/2019010817194848.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==,size_16,color_FFFFFF,t_70)

#### 1.1 优势

Hbase适合存储PB级别的海量数据，在PB级别的数据以及采用廉价PC存储的情况下，能在几十到百毫秒内返回数据。

这与Hbase的极易扩展性息息相关。正式因为Hbase良好的扩展性，才为海量数据的存储提供了便利。

#### 1.2 使用场景

- 是巨量大（百T、PB级别）
- 查询简单（基于rowkey或者rowkey范围查询）
- 不涉及到复杂的关联

有几个典型的场景特别适合使用Hbase来存储：

- 海量订单流水数据（长久保存）
- 交易记录
- 数据库历史数据

#### 1.3 HBase架构

![img](https://img-blog.csdnimg.cn/20190111181051856.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==,size_16,color_FFFFFF,t_70)

### 2. 存储机制

HBase是一个面向列的数据库，在表中它由行排序。表模式定义只能列族，也就是键值对。一个表有多个列族以及每一个列族可以有任意数量的列。后续列的值连续地存储在磁盘上。表中的每个单元格值都具有时间戳。总之，在一个HBase：

- 表是行的集合
- 行是列族的集合
- 列族是列的集合
- 列是键值对的集

这里的列式存储其实说的是列族存储，Hbase是根据列族来存储数据的。列族下面可以有非常多的列，列族在创建表的时候就必须指定。为了加深对Hbase列族的理解，下面是一个简单的关系型数据库的表和Hbase数据库的表：

RDBMS的表：

![img](https://img-blog.csdnimg.cn/20190111174821179.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==,size_16,color_FFFFFF,t_70)

Hbase的表：

![img](https://img-blog.csdnimg.cn/20190111174829372.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==,size_16,color_FFFFFF,t_70)

我们再从逻辑视图与物理存储视图来看表结构：
 ![img](https://img-blog.csdnimg.cn/20190111213720181.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==,size_16,color_FFFFFF,t_70)
 **所以我们需要通过Rowkey+CF+Timestamp/version 才能定位到具体的单元格**

下图是针对Hbase和关系型数据库的基本的一个比较：

| 区别       | HBase                                              | RDBMS                                   |
| ---------- | -------------------------------------------------- | --------------------------------------- |
| 模式       | HBase无模式，它不具有固定列模式的概念;仅定义列族。 | RDBMS有它的模式，描述表的整体结构的约束 |
| 可伸缩性   | 专门创建为宽表，横向扩展，可伸缩性好               | 细而专为小表，很难形成规模              |
| 事务       | 无                                                 | RDBMS是事务性的                         |
| 规范化     | 反规范化的数据                                     | 具有规范化的数据                        |
| 结构化数据 | 用于半结构以及结构化数据                           | 用于结构化数据非常好                    |
| 数据类型   | 字符串                                             | 丰富的数据类型                          |
| 数据操作   | 简单增删改查                                       | 丰富                                    |
| 存储模式   | 列存储                                             | 行存储                                  |
| 数据保护   | 保留                                               | 替换                                    |

#### 2.1 Column Family(CF)

Column Family(CF)又叫列族，Hbase通过列族划分数据的存储，列族下面可以包含任意多的列，实现灵活的数据存取。刚接触的时候，理解起来有点吃力。我想到了一个非常类似的概念，理解起来就非常容易了。**那就是家族的概念，我们知道一个家族是由于很多个的家庭组成的。列族也类似，列族是由一个一个的列组成（任意多）。**

Hbase表的创建的时候就必须指定列族。就像关系型数据库创建的时候必须指定具体的列是一样的。

**Hbase的列族不是越多越好**，官方推荐的是列族最好小于或者等于3。我们使用的场景一般是1个列族。

#### 2.2 Rowkey

Rowkey的概念和mysql中的主键是完全一样的，Hbase使用Rowkey来唯一的区分某一行的数据。

由于Hbase只支持3中查询方式：

- 基于Rowkey的单行查询
- 基于Rowkey的范围扫描
- 全表扫描

因此，Rowkey对Hbase的性能影响非常大，Rowkey的设计就显得尤为的重要。设计的时候要兼顾基于Rowkey的单行查询也要键入Rowkey的范围扫描。下一节会详细描述。

#### 2.3 Region

Region的概念和关系型数据库的分区或者分片差不多。
 Hbase会将一个大表的数据基于Rowkey的不同范围分配到不通的Region中，每个Region负责一定范围的数据访问和存储。这样即使是一张巨大的表，由于被切割到不通的region，访问起来的时延也很低。

#### 2.4 TimeStamp

TimeStamp对Hbase来说至关重要，因为它是实现Hbase多版本的关键。在Hbase中使用不同的timestame来标识相同rowkey行对应的不通版本的数据。

在写入数据的时候，如果用户没有指定对应的timestamp，Hbase会自动添加一个timestamp，timestamp和服务器时间保持一致。
 在Hbase中，相同rowkey的数据按照timestamp倒序排列。默认查询的是最新的版本，用户可同指定timestamp的值来读取旧版本的数据。

### 3. 预分区与rowkey设计(如何解决数据倾斜和热点问题)

HBase中的行是按照rowkey的字典顺序排序的，这种设计优化了scan操作，可以将相关的行以及会被一起读取的行存取在临近位置，便于scan。

**rowkey设计是热点的源头。**

HBase中，表会被划分为1…n个Region，被托管在RegionServer中。Region有二个重要的属性:StartKey与EndKey表示这个Region维护的rowKey范围，当我们要读/写数据时，如果rowKey落在某个start-end  key范围内，那么就会定位到目标region并且读/写到相关的数据。

默认的情况下，创建一张表是，只有1个region，start-end  key没有边界，所有数据都在这个region里装，然而，当数据越来越多，region的size越来越大时，大到一定的阀值，hbase认为再往这个region里塞数据已经不合适了，就会找到一个midKey将region一分为二，成为2个region,这个过程称为分裂(region-split)。而midKey则为这二个region的临界。

此时，我们假设假设rowkey小于midKey则为阴被塞到1区，大于等于midKey则会被塞到2区，如果rowkey还是顺序增大的，那数据就总会往2区里面写数据，而1区现在处于一个被冷落的状态，而且是半满的。2区的数据满了会被再次分裂成2个区，如此不断产生被冷落而且不满的Region，当然，这些region有提供数据查询的功能。

这种设计是分布式系统一个很大的弊端，而且这样导致数据倾斜和热点问题，从而导致集群的资源得不到很好的利用。

#### 3.1 预分区设计

预分区，让表的数据可以均衡的分散在集群中，而不是默认只有一个region分布在集群的一个节点上。**（预分区个数=节点的倍数，看数据量估算，region不足了会被分列，预分区后每个region的rowkey还是有序的）**

一个RegionServer能管理10-1000个Region，0.92.x版本后，默认的Region大小为10G，向下可以支持256MB，向上可以支持到20G，也就是说，**每个RegionServer能管理的数据量为2.5GB-20TB。**

如果有5个节点，3年内数据量为5T，那么分区数可以预设为：
 5000G/10G=500个region
 这500个Region就会被均衡的分布在集群各个节点上（具体分布看机器的性能和存储空间而定），机器硬盘不足可以添加硬盘，性能不足可以添加新节点（添加新机器）。

#### 3.2 Rowkey设计

##### 3.2.1 Rowkey长度原则（最好不超过16字节）

Rowkey是一个二进制码流，Rowkey的长度被很多开发者建议说设计在10~100个字节，不过建议是越短越好，不要超过16个字节。
 原因如下：

1. 数据的持久化文件HFile中是按照KeyValue存储的，如果Rowkey过长比如100个字节，1000万列数据光Rowkey就要占用100*1000万=10亿个字节，将近1G数据，这会极大影响HFile的存储效率；
2. MemStore将缓存部分数据到内存，如果Rowkey字段过长内存的有效利用率会降低，系统将无法缓存更多的数据，这会降低检索效率。因此Rowkey的字节长度越短越好。
3. 目前操作系统是都是64位系统，内存8字节对齐。控制在16个字节，8字节的整数倍利用操作系统的最佳特性。

##### 3.2.2 rowkey散列原则

把主键哈希后当成rowkey的头部

##### 3.2.3 rowkey唯一原则

必须在设计上保证其唯一性，rowkey是按照字典顺序排序存储的，因此，设计rowkey的时候，要充分利用这个排序的特点，将经常读取的数据存储到一块，将最近可能会被访问的数据放到一块。

##### 3.4.4 时间戳反转

如果数据需要保留多个版本，可以使用反转的时间戳作为rowkey的一部分，用 Long.Max_Value - timestamp  追加到key的末尾，例如 [key][reverse_timestamp] , [key] 的最新值可以通过scan  [key]获得[key]的第一条记录，因为HBase中rowkey是有序的，第一条记录是最后录入的数据。

##### 3.4.5 整个rowkey如下（timestamp并不是必要的，视业务而定）

```
rowkey=哈希（主键<递增的id\手机号码等>）+Long.Max_Value - timestamp
1
```

### 4. HBase架构

这里我们了解下 HBase 都有哪些模块，以及大致的工作流程。前面我们提到过 HBase 也是构建于 HDFS  之上，这是正确的，但也不是完全正确。HBase 其实也支持直接在本地文件系统之上运行，不过这样的 HBase  只能运行在一台机器上，那么对于分布式大数据的环境是没有意义的（这也是所谓的 HBase 的单机模式）。一般只用于测试或者验证某一个 HBase  的功能，后面我们在详细的介绍 HBase 的几种运行模式。这里我们只要记得在分布式的生产环境中，HBase 需要运行在 HDFS 之上，以  HDFS 作为其基础的存储设施。HBase 上层提供了访问的数据的 Java API 层，供应用访问存储在 HBase 的数据。在 HBase  的集群中主要由 Master 和 Region Server 组成，以及 Zookeeper，具体模块如下图所示。
 ![img](https://img-blog.csdnimg.cn/20190111214206845.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==,size_16,color_FFFFFF,t_70)
 接下来，我们简单的一一介绍下 HBase 中相关模块的作用。

- **Master**
   HBase Master 用于协调多个 Region Server，侦测各个 Region Server 之间的状态，并平衡 Region  Server 之间的负载。HBase Master 还有一个职责就是负责分配 Region 给 Region Server。HBase 允许多个  Master 节点共存，但是这需要 Zookeeper 的帮助。不过当多个 Master 节点共存时，只有一个 Master  是提供服务的，其他的 Master 节点处于待命的状态。当正在工作的 Master 节点宕机时，其他的 Master 则会接管 HBase  的集群。
- **Region Server**
   对于一个 Region Server 而言，其包括了多个 Region。Region Server  的作用只是管理表格，以及实现读写操作。Client 直接连接 Region Server，并通信获取 HBase 中的数据。对于 Region  而言，则是真实存放 HBase 数据的地方，也就说 Region 是 HBase 可用性和分布式的基本单位。如果当一个表格很大，并由多个 CF  组成时，那么表的数据将存放在多个 Region 之间，并且在每个 Region 中会关联多个存储的单元（Store）。
- **Zookeeper**
   对于 HBase 而言，Zookeeper 的作用是至关重要的。首先 Zookeeper 是作为 HBase Master 的 HA  解决方案。也就是说，是 Zookeeper 保证了至少有一个 HBase Master 处于运行状态。并且 Zookeeper 负责  Region 和 Region Server 的注册。其实 Zookeeper  发展到目前为止，已经成为了分布式大数据框架中容错性的标准框架。不光是 HBase，几乎所有的分布式大数据相关的开源框架，都依赖于  Zookeeper 实现 HA。

一个完整分布式的 HBase 的工作原理示意图如下：
 ![img](https://img-blog.csdnimg.cn/20190111214252319.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==,size_16,color_FFFFFF,t_70)
 首先我们需要知道 HBase 的集群是通过 Zookeeper 来进行机器之前的协调，也就是说 HBase Master 与 Region Server 之间的关系是依赖 Zookeeper 来维护。

当一个 Client 需要访问 HBase 集群时，Client 需要先和 Zookeeper 来通信，然后才会找到对应的 Region  Server。每一个 Region Server 管理着很多个 Region。对于 HBase 来说，Region 是 HBase  并行化的基本单元。因此，数据也都存储在 Region 中。这里我们需要特别注意，每一个 Region 都只存储一个 Column Family  的数据，并且是该 CF 中的一段（按 Row 的区间分成多个 Region）。

Region 所能存储的数据大小是有上限的，当达到该上限时（Threshold），Region 会进行分裂，数据也会分裂到多个 Region 中，这样便可以提高数据的并行化，以及提高数据的容量。每个 Region 包含着多个 Store 对象。

每个 Store 包含一个 MemStore，和一个或多个 HFile。MemStore  便是数据在内存中的实体，并且一般都是有序的。当数据向 Region 写入的时候，会先写入 MemStore。当 MemStore  中的数据需要向底层文件系统倾倒（Dump）时（例如 MemStore 中的数据体积到达 MemStore 配置的最大值），Store 便会创建  StoreFile，而 StoreFile 就是对 HFile 一层封装。所以 MemStore 中的数据会最终写入到 HFile  中，也就是磁盘 IO。
 由于 HBase 底层依靠 HDFS，因此 HFile 都存储在 HDFS 之中。这便是整个 HBase 工作的原理简述。

### 5. Hbase的写逻辑

Hbase的写逻辑涉及到写内存、写log、刷盘等操作，看起来简单，其实里面又有很多的逻辑，下面就来做详细的介绍

#### 5.1 Hbase写入逻辑

Hbase的写入流程如下图所示：
 ![img](https://img-blog.csdnimg.cn/20190111212459813.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==,size_16,color_FFFFFF,t_70)

从上图可以看出氛围3步骤：

- 第1步：Client获取数据写入的Region所在的RegionServer
- 第2步：请求写Hlog
- 第3步：请求写MemStore

只有当写Hlog和写MemStore都成功了才算请求写入完成。MemStore后续会逐渐刷到HDFS中。

备注：Hlog存储在HDFS，当RegionServer出现异常，需要使用Hlog来恢复数据。

更多细节详见Ref

### Ref

1. Google Big Table
2. <https://www.ibm.com/developerworks/cn/analytics/library/ba-cn-bigdata-hbase/index.html>
3. <https://www.yiibai.com/hbase>
4. <https://cloud.tencent.com/developer/article/1006043>
5. <https://blog.csdn.net/weixin_41279060/article/details/78855679>
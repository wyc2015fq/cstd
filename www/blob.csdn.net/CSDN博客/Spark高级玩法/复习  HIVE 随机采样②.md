# 复习 | HIVE 随机采样② - Spark高级玩法 - CSDN博客
2018年08月30日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：279
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDd5tXcheeGtYr8aRQpSZTS7fncO0z5lyibEZI7v8YjVHkTXa79OYCZdqQ/640?wx_fmt=png)
前面一篇文章，三种采样机制的其中一种
[复习:聊聊hive随机采样①](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485296&idx=1&sn=54927ffb2110be75cbffe4660a8fdecf&chksm=9f38e458a84f6d4e19fb0123872deec76a55b2354b372fde00042ff7a963e6f7666938e4d7ae&scene=21#wechat_redirect)
今天将剩下的两种，**分桶采样**和**数据块采样**。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDdxOq06gyribibH9twuLMzRTeJWxqepiawF1VTZcLYub1wnWQic3lrqRrSiaw/640?wx_fmt=png)
当数据量特别大时，对全体数据进行处理存在困难时，抽样就显得尤其重要了。抽样可以从被抽取的数据中估计和推断出整体的特性，是科学实验、质量检验、社会调查普遍采用的一种经济有效的工作和研究方法。
Hive支持桶表抽样和块抽样，下面分别学习。所谓桶表指的是在创建表时使用CLUSTERED BY子句创建了桶的表。桶表抽样的语法如下：
`table_sample: TABLESAMPLE (BUCKET x OUT OF y [ON colname])`
TABLESAMPLE子句允许用户编写用于数据抽样而不是整个表的查询，该子句出现FROM子句中，可用于任何表中。桶编号从1开始，colname表明抽取样本的列，可以是非分区列中的任意一列，或者使用rand()表明在整个行中抽取样本而不是单个列。在colname上分桶的行随机进入1到y个桶中，返回属于桶x的行。下面的例子中，返回32个桶中的第3个桶中的行：
```
SELECT *
FROM source TABLESAMPLE(BUCKET 3 OUT OF 32 ON rand()) s;
```
通常情况下，TABLESAMPLE将会扫描整个表然后抽取样本，显然这种做法效率不是很高。替代方法是，由于在使用CLUSTERED BY时指定了分桶的列，如果抽样时TABLESAMPLE子句中指定的列匹配CLUSTERED BY子句中的列，TABLESAMPLE只扫描表中要求的分区。假如上面的例子中，source表在创建时使用了CLUSTEREDBY id INTO 32 BUCKETS，那么下面的语句将返回第3个和第19个簇中的行，因为每个桶由(32/16)=2个簇组成。为什么是3和19呢，因为要返回的是第3个桶，而每个桶由原来的2个簇组成，第3个桶就由原来的第3个和19个簇组成，根据简单的哈希算法（3%16=19%16）。
`TABLESAMPLE(BUCKET 3 OUT OF 16 ON id)`
相反，下面的语句将会返回第3个簇的一半，因为每个桶由(32/64)=1/2个簇组成。
`TABLESAMPLE(BUCKET 3 OUT OF 64 ON id)`
从Hive-0.8开始可以使用块抽样，语法为：
`block_sample: TABLESAMPLE (n PERCENT)`
该语句允许抽取数据大小的至少n%（不是行数，而是数据大小）做为输入，支持CombineHiveInputFormat而一些特殊的压缩格式是不能够被处理的，如果抽样失败，MapReduce作业的输入将是整个表。由于在HDFS块层级进行抽样，所以抽样粒度为块的大小，例如如果块大小为256MB，即使输入的n%仅为100MB，也会得到256MB的数据。下面的例子中输入的0.1%或更多将用于查询：
```
SELECT *
FROM source TABLESAMPLE(0.1 PERCENT) s;
```
如果希望在不同的块中抽取相同的数据，可以改变下面的参数：
`set hive.sample.seednumber=<INTEGER>;`
也可以指定读取数据的长度，该方法与PERCENT抽样具有一样的限制，为什么有相同的限制，是因为该语法仅将百分比改为了具体值，但没有改变基于块抽样这一前提条件。该语法为：
```
block_sample: TABLESAMPLE (ByteLengthLiteral)
ByteLengthLiteral : (Digit)+ ('b' | 'B' | 'k' | 'K' | 'm' | 'M' | 'g' | 'G')
```
下面的例子中输入的100M或更多将用于查询：
```
SELECT *
FROM source TABLESAMPLE(100M) s;
```
Hive也支持基于行数的输入限制，当效果与上面介绍的两个不同。首先不需要CombineHiveInputFormat，这意味着可以被用在非原生表中。其次行数被用在每个split中。因此总的行数根据输入的split数而变化很大。语法格式为：
`block_sample: TABLESAMPLE (n ROWS)`
例如下面的查询将从每个split中抽取10行：
`SELECT * FROM source TABLESAMPLE(10 ROWS);`
更多大数据知识，大数据学习技巧，spark 调优，源码等，欢迎加入**知识星球**。
**推荐阅读:**
[一文详解scala泛型及类型限定](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485264&idx=1&sn=e036a12fdc931ebc36d5b270fd609980&chksm=9f38e478a84f6d6e79d43128a2636fd9cbc0db94198666a7baff6910d6860a353f08cdbbccdc&scene=21#wechat_redirect)
[一文详解scala泛型及类型限定](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485264&idx=1&sn=e036a12fdc931ebc36d5b270fd609980&chksm=9f38e478a84f6d6e79d43128a2636fd9cbc0db94198666a7baff6910d6860a353f08cdbbccdc&scene=21#wechat_redirect)
[重要 | mr使用hcatalog读写hive表](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485234&idx=1&sn=a4a2795b320acdbe6f8767e4d145549f&chksm=9f38e41aa84f6d0c0b3fffad5212ae0ea3501b974fc6ce81856f2de06bb6162056ad644c2a6e&scene=21#wechat_redirect)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWEbA0t2WgVoUbCSPAic4uAuwkHOg8iczLJQ4gpogt2Gy2FelL1iaM9jW6IPAtjuB4c3ZmfmNb1vRy2w/640?wx_fmt=jpeg)

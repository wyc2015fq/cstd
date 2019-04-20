# 基于Alluxio系统的Spark DataFrame高效存储管理技术 - Spark高级玩法 - CSDN博客
2019年01月19日 23:38:08[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：67
## **转自Alluxio**
## 介绍
越来越多的公司和组织开始将Alluxio和Spark一起部署从而简化数据管理，提升数据访问性能。Qunar最近将Alluxio部署在他们的生产环境中，从而将Spark streaming作业的平均性能提升了15倍，峰值甚至达到300倍左右。在未使用Alluxio之前，他们发现生产环境中的一些Spark作业会变慢甚至无法完成。而在采用Alluxio后这些作业可以很快地完成。在这篇文章中，我们将介绍如何使用Alluxio帮助Spark变得更高效，具体地，我们将展示如何使用Alluxio高效存储Spark DataFrame。
## Alluxio和Spark缓存
用户使用Alluxio存储Spark DataFrame非常简单：通过Spark DataFrame write API将DataFrame作为一个文件写入Alluxio。通常的做法是使用df.write.parquet()将DataFrame写成parquet文件。在DataFrame对应的parquet文件被写入Alluxio后，在Spark中可以使用sqlContext.read.parquet()读取。为了分析理解使用Alluxio存储DataFrame和使用Spark内置缓存存储DataFrame在性能上差异，我们进行了如下的一些实验。
实验相关设置如下：
- 
硬件配置：单个worker安装在一个节点上，节点配置：61 GB内存 + 8核CPU；
- 
软件版本：Spark 2.0.0和Alluxio1.2.0，参数均为缺省配置；
- 
运行方式：以standalone模式运行Spark和Alluxio。
- 
在本次实验中，我们使用Spark内置的不同缓存级别存储DataFrame对比测试使用Alluxio存储DataFrame，然后收集分析性能测试结果。同时通过改变DataFrame的大小来展示存储的DataFrame的规模对性能的影响。
## 存储DataFrame
Spark DataFrame可以使用persist() API存储到Spark缓存中。persist()可以缓存DataFrame数据到不同的存储媒介。
本次实验使用了以下Spark缓存存储级别（StorageLevel）：
- 
MEMORY_ONLY：在Spark JVM内存中存储DataFrame对象
- 
MEMORY_ONLY_SER：在Spark JVM内存中存储序列化后的DataFrame对象
- 
DISK_ONLY: 将DataFrame数据存储在本地磁盘
- 
下面是一个如何使用persist() API缓存DataFrame的例子:
`df.persist(MEMORY_ONLY)`
将DataFrame保存在内存中的另一种方法是将DataFrame作为一个文件写入Alluxio。Spark支持将DataFrame写成多种不同的文件格式，在本次实验中，我们将DataFrame写成parquet文件。
下面是一个将DataFrame写入Alluxio的例子：
## 查询存储在Alluxio上的DataFrame
DataFrame被保存后（无论存储在Spark内存还是Alluxio中），应用可以读取DataFrame以进行后续的计算任务。本次实验中，我们创建了一个包含2列的DataFrame（这2列的数据类型均为浮点型），计算任务则是分别计算这2列数据之和。
当DataFrame存储在Alluxio时，Spark读取DataFrame就像从Alluxio中读取文件一样简单。下面是一个从Alluxio中读取DataFrame的例子：
```
df = sqlContext.read.parquet(alluxioFile)
df.agg(sum("s1"), sum("s2")).show()
```
我们分别从Alluxio中 parquet文件以及各种Spark存储级别缓存中读取DataFrame，并进行上述的聚合计算操作。下图显示了不同存储方案中的聚合操作的完成时间。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/uJfRNxceUSVNdXHkjKdvLpXD4m9K462sfnYnnptDLyhulH9RoMnTEYKYZwxiaktPZEBXbHH5KSJ2rbhdtwMSctA/640?wx_fmt=png)
从上图可以看出，从Alluxio中读取DataFrame进行聚合操作具有比较稳定的执行性能。对于从Spark缓存中读取DataFrame，在DataFrame规模较小时执行性能具有一定优势，但是随着DataFrame规模的增长，性能急剧下降。在本文的实验环境中，对于各种Spark内置的存储级别， DataFrame规模达到20 GB以后，聚合操作的性能下降比较明显。
另一方面，相比使用Spark内置缓存，使用Alluxio存储DataFrame并进行聚合操作，其性能在小规模数据上略有劣势。然而，随着DataFrame数据规模的增长，从Alluxio中读取DataFrame性能更好，因为从Alluxio中读取DataFrame的耗时几乎始终随着数据规模线性增长。由于使用Alluxio存储DataFrame的读写性能具有较好的线性可扩展性，上层应用可以稳定地以内存速度处理更大规模的数据。
## 使用Alluxio共享存储的DataFrame
使用Alluxio存储DataFrame的另一大优势是可以在不同Spark应用或作业之间共享存储在Alluxio中的数据。当一个DataFrame文件被写入Alluxio后，它可以被不同的作业、SparkContext、甚至不同的计算框架共享。因此，如果一个存储在Alluxio中的DataFrame被多个应用频繁地访问，那么所有的应用均可以从Alluxio内存中直接读取数据，并不需要重新计算或者从另外的底层外部数据源中读取数据。
为了验证采用Alluxio共享内存的优势，我们在如上述的同样的实验环境中进行相同规模的DataFrame聚合操作。当使用50 GB规模的DataFrame时，我们在单个Spark应用中进行聚合操作，并且记录该聚合操作的耗时。没有使用Alluxio时，Spark应用需要每次都从数据源读取数据(在本次实验中是一个本地SSD)。在使用Alluxio时，数据可以直接从Alluxio内存中读取。下图显示了2次聚合操作的完成时间性能对比。使用Alluxio的情况下，聚合操作快了约2.5倍。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/uJfRNxceUSVNdXHkjKdvLpXD4m9K462sASfX74RMMFNPNSaNbia6ZgzktbJBdbqkVXLia6Q2NdovPR80cl9q4wnQ/640?wx_fmt=png)
在上图的实验中，数据源是本地SSD。如果DataFrame来自访问起来更慢或不稳定的数据源，Alluxio的优势就更加明显了。举例而言，下图是DataFrame数据源由本地SSD替换为某公有云存储的实验结果。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/uJfRNxceUSVNdXHkjKdvLpXD4m9K462sxXCxFK2uj3OrlG4df2DBaNcReInAvlFSiay9zOjPNbUpXsAHcUmic6tg/640?wx_fmt=png)
这张图显示是执行7次聚合操作的平均完成时间。图中的红色的误差范围（error bar）代表完成时间的最大和最小范围。这些结果清晰地显示出Alluxio可以显著提升操作的平均性能。这是因为使用Alluxio缓存DataFrame时，Spark可以直接从Alluxio内存中读取DataFrame，而不是从远程的公有云存储中。平均而言，Alluxio可以加速上述DataFrame的聚集操作性能超过10倍。
另一方面，由于数据源是公有云系统，Spark必须跨网络远程读取数据。错综复杂的网络状况会导致读取性能难以预测。这种性能的不稳定性从上图中的误差范围（error bar）可以很明显地看出。在不使用Alluxio的情况下，Spark作业的完成时间变化范围超过1100秒。当使用Alluxio之后，完成时间的变化范围只有10秒。在本实验中，Alluxio能够将数据读取造成的不稳定性降低超过100倍。
由于共有云存储系统的网络访问性能不可预测性，最慢的Spark作业执行时间超过1700秒, 比平均慢2倍。然而，当使用Alluxio时，最慢的Spark作业执行时间大约比平均时间只慢6秒。因此，如果以最慢的Spark作业执行时间来评估，Alluxio可以加速DataFrame聚合操作超过17倍。
## 结论
Alluxio可以在多个方面帮助Spark变得更高效。这篇文章介绍了如何使用Alluxio存储Spark DataFrame，并且实验验证了采用Alluxio带来的优势：
- 
Alluxio可以直接在内存中保存大规模的数据来加速Spark应用；
- 
Alluxio能够在多个Spark应用之间快速共享存储在内存中的数据；
- 
Alluxio可以提供稳定和可预测的数据访问性能。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)

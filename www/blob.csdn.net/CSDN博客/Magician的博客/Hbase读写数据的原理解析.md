
# Hbase读写数据的原理解析 - Magician的博客 - CSDN博客


2018年03月29日 21:44:58[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：2124所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)



[目录](#目录)
[1、体系图](#1体系图)[写数据的流程（参考上图）：](#写数据的流程参考上图)
[读数据的流程（参考下图）：](#读数据的流程参考下图)


# 目录
# 1、体系图
![这里写图片描述](https://img-blog.csdn.net/20180329212149584?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180329212149584?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**针对上图的一些解释：**
这里面数据**分区（region）存储是为了查询方便（即因为是集群所以能充分利用磁盘的IO性）**。添加数据时，**数据先进入Hlog–预写日志（数据只能追加不能修改）<防止数据丢失>，数据在Hlog写完后再写到内存中**。
HFile：认为是将数据进行序列化。
StoreFile：认为是一个文件。
DFS:调用HDFS的客户端API来将数据传到HDFS。
## 写数据的流程（参考上图）：
1、客户端向hregionServer请求写数据
2、hregionServer将数据**先写**入hlog中。
3、hregionServer将数据**后写**入memstore中。
4、当内存中的数据达到阈值64M的时候，将数据Flush到硬盘中，并同时删除内存和hlog中的历史数据。
5、将硬盘中数据通过**HFile来序列化**，再将数据传输到HDFS进行存储。并对Hlog做一个标记。
6、当HDFS中的数据块达到4块的时候，Hmaster将数据加载到本地进行一个合并（如果合并后数据的大小小于256M则当数据块再次达到4块时（包含小于256M的数据块）将最新4块数据块再次进行合并，此时数据块大于256M）。
7、若数据块大于256M，则将数据重新拆分，将分配后的region重新分配给不同的hregionServer进行管理。
8、当hregionServer宕机后，将hregionServer上的hlog重新分配给不同的hregionServer进行加载（修改.META文件中关于数据所在server的信息）。注意：hlog会同步到HDFS中。
## 读数据的流程（参考下图）：
1、通过zk来获取ROOT表在那个节点上，然后进一步通过-ROOT表和-META表来获取最终的位置信息。
2、数据从内存和硬盘合并后返回到客户端。
![这里写图片描述](https://img-blog.csdn.net/20180329213056798?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
PS：由上图可看到，当客户端在执行查询语句的时候，会先到zk上寻找对应-ROOT表（主要描述-META表在哪里）的位置信息（由此也知道ZK在Hbase中的作用），接下来根据-ROOT表中数据进而找到对应的HRegionServer，在对应的HRegionServer上的-META表（主要记载表的元数据信息）中找到对应的Table表在哪个HRegionServer上，再到对应的HRegionServer中查找对应的数据。


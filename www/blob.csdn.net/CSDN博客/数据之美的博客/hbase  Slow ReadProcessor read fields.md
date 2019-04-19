# hbase : Slow ReadProcessor read fields - 数据之美的博客 - CSDN博客
2018年11月28日 20:18:41[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：547
个人分类：[大数据																[Hbase](https://blog.csdn.net/oppo62258801/article/category/8281289)](https://blog.csdn.net/oppo62258801/article/category/6768103)
在测试时，经常会看到hbase日志里有报DFSClient Slow ReadProcessor read fields的消息。
`2018-11-28 18:20:34,394 WARN org.apache.hadoop.hdfs.DFSClient: Slow ReadProcessor read fields took 31274ms (threshold=30000ms); ack: seqno: 904 reply: SUCCESS reply: SUCCESS reply: SUCCESS downstreamAckTimeNanos: 1062913 flag: 0 flag: 0 flag: 0, targets: [DatanodeInfoWithStorage[*IP*:50010,DS-*ID*-c6e44cda51b2,DISK], DatanodeInfoWithStorage[*IP*:50010,DS-*ID*-dc34bbcf7729,DISK], DatanodeInfoWithStorage[*IP*:50010,DS-*ID*6a3b7f64e85b,DISK]]`- 大致定位问题
这个问题主要是由于hdfs引起的，因为hbase作为客户端向hdfs写入数据进行持久化，和hbase本身没有太大关系。至于是因为哪一部分的问题，可以用如下命令对datanode上的日志来分析一下：
`egrep -o "Slow.*?(took|cost)" /path/to/current/datanode/log | sort | uniq -c`
```
[root@*IP* hdfs1]# egrep -o "Slow.*?(took|cost)" hadoop-hdfs-datanode-*IP*.log.1 | sort | uniq -c
      8 Slow BlockReceiver write data to disk cost
   3326 Slow BlockReceiver write packet to mirror took
     22 Slow manageWriterOsCache took
      4 Slow PacketResponder send ack to upstream took
```
好的，得到返回结果，这部分怎么看呢？可以参照：
Slow BlockReceiver write data to disk cost : 表明在将块写入OS缓存或磁盘时存在延迟
Slow BlockReceiver write packet to mirror took ：表明在网络上写入块时有延迟
Slow manageWriterOsCache took : 表明在将块写入OS缓存或磁盘时存在延迟
Slow PacketResponder send ack to upstream took : 母鸡 。。。
Slow flushOrSync took : 表明在将块写入OS缓存或磁盘时存在延迟
另外就是当集群负载高的时候，即集群处于或接近内存，cpu或磁盘上限，集群可能无法确保数据本地化，因此需要在网络上传输数据块。如果是这种情况，由于使用集群上的额外负载来传输数据块，因此可能会在作业或数据节点中看到WARN消息。
此外，单个节点的硬件问题可能会在整个群集中导致“Slow”错误。
- 一些分析方法
如果单个节点的一个或多个类别的”Slow“消息比其他主机的”Slow“消息数量多出数量级，则需要查看底层硬件是否存在问题。
如果Slow消息数最多的是Slow BlockReceiver write packet tomirror took，请通过以下命令的输出来调查可能的网络问题：
- ifconfig -a(定期检查问题主机上增加的errors和dropped的数量，往往代表的是网卡，网线或者上游的网络有问题)
- netstat -s（与正常节点相比，查找大量重新传输的数据包或其他异常高的指标）。
- netstat -s | grep -i retrans（整个集群执行）。 （在一个或多个节点上查找大于正常的计数）。
如果Slow消息最多的是一些其他消息，建议使用以下命令检查磁盘问题：
- iostat[高iowait百分比，超过15%]
- iostat -x和sar -d（特定分区的高await或％util）
- dmesg (磁盘错误)
- 使用smartctl对磁盘进行健康检查：停止受影响节点的所有Hadoop进程，然后运行sudo smartctl -H /dev/<disk>，检查HDFS使用的每块<disk>

# HBase 内核组件协调及RegionServer JVM参数调优-OLAP商业环境实战 - weixin_33985507的博客 - CSDN博客
2018年11月27日 15:23:29[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
本套技术专栏是作者（秦凯新）平时工作的总结和升华，通过从真实商业环境抽取案例进行总结和分享，并给出商业应用的调优建议和集群环境容量规划等内容，请持续关注本套博客。版权声明：禁止转载，欢迎学习。QQ邮箱地址：1120746959@qq.com，如有任何学术交流，可随时联系。
## 1 弱化的Master
HBase中Master的角色被弱化了，就算短时间待机都不会受影响，但不能宕机时间太久，数据的读取和写入都跟他没有什么关系。那么Master主要负责什么呢？创建表，修改列族配置，Region的分割和合并操作。
## 2 干保险业的WAL
HBase所有数据的写入操作第一步先二话不说写进WAL中，然后进一步写进MemStore中。等到MemStore的数据达到一定的数量时才刷写到最终存储的HFile中。如果在刷写的时候宕机或者断电了，干保险业的WAl就会起到数据恢复的作用。为什么呢？因为WAL是放在HDFS上的，宕机是不影响WAL文件的。
## 3 MemStore 真的是加速机吗？
注意数据写入到MemStore之前，是要先要写入到WAL的，所以增加MemStore并不能加快写入速度，MemStore最大的意义，在于维持数据按照rowkey的顺序排列，进而数据在MemStore中被整理成LSM树，最后再刷写到HFile上。
## 4 Store 和 Column Family的关系
在一个RegionServer中，会有多个Region，而在每一个Region中，会根据 Column Family生成多个Store。注意一个Store又会对应一个MemStore。而一个RegionServer对应一个全局的BlockCache。
## 5 Minor compact 和 Major Compact
- 
Minor compact ：可以用于把多个HFile文件进行合并，同时可以删除TTL过期的数据，但是手动删除数据操作是不能被删除的。为什么这里可以删除TTL过期的数据？因为MemStore在LSM整理时，对于TTL过期只要不写入HFile文件就算是删除了，而对于手动删除数据操作则可能位于不同的HFile文件中，因此做不到删除。
- 
Major Compact：是把一个Store中的HFile文件合并为一个HFile文件，而不是把一个Region内的所有HFile文件，因为一个Region可能有多个Column Family对应的Store。
## 6 RegioneServer的JVM参数调优
- 
编辑hbase-env . sh，调整HBASE_HEAPSIZE大小，默认是1G，而MemStore则占用40%，也就是400M。显然是不够的。因此需要根据需要设置，比如：
```
export HBASE_HEAPSIZE =8G
复制代码
```
- 
HBase JVM 最大使用内存调整：
（1） HBASE_MASTER_OPTS
```
export HBASE_MASTER_OPTS="$HBASE_MASTER_OPTS  -Xmx4g -Xms4g"
复制代码
```
（2） HBASE_REGIONSERVER_OPTS
```
export HBASE_REGIONSERVER_OPTS="$HBASE_REGIONSERVER_OPTS  -Xms8g -Xmx8g"
复制代码
```
注意永远至少留10%的内存给操作系统。
- 
举例如下：一台16GB的机器，上面有MapReduce服务，RegionServer 和 DataNode
分配策略如下：
- 系统进程：2G
- MapReduce:8GB
- RegionServer服务:4GB
- TaskTracker：1GB
- DataNode:1GB
## 7 朱丽叶暂停FULL GC
- 
原因主要是Zookeeper惹的祸，在RegionServer发生FULL GC的时候，STW期间太长，被ZK标记为宕机，当RegionerServer GC完成后，苏醒了发现被标记为宕机了，这时候RegionerServer GC就自杀，防止脑裂发生。醒来再自杀，朱丽叶暂停，哈哈！
- 
JVM提供4种GC方案
- 
如果你的RegionServer内存大于32GB，建议使用G1GC策略，因为G1Gc会把堆内存划分为多个Region,然后对各个Region单独进行GC,这样整体的Full GC 可以被最大限度地避免。另外通过设置MaxGCPauseMillis最大暂停时间，避免时间太长RegionServer自杀。
```
export HBASE_REGIONSERVER_OPTS="$HBASE_REGIONSERVER_OPTS  -Xms8g -Xmx8g  -XX:+UseG1GC
-XX:+MaxGCPauseMillis=100"
复制代码
```
- 
如果你的RegionServer内存小于4GB，就不需要考虑G1GC策略了，直接使用
```
-XX:+UseParNewGC （年轻代使用）
-XX:+UseConcMarkSweepGC（年老代使用）
export HBASE_REGIONSERVER_OPTS="$HBASE_REGIONSERVER_OPTS  -Xms8g -Xmx8g 
-XX:+UseParNewGC -XX:+UseConcMarkSweepGC "
复制代码
```
## 8 Justin Kestelyn 测试调优论文
- 32GB heap : -XX:G1NewSizePercent=3 设置要用作年轻代大小最小值的堆百分比。默认值是 Java 堆的 5%。这是一个实验性的标志
- 64GB heap:  -XX:G1NewSizePercent=2
- 100GB 或者更大的heap时，-XX:G1NewSizePercent=1
## 9 总结
网上的Hbase调优资料参差不齐，实在是不忍卒读，有些都是拼凑且版本过时的东西，我这里决定综合所有优质资源进行整合，写一份最全，最有深度，不过时的技术博客。辛苦成文，各自珍惜，谢谢！
本套技术专栏是作者（秦凯新）平时工作的总结和升华，通过从真实商业环境抽取案例进行总结和分享，并给出商业应用的调优建议和集群环境容量规划等内容，请持续关注本套博客。版权声明：禁止转载，欢迎学习。QQ邮箱地址：1120746959@qq.com，如有任何学术交流，可随时联系。
秦凯新 于深圳  20182309

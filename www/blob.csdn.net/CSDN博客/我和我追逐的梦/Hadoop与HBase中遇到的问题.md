
# Hadoop与HBase中遇到的问题 - 我和我追逐的梦~~~ - CSDN博客


2014年05月09日 23:01:20[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：23056


1. Hadoop中遇到的问题
以前所遇到的问题由于没有记录,所以忘了
(1)NameNode没有启动成功, 是由于你对HDFS多次格式化,导致datanode中与namenode中的VERSION文件中的namespaceID不一致(对于NameNode节点,该文件位于hdfs-site配置文件中dfs.name.dir参数所指定的路径下的current文件夹中, 对于DataNode节点, 该文件位于hdfs-site配置文件中dfs.data.dir参数所指定的路径下的current文件夹中.
解决方法: 第一种是把namespaceID的值改成一致,然后重启Hadoop；第二种删除dfs.name.dir与dfs.data.dir参数指定的目录,然后使用bin/hadoop namenode -formate 重新格式化,这种方法有风险, 因为会删除所有原来在HDFS上的文件.
(2)Eclipse的Run On Hadoop就是一个坑, 其根本就没运行在集群上, (可以通过job.setNumReduceTasks设置ReducerTask个数,无论你设置多少个,都只有一个,因为运行在本地,只是文件数据在集群上, 也就是说Mapper与Reducer任务都运行在本地; 还可以通过控制台信息查看到: 如果是集群上则会有这样的信息Running job: job_201405090934_0024, 如果是本地任务,则会显示Running job: job_local426339719_0001,看到没有,
 中间有个local； 还可以通过web node1:50030查看任务运行情况,如果是本地任务,则不会在上面显示).
解决方法: 如果需要运行在集群上,要做三件事,如下:

```python
//特别注意: 一定要设置,不然会报cannot read partitioner file错误
         conf.set("fs.default.name","node1:49000");
         //特别注意: 一定要设置,不然不会运行在集群上
         conf.set("mapred.job.tracker","node1:49001");
         //特别注意: 对相关类以及依赖的jar文件进行打包,这是运行在集群上必须要做的一步,不然集群找不到相关的Mapper等类文件
 		 File jarpath;
		 try {
			 jarpath = JarTools.makeJar("bin");
			 conf.set("mapred.jar", jarpath.toString());
		 } catch (Exception e) {
			 logger.error("进行jar打包出错!");
			 e.printStackTrace();
			 return;
		 }
```
(3) Unable to load native-hadoop library for your platform... using builtin-java classes where applicable
在运行时,Run Configurations 在Arguments中的 VM arguments 添加-Djava.library.path=/home/hadoop/hadoop-1.2.1/lib/native/Linux-i386-32
该路径依据你的实际路径为准
2.HBase问题
(1) Opening socket connection to server localhost/127.0.0.1:2181. Will not attempt to authenticate using SASL (无法定位登录配置)
22:32:56,821 WARN                    ClientCnxn:1089 - Session 0x0 for server null, unexpected error, closing socket connection and attempting reconnect
java.net.ConnectException: Connection refused
at sun.nio.ch.SocketChannelImpl.checkConnect(Native Method)
at sun.nio.ch.SocketChannelImpl.finishConnect(SocketChannelImpl.java:567)
at org.apache.zookeeper.ClientCnxnSocketNIO.doTransport(ClientCnxnSocketNIO.java:350)
at org.apache.zookeeper.ClientCnxn$SendThread.run(ClientCnxn.java:1068)
22:32:56,951 WARN          RecoverableZooKeeper:253 - Possibly transient ZooKeeper, quorum=localhost:2181, exception=org.apache.zookeeper.KeeperException$ConnectionLossException: KeeperErrorCode = ConnectionLoss for /hbase/hbaseid
22:32:56,952 INFO                  RetryCounter:155 - Sleeping 1000ms before retry \#0...
这是因为在代码中没有设置Zookeeper集群
```python
//设置zookeeper集群
HBASE_CONFIG.set("hbase.zookeeper.quorum", "node2,node3,node4,node5,node6,node7,node8");
```

最好还设置HMaster

```python
//设置HMatser
 HBASE_CONFIG.set("hbase.zookeeper.master","node1:60000");
```
(2)
JobClient:1422 - Task Id : attempt_201405081252_0008_m_000000_0, Status : FAILED
java.lang.IllegalArgumentException: Can't read partitions file
at org.apache.hadoop.mapreduce.lib.partition.TotalOrderPartitioner.setConf(TotalOrderPartitioner.java:116)
at org.apache.hadoop.util.ReflectionUtils.setConf(ReflectionUtils.java:62)
at org.apache.hadoop.util.ReflectionUtils.newInstance(ReflectionUtils.java:117)
at org.apache.hadoop.mapred.MapTask$NewOutputCollector.<init>(MapTask.java:676)
at org.apache.hadoop.mapred.MapTask.runNewMapper(MapTask.java:756)
at org.apache.hadoop.mapred.MapTask.run(MapTask.java:364)
at org.apache.hadoop.mapred.Child$4.run(Child.java:255)
at java.security.AccessController.doPrivileged(Native Method)
at javax.security.auth.Subject.doAs(Subject.java:396)
at org.apache.hadoop.security.UserGroupInformation.doAs(UserGroupInformation.java:1190)
at org.apache.hadoop.mapred.Child.main(Child.java:249)
Caused by: java.io.FileNotFoundException: File /tmp/partitions_de363500-5535-466b-91bb-36472457386d does not exist.
at org.apache.hadoop.fs.RawLocalFileSystem.getFileStatus(RawLocalFileSystem.java:402)
at org.apache.hadoop.fs.FilterFileSystem.getFileStatus(FilterFileSystem.java:255)
at org.apache.hadoop.fs.FileSystem.getLength(FileSystem.java:816)
at org.apache.hadoop.io.SequenceFile$Reader.<init>(SequenceFile.java:1479)
at org.apache.hadoop.io.SequenceFile$Reader.<init>(SequenceFile.java:1474)
at org.apache.hadoop.mapreduce.lib.partition.TotalOrderPartitioner.readPartitions(TotalOrderPartitioner.java:301)
at org.apache.hadoop.mapreduce.lib.partition.TotalOrderPartitioner.setConf(TotalOrderPartitioner.java:88)
... 10 more
因为在生成HFile时使用了HFileOutputFormat与TotalOrderPartitioner(HFile对RowKey需要进行全局排序),所以需要分区文件, 但是TaskTracker找不到分区文件,要想TaskTracker读取分区文件,该文本必须存在与HDFS上, 所以需要设置一个参数:

```python
Configuration conf = HbaseOperation.HBASE_CONFIG; 
         conf.set("fs.default.name","node1:49000");
```
对于具体解释,还需进一步研究.

(3)Wrong number of partitions in keyset
说明你的分区文件中的分区个数不等于reducer的个数减一,即Regions的个数不等于reducer的个数减一,其实是因为你的任务运行在本地(这样只有一个Reducer),而Regions有多个,有兴趣可以查看
```python
TotalOrderPartitioner
```
的源代码, 中间有一段代码为:

```python
for (int i = 0; i < splitPoints.length - 1; ++i) {
        if (comparator.compare(splitPoints[i], splitPoints[i+1]) >= 0) {
          throw new IOException("Split points are out of order");
        }
      }
```
HFileOutputFormat.configureIncrementalLoad(job, table);自动对job进行配置。TotalOrderPartitioner是需要先对key进行整体排序，然后划分到每个reduce中，保证每一个reducer中的的key最小最大值区间范围，是不会有交集的。因为入库到HBase的时候，作为一个整体的Region，key是绝对有序的。

暂时写到这里, 有些问题不记得了,以后遇到问题会继续更新,....



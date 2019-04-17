# Apache Hadoop DistCP(分布式拷贝)示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月10日 22:47:53[boonya](https://me.csdn.net/boonya)阅读数：2092








本篇文章来自：[https://examples.javacodegeeks.com/enterprise-java/apache-hadoop/apache-hadoop-distcp-example/](https://examples.javacodegeeks.com/enterprise-java/apache-hadoop/apache-hadoop-distcp-example/)


在这个例子中，我们将向您展示如何使用分布式复制工具在Hadoop的集群/集群内设置中复制大型文件。


# 1.介绍

DistCP是Apache Hadoop上下文中的Distributed Copy（分布式拷贝）的缩写。它基本上是一个工具，可以使用在我们需要复制大量的数据/文件在集群内/集群设置。在后台，DisctCP使用MapReduce分发和复制数据，这意味着操作分布在集群中的多个可用节点上。这使得它更有效和有效的复制工具。

DistCP获取文件列表（在多个文件的情况下），并在多个Map任务之间分发数据，这些映射任务将分配给它们的数据部分复制到目标。

# 2.语法和示例

在本节中，我们将检查DistCP的语法以及一些示例。


## 2.1基本

以下是distCp命令的基本语法。


`hadoop distcp hdfs://namenode:port/source hdfs://namenode:port/destination`
遵循distcp第一个参数应该是源的完全限定地址，包括namenode和端口号。第二个参数应该是目标地址。distcp的基本语法是很容易和简单。它使用MapReduce自动处理所有分发和复制。

如果在同一个集群之间复制，源和目的地的namenode和端口号将是相同的，并且在不同集群的情况下，两者将不同。

基本distcp的示例：



`hadoop distcp hdfs://quickstart.cloudera:8020/user/access_logs hdfs://quickstart.cloudera:8020/user/destination_access_logs`
以下是命令执行的日志：





```
15/12/01 17:13:07 INFO tools.DistCp: Input Options: DistCpOptions{atomicCommit=false, syncFolder=false, deleteMissing=false, ignoreFailures=false, maxMaps=20, sslConfigurationFile='null', copyStrategy='uniformsize', sourceFileListing=null, sourcePaths=[hdfs://quickstart.cloudera:8020/user/access_logs], targetPath=hdfs://quickstart.cloudera:8020/user/destination_access_logs, targetPathExists=false, preserveRawXattrs=false, filtersFile='null'}
15/12/01 17:13:07 INFO client.RMProxy: Connecting to ResourceManager at /0.0.0.0:8032
15/12/01 17:13:08 INFO tools.SimpleCopyListing: Paths (files+dirs) cnt = 2; dirCnt = 1
15/12/01 17:13:08 INFO tools.SimpleCopyListing: Build file listing completed.
15/12/01 17:13:08 INFO Configuration.deprecation: io.sort.mb is deprecated. Instead, use mapreduce.task.io.sort.mb
15/12/01 17:13:08 INFO Configuration.deprecation: io.sort.factor is deprecated. Instead, use mapreduce.task.io.sort.factor
15/12/01 17:13:08 INFO tools.DistCp: Number of paths in the copy list: 2
15/12/01 17:13:08 INFO tools.DistCp: Number of paths in the copy list: 2
15/12/01 17:13:08 INFO client.RMProxy: Connecting to ResourceManager at /0.0.0.0:8032
15/12/01 17:13:09 INFO mapreduce.JobSubmitter: number of splits:2
15/12/01 17:13:09 INFO mapreduce.JobSubmitter: Submitting tokens for job: job_1449017643353_0001
15/12/01 17:13:10 INFO impl.YarnClientImpl: Submitted application application_1449017643353_0001
15/12/01 17:13:10 INFO mapreduce.Job: The url to track the job: http://quickstart.cloudera:8088/proxy/application_1449017643353_0001/
15/12/01 17:13:10 INFO tools.DistCp: DistCp job-id: job_1449017643353_0001
15/12/01 17:13:10 INFO mapreduce.Job: Running job: job_1449017643353_0001
15/12/01 17:13:20 INFO mapreduce.Job: Job job_1449017643353_0001 running in uber mode : false
15/12/01 17:13:20 INFO mapreduce.Job:  map 0% reduce 0%
15/12/01 17:13:32 INFO mapreduce.Job:  map 50% reduce 0%
15/12/01 17:13:34 INFO mapreduce.Job:  map 100% reduce 0%
15/12/01 17:13:34 INFO mapreduce.Job: Job job_1449017643353_0001 completed successfully
15/12/01 17:13:35 INFO mapreduce.Job: Counters: 33
	File System Counters
		FILE: Number of bytes read=0
		FILE: Number of bytes written=228770
		FILE: Number of read operations=0
		FILE: Number of large read operations=0
		FILE: Number of write operations=0
		HDFS: Number of bytes read=39594819
		HDFS: Number of bytes written=39593868
		HDFS: Number of read operations=28
		HDFS: Number of large read operations=0
		HDFS: Number of write operations=7
	Job Counters 
		Launched map tasks=2
		Other local map tasks=2
		Total time spent by all maps in occupied slots (ms)=20530
		Total time spent by all reduces in occupied slots (ms)=0
		Total time spent by all map tasks (ms)=20530
		Total vcore-seconds taken by all map tasks=20530
		Total megabyte-seconds taken by all map tasks=21022720
	Map-Reduce Framework
		Map input records=2
		Map output records=0
		Input split bytes=276
		Spilled Records=0
		Failed Shuffles=0
		Merged Map outputs=0
		GC time elapsed (ms)=94
		CPU time spent (ms)=1710
		Physical memory (bytes) snapshot=257175552
		Virtual memory (bytes) snapshot=3006455808
		Total committed heap usage (bytes)=121503744
	File Input Format Counters 
		Bytes Read=675
	File Output Format Counters 
		Bytes Written=0
	org.apache.hadoop.tools.mapred.CopyMapper$Counter
		BYTESCOPIED=39593868
		BYTESEXPECTED=39593868
		COPY=2
```

日志中的行号35表示执行的映射任务的数量，在这种情况下为2。

要检查复制是否成功，我们可以在HDFS中运行以下命令：




`hadoop fs -ls /user/destination_access_logs`
以下是如果复制成功并且数据存在于目标文件夹中的输出：


![](https://img-blog.csdn.net/20170210223605210)


注意：当文件在两个不同的集群之间复制时，两个集群上的HDFS版本应该相同或在不同版本的情况下，较高版本应向后兼容。




## 2.2多源

如果有多个文件源并且需要转到相同的目标源，则所有源都可以作为参数传递，如下面的示例语法中所示：

`hadoop distcp -f hdfs://namenode:port/sourceListFile hdfs://namenode:port/destination`
因此，来自所有三个源的文件将被复制到指定的目的地。

还有另一种选择，如果有很多来源和写长命令成为一个问题。以下是替代方法：

`hadoop distcp hdfs://namenode:port/source1 hdfs://namenode:port/source2 hdfs://namenode:port/source3 hdfs://namenode:port/destination`
其中，sourceListFile是一个包含所有源的列表的简单文件。在这种情况下，源列表文件需要传递标志-f，表示源不是要复制的文件，而是包含所有源的文件。

注意：当distcp与多个源一起使用时，如果源冲突，distcp将使用错误消息中止副本。但是在目的地发生冲突的情况下，不会中止复制，但根据指定的选项解决冲突。如果未指定任何选项，则默认为跳过目标已存在的文件。



## 2.3更新和覆盖标志

如名称所示，更新将更新目标文件夹中的文件，但仅在满足更新条件时更新。要执行的更新的条件是，更新检查id目的地具有相同的文件名，如果文件大小和内容与源文件相同，如果一切相同则文件不更新，但如果不同的文件被更新源到目的地。

覆盖将覆盖目的地文件的目的地具有相同的文件名，如果是，则文件将被覆盖。


`hadoop distcp -update hdfs://namenode:port/source hdfs://namenode:port/destination``hadoop distcp -overwrite hdfs://namenode:port/source hdfs://namenode:port/destination`



## 2.4忽略故障标志

在distcp中，任何映射任务失败，它也停止其他映射任务，并且复制过程完全停止并出现错误。在情况下，即使一个或多个映射任务失败，也需要继续复制其他数据块，我们有一个忽略失败标志，即-i。



`hadoop distcp -i hdfs://namenode:port/source hdfs://namenode:port/destination`



## 2.5最大映射任务

如果用户想要指定可以分配给distcp执行的映射任务的最大数量，则有另一个标志 -m <max_num>。


`hadoop distcp -m 5 hdfs://namenode:port/source hdfs://namenode:port/destination`
此示例命令将向distcp命令最多分配5个映射任务。

distcp中设置最大映射任务的示例：



`hadoop distcp -m 1 hdfs://quickstart.cloudera:8020/user/access_logs hdfs://quickstart.cloudera:8020/user/destination_access_logs_3`
这里我们将map任务限制为1.从上面的示例日志输出我们知道这个文件数据的默认map任务是2。

下面是命令执行的日志：





```
15/12/01 17:19:33 INFO tools.DistCp: Input Options: DistCpOptions{atomicCommit=false, syncFolder=false, deleteMissing=false, ignoreFailures=false, maxMaps=1, sslConfigurationFile='null', copyStrategy='uniformsize', sourceFileListing=null, sourcePaths=[hdfs://quickstart.cloudera:8020/user/access_logs], targetPath=hdfs://quickstart.cloudera:8020/user/destination_access_logs_3, targetPathExists=false, preserveRawXattrs=false, filtersFile='null'}
15/12/01 17:19:33 INFO client.RMProxy: Connecting to ResourceManager at /0.0.0.0:8032
15/12/01 17:19:34 INFO tools.SimpleCopyListing: Paths (files+dirs) cnt = 2; dirCnt = 1
15/12/01 17:19:34 INFO tools.SimpleCopyListing: Build file listing completed.
15/12/01 17:19:34 INFO Configuration.deprecation: io.sort.mb is deprecated. Instead, use mapreduce.task.io.sort.mb
15/12/01 17:19:34 INFO Configuration.deprecation: io.sort.factor is deprecated. Instead, use mapreduce.task.io.sort.factor
15/12/01 17:19:34 INFO tools.DistCp: Number of paths in the copy list: 2
15/12/01 17:19:34 INFO tools.DistCp: Number of paths in the copy list: 2
15/12/01 17:19:34 INFO client.RMProxy: Connecting to ResourceManager at /0.0.0.0:8032
15/12/01 17:19:35 INFO mapreduce.JobSubmitter: number of splits:1
15/12/01 17:19:35 INFO mapreduce.JobSubmitter: Submitting tokens for job: job_1449017643353_0003
15/12/01 17:19:35 INFO impl.YarnClientImpl: Submitted application application_1449017643353_0003
15/12/01 17:19:35 INFO mapreduce.Job: The url to track the job: http://quickstart.cloudera:8088/proxy/application_1449017643353_0003/
15/12/01 17:19:35 INFO tools.DistCp: DistCp job-id: job_1449017643353_0003
15/12/01 17:19:35 INFO mapreduce.Job: Running job: job_1449017643353_0003
15/12/01 17:19:44 INFO mapreduce.Job: Job job_1449017643353_0003 running in uber mode : false
15/12/01 17:19:44 INFO mapreduce.Job:  map 0% reduce 0%
15/12/01 17:19:52 INFO mapreduce.Job:  map 100% reduce 0%
15/12/01 17:19:52 INFO mapreduce.Job: Job job_1449017643353_0003 completed successfully
15/12/01 17:19:52 INFO mapreduce.Job: Counters: 33
	File System Counters
		FILE: Number of bytes read=0
		FILE: Number of bytes written=114389
		FILE: Number of read operations=0
		FILE: Number of large read operations=0
		FILE: Number of write operations=0
		HDFS: Number of bytes read=39594404
		HDFS: Number of bytes written=39593868
		HDFS: Number of read operations=20
		HDFS: Number of large read operations=0
		HDFS: Number of write operations=5
	Job Counters 
		Launched map tasks=1
		Other local map tasks=1
		Total time spent by all maps in occupied slots (ms)=5686
		Total time spent by all reduces in occupied slots (ms)=0
		Total time spent by all map tasks (ms)=5686
		Total vcore-seconds taken by all map tasks=5686
		Total megabyte-seconds taken by all map tasks=5822464
	Map-Reduce Framework
		Map input records=2
		Map output records=0
		Input split bytes=138
		Spilled Records=0
		Failed Shuffles=0
		Merged Map outputs=0
		GC time elapsed (ms)=45
		CPU time spent (ms)=1250
		Physical memory (bytes) snapshot=123002880
		Virtual memory (bytes) snapshot=1504280576
		Total committed heap usage (bytes)=60751872
	File Input Format Counters 
		Bytes Read=398
	File Output Format Counters 
		Bytes Written=0
	org.apache.hadoop.tools.mapred.CopyMapper$Counter
		BYTESCOPIED=39593868
		BYTESEXPECTED=39593868
		COPY=2
```

此示例中的map任务最大为1，如上述日志的第34行所示。



# 3.最后说明

在这个例子中，我们看到在Apache Hadoop中使用distcp命令来复制大量的数据。有关distcp命令和所有可用选项的更多帮助和详细信息，请使用以下命令检查内置帮助：


`hadoop distcp`







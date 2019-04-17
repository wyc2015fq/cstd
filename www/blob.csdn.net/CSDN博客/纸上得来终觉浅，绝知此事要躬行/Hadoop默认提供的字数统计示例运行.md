# Hadoop默认提供的字数统计示例运行 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2019年01月03日 17:09:45[boonya](https://me.csdn.net/boonya)阅读数：420
所属专栏：[Hadoop学习实践](https://blog.csdn.net/column/details/14484.html)









开始之前先了解 hadoop fs 命令使用，然后再通过运行示例程序来观看Hadoop的简单运行效果。

**目录**

[Hadoop fs命令](#Hadoop%20fs%E5%91%BD%E4%BB%A4)

[基本目录操作](#%E5%9F%BA%E6%9C%AC%E7%9B%AE%E5%BD%95%E6%93%8D%E4%BD%9C)

[创建输入目录](#%E5%88%9B%E5%BB%BA%E8%BE%93%E5%85%A5%E7%9B%AE%E5%BD%95)

[创建输出目录](#%E5%88%9B%E5%BB%BA%E8%BE%93%E5%87%BA%E7%9B%AE%E5%BD%95)

[删除目录操作](#%E5%88%A0%E9%99%A4%E7%9B%AE%E5%BD%95%E6%93%8D%E4%BD%9C)

[上传作业文件](#%E4%B8%8A%E4%BC%A0%E4%BD%9C%E4%B8%9A%E6%96%87%E4%BB%B6)

[执行MapReduce计算](#%E6%89%A7%E8%A1%8CMapReduce%E8%AE%A1%E7%AE%97)

[定位执行目录](#%E5%AE%9A%E4%BD%8D%E6%89%A7%E8%A1%8C%E7%9B%AE%E5%BD%95)

[运行指令](#%E8%BF%90%E8%A1%8C%E6%8C%87%E4%BB%A4)

[运行状态](#%E8%BF%90%E8%A1%8C%E7%8A%B6%E6%80%81)

[执行成功日志](#%E6%89%A7%E8%A1%8C%E6%88%90%E5%8A%9F%E6%97%A5%E5%BF%97)

[查看执行结果](#%E6%9F%A5%E7%9C%8B%E6%89%A7%E8%A1%8C%E7%BB%93%E6%9E%9C)

[注意事项](#%E6%B3%A8%E6%84%8F%E4%BA%8B%E9%A1%B9)

[输出目录必须是不存在的目录](#%E8%BE%93%E5%87%BA%E7%9B%AE%E5%BD%95%E5%BF%85%E9%A1%BB%E6%98%AF%E4%B8%8D%E5%AD%98%E5%9C%A8%E7%9A%84%E7%9B%AE%E5%BD%95)

[执行过程中出错classpath未配置](#%E6%89%A7%E8%A1%8C%E8%BF%87%E7%A8%8B%E4%B8%AD%E5%87%BA%E9%94%99classpath%E6%9C%AA%E9%85%8D%E7%BD%AE)

## Hadoop fs命令

```
1. Hadoop fs –fs [local | <file system URI>]：

列出在指定目录下的文件内容，支持pattern匹配。输出格式如filename(full path)   <r n> size. 其中n代表replica的个数，size代表大小（单位bytes）。
2. hadoop fs –ls <path>

递归列出匹配pattern的文件信息，类似ls，只不过递归列出所有子目录信息。
3. hadoop fs –lsr <path>：

列出匹配pattern的指定的文件系统空间总量（单位bytes），等价于unix下的针对目录的du –sb <path>/*和针对文件的du –b <path> ，输出格式如name(full path)  size(in bytes)。
4. hadoop fs –du <path>

等价于-du，输出格式也相同，只不过等价于unix的du -sb。
5. hadoop fs –dus <path>

将制定格式的文件 move到指定的目标位置。当src为多个文件时，dst必须是个目录。
6. hadoop fs –mv <src> <dst>

拷贝文件到目标位置，当src为多个文件时，dst必须是个目录。
7. hadoop fs –cp <src> <dst>

删除匹配pattern的指定文件，等价于unix下的rm <src>
8. hadoop fs –rm [-skipTrash] <src>

递归删掉所有的文件和目录，等价于unix下的rm –rf <src>
9. hadoop fs –rmr [skipTrash] <src>

等价于unix的rm –rfi <src>
10. hadoop fs –rmi [skipTrash] <src>

从本地系统拷贝文件到DFS
11. hadoop fs –put <localsrc> … <dst>

等价于-put
12. hadoop fs –copyFromLocal <localsrc> … <dst>

等同于-put，只不过源文件在拷贝后被删除
13. hadoop fs –moveFromLocal <localsrc> … <dst>

从DFS拷贝文件到本地文件系统，文件匹配pattern，若是多个文件，则dst必须是目录
14. hadoop fs –get [-ignoreCrc] [-crc] <src> <localdst>

顾名思义，从DFS拷贝多个文件、合并排序为一个文件到本地文件系统
15. hadoop fs –getmerge <src> <localdst>

 输出文件内容
16. hadoop fs –cat <src>

等价于-get
17. hadoop fs –copyToLocal [-ignoreCrc] [-crc] <src> <localdst>

在指定位置创建目录
18. hadoop fs –mkdir <path>

设置文件的备份级别，-R标志控制是否递归设置子目录及文件
19. hadoop fs –setrep [-R] [-w] <rep> <path/file>

修改文件的权限，-R标记递归修改。MODE为a+r,g-w,+rwx等，OCTALMODE为755这样
20. hadoop fs –chmod [-R] <MODE[,MODE]…|OCTALMODE> PATH…

修改文件的所有者和组。-R表示递归
21. hadoop fs -chown [-R] [OWNER][:[GROUP]] PATH…

等价于-chown … :GROUP …
22. hadoop fs -chgrp [-R] GROUP PATH…

计数文件个数及所占空间的详情，输出表格的列的含义依次为：DIR_COUNT,FILE_COUNT,CONTENT_SIZE,FILE_NAME或者如果加了-q的话，还会列出QUOTA,REMAINING_QUOTA,SPACE_QUOTA,REMAINING_SPACE_QUOTA
23. hadoop fs –count[-q] <path>
```

## 基本目录操作

### 创建输入目录
`hadoop fs -mkdir -p /data/wordcount`
### 创建输出目录

```
hadoop fs -mkdir -p /output/
```

### 删除目录操作

请参看注意事项，输出目录如果存在会产生异常，必要时你需要删除

```
hadoop fs -rm -r -skipTrash /output/wordcount
```

### 上传作业文件

在usr下面创建一个inputWord文件，写入一些内容，通过如下命令上传到hadoop 文件系统：

```
hadoop fs -put /usr/inputWord /data/wordcount/
```

## 执行MapReduce计算

### 定位执行目录

hadoop默认示例程序在/hadoop/share/hadoop/mapreduce下

```
[root@master mapreduce]# ls
hadoop-mapreduce-client-app-3.1.0.jar              hadoop-mapreduce-client-nativetask-3.1.0.jar
hadoop-mapreduce-client-common-3.1.0.jar           hadoop-mapreduce-client-shuffle-3.1.0.jar
hadoop-mapreduce-client-core-3.1.0.jar             hadoop-mapreduce-client-uploader-3.1.0.jar
hadoop-mapreduce-client-hs-3.1.0.jar               hadoop-mapreduce-examples-3.1.0.jar
hadoop-mapreduce-client-hs-plugins-3.1.0.jar       jdiff
hadoop-mapreduce-client-jobclient-3.1.0.jar        lib-examples
hadoop-mapreduce-client-jobclient-3.1.0-tests.jar  sources
```

### 运行指令
` hadoop jar hadoop-mapreduce-examples-3.1.0.jar wordcount /data/wordcount /output/wordcount`
### 运行状态

执行中

![](https://img-blog.csdnimg.cn/20190103165202151.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

执行成功： 

![](https://img-blog.csdnimg.cn/201901031656141.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

### 执行成功日志

```
[root@master mapreduce]#  hadoop jar hadoop-mapreduce-examples-3.1.0.jar wordcount /data/wordcount /output/wordcount
2019-01-03 03:50:13,643 INFO client.RMProxy: Connecting to ResourceManager at master/192.168.1.10:8032
2019-01-03 03:50:14,637 INFO mapreduce.JobResourceUploader: Disabling Erasure Coding for path: /tmp/hadoop-yarn/staging/root/.staging/job_1546505363471_0001
2019-01-03 03:50:15,910 INFO input.FileInputFormat: Total input files to process : 1
2019-01-03 03:50:16,049 INFO mapreduce.JobSubmitter: number of splits:1
2019-01-03 03:50:16,097 INFO Configuration.deprecation: yarn.resourcemanager.system-metrics-publisher.enabled is deprecated. Instead, use yarn.system-metrics-publisher.enabled
2019-01-03 03:50:16,315 INFO mapreduce.JobSubmitter: Submitting tokens for job: job_1546505363471_0001
2019-01-03 03:50:16,317 INFO mapreduce.JobSubmitter: Executing with tokens: []
2019-01-03 03:50:16,584 INFO conf.Configuration: resource-types.xml not found
2019-01-03 03:50:16,585 INFO resource.ResourceUtils: Unable to find 'resource-types.xml'.
2019-01-03 03:50:17,081 INFO impl.YarnClientImpl: Submitted application application_1546505363471_0001
2019-01-03 03:50:17,131 INFO mapreduce.Job: The url to track the job: http://master:8088/proxy/application_1546505363471_0001/
2019-01-03 03:50:17,131 INFO mapreduce.Job: Running job: job_1546505363471_0001
2019-01-03 03:51:24,818 INFO mapreduce.Job: Job job_1546505363471_0001 running in uber mode : false
2019-01-03 03:51:24,820 INFO mapreduce.Job:  map 0% reduce 0%
2019-01-03 03:52:40,092 INFO mapreduce.Job:  map 100% reduce 0%
2019-01-03 03:53:26,431 INFO mapreduce.Job:  map 100% reduce 100%
2019-01-03 03:53:28,463 INFO mapreduce.Job: Job job_1546505363471_0001 completed successfully
2019-01-03 03:53:28,698 INFO mapreduce.Job: Counters: 53
	File System Counters
		FILE: Number of bytes read=159
		FILE: Number of bytes written=426703
		FILE: Number of read operations=0
		FILE: Number of large read operations=0
		FILE: Number of write operations=0
		HDFS: Number of bytes read=204
		HDFS: Number of bytes written=97
		HDFS: Number of read operations=8
		HDFS: Number of large read operations=0
		HDFS: Number of write operations=2
	Job Counters 
		Launched map tasks=1
		Launched reduce tasks=1
		Data-local map tasks=1
		Total time spent by all maps in occupied slots (ms)=48735
		Total time spent by all reduces in occupied slots (ms)=43722
		Total time spent by all map tasks (ms)=48735
		Total time spent by all reduce tasks (ms)=43722
		Total vcore-milliseconds taken by all map tasks=48735
		Total vcore-milliseconds taken by all reduce tasks=43722
		Total megabyte-milliseconds taken by all map tasks=49904640
		Total megabyte-milliseconds taken by all reduce tasks=44771328
	Map-Reduce Framework
		Map input records=4
		Map output records=20
		Map output bytes=173
		Map output materialized bytes=159
		Input split bytes=108
		Combine input records=20
		Combine output records=14
		Reduce input groups=14
		Reduce shuffle bytes=159
		Reduce input records=14
		Reduce output records=14
		Spilled Records=28
		Shuffled Maps =1
		Failed Shuffles=0
		Merged Map outputs=1
		GC time elapsed (ms)=3037
		CPU time spent (ms)=7320
		Physical memory (bytes) snapshot=553144320
		Virtual memory (bytes) snapshot=5598339072
		Total committed heap usage (bytes)=410517504
		Peak Map Physical memory (bytes)=319217664
		Peak Map Virtual memory (bytes)=2806431744
		Peak Reduce Physical memory (bytes)=233926656
		Peak Reduce Virtual memory (bytes)=2791907328
	Shuffle Errors
		BAD_ID=0
		CONNECTION=0
		IO_ERROR=0
		WRONG_LENGTH=0
		WRONG_MAP=0
		WRONG_REDUCE=0
	File Input Format Counters 
		Bytes Read=96
	File Output Format Counters 
		Bytes Written=97
[root@master mapreduce]#
```

### 查看执行结果

_SUCCESS表示数据处理成功

```
[root@master mapreduce]# hadoop fs -ls /output/wordcount
Found 2 items
-rw-r--r--   2 root supergroup          0 2019-01-03 03:53 /output/wordcount/_SUCCESS
-rw-r--r--   2 root supergroup         97 2019-01-03 03:53 /output/wordcount/part-r-00000
[root@master mapreduce]# hadoop fs -text /output/wordcount/part-r-00000
English	1
I	2
If	1
Manlan	1
will	1
boonya	2
hello	1
is	2
love	1
marry	1
my	2
name	2
nick	1
you	2
[root@master mapreduce]#
```

## 注意事项

### 输出目录必须是不存在的目录

```
[root@master mapreduce]# hadoop jar hadoop-mapreduce-examples-3.1.0.jar wordcount /data/wordcount /output/wordcount
2019-01-03 02:53:17,357 INFO client.RMProxy: Connecting to ResourceManager at master/192.168.1.10:8032
org.apache.hadoop.mapred.FileAlreadyExistsException: Output directory hdfs://master:9000/output/wordcount already exists
	at org.apache.hadoop.mapreduce.lib.output.FileOutputFormat.checkOutputSpecs(FileOutputFormat.java:164)
	at org.apache.hadoop.mapreduce.JobSubmitter.checkSpecs(JobSubmitter.java:280)
	at org.apache.hadoop.mapreduce.JobSubmitter.submitJobInternal(JobSubmitter.java:146)
	at org.apache.hadoop.mapreduce.Job$11.run(Job.java:1570)
	at org.apache.hadoop.mapreduce.Job$11.run(Job.java:1567)
	at java.security.AccessController.doPrivileged(Native Method)
	at javax.security.auth.Subject.doAs(Subject.java:422)
	at org.apache.hadoop.security.UserGroupInformation.doAs(UserGroupInformation.java:1682)
	at org.apache.hadoop.mapreduce.Job.submit(Job.java:1567)
	at org.apache.hadoop.mapreduce.Job.waitForCompletion(Job.java:1588)
	at org.apache.hadoop.examples.WordCount.main(WordCount.java:87)
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62)
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
	at java.lang.reflect.Method.invoke(Method.java:498)
	at org.apache.hadoop.util.ProgramDriver$ProgramDescription.invoke(ProgramDriver.java:71)
	at org.apache.hadoop.util.ProgramDriver.run(ProgramDriver.java:144)
	at org.apache.hadoop.examples.ExampleDriver.main(ExampleDriver.java:74)
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62)
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
	at java.lang.reflect.Method.invoke(Method.java:498)
	at org.apache.hadoop.util.RunJar.run(RunJar.java:308)
	at org.apache.hadoop.util.RunJar.main(RunJar.java:222)
[root@master mapreduce]#
```

解决方法：删除存在的目录即可。 

### 执行过程中出错classpath未配置

```
[root@master mapreduce]# hadoop jar hadoop-mapreduce-examples-3.1.0.jar wordcount /data/wordcount /output/wordcount
2019-01-03 02:59:48,453 INFO client.RMProxy: Connecting to ResourceManager at master/192.168.1.10:8032
2019-01-03 02:59:51,905 INFO mapreduce.JobResourceUploader: Disabling Erasure Coding for path: /tmp/hadoop-yarn/staging/root/.staging/job_1546499638284_0001
2019-01-03 03:00:00,229 INFO input.FileInputFormat: Total input files to process : 1
2019-01-03 03:00:05,526 INFO mapreduce.JobSubmitter: number of splits:1
2019-01-03 03:00:09,917 INFO Configuration.deprecation: yarn.resourcemanager.system-metrics-publisher.enabled is deprecated. Instead, use yarn.system-metrics-publisher.enabled
2019-01-03 03:00:21,836 INFO mapreduce.JobSubmitter: Submitting tokens for job: job_1546499638284_0001
2019-01-03 03:00:21,838 INFO mapreduce.JobSubmitter: Executing with tokens: []
2019-01-03 03:00:23,756 INFO conf.Configuration: resource-types.xml not found
2019-01-03 03:00:23,757 INFO resource.ResourceUtils: Unable to find 'resource-types.xml'.
2019-01-03 03:00:28,935 INFO impl.YarnClientImpl: Submitted application application_1546499638284_0001
2019-01-03 03:00:29,108 INFO mapreduce.Job: The url to track the job: http://master:8088/proxy/application_1546499638284_0001/
2019-01-03 03:00:29,108 INFO mapreduce.Job: Running job: job_1546499638284_0001
2019-01-03 03:00:57,004 INFO mapreduce.Job: Job job_1546499638284_0001 running in uber mode : false
2019-01-03 03:00:57,005 INFO mapreduce.Job:  map 0% reduce 0%
2019-01-03 03:00:57,037 INFO mapreduce.Job: Job job_1546499638284_0001 failed with state FAILED due to: Application application_1546499638284_0001 failed 2 times due to AM Container for appattempt_1546499638284_0001_000002 exited with  exitCode: 1
Failing this attempt.Diagnostics: [2019-01-03 03:00:54.015]Exception from container-launch.
Container id: container_1546499638284_0001_02_000001
Exit code: 1

[2019-01-03 03:00:54.112]Container exited with a non-zero exit code 1. Error file: prelaunch.err.
Last 4096 bytes of prelaunch.err :
Last 4096 bytes of stderr :
Error: Could not find or load main class org.apache.hadoop.mapreduce.v2.app.MRAppMaster

Please check whether your etc/hadoop/mapred-site.xml contains the below configuration:
<property>
  <name>yarn.app.mapreduce.am.env</name>
  <value>HADOOP_MAPRED_HOME=${full path of your hadoop distribution directory}</value>
</property>
<property>
  <name>mapreduce.map.env</name>
  <value>HADOOP_MAPRED_HOME=${full path of your hadoop distribution directory}</value>
</property>
<property>
  <name>mapreduce.reduce.env</name>
  <value>HADOOP_MAPRED_HOME=${full path of your hadoop distribution directory}</value>
</property>

[2019-01-03 03:00:54.113]Container exited with a non-zero exit code 1. Error file: prelaunch.err.
Last 4096 bytes of prelaunch.err :
Last 4096 bytes of stderr :
Error: Could not find or load main class org.apache.hadoop.mapreduce.v2.app.MRAppMaster

Please check whether your etc/hadoop/mapred-site.xml contains the below configuration:
<property>
  <name>yarn.app.mapreduce.am.env</name>
  <value>HADOOP_MAPRED_HOME=${full path of your hadoop distribution directory}</value>
</property>
<property>
  <name>mapreduce.map.env</name>
  <value>HADOOP_MAPRED_HOME=${full path of your hadoop distribution directory}</value>
</property>
<property>
  <name>mapreduce.reduce.env</name>
  <value>HADOOP_MAPRED_HOME=${full path of your hadoop distribution directory}</value>
</property>

For more detailed output, check the application tracking page: http://master:8088/cluster/app/application_1546499638284_0001 Then click on links to logs of each attempt.
. Failing the application.
2019-01-03 03:00:57,107 INFO mapreduce.Job: Counters: 0
```

解决方法：在mapred-site.xml中加入如下配置（集群每个节点都需要修改）

```
<property>
         <name>mapreduce.application.classpath</name>
         <value>
            /opt/hadoop/hadoop-3.1.0/etc/hadoop,
            /opt/hadoop/hadoop-3.1.0/share/hadoop/common/*,
            /opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/*,
            /opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/*,
            /opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/*,
            /opt/hadoop/hadoop-3.1.0/share/hadoop/mapreduce/*,
            /opt/hadoop/hadoop-3.1.0/share/hadoop/mapreduce/lib/*,
            /opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/*,
            /opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/lib/*
         </value>
     </property>
```



环境安装参考：[CentOs7 安装Hadoop-3.1.0集群环境](https://blog.csdn.net/boonya/article/details/80719245)




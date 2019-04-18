# Hadoop异常合集（更新中～） - Soul Joy Hub - CSDN博客

2016年07月04日 20:43:15[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：3729


# 格式化namenode造成datanode无法启动

```
16/08/05 12:56:15 WARN hdfs.DFSClient: DataStreamer Exception
org.apache.hadoop.ipc.RemoteException(java.io.IOException): File /user/sort.text._COPYING_ could only be replicated to 0 nodes instead of minReplication (=1).  There are 0 datanode(s) running and no node(s) are excluded in this operation.
```

> 
namenode和datanode的clusterID不一致。突然想起来之前格式化了两次namenode，应该是由于namenode多次格式化造成了namenode和datanode的clusterID不一致！每次格式化时，namenode会更新clusterID，但是datanode只会在首次格式化时确定，因此就造成不一致现象。 

  打开hdfs-site.xml里配置的datanode和namenode对应的目录，分别打开current文件夹里的VERSION，可以看到clusterID项正如日志里记录的一样，确实不一致. 

  修改datanode里VERSION文件的clusterID 与namenode里的一致，再重新启动dfs（执行start-dfs.sh）再执行jps命令可以看到datanode已正常启动。

# wrong key class

## Error Log

```
java.lang.Exception: java.io.IOException: wrong key class: class org.apache.hadoop.io.Text is not class org.apache.hadoop.io.IntWritable
    at org.apache.hadoop.mapred.LocalJobRunner$Job.runTasks(LocalJobRunner.java:462)
    at org.apache.hadoop.mapred.LocalJobRunner$Job.run(LocalJobRunner.java:522)
Caused by: java.io.IOException: wrong key class: class org.apache.hadoop.io.Text is not class org.apache.hadoop.io.IntWritable
    at org.apache.hadoop.mapred.IFile$Writer.append(IFile.java:191)
    at org.apache.hadoop.mapred.Task$CombineOutputCollector.collect(Task.java:1350)
    at org.apache.hadoop.mapred.Task$NewCombinerRunner$OutputConverter.write(Task.java:1667)
    at org.apache.hadoop.mapreduce.task.TaskInputOutputContextImpl.write(TaskInputOutputContextImpl.java:89)
    at org.apache.hadoop.mapreduce.lib.reduce.WrappedReducer$Context.write(WrappedReducer.java:105)
    at FindMaxValue.FindMaxValueReducer.reduce(FindMaxValueReducer.java:32)
    at FindMaxValue.FindMaxValueReducer.reduce(FindMaxValueReducer.java:1)
    at org.apache.hadoop.mapreduce.Reducer.run(Reducer.java:171)
    at org.apache.hadoop.mapred.Task$NewCombinerRunner.combine(Task.java:1688)
    at org.apache.hadoop.mapred.MapTask$MapOutputBuffer.sortAndSpill(MapTask.java:1637)
    at org.apache.hadoop.mapred.MapTask$MapOutputBuffer.flush(MapTask.java:1489)
    at org.apache.hadoop.mapred.MapTask$NewOutputCollector.close(MapTask.java:723)
    at org.apache.hadoop.mapred.MapTask.runNewMapper(MapTask.java:793)
    at org.apache.hadoop.mapred.MapTask.run(MapTask.java:341)
    at org.apache.hadoop.mapred.LocalJobRunner$Job$MapTaskRunnable.run(LocalJobRunner.java:243)
    at java.util.concurrent.Executors$RunnableAdapter.call(Executors.java:471)
    at java.util.concurrent.FutureTask.run(FutureTask.java:262)
    at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1145)
    at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:615)
    at java.lang.Thread.run(Thread.java:745)
```

再看下main函数：

```java
public static void main(String[] args) throws IOException, ClassNotFoundException, InterruptedException {
        Configuration conf = new Configuration();
        @SuppressWarnings("deprecation")
        Job job = new Job(conf,"FindMaxValue");

        job.setJarByClass(MaxValueDriver.class);
        job.setMapperClass(FindMaxValueMapper.class);
        job.setCombinerClass(FindMaxValueReducer.class);
        job.setReducerClass(FindMaxValueReducer.class);
        job.setInputFormatClass(FindMaxValueInputFormat.class);
        job.setMapOutputKeyClass(IntWritable.class);
        job.setMapOutputValueClass(FloatWritable.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(FloatWritable.class);
        FileOutputFormat.setOutputPath(job, new Path(args[0]));
        System.out.println(conf.get("mapred.job.tracker"));
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
```

可以看到job.setOutputKeyClass(Text.class)，但是为什么代码要的是IntWritable呢？ 

原因是

> 
Combiner是Mapper端运行的，所以Combiner的配置是用Mapper的，即：

```java
job.setMapOutputKeyClass(IntWritable.class);
        job.setMapOutputValueClass(FloatWritable.class);
```

解决方法是：

> 
要么不setCombinerClass，要么写一个符合Mapper配置的CombinerClass，而不是直接用ReducerClass

# java.net.ConnectException

## Error Log

```
hadoop jar  FilesystemReader.jar  FilesystemReader.Main  hdfs://127.0.0.1/user/hadoop/file
Exception in thread "main" java.net.ConnectException: Call From caizhenwei-Inspiron-3847/127.0.1.1 to localhost:8020 failed on connection exception: java.net.ConnectException: 拒绝连接;
```

原因是端口号没设置，查看core-site.xml：

```xml
<property>
            <name>fs.default.name</name>
            <value>hdfs://localhost:9000</value>
  </property>
```

加上端口号，通过：
`hadoop jar  FilesystemReader.jar  FilesystemReader.Main  hdfs://127.0.0.1:9000/user/hadoop/file`
# JAVA_HOME is not set and could not be found.

## Error Log

```
root@soso-Mai-II:/opt/hadoop# ./sbin/start-dfs.sh
Starting namenodes on [localhost]
localhost: Error: JAVA_HOME is not set and could not be found.
localhost: Error: JAVA_HOME is not set and could not be found.
Starting secondary namenodes [0.0.0.0]
0.0.0.0: Error: JAVA_HOME is not set and could not be found.
```

看下环境变量：

```
root@soso-Mai-II:/opt/hadoop# echo $JAVA_HOME
/usr/lib/jvm/java-8-openjdk-amd64
```

奇怪，发现JAVA_HOME已经设好了啦～ 

摸索半天发现，hadoop-env.sh里面有坑。把

```bash
export JAVA_HOME=${JAVA_HOME}
```

换成
`export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64`
完美解决：

```
root@soso-Mai-II:/opt/hadoop# ./sbin/start-dfs.sh 
Starting namenodes on [localhost]
localhost: starting namenode, logging to /opt/hadoop/logs/hadoop-root-namenode-soso-Mai-II.out
localhost: starting datanode, logging to /opt/hadoop/logs/hadoop-root-datanode-soso-Mai-II.out
Starting secondary namenodes [0.0.0.0]
0.0.0.0: starting secondarynamenode, logging to /opt/hadoop/logs/hadoop-root-secondarynamenode-soso-Mai-II.out
root@soso-Mai-II:/opt/hadoop# jps
17314 Jps
17189 SecondaryNameNode
16857 NameNode
17002 DataNode
```

# java.lang.UnsupportedClassVersionError

## Error Log

```
hadoop jar WordCount.jar WordCount.WordCount /user/hadoop/file /user/hadoop/result
Exception in thread "main" java.lang.UnsupportedClassVersionError: WordCount/WordCount : Unsupported major.minor version 52.0
    at java.lang.ClassLoader.defineClass1(Native Method)
    at java.lang.ClassLoader.defineClass(ClassLoader.java:800)
    at java.security.SecureClassLoader.defineClass(SecureClassLoader.java:142)
    at java.net.URLClassLoader.defineClass(URLClassLoader.java:449)
    at java.net.URLClassLoader.access$100(URLClassLoader.java:71)
    at java.net.URLClassLoader$1.run(URLClassLoader.java:361)
    at java.net.URLClassLoader$1.run(URLClassLoader.java:355)
    at java.security.AccessController.doPrivileged(Native Method)
    at java.net.URLClassLoader.findClass(URLClassLoader.java:354)
    at java.lang.ClassLoader.loadClass(ClassLoader.java:425)
    at java.lang.ClassLoader.loadClass(ClassLoader.java:358)
    at java.lang.Class.forName0(Native Method)
    at java.lang.Class.forName(Class.java:274)
    at org.apache.hadoop.util.RunJar.run(RunJar.java:214)
    at org.apache.hadoop.util.RunJar.main(RunJar.java:136)
```

> 
编译的时候java版本太新了。试着从1.8改成1.7，还是不行。改成1.6，通过了～

# java.lang.ClassNotFoundException

## Error Log

```
hadoop jar WordCount.jar /user/hadoop/file /user/hadoop/result
Exception in thread "main" java.lang.ClassNotFoundException: /user/hadoop/file
    at java.lang.Class.forName0(Native Method)
    at java.lang.Class.forName(Class.java:274)
    at org.apache.hadoop.util.RunJar.run(RunJar.java:214)
    at org.apache.hadoop.util.RunJar.main(RunJar.java:136)
```

被书坑，命令应该是：

> 
hadoop jar [jar包] [项目包名.主类名] [HDFS in dir] [HDFS out dir]

# java.io.FileNotFoundException: File file does not

## Error Log

```
Exception in thread "main" java.io.FileNotFoundException: File file:/root/tpcds/generator/target/lib/dsdgen.jar does not exist
    at org.apache.hadoop.fs.RawLocalFileSystem.deprecatedGetFileStatus(RawLocalFileSystem.java:534)
    at org.apache.hadoop.fs.RawLocalFileSystem.getFileLinkStatusInternal(RawLocalFileSystem.java:747)
    at org.apache.hadoop.fs.RawLocalFileSystem.getFileStatus(RawLocalFileSystem.java:524)
    at org.apache.hadoop.fs.FilterFileSystem.getFileStatus(FilterFileSystem.java:424)
    at org.apache.hadoop.fs.FileUtil.copy(FileUtil.java:340)
    at org.apache.hadoop.fs.FileSystem.copyFromLocalFile(FileSystem.java:1944)
    at org.notmysock.tpcds.GenTable.copyJar(GenTable.java:128)
    at org.notmysock.tpcds.GenTable.run(GenTable.java:71)
    at org.apache.hadoop.util.ToolRunner.run(ToolRunner.java:70)
    at org.notmysock.tpcds.GenTable.main(GenTable.java:28)
    at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
    at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:57)
    at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
    at java.lang.reflect.Method.invoke(Method.java:606)
    at org.apache.hadoop.util.RunJar.run(RunJar.java:221)
    at org.apache.hadoop.util.RunJar.main(RunJar.java:136)
```

组长回了我一句话：

> 
不要把文件放在root目录下

==|| 完美解决

# HDFS相应目录没有写权限的问题

> 
hdfs dfs -chmod 777 相应目录

但是不知道为什么不起作用。

## [网上查到](http://stackoverflow.com/questions/11593374/permission-denied-at-hdfs):

### 方法一（对我没用）：

> 
I solved this problem temporary by disabling the dfs permission.By adding below property code to conf/hdfs-site.xml

```xml
<property>
      <name>dfs.permissions</name>
      <value>false</value>
    </property>
```

### 方法二：

> 
改变环境变量

```bash
export HADOOP_USER_NAME=hdfs
```

完美解决～ 


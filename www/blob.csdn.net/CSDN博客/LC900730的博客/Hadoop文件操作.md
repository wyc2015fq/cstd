# Hadoop文件操作 - LC900730的博客 - CSDN博客
2017年11月23日 22:32:11[lc900730](https://me.csdn.net/LC900730)阅读数：138
### Jobtracker
Jobtracker守护进程是应用程序和Hadoop之间的纽带。一旦代码提交到集群上，JobTracker就会确定执行计划，包括决定处理哪些文件，为不同的任务分配节点以及监控所有任务的运行。如果任务失败，JobTracker将会自动重启任务，但是所分配的节点可能会不同，同时会受到预定义的重试次数限制。 
每个Hadoop集群只有一个JobTracker守护进程，通常运行在服务器集群的主节点上。
### Tasktracker
与守护进程类似，计算的守护进程也遵循主从架构：JobTracker作为主节点，监测。
## 文件API
#### FileSystem类
一个与文件系统交互的抽象类，存在与不同的具体实现子类用来处理不同HDFS和本地文件系统。
##### FileSystem.get(Configuration conf)得到需要的FileSystem实例
##### Configuration类是保存键/值配置参数的特殊类,默认实例化方法是以HDFS系统的资源配置为基础的。
```
Configuration conf=new Configuration();
FileSystem hdfs=FileSysten.get(conf);
//得到一个专用于本地文件系统的FileSystem对象
FileSystem local=FileSystem.getLocal(conf);
```
##### FileStatus对象来存储文件和目录的元数据。
```
FileStatus[] inputFiles=local.listStatus(inputDir);
//count(inputFiles):等于目录中文件个数，数组中每一个FileStatus对象均有元数据信息，如文件长度、权限、修改时间等。
```
#### 读取文件
每个文件的path，即inputFiles[i].getPath(),可以通过FSDataInputStream对象来访问这个path读取文件。 
FSDataInputStream in=local.open(inputFiles[i].getPath());
#### 写入文件
```
Path hdfsFile=new Path(args[1]);
FSDataOutputStream out=hdfs.create(hdfsFile);
out.write(buffer,0,bytesRead);
out.close();
```

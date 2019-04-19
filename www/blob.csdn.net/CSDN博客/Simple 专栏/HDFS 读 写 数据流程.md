# HDFS  读/写 数据流程 - Simple 专栏 - CSDN博客
2015年03月20日 04:24:36[Simple_Zz](https://me.csdn.net/love284969214)阅读数：433
所属专栏：[Hadoop 浅谈](https://blog.csdn.net/column/details/28029.html)
# HDFS  写数据
![HDFS 写数据流程](https://img-blog.csdn.net/20180929111840644?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xvdmUyODQ5NjkyMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)HDFS 写数据流程
- 客户端通过 Distributed FileSystem 模块向 namenode 请求上传文件，namenode 检查目标文件是否已存在，父目录是否存在。
- namenode 返回是否可以上传。
- 客户端请求第一个 block 上传到哪几个 datanode 服务器上。
- namenode 返回3个 datanode 节点，分别为 dn1、dn2、dn3。
- 客户端通过 FSDataOutputStream 模块请求 dn1 上传数据，dn1 收到请求会继续调用 dn2，然后 dn2 调用 dn3，将这个通信管道建立完成。
- dn1、dn2、dn3 逐级应答客户端。
- 客户端开始往 dn1 上传第一个 block （先从磁盘读取数据放到一个本地内存缓存），以 packet 为单位，dn1 收到一个packet 就会传给 dn2，dn2 传给 dn3；dn1 每传一个 packet 会放入一个应答队列等待应答。
- 当一个 block 传输完成之后，客户端再次请求 namenode 上传第二个 block 的服务器。（重复执行3-7步）。
# HDFS  读数据
![HDFS 读数据流程](https://img-blog.csdn.net/2018092911374494?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xvdmUyODQ5NjkyMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)HDFS 读数据流程
- 客户端通过 Distributed FileSystem向namenode 请求下载文件，namenode 通过查询元数据，找到文件块所在的 datanode地址。
- 挑选一台 datanode（就近原则，然后随机）服务器，请求读取数据。
- datanode 开始传输数据给客户端（从磁盘里面读取数据输入流，以 packet 为单位来做校验）。
- 客户端以 packet 为单位接收，先在本地缓存，然后写入目标文件。
# 写数据的一致性
```java
// 向 HDFS 上写数据
@Test
public void writeFile() throws Exception {
    // 1 获取文件系统
    Configuration conf = new Configuration();
    // 2 获取文件系统
    FileSystem fs = FileSystem.get(new URI("hdfs://hadoop102:9000"), conf, "user001");
    // 3 获取输出流
    FSDataOutputStream fos = fs.create(new Path("/hello.txt"));
    // 4 写数据
    fos.write("hello".getBytes());
    // 5 一致性刷新
    fos.hflush();
    // 6 关闭资源
    fos.close();
    fs.close();
}
```
写入数据时，如果希望数据被其他 client 立即可见，调用如下方法：
**FsDataOutputStream.hflush();        //清理客户端缓冲区数据，被其他client立即可见。**

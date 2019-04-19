# HDFS  JavaAPI  操作 - Simple 专栏 - CSDN博客
2015年03月09日 11:31:44[Simple_Zz](https://me.csdn.net/love284969214)阅读数：4056标签：[Hadoop																[大数据																[HDFS](https://so.csdn.net/so/search/s.do?q=HDFS&t=blog)](https://so.csdn.net/so/search/s.do?q=大数据&t=blog)](https://so.csdn.net/so/search/s.do?q=Hadoop&t=blog)
个人分类：[hadoop生态圈](https://blog.csdn.net/love284969214/article/category/7975457)
所属专栏：[Hadoop 浅谈](https://blog.csdn.net/column/details/28029.html)
### HDFS  获取文件系统
```java
@Test
public void initHDFS() throws Exception{
    // 1 创建配置信息对象
    Configuration conf = new Configuration();
    // 2 获取文件系统
    FileSystem fs = FileSystem.get(conf);
    // 3 打印文件系统
    System.out.println(fs.toString());
}
```
### HDFS  文件上传
```java
@Test
public void testCopyFromLocalFile() throws Exception {
    // 1 创建配置信息对象
    Configuration conf = new Configuration();
    // 1.1 设置副本数，代码设置优先级最高
    configuration.set("dfs.replication", "2");
    // 2 获取文件系统
    FileSystem fs = FileSystem.get(new URI("hdfs://hadoop102:9000"), conf, "user001");
    // 3 上传文件
    fs.copyFromLocalFile(new Path("e:/hello.txt"), new Path("/hello.txt"));
    // 4 关闭资源
    fs.close();
}
```
### HDFS  文件下载
```java
@Test
public void testCopyToLocalFile() throws Exception {
    // 1 创建配置信息对象
    Configuration conf = new Configuration();
    // 1.1 设置副本数，代码设置优先级最高
    configuration.set("dfs.replication", "2");
    // 2 获取文件系统
    FileSystem fs = FileSystem.get(new URI("hdfs://hadoop102:9000"), conf, "user001");
    // 3 执行下载操作
    // boolean delSrc 指是否将原文件删除
    // Path src 指要下载的文件路径
    // Path dst 指将文件下载到的路径
    // boolean useRawLocalFileSystem 是否开启文件效验
    fs.copyToLocalFile(false, new Path("/hello1.txt"), new Path("e:/hello1.txt"), true);
    // 4 关闭资源
    fs.close();
}
```
### HDFS  目录创建
```java
@Test
public void testMkdirs() throws Exception {
    // 1 创建配置信息对象
    Configuration conf= new Configuration();
    // 2 获取文件系统
    FileSystem fs = FileSystem.get(new URI("hdfs://hadoop102:9000"), conf, "user001");
    // 3 创建目录
    fs.mkdirs(new Path("/2015/0309/dataSource"));
    // 4 关闭资源
    fs.close();
}
```
### HDFS  删除文件夹
```java
@Test
public void testDelete() throws Exception {
    // 1 创建配置信息对象
    Configuration conf = new Configuration();
    // 2 获取文件系统
    FileSystem fs = FileSystem.get(new URI("hdfs://hadoop102:9000"), conf, "user001");
    // 3 创建目录
    // 返回值 是否删除成功
    // 参数2  是否递归删除
    boolean flag = fs.delete(new Path("/2015/"), true);
    // 4 关闭资源
    fs.close();
}
```
### HDFS  文件名更改
```java
@Test
public void testRename() throws Exception {
    // 1 创建配置信息对象
    Configuration conf = new Configuration();
    // 2 获取文件系统
    FileSystem fs = FileSystem.get(new URI("hdfs://hadoop102:9000"), conf, "user001");
    // 3 创建目录
    fs.rename(new Path("/hello.txt"), new Path("/hello1.txt"));
    // 4 关闭资源
    fs.close();
}
```
### HDFS  文件详情查看
```java
@Test
public void testListFiles() throws Exception {
    // 1 创建配置信息对象
    Configuration conf = new Configuration();
    // 2 获取文件系统
    FileSystem fs = FileSystem.get(new URI("hdfs://hadoop102:9000"), conf, "user001");
    // 3 获取文件详情
    RemoteIterator<LocatedFileStatus> listFiles = fs.listFiles(new Path("/"), true);
    while(listFiles.hasNext()){
        LocatedFileStatus status = listFiles.next();
        // 文件名称
        System.out.println(status.getPath().getName());
        // 长度
        System.out.println(status.getLen());
        // 权限
        System.out.println(status.getPermission());
        // 组
        System.out.println(status.getGroup());
        // 获取存储的块信息
        BlockLocation[] blockLocations = status.getBlockLocations();
        for (BlockLocation blockLocation : blockLocations) {
            // 获取块存储的主机节点
            String[] hosts = blockLocation.getHosts();
            for (String host : hosts) {
                System.out.println(host);
            }
        }
        System.out.println("----------------华丽的分割线----------------");
    }
    // 4 关闭资源
    fs.close();
}
```
### HDFS  文件和文件夹判断
```java
@Test
public void testListStatus() throws Exception {
    // 1 创建配置信息对象
    Configuration conf = new Configuration();
    // 2 获取文件系统
    FileSystem fs = FileSystem.get(new URI("hdfs://hadoop102:9000"), conf, "user001");
    // 3 判断是否是文件还是文件夹
    FileStatus[] listStatus = fs.listStatus(new Path("/"));
    for (FileStatus fileStatus : listStatus) {
        // 如果是文件
        if (fileStatus.isFile()) {
            System.out.println("file:" + fileStatus.getPath().getName());
        } else {
            System.out.println("dict:" + fileStatus.getPath().getName());
        }
    }
    // 4 关闭资源
    fs.close();
}
```

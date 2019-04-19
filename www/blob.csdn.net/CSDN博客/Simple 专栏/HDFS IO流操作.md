# HDFS  IO流操作 - Simple 专栏 - CSDN博客
2015年03月18日 02:16:43[Simple_Zz](https://me.csdn.net/love284969214)阅读数：405
所属专栏：[Hadoop 浅谈](https://blog.csdn.net/column/details/28029.html)
### HDFS  文件上传
```java
@Test
public void putFileToHDFS() throws Exception {
    // 1 创建配置信息对象
    Configuration conf = new Configuration();
 
    // 2 获取文件系统
    FileSystem fs = FileSystem.get(conf);
    // 3 创建输入流
    FileInputStream fis = new FileInputStream(new File("e:/hello.txt"));
    // 4 获取输出流
    FSDataOutputStream fos = fs.create(new Path("/hello.txt"));
    // 5 流对拷
    IOUtils.copyBytes(fis, fos, conf);
    // 6 关闭资源
    IOUtils.closeStream(fis);
    IOUtils.closeStream(fos);
    fs.close();
}
```
### HDFS  文件下载
```java
@Test
public void getFileFromHDFS() throws Exception {
    // 1 创建配置信息对象
    Configuration conf = new Configuration();
 
    // 2 获取文件系统
    FileSystem fs = FileSystem.get(conf);
    // 3 创建输入流
    FSDataInputStream fis = fs.open(new Path("/hello.txt"));
    // 4 获取输出流
    FileOutputStream fos = new FileOutputStream(new File("e:/helloTest.txt"));
    // 5 流对拷
    IOUtils.copyBytes(fis, fos, conf);
    // 6 关闭资源
    IOUtils.closeStream(fis);
    IOUtils.closeStream(fos);
    fs.close();
}
```
### HDFS  定位读取
```java
// 下载大文件的第一块数据
@Test
public void getFileFromHDFSSeek1() throws Exception {
    // 1 创建配置信息对象
    Configuration conf = new Configuration();
 
    // 2 获取文件系统
    FileSystem fs = FileSystem.get(conf);
	
	// 3 获取输入流
    FSDataInputStream fis = fs.open(new Path("/user/input/hadoop-2.6.0.tar.gz"));
    // 4 创建输出流
    FileOutputStream fos = new FileOutputStream(new File("e:/hadoop-2.6.0.tar.gz.part1"));
    // 5 流对接（只读取128m）
    byte[] buf = new byte[1024];
    // 1024 * 1024 * 128
    for(int i = 0; i < 1024 * 128; i++){
        fis.read(buf);
        fos.write(buf);
    }
    // 6 关闭资源
    IOUtils.closeStream(fis);
    IOUtils.closeStream(fos);
    fs.close();
}
// 下载大文件的第二块数据
@Test
public void getFileFromHDFSSeek2() throws Exception {
    // 1 创建配置信息对象
    Configuration conf = new Configuration();
 
    // 2 获取文件系统
    FileSystem fs = FileSystem.get(conf);
    // 3 获取输入流
    FSDataInputStream fis = fs.open(new Path("/user/input/hadoop-2.7.2.tar.gz"));
    // 4 创建输出流
    FileOutputStream fos = new FileOutputStream(new File("e:/hadoop-2.7.2.tar.gz.part2"));
    // 5 流对接（指向第二块数据的首地址） 定位到128m
    fis.seek(1024*1024*128);
    // 6 流对拷
    IOUtils.copyBytes(fis, fos, conf);
    // 7 关闭资源
    IOUtils.closeStream(fis);
    IOUtils.closeStream(fos);
    fs.close();
}
```

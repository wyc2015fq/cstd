# Hadoop中的Java api - LC900730的博客 - CSDN博客
2017年12月05日 22:35:27[lc900730](https://me.csdn.net/LC900730)阅读数：362
从Hadoop文件系统中读取文件
## 1.通过java.net.URL对象打开数据流，读取数据
```
InputStream in=null；
try{
    in=new URL('hdfs://host/path').openStream();
}finally{
    IOUtils.closeStream(in);
}
```
## 2.通过FileSystem API读取数据
FileSystem是一个普通的文件系统API，所以我们首要任务是检索我们要用的文件系统实例，这里是HDFS。取得FileSystem实例有两种静态工厂方法：
```java
public static FileSystem get(Configuration conf) throws IOException
```
```java
public static FileSystem get(URI uri,Configuration conf) throws IOException
```
#### Configuration对象封装了一个客户端或者服务器的配置，这是用从类路径读取而来的配置文件(如conf/core-site.xml)来设置的。第一个方法返回的是默认文件系统(在conf/core-site.xml中设置的，如果没有设置过，则是默认本地文件系统)。第二个方法是使用指定URI方案及决定所用文件系统的权限，如果指定URI中没有指定方案，则退回默认文件系统。
有了FileSystem实例后，我们调用open()来得到一个文件输入流。
## 上传
```java
public class HdfsClientDemo {
    FileSystem fs=null;
    @Before
    public void init() throws Exception {
        Configuration conf=new Configuration();
        //拿到一个文件系统的客户端实例
        conf.set("fs.defaultFS","hdfs://192.168.100.93:9000");
        //fs=FileSystem.get(new URI("hdfs://192.168.100.93:9000"),conf,"hadoop");
        fs=FileSystem.get(conf);
    }
    @Test
    public void testUpload() throws IllegalArgumentException, IOException {
        fs.copyFromLocalFile(new Path("fh_php.sql"), new Path("/"));
        fs.close();
    }
```
## 下载
## 执行任务
java -cp wordcount.jar cn.itcast.bigdata.mr.wcdemo.WordcountDriver /wcount/input /wcount/output
hadoop jar wordcount.jar cn.itcast.bigdata.mr.wcdemo.WordcountDriver /wcount/input /wcount/output

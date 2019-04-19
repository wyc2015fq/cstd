# JavaApi操作Hadoop - LC900730的博客 - CSDN博客
2017年11月20日 18:47:56[lc900730](https://me.csdn.net/LC900730)阅读数：120标签：[hadoop](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)
个人分类：[hadoop](https://blog.csdn.net/LC900730/article/category/7272965)
//在上传的时候出现权限问题，解决方法是hdfs-site.xml中
```xml
<property>
    <name>fs.permissions</name>
    <value>false</value>
</property>
```
并且将hdfs的data目录权限开放。
### Java Api上传
```java
package cn.itcast.bigdata.hdfs;
import java.io.IOException;
import java.net.URI;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.junit.Before;
import org.junit.Test;
public class HdfsClientDemo {
    //客户端去操作hdfs时候，是有一个用户参数的
    //默认情况下，hdfs客户端api是从jvm中获取一个参数DHADOOP_USER_NAME
    //也可以在构造中通过代码中告诉身份
    FileSystem fs=null;
    @Before
    public void init() throws Exception {
        Configuration conf=new Configuration();
        conf.set("fs.defaultFS", "hdfs://www.tonote.cn:9000");
        //拿到一个文件系统的客户端实例
        fs=FileSystem.get(conf);
        //或者是：
        //第3个参数：用户
        fs=FileSystem.get(new URI("hdfs://www.tonote.cn:9000"),conf,"hadoo");
        fs=FileSystem.get(conf);
    }
    @Test
    public void testUpload() throws Exception {
        fs.copyFromLocalFile(new Path("/Users/liuchao/Downloads/fh_php.sql"), new Path("/liuchao"));
        fs.close();
    }
```
### Java Api下载

# hadoop部署错误集锦 - 三少GG - CSDN博客
2013年09月03日 18:24:37[三少GG](https://me.csdn.net/scut1135)阅读数：1400
个人分类：[云计算/大数据](https://blog.csdn.net/scut1135/article/category/1449467)

注意 伪分布式 和 分布式 的不同！！！环境移植时会出现以下问题
**1. 错误： readObject can't find class**
caught: java.lang.RuntimeException:
readObject can't find class
解决： JobConf job = new JobConf();
改为 JobConf job = new (getConf(), BloomFilterJoin.class);
进一步： （未尝试） ((JobConf) job.getConfiguration()).setJar("pr.jar");
  
**2.   DistributedCache 返回null**
1) 注意路径！
** conf.get("fs.default.name");**
 conf.set("fs.default.name", "hdfs://master:9000");
  
2) 注意 getCacheFile() 和 getLocalCacheFile()的区别
跟具体的分布式 或 伪分布式有关！！！ 
解决办法： 可以 先用getLocalCacheFile() 若为空， 再getCacheFile() 
Subject: Re: reading distributed cache returns null pointer
**The DistributedCache behavior is not symmetrical in local mode vsdistributed mode.**
As I replied earlier, you need to use
DistributedCache.getCacheFiles() in distributed mode.
In your code, you can put  a check:
if (getLocalCacheFiles()) returns null then use getCacheFiles()) instead. Or
use the right API depending upon the mode you are executing in
Thanks Rahul... That worked. **Using DistributedCache.getCacheFiles() in distributed mode and ****DistributedCache.getLocalCacheFiles() in pseudo-distributed mode.**
**3. 序列化**
 特殊处理
java.lang.NullPointerException
        at xxxjoin$TaggedWritable.readFields(xxxjoin.java:166)
        at org.apache.hadoop.io.serializer.WritableSerialization$WritableDeserializer.deserialize(WritableSerialization.java:67)
readFields按照 stackoverflow 回复stackoverflow.com/questions/10201500/hadoop-reduce-side-join-using-datajoin 一模一样处理！尤其是为空的判断！
4.   java heap over
**亦可以在mapreduce代码里通过全局变量更改**。 job.set("mapred.child.java.opts", "-Xmx512m");
启动虚拟机的时候，加上一个参数：-Xms800m -Xmx800m就好了 
**-Xms <size> 设置JVM初始化堆内存大小 **
-Xmx <size> 
设置JVM最大的堆内存大小 
如果是应用程序，则：java -Xms800m -Xmx800m 你的类名 
5.

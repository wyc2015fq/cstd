# 用java提交一个Spark应用程序 - Spark高级玩法 - CSDN博客
2017年12月09日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：2885
第一种方式
经常有人在公众号留言或者在群里问浪尖，如何使用java代码提交一个Spark应用程序。在这里浪尖介绍一种简单的方法来实现这个功能。
首先用vim打开一个文件，MyLauncher.java
编写代码如下：
import org.apache.spark.launcher.SparkAppHandle;
import org.apache.spark.launcher.SparkLauncher;
import java.util.HashMap;
public class MyLauncher {
 public static void main(String[] args) throws Exception {
HashMap<String,String> map = newHashMap<String, String>();
       map.put("HADOOP_CONF_DIR","/home/hadoop/conf");
       map.put("YARN_CONF_DIR","/home/hadoop/conf");
       map.put("SPARK_CONF_DIR","/home/hadoop/spark/conf");
     new SparkLauncher(map)
        .setAppResource("/data/newStreaming/uesc-analyzer.jar")
        .setMainClass("ucloud.UESBash.testSchema")
        .setMaster("yarn-cluster")
        .setConf(SparkLauncher.DRIVER_MEMORY, "2g")
       .setVerbose(true).startApplication();
       Thread.sleep(100000);
      // Use handle API to monitor / control application.
}
}
接着，进行编译
javac -cp /home/hadoop/spark/lib/spark-assembly-1.6.0-hadoop2.6.0-cdh5.4.9.jarMyLauncher.java
然后提交执行
java -cp/home/hadoop/spark/lib/spark-assembly-1.6.0-hadoop2.6.0-cdh5.4.9.jar:.MyLauncher
这样就可以在yarn的界面上看到运行情况了。
注释：可能你比较奇怪我为啥在那加了个停顿，因为正常情况下我们要进行一些其它操作监控Spark应用而不是直接退出，直接退出可能会导致提交还没完成退出，以至于应用程序不能被提交了。
当然，还有另外一种方式，但是不建议使用，上面采用的这种方式呢可以更好地监控我们Spark程序的状态。
**第二种方式**
import org.apache.spark.launcher.SparkAppHandle;
import org.apache.spark.launcher.SparkLauncher;
import java.util.HashMap;
public class MyLauncher {
 public static void main(String[] args) throws Exception {
HashMap<String,String> map = newHashMap<String, String>();
       map.put("HADOOP_CONF_DIR","/home/hadoop/conf");
       map.put("YARN_CONF_DIR","/home/hadoop/conf");
       map.put("SPARK_CONF_DIR","/home/hadoop/spark/conf");
    Process spark  = new SparkLauncher(map)
        .setAppResource("/data/newStreaming/uesc-analyzer.jar")
        .setMainClass("ucloud.UESBash.testSchema")
        .setMaster("yarn-cluster")
        .setConf(SparkLauncher.DRIVER_MEMORY, "2g")
       .setVerbose(true).launch();
      // Use handle API to monitor / control application.
       spark.waitFor();    
}
}
第二种方式是启动了一个子进程来加载提交应用程序。
至于SparkLauncher其它标准函数操作，比如如何传参，如何指定内存，cpu，如何指定钩子函数这些浪尖就不在这里啰嗦了，有兴趣可以去翻看这个类的源码。也可以在公众号里输入:**submit**，来获取这个类和钩子函数使用的复杂代码。
** 推荐阅读：**
1，[读懂Word2Vec之Skip-Gram](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484321&idx=1&sn=ec14f031ffc7dbc699502a9d0b09f64b&chksm=9f38e089a84f699fb1ca5903124a69ab2286ad5159c6b90d8ee677f1bdcde523a121a8249ced&scene=21#wechat_redirect)
2，[CountVectorizer](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484313&idx=1&sn=939b59d6ae32963d4418390db988dc85&chksm=9f38e0b1a84f69a773fe140c21eaae240fd3f398d780d8afe79212c3f89701c66e85b3fa34d5&scene=21#wechat_redirect)
3，[spark调优系列之内存和GC调优](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483782&idx=1&sn=52059883b33cb97005186b0cba4e5f5a&chksm=9f38e2aea84f6bb8230189ee4e3de9cc7e11497f20f6cd498e540ef7f713478481c830106f51&scene=21#wechat_redirect)
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)
**关于Spark高级玩法**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**


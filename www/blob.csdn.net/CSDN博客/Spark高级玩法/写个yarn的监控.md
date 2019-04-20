# 写个yarn的监控 - Spark高级玩法 - CSDN博客
2018年10月25日 00:06:21[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1011
在星球里和微信群里很多朋友都有疑惑，如何监控 yarn 上 spark 或者 mr 应用的存活状态，浪尖今天在这里分享一下实现方法，实际上只需要简单的几行代码即可。
首先是，pom文件，添加yarn相关的配置
```
<!-- https://mvnrepository.com/artifact/org.apache.hadoop/hadoop-common -->
    <dependency>
      <groupId>org.apache.hadoop</groupId>
      <artifactId>hadoop-common</artifactId>
      <version>2.7.4</version>
    </dependency>
    <!-- https://mvnrepository.com/artifact/org.apache.hadoop/hadoop-client -->
    <dependency>
      <groupId>org.apache.hadoop</groupId>
      <artifactId>hadoop-client</artifactId>
      <version>2.7.4</version>
    </dependency>
    <!-- https://mvnrepository.com/artifact/org.apache.hadoop/hadoop-yarn-api -->
    <dependency>
      <groupId>org.apache.hadoop</groupId>
      <artifactId>hadoop-yarn-api</artifactId>
      <version>2.7.4</version>
    </dependency>
    <!-- https://mvnrepository.com/artifact/org.apache.hadoop/hadoop-yarn-client -->
    <dependency>
      <groupId>org.apache.hadoop</groupId>
      <artifactId>hadoop-yarn-client</artifactId>
      <version>2.7.4</version>
    </dependency>
```
接着将yarn-site.xml配置文件放到resources目录下：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFW9ic3icRb5tjqCF0dUsf7QhFibINGlRl3DK5V4POv64RIh5RwuogOPq1OuhWGyhBL2WynEBC2wjG9EA/640)
具体实现代码其实很简单就是，通过yarnclient获取resourcemanager上 spark 或者 mapreduce的状态。
```
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.yarn.api.records.ApplicationReport;
import org.apache.hadoop.yarn.api.records.YarnApplicationState;
import org.apache.hadoop.yarn.client.api.YarnClient;
import org.apache.hadoop.yarn.conf.YarnConfiguration;
import org.apache.hadoop.yarn.exceptions.YarnException;
import java.io.IOException;
import java.util.EnumSet;
import java.util.List;
public class client {
    public static void main(String[] args){
        Configuration conf = new YarnConfiguration();
        YarnClient yarnClient = YarnClient.createYarnClient();
        yarnClient.init(conf);
        yarnClient.start();
        try {
            List<ApplicationReport> applications = yarnClient.getApplications(EnumSet.of(YarnApplicationState.RUNNING, YarnApplicationState.FINISHED));
            System.out.println("ApplicationId ============> "+applications.get(0).getApplicationId());
            System.out.println("name ============> "+applications.get(0).getName());
            System.out.println("queue ============> "+applications.get(0).getQueue());
            System.out.println("queue ============> "+applications.get(0).getUser());
        } catch (YarnException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        yarnClient.stop();
    }
}
```
可以看到，我们可以通过YarnApplicationState设置状态，来过滤调一些我们不需要的任务状态。状态列表如下：
```
public enum YarnApplicationState {
  /** Application which was just created. */
  NEW,
  /** Application which is being saved. */
  NEW_SAVING,
  /** Application which has been submitted. */
  SUBMITTED,
  /** Application has been accepted by the scheduler */
  ACCEPTED,
  /** Application which is currently running. */
  RUNNING,
  /** Application which finished successfully. */
  FINISHED,
  /** Application which failed. */
  FAILED,
  /** Application which was terminated by a user or admin. */
  KILLED
}
```
上述demo监控的是spark streaming 的状态，运行结果如下：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFW9ic3icRb5tjqCF0dUsf7QhFWk8z6TrtiatrchsOXMZ7Sn5MQJYy3zwwFlib7RaxA6GmpjKZnFsRACxA/640)
**这样，我们通过app name字段可以获取到存活的 spark 等任务，然后通过比对我们要监控的任务列表，不存在的发出告警即可。**
对于 spark streaming 或者 spark其他任务，可以通过一个配置来制定spark 任务在yarn上显示的name，设置的参数是
`new SparkConf().setAppName(this.getClass.getName)`this.getClass.getName该方式在yarn-client和 yarn-cluster有稍微的区别，具体可以测一下。
类名作为spark app的 name 还是很方便及便于识别的。
**推荐阅读：**
[如何简单写yarn app](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485317&idx=1&sn=31f48ab5504605278c3116954994c5c3&chksm=9f38e4ada84f6dbbba86c08daba7199a24c7204cee17cc5403205cc56bb80dcae53fa94f715b&scene=21#wechat_redirect)
[RDD的join和Dstream的join有什么区别？](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485690&idx=1&sn=7045e82caab456c3a16acf145181fc7c&chksm=9f38ebd2a84f62c4bc3aac48a523c9377115c456884e43809c924912b074446007bf75897c7b&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)

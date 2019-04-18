# Hadoop平台作业参数设置关于mapreduce.job.split.metainfo.maxsize的说明 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年04月20日 14:36:50[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：5838
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









                
Hadoop平台作业参数设置关于mapreduce.job.split.metainfo.maxsize的说明



1、MR程序时执行时报错：

   YarnRuntimeException: java.io.IOException:Split metadata size exceeded 10000000. 



2、原因分析：

   输入文件包括大量小文件或者文件目录，造成Splitmetainfo文件超过默认上限。




3、解决办法：



   在mapred-site.xml配置文件中：

   修改默认作业参数mapreduce.jobtracker.split.metainfo.maxsize =100000000 

   或者mapreduce.jobtracker.split.metainfo.maxsize = －1 （默认值是1000000)


  <property>

    <name>mapreduce.job.split.metainfo.maxsize</name>

    <value>10000000</value>

  </property>



4、深入分析：

   job.splitmetainfo该文件记录split的元数据信息，如input文件过多，记录的文件结构信息超出默认设置就会报错；

   这个机制也是Hadoop集群要求文件大小不能过小或目录过多，避免namenode出现元数据加载处理瓶颈，这种业务一般会出现在存储图片上。

   如block默认128M，则文件应大于这个，尽量合并小文件。



5、源码分析：

   org.apache.hadoop.mapreduce.split.JobSplit

   可以看出splitmetainfo存储的文件结构信息内容：

   @Override

    public String toString() {

      StringBuffer buf = new StringBuffer();

      buf.append("data-size : " + inputDataLength + "\n");

      buf.append("start-offset : " + startOffset + "\n");

      buf.append("locations : " + "\n");

      for (String loc : locations) {

        buf.append("  " + loc + "\n");

      }

      return buf.toString();

    }

  }
            



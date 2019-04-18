# Flume NG之Agent部署和sink配置HDFS且吐槽CSDN博客及客服态度 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月08日 08:23:14[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2680








实在是想对csdn博客吐槽，一天到晚要么发布不了，发布成功的居然还时不时看不到文章内容（空白的），有时还打不开博客，问客服就说换浏览器。我真想和csdn客服说，你妹的，你知不知道我是码农啊，初步的问题诊断是会的，如果不是怀疑csdn博客服务端问题，我会找你。

这篇文章是重复http://blog.csdn.net/fjssharpsword/article/details/60136224，因为这个时不时打开是空白，而摘要确有内容，有时又能正常打开。昨天下午5:20多分，和csdn客服说，然后妹纸居然下班了，不理会人了，这个客服态度经常是推卸责任，然后就是不理你，我也是醉了。实在是csdn博客坚持这么久，不想换一家。

先重复发布吧，还好之前有保存原稿，现在重新发布：




1、Flume 

   flume是一个分布式、可靠、和高可用的海量日志采集、聚合和传输的系统。支持在日志系统中定制各类数据发送方，用于收集数据；同时，Flume提供对数据进行简单处理，并写到各种数据接受方(比如文本、HDFS、Hbase等)的能力。

　 flume的数据流由事件(Event)贯穿始终，事件是Flume的基本数据单位，它携带日志数据(字节数组形式)并且携带有头信息。Event由Agent外部的Source生成，当Source捕获事件后会进行特定的格式化，然后Source会把事件推入(单个或多个)Channel中；Channel看作是一个缓冲区，它将保存事件直到Sink处理完该事件；Sink负责持久化日志或者把事件推向另一个Source。

   flume的一些核心概念：

   1）Agent：使用JVM 运行Flume。每台机器运行一个agent，但是可以在一个agent中包含多个sources和sinks。

   2）Client：生产数据，运行在一个独立的线程。

   3）Source ：从Client收集数据，传递给Channel。

   4）Sink ： 从Channel收集数据，运行在一个独立线程。

   5）Channel  ：连接 sources 和 sinks ，这个有点像一个队列。

   6）Events ： 可以是日志记录、 avro 对象等。


![](https://img-blog.csdn.net/20170308082205060?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


下面的部署和配置参照：http://flume.apache.org/FlumeUserGuide.html



2、Flume Agent部署



1）下载并解压jdk-7u67-linux-x64.gz，到/home/flume/

    #cd /tmp

    #tar -xzvf jdk-7u67-linux-x64.gz

    #cp -r /tmp/jdk1.7.0_67  /home/flume/



2）下载并解压apache-flume-1.7.0-bin.tar.gz，到/home/flume/

    #tar -xzvf /tmp/apache-flume-1.7.0-bin.tar.gz  -C /home/flume/



3） 修改flume-env.sh 配置文件，设置JAVA_HOME变量

    #cd /home/flume/apache-flume-1.7.0-bin/conf

    #cp flume-env.sh.template　flume-env.sh

    #vi flume-env.sh

     添加export JAVA_HOME=/home/flume/jdk1.7.0_67

         FLUME_CLASSPATH="/home/flume/apache-flume-1.7.0-bin"



3、配置flume-conf.properties文件

    #cd /home/flume/apache-flume-1.7.0-bin/conf

    #cp flume-conf.properties.template　flume-conf.properties


   1）查找FTP文件本地路径

   #cat /etc/vsftpd/vsftpd.conf

    找到user_config_dir=/etc/vsftpd/user_config_dir

   #ls -l /etc/vsftpd/user_config_dir

     查看下面具体ftp用户的配置文件，如用户名是user；

   #cat /etc/vsftpd/user_config_dir/user

     找到local_root=/data/user，即为ftp本地文件目录；

    将/data/user配置到source。



   2）Hadoop环境配置

     引入hadoop-common-2.7.3.2.5.3.0-37.jar、commons-configuration-1.6.jar、

         hadoop-auth-2.7.3.2.5.3.0-37.jar、hadoop-hdfs-2.7.3.2.5.3.0-37.jar、

         htrace-core-3.1.0-incubating.jar、commons-io-2.4.jar到apache-flume-1.7.0-bin/lib下

     引入core-site.xml和hdfs-site.xml到apache-flume-1.7.0-bin/conf下；

     namenode要在/etc/hosts上配置对应的IP地址；

     spoolDir的hdfs上目录要读写权限：

     #hadoop fs -chown -R mapred:hadoop /tmp/flume

     #hadoop fs -chmod -R 777 /tmp/flume


   3）#vi flume-conf.properties 配置如下




```
#agent
agent.sources=se
agent.sinks=sk
agent.channels=ch


#set source se
agent.sources.se.type=spooldir
agent.sources.se.spoolDir=/data/kaitong/gz/20170303/
agent.sources.se.channels=ch
agent.sources.se.fileHeader = false


#set sink sk
agent.sinks.sk.type=hdfs
agent.sinks.sk.hdfs.path=hdfs://namenode49:8020/tmp/flume/
agent.sinks.sk.channel=ch
agent.sinks.sk.hdfs.filePrefix = flume-
agent.sinks.sk.hdfs.round = true
agent.sinks.sk.hdfs.roundValue = 10
agent.sinks.sk.hdfs.roundUnit = minute
agent.sinks.sk.hdfs.useLocalTimeStamp = true


#set channel ch
# Use a channel which buffers events in memory
agent.channels.ch.type = memory
agent.channels.ch.capacity = 1000
agent.channels.ch.transactionCapacity = 100
```
4、启动Flume Agent并测试

   #cd /home/flume/apache-flume-1.7.0-bin/

   #bin/flume-ng agent -n agent -c conf -f conf/flume-conf.properties  -Dflume.root.logger=INFO,console



5、后续工作：



   1)如果集群通过kerberos认证的话，要配置kerberos认证；

     agent.sinks.sk.hdfs.kerberosPrincipal = etl/host

     agent.sinks.sk.hdfs.kerberosKeytab = /tmp/etl.app.keytab



   2)Flume支持引入第三方jar包 ，自定义拦截器，可将Flume采集到的数据进行序列化、结构化等预处理，最后每条数据生成一条Event数据返回。







# 关于Hadoop安全集群和非安全集群间Distcp的使用 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月15日 11:25:22[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4907
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









1、需求：有两个集群，网络中节点是互通的，现在要用distcp进行文件迁移，但一个集群是非安全集群，一个是配置了kerberos认证的安全集群，怎么执行呢？



2、前提：两个集群都做了HA配置，所以要通过如下命令查看活动Namenode并获取其IP地址；

    HA配置查看活动Namenode：hdfs haadmin -getServiceState nn1或nn2(namenode名)；




3、试验：

      目的平台执行hadoop distcp hdfs://IP:8020/pgw/   hdfs://namenode:8020/pgw/

      源平台执行hadoop distcp hdfs://IP:8020/pgw/  hdfs://IP:8020/pgw/

     都不行。

     分析后，关系  Cross-realm authentication问题，配置core-site.xml 需降低安全配置；

      <property>

          <name>ipc.client.fallback-to-simple-auth-allowed</name>

          <value>true</value>

      </property>




4、解决：在目的平台执行

hadoop distcp -D ipc.client.fallback-to-simple-auth-allowed=true -D dfs.checksum.type=CRC32 webhdfs://namenode IP:50070/data/  /data

命令中直接回退安全配置，同时采用webhdfs来传输，不过这个采用restfull机制，有IO阻塞风险。

hadoop distcp -D ipc.client.fallback-to-simple-auth-allowed=true webhdfs://namenode IP:50070/flume/data/    /data/   


大文件传输时，去掉校验-D dfs.checksum.type=CRC32就正常。




# Hadoop出现core-site.xml not found的解决办法 - Machine Learning with Peppa - CSDN博客





2018年05月27日 20:30:19[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：1337标签：[大数据																[Hadoop																[分布式系统](https://so.csdn.net/so/search/s.do?q=分布式系统&t=blog)
个人分类：[Hadoop/Spark																[***大数据框架***](https://blog.csdn.net/qq_39521554/article/category/7524600)](https://blog.csdn.net/qq_39521554/article/category/7361348)





安装完Hadoop之后跑的第一个命令就出了一个exception，如下：

![](https://img-blog.csdn.net/20180527202524412?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




提示说core-site.xml找不到。当时很奇怪，因为明明才更新了这个文件的配置，增加了几个<property>进去。经过查阅资料和 翻论坛，好在找到了修改办法：

```java
25 export JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk
 26 export HADOOP_HOME=/usr/local/hadoop-2.9.1
 27 export HADOOP_INSTALL=$HADOOP_HOME
 28 export HADOOP_MAPRED_HOME=$HADOOP_HOME
 29 export HADOOP_COMMON_HOME=$HADOOP_HOME
 30 export HADOOP_HDFS_HOME=$HADOOP_HOME
 31 export YARN_HOME=$HADOOP_HOME
 32 export HADOOP_COMMON_LIB_NATIVE_DIR=$HADOOP_HOME/lib/native
 33 export PATH=$PATH:$HADOOP_HOME/sbin:$HADOOP_HOME/bin
 34 
 35 # The jsvc implementation to use. Jsvc is required to run secure datanodes
 36 # that bind to privileged ports to provide authentication of data transfer
 37 # protocol.  Jsvc is not required if SASL is configured for authentication of
 38 # data transfer protocol using non-privileged ports.
 39 #export JSVC_HOME=${JSVC_HOME}
 40 
 41 export HADOOP_CONF_DIR=/usr/local/hadoop-2.9.1/etc/hadoop
```

27-33行都是为了避免可能的报错而增加的几个path变量（其实不加也没事），最主要的是要修改最后一行那个HADOOP_CONF_DIR 变量为自己的Hadoop目录（默认是个错误的路径所以会跳错）

再次运行命令，一切正常](https://so.csdn.net/so/search/s.do?q=Hadoop&t=blog)](https://so.csdn.net/so/search/s.do?q=大数据&t=blog)





# hadoop配置及启动 - LC900730的博客 - CSDN博客
2017年11月12日 21:53:00[lc900730](https://me.csdn.net/LC900730)阅读数：121
logo.gif？加载是向tomcat提交
向hdfs集群汇聚数据 
producer一边读log，然后写到hdfs。 
flume
如看了又看   请求某个action 
猜你喜欢 。  请求某个action 
热门关注      请求某个action
传统io： 
1.先将文件内容从磁盘中拷贝到操作系统buffer 
2.再从操作系统buffer拷贝到程序应用buffer 
3.从程序buffer拷贝到socket buffer 
4.从socket buffer拷贝到协议引擎。
从操作系统buffer写入到
## Hadoop配置
hadoop_env.sh
//查看jdk位置 
因为我是yum安装，所以rpm查看 
rpm -qa | grep java
然后rpm -ql java-1.8.0-openjdk-1.8.0.151-1.b12.el7_4.x86_64|more 
找到jdk位置，在hadoop.env.sh中加上JAVA_HOME
## 配置core-site.xml
```xml
<configuration>
        <property>
            <name>fs.defaultFS</name>
            <value>hdfs://centos3:9000</value>
        </property>
        <property>
            <name>hadoop.tmp.dir</name>
            <value>/home/hadoop/hdpdata</value>
        </property>
    </configuration>
```
### 配置配置hdfs-site.xml
### 格式化
hadoop namenode -format
然后进入到sbin目录 
./hadoop-daemon.sh start namenode
然后启动datanode 
./hadoop-daemon.sh start datanode
### 启动失败原因
1.运行mr程序出错，原因是没有启动yarn或者启动失败 
2.初始化工作目录结构 
hdfs namenode -format 只是初始化namenode工作目录，而datanode工作目录是在datanode启动后自己初始化等。 
3./datanode不被原来namenode识别的问题 
namenode在format初始化时候会形成2个标识别： 
blockPoolId 
clusterId 
新的datanode加入时候，会获取这2个标识作为自己工作目录中的标识。 
一旦namenode重新format后，namenode的身份识别已变，而datanode如果依然持有原来id，就不会被namenode识别。 
4./datanode下线后多久看到效果？ 
datanode不是一下线就被namenode认定为下线的，有一个超时时间。
1.namenode负责管理整个文件系统的元数据； 
2.Datanode负责管理用户的文件数据块； 
3.文件按照固定的大小切块分布式存储于若干台datanode上 
4.每一个文件可以有多个副本，并存放在不同的datanode上 
5.Datanode会定期向namenode汇报自身所保存的文件block信息，而namenode则会负责保存文件副本数量 
6.HDFS的内部工作机制对客户端保持透明，客户端请求访问HDFS都是通过向namenode申请来进行。
## 启动失败的JAVA_HOME
即使在/etc/profile中指定了JAVA_HOME,启动时候还是报错找不到JAVA_HOME   ,是因为在etc/hadoop/hadoop_env.sh中，还需要指定JAVA_HOME
## Master配置
core-site.xml
```xml
<property>
     <name>fs.defaultFS</name>
     <value>hdfs://mini1:9000</value>
</property>
<property>
     <name>hadoop.tmp.dir</name>
     <value>/home/hadoop/hdpdata</value>
</property>
```

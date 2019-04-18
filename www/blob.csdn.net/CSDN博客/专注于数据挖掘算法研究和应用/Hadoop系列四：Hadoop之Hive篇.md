# Hadoop系列四：Hadoop之Hive篇 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年03月11日 16:42:38[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：16066
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









Hive安装和运行

Pig与RDBMS、Hive比较

Pig Latin是一种数据流编程语言，而SQL是一种描述性编程语言。换句话说，Pig程序是相对于输入的一步步操作，其中每一步是对数据的一个简答的变换。相反，SQL语句是一个约束的集合，这些约束的集合在一起，定义了输出。

Hive介于Pig和传统RDBMS(关系数据库管理系统Relational DatabaseManagement System)之间，Hive的设计目的是让精通SQL既能的分析师能够在存放在HDFS的大规模数据集上运行查询。Hive在很多方面和传统数据库类似，但是它底层对HDFS和MapReduce的依赖意味着它的体系结构有别于传统数据库。

Hive本身不存储数据，完全依赖于HDFS和MapReduce，Hive可以将结构化的数据文件映射为一张数据库表，Hive中表纯逻辑，就是表的元数据。而HBase是物理表，定位是NoSQL。

Ubuntu操作系统，主机名hp，已部署Hadoop单节点伪分布，集成mysql作为元数据库。Hive默认的安装方式是Apache Derby，将元数据存储在Derby数据库中。Derby是一个完全用java编写的数据库，可跨平台在JVM中运行。


1）mysql安装

$sudo apt-get installmysql-server

登录mysql：$mysql -u root -p 

建立数据库hive：mysql>create database hive;

                mysql>show databases;//查看创建；

创建hive用户,并授权：mysql>grant all on hive.* to hive@'%'  identified by 'hive'; 

mysql>flush privileges; 

     查询mysql的版本：mysql>select version();//这里是5.6.28

下载mysql的JDBC驱动包：

http://dev.mysql.com/downloads/connector/j/

下载mysql-connector-java-5.1.38.tar.gz

复制msyql的JDBC驱动包到Hive的lib目录下。

2）Hive安装

官网[http://hive.apache.org/下载apache-hive-2.0.0-bin.tar.gz](http://hive.apache.org/%E4%B8%8B%E8%BD%BDapache-hive-2.0.0-bin.tar.gz)并解压在home/hp路径下。

环境配置

$sudo gedit/etc/profile

添加如下：

#Hive

exportHIVE_HOME=/home/hp/hive

exportPATH=$PATH:${HIVE_HOME}/bin

exportCLASSPATH=$CLASSPATH.:{HIVE_HOME}/lib

3）配置hive-env.sh文件

复制hive-env.sh.template，修改hive-env.sh文件

指定HADOOP_HOME及HIVE_CONF_DIR的路径如下： 

HADOOP_HOME=/home/hp/hadoop 

export HIVE_CONF_DIR=/home/hp/hive/conf

4）配置hive-site.xml文件

#sudo gedit/home/hp/conf/hive-site.xml //hive-site.xml新建

<?xmlversion="1.0"?>

<?xml-stylesheettype="text/xsl" href="configuration.xsl"?>

<configuration>

<property> 

   <name>javax.jdo.option.ConnectionURL</name> 

   <value>jdbc:mysql://localhost:3306/hive</value> 

</property> 

<property> 

   <name>javax.jdo.option.ConnectionDriverName</name> 

   <value>com.mysql.jdbc.Driver</value> 

</property>

<property> 

   <name>javax.jdo.option.ConnectionUserName</name> 

   <value>hive</value> 

</property> 

<property> 

   <name>javax.jdo.option.ConnectionPassword</name> 

   <value>hive</value> 

</property> 

</configuration>

<property>

  <name>hive.metastore.local</name>

  <value>true</value>

  <description></description>

</property>

5）运行hive

这里Hive中metastore（元数据存储）采用Local方式，非remote方式。

第一次需执行初始化命令$schematool-dbType mysql –initSchema

查看初始化后信息$ schematool -dbTypemysql –info

启动Hadoop服务：$sbin/start-dfs.sh和$sbin/start-yarn.sh

执行$hive

hive>show tables;

6）验证配置

Hive新建表：

hive>hive> CREATETABLE xp(id INT,name string) ROW FORMAT DELIMITED FIELDS TERMINATED BY '\t';

mysql中查看元数据信息

$mysql –u hive –p

mysql>showtables; //显示 hive 数据库中的数据表

mysql> select* from TBLS;//查看 hive 的元数据信息

Hadoop中查看生成的文件

$hadoop fs –ls/user/hive/warehouse/    //可以看到xp目录。

7）应用案例

Ø  场景：统计每日用户登陆总数。

每分钟的原始日志内容如下：[http://www.blue.com/uid=xxxxxx&ip=xxxxxx](http://www.blue.com/uid=xxxxxx&ip=xxxxxx)

假设只有两个字段,uid和ip,其中uid是用户的uid，是用户的唯一标识，ip是用户的登陆ip，每日的记录行数是10亿，要统计出一天用户登陆的总数。

Ø  Hive建表

hive>CREATE TABLElogin (uid  STRING, ip  STRING) 

PARTITIONEDBY (dt STRING)

ROW FORMATDELIMITED

FIELDSTERMINATED BY ','

STORED ASTEXTFILE;

表名是login,字段之间以,隔开,存储是TEXT,其次还以dt这个字段作为分区。创建成功之后,在hdfs上新建了/user/hive/warehouse/login目录。

Ø  格式化原始日志

将每天的每分钟的原始日志，转换成以下文件格式

123,17.6.2.6

112,11.3.6.2

………..

 根据文件大小，合并文件，例如合并为24个文件。

Ø  数据入库到hive

hive>LOADDATA LOCAL  INPATH'/data/login/20120713/*' OVERWRITE INTO TABLE login PARTITION (dt='20120713');

执行成功，转换过的文件会上传到hdfs的/user/hive/warehouse/login/dt=20120713目录里。

Ø  在hive上统计分析

hive>selectcount(distinct uid) from login where dt=’20120713’;

使用dt这个分区条件查询，就可以避免hive去查询其他分区的文件，减少IO操作，这个是hive分区很重要的特性，也是以天为单位，作为login表分区的重要意义。

执行完毕后，就可以在命令里出现结果，一般通过管道执行hive shell命令，读取管道的内容，把结果入库到mysql里就完成分析。

     该案例通过网上复制而来，作为理解hive的应用流程来理解。整个hive过程主要：Hive建表-Hive上传文件到hdfs-Hive基于Hadoop执行统计分析-Hive输出结果。这个案例比较清晰地理解了Hive的价值。
            



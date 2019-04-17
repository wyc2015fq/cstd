# Hadoop系列七：Hadoop之sqoop篇 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年03月14日 17:15:37[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2272
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









Sqoop安装和运行

sqoop是一个把关系型数据库数据抽向hadoop的工具。同时，也支持将hive、pig等查询的结果导入关系型数据库中存储。

1）安装和配置

Ubuntu操作系统，主机名hp。

官网[http://sqoop.apache.org/](http://sqoop.apache.org/) 下载sqoop-1.4.6.bin __hadoop-2.0.4-alpha.tar.gz并解压。

复制mysql的jdbc驱动mysql-connector-java-5.1.38.bin.jar到sqoop项目lib目录下。

HADOOP_HOME、HBASE_HOME、HIVE_HOME以及ZOOKEEPER_HOME都在etc/profile已配置。配置/etc/profile文件sqoop的环境变量

$sudo gedit/etc/profile

export SQOOP_HOME=/home/hp/sqoop

export PATH=$PATH:${SQOOP_HOME}/bin

$source /etc/profile

配置sqoop-env.sh文件

sqoop/conf目录下

$mv sqoop-env-template.sh  sqoop-env.sh

$gedit sqoop-env.sh

#Set path to where bin/hadoop is available

export HADOOP_COMMON_HOME=/home/hp/hadoop/



#Set path to where hadoop-*-core.jar is available

export HADOOP_MAPRED_HOME=/home/hp/hadoop



#set the path to where bin/hbase is available

export HBASE_HOME=/home/hp/hbase



#Set the path to where bin/hive is available

export HIVE_HOME=/home/hp/hive



#Set the path for where zookeper config dir is

export ZOOCFGDIR=/home/hp/zookeeper/conf

3）运行使用

启动Hadoop服务。

mysql数据库hive中数据表xp导入到HDFS中：

$sqoop import --connectjdbc:mysql://127.0.0.1:3306/hive --username hive --password hive --table xp --fields-terminated-by '\t' -m 1  

$hadoop fs –ls/user/hp/xp/    //可以看到已迁移过来的文件part-m-00000。

把hdfs的数据导出到mysql中：

mysql中创建数据表xp2，和xp一样有两个字段(id int,name char(20))

mysql>create tablexp2(id int,name char(20));

$sqoop export --connectjdbc:mysql://127.0.0.1:3306/hive --username hive --password hive --table xp2--export-dir ‘/user/hp/xp/part-m-00000’ --fields-terminated-by '\t'

进入mysql查看xp2内容

$mysql –u hive –p

mysql>use hive;

mysql>select* from xp2;//和xp原来内容一样
            



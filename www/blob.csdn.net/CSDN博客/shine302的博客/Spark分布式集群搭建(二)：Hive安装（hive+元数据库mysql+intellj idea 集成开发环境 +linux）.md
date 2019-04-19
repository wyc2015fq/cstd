# Spark分布式集群搭建(二)：Hive安装（hive+元数据库mysql+intellj idea 集成开发环境 +linux） - shine302的博客 - CSDN博客
2018年04月29日 19:26:01[SmartMap_Joule](https://me.csdn.net/shine302)阅读数：37标签：[hive安装																[spark集群																[intellj idea hive																[元数据库mysql](https://so.csdn.net/so/search/s.do?q=元数据库mysql&t=blog)](https://so.csdn.net/so/search/s.do?q=intellj idea hive&t=blog)](https://so.csdn.net/so/search/s.do?q=spark集群&t=blog)](https://so.csdn.net/so/search/s.do?q=hive安装&t=blog)
个人分类：[Spark																[Hive](https://blog.csdn.net/shine302/article/category/7559969)](https://blog.csdn.net/shine302/article/category/7559967)
本篇内容分为三部分：元数据库mysql安装+hive安装+intellj idea开发使用hive）
**1）元数据库mysql安装：**
Step1： 采用apt-get安装mysql 
Step2： 启动mysql服务 
Step3： 进入mysql命令行 
Step4： 创建一个database name为hive的数据库用于hive数据文件的存储 
Step5： 给当前用户授权 
命令如下：
sudo apt-get install mysql-server//采用apt-get安装mysql，会提示你设置mysql的用户名和密码，我的设为用户名root，密码root
service mysql start    //启动mysql服务
mysql -uroot-p//进入mysql命令行,输入之后会提示你输入密码，输入上述密码root
mysql> create user hive identified by'hive'; //创建一个用户名为hive，密码为hive的数据库
mysql> create database hive;  //创建一个叫做hive的数据库
mysql> grant allon hive.*to hive@'%' identified by'hive';  //给hive用户授权
mysql> grant allon hive.*to hive@'localhost' identified by'hive'; //给hive用户授权
mysql> flush privileges;  
mysql> exit;  //退出mysql
添加之后，可以查询用户，看到如下信息： 
![](https://img-blog.csdn.net/20180429182904213?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2) Hive 安装与配置
2.1hive的下载地址：
http://mirror.bit.edu.cn/apache/hive/hive-2.3.3/
下载bin版本
![](https://img-blog.csdn.net/20180429183340257?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
下载后将其解压，放在你想要的安装目录下。
2.2 将上述安装目录添加到环境变量中：vi /etc/profile
export HIVE_HOME=/home/jiaoshuhai/hive(安装目录)
export PATH=$HIVE_HOME/bin:$HIVE_HOME/conf:$PATH
2.3. 修改`hive/conf`下的hive-site.xml
将hive-default.xml.template重命名为hive-default.xml；新建一个文件`touch hive-site.xml`，并在hive-site.xml中粘贴如下配置信息：
```
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<?xml-stylesheet type="text/xsl" href="configuration.xsl"?>
<configuration>
  <property>
    <name>javax.jdo.option.ConnectionURL</name>
    <value>jdbc:mysql://localhost:3306/hive?createDatabaseIfNotExist=true</value>
    <description>JDBC connect string for a JDBC metastore</description>
  </property>
  <property>
    <name>javax.jdo.option.ConnectionDriverName</name>
    <value>com.mysql.jdbc.Driver</value>
    <description>Driver class name for a JDBC metastore</description>
  </property>
  <property>
    <name>javax.jdo.option.ConnectionUserName</name>
    <value>hive</value>
    <description>username to use against metastore database</description>
  </property>
  <property>
    <name>javax.jdo.option.ConnectionPassword</name>
    <value>hive</value>
    <description>password to use against metastore database</description>
  </property>
</configuration>
```
修改hive-env.sh 
进入${HIVE_HOME}\conf\即/opt/Hadoop/apache-hive-2.1.1/conf目录下，使用`vi hive-en.sh`打开文件，在hive-env.sh中添加以下路径：
```
export JAVA_HOME=/opt/Java/jdk1.8    //Java路径
export HADOOP_HOME=/opt/Hadoop/hadoop-2.7.3   //Hadoop安装路径
export HIVE_HOME=/opt/Hadoop/hive-2.1.1    //Hive安装路径
export HIVE_CONF_DIR=/opt/Hadoop/hive-2.1.1/conf   //Hive配置文件路径
```
创建hdfs目录，用于配置hive-site.xml 
在根目下，执行以下命令：
```
hdfs dfs -mkdir -p /user/hive/warehouse
hdfs dfs -mkdir -p /user/hive/tmp
hdfs dfs -mkdir -p /user/hive/log
hdfs dfs -chmod -R 777 /user/hive/warehouse
hdfs dfs -chmod -R 777 /user/hive/tmp
hdfs dfs -chmod -R 777 /user/hive/log
```
在apache-2.1.1安装目录下创建一个tmp文件夹，用于存储临时文件，命令如下：
```bash
cd /opt/Hadoop/apache-hive-2.1.1/
mkdir tmp
```
修改hive-site.xml 
将hive-site.xml文件中的以下几个配置项的值设置成上一步中创建的几个路径：
```xml
<property>
    <name>hive.exec.scratchdir</name>
    <value>/user/hive/tmp</value>
</property>
<property>
    <name>hive.exec.local.scratchdir</name>
    <value>/opt/Hadoop/apache-hive-2.1.1/tmp</value>
</property>
<property>
    <name>hive.metastore.warehouse.dir</name>
    <value>/user/hive/warehouse</value>
</property>
<property>
    <name>hive.downloaded.resources.dir</name>
    <value>/opt/Hadoop/apache-hive-2.1.1/tmp/${hive.session.id}_resources</value>
</property>
<property>
    <name>hive.querylog.location</name>
    <value>/user/hive/log</value>
</property>
```
更改{system:java.io.tmpdir} / {system:user.name}
```
在配置文件hive-site.xml中：
将{system:java.io.tmpdir} 改成 /opt/Hadoop/apache-hive-2.1.1/tmp/
将{system:user.name} 改成 {user.name}
```
### 配置jdbc驱动包
mysql的jdbc驱动包下载地址：[https://dev.mysql.com/downloads/connector/j/](https://dev.mysql.com/downloads/connector/j/)
下载之后将文件夹解压，得到mysql-connector-java-5.1.42-bin.jar，将此jar包放在${HIVE_HOME}/lib目录，即/opt/Hadoop/apache-hive-2.1.1/lib下。
从 Hive 2.1 版本开始, 我们需要先运行 schematool 命令来执行初始化操作。
`schematool -dbType mysql -initSchema`
看到schemaTool completed 则初始化完成 。 
可以使用`schematool -dbType mysql -initInfo` 查看数据库初始化信息。 
进入mysql中，查看hive中的表格信息，可看到如图内容：
```php
mysql -uroot -p  //进入mysql命令行,输入之后会提示你输入密码，输入上述密码root
use hive;
show tables;
```
![这里写图片描述](https://img-blog.csdn.net/20170717151538151)
检测hive 是否成功，直接在命令行输入hive即可：
`hive`
启动成功的提示如图： 
![这里写图片描述](https://img-blog.csdn.net/20170713213457623)
### 在hive中创建一个表格
hive启动成功后，使用下面的语句验证是否hive可用：
```
create table testHive(
id int,
name string
);
show tables;
```
如下图： 
![这里写图片描述](https://img-blog.csdn.net/20170713214403449)
**3)intellj idea 中开发hive**
3.1将hive-site放入到source文件下
![](https://img-blog.csdn.net/20180429192813799?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3.2 添加jar包引用
jars为本地目录下的spark包
lib为本地目录下的hive包
![](https://img-blog.csdn.net/20180429192926810?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3.3Vm设置
-Xms2g -Xmx2g -XX:MaxNewSize=2g -XX:MaxPermSize=2g
![](https://img-blog.csdn.net/20180429193131319?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3.4代码
**object **HiveTest {
  **def **main(args: Array[String]): Unit = {
    System.setProperty("spark.eventLog.enabled", "true");
System.setProperty("spark.eventLog.dir", "hdfs://172.20.161.96:9000/history/");
**val **conf = **new **SparkConf().setMaster("spark://172.20.161.96:7077").setAppName("201804151154")
    **val **sc = **new **SparkContext(conf)
    **val **hivetest = **new **HiveContext(sc)
    hivetest.sql("show databases").collect().foreach(println)
    hivetest.sql("use taxidata")
    hivetest.sql("select medallion from triprecord limit 100").collect().foreach(println)
    sc.stop()
  }
}

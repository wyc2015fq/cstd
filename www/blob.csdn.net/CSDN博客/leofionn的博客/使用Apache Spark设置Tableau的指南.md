
# 使用Apache Spark设置Tableau的指南 - leofionn的博客 - CSDN博客


2019年02月11日 21:40:26[leofionn](https://me.csdn.net/qq_36142114)阅读数：70


[https://community.tableau.com/docs/DOC-7638](https://community.tableau.com/docs/DOC-7638)
Apache Spark是大数据分析中最热门的事情，而Tableau是最热门的数据可视化和发现工具之一。将它们组合在一起，您就可以在大数据分析和可视化领域获得潜在的游戏规则改变。
Tableau 9支持与Spark一起使用，但设置并非完全直截了当，直到您正确设置后端组件设置。
**技术堆栈**
随着快速发展的开源生态系统，Apache Hadoop和Spark具有多个稳定和活跃的版本。以下技术堆栈是我们测试使用Tableau集成的。
Hadoop 2.6或更高版本
蜂巢
mySQL - 用于容纳Hive metastore_db（稍后会详细介绍）
Spark 1.4或更高版本
Tableau Desktop 9或更高版本
Spark ODBC驱动程序
**Hadoop 2.6**
为了使所有Hive功能正常工作，您需要确保使用的是Hadoop 2.6或更高版本。有一些工作可以在早期的Hadoop发行版上使用它，但它不值得付出努力。进行Hadoop升级要简单得多。由于Hadoop升级本身就是一个广泛的过程，我将其从这篇文章中删除。
**蜂巢设置**
安装或升级到Hadoop 2.6后，您需要安装Hive。首先下载最新的稳定版本，解压缩分发和链接。

*root @XXXXX：/ d01 / swDump \#wget**[http://mirror.cc.columbia.edu/pub/software/apache/hive/stable/apache-hive-1.1.0-bin.tar.gz](http://mirror.cc.columbia.edu/pub/software/apache/hive/stable/apache-hive-1.1.0-bin.tar.gz)*
*root @XXXXX：/ d01 / swDump＃md5sum apache-hive-1.1.0-bin.tar.gz*
*root @XXXXX：/ d01 / swDump \#tar -xzf apache-hive-1.1.0-bin.tar.gz*
*root @XXXXX：/ d01 / swDump \#mv apache-hive-1.1.0-bin / srv*
*root @XXXXX：/ d01 / swDump \#chown -R hadoop：hadoop /srv/apache-hive-1.1.0-bin*
*root @XXXXX：/ d01 / swDump＃ln -s /srv/apache-hive-1.1.0-bin / srv / hive*
设置配置单元特定的环境变量
*vim .bashrc*
*- ＃配置Hive环境*
*导出HIVE_HOME = / srv / hive*
*export PATH = $ PATH：$ HIVE_HOME / bin*
*- \#\#解决由于使用jline 0.9x的hadoop导致的hive错误，而hive升级到Jline2*
*export HADOOP_USER_CLASSPATH_FIRST = true*
还有其他一些配置，但我们稍后会处理。
**MySQL设置**
Hive的默认配置附带嵌入式Derby数据存储，但它有一些限制。首先，它不能很好地适用于任何类型的生产部署。接下来它也不支持每个井的共享连接。如果我们要与Tableau连接，我们将需要这个。假设我们已经在您的系统上安装了mySQL，您可以使用Hive配置它，如下所示
以root用户身份登录mySQL，创建metastore_db，hive用户帐户和所需权限
*mysql -u root -p*
*mysql> CREATE DATABASE metastore_db;*
*查询正常，1行受影响（0.00秒）*
*mysql> CREATE USER'hiveuser'@'％'IDENTIFIED BY'hive123';*
*查询正常，0行受影响（0.00秒）*
*mysql> GRANT all on *。* to'hiveuser'@ localhost''hive123';*
*查询正常，0行受影响（0.00秒）*
*mysql> flush特权;*
*查询正常，0行受影响（0.00秒）*
您还需要从此处下载mySQL java连接器（[http://dev.mysql.com/downloads/connector/j ](http://dev.mysql.com/downloads/connector/j)），将其解压缩并复制到$ HIVE_HOME / lib
*root @XXXXX：/d01/swDump/mysql-connector-java-5.1.36\#cp mysql-connector-java-5.1.36-bin.jar / srv / hive / lib /*
*root @XXXXX：/d01/swDump/mysql-connector-java-5.1.36\# chown hadoop：hadoop /srv/hive/lib/mysql-connector-java-5.1.36-bin.jar*
最后*在$ HIVE*HOME / conf中创建一个hive*site.xml文件，*其中包含以下条目。使用适当的值替换配置单元用户和密码。
*<结构>*
*<属性>*
*<名称> javax.jdo.option.ConnectionURL </名称>*
*<值> JDBC：MySQL的：//本地主机/ metastore_db createDatabaseIfNotExist =真</值>*
*<description>元数据存储在MySQL服务器</ description>中*
*</属性>*
*<属性>*
*<名称> javax.jdo.option.ConnectionDriverName </名称>*
*<值> com.mysql.jdbc.Driver </值>*
*<description> MySQL JDBC驱动程序类</ description>*
*</属性>*
*<属性>*
*<名称> javax.jdo.option.ConnectionUserName </名称>*
*<值> hiveuser </值>*
*<description>用于连接mysql服务器的用户名</ description>*
*</属性>*
*<属性>*
*<名称> javax.jdo.option.ConnectionPassword </名称>*
*<值> hive123 </值>*
*<description>用于连接mysql服务器的密码</ description>*
*</属性>*
*</配置>*
这就是你所有的一切（差不多）。下次登录到hive shell时，它应该连接到mySQL metastore_db，并且所有对象都将与之一起存储。您可以开始加载测试数据。
*hive> create table testaadh（state String，district String，gen int，reject int）以'，'结尾的行格式分隔的字段存储为textfile;*
*好*
*所用时间：1.526秒*
*蜂巢>*
*hive> LOAD DATA INPATH'/ user/samx18/tempdata.csv'INTO TABLE testaadh;*
*将数据加载到表default.testaadh*
*表default.testaadh stats：[numFiles = 1，numRows = 0，totalSize = 552089，rawDataSize = 0]*
*好*
您可以直接查询mySQL metastore_db以检查您的hive表是否已成功创建。

*mysql -u hiveuser -p metastore_db*
*mysql> select * from TBLS;*
*+--------+-------------+-------+------------------+--------+-----------+-------+-------------+---------------+--------------------+--------------------+*
*| TBL_ID | CREATE_TIME | DB_ID | LAST_ACCESS_TIME | 所有者| 保留| SD_ID | TBL_NAME | TBL_TYPE | VIEW_EXPANDED_TEXT | VIEW_ORIGINAL_TEXT |*
*+--------+-------------+-------+------------------+--------+-----------+-------+-------------+---------------+--------------------+--------------------+*
*| 1 | 1441400778 | 1 | 0 | hadoop | 0 | 1 | hivetest | MANAGED_TABLE | NULL | NULL |*
*| 6 | 1441410858 | 1 | 0 | hadoop | 0 | 6 | testaadh | MANAGED_TABLE | NULL | NULL |*
*+--------+-------------+-------+------------------+--------+-----------+-------+-------------+---------------+--------------------+--------------------+*
*2行（0.00秒）*

**火花设置**
确保在Hadoop集群上安装了spark 1.4或更高版本，否则可以使用此处的说明进行设置。
要使用配置单元完成spark设置，您需要将hive-site.xml复制到您的
*$ SPARK_HOME / conf目录*
*$ cp $ HIVE_HOME / conf / hive-site.xml $ SPARK_HOME / conf /*
连接到您最喜欢的Spark shell（在我们的例子中是pyspark），并使用Spark Hive上下文测试与Hive表的连接。根据您的火花构建，您的hive上下文可能已经或可能没有为您构建。

*> sqlContext = HiveContext（sc）*
*> results = sqlContext.sql（“SELECT * FROM testaadh”）*
*>*
确认后，您可以通过SparkSQL访问配置单元表，启动Spark Thrift服务器。这将使Tableau能够建立连接并运行SparkSQL查询。
*hadoop @ XXXXX：〜$ cd $ SPARK_HOME / sbin*
*hadoop @ XXXXX：/ srv / spark / sbin $ ./start-thriftserver.sh*
*启动org.apache.spark.sql.hive.thriftserver.HiveThriftServer2，登录到/srv/spark-1.4.1-bin-hadoop2.6/sbin/../logs/spark-hadoop-org.apache.spark.sql .hive.thriftserver.HiveThriftServer2-1-XXXXX.out*

使用beeline测试与Spark Thrift服务器的连接
*hadoop @ XXXXX：〜$ cd $ SPARK_HOME*
*hadoop @ XXXXX：/ srv / spark $ ./bin/beeline*
*Apache Hive的Beeline版本1.4.1*
*beeline>！connect jdbc：hive2：// localhost：10000*
*扫描完成2ms*
*连接到jdbc：hive2：// localhost：10000*
*输入jdbc的用户名：hive2：// localhost：10000：*
*输入jdbc：hive2：// localhost：10000的密码：*
*连接到：Spark SQL（版本1.4.1）*
*驱动程序：Spark Project Core（版本1.4.1）*
*事务隔离：TRANSACTION_REPEATABLE_READ*
*0：jdbc：hive2：// localhost：10000>*

**桌面设置**
您的服务器组件现已设置好，您可以转到桌面设置。
下载并安装以下内容
Tableau Desktop 9
Spark ODBC驱动程序
打开Tableau，并创建一个新的数据源。选择SparkSQL作为服务器类型，并填写Spark Thrift服务器的详细信息
![Tableau1.JPG](https://community.tableau.com/servlet/JiveServlet/downloadImage/102-7638-1-41513/Tableau1.JPG)

浏览到默认架构，您应该看到能够看到您的Hive Hadoop集群表。
![Tableau2.JPG](https://community.tableau.com/servlet/JiveServlet/downloadImage/102-7638-1-41514/Tableau2.JPG)


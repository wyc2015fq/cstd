# Nutch编译及集成eclipse+mysql开发环境的部署总结 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年04月01日 18:02:37[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3008








Nutch是一个应用程序，以Lucene为基础实现的搜索引擎应用，Lucene为Nutch 提供了文本搜索和索引的API，Nutch不仅提供搜索，而且还有数据抓取的功能。

1）linux下nutch集成mysql编译

预先部署ubuntu+jdk+tomcat+mysql，配置mysql的配置文件my.ini为：

[mysqld]下添加skip-grant-tables和character-set-server=utf8

[mysql]、[client]下添加default-character-set=utf8

根据nutch的conf文件gora-sql-mapping创建数据库和数据表。也可通过自动方式生成数据库和表：配置gora-sql-mapping、gora.properties及其它文件后，首次运行bin/nutch inject urls即可自动生成数据库和表。

重启mysql服务。

第一步：下载解压

官网http://nutch.apache.org/下载apache-nutch-2.2-src.tar.gz版本。

#tar –xzvfapache-nutch-2.2-src.tar.gz

#cp-Rapache-nutch-2.2 /usr/nutch

#cd/usr/nutch

第二步：配置nutch对mysql的支持

#vi ivy/ivy.xml   //启用mysql数据库

取消下面两行注释：

<dependencyorg="mysql" name="mysql-connector-java"rev="5.1.18" conf="*->default"/>

<dependencyorg="org.apache.gora" name="gora-sql"rev="0.1.1-incubating" conf="*->default" />

修改下行的rev值：原rev="0.3" 改成 rev="0.2.1"

<dependencyorg="org.apache.gora" name="gora-core" rev="0.3"conf="*->default" />

#viconf/gora.properties   //设置mysql连接

注释掉默认的数据库连接配置，同时添加以下内容：

###############################

# MySQLproperties           

 ################################

gora.sqlstore.jdbc.driver=com.mysql.jdbc.Driver

gora.sqlstore.jdbc.url=jdbc:mysql://localhost:3306/nutch?createDatabaseIfNotExist=true

gora.sqlstore.jdbc.user=root

gora.sqlstore.jdbc.password=123

#viconf/nutch-site.xml  //修改配置文件

configuration节点增加如下内容：

<property>

<name>http.agent.name</name>

<value>Nutch</value>

</property>

<property>

<name>http.accept.language</name>

<value>ja-jp,en-us,en-gb,en;q=0.7,*;q=0.3</value>

<description>Valueof the “Accept-Language” request header field.

This allowsselecting non-English language as default one to retrieve.

It is auseful setting for search engines build for certain national group.

</description>

</property>

<property>

<name>parser.character.encoding.default</name>

<value>utf-8</value>

<description>Thecharacter encoding to fall back to when no other information 

isavailable</description>

</property>

<property>

<name>storage.data.store.class</name>

<value>org.apache.gora.sql.store.SqlStore</value>

<description>TheGora DataStore class for storing and retrieving data. 

Currentlythe following stores are available: …. 

</description>

</property>

<property>

<name>generate.batch.id</name>

<value>*</value>

</property>

第三步：nutch根目录下编译

#apt-getinstall ant    //安装ant

#ant  runtime //ant编译，持续时间很长

编译后新增runtime目录。

第四步：网页抓取

进入runtime/local目录，使用Crawl命令抓取网页。

#mkdir -purls

#echo'http://www.oschina.net/' >urls/seed.txt

#bin/nutchcrawl urls -depth 3 -topN 5

执行完到mysql中查看结果。

mysql>select baseUrl from webpage;//默认数据库是nutch

+---------------------+

|baseUrl             |

+---------------------+

|http://www.163.com/ |

| NULL                |

+---------------------+

2 rows inset (0.00 sec)

2）eclipse集成nutch

Windows环境下配置elicpse集成nutch开发。

用ant命令将项目转换成eclipse项目，参考：

http://wiki.apache.org/nutch/RunNutchInEclipse

为避免编译，直接下载有bin包的nutch版本，apache-nutch-1.6-src.zip和apache-nutch-1.6-bin.zip两个包。

第一步：建立工程

新建Java Project工程，命名为nutch。

复制 nutch-1.6-src 包下 java 包里的org整个包放在 项目的src包下。

复制 nutch-1.6-bin包里的conf 文件夹至项目的src包下。在conf 目录右键，BuildPath ->Use as Source Folder。

复制nutch-1.6-bin 包里的 lib包下的所有jar包至项目目录下。将lib下的jar包手动在Bulid Path里引入，Libraries->Add JARS内添加。

复制 nutch-1.6-bin 包下的 plugins 文件夹整个放在项目的 src 包下。

第二步：工程执行

conf/nutch-site.xml配置

<property>

<name>http.agent.name</name>

<value>Nutch-demo</value>

</property>

<property>

<name>plugin.folders</name>

<value>./src/plugins</value>

</property>

工程下新建urls目录，下新建seed.txt，输入几个网址。

在src包下找到Crawl.java 文件

Runas ->Run Configuration ->Arguments 

Programarguments输入：crawlurls -dir out -threads 20 -depth 2

VMarguments输入：-Xms32m -Xmx800m（注：这是设置内存大小，如果不设置会导致内存溢出异常）

上网下载Hadoop-core-1.2.1，用来替换Nutch1.7在windows下执行因权限报错的问题，替换掉原来的Hadoop-core-1.03.jar。

下载地址：http://download.csdn.net/detail/leave00608/7060765

配置执行Run。执行后，看到工程目录下out文件夹，Hadoop存储。为了开发方便需要集成mysql，部署高版本的nutch。

3）eclipse集成nutch和mysql开发

下载apache-nutch-2.2-src.zip包，linux下ant编译后apache-nutch-2.2-bin文件夹（集成mysql）。

第一步：新建工程nutch

复制 nutch-2.2-bin下src/ java 目录下org到项目src包下。

复制 nutch-2.2-bin下conf到项目src包下，并conf 目录右键，BuildPath ->Use as Source Folder。

复制nutch-2.2-bin下runtime/local目录lib到项目目录下，将所有jar包手动在BulidPath里引入，Libraries->Add JARS内添加。

复制 nutch-2.2-bi下的 runtime/local目录plugins 到项目src包下，并BuildPath ->Use as Source Folder。

第二步：执行工程

启动myql并检查conf/gora.properties文件夹内mysql配置的信息。

conf/nutch-site.xml新增

<property>

<name>plugin.folders</name>

<value>./src/plugins</value>

</property>

工程下新建urls目录并在其目录下新建urls.txt，输入几个网址。

Hadoop权限问题：Hadoop-core-1.2.1替换nutch2.2中的Hadoop-core-1.1.1包。

在src包下找到Crawl.java 文件

Runas ->Run Configuration ->Arguments 

Programarguments输入：crawlurls -dir out -threads 20 -depth 2

VMarguments输入：-Xms32m -Xmx800m

执行后查看mysql

mysql>select baseUrl from webpage;//默认数据库是nutch

下面就是具体在eclipse中应用nutch开发，并存储在mysql中。
            



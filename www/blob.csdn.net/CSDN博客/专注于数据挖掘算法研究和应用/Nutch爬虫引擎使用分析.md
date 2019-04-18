# Nutch爬虫引擎使用分析 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年04月08日 17:19:53[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3507








Nutch2.X主要执行流程：

1）InjectorJob: 从文件中得到一批种子网页，把它们放到抓取数据库中去

2）GeneratorJob:从抓取数据库中产生要抓取的页面放到抓取队列中去

3）FetcherJob:   对抓取队列中的网页进行抓取,在reducer中使用了生产/消费者模型

4）ParserJob:    对抓取完成的网页进行解析，产生一些新的链接与网页内容的解析结果

5）DbUpdaterJob:把新产生的链接更新到抓取数据库中去

6）SolrIndexerJob:对解析后的内容进行索引建立

源码解读并编写后，发现nutch2.2未执行DbUpdaterJob，解析出的链接没有在webpage数据库中，只进行了第一层爬虫。重新部署nutch2.0试验，分布执行命令，重点是将nutch-default.xml直接复制到nutch-site.xml，其中conf/regex-urlfilter.txt可修改网页过滤器。

编译后进入antime/local目录

#bin/nutch  inject urls //urls为种子地址目录

#bin/nutch generate-topN 5

# bin/nutchfetch –all

# bin/nutchparse –all

# bin/nutchupdatedb

看mysql中nutch数据库的webpage表还是没有链接更新进去，查看WebDB中的网页数目和链接数目：

#bin/nutchreaddb crawl-tinysite/db –stats  //只有2个url

实在无解，着手nutch-site.xml配置上检查，并分析hadoop.log文件中的日志。在nutch-site.xml中增加如下设置后就不再出现提示http.robots.agents的错误。

<property>

<name>http.robots.agents</name>

<value>nutch2.0,*</value>

<description>The agent strings we'll look for inrobots.txt files,

  comma-separated, in decreasingorder of precedence. You should

  put the value of http.agent.nameas the first agent name, and keep the

  default * at the end of the list.E.g.: BlurflDev,Blurfl,*

</description>

</property>

Hadoop.log中还提示mapred.FileOutputCommitter- Output path is null in cleanup的警告，暂未找到解决办法，不过官网上解答说不影响。

目前的情况是：只进行第一层爬虫的ParserJob，未进行DbUpdaterJob，自然无法将爬虫到得链接更新到数据库，也就无法开始第二层爬虫。综合1.6、2.0、2.1、2.2四个版本的编译后执行情况，以及nutch-site.xml和mysql字符集的设置调整，初步筛选出可能存在的问题点在于解析出的数据未能更新到数据库。

1）1.x版仅支持hadoop存储，执行正常；

2）2.x版支持mysql存储，部署nutch时为方便开发直接集成mysql，未试验hbase是否正常；

3）从2.x系列版本执行来看，与版本无关系，与执行参数配置无关，可能与mysql集成有关系；

4）分步执行2.2每个步骤job，以及从hadoop.log看出，停留在ParserJob上，出现字符报错；

5）因此初步定位到mysql集成上nutch自动生成的数据库和表与mysql字符不兼容导致。

在配置MySQL时，由于编码问题，采用手动创建nutch在mysql的数据库和webpage表，参考网上的说明（http://www.cnblogs.com/AloneSword/p/3798126.html），对编译nutch2.2版本前配置mysql，执行如下步骤：

1）#vi/etc/mysql/my.cnf

在[mysqld]下添加：

[mysqld]

*innodb_file_format=barracuda*

*innodb_file_per_table=true*

*innodb_large_prefix=true*

*character-set-server=utf8mb4*

*collation-server=utf8mb4_unicode_ci*

*max_allowed_packet=500M*

重启mysql，查看mysql是否启动：

#netstat-tap | grep mysql   //本环境配置的mysql端口是5306

2）授予访问权限：

#mysql –uroot –p 

mysql>GRANTALL PRIVILEGES ON *.* TO root@"%" IDENTIFIED BY  "password";

3）手动创建数据库nutch和数据表webpage

#mysql –uroot –p

*mysql>CREATE DATABASE nutch DEFAULTCHARACTER SET utf8mb4 DEFAULT COLLATE utf8mb4_unicode_ci;*

*msyql>use nutch;*

*msyql> *

CREATE TABLE`webpage` (

`id` varchar(767)NOT NULL,

`headers`blob,

`text`mediumtext DEFAULT NULL,

`status`int(11) DEFAULT NULL,

`markers`blob,

`parseStatus`blob,

`modifiedTime`bigint(20) DEFAULT NULL,

`score`float DEFAULT NULL,

`typ`varchar(32) CHARACTER SET latin1 DEFAULT NULL,

`baseUrl`varchar(767) DEFAULT NULL,

`content`longblob,

`title`varchar(2048) DEFAULT NULL,

`reprUrl`varchar(767) DEFAULT NULL,

`fetchInterval`int(11) DEFAULT NULL,

`prevFetchTime`bigint(20) DEFAULT NULL,

`inlinks`mediumblob,

`prevSignature`blob,

`outlinks`mediumblob,

`fetchTime`bigint(20) DEFAULT NULL,

`retriesSinceFetch`int(11) DEFAULT NULL,

`protocolStatus`blob,

`signature`blob,

`metadata`blob,

PRIMARY KEY(`id`)

)ENGINE=InnoDB

ROW_FORMAT=COMPRESSED

DEFAULTCHARSET=utf8mb4;

表中的字段根据nutch的conf文件“gora-sql-mapping”进行设置。如通过自动方式生成数据库和表：配置好“gora-sql-mapping”、“gora.properties”及其它文件后，首次通过运行”bin/nutch inject urls”即可自动生成数据库和表，不过自动生成可能会遇到问题，通过查看hadoop.log文件发现很多问题与MySQL支持的数据类型、数据长度有关，只需要根据日志提示做修改、调试（可借助navicat工具像SQL Server方便操作数据库），然后再重复自动生成过程，直到成功为止。

下面进入nutch目录下配置后重新编译：

1）# viivy/ivy.xml  //取消下面行的注释，启用mysql

<dependencyorg=”mysql” name=”mysql-connector-java” rev=”5.1.18″ conf=”*->default”/>

2）viconf/gora.properties

注释掉默认的数据库，增加mysql数据库信息，如下：

###############################

# MySQLproperties            #

###############################

gora.sqlstore.jdbc.driver=com.mysql.jdbc.Driver

gora.sqlstore.jdbc.url=jdbc:mysql://localhost:5306/nutch?createDatabaseIfNotExist=true

gora.sqlstore.jdbc.user=root

gora.sqlstore.jdbc.password=123

3）viconf/gora-sql-mapping.xml

将primarykey的length从512改成767，注意有两个地方：

<primarykeycolumn=”id” length=”767″/>

4）viconf/nutch-site.xml

可以从nutch-default.xml中复制出来，不过基本配置一些关键就可以，增加如下：

<property>

<name>http.agent.name</name>

<value>Nutch2.2</value>

</property>



<property>

  <name>http.robots.agents</name>

  <value>nutch2.2,*</value>

  <description>The agent strings we'lllook for in robots.txt files,

  comma-separated, in decreasing order ofprecedence. You should

  put the value of http.agent.name as the firstagent name, and keep the

  default * at the end of the list. E.g.:BlurflDev,Blurfl,*

  </description>

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

确保nutch-site文件保存为utf-8格式。

5）编译

#apt-getinstall ant  //如无ant 则先安装

#ant runtime   //进入nutch2.2目录编译 

编译后进入runtime/local目录进行爬虫，具体步骤：

1）爬虫

#cdruntime/local

#mkdir -purls

#echo'http://nutch.apache.org/' > urls/seed.txt

#bin/nutchcrawl urls -depth 3 -topN 5

2）错误处理

错误一：执行到GeneratorJob出现错误，查看hadoop.log提示是

java.lang.NullPointerException

   atorg.apache.avro.util.Utf8.<init>(Utf8.java:37)

   atorg.apache.nutch.crawl.GeneratorReducer.setup(GeneratorReducer.java:100)

查看GeneratorReducer第100行代码：

batchId =newUtf8(conf.get(GeneratorJob.BATCH_ID));

可以看到是获取GeneratorJob.BATCH_ID时传空值。

解决办法一：

修改GeneratorJob中的public Map<String,Object> run(Map<String,Object> args) 方法，添加如下代码：


1.  // generate batchId

2. 
**int** randomSeed = Math.abs(**new** Random().nextInt());  

3.     String batchId = (curTime / 1000) + "-" + randomSeed;  

4. 
   getConf().set(BATCH_ID, batchId);  


解决办法二：

在nutch-site.xml中添加generate.batch.id配置项，value不为空即可，如下面：

<property>

    <name>generate.batch.id</name>

    <value>*</value>

</property>

采取办法二先解决，后期观察是否存在问题再采用方法一。

错误二：执行到GeneratorJob出现错误，查看hadoop.log提示是Unknown column 'batchId' in 'field list'。

解决办法：在webpage上增加batchId字段，如下：

`batchId` varchar(767)DEFAULT NULL,

mysql>alerttable add batchId varchar(767) default NULL;

mysql>showcolumns from webpage;//查看字段batchId

3）查看结果

#mysql -u root-p

msyql>usenutch;

mysql>SELECTcount( *) FROM nutch.webpage;

mysql>select count(*) from webpage;

+----------+

| count(*) |

+----------+

|      495 |

+----------+

共495条记录，成功。对于加入solar索引后续根据爬虫数据量再研究。经验上，还是多借鉴网上的步骤，自己摸索要走很多弯路啊。

在爬虫试验中发现部分网站无法爬虫出网页内的链接，如163门户、新浪门户、腾讯门户、天涯论坛等，继续观察hadoop.log日志，没有任何错误，又陷入死结。怀疑是设置了反爬虫策略？

nutch只能抓取到的是简单页面的内容，即不包括该页面加载后又执行的js请求、ajax请求、内嵌iframe等页面。

单独爬虫种子地址http://www.163.com 分析：

mysql>select id,title,status from nutch.webpage;

+------------------------------+--------+--------+

| id                           | title  | status |

+------------------------------+--------+--------+

|com.163.www:http/            | 网易   |     2 |

|com.netease.cache.img1:http/ | NULL  |      3 |

+------------------------------+--------+--------+

可以看到status中2是正确的，3是网页不存在，那为什么网易只爬虫出一个链接呢？通过看爬虫到网易的content发现charset=gb2312，最重要是body正文都没有内容，可见是设置了反爬虫策略。

单独爬虫种子地址http://www.sina.com.cn 分析：

从webpage表中看也是抓取正确，且有body网页内容，但为什么没有爬虫出更多链接呢？

单独爬虫种子地址http://wwwtianya.cn 分析：和sina网一样正确抓取，网页内容也正确，为什么没有爬虫出更多链接呢？腾讯网和搜狐网类似。

尝试模拟Chrome浏览器绕过反爬虫限制：

<property>

    <name>http.agent.name</name>

    <value>Mozilla/5.0 (Windows NT 6.1;WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/33.0.1750.117Safari</value>

</property>

<property>

    <name>http.agent.version</name>

    <value>537.36</value>

</property>

发现没有效果。

需要进一步定位为什么部分网站无法爬虫出链接？

在conf/nutch-site.xml中配置了http.robots.agents项，去掉观察。

<property>

  <name>http.robots.agents</name>

  <value>nutch2.2,*</value>

  <description>The agent strings we'lllook for in robots.txt files,

  comma-separated, in decreasing order of precedence.You should

  put the value of http.agent.name as the firstagent name, and keep the

  default * at the end of the list. E.g.:BlurflDev,Blurfl,*

  </description>

</property>

仍发现没有效果。

百般无奈下，只好多放几个网址测试，突然发现二级域名都可以爬虫。如http://sports.sina.com.cn，而http://www.sina.com.cn就不可以。具体原因不清楚，想来要么是门户网站设置了反爬虫策略，要么是nutch本身机制存在问题，网上说需要二次开发才能实现，那就留后续源码中再处理。

试验中，种子地址分别加了如下：

http://focus.tianya.cn/

http://sports.sina.com.cn/

http://sports.163.com/

http://sports.sohu.com/

爬虫设置depth=10和topN=200，耗近1个小时共爬虫出 30365条。直接中断执行了，还不知道要执行多久，机器主要配置如下：

# cat/proc/cpuinfo

八核Intel(R)Xeon(R) CPU           E5410  @ 2.33GHz

#free -m

             total       used       free    shared    buffers     cached

Mem:         16043       6768       9275          0        358       4907

-/+buffers/cache:       1502      14540

Swap:            0          0          0

Nutch适合大型爬虫用，最好是用hadoop直接存储并建立solar索引来检索爬虫结果。
            



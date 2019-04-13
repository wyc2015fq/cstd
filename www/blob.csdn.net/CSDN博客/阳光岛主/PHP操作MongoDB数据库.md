
# PHP操作MongoDB数据库 - 阳光岛主 - CSDN博客

2015年10月27日 15:51:54[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：15633



**一、MongoDB简介**
MongoDB (名称来自”humongous”) 是一个可扩展的、高性能、开源、模式自由、面向文档的数据库，集文档数据库、键值对存储和关系型数据库的优点于一身。
**官方站点：**[https://www.mongodb.org](https://www.mongodb.org/)
**MongoDB特点：**
• 面向文档存储(类JSON数据模式简单而强大)
• 动态查询
• 全索引支持，扩展到内部对象和内嵌数组
• 查询记录分析
• 快速，就地更新
• 高效存储二进制大对象 (比如照片和视频)
• 复制和故障切换支持
• Auto-Sharding自动分片支持云级扩展性
• MapReduce 支持复杂聚合
• 商业支持，培训和咨询
**二、安装MongoDB**
安装MongoDB非常的简单，仅需下载压缩包解压运行命令即可
**下载地址：**[https://www.mongodb.org/downloads](https://www.mongodb.org/downloads)
本文为windows平台，MongoDB运行命令：>bin/mongod
提示：首先要创建存储数据的文件夹，MongoDB 默认存储数据目录为 /data/db/ (或者 c:\data\db)，当然你也可以修改成不同目录，只需要指定 –dbpath 参数，
**示例：**
>bin/mongod –dbpath=d:\mgdata\db
更多请参看我的CSDN博客：[MongoDB 安装配置](http://blog.csdn.net/sunboy_2050/article/details/8555545)
**三、安装MongoDB PHP扩展**
根据自己的PHP版本下载PHP扩展：
[http://php.net/manual/en/mongo.installation.php\#mongo.installation.nix](http://php.net/manual/en/mongo.installation.php#mongo.installation.nix)
[http://www.php.net/manual/en/mongo.installation.php\#mongo.installation.windows](http://php.net/manual/en/mongo.installation.php#mongo.installation.windows)
提示：
1、VC6适合Apache、VC9适合IIS；
2、Thread safe适合PHP以模块运行方式、Non-thread safe适合CGI运行方式。
修改php.ini，加入：extension=php_mongo.dll，重启Web服务器。
Ubuntu安装步骤：
1. 下载插件：[mongo-php-driver](https://github.com/mongodb/mongo-php-driver/archive/master.zip)
2. 解压安装：
$ tar zxvf mongodb-mongodb-php-driver-<commit_id>.tar.gz
$ cd mongodb-mongodb-php-driver-<commit_id>$ phpize
$ ./configure
$ make all
$ sudo make install3.  配置php.ini
\#extension=uploadprogress.so
\#extension=http.so
\#extension=intl.so
\#extension=curl.so
extension=/usr/lib/php5/20121212/curl.so
extension=/usr/lib/php5/20121212/gd.so
extension=/usr/lib/php5/20121212/json.so
extension=/usr/lib/php5/20121212/mysql.so
extension=/usr/lib/php5/20121212/mysqli.so
\#extension=/usr/lib/php5/20121212/opcache.so
extension=/usr/lib/php5/20121212/pdo.so
extension=/usr/lib/php5/20121212/pdo_mysql.so
\#extension=/usr/lib/php5/20121212/readline.so
\#extension=/usr/lib/php5/20121212/xdebug.so
extension=/usr/local/lib/php/extensions/no-debug-non-zts-20121212/redis.so
extension=/usr/local/lib/php/extensions/no-debug-non-zts-20121212/mongo.so
4. 重启apache
sudo  /etc/init.d/apache2  restart
5. 修改默认启动项目（可选）
sudo  vim  /etc/apache2/apache2.conf
添加配置代码如下：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|<VirtualHost|*|:|80|>
|ServerAdmin webmaster@localhost
|DocumentRoot|/|home|/|homer|/|workspace
|<Directory|/|home|/|homer|/|workspace|/|money_client>
|DirectoryIndex index.html index.php
|Options Indexes FollowSymLinks
|AllowOverride|None
|Require|all|granted
|<|/|Directory>
|ServerName localhost
|DirectoryIndex index.html index.php
|ErrorLog ${APACHE_LOG_DIR}|/|error_alibuyer.log
|CustomLog ${APACHE_LOG_DIR}|/|access_alibuyer.log combined
|<|/|VirtualHost>
|

**四、PHP示例**
**1、连接Mongo服务器**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|<?php
|$conn|=|new|Mongo();|// 连接localhost:27017
|$conn|=|new|Mongo(|'test.com'|);|// 连接远程主机默认端口
|$conn|=|new|Mongo(|'test.com:22011'|);|//连接远程主机22011端口
|// MongoDB有用户名密码
|$conn|=|new|Mongo(|"mongodb://${username}:${password}@localhost"|);
|// MongoDB有用户名密码并指定数据库blog
|$conn|=|new|Mongo(|"mongodb://${username}:${password}@localhost/blog"|);
|// 多个服务器
|$conn|=|new|Mongo(|"mongodb://localhost:27017,localhost:27018"|);
|?>
|

或者使用**MongoClient()**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|<?php
|$MONGO_SERVER|=|array|(|'host'|=>|'10.123.55.16'|,
|'port'|=>27017,
|'dbname'|=>|'mimvp_money'|,
|'user'|=>|''|,
|'pwd'|=>|''|);
|$count|=|$coll|->|count|();
|print|(|"count: "|.|$count|);
|$host_port|=|$MONGO_SERVER|[|'host'|] .|":"|.|$MONGO_SERVER|[|'port'|];|// '10.123.55.16:20517'
|$conn|=|new|MongoClient(|$host_port|);
|$db|=|$conn|->selectDB(|$MONGO_SERVER|[|'dbname'|]);
|$coll|=|new|MongoCollection(|$db|,|$connName|);
|$conn|->close();
|?>
|

**2、指定数据库和数据集名**（表名）
|1
|2
|3
|4
|<?php
|$db|=|$conn|->blog;|// 选择数据库blog
|$collection|=|$db|->users;|// 制定结果集（表名：users）
|?>
|

**3、CRUD 操作**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|<?php
|$user|=|array|(|'name'|=>|'caleng'|,|'email'|=>|'admin@admin.com'|);|// 新增
|$collection|->insert(|$user|);
|$newdata|=|array|(|'$set'|=>|array|(|"email"|=>|"test@test.com"|));|// 修改
|$collection|->update(|array|(|"name"|=>|"caleng"|),|$newdata|);
|$collection|->remove(|array|(|'name'|=>|'caleng'|),|array|(|"justOne"|=> true));|// 删除
|$cursor|=|$collection|->find();|// 查找
|var_dump(|$cursor|);
|$user|=|$collection|->findOne(|array|(|'name'|=>|'caleng'|),|array|(|'email'|));|// 查找一条
|var_dump(|$user|);
|?>
|

**4、关闭连接**
|1
|2
|3
|<?php
|$conn|->close();
|?>
|

**五、示例代码**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|<?php
|//这里采用默认连接本机的27017端口，当然你也可以连接远程主机如192.168.0.4:27017,如果端口是27017，端口可以省略
|$m|=|new|Mongo();
|// 选择comedy数据库，如果以前没该数据库会自动创建，也可以用$m->selectDB("comedy");
|$db|=|$m|->comedy;
|//选择comedy里面的collection集合，相当于RDBMS里面的表，也-可以使用
|$collection|=|$db|->collection;
|$db|->selectCollection(|"collection"|);
|//添加一个元素
|$obj|=|array|(|"title"|=>|"Calvin and Hobbes-"|.|date|(|'i:s'|),|"author"|=>|"Bill Watterson"|);
|//将$obj 添加到$collection 集合中
|$collection|->insert(|$obj|);
|//添加另一个元素
|$obj|=|array|(|"title"|=>|"XKCD-"|.|date|(|'i:s'|),|"online"|=> true );
|$collection|->insert(|$obj|);
|//查询所有的记录
|$cursor|=|$collection|->find();
|//遍历所有集合中的文档
|foreach|(|$cursor|as|$obj|)
|{
|echo|$obj|[|"title"|] .|"<br />\n"|;
|}
|//删除所有数据
|//$collection->remove();
|//删除 name 为hm
|//$collection->remove(array('name'=>'hm'));
|//断开MongoDB连接
|$m|->close();
|?>

|


**PHP操作MongoDB数据库实例**
米扑理财：[http://money.mimvp.com](http://money.mimvp.com/)



# MongoDB3.0以上学习实践 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年07月10日 14:10:24[boonya](https://me.csdn.net/boonya)阅读数：343







MongoDB 是一个基于分布式文件存储的数据库。由C++语言编写。旨在为WEB应用提供可扩展的高性能数据存储解决方案。MongoDB是一个介于关系数据库和非关系数据库之间的产品，是非关系数据库当中功能最丰富，最像关系数据库的。他支持的数据结构非常松散，是类似json的bson格式，因此可以存储比较复杂的数据类型。Mongo最大的特点是他支持的查询语言非常强大，其语法有点类似于面向对象的查询语言，几乎可以实现类似关系数据库单表查询的绝大部分功能，而且还支持对数据建立索引。

## 下载

[https://www.mongodb.com/download-center#community](https://www.mongodb.com/download-center#community)


## 安装
Enterprise[https://docs.mongodb.com/manual/administration/install-enterprise/](https://docs.mongodb.com/manual/administration/install-enterprise/)
Ubuntu [https://docs.mongodb.com/manual/tutorial/install-mongodb-on-ubuntu/](https://docs.mongodb.com/manual/tutorial/install-mongodb-on-ubuntu/)
Windows  [https://docs.mongodb.com/manual/tutorial/install-mongodb-on-windows/](https://docs.mongodb.com/manual/tutorial/install-mongodb-on-windows/)


## CRUD操作
[https://docs.mongodb.com/manual/crud/](https://docs.mongodb.com/manual/crud/)


## 中文手册
[http://docs.mongoing.com/manual-zh/](http://docs.mongoing.com/manual-zh/)


## Shell方法
[https://docs.mongodb.com/manual/reference/method/](https://docs.mongodb.com/manual/reference/method/)


## MongoDBJava驱动

[http://mongodb.github.io/mongo-java-driver/3.4/](http://mongodb.github.io/mongo-java-driver/3.4/)


## MongoDBJava示例

[http://www.runoob.com/mongodb/mongodb-java.html](http://www.runoob.com/mongodb/mongodb-java.html)


## MongoDB教程

[http://www.runoob.com/mongodb/mongodb-tutorial.html](http://www.runoob.com/mongodb/mongodb-tutorial.html)


## MogoDB可视化工具robomongo



[https://robomongo.org/download](https://robomongo.org/download)




## 安装注意事项



windows下安装后需要手动添加MongoDB的数据目录启动，否则MongoDB客户端连不上，如下所示：



```
C:\Users\boonya>cd C:\Program Files\MongoDB\Server\3.4\bin

C:\Program Files\MongoDB\Server\3.4\bin>mongod -dbpath C:\DEVELOPERS\Mongo\data\db
2017-08-10T18:06:49.747+0800 I CONTROL  [initandlisten] MongoDB starting : pid=12892 port=27017 dbpath=C:\DEVELOPERS\Mongo\data\db 64-bit host=PengJunlin
2017-08-10T18:06:49.748+0800 I CONTROL  [initandlisten] targetMinOS: Windows 7/Windows Server 2008 R2
2017-08-10T18:06:49.748+0800 I CONTROL  [initandlisten] db version v3.4.5
2017-08-10T18:06:49.748+0800 I CONTROL  [initandlisten] git version: 520b8f3092c48d934f0cd78ab5f40fe594f96863
2017-08-10T18:06:49.748+0800 I CONTROL  [initandlisten] OpenSSL version: OpenSSL 1.0.1u-fips  22 Sep 2016
2017-08-10T18:06:49.748+0800 I CONTROL  [initandlisten] allocator: tcmalloc
2017-08-10T18:06:49.748+0800 I CONTROL  [initandlisten] modules: none
2017-08-10T18:06:49.748+0800 I CONTROL  [initandlisten] build environment:
2017-08-10T18:06:49.748+0800 I CONTROL  [initandlisten]     distmod: 2008plus-ssl
2017-08-10T18:06:49.748+0800 I CONTROL  [initandlisten]     distarch: x86_64
2017-08-10T18:06:49.749+0800 I CONTROL  [initandlisten]     target_arch: x86_64
2017-08-10T18:06:49.749+0800 I CONTROL  [initandlisten] options: { storage: { dbPath: "C:\DEVELOPERS\Mongo\data\db" } }
2017-08-10T18:06:49.864+0800 I STORAGE  [initandlisten] wiredtiger_open config: create,cache_size=3535M,session_max=20000,eviction=(threads_min=4,threads_max=4),config_base=false,statistics=(fast),log=(enabled=true,archive=true,path=journal,compressor=snappy),file_manager=(close_idle_time=100000),checkpoint=(wait=60,log_size=2GB),statistics_log=(wait=0),
2017-08-10T18:06:50.647+0800 I CONTROL  [initandlisten]
2017-08-10T18:06:50.647+0800 I CONTROL  [initandlisten] ** WARNING: Access control is not enabled for the database.
2017-08-10T18:06:50.648+0800 I CONTROL  [initandlisten] **          Read and write access to data and configuration is unrestricted.
2017-08-10T18:06:50.650+0800 I CONTROL  [initandlisten]
2017-08-10T18:06:51.190+0800 I FTDC     [initandlisten] Initializing full-time diagnostic data capture with directory 'C:/DEVELOPERS/Mongo/data/db/diagnostic.data'
2017-08-10T18:06:51.388+0800 I INDEX    [initandlisten] build index on: admin.system.version properties: { v: 2, key: { version: 1 }, name: "incompatible_with_version_32", ns: "admin.system.version" }
2017-08-10T18:06:51.388+0800 I INDEX    [initandlisten]          building index using bulk method; build may temporarily use up to 500 megabytes of RAM
2017-08-10T18:06:51.410+0800 I INDEX    [initandlisten] build index done.  scanned 0 total records. 0 secs
2017-08-10T18:06:51.413+0800 I COMMAND  [initandlisten] setting featureCompatibilityVersion to 3.4
2017-08-10T18:06:51.417+0800 I NETWORK  [thread1] waiting for connections on port 27017
2017-08-10T18:07:14.767+0800 I NETWORK  [thread1] connection accepted from 127.0.0.1:50669 #1 (1 connection now open)
2017-08-10T18:07:14.769+0800 I NETWORK  [conn1] received client metadata from 127.0.0.1:50669 conn1: { application: { name: "Robomongo" }, driver: { name: "MongoDB Internal Client", version: "3.4.3-7-gf394f8c" }, os: { type: "Windows", name: "Microsoft Windows 8", architecture: "x86_64", version: "6.2 (build 9200)" } }
2017-08-10T18:07:14.809+0800 I NETWORK  [thread1] connection accepted from 127.0.0.1:50670 #2 (2 connections now open)
2017-08-10T18:07:14.810+0800 I NETWORK  [conn2] received client metadata from 127.0.0.1:50670 conn2: { application: { name: "MongoDB Shell" }, driver: { name: "MongoDB Internal Client", version: "3.4.3-7-gf394f8c" }, os: { type: "Windows", name: "Microsoft Windows 8", architecture: "x86_64", version: "6.2 (build 9200)" } }
2017-08-10T18:08:34.251+0800 I COMMAND  [conn1] insert mongo-first-db.temp appName: "Robomongo" ninserted:1 keysInserted:1 numYields:0 locks:{ Global: { acquireCount: { r: 3, w: 3 } }, Database: { acquireCount: { w: 2, W: 1 } }, Collection: { acquireCount: { w: 2 } } } 182ms
2017-08-10T18:08:34.252+0800 I COMMAND  [conn1] CMD: drop mongo-first-db.temp
```

注意：目录如果权限不够需要修该数据文件目录位置。参考文章：[http://www.cnblogs.com/wzlblog/p/6364045.html](http://www.cnblogs.com/wzlblog/p/6364045.html)



## Windows命令行操作

### MongoDB服务启动

命令："C:\Program Files\MongoDB\Server\3.4\bin\mongod.exe"


`C:\Users\boonya>"C:\Program Files\MongoDB\Server\3.4\bin\mongod.exe"`


![](https://img-blog.csdn.net/20170828140139977)


注：请保持此服务窗口。

### 客户端命令行操作

命令："C:\Program Files\MongoDB\Server\3.4\bin\mongo.exe"



`C:\Users\boonya>"C:\Program Files\MongoDB\Server\3.4\bin\mongo.exe"`![](https://img-blog.csdn.net/20170828140217592)







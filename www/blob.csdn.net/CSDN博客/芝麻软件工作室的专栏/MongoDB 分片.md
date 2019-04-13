
# MongoDB 分片 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月31日 07:49:15[seven-soft](https://me.csdn.net/softn)阅读数：180个人分类：[MongoDB																](https://blog.csdn.net/softn/article/category/6339531)



## 分片
在Mongodb里面存在另一种集群，就是分片技术,可以满足MongoDB数据量大量增长的需求。
当MongoDB存储海量的数据时，一台机器可能不足以存储数据，也可能不足以提供可接受的读写吞吐量。这时，我们就可以通过在多台机器上分割数据，使得数据库系统能存储和处理更多的数据。
---

## 为什么使用分片
复制所有的写入操作到主节点
延迟的敏感数据会在主节点查询
单个副本集限制在12个节点
当请求量巨大时会出现内存不足。
本地磁盘不足
垂直扩展价格昂贵
---

## MongoDB分片
下图展示了在MongoDB中使用分片集群结构分布：
![](http://www.runoob.com/wp-content/uploads/2013/12/sharding.png)
上图中主要有如下所述三个主要组件：
Shard:用于存储实际的数据块，实际生产环境中一个shard server角色可由几台机器组个一个relica set承担，防止主机单点故障
Config Server:mongod实例，存储了整个 ClusterMetadata，其中包括 chunk信息。
Query Routers:前端路由，客户端由此接入，且让整个集群看上去像单一数据库，前端应用可以透明使用。

---

## 分片实例
分片结构端口分布如下：
ShardServer1：27020ShardServer2：27021ShardServer3：27022ShardServer4：27023ConfigServer：27100RouteProcess：40000步骤一：启动Shard Server
[root@100/]\#mkdir-p/www/mongoDB/shard/s0[root@100/]\#mkdir-p/www/mongoDB/shard/s1[root@100/]\#mkdir-p/www/mongoDB/shard/s2[root@100/]\#mkdir-p/www/mongoDB/shard/s3[root@100/]\#mkdir-p/www/mongoDB/shard/log[root@100/]\#/usr/local/mongoDB/bin/mongod--port27020--dbpath=/www/mongoDB/shard/s0--logpath=/www/mongoDB/shard/log/s0.log--logappend--fork....[root@100/]\#/usr/local/mongoDB/bin/mongod--port27023--dbpath=/www/mongoDB/shard/s3--logpath=/www/mongoDB/shard/log/s3.log--logappend--fork步骤二： 启动Config Server
[root@100/]\#mkdir-p/www/mongoDB/shard/config[root@100/]\#/usr/local/mongoDB/bin/mongod--port27100--dbpath=/www/mongoDB/shard/config--logpath=/www/mongoDB/shard/log/config.log--logappend--fork注意：这里我们完全可以像启动普通mongodb服务一样启动，不需要添加—shardsvr和configsvr参数。因为这两个参数的作用就是改变启动端口的，所以我们自行指定了端口就可以。
步骤三： 启动Route Process
/usr/local/mongoDB/bin/mongos--port40000--configdb localhost:27100--fork--logpath=/www/mongoDB/shard/log/route.log--chunkSize500mongos启动参数中，chunkSize这一项是用来指定chunk的大小的，单位是MB，默认大小为200MB.
步骤四： 配置Sharding
接下来，我们使用MongoDB Shell登录到mongos，添加Shard节点
[root@100 shard]\#/usr/local/mongoDB/bin/mongo admin--port40000MongoDBshell version:2.0.7connecting to:127.0.0.1:40000/admin
mongos>db.runCommand({addshard:"localhost:27020"}){"shardAdded":"shard0000","ok":1}......mongos>db.runCommand({addshard:"localhost:27029"}){"shardAdded":"shard0009","ok":1}mongos>db.runCommand({enablesharding:"test"})\#设置分片存储的数据库{"ok":1}mongos>db.runCommand({shardcollection:"test.log",key:{id:1,time:1}}){"collectionsharded":"test.log","ok":1}步骤五： 程序代码内无需太大更改，直接按照连接普通的mongo数据库那样，将数据库连接接入接口40000


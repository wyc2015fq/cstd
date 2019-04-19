# MongoDB集群——分片 - 左直拳的马桶_日用桶 - CSDN博客
2014年10月03日 22:04:26[左直拳](https://me.csdn.net/leftfist)阅读数：3882
**1、分片的结构及原理**
分片集群结构分布：
![](https://img-blog.csdn.net/20141003220136359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20141003220324118?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
分片集群主要由三种组件组成：mongos，config server，shard
1)MONGOS
数据库集群请求的入口，所有的请求都通过mongos进行协调，不需要在应用程序添加一个路由选择器，mongos自己就是一个请求分发中心，它负责把对应的数据请求请求转发到对应的shard服务器上。在生产环境通常有多个mongos作为请求的入口，防止其中一个挂掉所有的mongodb请求都没有办法操作。
2)CONFIG SERVER
顾名思义为配置服务器，存储所有数据库元信息（路由、分片）的配置。mongos本身没有物理存储分片服务器和数据路由信息，只是缓存在内存里，配置服务器则实际存储这些数据。
mongos第一次启动或者关掉重启就会从 config server 加载配置信息，以后如果配置服务器信息变化会通知到所有的 mongos 更新自己的状态，这样 mongos 就能继续准确路由。在生产环境通常有多个 config server 配置服务器，因为它存储了分片路由的元数据，这个可不能丢失！就算挂掉其中一台，只要还有存货， mongodb集群就不会挂掉。
3)SHARD
这就是传说中的分片了。
如图所示，一台机器的一个数据表 Collection1 存储了 1T 数据，压力太大了！在分给4个机器后，每个机器都是256G，则分摊了集中在一台机器的压力。
事实上，上图4个分片如果没有副本集（replica set）是个不完整架构，假设其中的一个分片挂掉那四分之一的数据就丢失了，所以在高可用性的分片架构还需要对于每一个分片构建 replica set 副本集保证分片的可靠性。生产环境通常是 2个副本 + 1个仲裁。
**2、搭建分片**
假设我们要配置如下图所示的一个分片集群
![](https://img-blog.csdn.net/20141003220225656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
因为没有足够的机器，现在都部署于同一台机器上，通过不同端口来进行划分：
||“机器”一|“机器”二|“机器”三|
|----|----|----|----|
|mongos|20000|20001|20002|
|config|30000|30001|30002|
|shard|40000，40001(副本），40002(仲裁)|41000，41001(副本），42002(仲裁)|42000，42001(副本），42002(仲裁)|
1)启动配置服务器
示例，启动3台“机器”的配置服务器：
mongod --configsvr --dbpath config\1\data --port 30000 --logpath config\1\log\config.log
mongod --configsvr --dbpath config\2\data --port 30001 --logpath config\2\log\config.log
mongod --configsvr --dbpath config\3\data --port 30002 --logpath config\3\log\config.log
2)启动MONGOS服务器
示例，启动3台“机器”的mongos，jsj-1306-201是在下电脑的机器名：
mongos --configdb jsj-1306-201:30000,jsj-1306-201:30001,jsj-1306-201:30002 --port 20000 --logpath mongos\1\log\mongos.log
mongos --configdb jsj-1306-201:30000,jsj-1306-201:30001,jsj-1306-201:30002 --port 20001 --logpath mongos\2\log\mongos.log
mongos --configdb jsj-1306-201:30000,jsj-1306-201:30001,jsj-1306-201:30002 --port 20002 --logpath mongos\3\log\mongos.log
3)启动分片及其副本集实例
shard1：
mongod --shardsvr --replSet shard1 --port 40000 --dbpath shard\1\data  --logpath shard\1\log\shard.log --nojournal  --oplogSize 10
mongod --shardsvr --replSet shard1 --port 40001 --dbpath shard\1_1\data  --logpath shard\1_1\log\shard.log --nojournal  --oplogSize 10
mongod --shardsvr --replSet shard1 --port 40002 --dbpath shard\1_2\data  --logpath shard\1_2\log\shard.log --nojournal  --oplogSize 10
shard2:
mongod --shardsvr --replSet shard2 --port 41000 --dbpath shard\2\data  --logpath shard\2\log\shard.log --nojournal  --oplogSize 10
mongod --shardsvr --replSet shard2 --port 41001 --dbpath shard\2_1\data  --logpath shard\2_1\log\shard.log --nojournal  --oplogSize 10
mongod --shardsvr --replSet shard2 --port 41002 --dbpath shard\2_2\data  --logpath shard\2_2\log\shard.log --nojournal  --oplogSize 10
shard3:
mongod --shardsvr --replSet shard3 --port 42000 --dbpath shard\3\data  --logpath shard\3\log\shard.log --nojournal  --oplogSize 10
mongod --shardsvr --replSet shard3 --port 42001 --dbpath shard\3_1\data  --logpath shard\3_1\log\shard.log --nojournal  --oplogSize 10
mongod --shardsvr --replSet shard3 --port 42002 --dbpath shard\3_2\data  --logpath shard\3_2\log\shard.log --nojournal  --oplogSize 10
4)启动分片副本集
登录各个副本集中的任意实例，进行副本集初始化
config={_id:"shard1",members:[
{_id:0,host:"jsj-1306-201:40000","priority":2},
{_id:1,host:"jsj-1306-201:40001"},
{_id:2,host:"jsj-1306-201:40002",arbiterOnly:true}
]
}
config={_id:"shard2",members:[
{_id:0,host:"jsj-1306-201:41000",arbiterOnly:true},
{_id:1,host:"jsj-1306-201:41001","priority":2},
{_id:2,host:"jsj-1306-201:41002"}
]
}
config={_id:"shard3",members:[
{_id:0,host:"jsj-1306-201:42000"},
{_id:1,host:"jsj-1306-201:42001",arbiterOnly:true},
{_id:2,host:"jsj-1306-201:42002","priority":2}
]
}
详细操作方法参考副本集
5)设置分片，让分片生效
连接mongos，比如
mongo jsj-1306-201:20000
use admin
db.runCommand({addshard:"shard1/jsj-1306-201:40000,jsj-1306-201:40001,jsj-1306-201:40002"});
db.runCommand({addshard:"shard2/jsj-1306-201:41000,jsj-1306-201:41001,jsj-1306-201:41002"});
db.runCommand({addshard:"shard3/jsj-1306-201:42000,jsj-1306-201:42001,jsj-1306-201:42002"});
6)查看分片
在mongos下
db.runCommand( { listshards : 1 } );
**3、测试分片**
在mongos下：
mongos> use test
switched to db test
mongos> use admin
switched to db admin
mongos> db.runCommand({enablesharding:"testdb"});
{ "ok" : 1 }
mongos> db.runCommand({shardcollection:"testdb.table1",key:{id:1}});
{ "collectionsharded" : "testdb.table1", "ok" : 1 }
mongos> use testdb;
switched to db testdb
mongos> for(var i=1;i<=100000;i++) db.table1.save({id:i,"test1":"testval1"});
查阅数据分布情况：
mongos> db.table1.stats();
**4、查阅分片信息**
分片集群的信息保存在congfig服务器，名为config的数据库里。通过mongos可以进行访问。
mongos> use config
1)查询当前的版本
mongos> db.getCollection("version").findOne()
2)查询当前的配置CHUNKSIZE的大小
mongos> db.settings.find()
3)查询整个SHARD集群的成员
mongos> db.shards.find()
4)查询被水平拆分的集合：
mongos>  db.collections.find()
5)查询被水平拆分的集合分成的CHUNK分布
mongos> db.chunks.find()
6)查询当前MONGODB里面的数据库的SHARD信息：
mongos> db.databases.find()
7)查询MONGOS集合
mongos> db.mongos.find()
8)查看整个MONGODB 数据库SHARD的详细信息
mongos> printShardingStatus();
参考文章：
http://www.lanceyan.com/tech/arch/mongodb_shard1.html
http://blog.itpub.net/22664653/viewspace-710281/


# MongoDB 常用命令 - 阳光岛主 - CSDN博客

2013年12月11日 22:12:19[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：10875个人分类：[SQL Index																](https://blog.csdn.net/sunboy_2050/article/category/581634)



**[mongodb](http://www.mongodb.org/)**由C++编写，其名字来自humongous这个单词的中间部分，从名字可见其野心所在就是海量数据的处理。关于它的一个最简洁描述为：scalable, high-performance, open source, schema-free, document-oriented database。MongoDB的主要目标是在键/值存储方式（提供了高性能和高度伸缩性）以及传统的RDBMS系统（丰富的功能）架起一座桥梁，集两者的优势于一身。
**安装使用：**
首先在Ubuntu上安装MongoDB。
下载MongoDB, 现在最新的生产版本1.7.0
1. 解压文件.
$ tar -xvf mongodb-linux-i686-1.4.3.tgz
2. 为MongoDB创建数据目录，默认情况下它将数据存储在/data/db
$ sudo mkdir -p /data/db/
$ sudo chown `id -u` /data/db
3. 启动MongoDB服务.
$ cd mongodb-linux-i686-1.4.3/bin
$ ./mongod
4. 打开另一个终端，并确保你在MongoDB的bin目录，输入如下命令.
$ ./mongo

**一些概念**
一个mongod服务可以有建立多个数据库，每个数据库可以有多张表，这里的表名叫collection，每个collection可以存放多个文档（document），每个文档都以BSON（binary json）的形式存放于硬盘中，因此可以存储比较复杂的数据类型。它是以单文档为单位存储的，你可以任意给一个或一批文档新增或删除字段，而不会对其它文档造成影响，这就是所谓的schema-free，这也是文档型数据库最主要的优点。跟一般的key-value数据库不一样的是，它的value中存储了结构信息，所以你又可以像关系型数据库那样对某些域进行读写、统计等操作。Mongo最大的特点是他支持的查询语言非常强大，其语法有点类似于面向对象的查询语言，几乎可以实现类似关系数据库单表查询的绝大部分功能，而且还支持对数据建立索引。Mongo还可以解决海量数据的查询效率，根据官方文档，当数据量达到50GB以上数据时，Mongo数据库访问速度是MySQL10 倍以上。
**BSON**
BSON是Binary JSON 的简称，是一个JSON文档对象的二进制编码格式。BSON同JSON一样支持往其它文档对象和数组中再插入文档对象和数组，同时扩展了JSON的数据类型。如：BSON有Date类型和BinDate类型。
BSON被比作二进制的交换格式，如同Protocol Buffers，但BSON比它更“schema-less”，非常好的灵活性但空间占用稍微大一点。
BSON有以下三个特点：
1．  轻量级
2．  跨平台
3．  效率高
**命名空间**
MongoDB存储BSON对象到collections,这一系列的数据库名和collection名被称为一个命名空间。如同：java.util.List;用来管理数据库中的数据。
**索引**
mongodb可以对某个字段建立索引，可以建立组合索引、唯一索引，也可以删除索引，建立索引就意味着增加空间开销。默认情况下每个表都会有一个唯一索引：_id，如果插入数据时没有指定_id，服务会自动生成一个_id，为了充分利用已有索引，减少空间开销，最好是自己指定一个unique的key为_id，通常用对象的ID比较合适，比如商品的ID。
shell操作数据库：
**1.  超级用户：**
1. \#进入数据库admin
use admin
2. \#增加或修改用户密码
db.addUser('name','pwd')
3. \#查看用户列表
db.system.users.find()
4. \#用户认证
db.auth('name','pwd')
5. \#删除用户
db.removeUser('name')
6. \#查看所有用户
show users
7. \#查看所有数据库
show dbs
8. \#查看所有的collection
show collections
9. \#查看各collection的状态
db.printCollectionStats()
10. \#查看主从复制状态
db.printReplicationInfo()
11. \#修复数据库
db.repairDatabase()
12. \#设置记录profiling，0=off 1=slow 2=all
db.setProfilingLevel(1)
13. \#查看profiling
show profile
14. \#拷贝数据库
db.copyDatabase('mail_addr','mail_addr_tmp')
15. \#删除collection
db.mail_addr.drop()
16. \#删除当前的数据库
db.dropDatabase()
17. 备份数据库
mongodump -h localhost:27017 -d dataname -o /data/dump
18. 恢复数据库
mongorestore -d dataname /data/dump
19. 备份数据库表
mongodump -h localhost:27017 -d dataname -c tablename -o /data/dump
20. 恢复数据库表
mongorestore -d dataname -c tablename /data/dump
mongorestore -h host:port -d dataname --collection tablename ./tmpdump/some.bson
21. 远程连接服务器mongodb (mongo --help)
mongo--host18.126.23.156--port27017

**2. 增删改**
1. \#存储嵌套的对象
db.foo.save({'name':'ysz','address':{'city':'beijing','post':100096},'phone':[138,139]})
2. \#存储数组对象
db.user_addr.save({'Uid':'yushunzhi@sohu.com','Al':['test-1@sohu.com','test-2@sohu.com']})
3. \#根据query条件修改，如果不存在则插入，允许修改多条记录
db.foo.update({'yy':5},{'$set':{'xx':2}},upsert=true,multi=true)      // 增加/修改一个字段
db.foo.update({'yy':5},{'$unset':{'xx':2}},upsert=true,multi=true)     // 删除一个字段
4. \#删除yy=5的记录
db.foo.remove({'yy':5})
5. \#删除所有的记录
db.foo.remove()
6. 插入_id为整数
db.foo.insert({"_id":NumberLong(1),"cpname":"-"})
**3. 索引**
1. \#增加索引：1(ascending),-1(descending)
2. db.foo.ensureIndex({firstname: 1, lastname: 1}, {unique: true, dropDups: true});
3. \#索引子对象
4. db.user_addr.ensureIndex({'Al.Em': 1})
5. \#查看索引信息
6. db.foo.getIndexes()
7. db.foo.getIndexKeys()
8. \#根据索引名删除索引
9. db.user_addr.dropIndex('Al.Em_1')
**4. 查询**
1. \#查找所有
2. db.foo.find()
3. \#查找一条记录
4. db.foo.findOne()
5. \#根据条件检索10条记录
6. db.foo.find({'msg':'Hello 1'}).limit(10)
7. \#sort排序
8. db.deliver_status.find({'From':'ixigua@sina.com'}).sort({'_id':-1})
python 命令行中的一个字段排序：db.galleryimg.find({},{"_id":1,"publishtime":1}).sort({"_id":-1},{"publishtime":-1}).limit(100)
9. db.deliver_status.find().sort({'Ct':-1}).limit(1)
python 代码中的一个字段排序：db.deliver_status.find().sort('Ct',-1).limit(1)
python 代码中的多字段排序：db.deliver_status.find().sort([('Ct',-1),('name', pymongo.DESCENDING), ('sex', 1)]).limit(1)
10. \#count操作
11. db.user_addr.count()
12. \#distinct操作,查询指定列，去重复
13. db.foo.distinct('msg')
14. \#”>=”操作
15. db.foo.find({"timestamp": {"$gte" : 2}})
16. \#子对象的查找
17. db.foo.find({'address.city':'beijing'})
**5. 管理**
1. \#查看collection数据的大小
2. db.deliver_status.dataSize()
3. \#查看colleciont状态
4. db.deliver_status.stats()
5. \#查询所有索引的大小
6. db.deliver_status.totalIndexSize()

**5.  高级查询**
**条件操作符**
$gt : >
$lt : <
$gte: >=
$lte: <=
$ne : !=、<>
$in : in
$nin: not in
$all: all
$not:反匹配(1.3.3及以上版本)
查询 name <> "bruce" and age >= 18 的数据
db.users.find({name: {$ne: "bruce"}, age: {$gte: 18}});
查询 creation_date > '2010-01-01' and creation_date <= '2010-12-31' 的数据
db.users.find({creation_date:{$gt:new Date(2010,0,1), $lte:new Date(2010,11,31)});
查询 age in (20,22,24,26) 的数据
db.users.find({age: {$in: [20,22,24,26]}});
查询 age取模10等于0 的数据
db.users.find('this.age % 10 == 0');
或者
db.users.find({age : {$mod : [10, 0]}});
匹配所有
db.users.find({favorite_number : {$all : [6, 8]}});
可以查询出{name: 'David', age: 26, favorite_number: [ 6, 8, 9 ] }
可以不查询出{name: 'David', age: 26, favorite_number: [ 6, 7, 9 ] }
查询不匹配name=B*带头的记录
db.users.find({name: {$not: /^B.*/}});
查询 age取模10不等于0 的数据
db.users.find({age : {$not: {$mod : [10, 0]}}});
**\#返回部分字段**
选择返回age和_id字段(_id字段总是会被返回)
db.users.find({}, {age:1});
db.users.find({}, {age:3});
db.users.find({}, {age:true});
db.users.find({ name : "bruce" }, {age:1});
0为false, 非0为true
选择返回age、address和_id字段
db.users.find({ name : "bruce" }, {age:1, address:1});
排除返回age、address和_id字段
db.users.find({}, {age:0, address:false});
db.users.find({ name : "bruce" }, {age:0, address:false});
数组元素个数判断
对于{name: 'David', age: 26, favorite_number: [ 6, 7, 9 ] }记录
匹配db.users.find({favorite_number: {$size: 3}});
不匹配db.users.find({favorite_number: {$size: 2}});
$exists判断字段是否存在
查询所有存在name字段的记录
db.users.find({name: {$exists: true}});
查询所有不存在phone字段的记录
db.users.find({phone: {$exists: false}});
$type判断字段类型
查询所有name字段是字符类型的
db.users.find({name: {$type: 2}});
查询所有age字段是整型的
db.users.find({age: {$type: 16}});
对于字符字段，可以使用正则表达式
查询以字母b或者B带头的所有记录
db.users.find({name: /^b.*/i});
$elemMatch(1.3.1及以上版本)
为数组的字段中匹配其中某个元素
Javascript查询和$where查询
查询 age > 18 的记录，以下查询都一样
db.users.find({age: {$gt: 18}});
db.users.find({$where: "this.age > 18"});
db.users.find("this.age > 18");
f = function() {return this.age > 18} db.users.find(f);
**排序sort()**
以年龄升序asc
db.users.find().sort({age: 1});
以年龄降序desc
db.users.find().sort({age: -1});
限制返回记录数量limit()
返回5条记录
db.users.find().limit(5);
返回3条记录并打印信息
db.users.find().limit(3).forEach(function(user) {print('my age is ' + user.age)});
结果
my age is 18
my age is 19
my age is 20
限制返回记录的开始点skip()
从第3条记录开始，返回5条记录(limit 3, 5)
db.users.find().skip(3).limit(5);
查询记录条数count()
db.users.find().count();
db.users.find({age:18}).count();
以下返回的不是5，而是user表中所有的记录数量
db.users.find().skip(10).limit(5).count();
如果要返回限制之后的记录数量，要使用count(true)或者count(非0)
db.users.find().skip(10).limit(5).count(true);
**分组group()**
假设test表只有以下一条数据
{ domain: "[www.mongodb.org](http://www.mongodb.org/)"
, invoked_at: {d:"2009-11-03", t:"17:14:05"}
, response_time: 0.05
, http_action: "GET /display/DOCS/Aggregation"
}
使用group统计test表11月份的数据count:count(*)、total_time:sum(response_time)、avg_time:total_time/count;
db.test.group(
{ cond: {"invoked_at.d": {$gt: "2009-11", $lt: "2009-12"}}
, key: {http_action: true}
, initial: {count: 0, total_time:0}
, reduce: function(doc, out){ out.count++; out.total_time+=doc.response_time }
, finalize: function(out){ out.avg_time = out.total_time / out.count }
} );
[
{
"http_action" : "GET /display/DOCS/Aggregation",
"count" : 1,
"total_time" : 0.05,
"avg_time" : 0.05
}
]

**Mongo-data 对日期类型数据的操作**

一、Mongo直接操作日期类型：
|1|db.guideline.find({|'monitorDate'|: {|'$gte'|:|new|Date(|'2013-9-22 00:00:00'|),|'$lte'|:|new|Date(|'2013-9-22 23:59:59'|)}})|


或者
|1|db.guideline.find({|"monitorDate"|: {|"$gte"|: ISODate(|"2013-09-21T16:00:00Z"|),|"$lte"|: ISODate(|"2013-09-22T15:59:59Z"|) } })|



二、Spring-data的操作：
|01|Query query =|new|Query();|
|02|
|03|//添加日期限制，只获取当天的信息|
|04|SimpleDateFormat format =|new|SimpleDateFormat(|"yyyy-MM-dd 00:00:00"|);|
|05|Date date =|new|Date();|
|06|
|07|try|{|
|08|query.addCriteria(Criteria.where(|"monitorDate"|).gte(format.parse(format.format(date))));|
|09|}|catch|(ParseException e) {|
|10|e.printStackTrace();|
|11|}|



**参考推荐：**
[mongodb常用命令](http://www.cnblogs.com/cxd4321/archive/2011/06/24/2089051.html)
[MongoDB 安装配置](http://blog.csdn.net/ithomer/article/details/8555545)
[8篇MongoDB教程快速学会入门](http://www.eduyo.com/database/nosql/p3.html)
[Mongo 翻译 官方文档](http://blog.csdn.net/tgipfv/article/category/1331918)（推荐）
[Mongo 官方文档](http://docs.mongodb.org/manual/)（英文）
[MongoDB 日期查询与mongodump 按日期范围导出数据](http://nosqldb.org/topic/512b3dc7cd0cc40a0a0605d3)


db.foo.update({'yy':5},{'$set':{'xx':2}},upsert=true,multi=true)


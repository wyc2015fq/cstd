
# MongoDB 学习笔记 - 阳光岛主 - CSDN博客

2013年12月12日 23:54:50[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：4513


**基本概念：**
1、文档 --> 对应关系数据库的行，也就是一条记录。它比关系数据库的行的功能要强大，更像是是某个具体的对象。文档以一种Map的形式展现出来，当然value可以是任意的类型，也可以继续是一个文档（递归的定义）
2、集合 --> 对应关系数据库的表。但是它又是无模式的，即文档不要求一致。

**基本操作：**
1、显示数据库
show dbs
2、创建数据库：
use yourDateBaseName 这就创建了一个数据库。
不过你用show dbs，并不能显示yourDataBaseName，因为这个数据库没有任何操作。所以如果你直接离开，则本数据库会被废除。
如果你插入一条记录，此时就可以通过show dbs看到对应的数据库
如下：
record = {"name" : "wang", "age" : 20, "password" : "123456"}
db.yiyou.you_upload_photo.insert(record)
这时使用show dbs就可以看到你的数据库了
3、显示数据库
show collections。默认的包含system.indexes 表
4、创建集合：
因为mongodb中集合是无模式的，不像传统的关系型数据库是需要预先申明表包含哪些字段并且定义字段的属性。虽有无需预先定义。使用的时候直接插入。可以见上面的例子。
5、删除集合
db.yourDatabaseName.yourCollectionName.drop()
6、将文档插入集合
record = {"name" : "wang", "age" : 20, "password" : "123456"}
db.yourDatabaseName.yourCollectionName.insert(record)
7、删除集合中的文档
db.yourDatabaseName,yourCollectionName.remove()
会清空此集合中的所有文档
8、文档更新
Mongo中的更新有两种
一种是：用新的文档去替换旧的文档。【就是通过条件先将满足条件的选出来该文档，修改完之后替换旧的文档】，个人不喜欢这种操作，违背了原子性啊。
另外一种：采用Mongo的修改器对文档进行修改。
修改器列表：
8.1、“$set”修改器：用了指定一个健的值。如果健不存在，就创建它。
Example:
db.myMongodb.user.update({"_id":ObjectId("4ed373c46d375f1a1960ed07")}, {"$set" : {"sex" : "male"}})
“$unset”删除某个健
Example:
db.myMongodb.user.update({"_id":ObjectId("4ed373c46d375f1a1960ed07")},  {"$unset" : {"sex" : 1}})
8.2、“$inc”修改器用了增加已有的健的值，或者在键不存在的时候创建一个健。
Example:
db.myMongodb.user.update({"_id":ObjectId("4ed373c46d375f1a1960ed07")},  {"$inc" : {"age" : 1}})
Tips: $inc 只能用于整数，长整数和双精度浮点数。其他数会导致失败。
8.3、“$push”会向已有的数组末尾加入一个元素，要是没有就会创建一个新的数组。
Example:
db.myMongodb.user.update({"_id":ObjectId("4ed373c46d375f1a1960ed07")},
{$push : {"comments" : "this is my comments"}})
8.4、“$ne”not equal判定器，【判定给定的属性不等于给定值】
Example:
db.myMongodb.user.update({"comments" : {"$ne" : "this is comments"},
{$push : {"comments" : "your comments"}})
8.5、“$addToSet”在不重复的情况下添加
Example:
db.myMongodb.user.update({"_id":ObjectId("4ed373c46d375f1a1960ed07")},
{"$addToSet" : {"comments" : "your comments"}})
8.6、“$each”数组遍历修改器
Example:
db.myMongodb.user.update({"_id":ObjectId("4ed373c46d375f1a1960ed07")},
{"$addToSet" : {"comments" : {"$each" : ["a commnet", "b comment"]}}})
8.7、“$pop”修改器，从数组中删除任何一端删除元素
Example:
{$pop : {key : 1}}从数组末尾删除一个元素
{$pop : {key : -1}}从数组头部删除
8.8、数组定位修改器
Example:
db.myMongodb.user.update({"comments.author" : "john"},
{"$set" : {"comments.$.author" : "jim"}})
定位到第一个john的第一条评论
9、特殊的更新or插入 upsert
Upsert的特点是如果存在就更新，如果不存在就根据update的条件插入一条新的记录
10、批量更新
需要设置update的第四个参数为true,否则默认更新第一个匹配的文档
11、得到上一条命令的执行结果
db.runCommand({getLastError : 1})

实验室从去年开始接触Nosql,不过那个时候自己不是负责这部分的所以只是介于组会的时候讨论了解。后来去实习，这个东东越来越火了。以至于俺觉得自己也要去学习一下，这些天一直看这方面的东西。最大的感觉就是它组织结构的变化带来的方便，这也是算是肤浅的感觉吧，新手一般都有这样的体验，至于网上一直叫好的速度快（据说比传统的关系型数据库快5倍左右），由于木有实战的体验，目前还木有深刻的认识。前段时间一直在做一个MIS系统，被数据库折腾的却是够呛，数据库设计的人太过考虑各种范式，以至于开发困难很多。在看看mogodb确实给人焕然一新的感觉。呵呵。目前感觉就是这么多，继续记录一下自己的学习的笔记，这部分主要是mogodb的查询操作。mogodb的查询功能确实比标准的sql强大很多，特别是各种修改器和基于js函数的$where查询。

**Mongodb 查询操作**
1、查询全部集合
db.myMongodb.post.find()
如果是console的话，默认显示20条记录
2、返回指定的键
db.myMongodb.post.find({}, {"foo" : 1, "baz" : 1})
返回键foo和健baz，同时默认返回_id键的值
3、指定不返回的键
db.myMongodb.post.find({}, {"foo" : 0})
返回除foo之外的其他键
4、判定条件查询
a) “$lt” 小于
b) “$lte”小于等于
c) “$gt”大于
d) “$gte”大于等于
e) “$ne”不等于
db.myMongodb.user.find({"age" : {"$gte" : 18, "$lte" : 30}})
5、其他条件查询
a) “$in” ：是否在某一个集合的查询
db.myMongodb.user.find({"age" : {"$in" : [17,18,19]}})
b) “$nin”：与in相对应，不处于某个结合的文档
c) “$or”：多条件查询
db.myMongodb.user.find({"$or" : [{"age" : 17}, {"age" : 18}]})
d) “$not”非的情况下使用。其他条件可以基于它
e) “$mod”取模查询
f) “$null”可以匹配值为null的字段，同时也能匹配键不存在的文档
如果同时要判定键存在，需要加上"$exists" 判定
6、查询数组
a) “$all” 通过多个元素匹配数组
db.myMongodb.food.find({"fruit" : {"$all" : ["apple", "oracle"]}})
b) 数组下标匹配
db.myMongodb.food.find({"fruit.2" :  "peach"})
c) "$size"数组长度匹配
db.myMongodb.food.find("fruit" : {"$size" : 3})
7、"$slice"指定返回集合的个数
db.myMongodb.posts.find(criteria, {"$silce" : n})
n = 10 返回前10条记录
n = -10 返回后10条记录
n = [23, 10] 返回从23条记录开始的10条记录，感觉应该可以做分页
8、采用点表示法查询内嵌文档(推荐)
db.myMongodb.find({"name.first" : "Wang", "name.last" : "Hai"})
9、采用分层的方式查询
db.myMongodb.find({"name" : {"first" : "Wang, "last" : "Hai""}})
10、$where查询
$where查询非常强大，它可以添加js的函数实现高级的查询功能。
基本样式如下：
db.myMongodb.find({"$where" : function() {}})
如果function返回true，该记录就返回。否则就不返回。


**参考推荐：**
[mongodb 常用命令](http://blog.csdn.net/ithomer/article/details/17111943)
[MongoDB SQL语法对比](http://blog.csdn.net/ithomer/article/details/13508013)
[MongoDB 客户端 MongoVue](http://blog.csdn.net/ithomer/article/details/13290425)




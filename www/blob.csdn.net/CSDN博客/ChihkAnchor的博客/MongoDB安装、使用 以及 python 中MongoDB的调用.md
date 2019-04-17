# MongoDB安装、使用 以及 python 中MongoDB的调用 - ChihkAnchor的博客 - CSDN博客





2019年02月13日 23:58:14[Chihk-Anchor](https://me.csdn.net/weixin_40871455)阅读数：49








### 安装：

Windows 平台安装 MongoDB：[http://www.runoob.com/mongodb/mongodb-window-install.html](http://www.runoob.com/mongodb/mongodb-window-install.html)

### 使用：

成功启动MongoDB后，再打开一个命令行窗口输入mongo，启动MongoDB客户端，就可以进行数据库操作了。

输入help可以看到基本操作命令：

![](https://img-blog.csdnimg.cn/20190213214121464.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

show dbs:显示数据库列表 

show collections：显示当前数据库中的集合（类似关系数据库中的表） 

show users：显示用户

use <db name>：切换数据库

db.help()：显示数据库操作命令，里面有很多的命令 

db.foo.help()：显示集合操作命令，同样有很多的命令，foo指的是当前数据库下，一个叫foo的集合，并非真正意义上的命令 ，

db.foo.find()：对于当前数据库中的foo集合进行数据查找（由于没有条件，会列出所有数据） 

db.foo.find( { a : 1 } )：对于当前数据库中的foo集合进行查找，条件是数据中有一个属性叫a，且a的值为1

**MongoDB没有创建数据库的命令，但有类似的命令。**

MongoDB的数据库不需特定的create命令来创建，直接use，use后插入数据 数据库会自动生成

如：如果你想创建一个"myTest"的数据库，先运行use myTest命令，之后就做一些操作（如：db.createCollection('user')）,这样就可以创建一个名叫"myTest"的数据库。



**数据库常用命令：**

1、切换/创建数据库

 use testDB;  当创建一个集合(table)的时候会自动创建当前数据库

2、查询所有数据库

 show dbs;

3、删除当前使用数据库

 db.dropDatabase();    db代表当前使用的数据库

4、从指定主机上克隆数据库

 db.cloneDatabase("127.0.0.1"); 将指定机器上的数据库的数据克隆到当前数据库

5、从指定的机器上复制指定数据库数据到某个数据库

 db.copyDatabase("mydb", "tmp", "127.0.0.1");将本机的mydb的数据复制到tmp数据库中

6、修复当前数据库

 db.repairDatabase();

7、查看当前使用的数据库

 db.getName();

8、显示当前db状态

 db.stats();

9、当前db版本

 db.version();

10、查看当前db的链接机器地址

 db.getMongo();



**Collection聚集集合**

1、创建一个聚集集合（table）

db.createCollection(name, options)      name: 要创建的集合名称    options: 可选参数, 指定有关内存大小及索引的选项

options 可以是如下参数：
|字段|类型|描述|
|----|----|----|
|capped|布尔|（可选）如果为 true，则创建固定集合。固定集合是指有着固定大小的集合，当达到最大值时，它会自动覆盖最早的文档。**当该值为 true 时，必须指定 size 参数。**|
|autoIndexId|布尔|（可选）如为 true，自动在 _id 字段创建索引。默认为 false。|
|size|数值|（可选）为固定集合指定一个最大值（以字节计）。**如果 capped 为 true，必须要指定该字段。**|
|max|数值|（可选）指定固定集合中包含文档的最大数量。|

在插入文档时，MongoDB 首先检查固定集合的 size 字段，然后检查 max 字段。

 例如：在 test 数据库中创建 runoob 集合：

```
> use test
switched to db test
> db.createCollection("mycollection", { capped : true, autoIndexId : true, size : 6142800, max : 10000 } )
{ "ok" : 1 }
```

2、得到指定名称的聚集集合（table）

 db.getCollection("mycollection");

3、得到当前db的所有聚集集合

 db.getCollectionNames();     或    show collections

4、显示当前db所有聚集索引的状态

 db.printCollectionStats();



** 用户相关**

1、添加一个用户

 db.addUser("name");

 db.addUser("userName", "pwd123", true); 添加用户、设置密码、是否只读

2、数据库认证、安全模式

 db.auth("userName", "123123");

3、显示当前所有用户

 show users;

4、删除用户

 db.removeUser("userName");



**其他**

1、查询之前的错误信息

 db.getPrevError();

2、清除错误记录

 db.resetError();



**查看聚集集合基本信息**

1、查看帮助 db.mycollection.help();

2、查询当前集合的数据条数 db.mycollection.count();

3、查看数据空间大小 db.mycollection.dataSize();

4、得到当前聚集集合所在的db db.mycollection.getDB();

5、得到当前聚集的状态 db.mycollection.stats();

6、得到聚集集合总大小 db.mycollection.totalSize();

7、聚集集合储存空间大小 db.mycollection.storageSize();

8、Shard版本信息 db.mycollection.getShardVersion()

9、聚集集合重命名 db.mycollection.renameCollection("users"); 将userInfo重命名为users

10、删除当前聚集集合 db.mycollection.drop();



# MongoDB的使用：

### 创建&新增
- db.users.save({"name":"lecaf"})    *创建了名为users的集合，并新增了一条{"name":"lecaf"}的数据*
- db.users.insert({"name":"ghost", "age":10})    *在users集合中插入一条新数据，，如果没有users这个集合，mongodb会自动创建*
- save()和insert()也存在着些许区别：若新增的数据主键已经存在，insert()会不做操作并提示错误，而save() 则更改原来的内容为新内容。
	- 存在数据：{ _id : 1, " name " : " n1 "} ，_id是主键
- insert({ _id : 1, " name " : " n2 " })    *会提示错误*
- save({ _id : 1, " name " : " n2 " })*     会把 n1 改为  n2 ，有update的作用。*


### 删除
- db.users.remove()    *删除users集合下所有数据*
- db.users.remove({"name": "lecaf"})    *删除users集合下name=lecaf的数据*
- db.users.drop()或db.runCommand({"drop","users"})    *删除集合users*
- db.runCommand({"dropDatabase": 1})   * 删除当前数据库*

### 修改
- db.users.update({"name":"lecaf"}, {"age":10})    *修改name=lecaf的数据为age=10，第一个参数是查找条件，第二个参数是修改内容，除了主键，其他内容会被第二个参数的内容替换，主键不能修改，如图*
![](https://img-blog.csdnimg.cn/20190213233335409.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)


1、查询所有记录

db.mycollection.find();   相当于：select* from mycollection;

默认每页显示20条记录，当显示不下的情况下，可以用it迭代命令查询下一页数据。注意：键入it命令不能带"；"   但是你可以设置每页显示数据的大小，用DBQuery.shellBatchSize= 50;这样每页就显示50条记录了。



2、查询去掉后的当前聚集集合中的某列的重复数据

db.mycollection.distinct("name");   会过滤掉name中的相同数据，相当于：select distict name from mycollection;   

![](https://img-blog.csdnimg.cn/20190213222626563.png)



3、查询age = 22的记录

db.mycollection.find({"age": 22});    相当于： select * from mycollectionwhere age = 22;



4、查询age > 22的记录

db.mycollection.find({age: {$gt: 22}});   相当于：select * from mycollectionwhere age >22;



5、查询age < 22的记录

db.mycollection.find({age: {$lt: 22}});   相当于：select * from mycollectionwhere age <22;



6、查询age >= 25的记录

db.mycollection.find({age: {$gte: 25}});   相当于：select * from mycollectionwhere age >= 25;



7、查询age <= 25的记录

db.mycollection.find({age: {$lte: 25}});



8、查询age >= 23 并且 age <= 26

db.mycollection.find({age: {gte:23,lte: 26}});



9、查询name中包含 mongo的数据

db.mycollection.find({name: /mongo/});    相当于   select * from mycollectionwhere name like '%mongo%';



10、查询name中以mongo开头的

db.mycollection.find({name: /^mongo/});    类似 select * from mycollection where name like 'mongo%';



11、查询指定列name、age数据

db.mycollection.find({}, {name: 1, age: 1});   相当于：select name, age from mycollection;

name也可以用true或false,当用ture的情况下和 name:1效果一样，如果用false就是排除name，显示name以外的列信息。

![](https://img-blog.csdnimg.cn/20190213232559888.png)



12、查询指定列name、age数据, age > 25

db.mycollection.find({age: {$gt: 25}}, {name: 1, age: 1});   相当于：select name, age from mycollection where age >25;



13、按照年龄排序

升序：db.mycollection.find().sort({age: 1});

降序：db.mycollection.find().sort({age: -1});



14、查询name = zhangsan, age = 22的数据

db.mycollection.find({name: 'zhangsan', age: 22});    相当于：select * from mycollection where name = 'zhangsan' and age = '22';



15、查询前5条数据

db.mycollection.find().limit(5);     相当于：selecttop 5 * from mycollection;



16、查询10条以后的数据

db.mycollection.find().skip(10);     相当于：select * from mycollection where id not in (selecttop 10 * from userInfo);



17、查询在5-10之间的数据

db.mycollection.find().limit(10).skip(5);    可用于分页，limit是pageSize，skip是第几页*pageSize



18、or与 查询

db.mycollection.find({$or: [{age: 22}, {age: 25}]});     相当于：select * from userInfo where age = 22 or age = 25;



19、查询第一条数据

db.mycollection.findOne();     相当于：select top 1 * from mycollection; db.mycollection.find().limit(1);



20、查询某个结果集的记录条数

db.mycollection.find({age: {$gte: 25}}).count(); 相当于：select count(*) from userInfo where age >= 20;



21、按照某列进行排序

db.mycollection.find({sex: {$exists: true}}).count();     相当于：select count(sex) from userInfo;



### 索引

1、创建索引

db.mycollection.ensureIndex({name: 1});

db.mycollection.ensureIndex({name: 1, ts: -1});



2、查询当前聚集集合所有索引

db.mycollection.getIndexes();



3、查看总索引记录大小

db.mycollection.totalIndexSize();



4、读取当前集合的所有index信息

db.mycollection.reIndex();



5、删除指定索引

db.mycollection.dropIndex("name_1");



6、删除所有索引索引

db.mycollection.dropIndexes();



# Python中调用MongoDB

首先安装python环境： pip install pymongo

启动MongoDB服务，保证服务能在shell下正常访问

```python
from pymongo import MongoClient

client = MongoClient('192.168.0.113', 27017)
db = client.mydb  #连接mydb数据库，没有则自动创建
my_set = db.test_set　　#使用test_set集合，没有则自动创建

#上边两行可以替换为
#my_set = client["mydb"]["test_set"]

#插入数据（insert插入一个列表多条数据不用遍历，效率高， save需要遍历列表，一个个插入）
my_set.insert({"name":"zhangsan","age":18})
#或
my_set.save({"name":"zhangsan","age":18})

#添加多条数据到集合中
users=[{"name":"zhangsan","age":18},{"name":"lisi","age":20}]  
my_set.insert(users) 
#或
my_set.save(users) 

#查询数据
#查询全部
for i in my_set.find():
    print(i)
#查询name=zhangsan的
for i in my_set.find({"name":"zhangsan"}):
    print(i)
print(my_set.find_one({"name":"zhangsan"}))

#更新数据
my_set.update(
   <query>,    #查询条件
   <update>,    #update的对象和一些更新的操作符
   {
     upsert: <boolean>,    #如果不存在update的记录，是否插入
     multi: <boolean>,        #可选，mongodb 默认是false,只更新找到的第一条记录
     writeConcern: <document>    #可选，抛出异常的级别。
   }
)

#把上面插入的数据内的age改为20
my_set.update({"name":"zhangsan"},{'$set':{"age":20}})

#删除数据
my_set.remove(
   <query>,    #（可选）删除的文档的条件
   {
     justOne: <boolean>,    #（可选）如果设为 true 或 1，则只删除一个文档
     writeConcern: <document>    #（可选）抛出异常的级别
   }
)

#删除name=lisi的全部记录
my_set.remove({'name': 'zhangsan'})

#删除name=lisi的某个id的记录
id = my_set.find_one({"name":"zhangsan"})["_id"]
my_set.remove(id)

#删除集合里的所有记录
db.users.remove()　

#跳过两条数据后读取6条
for i in my_set.find().skip(2).limit(6):
    print(i)

#找出age是20、30、35的数据
for i in my_set.find({"age":{"$in":(20,30,35)}}):
    print(i)

#找出age是20或35的记录
for i in my_set.find({"$or":[{"age":20},{"age":35}]}):
    print(i)
```






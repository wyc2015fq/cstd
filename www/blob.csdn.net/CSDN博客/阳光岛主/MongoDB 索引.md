
# MongoDB 索引 - 阳光岛主 - CSDN博客

2014年01月18日 23:14:03[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：11081个人分类：[SQL Index																](https://blog.csdn.net/sunboy_2050/article/category/581634)



**1. ensureIndex添加索引**
ensureIndex 函数帮助文档
db.blog.ensureIndex(keypattern[,options])- options is an object with these possible fields: name, unique, dropDups
name：指定索引名称
unique：是否唯一索引
dropDups：是否删除重复
创建索引的缺点：每次插入、更新、删除时都会产生额外的开销，要尽可能少创建索引。每个集合默认的最大索引个数为64个。
如果没有对应的键，索引会将其作为null存储，所以，如果对某个建立了唯一索引，但插入了多个缺少该索引键的文档，则由于文档包含null值而导致插入失败。
**例子**
>db.users.find()
{ "_id" : ObjectId("4fc6d0c9387a7fee4eb6bfa9"), "name" : "aaa", "age" : 23, "sex" : "male" }
{ "_id" : ObjectId("4fc6d0e5387a7fee4eb6bfaa"), "name" : "bbb", "age" : 25, "sex" : "male" }
{ "_id" : ObjectId("4fc6d0f4387a7fee4eb6bfab"), "name" : "ccc", "age" : 25, "sex" : "male" }
{ "_id" : ObjectId("4fc6d100387a7fee4eb6bfac"), "name" : "ddd", "age" : 25, "sex" : "male" }
{ "_id" : ObjectId("4fc6d110387a7fee4eb6bfad"), "name" : "eee", "age" : 23, "sex" : "male" }
>db.users.ensureIndex({"name":1,"age":-1},{"name":"userIndex"})
//1，-1代表索引方向
//查找索引
>db.system.indexes.find()
{ "name" : "_id_", "ns" : "blog.users", "key" : { "_id" : 1 }, "v" : 0 }
{ "_id" : ObjectId("4fc6d1d0387a7fee4eb6bfb1"), "ns" : "blog.users", "key" : { "name" : 1, "age" : -1 }, "name" : "userIndex", "v" : 0 }
**2. explain()监控索引使用情况**
>db.users.find({"name":"qff","age":23}).explain()
{
"cursor" : "BtreeCursor userIndex",//没有使用索引时为BasicCursor,索引存储在B树结构中，所以使用查询时候会使用BtreeCursor类型的游标，userIndex为使用索引的名称
"nscanned" : 1,//查找了多少个文档
"nscannedObjects" : 1,
"n" : 1,//返回文档的数量
"millis" : 50,//数据库执行查询时间
"nYields" : 0,
"nChunkSkips" : 0,
"isMultiKey" : false,
"indexOnly" : false,
"indexBounds" : {
"name" : [
[
"qff",
"qff"
]
],
"age" : [
[
23,
23
]
]
}
}
**3. hint强制使用某个索引**
>db.users.find({"age":23,"name":/.*/}).hint({"name":1,"age":-1})
{ "_id" : ObjectId("4fc6d110387a7fee4eb6bfad"), "name" : "aaa", "age" : 23, "sex
" : "male" }
{ "_id" : ObjectId("4fc6d121387a7fee4eb6bfaf"), "name" : "bbb", "age" : 23, "se
x" : "male" }
{ "_id" : ObjectId("4fc6d11b387a7fee4eb6bfae"), "name" : "ccc", "age" : 23, "sex
" : "male" }
{ "_id" : ObjectId("4fc6d12b387a7fee4eb6bfb0"), "name" : "ddd", "age" : 23, "se
x" : "male" }
{ "_id" : ObjectId("4fc6d0c9387a7fee4eb6bfa9"), "name" : "eee", "age" : 23, "
sex" : "male" }
MongoDB的查询优化器非常智能，会替你选择该用哪个索引，多数情况下不需要指定的。


**参考推荐：**
[Create a Unique Index](http://docs.mongodb.org/manual/tutorial/create-a-unique-index/)（官方文档）
[MongoDB 索引](http://bigboy.iteye.com/blog/1545368)
[MongoDB基本管理命令](http://blog.csdn.net/shirdrn/article/details/7105539)
[MongoDB 常用命令](http://blog.csdn.net/ithomer/article/details/17111943)
[MongoDB 基本操作](http://blog.csdn.net/ithomer/article/details/17242225)




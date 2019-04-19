# mongodb_修改器($inc/$set/$unset/$push/$pop/upsert/save()) - 零度的博客专栏 - CSDN博客
2016年08月02日 11:00:36[零度anngle](https://me.csdn.net/zmx729618)阅读数：304
对于文档的更新除替换外，针对某个或多个文档只需要部分更新可使用原子的更新修改器，能够高效的进行文档更新。更新修改器是中特殊的键，
用来指定复杂的操作，比如增加、删除或者调整键，还可能是操作数组或者内嵌文档。
1.$inc
--------------------------------------------------------------------------
这个修改器干什么使的呢？看看下面示例的具体操作后的结果即可知道。
示例文档：{"uid":"201203","type":"1",size:10}
> db.b.insert({"uid":"201203","type":"1",size:10})
> db.b.find()
{ "_id" : ObjectId("5003b6135af21ff428dafbe6"), "uid" : "201203", "type" : "1",
"size" : 10 }
> db.b.update({"uid" : "201203"},{"$inc":{"size" : 1}})
> db.b.find()
{ "_id" : ObjectId("5003b6135af21ff428dafbe6"), "uid" : "201203", "type" : "1",
"size" : 11 }
> db.b.update({"uid" : "201203"},{"$inc":{"size" : 2}})
> db.b.find()
{ "_id" : ObjectId("5003b6135af21ff428dafbe6"), "uid" : "201203", "type" : "1",
"size" : 13 }
> db.b.update({"uid" : "201203"},{"$inc":{"size" : -1}})
> db.b.find()
{ "_id" : ObjectId("5003b6135af21ff428dafbe6"), "uid" : "201203", "type" : "1",
"size" : 12 }
得出结论：修改器$inc可以对文档的某个值为数字型（只能为满足要求的数字）的键进行增减的操作。
（这里有个问题：上篇中说到更新默认只对满足条件的记录集中第一个文档进行更新，那么使用$inc修改器之后，还是一样吗？）
2.$set
-------------------------------------------------------------------
用来指定一个键并更新键值，若键不存在并创建。来看看下面的效果：
> db.a.findOne({"uid" : "20120002","type" : "3"})
{ "_id" : ObjectId("500216de81b954b6161a7d8f"), "desc" : "hello world2!", "num"
: 40, "sname" : "jk", "type" : "3", "uid" : "20120002" }
--size键不存在的场合
> db.a.update({"uid" : "20120002","type" : "3"},{"$set":{"size":10}})
> db.a.findOne({"uid" : "20120002","type" : "3"})
{ "_id" : ObjectId("500216de81b954b6161a7d8f"), "desc" : "hello world2!", "num"
: 40, "size" : 10, "sname" : "jk", "type" : "3", "uid" : "20120002" }
--sname键存在的场合
> db.a.update({"uid" : "20120002","type" : "3"},{"$set":{"sname":"ssk"}})
> db.a.find()
{ "_id" : ObjectId("500216de81b954b6161a7d8f"), "desc" : "hello world2!", "num"
: 40, "size" : 10, "sname" : "ssk", "type" : "3", "uid" : "20120002" }
{ "_id" : ObjectId("50026affdeb4fa8d154f8572"), "desc" : "hello world1!", "num"
: 50, "sname" : "jk", "type" : "1", "uid" : "20120002" }
--可改变键的值类型
> db.a.update({"uid" : "20120002","type" : "3"},{"$set":{"sname":["java",".net","c++"]}})
> db.a.findOne({"uid" : "20120002","type" : "3"})
{
        "_id" : ObjectId("500216de81b954b6161a7d8f"),
        "desc" : "hello world2!",
        "num" : 40,
        "size" : 10,
        "sname" : [
                "java",
                ".net",
                "c++"
        ],
        "type" : "3",
        "uid" : "20120002"
}
对于内嵌的文档，$set又是如何进行更新的内嵌的文档的呢，请看下面的示例：
示例文档：{"name":"toyota","type":"suv","size":{"height":10,"width":5,"length":15}}
> db.c.findOne({"name":"toyota"})
{
        "_id" : ObjectId("5003be465af21ff428dafbe7"),
        "name" : "toyota",
        "type" : "suv",
        "size" : {
                "height" : 10,
                "width" : 5,
                "length" : 15
        }
}
> db.c.update({"name":"toyota"},{"$set":{"size.height":8}})
> db.c.findOne({"name":"toyota"})
{
        "_id" : ObjectId("5003be465af21ff428dafbe7"),
        "name" : "toyota",
        "type" : "suv",
        "size" : {
                "height" : 8,
                "width" : 5,
                "length" : 15
        }
}
> db.c.update({"name":"toyota"},{"$set":{"size.width":7}})
> db.c.findOne({"name":"toyota"})
{
        "_id" : ObjectId("5003be465af21ff428dafbe7"),
        "name" : "toyota",
        "type" : "suv",
        "size" : {
                "height" : 8,
                "width" : 7,
                "length" : 15
        }
}
可见：对于内嵌文档在使用$set更新时，使用"."连接的方式。
3.$unset
----------------------------------------------------------------
从字面就可以看出其意义，主要是用来删除键。
示例操作效果如下：
> db.a.update({"uid" : "20120002","type" : "3"},{"$unset":{"sname":1}})
> db.a.findOne({"uid" : "20120002","type" : "3"})
{
        "_id" : ObjectId("500216de81b954b6161a7d8f"),
        "desc" : "hello world2!",
        "num" : 40,
        "size" : 10,
        "type" : "3",
        "uid" : "20120002"
}
> db.a.update({"uid" : "20120002","type" : "3"},{"$unset":{"num":0}})
> db.a.findOne({"uid" : "20120002","type" : "3"})
{
        "_id" : ObjectId("500216de81b954b6161a7d8f"),
        "desc" : "hello world2!",
        "size" : 10,
        "type" : "3",
        "uid" : "20120002"
}
> db.a.update({"uid" : "20120002","type" : "3"},{"$unset":{"size":-1}})
> db.a.findOne({"uid" : "20120002","type" : "3"})
{
        "_id" : ObjectId("500216de81b954b6161a7d8f"),
        "desc" : "hello world2!",
        "type" : "3",
        "uid" : "20120002"
}
> db.a.update({"uid" : "20120002","type" : "3"},{"$unset":{"desc":"sssssss"}})
> db.a.findOne({"uid" : "20120002","type" : "3"})
{
        "_id" : ObjectId("500216de81b954b6161a7d8f"),
        "type" : "3",
        "uid" : "20120002"
}
得出结论：使用修改器$unset时，不论对目标键使用1、0、-1或者具体的字符串等都是可以删除该目标键。
4.数组修改器--$push
------------------------------------------------------------------
示例操作效果如下：
> db.c.find()
{ "_id" : ObjectId("5003be465af21ff428dafbe7"), "name" : "toyota", "type" : "suv",
"size" : { "height" : 8, "width" : 7, "length" : 15 } }
--先push一个当前文档中不存在的键title
> db.c.update({"name" : "toyota"},{$push:{"title":"t1"}})
> db.c.find()
{ "_id" : ObjectId("5003be465af21ff428dafbe7"), "name" : "toyota", "size" : { "height" : 8,
 "width" : 7, "length" : 15 }, "title" : [ "t1" ], "type" : "suv" }
--再向title中push一个值
> db.c.update({"name" : "toyota"},{$push:{"title":"t2"}})
> db.c.find()
{ "_id" : ObjectId("5003be465af21ff428dafbe7"), "name" : "toyota", "size" : { "height" : 8,
 "width" : 7, "length" : 15 }, "title" : [ "t1", "t2" ], "type" : "suv" }
--再向title中push一个值
> db.c.update({"name" : "toyota"},{$push:{"title":"t2"}})
> db.c.find()
{ "_id" : ObjectId("5003be465af21ff428dafbe7"), "name" : "toyota", "size" : { "height" : 8,
 "width" : 7, "length" : 15 }, "title" : [ "t1", "t2", "t2" ], "type" : "suv" }
--再向一个已经存在的键值非数组类型的键push一个值
> db.c.update({"name" : "toyota"},{$push:{"size.height":10}})
Cannot apply $push/$pushAll modifier to non-array
> db.c.update({"name" : "toyota"},{$push:{"name":"ddddddd"}})
Cannot apply $push/$pushAll modifier to non-array
得出结论：$push--向文档的某个数组类型的键添加一个数组元素，不过滤重复的数据。添加时键存在，要求键值类型必须是数组；键不存在，则创建数组类型的键。
5.数组修改器--$ne/$addToSet
---------------------------------------------------------------------
主要给数组类型键值添加一个元素时，避免在数组中产生重复数据，$ne在有些情况是不通行的。
> db.c.update({"title" : {$ne:"t2"}},{$push:{"title":"t2"}})
> db.c.find()
{ "_id" : ObjectId("5003be465af21ff428dafbe7"), "name" : "toyota", "size" : { "height" : 8,
 "width" : 7, "length" : 15 }, "title" : [ "t1", "t2", "t2" ], "type" : "suv" }
> db.c.update({"name" : "toyota"},{$addToSet:{"title":"t2"}})
> db.c.find()
{ "_id" : ObjectId("5003be465af21ff428dafbe7"), "name" : "toyota", "size" : { "height" : 8,
 "width" : 7, "length" : 15 }, "title" : [ "t1", "t2", "t2" ], "type" : "suv" }
6.数组修改器--$pop、$pull
------------------------------------------------------------
$pop从数组的头或者尾删除数组中的元素，示例如下：
{ "_id" : ObjectId("5003be465af21ff428dafbe7"), "name" : "toyota", "size" : { "height" : 8,
 "width" : 7, "length" : 15 }, "title" : [ "t1", "t2", "t3", "t4" ],"type" : "suv" }
--从数组的尾部删除 1
> db.c.update({"name" : "toyota"},{$pop:{"title":1}})
> db.c.find()
{ "_id" : ObjectId("5003be465af21ff428dafbe7"), "name" : "toyota", "size" : { "height" : 8,
 "width" : 7, "length" : 15 }, "title" : [ "t1", "t2", "t3" ], "type" : "suv" }
--从数组的头部 -1
> db.c.update({"name" : "toyota"},{$pop:{"title":-1}})
> db.c.find()
{ "_id" : ObjectId("5003be465af21ff428dafbe7"), "name" : "toyota", "size" : { "height" : 8,
 "width" : 7, "length" : 15 }, "title" : [ "t2", "t3" ], "type" : "suv" }
--从数组的尾部删除 0
> db.c.update({"name" : "toyota"},{$pop:{"title":0}})
> db.c.find()
{ "_id" : ObjectId("5003be465af21ff428dafbe7"), "name" : "toyota", "size" : { "height" : 8,
 "width" : 7, "length" : 15 }, "title" : [ "t2" ], "type" : "suv" }
$pull从数组中删除满足条件的元素，示例如下：
{ "_id" : ObjectId("5003be465af21ff428dafbe7"), "name" : "toyota", "size" : { "height" : 8,
 "width" : 7, "length" : 15 }, "title" : [ "t1", "t2", "t2", "t3" ],"type" : "suv" }
> db.c.update({"name" : "toyota"},{$pull:{"title":"t2"}})
> db.c.find()
{ "_id" : ObjectId("5003be465af21ff428dafbe7"), "name" : "toyota", "size" : { "height" : 8,
 "width" : 7, "length" : 15 }, "title" : [ "t1", "t3" ], "type" : "suv" }
7.数组的定位修改器
-------------------------------------------------------------------
在需要对数组中的值进行操作的时候，可通过位置或者定位操作符（"$"）.数组是0开始的，可以直接将下标作为键来选择元素。
示例如下：
{"uid":"001",comments:[{"name":"t1","size":10},{"name":"t2","size":12}]}
> db.c.find({"uid":"001"})
{ "_id" : ObjectId("5003da405af21ff428dafbe8"), "uid" : "001", "comments" : [ {
"name" : "t1", "size" : 10 }, { "name" : "t2", "size" : 12 } ] }
> db.c.update({"uid":"001"},{$inc:{"comments.0.size":1}})
> db.c.find({"uid":"001"})
{ "_id" : ObjectId("5003da405af21ff428dafbe8"), "uid" : "001", "comments" : [ {
"name" : "t1", "size" : 11 }, { "name" : "t2", "size" : 12 } ] }
> db.c.update({"comments.name":"t1"},{$set:{"comments.$.size":1}})
> db.c.find({"uid":"001"})
{ "_id" : ObjectId("5003da405af21ff428dafbe8"), "uid" : "001", "comments" : [ {
"name" : "t1", "size" : 1 }, { "name" : "t2", "size" : 12 } ] }
--若为多个文档满足条件，则只更新第一个文档。
8.upsert
-----------------------------------------------------------------
upsert是一种特殊的更新。当没有符合条件的文档，就以这个条件和更新文档为基础创建一个新的文档，如果找到匹配的文档就正常的更新。
使用upsert，既可以避免竞态问题，也可以减少代码量（update的第三个参数就表示这个upsert，参数为true时）
> db.c.remove()
> db.c.update({"size":11},{$inc:{"size":3}})
> db.c.find()
> db.c.update({"size":11},{$inc:{"size":3}},false)
> db.c.find()
> db.c.update({"size":11},{$inc:{"size":3}},true)
> db.c.find()
{ "_id" : ObjectId("5003ded6c28f67507a6df1de"), "size" : 14 }
9.save函数
-----------------------------------------------------------------
1.可以在文档不存在的时候插入，存在的时候更新，只有一个参数文档。
2.要是文档含有"_id"，会调用upsert。否则，会调用插入。
> db.a.find()
{ "_id" : ObjectId("50026affdeb4fa8d154f8572"), "desc" : "hello world1!", "num": 50,
 "sname" : "jk", "type" : "1", "uid" : "20120002" }
> var o = db.a.findOne()
> o.num = 55
55
> db.a.save(o)
> db.a.find()
{ "_id" : ObjectId("50026affdeb4fa8d154f8572"), "desc" : "hello world1!", "num": 55,
 "sname" : "jk", "type" : "1", "uid" : "20120002" }

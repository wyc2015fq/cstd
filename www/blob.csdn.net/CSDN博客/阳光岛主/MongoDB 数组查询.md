
# MongoDB 数组查询 - 阳光岛主 - CSDN博客

2013年12月12日 00:05:53[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：10288


[root@sunrise16 bin]\#./mongo
MongoDB shell version: 2.0.0
connecting to: test

插入一个带有数组元素的文档
>db.food.insert({"fruit": ["apple", "banana", "peach"]})
>db.food.find()
{ "_id" : ObjectId("4ea6a4ef0b12b1d429b4057f"), "fruit" : [ "apple", "banana", "peach" ] }

查询数组元素中包含某个值的文档
>db.food.find({"fruit": "banana"})
{ "_id" : ObjectId("4ea6a4ef0b12b1d429b4057f"), "fruit" : [ "apple", "banana", "peach" ] }
>db.food.find({"fruit": {"$all": ["banana"]}})
{ "_id" : ObjectId("4ea6a4ef0b12b1d429b4057f"), "fruit" : [ "apple", "banana", "peach" ] }

查询匹配指定数组的文档。注意要完全相同，数量和顺序都不能有错，否则找不到
>db.food.find({"fruit": ["banana"]})
>db.food.find({"fruit": ["apple", "banana", "peach"]})
{ "_id" : ObjectId("4ea6a4ef0b12b1d429b4057f"), "fruit" : [ "apple", "banana", "peach" ] }
>db.food.find({"fruit": ["banana", "apple", "peach"]})

查询数组元素中包含多个指定值的文档，全部包含，顺序无关
>db.food.find({"fruit": {"$all": ["banana", "apple", "peach"]}})
{ "_id" : ObjectId("4ea6a4ef0b12b1d429b4057f"), "fruit" : [ "apple", "banana", "peach" ] }

查询包含多个值之一的文档
>db.food.find({"fruit":["banna","cherry"]})
>db.food.find({"fruit":{"$all":["banna","cherry"]}})
>db.food.find({"fruit":{"$in":["banna","cherry"]}})

查询指定长度的数组。但不能查询指定长度范围的数组
>db.food.find({"fruit": {"$size": 3}})
{ "_id" : ObjectId("4ea6a4ef0b12b1d429b4057f"), "fruit" : [ "apple", "banana", "peach" ] }
>db.food.find({"fruit": {"$size": 2}})
>db.food.find({"fruit": {"$size": 4}})

**模糊查询：**
db.collname.find({"actors.name":/Catterfeld/i}, {"tag":1,"_id":1,"actors":1})


**参考推荐：**
[MongoDB查询(数组、内嵌文档和$where)](http://blog.csdn.net/drifterj/article/details/7833883)
[mongodb 常用命令](http://blog.csdn.net/ithomer/article/details/17111943)
[MongoDB 查询上](http://www.cnblogs.com/refactor/archive/2012/07/30/2591344.html)




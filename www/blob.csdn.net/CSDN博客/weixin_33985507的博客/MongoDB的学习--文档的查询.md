# MongoDB的学习--文档的查询 - weixin_33985507的博客 - CSDN博客
2014年01月20日 18:05:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
继续关于《MongoDB权威指南》记录，今天的内容是文档的查询~~
MongoDB官网地址：[http://www.mongodb.org/](http://www.mongodb.org/)
我使用的是MongoDB 2.4.8
## find函数
在上一篇博客中已经使用过此函数，都是最基本的使用，如
```
db.user.find()
```
它会返回集合user中的所有内容。
也可以在find中添加一个参数，如{"age":27}，这样就会返回年龄为27的user的集合。
若是参数为{"name":"jun","age":27}，就会返回name为jun年龄为27的user的集合。
有时候并不需要返回所有的键值，可以在find中设置第二个参数，如下
```
db.user.find({}, {"name": 1, "score": 1})
```
详细如下图
![](https://images0.cnblogs.com/blog/587057/201401/171720168619.png)
从上图可以看到_id这个键总是被返回，即使没有指定也一样。
如果将第二个参数中的1，改为0，会产生什么结果呢？
![](https://images0.cnblogs.com/blog/587057/201401/171723046420.png)
没错，就如上图，返回值中的没有了name和score，这两个键被去掉了。
这个方法可以用来防止返回_id，只需建第二个参数设为{"_id": 0}。
## 查询条件
查询不仅仅能够像之前那样精确匹配，还能够匹配更加复杂的条件，如范围、OR子句和取反。
### 1.比较操作符
"$lt"、"$lte"、"$gt"、"$gte"和"$ne"就是全部的比较操作符了，他们分别对应<、<=、>、>=和<>(不等于)。可以将其组合起来查找一个范围的值。如下
```
db.user.find({"age": {"$gte": 18, "$lte":30}})
```
这样就可以查找到年龄在18到30岁之间的user的集合了。
这样的查询对日期特别有用。如下
```
start = new Date("01/01/2009");
db.user.find({"createTime": {"$gt": start}})
```
这样就会查找到2009年1月1日以后注册的用户，其中createTime键为用户注册日期。
### 2.OR查询
MongoDB有两种OR查询。"$in"和"$nin"为一种，可以查询一个键的多个值。"$or"和"$nor"是另一种，可以组合完成多个键值的任意个定值。
先来造以下数据，命令如下
```
for(var i=0;i<5;i++){
    db.user.insert({"name": "jun"+i, "email":"jun"+i+"@sina.com", "score": 15+i})
}
```
使用"$in"，命令如下
```
db.user.find({name: {$in:["jun1", "jun2"]}})
```
结果如下图
![](https://images0.cnblogs.com/blog/587057/201401/171842005957.png)
查找出name为jun1和jun2的user集合，若使用"$nin"，则结果为name不为jun1和jun2的集合。
使用"$or"，命令如下
```
db.user.find({$or: [{name: {$in: ["jun1", "jun2"]}},
　　　　　　　　　　   {email: "jun@126.com"}]})
```
结果如下图
![](https://images0.cnblogs.com/blog/587057/201401/201525362815.png)
查找出name为jun1和jun2或者email为jun@126.com的user集合，若使用"$nor"，则结果为name不为jun1和jun2且email不为jun@126.com的集合""。
其实"$nin"和"$nor"分别是"$in"和"$or"的取反。
### 3.$not
"$not"是元条件句，即可以用在任何其他条件之上。
如果命令为
```
db.user.find({score: {$mod: [5, 1]}})
```
结果返回score除5取余为1的user集合。本来想贴图的，上传挂了。。。就这么说吧。。。
若想返回score除5取余不为1的user集合，可用如下命令
```
db.user.find({score: {$not: {$mod: [5, 1]}}});
db.user.find({score: {$not: {$eq: 5}}});
```
### **4. **$type
$type操作符是基于BSON类型来检索集合中匹配的结果。
MongoDB中可以使用的类型：
|类型描述|类型值|
|----|----|
|Double|1|
|String|2|
|Object|3|
|Array|4|
|Binary data|5|
|Object id|7|
|Boolean|8|
|Date|9|
|Null|10|
|Regular expression|11|
|JavaScript code|13|
|Symbol|14|
|JavaScript code with scope|15|
|32-bit integer|16|
|Timestamp|17|
|64-bit integer|18|
|Min key|255|
|Max key|127|
如果命令为
```
db.user.find({"extra" : {$type : 3}})
```
结果返回user中extra属性为BSON对象的集合。
**注:**
**    条件句是内层文档的键，而修改器则是外层文档的键。**
**    一个键可以有多个条件，但是一个键不能对应多个更新修改器。**
## 特定类型的查询
### 1.null
null不仅仅能匹配自身，而且能匹配"不存在的"。
如果仅仅想要匹配键值为null的文档，既要检查该键的值是否为null，还要通过"$exists"条件判定键值已经存在，命令如下
```
db.user.find({email: {$in: [null], $exists: true}})
```
返回用户中email为null的user的集合。
这里没有$eq操作符，所以使用只有一个元素的$in，其效果是一样的。
### 2.正则表达式
MongoDB使用Perl兼容的正则表达式(PCRE)库来匹配正则表达式，PCRE支持的正则表达式语法都能被MongoDB所接受。
如果你对正则表达式不了解，可以查看我之前的日志：[JavaScript的学习--正则表达式](http://www.cnblogs.com/CraryPrimitiveMan/p/3504209.html)
MongoDB可以为前缀正则表达式(比如/^joey/)查询创建索引，所以这种类型的查询效率非常高。
正则表达式也可以匹配自己。如果有人直接将正则表达式插入数据库，正则表达式也是可以匹配的。
命令如下
```
db.user.insert({name: /sgj/})
db.user.find({name: /sgj/})
```
返回结果是(图片不能上传了，就不截图，直接写结果了)
```
{ "_id" : ObjectId("52dce1e664d83577540f5aa9"), "name" : /sgj/ }
```
### 3.查询数组
移除掉user中的数据，重新插入几条
```
db.user.remove()
db.user.insert({name:"jun", loveFruit: ["apple", "banana", "peach"]})
db.user.insert({name:"jun1", loveFruit: ["apple", "kumquat", "orange"]})
db.user.insert({name:"jun2", loveFruit: ["apple", "banana", "cherry"]})
```
普通的数组查找
```
db.user.find({loveFruit: "cherry"})
```
返回结果为
```
{ "_id" : ObjectId("52dce40364d83577540f5aac"), "name" : "jun2", "loveFruit" : [ "apple",  "banana",  "cherry" ] }
```
#### 1).$all
如果需要通过多个元素来匹配数组，就需要使用"$all"。
命令如下
```
db.user.find({loveFruit: {$all: ["apple", "banana"]}})
```
返回结果为
```
{ "_id" : ObjectId("52dce40164d83577540f5aaa"), "name" : "jun", "loveFruit" : [ "apple",  "banana",  "peach" ] }
{ "_id" : ObjectId("52dce40364d83577540f5aac"), "name" : "jun2", "loveFruit" : [ "apple",  "banana",  "cherry" ] }
```
$all意味着其后的数组中的元素，都要被包含，才符合条件。
也可查询数组指定位置的元素，可以使用key.index语法指定下标。
```
db.user.find({"loveFruit.2": "cherry"})
```
返回结果为
```
{ "_id" : ObjectId("52dce40364d83577540f5aac"), "name" : "jun2", "loveFruit" : [ "apple",  "banana",  "cherry" ] }
```
数组的下标都是从0开始的。
其中的 loveFruit.2必须用双引号括起来，否则会报错。
#### 2).$size
$size可以查询指定长度的数组。
命令如下
```
db.user.find({loveFruit: {$size: 3}})
```
查找loveFruit的数组长度为3的user集合。
这种查询不能和其他查询子句组合(如"$gt")。
#### 3).$slice
find的第二个参数是可选的，可以指定返回哪些键，$slice返回数组的一个子集合。
如返回第一个user的前两个loveFriut元素
```
db.user.findOne({}, {loveFriut: {$slice: 2}})
```
返回第一个user的后两个loveFriut元素
```
db.user.find({}, {loveFriut: {$slice: -2}})
```
返回第一个user的第24~33个loveFriut元素
```
db.user.find({}, {loveFriut: {$slice: [23, 10]}})
```
### 查询内嵌文档
有两种方法，查询整个文档，或只针对其键/值查询。
如果有以下文档
```
{
    "name": {"first": "harry", "last": "sun"},
    "age": 25  
}
```
查询整个文档的方法
```
db.user.find({name: {"first": "harry", "last": "sun"}})
```
如果harry添加一个中间名的键，这个查询就不起作用了，如果顺序不对，first在后，last在前，也不会起作用。
针对其键/值查询的方法
```
db.user.find({"name.first": "harry", "name.last": "sun"})
```
$elemMatch可以限定条件分组，仅当需要对一个内嵌文档的多个键操作时才会用到。
```
db.blog.find({"comments": {"$elemMatch": {"author": "joe", 
　　　　　　　　　　　　　　　　　　　　　　　　 "score": {"$gte": 5}}}})
```
返回由joe发表的5分以上的评论。
## $where查询
如果不是非常必要，一定要避免使用$where查询，$where查询比常规查询慢很多。
使用方法有如下两种
```
db.user.find({"$where": "this.x + this.y == 10"})
db.user.find({"$where": "function() {this.x + this.y == 10;}"})
```
## 游标
游标通常能对最终结果进行有效地控制，可以限制结果的数量，略过部分结果，根据任意方向键的组合杜结果进行各种排序。
```
db.user.find({"name": "jun"}).limit(50).skip(50).sort({"score": -1})
```
返回name为jun，按score逆序(sort)，忽略前50条(skip)，限制取50条(limit)的user的集合。
尽量避免使用skip略过大量结果，否则会有性能问题。
不同类型键值排序的顺序(从小到大)
(1)最小值 (2)null (3)数字(整型、长整型、双精度) (4)字符串 (5)对象/文档 (6)数组 (7)二进制数据 (8)对象ID (9)布尔型 (10)日期类 (11)时间戳 (12)正则表达式 (13)最大值
今天就先到这里~~~

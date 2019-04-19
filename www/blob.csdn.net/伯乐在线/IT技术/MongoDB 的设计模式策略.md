# MongoDB 的设计模式策略 - 文章 - 伯乐在线
原文出处： [thetechnick](http://thetechnick.blogspot.sg/2016/06/mongodb-design-patterns.html)   译文出处：[CSDN-仲培艺](http://www.iteye.com/news/31667)
MongoDB是一项通用工具，但它也并非完美。针对某些MongoDB不适用的场合，有时可选用设计模式来加以应对。
MongoDB是一个NoSQL文档数据库，在大多数情况下是一个相对理想的选择，即使是在其不适用的情况下，也仍然可以依靠下面所列举的这些设计模式来克服其局限性。
本文将针对我的另一篇文章[《MongoDB的好坏恶》（ MongoDB : The Good, The Bad, and the Ugly）](http://thetechnick.blogspot.sg/2016/06/mongodb-good-bad-and-ugly.html)中所提及的一些局限性，提供一个相对应的解决方案。
**1. 查询命令分离模式**
![1](http://jbcdn2.b0.upaiyun.com/2016/07/28c8edde3d61a0411511d3b1866f0636.png)
在副本集中职责被分离到不同的节点。最基本的第一类节点可能也同时占据着首要地位，它只需要储存那些写入和更新所需的数据。而查询工作则交由第二类节点来执行。这一模式将提升首要节点服务器的写吞吐量，因为当写入一组对象时，需要更新及插入的数据量也随之减少，除此之外，二类节点也得益于较少的待更新数据和其自身所具有的为其工作量而优化的内存工作集。
**2. 应用程序级事务模式**
MongoDB不支持事务和文件内部锁定。然而，依据应用逻辑，应当保留queue用法。

Java
```
db.queue.insert( { _id : 123,  
    message : { },  
    locked : false,  
    tlocked : ISODate(),  
    try : 0 });  
var timerange = date.Now() - TIMECONSTANT;  
var doc = db.queue.findAndModify( { $or : [ { locked : false }, { locked : true, tlocked : {
$lt : timerange } } ], { $set : { locked : true, tlocked : date.Now(), $inc : { try : 1 } } }  
);  
//do some processing  
db.queue.update( { _id : 123, try : doc.try }, { } );
```
**3. Bucketing模式**
当文本含有一个不断增长的数组时，则使用Bucketing模式，例如指令。而指令线可能会扩展到超过文档大小的合理值。该模式经由编程方式处理，并通过公差计算触发。

Java
```
var TOLERANCE = 100;  
    for( recipient in msg.to) {  
        db.inbox.update( {  
            owner: msg.to[recipient], count: { $lt : TOLERANCE }, time : { $lt : Date.now() } },  
{ $setOnInsert : { owner: msg.to[recipient], time : Date.now() },  
{ $push: { "messages": msg }, $inc : { count : 1 } },  
{ upsert: true } );
```
**4. 关系模式**
有时，会有不能插入整个文档的情况，例如人体建模时，我们就可以使用该模式来建立关系。
- 确定数据是否属于该文档，即二者间是否有关系。
- 如果可能的话，特别是面对有用的独有（专属）数据时，插入文档。
- 尽可能不参考id值。
- 对关系中的有用部分进行反规范化处理。好的候选不会经常甚至从不更改值，并且颇为有用。
- 关注反规范数据的更新和关系修复。

Java
```
{  
     _id : 1,  
    name : ‘Sam Smith’,  
    bio : ‘Sam Smith is a nice guy’,  
    best_friend : { id : 2, name : ‘Mary Reynolds’ },  
    hobbies : [ { id : 100, n :’Computers’ }, { id : 101, n : ‘Music’ } ]  
}  
{  
    _id : 2,  
    name : ‘Mary Reynolds’  
    bio : ‘Mary has composed documents in MongoDB’,  
    best_friend : { id : 1, name : ‘Sam Smith’ },  
    hobbies : [ { id : 101, n : ‘Music’ } ]  
}
```
**5. 物化路径模式**
![2](http://jbcdn2.b0.upaiyun.com/2016/07/665f644e43731ff9db3d341da5c827e1.png)
在一个数据模型的树模式中，同一对象类型是该对象的子对象，这种情况下可以使用物化路径模型来以获取更高效的检索、查询。示例如下：

Java
```
{ _id: "Books", path: null }  
{ _id: "Programming", path: ",Books," }  
{ _id: "Databases", path: ",Books,Programming," }  
{ _id: "Languages", path: ",Books,Programming," }  
{ _id: "MongoDB", path: ",Books,Programming,Databases," }  
{ _id: "dbm", path: ",Books,Programming,Databases," }
```
按字段路径查询树模式：

Java
```
db.collection.find().sort( { path: 1 } )
```
使用路径字段的常规表达来找出Programming的后代集：

Java
```
db.collection.find( { path: /,Programming,/ } )
```
在Books是top parent的情况下查询Books的后代集：

Java
```
db.collection.find( { path: /^,Books,/ } )
```

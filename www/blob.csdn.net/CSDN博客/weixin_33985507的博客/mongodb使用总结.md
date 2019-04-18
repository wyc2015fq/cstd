# mongodb使用总结 - weixin_33985507的博客 - CSDN博客
2018年11月11日 16:17:15[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
![图片描述](https://image-static.segmentfault.com/293/128/2931283665-5aa7c32c7a173_articlex)
MongoDB 是一个基于分布式文件存储的数据库。由 C++ 语言编写。旨在为 WEB 应用提供可扩展的高性能数据存储解决方案。
MongoDB 是一个介于关系数据库和非关系数据库之间的产品，是非关系数据库当中功能最丰富，最像关系数据库的。是世界上最大的nosql(not only sql)数据库。执行mongodb数据库需要mongod环境和mongo环境。
# 数据库
## 传统数据库技术回顾
> 概念：存储数据的仓库我们称为数据库。数据库分为非关系型数据库和关系型数据库。关系型数据库（Oracle,mysql,db2,...）往往以表结构的形式进行存储。存储数据用txt就行了，为什么要有数据库?
- `理由1：数据库有行、列的概念，数据有关系，数据不是散的`
老牌数据库，比如MySQL、SQLServer、Oracle、Access。这些数据库，我们管他们叫结构型数据库。为什么？因为每个表中，都有明确的字段、每行记录，都有这些字段。不能有的行有，有的行没有。
![图片描述](https://image-static.segmentfault.com/142/830/1428309088-5c03eadfe453c_articlex)
- `理由2：数据库能够提供非常方便的接口，让增删改查变得简单`
我们的老牌数据库，都无一例外的使用SQL语言，管理数据库。
SQL就是structure query language
比如，查询所有女生：
`SELECT * FROM users WHERE sex = '女';`
再比如，查询所有女生，并且年龄20~24之间，并且在北京：
`SELECT * FROM users WHERE sex = '女' AND age < 24 AND age >= 20 AND address = '北京'`- `理由3：数据库不能自己玩儿，要给向PHP、.net、jsp等语言提供接口`
用PHP这些语言，能够向数据库中增删改查。
老牌数据库，都是结构型数据库，现在出了什么问题？
比如，我们现在想往一个已经有1000条数据的数据库中增加一个字段‘是否为本地常住居民’
![图片描述](https://image-static.segmentfault.com/238/571/2385712962-5c03ed1013fec_articlex)
之前已经存在的数据，实际上不需要增加这个字段，因为这些用户已经填写完毕表单了。不需要完善个人信息了。我们的想法是在今后注册的用户，需要添加‘是否为本地常住居民’信息。但是，我们刚才说了，所谓的字段，是表的一个结构。所有的行都必须拥有，不能有的行有这个字段，有的行没有这个字段。
可想而知，大数据时代，数据库中有100万条数据都算少的。我们如果要动字段，太耗费时间。
非结构型数据库NoSQL应运而生。
NoSQL是个怪胎，无法挑战老牌数据库，但是在大数据时代有自己的意义。
## NoSQL
非结构型数据库。没有行、列的概念。用JSON来存储数据。
集合就相当于‘表’，文档就相当于‘行’
![图片描述](https://image-static.segmentfault.com/159/673/1596731400-5c03ef231ccf0_articlex)
文档就是JSON
![图片描述](https://image-static.segmentfault.com/343/530/3435307681-5c03ef5a466cd_articlex)
> 因此，我们总结NOSQL数据库在以下的这几种情况下比较适用：1.数据模型比较简单；2.需要灵活性更强的系统；3.对数据库性能要求较高；4.不需要高度的数据一致性；5.对于给定key，比较容易映射复杂值的环境。
NoSQL不是银弹，没有资格挑战老牌数据库，但是在某些特定情况下，还是适合的。
## mongodb和mysql的区别
- 前者非关系型数据库，后者是关系型数据库
- mongodb中是以集合的形式来充当mysql中的表结构
- mongodb中的数据是以文档的形式进行存储
![图片描述](https://image-static.segmentfault.com/124/777/1247775118-5be82b50d8434_articlex)
# mongodb的特点
## mongodb的优点
- 面向文档存储的数据库（BSON的数据格式）
![图片描述](https://image-static.segmentfault.com/503/567/50356763-5be7cadf25a2b_articlex)
- 有丰富的查询指令
- 支持索引
- 具有分片系统
- 无模式
## mongodb的缺点
- 占用的空间比较大
- 不支持事务
- 对于windows来说，它不支持32位的系统
# mongodb常用指令
- `show dbs` 查看当前所有数据库
![图片描述](https://image-static.segmentfault.com/230/072/230072389-5be7cec581a60_articlex)
- `use database_name` 创建数据库
- `db` 查询当前使用的数据库
- `db.stats()` 查询当前使用的数据库信息
- `db.dropDatabase()` 删除当前数据库
![图片描述](https://image-static.segmentfault.com/205/262/2052629226-5be7edf83b3fd_articlex)
- `db.help()` 获取查询帮助
- `db.database_name.help()` 获取指定数据库查询帮助
- `db.collection_name.find()` 查询集合的信息
- `db.createCollection(coll_name,options)` 创建集合
- `db.getCollectionNames()` 查询所有集合
- `db.getCollection(coll_name)` 查询某一个特定集合
- `db.coll_name.drop()` 对集合的删除
- `db.printCollectionStats()` 打印当前数据库中所有集合的状态
- `db.coll_name.insert/insertMany/save/insertOne` 添加一条/多条数据
- `db.coll_name.update(query,info,con,muti)` 修改数据（query: 查询的条件;info: 要更新的信息;con: 给异步操作提供扩展;muti: 返回布尔类型 默认false）（这里涉及到几个特殊属性`$inc`和`$set` 前者为相加后者为设置）
- `db.coll_name.remove(query)` 删除数据(query 删除的条件)
![图片描述](https://image-static.segmentfault.com/169/660/1696601716-5be82b050c6cb_articlex)
- 对数据的查询
- `db.coll_name.find()` 查询所有信息
- `db.coll_name.find({"age": 18})` 查询某一条信息
- `db.coll_name.find({age: {$gt: 22}})` gt大于某一条件
- `db.coll_name.find({age: {$lt: 22}})` lt小于某一条件
- `db.coll_name.find({age: {$gte: 22}})` gt大于等于某一条件
- `db.coll_name.find({age: {$lte: 22}})` lte小于等于某一条件
- `db.coll_name`.find({title: /好/}) 模糊查询
- 集合与集合之间的关联
- `async.waterfall([task],callback)` (task:代表执行的函数组,下一个函数总会继承上一个函数的返回值;callback:回调函数)
- `coll_name.findAndModify(data,sort,update,callback)` (data:查询的条件;sort:对返回的查询结果进行排序;update:对查询的结果进行更新;callback:回调函数)
## mongodb术语概念
|sql术语/概念|mongodb术语/概念|解释/说明|
|----|----|----|
|database|database|数据库|
|table|collection|数据库表/集合|
|row|document|数据记录行/文档|
|column|field|数据字段/域|
|index|index|索引|
|table joins||表连接，mongodb不支持|
|primary key|primary key|主键，mongodb自动将_id字段设置为主键|
# mongodb的封装(DAO)
> 把常用的增删改查，都封装成为module。
   开发DAO：J2EE开发人员使用数据访问对象（DAO）设计模式把底层的数据访问逻辑和高层的商务逻辑分开，实现DAO模式能够更加专注于编写数据访问代码。
   使用我们自己的DAO模块，来实现数据库插入。代码变得简单。
```
//这个模块里面封装了所有对数据库的常用操作
var MongoClient = require('mongodb').MongoClient;
var settings = require("../settings.js");
//不管数据库什么操作，都是先连接数据库，所以我们可以把连接数据库
//封装成为内部函数
function _connectDB(callback) {
    var url = settings.dburl;   //从settings文件中，都数据库地址
    //连接数据库
    MongoClient.connect(url, function (err, db) {
        if (err) {
            callback(err, null);
            return;
        }
        callback(err, db);
    });
}
//插入数据
exports.insertOne = function (collectionName, json, callback) {
    _connectDB(function (err, db) {
        db.collection(collectionName).insertOne(json, function (err, result) {
            callback(err, result);
            db.close(); //关闭数据库
        })
    })
};
//查找数据，找到所有数据。args是个对象{"pageamount":10,"page":10}
exports.find = function (collectionName, json, C, D) {
    var result = [];    //结果数组
    if (arguments.length == 3) {
        //那么参数C就是callback，参数D没有传。
        var callback = C;
        var skipnumber = 0;
        //数目限制
        var limit = 0;
    } else if (arguments.length == 4) {
        var callback = D;
        var args = C;
        //应该省略的条数
        var skipnumber = args.pageamount * args.page || 0;
        //数目限制
        var limit = args.pageamount || 0;
        //排序方式
        var sort = args.sort || {};
    } else {
        throw new Error("find函数的参数个数，必须是3个，或者4个。");
        return;
    }
    //连接数据库，连接之后查找所有
    _connectDB(function (err, db) {
        var cursor = db.collection(collectionName).find(json).skip(skipnumber).limit(limit).sort(sort);
        cursor.each(function (err, doc) {
            if (err) {
                callback(err, null);
                db.close(); //关闭数据库
                return;
            }
            if (doc != null) {
                result.push(doc);   //放入结果数组
            } else {
                //遍历结束，没有更多的文档了
                callback(null, result);
                db.close(); //关闭数据库
            }
        });
    });
}
//删除
exports.deleteMany = function (collectionName, json, callback) {
    _connectDB(function (err, db) {
        //删除
        db.collection(collectionName).deleteMany(
            json,
            function (err, results) {
                callback(err, results);
                db.close(); //关闭数据库
            }
        );
    });
}
//修改
exports.updateMany = function (collectionName, json1, json2, callback) {
    _connectDB(function (err, db) {
        db.collection(collectionName).updateMany(
            json1,
            json2,
            function (err, results) {
                callback(err, results);
                db.close();
            });
    })
}
exports.getAllCount = function (collectionName,callback) {
    _connectDB(function (err, db) {
        db.collection(collectionName).count({}).then(function(count) {
            callback(count);
            db.close();
        });
    })
}
```
# 项目中使用mongodb
> 切换到指定项目 npm init生成package.json
npm install mongodb -g 全局安装
npm install mongodb --save-dev 局部安装
mongodb.js
```
var Mongodb = require("mongodb")
// 连接到mongodb的服务端口
var server = new Mongodb.Server("localhost",27017,{auto_reconnect:true})
//创建数据库
var db = new Mongodb.Db('cloud',server,{safe:true})
//连接数据库
db.open((err,db) => {
    if(err) {
        console.log('连接数据库失败')
    } else {
        console.log('连接数据库成功')
    }
})
```

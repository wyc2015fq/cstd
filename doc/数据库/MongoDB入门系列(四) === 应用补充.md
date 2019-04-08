# MongoDB入门系列(四) ===> 应用补充

2019年01月12日 15:05:32 [刘翾](https://me.csdn.net/c_kite) 阅读数：119



版权声明：原创文章，如想转载，请注明原文网址，注明出处；否则，禁止转载；谢谢配合！	https://blog.csdn.net/c_kite/article/details/86361302

# 全系列目录

- MongoDB入门系列(一) ===> 概念解析 <https://blog.csdn.net/c_kite/article/details/85777134>
- MongoDB入门系列(二) ===> 创建用户 <https://blog.csdn.net/c_kite/article/details/85845983>
- MongoDB入门系列(三) ===> 基本操作:增删改查 <https://blog.csdn.net/c_kite/article/details/85853719>
- MongoDB入门系列(四) ===> 应用补充 <https://blog.csdn.net/c_kite/article/details/86361302>

该系列前几篇所遗漏知识点补充



### 文章目录

- [全系列目录](https://blog.csdn.net/c_kite/article/details/86361302#_0)

- - [1. 数据库引用](https://blog.csdn.net/c_kite/article/details/86361302#1__10)

  - [2. findAndModify 查找并修改](https://blog.csdn.net/c_kite/article/details/86361302#2_findAndModify__47)

  - - [2.1 使用findAndModify创造递增列](https://blog.csdn.net/c_kite/article/details/86361302#21_findAndModify_107)

  - [3. 建立索引](https://blog.csdn.net/c_kite/article/details/86361302#3__165)

  - [4. 时间戳](https://blog.csdn.net/c_kite/article/details/86361302#4__181)



## 1. 数据库引用

可以在一条数据里面, 保存相关数据库的引用

格式: `{ $ref : , $id : , $db : }`

三个字段表示的意义为：

- $ref：集合(表)名称
- $id：引用的id
- $db: 数据库名称，可选参数

例:

```
// 有一条这样的数据
{
   "_id":ObjectId("53402597d852426020000002"),
   "address": {
   "$ref": "address_home",
   "$id": ObjectId("534009e4d852427820000002"),
   "$db": "runoob"},
   "contact": "987654321",
   "dob": "01-01-1991",
   "name": "Tom Benzamin"
}

// 现在我想查询address字段(field)里面的内容, 注意这里要使用findOne
// 如果使用find的话, 要写明index位置, 例如let a = db.gkd.findOne({ b: 123 })[0]
let a = db.gkd.findOne({ b: 123 })
let asd = a.caonima
db[asd.$ref].find({"_id": asd.$id})

// 该例子来源于菜鸟教程: http://www.runoob.com/mongodb/mongodb-database-references.html
12345678910111213141516171819
```

## 2. findAndModify 查找并修改

没学这个命令之前是, 先find数据, 然后使用remove 或者 update再次操作. 这个命令可以整合这个两个操作为一个

语法:

```
db.collection.findAndModify({
    query: <document>,
    sort: <document>,
    remove: <boolean>,
    update: <document>,
    new: <boolean>,
    upsert: <boolean>,
    arrayFilters: [ <filterdocument1>, ... ]
});
123456789
```

| 参数         | 类型     | 描述                                                         |
| ------------ | -------- | ------------------------------------------------------------ |
| query        | document | 可选, 与find用一个选择器, 注意: 虽然find可能会匹配多个文档, 但findAndModify只会选择一个要修改的文档 |
| sort         | document | 可选, 排序query查询到的结果, findAndModify会选择排好序的第一条来修改文档 |
| remove       | boolean  | 必须指定remove或update字段。删除查询字段中指定的文档。将其设置为true可删除所选文档。默认值为false |
| update       | document | 必须指定remove或update字段。执行所选文档的更新。更新字段使用相同的更新运算符或字段 |
| upsert       | boolean  | 可选, 与update参数结合使用. 当为true的时候, 如果没有文档与查询匹配，则创建新文档, 并插入集合(表)中, 或者更新与查询匹配的单个文档。 |
| new          | boolean  | 可选, 如果为true，则返回修改后的文档而不是原始文档. 默认为false |
| arrayFilters | array    | 一组过滤器, 用于确定一个数组元素应该修改哪些                 |

```
// 例1
db.people.findAndModify({
    query: { name: "Andy" },
    sort: { rating: 1 }, // rating 升序排列
    update: { $inc: { score: 1 } }, score增加1
    upsert: true
})

// 例2 数组元素
// student集合(表)中插入数据
db.students.insert([
   { "_id" : 1, "grades" : [ 95, 92, 90 ] },
   { "_id" : 2, "grades" : [ 98, 100, 102 ] },
   { "_id" : 3, "grades" : [ 95, 110, 100 ] }
])

// 查找grades大于等于100的文档, 并将值修改为100
db.students.findAndModify({
   query: { grades: { $gte: 100 } },
   update: { $set: { "grades.$[element]" : 100 } },
   arrayFilters: [ { "element": { $gte: 100 } } ]
})

// 结果, 可以看到只有中间的那条数据被修改了, 因为findAndModify只会修改一条数据
{ "_id" : 1, "grades" : [ 95, 92, 90 ] }
{ "_id" : 2, "grades" : [ 98, 100, 100 ] }
{ "_id" : 3, "grades" : [ 95, 110, 100 ] }
123456789101112131415161718192021222324252627
```

### 2.1 使用findAndModify创造递增列

先说一下思路:
node的大多数做法是, 建一个集合(表)专门用来存储, 其他表的_id递增字段. 之后其他集合每次插入数据的时候, 手动插入对应的_id键
java的话这里有一篇文章是介绍监听saveListener的 <https://www.jianshu.com/p/2f292969bec4>

下面贴上node代码, java代码可以参考上面的链接, 或者下面的node思路

```
// 建一个单独的集合, 专门用来记录各个集合最后一个ID
db.counters.insert(
   {
      _id: "userid",
      seq: 0
   }
)
db.counters.insert(
   {
      _id: "productid",
      seq: 0
   }
)


// 创建自增函数
function getNextSequence(name) {
   var ret = db.counters.findAndModify(
          {
            query: { _id: name },
            update: { $inc: { seq: 1 } },
            new: true,
            upsert : true // 如果没有匹配到插入一条文档
          }
   );

   return ret.seq;
}


// 这样每次插入数据的时候就可以这样使用
db.users.insert(
   {
     _id: getNextSequence("userid"),
     name: "Mr. X",
     // ... more fields
   }
)

db.products.insert(
   {
     _id: getNextSequence("productid"),
     name: "Mr. Y",
     // ... more fields
   }
)
12345678910111213141516171819202122232425262728293031323334353637383940414243444546
```

## 3. 建立索引

语法:

```
db.collection.createIndex(keys, options)
1
```

Key为你要创建的索引字段，1 为指定按升序创建索引，如果你想按降序来创建索引指定为 -1

查看 查询结果是否调用了索引, 使用如下函数

```
db.collection.find({}).explain()
1
```

返回的参数的详细解释: <https://docs.mongodb.com/manual/reference/explain-results/index.html>

## 4. 时间戳

之前在系列的第一篇中介绍过ObjectId <https://blog.csdn.net/c_kite/article/details/85777134> 在这里稍微回忆一下, MongoDB自动生成的ObjectId的组成部分: ObjectId 是一个12字节 BSON(binary json) 类型数据, 前4个字节表示时间戳, 接下来的3个字节是机器标识码, 紧接的2个字节由进程id组成PID, 最后三个字节是随机数.

具体实现代码可以参考: <https://github.com/mongodb/mongo-java-driver/blob/master/bson/src/main/org/bson/types/ObjectId.java>

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190112145209479.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nfa2l0ZQ==,size_16,color_FFFFFF,t_70)

接下来举一个例子如果获取ObjectId中的时间戳

```
const id = db.gkd.find({a: 147})[0];
console.log(id._id.getTimestamp());
```
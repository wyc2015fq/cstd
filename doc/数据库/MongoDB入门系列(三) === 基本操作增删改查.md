# MongoDB入门系列(三) ===> 基本操作:增删改查



# 全系列目录

- MongoDB入门系列(一) ===> 概念解析 <https://blog.csdn.net/c_kite/article/details/85777134>
- MongoDB入门系列(二) ===> 创建用户 <https://blog.csdn.net/c_kite/article/details/85845983>
- MongoDB入门系列(三) ===> 基本操作:增删改查 <https://blog.csdn.net/c_kite/article/details/85853719>
- MongoDB入门系列(四) ===> 应用补充 <https://blog.csdn.net/c_kite/article/details/86361302>



### 文章目录

- [全系列目录](https://blog.csdn.net/c_kite/article/details/85853719#_1)

- [1. 数据库](https://blog.csdn.net/c_kite/article/details/85853719#1__9)

- - [1.1 创建数据库](https://blog.csdn.net/c_kite/article/details/85853719#11__11)
  - [1.2 删除数据库](https://blog.csdn.net/c_kite/article/details/85853719#12__26)

- [2. 集合(表)](https://blog.csdn.net/c_kite/article/details/85853719#2__35)

- - [2.1 创建集合](https://blog.csdn.net/c_kite/article/details/85853719#21__37)
  - [2.2 删除集合](https://blog.csdn.net/c_kite/article/details/85853719#22__73)

- [3. 文档(row)](https://blog.csdn.net/c_kite/article/details/85853719#3_row_85)

- - [3.1 插入文档](https://blog.csdn.net/c_kite/article/details/85853719#31__87)

  - [3.2 查询文档](https://blog.csdn.net/c_kite/article/details/85853719#32__100)

  - - [3.2.1 projection介绍](https://blog.csdn.net/c_kite/article/details/85853719#321_projection_113)

    - [3.2.2 查询运算符介绍](https://blog.csdn.net/c_kite/article/details/85853719#322__139)

    - - [3.2.2.1 运算符](https://blog.csdn.net/c_kite/article/details/85853719#3221__154)

    - [3.2.3 查询数组](https://blog.csdn.net/c_kite/article/details/85853719#323__173)

    - [3.2.4 正则匹配查询](https://blog.csdn.net/c_kite/article/details/85853719#324__206)

    - [3.2.5 限制条件](https://blog.csdn.net/c_kite/article/details/85853719#325__221)

    - - [3.2.5.1 排序sort](https://blog.csdn.net/c_kite/article/details/85853719#3251_sort_223)
      - [3.2.5.2 限制提取数量limit](https://blog.csdn.net/c_kite/article/details/85853719#3252_limit_240)
      - [3.2.5.3 跳过数据skip](https://blog.csdn.net/c_kite/article/details/85853719#3253_skip_247)

    - [3.2.6 聚合 aggregate](https://blog.csdn.net/c_kite/article/details/85853719#326__aggregate_257)

    - - [3.2.6.1 pipeline常用参数介绍](https://blog.csdn.net/c_kite/article/details/85853719#3261_pipeline_306)

      - - [3.2.6.1.1 $lookup例子](https://blog.csdn.net/c_kite/article/details/85853719#32611_lookup_318)
        - [3.2.6.1.2 $unwind例子](https://blog.csdn.net/c_kite/article/details/85853719#32612_unwind_533)

      - [3.2.6.1 常用表达式介绍](https://blog.csdn.net/c_kite/article/details/85853719#3261__570)

  - [3.3 更新文档](https://blog.csdn.net/c_kite/article/details/85853719#33__584)

  - - [3.3.1 $set](https://blog.csdn.net/c_kite/article/details/85853719#331_set_653)
    - [3.3.2 $inc](https://blog.csdn.net/c_kite/article/details/85853719#332_inc_657)
    - [3.3.3 save()](https://blog.csdn.net/c_kite/article/details/85853719#333_save_666)

  - [3.4 删除文档](https://blog.csdn.net/c_kite/article/details/85853719#34__693)



# 1. 数据库

## 1.1 创建数据库

语法: `use DATABASE_NAME`

```
> use gkd
switched to db gkd
12
```

但如果不插入数据, 数据库是显示不出来的.

```
db.集合名(表名).insert({key: value})
1
```

## 1.2 删除数据库

```
> use 数据库名
> db.dropDatabase()
12
```

------

# 2. 集合(表)

## 2.1 创建集合

语法:

```
db.createCollection(name 集合名, options 可选参数)
1
```

options可选参数表格

| 字段   | 类型             | 描述                                                         |
| ------ | ---------------- | ------------------------------------------------------------ |
| capped | 布尔	（可选） | 如果为 true，则创建固定集合。固定集合是指有着固定大小的集合，当达到最大值时，它会自动覆盖最早的文档。当该值为 true 时，必须指定 size 参数。 |
| size   | 数值	（可选） | 为固定集合指定一个最大值（以字节计）。如果 capped 为 true，也需要指定该字段。 |
| max    | 数值	（可选） | 指定固定集合中包含文档的最大数量。                           |

详细参数列表请参考 <https://docs.mongodb.com/manual/reference/method/db.createCollection/index.html>

```
// 例1
db.createCollection("gkd", { capped : true, size : 1024 } )
12
```

但如果没有特殊要求, 没有必要主动输入命令创建集合(表). 在我们插入数据的时候如果系统没有检测到表, 则会自动创建.

```
// 例2
 // 创建gkd表, 插入数据
 db.gkd.insert({"name" : "菜鸟教程"}) 
 
 // 查看集合
 show collections
123456
```

## 2.2 删除集合

```
// 删除集合
db.集合名.drop()
12
```

删除成功返回true

------

# 3. 文档(row)

## 3.1 插入文档

```
// 提到过还几次了
db.集合名.insert();
12
```

如果想一次插入多条数据, 例

```
db.集合名.insert([{key: value}, {key, value}])
1
```

## 3.2 查询文档

命令:

```
db.集合名.find(query, projection)
1
```

| 参数       | 类型     | 描述                                                         |
| ---------- | -------- | ------------------------------------------------------------ |
| query      | document | 可选, 查询条件, 如果要查询全部数据, 可忽略该参数或传递{} (空文档) |
| projection | document | 可选, 指定要在与query匹配的文档中返回的字段(类似select). 如果要查询全部数据, 可忽略该参数 |

### 3.2.1 projection介绍

这个参数差不多相当于mysql里的select, 下面是一个例子

```
// 插入一条文档
> db.gkd.insert({a: 1, b: 2})

// 查询全部文档
> db.gkd.find()
{ "_id" : ObjectId("5c318021a92a4958ddb16ae4"), "a" : 1, "b" : 2 }

// 选择特定参数查询
> db.gkd.find({}, {a: true})
{ "_id" : ObjectId("5c318021a92a4958ddb16ae4"), "a" : 1 }


// 也可以采用链式调用
> db.gkd.find().projection({a: true})
// 返回结果同上
123456789101112131415
```

注:

1. projection参数`key: value`里面的`value`, 也可以传1或者0, 1表示true, 0表示false
2. 除非显示指定`"_id": false`, 否则它默认返回

### 3.2.2 查询运算符介绍

如果我们想要查一个值是否符合某个条件的时候, 就需要用到查询运算符. 例:

```
// 先插入3条数据, 接下来我想找到a大于等于2
db.gkd.insert([{a: 1}, {a: 2}, {a: 3}]);

// 可以看到a的值为一个对象, $gte代表大于等于
db.gkd.find({a: {$gte: 2}})

// 组合筛选, 如果我想要匹配大于1小于3
db.gkd.find({a: {$gt: 1, $lt: 3}})
12345678
```

#### 3.2.2.1 运算符

| 名字    | 描述                          |
| ------- | ----------------------------- |
| $eq     | 等于                          |
| $gt     | 大于                          |
| $gte    | 大于等于                      |
| $in     | 匹配数组中指定的任何值        |
| $lt     | 小于                          |
| $lte    | 小于等于                      |
| $ne     | 匹配不等于指定值的值          |
| $nin    | 匹配不存在数组中指定的任何值. |
| $exists | 匹配具有指定字段的文档(row)   |
| $type   | 匹配字段是指定类型的文档(row) |

还有很多运算符不一一介绍, 只挑选了一些常用的列了出来, 详细请参看官方文档:
<https://docs.mongodb.com/manual/reference/operator/query/#query-selectors>

### 3.2.3 查询数组

有3个运算符需要介绍一下, 分别是`$in. $all. $size`

1. `$in` 只要包含了值就匹配成功
   例:

```
// 我们先插入两条数据
> db.gkd.insert({b: [1]})
> db.gkd.insert({b: [1, 2, 3]})


> db.gkd.find({b: {$in: [1,2]}})
{ "_id" : ObjectId("5c318d5da92a4958ddb16ae5"), "b" : [ 1, 2, 3 ] }
{ "_id" : ObjectId("5c319ed7a92a4958ddb16ae6"), "b" : [ 1 ] }
12345678
```

1. `$all`指定的值必须要全部包含

```
// 用上个例子的数据
> db.gkd.find({b: {$all: [1,2]}})
{ "_id" : ObjectId("5c318d5da92a4958ddb16ae5"), "b" : [ 1, 2, 3 ] }
123
```

1. `$size`数组长度匹配

```
// 用第一个例子的数据
> db.gkd.find({b: {$size: 3}})
{ "_id" : ObjectId("5c318d5da92a4958ddb16ae5"), "b" : [ 1, 2, 3 ] }
123
```

### 3.2.4 正则匹配查询

mongodb还支持正则的查询, 关于正则详细规则, 可以参考我的这篇文章:
<https://blog.csdn.net/c_kite/article/details/53959534>
下面举个例子:

```
// 插入两条数据
 > db.gkd.insert([{item: "liu"}, {item: "xuan"}])

// 匹配n结尾的的item文档
> db.gkd.find({item: /n$/})
{ "_id" : ObjectId("5c31a123a92a4958ddb16ae8"), "item" : "xuan" }
123456
```

### 3.2.5 限制条件

#### 3.2.5.1 排序sort

注意官方提示: 除非指定sort（）方法或使用$ near运算符，否则MongoDB不保证查询结果的顺序

例:

```
// 插入一些数据
> db.gkd.insert([{a: 1}, {b: "xuan"}, {a: 2}, {a: 3}])

// 解释: 查找含有字段a的文档, 并且按照字段a进行升序排列, 1为升序, -1位降序
> db.gkd.find({a:{ $exists: true}}).sort({a: 1})
{ "_id" : ObjectId("5c318558a56d67305c34fb80"), "a" : 1 }
{ "_id" : ObjectId("5c318558a56d67305c34fb81"), "a" : 2 }
{ "_id" : ObjectId("5c318558a56d67305c34fb82"), "a" : 3 }
12345678
```

#### 3.2.5.2 限制提取数量limit

```
db.gkd.find().limit(5); // 限制取5条数据
1
```

#### 3.2.5.3 跳过数据skip

skip（）方法控制结果集的起始点。以下操作将跳过gkd集合中的前5个文档并返回所有剩余文档

```
db.gkd.find().skip(5)
1
```

skip(), limilt(), sort()三个放在一起执行的时候，执行的顺序是先 sort(), 然后是 skip()，最后是显示的 limit()

### 3.2.6 聚合 aggregate

计算集合或视图中数据的聚合值, 类似于sql的count,语法:

```
db.collection.aggregate(pipeline, options)
1
```

| 参数     | 类型     | 描述                     |
| -------- | -------- | ------------------------ |
| pipeline | array    | 一系列数据聚合操作或阶段 |
| options  | document | 可选, 一些可选参数       |

options可以包含以下的值

| 字段         | 类型     | 描述                                                         |
| ------------ | -------- | ------------------------------------------------------------ |
| explain      | boolean  | 可选, 该操作返回带有文档的游标，该文档包含有关聚合管道处理的详细信息 |
| allowDiskUse | boolean  | 可选, 允许写入临时文件。设置为true时，聚合操作可以将数据写入dbPath目录中的_tmp子目录 |
| cursor       | document | 可选的, 指定游标的初始批处理大小                             |

只列举了少量参数, 更多详情请参见
<https://docs.mongodb.com/manual/reference/method/db.collection.aggregate/index.html>

在mongo shell中，如果从`db.collection.aggregate（）`返回的游标没有使用var关键字分配给变量，则mongo shell会自动迭代光标20次

先看一个简单的例子:

```
 // 数据存在集合gkd中
{ _id: 1, cust_id: "abc1", ord_date: ISODate("2012-11-02T17:04:11.102Z"), status: "A", amount: 50 }
{ _id: 2, cust_id: "xyz1", ord_date: ISODate("2013-10-01T17:04:11.102Z"), status: "A", amount: 100 }
{ _id: 3, cust_id: "xyz1", ord_date: ISODate("2013-10-12T17:04:11.102Z"), status: "D", amount: 25 }
{ _id: 4, cust_id: "xyz1", ord_date: ISODate("2013-10-11T17:04:11.102Z"), status: "D", amount: 125 }
{ _id: 5, cust_id: "abc1", ord_date: ISODate("2013-11-12T17:04:11.102Z"), status: "A", amount: 25 }

// 输入聚合命令
db.gkd.aggregate([
                     { $match: { status: "A" } },
                     { $group: { _id: "$cust_id", total: { $sum: "$amount" } } },
                     { $sort: { total: -1 } }
])

// 返回
{ "_id" : "xyz1", "total" : 100 }
{ "_id" : "abc1", "total" : 75 }
1234567891011121314151617
```

解释: 聚合操作选择状态等于“A”的文档，按`cust_id`字段对匹配文档进行分组，并从`amount`字段的总和计算每个`cust_id`字段的总数，并按降序对总字段对结果进行排序

#### 3.2.6.1 pipeline常用参数介绍

| 名字    | 描述                                                         |
| ------- | ------------------------------------------------------------ |
| $match  | 用于过滤数据，只输出符合条件的文档, $match使用MongoDB的标准查询操作 |
| $limit  | 用来限制MongoDB聚合管道返回的文档数                          |
| $skip   | 在聚合管道中跳过指定数量的文档，并返回余下的文档             |
| $group  | 将集合中的文档分组，可用于统计结果                           |
| $sort   | 将输入文档排序后输出                                         |
| $lookup | 将两个集合连接起来                                           |
| $unwind | 从输入文档解构数组字段以输出每个元素的文档(类似于js的解析异构, 下面有例子) |

##### 3.2.6.1.1 $lookup例子

语法:

```
{
   $lookup:
     {
       from: <collection to join>,
       localField: <field from the input documents>,
       foreignField: <field from the documents of the "from" collection>,
       as: <output array field>
     }
}
123456789
```

| 参数         | 描述               |
| ------------ | ------------------ |
| from         | 指定要连接的数据库 |
| localField   | 当前库对应的字段   |
| foreignField | 另一个库对应的字段 |
| as           | 新字段名称         |

例1 简单使用

```
// orders表插入数据
db.orders.insert([
   { "_id" : 1, "item" : "almonds", "price" : 12, "quantity" : 2 },
   { "_id" : 2, "item" : "pecans", "price" : 20, "quantity" : 1 },
   { "_id" : 3  }
])

// inventory库插入数据
db.inventory.insert([
   { "_id" : 1, "sku" : "almonds", description: "product 1", "instock" : 120 },
   { "_id" : 2, "sku" : "bread", description: "product 2", "instock" : 80 },
   { "_id" : 3, "sku" : "cashews", description: "product 3", "instock" : 60 },
   { "_id" : 4, "sku" : "pecans", description: "product 4", "instock" : 70 },
   { "_id" : 5, "sku": null, description: "Incomplete" },
   { "_id" : 6 }
])

// 左连接
db.orders.aggregate([
   {
     $lookup:
       {
         from: "inventory", 
         localField: "item",
         foreignField: "sku",
         as: "inventory_docs"
       }
  }
])

// 查看结果
db.orders.find()

{
   "_id" : 1,
   "item" : "almonds",
   "price" : 12,
   "quantity" : 2,
   "inventory_docs" : [
      { "_id" : 1, "sku" : "almonds", "description" : "product 1", "instock" : 120 }
   ]
}
{
   "_id" : 2,
   "item" : "pecans",
   "price" : 20,
   "quantity" : 1,
   "inventory_docs" : [
      { "_id" : 4, "sku" : "pecans", "description" : "product 4", "instock" : 70 }
   ]
}
{
   "_id" : 3,
   "inventory_docs" : [
      { "_id" : 5, "sku" : null, "description" : "Incomplete" },
      { "_id" : 6 }
   ]
}
12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455565758
```

例2: 如果要聚合数组怎么办, 需要用到下一小节讲的$unwind, 可以先看一下

```
// orders 库有如下的数据
{ "_id" : 1, "item" : "MON1003", "price" : 350, "quantity" : 2, "specs" :
[ "27 inch", "Retina display", "1920x1080" ], "type" : "Monitor" }

// inventory 库有如下的数据
{ "_id" : 1, "sku" : "MON1003", "type" : "Monitor", "instock" : 120,
"size" : "27 inch", "resolution" : "1920x1080" }
{ "_id" : 2, "sku" : "MON1012", "type" : "Monitor", "instock" : 85,
"size" : "23 inch", "resolution" : "1280x800" }
{ "_id" : 3, "sku" : "MON1031", "type" : "Monitor", "instock" : 60,
"size" : "23 inch", "display_type" : "LED" }

// 连接
db.orders.aggregate([
   {
      $unwind: "$specs" // 拆分specs字段
   },
   {
      $lookup:
         {
            from: "inventory",
            localField: "specs",
            foreignField: "size",
            as: "inventory_docs"
        }
   },
   {
      $match: { "inventory_docs": { $ne: [] } } // 匹配inventory_docs不为空的
   }
])

// 返回结果
{
   "_id" : 1,
   "item" : "MON1003",
   "price" : 350,
   "quantity" : 2,
   "specs" : "27 inch",
   "type" : "Monitor",
   "inventory_docs" : [
      {
         "_id" : 1,
         "sku" : "MON1003",
         "type" : "Monitor",
         "instock" : 120,
         "size" : "27 inch",
         "resolution" : "1920x1080"
      }
   ]
}
1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950
```

例3 指定多个连接条件 (信息量是挺多的… 慢慢看吧…)

如果想要除了相等连接之外, 还想要有其他的判断条件, 例如 ‘小于’ 之类的就需要用到下面这个语法
语法:

```
{
   $lookup:
     {
       from: <collection to join>,
       let: { <var_1>: <expression>, …, <var_n>: <expression> },
       pipeline: [ <pipeline to execute on the collection to join> ],
       as: <output array field>
     }
}
123456789
```

| 字段     | 描述                                   |
| -------- | -------------------------------------- |
| from     | 指定数据库                             |
| let      | 可选, 指定要在管道字段阶段中使用的变量 |
| pipeline | 执行各种操作写在这里                   |
| as       | 指定要添加到输入文档的新数组字段的名称 |

```
// 用法
// orders表插入数据
db.orders.insert([
  { "_id" : 1, "item" : "almonds", "price" : 12, "ordered" : 2 },
  { "_id" : 2, "item" : "pecans", "price" : 20, "ordered" : 1 },
  { "_id" : 3, "item" : "cookies", "price" : 10, "ordered" : 60 }
])

// warehouses表插入数据
db.warehouses.insert([
  { "_id" : 1, "stock_item" : "almonds", warehouse: "A", "instock" : 120 },
  { "_id" : 2, "stock_item" : "pecans", warehouse: "A", "instock" : 80 },
  { "_id" : 3, "stock_item" : "almonds", warehouse: "B", "instock" : 60 },
  { "_id" : 4, "stock_item" : "cookies", warehouse: "B", "instock" : 40 },
  { "_id" : 5, "stock_item" : "cookies", warehouse: "A", "instock" : 80 }
])

// 关于$expr参数的详细介绍 => https://docs.mongodb.com/manual/reference/operator/query/expr/#op._S_expr
// 关于$project参数的详细介绍 => https://docs.mongodb.com/manual/reference/operator/aggregation/project/index.html

// 简单总结$expr和$project的含义(太多了.... 写不完了, 详情请参考上面链接)
// $expr:  在$expr内部可以使用聚合表达式, 内部可以使用let参数中定义的变量, 要采用'$$name'形式
// $project: 类似于find内的projection, 可以过滤参数
db.orders.aggregate([
   {
      $lookup:
         {
           from: "warehouses",
           let: { order_item: "$item", order_qty: "$ordered" }, // 定义两个变量
           pipeline: [
              { $match: // 匹配query
                 { $expr: // 访问变量, 愈合操作
                    { $and: // 并操作
                       [
                         { $eq: [ "$stock_item",  "$$order_item" ] }, // warehouses.stock_item === orders.item
                         { $gte: [ "$instock", "$$order_qty" ] } // warehouses.instock >= orders.ordered
                       ]
                    }
                 }
              },
              { $project: { stock_item: 0 } } // 0和false同样的意思, 过滤掉stock_item字段
           ],
           as: "stockdata" // 新字段名字叫做stockdata
         }
    }
])
12345678910111213141516171819202122232425262728293031323334353637383940414243444546
```

##### 3.2.6.1.2 $unwind例子

语法:

```
{
  $unwind:
    {
      path: <field path>,
      includeArrayIndex: <string>,
      preserveNullAndEmptyArrays: <boolean>
    }
}
12345678
```

| 参数                       | 类型    | 描述                                                         |
| -------------------------- | ------- | ------------------------------------------------------------ |
| path                       | string  | 数组字段的字段路径。要指定字段路径，请在字段名称前加上美元符号$，并用引号括起来 |
| includeArrayIndex          | string  | 可选, 用于保存元素的数组索引的新字段的名称。该名称不能以美元符号$开头 |
| preserveNullAndEmptyArrays | boolean | 可选, 如果为true，如果路径为null，missing或为空数组，$ unwind将输出文档。如果为false，则$ unwind在路径为null，missing或空数组时不输出文档 |

```
// 例
// gkd库里有条数据
{ "_id" : 1, "item" : "ABC1", sizes: [ "S", "M", "L"] }

// 使用聚合给拆开
db.gkd.aggregate( [ { $unwind : "$sizes" } ] )

// 返回数据
{ "_id" : 1, "item" : "ABC1", "sizes" : "S" }
{ "_id" : 1, "item" : "ABC1", "sizes" : "M" }
{ "_id" : 1, "item" : "ABC1", "sizes" : "L" }
1234567891011
```

更多参数点击这里: <https://docs.mongodb.com/manual/reference/operator/aggregation-pipeline/>

#### 3.2.6.1 常用表达式介绍

| 表达式 | 描述                             |
| ------ | -------------------------------- |
| $sum   | 计算总和                         |
| $avg   | 计算平均值                       |
| $min   | 获取集合中所有文档对应值得最小值 |
| $max   | 获取集合中所有文档对应值得最大值 |
| $push  | 在结果文档中插入值到一个数组中   |

更多参数点击这里: <https://docs.mongodb.com/manual/reference/operator/aggregation/>

## 3.3 更新文档

语法

```
db.collection.update(
   <query>,
   <update>,
   {
     upsert: <boolean>,
     multi: <boolean>,
     writeConcern: <document>,
     collation: <document>,
     arrayFilters: [ <filterdocument1>, ... ]
   }
)
1234567891011
```

| 参数         | 类型     | 描述                                                         |
| ------------ | -------- | ------------------------------------------------------------ |
| query        | document | 查询条件, 语法和find方法相同                                 |
| update       | document | update的对象和一些更新的操作符                               |
| upsert       | boolean  | 可选。如果设置为true，则在没有文档与查询条件匹配时创建新文档。默认值为false，如果未找到匹配项，则不会插入新文档 |
| multi        | boolean  | 可选。如果设置为true，则更新符合查询条件的多个文档。如果设置为false，则更新一个文档。默认值为false |
| writeConcern | document | 可选, 抛出异常的级别                                         |
| collation    | document | 可选, collation允许用户为字符串比较指定特定于语言的规则，例如字母和重音标记的规则 |
| arrayFilters | array    | 可选的。一组过滤文档，用于确定要为array字段上的更新操作修改哪些元素. 例: [{ “x.a”: { $gt: 85 } }, { “x.b”: { $gt: 80 } }] |

```
// 例:
{
  _id: 1,
  item: "TBD",
  stock: 0,
  info: { publisher: "1111", pages: 430 },
  tags: [ "technology", "computer" ],
  ratings: [ { by: "ijk", rating: 4 }, { by: "lmn", rating: 5 } ],
  reorder: false
}


// 命令语句
db.books.update(
   { _id: 1 },
   {
     $inc: { stock: 5 },
     $set: {
       item: "ABC123",
       "info.publisher": "2222",
       tags: [ "software" ],
       "ratings.1": { by: "xyz", rating: 3 }
     }
   }
)

// 结果
{
  "_id" : 1,
  "item" : "ABC123",
  "stock" : 5,
  "info" : { "publisher" : "2222", "pages" : 430 },
  "tags" : [ "software" ],
  "ratings" : [ { "by" : "ijk", "rating" : 4 }, { "by" : "xyz", "rating" : 3 } ],
  "reorder" : false
}
123456789101112131415161718192021222324252627282930313233343536
```

我们可以看到如果想要修改数组中的某个值, 可以采用 `a.b` 的形式

### 3.3.1 $set

如果该字段不存在，$ set将添加具有指定值的新字段，前提是新字段不违反类型约束. 也就是说增加列的操作使用update操作也可以

### 3.3.2 $inc

我理解的意思是对于数字的增减. <https://docs.mongodb.com/manual/reference/operator/update/inc/#up._S_inc>

可以看下刚刚3.3节的例子, `$inc: {stock : 5}`, 最后的结果输出的事原来的stock(值为0) + 5 = 5

`$inc`只接受数字类型, 如果在原文档中不存在会创建一个

### 3.3.3 save()

save是替换和插入文档的整合, 如果文档不包含_id字段，则save（）方法将调用insert（）方法, 如果包含则调用update方法, 替换掉整条文档. 语法:

```
db.collection.save(
   <document>,
   {
     writeConcern: <document>
   }
)
123456
```

// 例

```
// update
db.gkd.save({
    _id: ObjectId("5c34262c2b52c0876812847c"),
    asd: 'asd'
})

// insert
db.gkd.save({
    asd: 'asd'
})
12345678910
```

## 3.4 删除文档

语法:

```
db.collection.remove(
   <query>,
   {
     justOne: <boolean>,
     writeConcern: <document>,
     collation: <document>
   }
)
12345678
```

| 参数    | 类型     | 描述                                                         |
| ------- | -------- | ------------------------------------------------------------ |
| query   | document | 使用查询运算符指定删除条件。要删除集合中的所有文档，请传递空文档（{}） |
| justOne | boolean  | 可选的。ture的话删除仅一个文档。省略默认值为false, 删除与删除条件匹配的所有文档 |

// 例

```
db.gkd.remove({
    likes: {
        $gte: 800
    }
})

// 仅删除一个
db.gkd.remove({
    likes: {
        $gte: 800
    }
}, true)
```
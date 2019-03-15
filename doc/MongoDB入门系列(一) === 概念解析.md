# MongoDB入门系列(一) ===> 概念解析

2019年01月04日 15:04:53 [刘翾](https://me.csdn.net/c_kite) 阅读数：47



学习笔记记录, 内容出自菜鸟教程http://www.runoob.com/mongodb/mongodb-databases-documents-collections.html

# 全系列目录

- MongoDB入门系列(一) ===> 概念解析 <https://blog.csdn.net/c_kite/article/details/85777134>
- MongoDB入门系列(二) ===> 创建用户 <https://blog.csdn.net/c_kite/article/details/85845983>
- MongoDB入门系列(三) ===> 基本操作:增删改查 <https://blog.csdn.net/c_kite/article/details/85853719>
- MongoDB入门系列(四) ===> 应用补充 <https://blog.csdn.net/c_kite/article/details/86361302>



### 文章目录

- [全系列目录](https://blog.csdn.net/c_kite/article/details/85777134#_2)

- - [1. 和sql术语的对比](https://blog.csdn.net/c_kite/article/details/85777134#1_sql_10)

  - [2. 数据库](https://blog.csdn.net/c_kite/article/details/85777134#2__26)

  - - [2.1 常用命令](https://blog.csdn.net/c_kite/article/details/85777134#21__28)

    - - [2.1.1. "show dbs" 命令可以显示所有数据的列表](https://blog.csdn.net/c_kite/article/details/85777134#211_show_dbs__37)
      - [2.1.2. 运行"use"命令，可以连接到一个指定的数据库](https://blog.csdn.net/c_kite/article/details/85777134#212_use_46)
      - [2.1.3. 执行 "db" 命令可以显示当前数据库对象或集合](https://blog.csdn.net/c_kite/article/details/85777134#213__db__53)

    - [2.2 数据库命名规则](https://blog.csdn.net/c_kite/article/details/85777134#22__60)

    - [2.3 默认数据库](https://blog.csdn.net/c_kite/article/details/85777134#23__68)

  - [3. 文档(也就是mysql的row)](https://blog.csdn.net/c_kite/article/details/85777134#3_mysqlrow_80)

  - - [3.1 文档键命名规范：](https://blog.csdn.net/c_kite/article/details/85777134#31__93)

  - [3.2 集合(也就是mysql里table表)](https://blog.csdn.net/c_kite/article/details/85777134#32_mysqltable_101)

  - - [3.4 合法的集合名](https://blog.csdn.net/c_kite/article/details/85777134#34__111)
    - [3.5 capped collections](https://blog.csdn.net/c_kite/article/details/85777134#35_capped_collections_118)

  - [4. 元数据](https://blog.csdn.net/c_kite/article/details/85777134#4__140)

  - [5. MongoDB 数据类型](https://blog.csdn.net/c_kite/article/details/85777134#5_MongoDB__168)

  - - [5.1 Object ID](https://blog.csdn.net/c_kite/article/details/85777134#51_Object_ID_190)



## 1. 和sql术语的对比

| SQL术语/概念 | MongoDB术语/概念 | 解释/说明                           |
| ------------ | ---------------- | ----------------------------------- |
| database     | database         | 数据库                              |
| table        | collection       | 数据库表/集合                       |
| row          | document         | 数据记录行/文档                     |
| column       | field            | 数据字段/域                         |
| index        | index            | 索引                                |
| table joins  |                  | 表连接,MongoDB不支持                |
| primary key  | primary key      | 主键,MongoDB自动将_id字段设置为主键 |

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190104141028982.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nfa2l0ZQ==,size_16,color_FFFFFF,t_70)

------

## 2. 数据库

### 2.1 常用命令

一个mongodb中可以建立多个数据库。

MongoDB的默认数据库为"db"，该数据库存储在data目录中。

MongoDB的单个实例可以容纳多个独立的数据库，每一个都有自己的集合和权限，不同的数据库也放置在不同的文件中。

#### 2.1.1. “show dbs” 命令可以显示所有数据的列表

```
 show dbs
admin   0.000GB
config  0.000GB
local   0.000GB
1234
```

#### 2.1.2. 运行"use"命令，可以连接到一个指定的数据库

```
use admin
switched to db admin
12
```

#### 2.1.3. 执行 “db” 命令可以显示当前数据库对象或集合

```
db
admin
12
```

### 2.2 数据库命名规则

- 不能是空字符串（"")。
- 不得含有’ '（空格)、.、$、/、\和\0 (空字符)。
- 应全部小写。
- 最多64字节。

### 2.3 默认数据库

从我刚刚安装好的数据库中可以看到, 有3个默认的数据库
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190104140645839.png)

解释:

- admin： 从权限的角度来看，这是"root"数据库。要是将一个用户添加到这个数据库，这个用户自动继承所有数据库的权限。一些特定的服务器端命令也只能从这个数据库运行，比如列出所有的数据库或者关闭服务器。
- local: 这个数据永远不会被复制，可以用来存储限于本地单台服务器的任意集合
- config: 当Mongo用于分片设置时，config数据库在内部使用，用于保存分片的相关信息。

------

## 3. 文档(也就是mysql的row)

文档是一组键值(key-value)对(即[BSON, Binary JSON](https://baike.baidu.com/item/BSON/10981745))

需要注意的是：

1. 文档中的键/值对是有序的。
2. 文档中的值不仅可以是在双引号里面的字符串，还可以是其他几种数据类型（甚至可以是整个嵌入的文档)。
3. MongoDB区分类型和大小写。
4. MongoDB的文档不能有重复的键。
5. 文档的键是字符串。除了少数例外情况，键可以使用任意UTF-8字符。

### 3.1 文档键命名规范：

1. 键不能含有`\0`(空字符)。这个字符用来表示键的结尾。
2. `.`和`$`有特别的意义，只有在特定环境下才能使用。
3. 以下划线`_`开头的键是保留的(不是严格要求的)。

------

## 3.2 集合(也就是mysql里table表)

集合存在于数据库中，集合没有固定的结构.
比如，我们可以将以下不同数据结构的文档插入到集合中：

```
{"site":"www.baidu.com"}
{"site":"www.google.com","name":"Google"}
12
```

当第一个文档插入时，集合就会被创建

### 3.4 合法的集合名

1. 集合名不能是空字符串`" "`。
2. 集合名不能含有`\0`字符（空字符)，这个字符表示集合名的结尾。
3. 集合名不能以`system.`开头，这是为系统集合保留的前缀。
4. 用户创建的集合名字不能含有保留字符。有些驱动程序的确支持在集合名里面包含，这是因为某些系统生成的集合中包含该字符。除非你要访问这种系统创建的集合，否则千万不要在名字里出现`$`。

### 3.5 capped collections

Capped collections 就是固定大小的集合

Capped collections 是高性能自动的维护对象的插入顺序。它非常适合类似记录日志的功能和标准的 collection 不同，你必须要显式的创建一个capped collection，指定一个 collection 的大小，单位是字节。collection 的数据存储空间值提前分配的

Capped collections 可以按照文档的插入顺序保存到集合(表)中，而且这些文档在磁盘上存放位置也是按照插入顺序来保存的，所以当我们更新Capped collections 中 文档(行)的时候，更新后的文档不可以超过之前文档(行)的大小，这样话就可以确保所有文档在磁盘上的位置一直保持不变

由于 Capped collection 是按照文档的插入顺序而不是使用索引确定插入位置，这样的话可以提高增添数据的效率。

```
db.createCollection("mycoll", {capped:true, size:100000})
1
```

- 在 capped collection 中，你能添加新的对象。
- 能进行更新，然而，对象不会增加存储空间。如果增加，更新就会失败 。
- 使用 Capped Collection 不能删除一个文档，可以使用 drop() 方法删除 collection 所有的行。
- 删除之后，你必须显式的重新创建这个 collection。
- 在32bit机器中，capped collection 最大存储为 1e9( 1X109)个字节。

------

## 4. 元数据

数据库的信息是存储在集合中。它们使用了系统的命名空间：

```
数据库名字.system.*
1
```

在MongoDB数据库中名字空间 `dbname.system.*` 是包含多种系统信息的特殊集合(Collection)，如下:

| 集合命名空间             | 描述                                      |
| ------------------------ | ----------------------------------------- |
| dbname.system.namespaces | 列出所有名字空间。                        |
| dbname.system.indexes    | 列出所有索引。                            |
| dbname.system.profile    | 包含数据库概要(profile)信息。             |
| dbname.system.users      | 列出所有可访问数据库的用户。              |
| dbname.local.sources     | 包含复制对端（slave）的服务器信息和状态。 |

对于修改系统集合中的对象有如下限制。

在`{{system.indexes}}`插入数据，可以创建索引。但除此之外该表信息是不可变的(特殊的drop index命令将自动更新相关信息)。

`{{system.users}}`是可修改的。`{{system.profile}}`是可删除的。

## 5. MongoDB 数据类型

| 数据类型           | 描述                                                         |
| ------------------ | ------------------------------------------------------------ |
| String             | 字符串。存储数据常用的数据类型。在 MongoDB 中，UTF-8 编码的字符串才是合法的。 |
| Integer            | 整型数值。用于存储数值。根据你所采用的服务器，可分为 32 位或 64 位。 |
| Boolean            | 布尔值。用于存储布尔值（真/假）。                            |
| Double             | 双精度浮点值。用于存储浮点值。                               |
| Min/Max keys       | 将一个值与 BSON（二进制的 JSON）元素的最低值和最高值相对比。 |
| Array              | 用于将数组或列表或多个值存储为一个键。                       |
| Timestamp          | 时间戳。记录文档修改或添加的具体时间。                       |
| Object             | 用于内嵌文档。                                               |
| Null               | 用于创建空值。                                               |
| Symbol             | 符号。该数据类型基本上等同于字符串类型，但不同的是，它一般用于采用特殊符号类型的语言。 |
| Date               | 日期时间。用 UNIX 时间格式来存储当前日期或时间。你可以指定自己的日期时间：创建 Date 对象，传入年月日信息。 |
| Object ID          | 对象 ID。用于创建文档的 ID。                                 |
| Binary Data        | 二进制数据。用于存储二进制数据。                             |
| Code               | 代码类型。用于在文档中存储 JavaScript 代码。                 |
| Regular expression | 正则表达式类型。用于存储正则表达式。                         |

### 5.1 Object ID

ObjectId 类似唯一主键，可以很快的去生成和排序，包含 12 bytes，含义是：

- 前 4 个字节表示创建 unix 时间戳,格林尼治时间 UTC 时间，比北京时间晚了 8 个小时
- 接下来的 3 个字节是机器标识码
- 紧接的两个字节由进程 id 组成 PID
- 最后三个字节是随机数

![在这里插入图片描述](https://img-blog.csdnimg.cn/2019010415002146.png)

具体实现代码可以参考这里: <https://github.com/mongodb/mongo-java-driver/blob/master/bson/src/main/org/bson/types/ObjectId.java>

MongoDB 中存储的文档必须有一个 _id 键。这个键的值可以是任何类型的，默认是个 ObjectId 对象

由于 ObjectId 中保存了创建的时间戳，所以你不需要为你的文档保存时间戳字段，你可以通过 `getTimestamp` 函数来获取文档的创建时间:
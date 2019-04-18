# MongoDB学习笔记(索引) - z69183787的专栏 - CSDN博客
2016年10月27日 07:16:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：485
一、索引基础：
    MongoDB的索引几乎与传统的关系型数据库一模一样，这其中也包括一些基本的优化技巧。下面是创建索引的命令：
    > db.test.ensureIndex({"username":1})
    可以通过下面的名称查看索引是否已经成功建立：
    > db.test.getIndexes()
    删除索引的命令是：
    > db.test.dropIndex({"username":1})
    在MongoDB中，我们同样可以创建复合索引，如：
--
 数字1表示username键的索引按升序存储，-1表示age键的索引按照降序方式存储。
    > db.test.ensureIndex({"username":1, "age":-1})
    该索引被创建后，基于username和age的查询将会用到该索引，或者是基于username的查询也会用到该索引，但是只是基于age的查询将不会用到该复合索引。因此可以说，如果想用到复合索引，必须在查询条件中包含复合索引中的前N个索引列。然而如果查询条件中的键值顺序和复合索引中的创建顺序不一致的话，MongoDB可以智能的帮助我们调整该顺序，以便使复合索引可以为查询所用。如：
    > db.test.find({"age": 30, "username": "stephen"})
    对于上面示例中的查询条件，MongoDB在检索之前将会动态的调整查询条件文档的顺序，以使该查询可以用到刚刚创建的复合索引。
    我们可以为内嵌文档创建索引，其规则和普通文档没有任何差别，如：
    > db.test.ensureIndex({"comments.date":1})
    对于上面创建的索引，MongoDB都会根据索引的keyname和索引方向为新创建的索引自动分配一个索引名，下面的命令可以在创建索引时为其指定索引名，如：
    > db.test.ensureIndex({"username":1},{"name":"testindex"})    
    随着集合的增长，需要针对查询中大量的排序做索引。如果没有对索引的键调用sort，MongoDB需要将所有数据提取到内存并排序。因此在做无索引排序时，如果数据量过大以致无法在内存中进行排序，此时MongoDB将会报错。
二、唯一索引：
    在缺省情况下创建的索引均不是唯一索引。下面的示例将创建唯一索引，如：
    > db.test.ensureIndex({"userid":1},{"unique":true})
    如果再次插入userid重复的文档时，MongoDB将报错，以提示插入重复键，如：
    > db.test.insert({"userid":5})
    > db.test.insert({"userid":5})
    E11000 duplicate key error index: test.test.$userid_1  dup key: { : 5.0 }    
    如果插入的文档中不包含userid键，那么该文档中该键的值为null，如果多次插入类似的文档，MongoDB将会报出同样的错误，如：
    > db.test.insert({"userid1":5})
    > db.test.insert({"userid1":5})
    E11000 duplicate key error index: test.test.$userid_1  dup key: { : null }        
    如果在创建唯一索引时已经存在了重复项，我们可以通过下面的命令帮助我们在创建唯一索引时消除重复文档，仅保留发现的第一个文档，如：
--先删除刚刚创建的唯一索引。
    > db.test.dropIndex({"userid":1}) 
--插入测试数据，以保证集合中有重复键存在。
    > db.test.remove()
    > db.test.insert({"userid":5})
    > db.test.insert({"userid":5})    
--创建唯一索引，并消除重复数据。
    > db.test.ensureIndex({"userid":1},{"unique":true,"dropDups":true})    
--查询结果确认，重复的键确实在创建索引时已经被删除。
    > db.test.find()
    { "_id" : ObjectId("4fe823c180144abd15acd52e"), "userid" : 5 }    
    我们同样可以创建复合唯一索引，即保证复合键值唯一即可。如：
    > db.test.ensureIndex({"userid":1,"age":1},{"unique":true})    
三、使用explain：
    explain是非常有用的工具，会帮助你获得查询方面诸多有用的信息。只要对游标调用该方法，就可以得到查询细节。explain会返回一个文档，而不是游标本身。如：
    > db.test.find().explain()
    {
        "cursor" : "BasicCursor",
        "nscanned" : 1,
        "nscannedObjects" : 1,
        "n" : 1,
        "millis" : 0,
        "nYields" : 0,
        "nChunkSkips" : 0,
        "isMultiKey" : false,
        "indexOnly" : false,
        "indexBounds" : {
        }    
    }
    explain会返回查询使用的索引情况，耗时和扫描文档数的统计信息。
    "cursor":"BasicCursor"表示没有使用索引。
    "nscanned":1 表示查询了多少个文档。
    "n":1 表示返回的文档数量。
    "millis":0 表示整个查询的耗时。
四、索引管理：
    system.indexes集合中包含了每个索引的详细信息，因此可以通过下面的命令查询已经存在的索引，如：
    > db.system.indexes.find()
    如果在为已有数据的文档创建索引时，可以执行下面的命令，以使MongoDB在后台创建索引，这样的创建时就不会阻塞其他操作。但是相比而言，以阻塞方式创建索引，会使整个创建过程效率更高，但是在创建时MongoDB将无法接收其他的操作。
    > db.test.ensureIndex({"username":1},{"background":true})

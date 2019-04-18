# MongoDB学习总结--入门篇 - z69183787的专栏 - CSDN博客
2016年10月26日 16:42:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2114
一、MongoDB简介
      MongoDB是一个高性能，开源，无模式的文档型数据库，是当前NoSql数据库中比较热门的一种。它在许多场景下可
用于替代传统的关系型数据库或键/值存储方式。Mongo使用C++开发。
Mongo的官方网站地址是[http://www.mongodb.org/](http://www.mongodb.org/)，读者可以在此获得更详细的信息。
二、MongoDB安装与配置
      第一步：下载安装包-->MongoDb官方下载地址：[http://www.mongodb.org/downloads](http://www.mongodb.org/downloads) ，如果是win系统，
注意是64位还是32位版本的，请选择正确的版本。
      第二步：可以在任意磁盘新建目录，如“F:\MongoDB”，解压下载到的安装包，找到bin目录下面全部.exe文件，拷
贝到刚创建的目录下。
      第三步：在“F:\MongoDB”目录下新建“data”文件夹，在其文件夹下新建db文件夹，它将会作为数据存放的根文
件夹，新建“logs ”件夹，在其文件夹新建“mongodb.log”文件存在日志信息。
      第四步：配置MongoDb客户端，打开CMD窗口，按照图示输入命令：
![](http://dl.iteye.com/upload/attachment/0066/3555/a331f9a4-0b75-3b7d-b73e-235ae03aab69.png)
       如图显示MongoDB数据库服务已经成功启动了，在浏览器输入：[http://localhost:27017/](http://localhost:27017/)，可以看到如下提示：
You are trying to access MongoDB on the native driver port. For http diagnostic access, add 1000 to the port number。
三、MongoDB Shell Operations 
      shell命令操作语法和JavaScript很类似，其实控制台底层的查询语句都是用JavaScript脚本完成操作的。
#### Help
```java
> help                          // top level help
> db.help()                     // help on db-specific methods
> db.mycollection.help()        // help on collection methods
> db.mycollection.find().help() // cursor help
```
#### Select Database
The following are three basic commands that provide information about the available databases, and collections in a given database.
|> show dbs|//displays all the databases on the server you are connected to|
|----|----|
|> use *db_name*|//switches to *db_name* on the same server|
|> show collections|//displays a list of all the collections in the current database|
#### Querying
mongo uses a JavaScript API to interact with the database. Because mongo is also a complete JavaScript shell, dbis the variable that is the current database connection.
To query a collection, you simply specify the collection name as a property of the db object, and then call the find() method. For example:
```java
> db.foo.find();
```
This will display the first 10 objects from the *foo* collection. Typing ***it*** after a find() will display the next 10 subsequent objects.
||By setting the shellBatchSize you can change this:```java> DBQuery.shellBatchSize = #```|
||If the shell does not accept the collection name (for example if it starts with a number, contains a space etc), use```java> db['foo'].find()```instead.|
#### Inserting
       In order to insert data into the database, you can simply create a JavaScript object, and call the save()method. For example, to save an object {name: "sara"} in a collection called *foo*, type:
```java
> db.foo.save({ name : "sara"});
```
Note that MongoDB will implicitly create any collection that doesn't already exist.
#### Updating
Let's say you want to change someone's address. You can do this using the following mongo commands:
```java
> person = db.people.findOne( { name : "sara" } );
> person.city = "New York";
> db.people.save( person );
```
#### Deleting
|> db.*foo*.drop()|//drop the entire *foo* collection|
|----|----|
|> db.*foo*.remove()|//remove all objects from the collection|
|> db.*foo*.remove( { name : "sara" } )|//remove objects from the collection where name is sara|
#### Indexes
|> db.*foo*.getIndexKeys()|//get all fields that have indexes on them|
|----|----|
|> db.*foo*.ensureIndex({ _field_ : 1 })|//create an index on *field* if it doesn't exist|
四、MongoDB Shell Reference
### Command Line
|--help|//Show command line options|
|----|----|
|--nodb|//Start without a db, you can connect later with new Mongo() or connect()|
|--shell|//After running a .js file from the command line, stay in the shell rather than terminating|
### Special Command Helpers
Non-javascript convenience macros:
|> help|//Show help|
|----|----|
|> db.help()|//Show help on db methods|
|> db.*myColl*.help()|//Show help on collection methods|
|> show dbs|//Print a list of all databases on this server|
|> use *dbname*|//Set the db variable to represent usage of *dbname* on the server|
|> show collections|//Print a list of all collections for current database|
|> show users|//Print a list of users for current database|
|> show profile|//Print most recent profiling operations that took >= 1ms|
### Basic Shell Javascript Operations
|> db|//The variable that references the current database object / connection. Already defined for you in your instance.|
|----|----|
|> db.auth(user,pass)|//Authenticate with the database (if running in secure mode).|
|> coll = db.*collection*|//Access a specific *collection* within the database.|
|> cursor = coll.find();|//Find all objects in the collection. See [queries](http://mongodb.onconfluence.com/display/DOCS/Queries+and+Cursors).|
|>coll.remove(*objpattern*);|//Remove matching objects from the collection. *objpattern* is an object specifying fields to match. E.g.: coll.remove( { name: "Joe" } );|
|> coll.save(*object*)|//Save an object in the collection, or update if already there. If your object has a presave method, that method will be called before the object is saved to the db (before both updates and inserts)|
|> coll.insert(object)|//Insert object in collection.  No check is made (i.e., no upsert) that the object is not already present in the collection.|
|> coll.update(...)|//Update an object in a collection.  See the [Updating](http://hogwartsrow.iteye.com/display/DOCS/Updating) documentation; update() has many options.|
|> coll.ensureIndex( { *name* : 1 } )|//Creates an index on tab.*name*. Does nothing if index already exists.|
|||
|> coll.drop()|//Drops the collection coll|
|> db.getSisterDB(name)|//Return a reference to another database using this same connection. This allows for cross database queries. Usage example: db.getSisterDB('production').getCollectionNames()|
### Queries
|> coll.find()|//Find all.|
|----|----|
|> it|//Continue iterating the last cursor returned from find().|
|> coll.find( *criteria* );|//Find objects matching *criteria* in the collection. E.g.: coll.find( { name: "Joe" } );|
|> coll.findOne( *criteria* );|//Find and return a single object. Returns null if not found. If you want only one object returned, this is more efficient than just find() as limit(1) is implied. You may use regular expressions if the element type is a string, number, or date: coll.find( { name: /joe/i } );|
|> coll.find( criteria, fields );|//Get just specific fields from the object. E.g.: coll.find( {}, {name:true} );|
|>coll.find().sort( {*field*:1[, *field*:1] });|//Return results in the specified order (field ASC). Use -1 for DESC.|
|> coll.find( *criteria* ).sort( { *field* : 1 } )|//Return the objects matching *criteria*, sorted by *field*.|
|> coll.find( ... ).limit(*n*)|//Limit result to *n* rows. Highly recommended if you need only a certain number of rows for best performance.|
|> coll.find( ... ).skip(*n* )|//Skip *n* results.|
|> coll.count()|//Returns total number of objects in the collection.|
|> coll.find( ... ).count()|//Returns the total number of objects that match the query. Note that the number ignores limit and skip; for example if 100 records match but the limit is 10, count() will return 100. This will be faster than iterating yourself, but still take time.|
More information: see [queries](http://mongodb.onconfluence.com/display/DOCS/Queries+and+Cursors).
### Error Checking
|> [db.getLastError()](http://mongodb.onconfluence.com/display/DOCS/getlasterror)|//Returns error from the last operation.|
|----|----|
|> db.getPrevError()|//Returns error from previous operations.|
|> db.resetError()|//Clear error memory.|
### Administrative Command Helpers
|> db.cloneDatabase(fromhost)|//Clone the current database from the other host specified. fromhost database must be in noauth mode.|
|----|----|
|> db.copyDatabase(fromdb, todb, fromhost)|//Copy fromhost/fromdb to todb on this server. fromhost must be in noauth mode.|
|>db.*fromColl*.renameCollection(toColl)|//Rename collection from fromColl to toColl.|
|> db.repairDatabase()|//Repair and compact the current database. This operation can be very slow on large databases.|
|> db.addUser(user,pwd)|//Add user to current database.|
|> db.getCollectionNames()|//get list of all collections.|
|> db.dropDatabase()|//Drops the current database.|
### Opening Additional Connections
|> db = connect("<host>:<port>/<dbname>")|//Open a new database connection. One may have multiple connections within a single shell, however, automatic getLastError reporting by the shell is done for the 'db' variable only.|
|----|----|
|> conn = new Mongo("hostname")|//Open a connection to a new server. Use getDB() to select a database thereafter.|
|> db = conn.getDB("dbname")|//Select a specific database for a connection|
### Miscellaneous
|> Object.bsonsize(db.foo.findOne())|//prints the bson size of a db object (mongo version 1.3 and greater)|
|----|----|
|> db.foo.findOne().bsonsize()|//prints the bson size of a db object (mongo versions predating 1.3)|
五、资料链接
1、[http://www.mongodb.org/display/DOCS/Overview+-+The+MongoDB+Interactive+Shell](http://www.mongodb.org/display/DOCS/Overview+-+The+MongoDB+Interactive+Shell)
2、[http://www.mongodb.org/display/DOCS/dbshell+Reference](http://www.mongodb.org/display/DOCS/dbshell+Reference)

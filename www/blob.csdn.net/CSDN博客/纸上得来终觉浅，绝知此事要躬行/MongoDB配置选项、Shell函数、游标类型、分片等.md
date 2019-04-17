# MongoDB配置选项、Shell函数、游标类型、分片等 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月14日 13:51:20[boonya](https://me.csdn.net/boonya)阅读数：755








原文地址：[https://dzone.com/refcardz/mongodb](https://dzone.com/refcardz/mongodb)

![](https://img-blog.csdn.net/20170214134808748)


本文采用Google翻译，有些地方数据类型被翻译成了中文，建议中英文对照一起看。


第1节

## 配置选项


### 设置选项

MongoDB的启动选项可以在命令行或配置文件中设置。
两者之间的语法略有不同。以下是三种类型的选项：
|命令行|配置文件|
|----|----|
|--dbpath / path / to / db|dbpath = / path / to / db|
|--auth|auth = true|
|-vvv|vvv = true|

运行mongod --help获取完整的选项列表。
这里有一些最有用：
|选项|描述|
|----|----|
|--config / path / to / config|指定其中设置其他选项的配置文件。|
|--dbpath / path / to / data|数据目录的路径。|
|--logpath /path/to/file.log|日志在哪里存储。这是一个到精确文件的路径，而不是目录。|
|--logappend|在重新启动时，追加到（不截断）现有日志文件。在使用--logpath选项时始终使用此选项。|
|- 叉子|将mongod作为守护进程。|
|--auth|在单个服务器上启用身份验证。|
|--keyFile /path/to/key.txt|在副本集和分片上启用身份验证。获取共享密钥的路径|
|--nohttpinterface|关闭HTTP界面。|
|--bind_ip地址|仅允许来自指定网络接口的连接。|

### 查看选项

如果你在六个月前开始使用一系列选项，你如何看到你使用哪些选项？
shell有帮助：
`> db.serverCmdLineOpts() { “argv” : [ “./mongod”, “--port”, “30000” ], “parsed” : { }, “ok” : 1 }`
解析的字段是从配置文件读取的参数列表





第2节

## 使用Shell



### Shell帮助

有一些功能，如果你忘记一个命令，给你一点帮助：
`> // basic help > help db.help() help on db methods db.mycoll.help() help on collection methods sh.help() sharding helpers rs.help() replica set helpers help admin administrative help help connect connecting to a db help ...`
请注意，对于数据库，集合，副本集，分片，管理等，有单独的帮助功能。
虽然没有明确列出，但也有光标的帮助：
`> // list common cursor functions > db.foo.find().help()`
您可以使用这些功能和帮助作为内置的备忘单。

**查看函数定义**

如果你不明白函数是做什么的，你可以运行它没有括号在shell中看到它的源代码：
`> // run the function > db.serverCmdLineOpts() { “argv” : [ “./mongod” ], “parsed” : { }, “ok” : 1 } > // see its source > db.serverCmdLineOpts`
这可以有助于查看它期望的参数或可以抛出的错误，以及如何从另一种语言运行它。

**使用编辑**

shell有限的多行支持，所以可能很难编程。shell帮助器编辑使这更容易，打开一个文本编辑器，并允许您从那里编辑变量。例如：
`<xmp> > x = function() { /* some function we're going to fill in */ } > edit x <opens emacs with the contents of x> </xmp>`
在编辑器中修改变量，然后保存并退出。
变量将在shell中设置。

必须将EDITOR环境变量或MongoDB shell变量EDITOR设置为使用edit。您可以在MongoDB shell中设置它，如下所示：
`<xmp$gt; > EDITOR=”/user/bin/emacs” </xmp>`
编辑不能从JavaScript脚本，只在交互式shell中。

**.mongorc.js**

如果.mongorc.js文件存在于主目录中，它将在shell启动时自动运行。
使用它来初始化您经常使用的任何帮助函数，并删除不想意外使用的函数。

例如，如果您希望默认情况下没有dropDatabase（）可用，可以将以下行添加到.mongorc.js文件中：
`<xmp> DB.prototype.dropDatabase = function() { print(“No dropping DBs!”); } db.dropDatabase = DB.prototype.dropDatabase; </xmp>`
上面的示例将dropDatabase（）帮助器更改为仅打印消息，而不是删除数据库。

请注意，此技术不应用于安全性，因为确定的用户仍然可以删除没有帮助程序的数据库。
但是，删除危险的管理命令可以帮助防止手指插入。

您可能想要从.mongorc.js中删除助手的几个建议是：
- DB.prototype.shutdownServer
- DBCollection.prototype.drop
- DBCollection.prototype.ensureIndex
- DBCollection.prototype.reIndex
- DBCollection.prototype.dropIndexes

### 更改提示

可以通过将prompt变量设置为返回字符串的函数来定制shell提示：
`<xmp> prompt = function() { try { db.getLastError(); } catch (e) { print(e); } return (new Date())+”$ “; } </xmp>`
如果你设置提示，它将在每次绘制提示时被执行（因此，上面的例子会给你上次操作完成的时间）。

尝试在您的提示中包括db.getLastError（）函数调用。
这包括在默认提示中，并负责服务器重新连接和从写入返回错误。

此外，总是放置任何可以在try / catch块中抛出异常的代码。
这是恼人的让你的提示变成一个例外！







第3节

## 诊断发生了什么



### 查看和杀戮操作

您可以使用currentOp函数查看当前操作：
`<xmp> > db.currentOp() { “inprog” : [ { “opid” : 123, “active” : false, “locktype” : “write”, “waitingForLock” : false, “secs_running” : 200, “op” : “query”, “ns” : “foo.bar”, “query” : { } ... }, ... ] } </xmp>`
使用上面的opid字段，可以终止操作：
`<xmp> > db.killOp(123) </xmp>`
并非所有操作都可以被杀死或立即被杀死。
通常，等待锁的操作在获取锁之前不能被终止。

### 索引用法

使用explain（）查看MongoDB用于查询的索引。
`<xmp> > db.foo.find(criteria).explain() { “cursor” : “BasicCursor”, “isMultiKey” : false, “n” : 2, “nscannedObjects” : 2, “nscanned” : 2, “nscannedObjectsAllPlans” : 2, “nscannedAllPlans” : 2, “scanAndOrder” : false, “indexOnly” : false, “nYields” : 0, “nChunkSkips” : 0, “millis” : 0, “indexBounds” : { }, “server” : “ubuntu:27017” } </xmp>`
在explain（）的输出中有几个重要字段：
- n：返回的结果数。
- nscanned：读取的索引条目的数量。
- nscannedObjects：索引引用的文档数。
- indexOnly：如果查询从来没有触摸集合本身。
- nYields：此查询已释放读取锁并等待其他操作的次数。
- indexBounds：使用索引时，这将显示索引扫描范围。







第4节

## 光标类型



BasicCursor表示未使用索引。
BtreeCursor表示使用正常索引。分片使用并行游标，地理空间索引使用自己的特殊游标。







第5节

## 提示



使用hint（）强制将特定索引用于查询：
`<xmp> > db.foo.find().hint({x:1}) </xmp>`






第6节

## 系统分析



您可以打开系统概要分析以查看数据库上当前正在进行的操作。
对性能分析有性能损失，但它可以帮助隔离慢查询。
`<xmp> > db.setProfilingLevel(2) // profile all operations > db.setProfilingLevel(1) // profile operations that take longer than 100ms > db.setProfilingLevel(1, 500) // profile operations that take longer than 500ms > db.setProfilingLevel(0) // turn off profiling > db.getProfilingLevel(1) // see current profiling setting </xmp>`
概要文件条目存储在启用概要分析的数据库中的上限集合（称为system.profile）中。可以为每个数据库打开和关闭分析。







第7节

## 副本集



要找到复制滞后，请连接到辅助节点并运行此函数：
`<xmp> > db.printReplicationStatus() configured oplog size: 2000MB log length start to end: 23091secs (6.4hrs) oplog first event time: Fri Aug 10 2012 04:33:03 GMT+0200 (CEST) oplog last event time: Mon Aug 20 2012 10:56:51 GMT+0200 (CEST) now: Mon Aug 20 2012 10:56:51 GMT+0200 (CEST) </xmp>`
要查看成员对整个集合的视图，请连接到该集合并运行：
`<xmp> > rs.status() </xmp>`
此命令将显示它认为其他成员的状态和状态。

在辅助节点上运行rs.status（）将在（差的命名）syncingTo字段中显示辅助节点正在同步的人员。







第8节

## 分片



要查看集群的元数据（分片，数据库，块等），请运行以下函数：
`<xmp> > db.printShardingStatus() > db.printShardingStatus(true) // show all chunks Mon Aug 20 2012 10:56:51 GMT+0200 (CEST) </xmp>`
您还可以使用“use config”连接到mongos并查看有关您的分片，数据库，集合或块的数据，然后查询相关集合。
`<xmp> > use config switched to db config > show collections chunks databases lockpings locks mongos settings shards system.indexes version </xmp>`
始终连接到mongos以获取分片信息。
切勿直接连接到配置服务器。不要直接写入配置服务器。
始终使用分片命令和助手。

维护后，有时mongos进程没有实际执行维护将不会有更新版本的配置。
退出这些服务器或运行flushRouterConfig命令通常是快速解决这个问题。
`<xmp> > use admin > db.runCommand({flushRouterConfig:1}) </xmp>`
通常这个问题将表现为setShardVersion失败的错误。

不要担心日志中的setShardVersion错误，但它们不应该涓流到你的应用程序（你不应该得到驱动程序的错误，除非它连接到的mongos不能到达任何配置服务器）。







第9节

## Mongo监控服务（MMS）



MMS是一种免费，容易设置的方式来监视MongoDB。
要使用它，请在[http://mms.10gen.com上](https://translate.googleusercontent.com/translate_c?depth=1&hl=zh-CN&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://mms.10gen.com/&usg=ALkJrhiIV_7is_QjZ_9UfAnsL4sTyZ8V3Q)创建一个帐户

![图形](https://dzone.com/storage/rc-covers/13225-thumb.png)

有关更多[文档](https://translate.googleusercontent.com/translate_c?depth=1&hl=zh-CN&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://mms.10gen.com/help&usg=ALkJrhivhwzLZ7qmo3nkaYBjp_gx6eKD8w)
 ，请参阅http://mms.10gen.com/help。







第10节

## 快速规则



### 数据库

数据库名称不能包含“。”，“$”或“\ 0”（空字符）。
名称只能包含可在文件系统上用作文件名的字符。Admin，config和local是保留的数据库名称（您可以在其中存储自己的数据，但不要删除它们）。

### 集合

集合名称不能包含“$”或“\ 0”。
以“system。”为前缀的名称由MongoDB保留，不能删除（即使您创建了集合）。点通常用于集合名称中的组织，但它们没有语义重要性。名为“che.se”的集合与名为“che”的集合与名为“cheese”的集合没有更多关系。

### 字段名称

字段名称不能包含“。”或“\ 0”。
字段只有当它们是数据库引用时才包含“$”。

### 索引选项
|背景|在后台构建索引，而其他连接可以读写。|
|----|----|
|独特|此键的每个值必须不同。|
|疏|不存在的值不编制索引。非常方便用于索引一些文档可能没有的唯一字段。|
|expireAfterSeconds|需要几秒钟，使它成为“生存时间”集合。|
|dropDups|在创建唯一索引时，删除重复的值，而不是错误输出。请注意，这将删除具有重复值的文档！|
|聚类|当创建聚簇辅助索引时，文档被复制到索引中，使得对该索引的任何查询都是范围查询，因此快。（仅TokuMX分发）|

### 查询格式

查询通常采用以下形式：
`<xmp> {key : {$op : value}} </xmp>`
例如：
`<xmp> {age : {$gte : 18}} </xmp>`
这个规则有三个例外：$和，$或，和$ nor，都是顶层：
`<xmp> {$or : [{age: {$gte : 18}}, {age : {$lt : 18}, parentalConsent:true}}]} </xmp>`
### 更新格式

更新始终采用以下形式：
`<xmp> {key : {$mod : value}} </xmp>`
例如：
`<xmp> {age : {$inc : 1}} </xmp>`






第11节

## 查询运算符



✓：匹配

**X** ：不匹配
|操作员|查询示例|示例文档|
|----|----|----|
|$ gt，$ gte，$ lt，$ lte，$ ne|{numSold：{$ lt：3}}|✓{numSold：1}**X** {numSold：“hello”}**X** {x：1}|
|$ in，$ nin|{age：{$ in：[10，14，21]}}|✓{age：21}✓{age：[9，10，11]}**X** {age：9}|
|$ all|{hand：{$ all：[“10”，“J”，“Q”，“K”，“A”]}}|✓{hand：[“7”，“8”，“9”，“10”，“J”，“Q”，“K”，“A”]}**X** {hand：[“J”，“Q”，“K”]}|
|$ not|{name：{$ not：/ jon / i}}|✓{name：“Jon”}**X** {name：“John”}|
|$ mod|{age：{$ mod：[10，0]}}|✓{age：50}**X** {age：42}|
|$存在|{phone：{$ exist：true}}|✓{phone：“555-555-5555”}**X** {phones：[“555-555-5555”，“1-800-555-5555”]}|
|$ type *|{age：{$ type：2}}|✓{age：“42”}**X** {age：42}|
|$ size|{“top-three”：{$ size：3}}|✓{“top-three”：[“gold”，“silver”，“bronze”]}**X** {“top-three”：[“blue ribbon”]}|
|$ regex|{role：/admin.*/i} {role：{$ regex：'admin。*'，$ options：'i'}}|✓{“top-three”：[“gold”，“silver”，“bronze”]}**X** {“top-three”：[“blue ribbon”]}|







第12节

## 更新修饰符


|修饰符|开始文档|实施例|结束文件|
|----|----|----|----|
|$ set|{x：“foo”}|{$ set：{x：[1,2,3]}}|{x：[1,2,3]}|
|$ unset|{x：“foo”}|{$ unset：{x：t​​rue}}|{}|
|$ inc|{countdown：5}|{$ inc：{countdown：-1}}|{countdown：4}|
|$ push，$ pushAll|{votes：[ - 1，-1,1]}|{$ push：{vote：-1}}|{votes：[ - 1，-1,1，-1}}|
|$ pull，$ pullAll|{blacklist：[“ip1”，“ip2”，“ip3”]}|{$ pull：{blacklist：“ip2”}}|{blacklist：“ip1”，“ip3”}|
|$ pop|{queue：[“1pm”，“3pm”，“8pm”]}|{$ pop：{queue：-1}}|{queue：[“3pm”，“8pm”]}|
|$ addToSet，$ each|{ints：[0,1,3,4]}|{$ addToSet：{ints：{$ each：[1,2,3]}}}|{ints：[0,1,2,3,4]|
|$ rename|{nmae：“sam”}|{$ rename：{nmae：“name”}}|{name：“sam”}|
|$ bit|{permission：6}|{$ bit：{permissions：{or：1}}}|{permission：7}|







第13节

## 聚合管道操作员



聚合框架可用于执行从简单查询到复杂聚合的所有操作。

要使用聚合框架，请将aggregate（）函数作为聚合阶段的管道：
`<xmp> > db.collection.aggregate({$match:{x:1}}, ... {$limit:10}, ... {$group:{_id : “$age”}}) </xmp>`
可用阶段列表：
|操作员|描述|
|----|----|
|{$ project：projection}|包括，排除，重命名和绿色字段。|
|{$ match：match}|查询，接受与传递给find（）的参数相同的参数。|
|{$ limit：num}|将结果限制为num。|
|{$ skip：skip}|跳过num结果。|
|{$ sort：sort}|按给定字段对结果进行排序。|
|{$ group：group}|组结果使用给出的表达式（见下表）。|
|{$ unwind：field}|将嵌入式数组分解成自己的顶级文档。|

要引用字段，请使用语法$ fieldName。
例如，此投影将返回现有的“时间”字段，其中具有新名称“自时代以来的时间”：
`<xmp> {$project: {“time since epoch”: “$time”}} </xmp>`
$ project和$ group都可以获取表达式，这可以使用这个$ fieldName语法如下所示
|表达式操作示例|描述|
|----|----|
|$ add：[“$ age”，1]|向年龄字段添加1。|
|$ divide：[“$ sum”，“$ count”]|将sum字段除以count。|
|$ mod：[“$ sum”，“$ count”]|除以count的余数。|
|$ multiply：[“$ mph”，24，365]|乘以24 * 365的mph。|
|$ subtract：[“$ price”，“$ discount”]|从价格中减去折扣。|
|$ strcasecmp：[“ZZ”，“$ name”]|1（如果名称小于ZZ），0（如果名称为ZZ），-1（如果名称大于ZZ）。|
|$ substr：[“$ phone”，0，3]|获取手机的区号（前三个字符）。|
|$ toLower：“$ str”|将str转换为全部小写。|
|$ toUpper：“$ str”|将str转换为全大写。|
|$ ifNull：[“$ mightExist”，$ add：[“$ doesExist”，1]]|如果mightExist不为null，则返回mightExist。否则返回第二个表达式的结果。|
|$ cond：[exp1，exp2，exp3]|如果exp1求值为true，则返回exp2，否则返回expr3。|







第14节

## 备份



创建备份的最佳方法是在数据库文件处于一致状态（即，不在从/到读取中间）时创建数据库文件的副本。
- 1.使用fsync + lock命令。
这将所有正在进行的写入刷新到磁盘，并阻止新的。`<xmp> {$project: {“time since epoch”: “$time”}} </xmp>`


- 将数据文件复制到新位置。
- 使用unlock命令解锁数据库。`<xmp> > db.fsyncUnlock() </xmp>`



要从此备份还原，请将文件复制到正确的服务器的dbpath并启动mongod。

如果您有一个具有文件系统快照的文件系统，并且您的日志位于相同的卷上，并且没有使用RAID进行任何操作，您可以在不锁定的情况下拍摄快照。在这种情况下，当您恢复时，日志将重放操作，以使数据文件一致。

Mongodump仅用于特殊情况下的备份。
如果你决定使用它，不要fsync +锁先。







第15节

## 副本集维护



### 保持会员当选

要永久停止成员选举，请将其优先级更改为0：
`<xmp> > var config = rs.config() > config.members[2].priority = 0 > rs.reconfig(config) </xmp>`
要防止临时选择辅助节点，请连接到辅助节点并发出freeze命令：
`<xmp> > rs.freeze(10*60) // # of seconds to not become primary </xmp>`
如果您不想永久更改优先级，但需要在主服务器上进行维护，这可以很方便。







第16节

## 降级会员



如果成员当前是主要的，并且您不希望成为，请使用stepDown：
`<xmp> > rs.stepDown(10*60) // # of seconds to not try to become primary again </xmp>`
### 作为独立服务器启动成员

对于维护，通常期望启动辅助并且能够对其进行写入（例如，用于构建索引）。
为了实现这一点，你可以作为一个独立的mongod临时启动一个辅助。

如果辅助节点最初使用以下参数启动：
`<xmp> $ mongod --dbpath /data/db --replSet setName --port 30000 </xmp>`
将其彻底关闭并重新启动它：
`<xmp> $ mongod --dbpath /data/db --port 30001 </xmp>`
请注意，dbpath不会更改，但端口会更改，并且replSet选项将被删除（所有其他选项可以保持不变）。这个mongod将作为一个独立的服务器。副本集的其余部分将在端口30000（而不是30001）上查找成员，因此它将仅显示为“下”到集合的其余部分。

完成维护后，使用原始参数重新启动服务器。







第17节

## 用户管理



### 检查当前用户权限
`<xmp> > db.runCommand( ... { ... usersInfo:”manager”, ... showPrivileges:true ... } ... ) </xmp>`
### 创建superAdmin
`<xmp> > use sensors switched to db sensors > db.createUser( ... { ... user: “sensorsUserAdmin”, ... pwd: “password”, ... roles: ... [ ... { ... role: “userAdmin”, ... db: “sensors” ... } ... ] ... } ... ) </xmp>`
### 查看用户角色
`<xmp> > use sensors switched to db sensors > db.getUser(“sensorsUserAdmin”) { “_id” : “sensors.sensorsUserAdmin”, “user” : “sensorsUserAdmin”, “db” : “sensors”, “roles” : [ { “role” : “userAdmin”, “db” : “sensors” } ] } </xmp>`
### 显示角色权限
`<xmp> > db.getRole( “userAdmin”, { showPrivileges: true } ) </xmp>`
### 授予角色
`<xmp> > db.grantRolesToUser( ... “sensorsUserAdmin”, ... [ ... { role: “read”, db: “admin” } ... ] ... ) </xmp>`
### 撤销角色
`<xmp> > db.revokeRolesFromUser( ... “sensorsUserAdmin”, ... [ ... { role: “userAdmin”, db: “sensors” } ... ] ... ) </xmp>`






第18节

## MongoDB限制


- 最大文档大小为16 MB。
- 命名空间必须小于123个字节。
- 每个命名空间文件必须不大于2047兆字节。
- 索引条目的总大小必须小于1024字节。
- 集合最多可以有64个索引。
- 索引名称（包含命名空间）不能超过125个字符。
- 副本集最多可以有12个成员。
- 分片键最多可以有512个字节。
- 分片键始终是不可变的。
- MongoDB非索引字段排序将返回结果，只有这个操作不使用超过32兆字节的内存。
- 聚合流水线级限制为100兆字节的RAM。
超过限制时，将抛出错误。“allowDiskUse”选项允许聚合流水线阶段使用临时文件进行处理。
- 批量操作仅限于1000次操作。
- 数据库名称区分大小写，并且最多可包含64个字符。
- 集合名称不能包含：$，null或以“system。”开头。

**字段名不能包含：$，null或。****（点）**







第19条

## 其他资源


- 在[http://www.mongodb.org/downloads](https://translate.googleusercontent.com/translate_c?depth=1&hl=zh-CN&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://www.mongodb.org/downloads&usg=ALkJrhgP4dwdmmVDu8c1B5Q_5LeO9rljeA)下载MongoDB
- 文档可从[http://docs.mongodb.org获取](https://translate.googleusercontent.com/translate_c?depth=1&hl=zh-CN&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://docs.mongodb.org/&usg=ALkJrhj04eYjL7ndrrkn0pRym9YwF0xwHQ)
- 请参阅[http://jira.mongodb.org上](https://translate.googleusercontent.com/translate_c?depth=1&hl=zh-CN&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://jira.mongodb.org/&usg=ALkJrhjFTzUiDbe9V40WfyG4P3KMMLC9dA)的路线图和文件错误和请求功能
- 在邮寄名单上提出问题： [http](https://translate.googleusercontent.com/translate_c?depth=1&hl=zh-CN&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://groups.google.com/group/mongodb-user&usg=ALkJrhgooZGSMIhVd9J7CtM6Fd5CGHo9Cw) ： [//groups.google.com/group/mongodb-user](https://translate.googleusercontent.com/translate_c?depth=1&hl=zh-CN&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://groups.google.com/group/mongodb-user&usg=ALkJrhgooZGSMIhVd9J7CtM6Fd5CGHo9Cw)










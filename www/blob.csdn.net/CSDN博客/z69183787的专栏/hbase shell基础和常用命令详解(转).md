# hbase shell基础和常用命令详解(转) - z69183787的专栏 - CSDN博客
2016年10月22日 21:33:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2024
HBase shell的基本用法
hbase提供了一个shell的终端给用户交互。使用命令hbase shell进入命令界面。通过执行 help可以看到命令的帮助信息。
以网上的一个学生成绩表的例子来演示hbase的用法。
|**name**|**grad**|**course**| |
|----|----|----|----|
|**math**|**art**| | |
|Tom|5|97|87|
|Jim|4|89|80|
这里grad对于表来说是一个只有它自己的列族,course对于表来说是一个有两个列的列族,这个列族由两个列组成math和art,当然我们可以根据我们的需要在course中建立更多的列族,如computer,physics等相应的列添加入course列族。
(1)建立一个表scores，有两个列族grad和courese
代码如下:
```
hbase(main):001:0> create ‘scores','grade', ‘course'
```
可以使用list命令来查看当前HBase里有哪些表。使用describe命令来查看表结构。（记得所有的表明、列名都需要加上引号） 
(2)按设计的表结构插入值： 
代码如下:
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
put ‘scores','Tom','grade:','5′ 
put ‘scores','Tom','course:math','97′ 
put ‘scores','Tom','course:art','87′ 
put ‘scores','Jim','grade','4′ 
put ‘scores','Jim','course:','89′ 
put ‘scores','Jim','course:','80′ 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
这样表结构就起来了，其实比较自由，列族里边可以自由添加子列很方便。如果列族下没有子列，加不加冒号都是可以的。 
put命令比较简单，只有这一种用法： 
```
hbase> put ‘t1′, ‘r1′, ‘c1′, ‘value', ts1
```
t1指表名，r1指行键名，c1指列名，value指单元格值。ts1指时间戳，一般都省略掉了。 
(3)根据键值查询数据 
```
get ‘scores','Jim' 
get ‘scores','Jim','grade'
```
可能你就发现规律了，HBase的shell操作，一个大概顺序就是操作关键词后跟表名，行名，列名这样的一个顺序，如果有其他条件再用花括号加上。 
get有用法如下： 
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
hbase> get ‘t1′, ‘r1′ 
hbase> get ‘t1′, ‘r1′, {TIMERANGE => [ts1, ts2]} 
hbase> get ‘t1′, ‘r1′, {COLUMN => ‘c1′} 
hbase> get ‘t1′, ‘r1′, {COLUMN => ['c1', 'c2', 'c3']} 
hbase> get ‘t1′, ‘r1′, {COLUMN => ‘c1′, TIMESTAMP => ts1} 
hbase> get ‘t1′, ‘r1′, {COLUMN => ‘c1′, TIMERANGE => [ts1, ts2], VERSIONS => 4} 
hbase> get ‘t1′, ‘r1′, {COLUMN => ‘c1′, TIMESTAMP => ts1, VERSIONS => 4} 
hbase> get ‘t1′, ‘r1′, ‘c1′ 
hbase> get ‘t1′, ‘r1′, ‘c1′, ‘c2′ 
hbase> get ‘t1′, ‘r1′, ['c1', 'c2'] 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
(4)扫描所有数据 
```
scan ‘scores'
```
也可以指定一些修饰词：TIMERANGE, FILTER, LIMIT, STARTROW, STOPROW, TIMESTAMP, MAXLENGTH,or COLUMNS。没任何修饰词，就是上边例句，就会显示所有数据行。 
例句如下： 
代码如下:
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
hbase> scan ‘.META.' 
hbase> scan ‘.META.', {COLUMNS => ‘info:regioninfo'} 
hbase> scan ‘t1′, {COLUMNS => ['c1', 'c2'], LIMIT => 10, STARTROW => ‘xyz'} 
hbase> scan ‘t1′, {COLUMNS => ‘c1′, TIMERANGE => [1303668804, 1303668904]} 
hbase> scan ‘t1′, {FILTER => “(PrefixFilter (‘row2′) AND (QualifierFilter (>=, ‘binary:xyz'))) AND (TimestampsFilter ( 123, 456))”} 
hbase> scan ‘t1′, {FILTER => org.apache.hadoop.hbase.filter.ColumnPaginationFilter.new(1, 0)} 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
过滤器filter有两种方法指出： 
a. Using a filterString – more information on this is available in the 
Filter Language document attached to the HBASE-4176 JIRA 
b. Using the entire package name of the filter. 
还有一个CACHE_BLOCKS修饰词，开关scan的缓存的，默认是开启的（CACHE_BLOCKS=>true），可以选择关闭（CACHE_BLOCKS=>false）。 
(5)删除指定数据 
代码如下:
```
delete ‘scores','Jim','grade' 
delete ‘scores','Jim'
```
删除数据命令也没太多变化，只有一个： 
```
hbase> delete ‘t1′, ‘r1′, ‘c1′, ts1
```
另外有一个deleteall命令，可以进行整行的范围的删除操作，慎用！ 
如果需要进行全表删除操作，就使用truncate命令，其实没有直接的全表删除命令，这个命令也是disable，drop，create三个命令组合出来的。 
(6)修改表结构 
代码如下:
```
disable ‘scores' 
alter ‘scores',NAME=>'info' 
enable ‘scores'
```
alter命令使用如下（如果无法成功的版本，需要先通用表disable）： 
a、改变或添加一个列族： 
```
hbase> alter ‘t1′, NAME => ‘f1′, VERSIONS => 5
```
b、删除一个列族： 
代码如下:
```
hbase> alter ‘t1′, NAME => ‘f1′, METHOD => ‘delete' 
hbase> alter ‘t1′, ‘delete' => ‘f1′
```
c、也可以修改表属性如MAX_FILESIZE 
```
MEMSTORE_FLUSHSIZE, READONLY,和 DEFERRED_LOG_FLUSH： 
hbase> alter ‘t1′, METHOD => ‘table_att', MAX_FILESIZE => '134217728′
```
d、可以添加一个表协同处理器 
```
hbase> alter ‘t1′, METHOD => ‘table_att', ‘coprocessor'=> ‘hdfs:///foo.jar|com.foo.FooRegionObserver|1001|arg1=1,arg2=2′
```
一个表上可以配置多个协同处理器，一个序列会自动增长进行标识。加载协同处理器（可以说是过滤程序）需要符合以下规则： 
```
[coprocessor jar file location] | class name | [priority] | [arguments]
```
e、移除coprocessor如下： 
```
hbase> alter ‘t1′, METHOD => ‘table_att_unset', NAME => ‘MAX_FILESIZE' 
hbase> alter ‘t1′, METHOD => ‘table_att_unset', NAME => ‘coprocessor$1′
```
f、可以一次执行多个alter命令： 
```
hbase> alter ‘t1′, {NAME => ‘f1′}, {NAME => ‘f2′, METHOD => ‘delete'}
```
(7)统计行数： 
代码如下:
```
hbase> count ‘t1′ 
hbase> count ‘t1′, INTERVAL => 100000 
hbase> count ‘t1′, CACHE => 1000 
hbase> count ‘t1′, INTERVAL => 10, CACHE => 1000
```
count一般会比较耗时，使用mapreduce进行统计，统计结果会缓存，默认是10行。统计间隔默认的是1000行（INTERVAL）。 
(8)disable 和 enable 操作 
很多操作需要先暂停表的可用性，比如上边说的alter操作，删除表也需要这个操作。disable_all和enable_all能够操作更多的表。 
(9)表的删除 
先停止表的可使用性，然后执行删除命令。 
```
drop ‘t1′
```
以上是一些常用命令详解，具体的所有hbase的shell命令如下，分了几个命令群，看英文是可以看出大概用处的，详细的用法使用help “cmd” 进行了解。 
代码如下:
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
COMMAND GROUPS: 
Group name: general 
Commands: status, version 
Group name: ddl 
Commands: alter, alter_async, alter_status, create, describe, disable, disable_all, drop, drop_all, 
enable, enable_all, exists, is_disabled, is_enabled, list, show_filters 
Group name: dml 
Commands: count, delete, deleteall, get, get_counter, incr, put, scan, truncate 
Group name: tools 
Commands: assign, balance_switch, balancer, close_region, compact, flush, hlog_roll, major_compact, 
move, split, unassign, zk_dump 
Group name: replication 
Commands: add_peer, disable_peer, enable_peer, list_peers, remove_peer, start_replication, 
stop_replication 
Group name: security 
Commands: grant, revoke, user_permission 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**hbase shell脚本 **既然是shell命令，当然也可以把所有的hbase shell命令写入到一个文件内，想linux shell脚本程序那样去顺序的执行所有命令。如同写linux shell，把所有hbase shell命令书写在一个文件内，然后执行如下命令即可： 
代码如下:
```
$ hbase shell test.hbaseshell
```
方便好用。
转自 [http://www.jb51.net/article/31172.htm](http://www.jb51.net/article/31172.htm)

# Hbase基础学习--shell常用命令 - BlueSky - CSDN博客
2016年03月07日 14:35:28[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：806
个人分类：[HBase](https://blog.csdn.net/ASIA_kobe/article/category/6114449)
## 1、启动Hbase shell
进入HBase安装目录，输入 ./hbase shell
```
[hadoop@Master bin]$ ./hbase shell
2016-03-07 11:07:02,174 WARN  [main] util.NativeCodeLoader: Unable to load native-hadoop library for your platform... using builtin-java classes where applicable
SLF4J: Class path contains multiple SLF4J bindings.
SLF4J: Found binding in [jar:file:/home/hadoop/soft/hbase-1.2.0/lib/slf4j-log4j12-1.7.5.jar!/org/slf4j/impl/StaticLoggerBinder.class]
SLF4J: Found binding in [jar:file:/home/hadoop/soft/hadoop-2.7.2/share/hadoop/common/lib/slf4j-log4j12-1.7.10.jar!/org/slf4j/impl/StaticLoggerBinder.class]
SLF4J: See http://www.slf4j.org/codes.html#multiple_bindings for an explanation.
SLF4J: Actual binding is of type [org.slf4j.impl.Log4jLoggerFactory]
HBase Shell; enter 'help<RETURN>' for list of supported commands.
Type "exit<RETURN>" to leave the HBase Shell
Version 1.2.0, r25b281972df2f5b15c426c8963cbf77dd853a5ad, Thu Feb 18 23:01:49 CST 2016
```
## 2、常用命令
status
命令返回包括在系统上运行的服务器的细节和系统的状态。
```
hbase(main):002:0> status
1 active master, 0 backup masters, 2 servers, 0 dead, 1.0000 average load
```
version
该命令返回HBase系统使用的版本。
```
hbase(main):003:0> version
1.2.0, r25b281972df2f5b15c426c8963cbf77dd853a5ad, Thu Feb 18 23:01:49 CST 2016
```
table_help
此命令将引导如何使用表引用的命令。
```
hbase(main):001:0> table_help
Help for table-reference commands.
You can either create a table via 'create' and then manipulate the table via commands like 'put', 'get', etc.
See the standard help information for how to use each of these commands.
However, as of 0.96, you can also get a reference to a table, on which you can invoke commands.
For instance, you can get create a table and keep around a reference to it via:
   hbase> t = create 't', 'cf'
Or, if you have already created the table, you can get a reference to it:
   hbase> t = get_table 't'
You can do things like call 'put' on the table:
  hbase> t.put 'r', 'cf:q', 'v'
which puts a row 'r' with column family 'cf', qualifier 'q' and value 'v' into table t.
To read the data out, you can scan the table:
  hbase> t.scan
which will read all the rows in table 't'.
Essentially, any command that takes a table name can also be done via table reference.
Other commands include things like: get, delete, deleteall,
get_all_columns, get_counter, count, incr. These functions, along with
the standard JRuby object methods are also available via tab completion.
For more information on how to use each of these commands, you can also just type:
   hbase> t.help 'scan'
which will output more information on how to use that command.
You can also do general admin actions directly on a table; things like enable, disable,
flush and drop just by typing:
   hbase> t.enable
   hbase> t.flush
   hbase> t.disable
   hbase> t.drop
Note that after dropping a table, your reference to it becomes useless and further usage
is undefined (and not recommended).
```
whoami
该命令返回HBase用户详细信息。
```
hbase(main):002:0> whoami
hadoop (auth:SIMPLE)
    groups: hadoop
```
可以使用命令创建一个表，在这里必须指定表名和列族名。在HBase shell中创建表的语法如下所示。
create ‘<table name>’,’<column family>’
下面给出的是一个表名为emp的样本模式。它有两个列族：“personal data”和“professional data”。
|Row key|personal data|professional data|
|----|----|----|
||||
||||
```
hbase(main):008:0> create 'emp','personal data','professional data'
0 row(s) in 27.2160 seconds
=> Hbase::Table - emp
hbase(main):009:0> list
TABLE                                                                                                      
emp                                                                                                        
1 row(s) in 0.1840 seconds
=> ["emp"]
```
list 是用来列出HBase中所有表的命令。下面给出了 list 命令的语法。
hbase(main):001:0> list
要删除表或改变其设置，首先需要使用 disable 命令关闭表。使用 enable 命令，可以重新启用它。
下面给出的语法是用来禁用一个表：
disable ‘emp’
下面给出的是一个例子，说明如何禁用表。
hbase(main):025:0> disable 'emp'0 row(s)in1.2760 seconds
## 验证
## 禁用表之后，仍然可以通过 list 和exists命令查看到。无法扫描到它存在，它会给下面的错误。
hbase(main):028:0> scan 'emp'
ROW         COLUMN+CELL
ERROR: emp is disabled.
## is_disabled
这个命令是用来查看表是否被禁用。它的语法如下。
hbase> is_disabled 'table name'
下面的例子验证表名为emp是否被禁用。如果禁用，它会返回true，如果没有，它会返回false。
hbase(main):031:0> is_disabled 'emp'true0 row(s)in0.0440 seconds
## disable_all
此命令用于禁用所有匹配给定正则表达式的表。disable_all命令的语法如下。
hbase> disable_all 'r.*'
假设有5个表在HBase，即raja, rajani, rajendra, rajesh 和 raju。下面的代码将禁用所有以 raj 开始的表。
hbase(main):002:0> disable_all 'raj.*'
raja
rajani
rajendra
rajesh
raju
Disable the above 5 tables (y/n)?
y
5 tables successfully disabled
启用表的语法：
enable ‘emp’
给出下面是一个例子，使一个表启用。
hbase(main):005:0> enable 'emp'0 row(s)in0.4580 seconds可以使用exists命令验证表的存在。下面的示例演示了如何使用这个命令。
```
hbase(main):015:0> exists 'emp'
Table emp does exist                                                                                       
0 row(s) in 0.0880 seconds
```
用drop命令可以删除表。在删除一个表之前必须先将其禁用。
hbase(main):018:0> disable 'emp'0 row(s)in1.4580 seconds
hbase(main):019:0> drop 'emp'0 row(s)in0.3060 seconds
使用exists 命令验证表是否被删除。
hbase(main):020:0> exists 'emp'Table emp does not exist
0 row(s)in0.0730 seconds
## drop_all
这个命令是用来在给出删除匹配“regex”表。它的语法如下：
hbase> drop_all ‘t.*’
注意：要删除表，则必须先将其禁用。
使用put命令，可以插入行到一个表。它的语法如下：
put ’<table name>’,’row1’,’<colfamily:colname>’,’<value>’
```
<span style="color: rgb(83, 91, 96);">hbase(main):017:0> put 'emp','1','personal data:name','asia'
0 row(s) in 0.4790 seconds
hbase(main):018:0> put 'emp','1','personal data:city','Beijing'
0 row(s) in 0.0250 seconds
hbase(main):019:0> put 'emp','1','professional data:designation','manager'
0 row(s) in 0.1160 seconds
hbase(main):020:0> put 'emp','1','professional data:salary','10000'
0 row(s) in 0.0230 seconds
hbase(main):021:0> put 'emp','2','personal data:name','kobe'
0 row(s) in 0.0270 seconds
hbase(main):022:0> put 'emp','2','personal data:city','NewYork'
0 row(s) in 0.0080 seconds
hbase(main):023:0> put 'emp','2','professional data:designation','hr'
0 row(s) in 0.0840 seconds
hbase(main):024:0> put 'emp','2','professional data:salary','5000'
0 row(s) in 0.0150 seconds
hbase(main):025:0> scan 'emp'
ROW                         COLUMN+CELL                                                                    
 1                          column=personal data:city, timestamp=1457330959378, value=Beijing              
 1                          column=personal data:name, timestamp=1457330941995, value=asia                 
 1                          column=professional data:designation, timestamp=1457331038117, value=manager   
 1                          column=professional data:salary, timestamp=1457331069327, value=10000          
 2                          column=personal data:city, timestamp=1457331142543, value=NewYork              
 2                          column=personal data:name, timestamp=1457331124334, value=</span><span style="color:#ff6666;">kobe </span><span style="color:#535b60;">                
 2                          column=professional data:designation, timestamp=1457331166755, value=hr        
 2                          column=professional data:salary, timestamp=1457331187014, value=5000           
2 row(s) in 0.1840 seconds
</span>
```
可以使用put命令更新现有的单元格值。按照下面的语法，并注明新值，如下图所示。
put ‘table name’,’row ’,'Column family:column name',’new value’
```
<span style="color:#535b60;">hbase(main):026:0> put 'emp','2','personal data:name','James'
0 row(s) in 0.0490 seconds
hbase(main):027:0> scan 'emp'
ROW                         COLUMN+CELL                                                                    
 1                          column=personal data:city, timestamp=1457330959378, value=Beijing              
 1                          column=personal data:name, timestamp=1457330941995, value=asia                 
 1                          column=professional data:designation, timestamp=1457331038117, value=manager   
 1                          column=professional data:salary, timestamp=1457331069327, value=10000          
 2                          column=personal data:city, timestamp=1457331142543, value=NewYork              
 2                          column=personal data:name, timestamp=1457331564816, value=</span><span style="color:#ff0000;">James</span><span style="color:#535b60;">                
 2                          column=professional data:designation, timestamp=1457331166755, value=hr        
 2                          column=professional data:salary, timestamp=1457331187014, value=5000           
2 row(s) in 0.2330 seconds
</span>
```
get命令和HTable类的get()方法用于从HBase表中读取数据。使用 get 命令，可以同时获取一行数据。它的语法如下：
get’<table name>’,’row1’
```
hbase(main):028:0> get 'emp','1'
COLUMN                      CELL                                                                           
 personal data:city         timestamp=1457330959378, value=Beijing                                         
 personal data:name         timestamp=1457330941995, value=asia                                            
 professional data:designat timestamp=1457331038117, value=manager                                         
 ion                                                                                                       
 professional data:salary   timestamp=1457331069327, value=10000                                           
4 row(s) in 0.3580 seconds
```
## 读取指定列
下面给出的是语法，使用get方法读取指定列。
hbase>get'table name',‘rowid’,{COLUMN =>‘column family:column name ’}
```
hbase(main):030:0> get 'emp','1','personal data:name'
COLUMN                      CELL                                                                           
 personal data:name         timestamp=1457330941995, value=asia                                            
1 row(s) in 0.0450 seconds
```
## 从表删除特定单元格
使用 delete 命令，可以在一个表中删除特定单元格。 delete 命令的语法如下：
delete‘<table name>’,‘<row>’,‘<column name >’,‘<time stamp>’
```
hbase(main):033:0> delete 'emp','2','personal data:city',1457331142543
0 row(s) in 0.0610 seconds
```
```
hbase(main):035:0> delete 'emp','2','professional data:salary'
0 row(s) in 0.0090 seconds
hbase(main):036:0> scan 'emp'
ROW                         COLUMN+CELL                                                                    
 1                          column=personal data:city, timestamp=1457330959378, value=Beijing              
 1                          column=personal data:name, timestamp=1457330941995, value=asia                 
 1                          column=professional data:designation, timestamp=1457331038117, value=manager   
 1                          column=professional data:salary, timestamp=1457331069327, value=10000          
 2                          column=personal data:name, timestamp=1457331564816, value=James                
 2                          column=professional data:designation, timestamp=1457331166755, value=hr        
2 row(s) in 0.0570 seconds
```
## 删除表的所有单元格
使用“deleteall”命令，可以删除一行中所有单元格。下面给出是 deleteall 命令的语法。
deleteall ‘<table name>’,‘<row>’,
scan 命令用于查看HTable数据。使用 scan 命令可以得到表中的数据。它的语法如下：
scan ‘<table name>’
## count
可以使用count命令计算表的行数量。它的语法如下：
count ‘<table name>’

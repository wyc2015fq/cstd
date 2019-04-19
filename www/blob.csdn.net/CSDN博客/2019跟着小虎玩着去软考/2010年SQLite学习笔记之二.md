# 2010年SQLite学习笔记之二 - 2019跟着小虎玩着去软考 - CSDN博客
2010年02月21日 14:55:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：3737
**2010年SQLite学习笔记之二**
# 一.建立数据库
sqlite3.exe test.db
# 二.双击sqlite-3_6_16目录下的程序sqlite3.exe，即可运行
# 三.退出
.exit
或者
.quit
# 四.SQLite支持如下5种数据类型
1.NULL：空值。
2.INTEGER：带符号的整型，具体取决有存入数字的范围大小。
3.REAL：浮点数字，存储为8-byte IEEE浮点数。
4.TEXT：字符串文本。
5.BLOB：二进制对象。
# 五.联系人表格结构如下
create table contact(id integer primary key autoincrement,
lastname varchar(20),firstname varchar(20), 
mobile varchar(30), telephone varchar(20),
emailvarchar(30), company varchar(50),
department varchar(16),address varchar(80),
id1 interger,id2 integer, updatetime datetime);
# 六.查看数据库有哪些数据表
命令是：.tables
# 七.如何插入一条记录
insert into contact(lastname,firstname,mobile,telephone,updatetime) values('刘','畅','13910128132','010-81749136','2009-07-22');
# 八.查看数据表的结构
针对整个数据库
.schema
针对仅仅是contact联系人该表
.schemacontact 注意没有分号
# 九.如何打开一个已经创建的数据库
sqlite3test.db
# 十.如何解决如下问题
**SQL error: near "sqlite3": syntax error**
SQL指令都是以分号（;）结尾的。如果遇到两个减号（--）则代表注解，sqlite3会略过去
# 十一.如何建立索引
create index index_name on table_name(field_to_be_indexed);
# 十二.如何删除一张数据表
drop table contact;
# 十三.查看当前的数据库
.database
# 十四.如何删除一个数据表的数据
delete from contact;
# 十五.如何导入一个文件到某个表中
.import文件路径表名
注意这是非SQL语句，所以不加分号
# 十六.如何设置文件字段的分隔符
.separator “,”
.import e:/contact.txt contact
# 十七.如何查看当前sqllite字段的分隔符是什么？
.show
# 十八.如何将查询结果导出到一个文件
第一步：.output a.txt
第二步：执行要导出的SQL语句
第三步：.output stdout
# 十九.SQL查询语句
select * from film order by year limit 10;
select * from film order by year desc limit 10;
select count(*) from film;
select * from film where starring like 'Jodie%';
select * from film where starring='Jodie Foster';
select title, year from film order by year desc limit 10;
select columns from table_name where expression; 
最常见的用法，当然是倒出所有数据库的内容：
select * from film; 
如果资料太多了，我们或许会想限制笔数：
select * from film limit 10; 
或是照着电影年份来排列：
select * from film order by year limit 10; 
或是年份比较近的电影先列出来：
select * from film order by year desc limit 10; 
或是我们只想看电影名称跟年份：
select title, year from film order by year desc limit 10; 
查所有茱蒂佛斯特演过的电影：
select * from film where starring='Jodie Foster'; 
查所有演员名字开头叫茱蒂的电影('%' 符号便是 SQL 的万用字符）：
select * from film where starring like 'Jodie%'; 
查所有演员名字以茱蒂开头、年份晚于1985年、年份晚的优先列出、最多十笔，只列出电影名称和年份：
select title, year from film where starring like 'Jodie%' and year >= 1985 order by year desc limit 10; 
有时候我们只想知道数据库一共有多少笔资料：
select count(*) from film; 
有时候我们只想知道1985年以后的电影有几部：
select count(*) from film where year >= 1985; 
（进一步的各种组合，要去看SQL专书，不过你大概已经知道SQL为什么这么流行了：这种语言允许你将各种查询条件组合在一起──而我们还没提到「跨数据库的联合查询」呢！）
如何更改或删除资料
了解select的用法非常重要，因为要在sqlite更改或删除一笔资料，也是靠同样的语法。
例如有一笔资料的名字打错了： 
update film set starring='Jodie Foster' where starring='Jodee Foster'; 
就会把主角字段里，被打成'Jodee Foster'的那笔（或多笔）资料，改回成Jodie Foster。 
delete from film where year < 1970; 
就会删除所有年代早于1970年（不含）的电影了。
其他sqlite的特别用法
sqlite可以在shell底下直接执行命令：
sqlite3 film.db "select * from film;" 
输出 HTML 表格： 
sqlite3 -html film.db "select * from film;" 
将数据库「倒出来」： 
sqlite3 film.db ".dump" > output.sql 
利用输出的资料，建立一个一模一样的数据库（加上以上指令，就是标准的SQL数据库备份了）： 
sqlite3 film.db < output.sql 
在大量插入资料时，你可能会需要先打这个指令：
begin; 
插入完资料后要记得打这个指令，资料才会写进数据库中： 
commit;
**创建数据库文件:**
   >SQLite3 d:/test.db 回车
就生成了一个test.db在d盘。
这样同时也SQLite3挂上了这个test.db
   2) 
**用.help可以看看有什么命令**
   >.help 回车即可
   3)可以在这里直接输入SQL语句创建表格 **用;结束** ，然后回车就可以看到了
   4)看看有**创建了多少表**
   >.tables 
   5)看表结构
   >.schema 表名
   6)**看看目前的数据库**
   >.database
   7)**如果要把查询输出到文件**
   >.output 文件名
   > 查询语句；
查询结果就输出到了文件c:/query.txt 
把查询结果用屏幕输出
   >.output stdout
   8)把表结构输出，同时索引也会输出
     .dump 表名
   9)**退出**
   >.exit 或者.quit
2。从[http://sqlite.phxsoftware.com/](http://sqlite.phxsoftware.com/)下载Ado.net驱动。
下载了安装，在安装目录中存在System.Data.SQLite.dll
我们只需要拷贝这个文件到引用目录，并添加引用即可对SQLite数据库操作了
所有的Ado.net对象都是以SQLite开头的，比如SQLiteConnection
连接串只需要如下方式
   Data Source=d:/test.db 或者DataSource=test.db--应用在和应用程序或者.net能够自动找到的目录
剩下的就很简单了~~
3。**SQL****语法**
由于以前用SQLServer或者ISeries，所以DDL的语法很汗颜
   1)**创建一个单个Primary Key的table**   CREATE TABLE  [Admin] (
 [UserName] [nvarchar] (20)   PRIMARY KEY NOT NULL ,
 [Password] [nvarchar] (50)   NOT NULL ,
 [Rank] [smallint] NOT NULL ,
 [MailServer] [nvarchar] (50)   NOT NULL ,
 [MailUser] [nvarchar] (50)   NOT NULL ,
 [MailPassword] [nvarchar] (50)   NOT NULL ,
 [Mail] [nvarchar] (50)   NOT NULL 
   ) ;
**   2)****创建一个多个Primary Key的table**
   CREATE TABLE  [CodeDetail] (
 [CdType] [nvarchar] (10)  NOT NULL ,
 [CdCode] [nvarchar] (20)  NOT NULL ,
 [CdString1] [ntext]   NOT NULL ,
 [CdString2] [ntext]   NOT NULL ,
 [CdString3] [ntext]   NOT NULL,
  PRIMARY KEY (CdType,CdCode)
   ) ;
   3)**创建索引**
   CREATE  INDEX [IX_Account] ON  [Account]([IsCheck], [UserName]);
还可以视图等等。
4.还有很有用的SQL
  Select * from Sqlite_master 
  Select datetime('now')
  Select date('now')
  Select time('now')
**SQLite****内建函数表**
last_statement_change_count()
oh,还有就是看到有人说，好像成批插入的时候，启动事务，比不启动事务快n倍
还有就是尽量使用参数化的SQL,估计和商用DB一样能够自动Prepare.
===========
sqlite可以在shell/dos command底下直接执行命令：
sqlite3 film.db "select * from film;"
**输出 HTML 表格：**
sqlite3 -html film.db "select * from film;"
**将数据库「倒出来」：**
sqlite3 film.db ".dump" > output.sql
利用输出的资料，建立一个一模一样的数据库（加上以上指令，就是标准的SQL数据库备份了）：
sqlite3 film.db < output.sql
在大量插入资料时，你可能会需要先打这个指令：
begin;
插入完资料后要记得打这个指令，资料才会写进数据库中：
commit; 
**SQLITE****深入------常见问题**
**如何建立自动增长字段?**
简短回答：**声明为 INTEGER PRIMARY KEY 的列将会自动增长**。
长一点的答案： 如果你声明表的一列为 INTEGER PRIMARY KEY，那么， 每当你在该列上插入一NULL值时， NULL自动被转换为一个比该列中最大值大1的一个整数，如果表是空的， 将会是1。 (如果是最大可能的主键 9223372036854775807，那个，将键值将是随机未使用的数。） 如，有下列表：
CREATE TABLE t1(
a INTEGER PRIMARY KEY,
b INTEGER
);
在该表上，下列语句
INSERT INTO t1 VALUES(NULL,123);
在逻辑上等价于：
INSERT INTO t1 VALUES((SELECT max(a) FROM t1)+1,123);
有一个新的API叫做 **sqlite3_last_insert_rowid()****， 它将返回最近插入的整数值。 **注意该整数会比表中该列上的插入之前的最大值大1。 该键值在当前的表中是唯一的。但有可能与已从表中删除的值重叠。要想建立在整个表的生命周期中唯一的键值，需要在 INTEGER PRIMARY KEY 上增加AUTOINCREMENT声明。那么，新的键值将会比该表中曾能存在过的最大值大1。如果最大可能的整数值在数据表中曾经存在过，INSERT将会失败， 并返回SQLITE_FULL错误代码。
**多个应用程序或一个应用程序的多个实例可以同时访问同一个数据库文件吗？**
多个进程可同时打开同一个数据库。多个进程可以同时进行SELECT 操作，但在任一时刻，只能有一个进程对数据库进行更改。
SQLite使用读、写锁控制对数据库的访问。（在Win95/98/ME等不支持读、写锁的系统下，使用一个概率性的模拟来代替。）但使用时要注意： 如果数据库文件存放于一个NFS文件系统上，这种锁机制可能不能正常工作。 这是因为 fcntl() 文件锁在很多NFS上没有正确的实现。 在可能有多个进程同时访问数据库的时候，应该避免将数据库文件放到NFS上。在Windows上，Microsoft的文档中说：如果使用 FAT 文件系统而没有运行 share.exe 守护进程，那么锁可能是不能正常使用的。那些在Windows上有很多经验的人告诉我：对于网络文件，文件锁的实现有好多Bug，是靠不住的。如果他们说的是对的，那么在两台或多台Windows机器间共享数据库可能会引起不期望的问题。
我们意识到，没有其它嵌入式的 SQL 数据库引擎能象 SQLite 这样处理如此多的并发。SQLite允许多个进程同时打开一个数据库，同时读一个数据库。当有任何进程想要写时，它必须在更新过程中锁住数据库文件。但那通常只是几毫秒的时间。其它进程只需等待写进程干完活结束。典型地，其它嵌入式的SQL数据库引擎同时只允许一个进程连接到数据库。
但是，Client/Server数据库引擎（如 PostgreSQL, MySQL, 或 Oracle）通常支持更高级别的并发，并且允许多个进程同时写同一个数据库。这种机制在Client/Server结构的数据库上是可能的，因为总是有一个单一的服务器进程很好地控制、协调对数据库的访问。如果你的应用程序需要很多的并发，那么你应该考虑使用一个Client/Server 结构的数据库。但经验表明，很多应用程序需要的并发，往往比其设计者所想象的少得多。
当SQLite试图访问一个被其它进程锁住的文件时，缺省的行为是返回 SQLITE_BUSY。 可以在C代码中使用 sqlite3_busy_handler() 或 sqlite3_busy_timeout() API 函数调整这一行为。
**在SQLite数据库中如何列出所有的表和索引？**
如果你运行 sqlite3 命令行来访问你的数据库，可以键入 “.tables”来获得所有表的列表。或者，你可以输入 “.schema” 来看整个数据库模式，包括所有的表的索引。输入这些命令，后面跟一个LIKE模式匹配可以限制显示的表。
在一个 C/C++ 程序中（或者脚本语言使用 Tcl/Ruby/Perl/Python 等） 你可以在一个特殊的名叫 SQLITE_MASTER 上执行一个SELECT查询以获得所有 表的索引。每一个 SQLite 数据库都有一个叫 SQLITE_MASTER 的表， 它定义数据库的模式。 SQLITE_MASTER 表看起来如下：
CREATE TABLE sqlite_master (
type TEXT,
name TEXT,
tbl_name TEXT,
rootpage INTEGER,
sql TEXT
);
对于表来说，type 字段永远是 'table'，name 字段永远是表的名字。所以，要获得数据库中所有表的列表，使用下列SELECT语句：
SELECT name FROM sqlite_master
WHERE type='table'
ORDER BY name;
对于索引，type 等于 'index', name 则是索引的名字，tbl_name 是该索引所属的表的名字。不管是表还是索引，sql 字段是原先用 CREATE TABLE 或 CREATE INDEX 语句创建它们时的命令文本。对于自动创建的索引（用来实现 PRIMARY KEY 或 UNIQUE 约束），sql字段为NULL。
SQLITE_MASTER 表是只读的。不能对它使用 UPDATE、INSERT 或 DELETE。 它会被 CREATE TABLE、CREATE INDEX、DROP TABLE 和 DROP INDEX 命令自动更新。
临时表不会出现在 SQLITE_MASTER 表中。临时表及其索引和触发器存放在另外一个叫 SQLITE_TEMP_MASTER 的表中。SQLITE_TEMP_MASTER 跟 SQLITE_MASTER 差不多，但它只是对于创建那些临时表的应用可见。如果要获得所有表的列表， 不管是永久的还是临时的，可以使用类似下面的命令：
SELECT name FROM 
   (SELECT * FROM sqlite_master UNION ALL
    SELECT * FROM sqlite_temp_master)
WHERE type='table'
ORDER BY name
**在SQLite中，VARCHAR字段最长是多少？**
SQLite 不强制 VARCHAR 的长度。 你可以在 SQLITE 中声明一个 VARCHAR(10)，SQLite还是可以很高兴地允许你放入500个字符。 并且这500个字符是原封不动的，它永远不会被截断。
**SQLite****支持二进制大对象吗？**
SQLite 3.0 及以后版本允许你在任何列中存储 BLOB 数据。 即使该列被声明为其它类型也可以。
**在SQLite中，如何在一个表上添加或删除一列？**
SQLite 有有限地 ALTER TABLE 支持。你可以使用它来在表的末尾增加一列，可更改表的名称。 如果需要对表结构做更复杂的改变，则必须重新建表。重建时可以先将已存在的数据放到一个临时表中，删除原表， 创建新表，然后将数据从临时表中复制回来。
如，假设有一个 t1 表，其中有 "a", "b", "c" 三列， 如果要删除列 c ，以下过程描述如何做:
BEGIN TRANSACTION;
CREATE TEMPORARY TABLE t1_backup(a,b);
INSERT INTO t1_backup SELECT a,b FROM t1;
DROP TABLE t1;
CREATE TABLE t1(a,b);
INSERT INTO t1 SELECT a,b FROM t1_backup;
DROP TABLE t1_backup;
COMMIT;
**在数据库中删除了很多数据，但数据库文件没有变小，是Bug吗？**
不是。当你从SQLite数据库中删除数据时， 未用的磁盘空间将会加入一个内部的“自由列表”中。 当你下次插入数据时，这部分空间可以重用。磁盘空间不会丢失，但也不会返还给操作系统。
如果删除了大量数据，而又想缩小数据库文件占用的空间，执行 VACUUM 命令。 VACUUM 将会从头重新组织数据库。这将会使用数据库有一个空的“自由链表”， 数据库文件也会最小。但要注意的是，VACUUM 的执行会需要一些时间（在SQLite开发时，在Linux上，大约每M字节需要半秒种），并且， 执行过程中需要原数据库文件至多两倍的临时磁盘空间。
对于 SQLite 3.1版本，一个 auto-vacumm 模式可以替代 VACUUM 命令。 可以使用 auto_vacuum pragma 打开。
**SQLITE_SCHEMA error****是什么错误？为什么会出现该错误？**
当一个准备好的（prepared）SQL语句不再有效或者无法执行时， 将返回一个 SQLITE_SCHEMA 错误。发生该错误时，SQL语句必须使用 sqlite3_prepare() API来重新编译. 在 SQLite 3 中, 一个 SQLITE_SCHEMA 错误只会发生在用 sqlite3_prepare()/sqlite3_step()/sqlite3_finalize() API 执行 SQL 时。而不会发生在使用 sqlite3_exec()时。 在版本2中不是这样。
准备好的语句失效的最通常原因是：在语句准备好后， 数据库的模式又被修改了。另外的原因会发生在：
数据库离线：DETACHed. 
数据库被 VACUUMed 
一个用户存储过程定义被删除或改变。
一个 collation 序列定义被删除或改变。
认证函数被改变。
在所有情况下，解决方法是重新编译并执行该SQL语句。 因为一个已准备好的语句可以由于其它进程改变数据库模式而失效，所有使用 sqlite3_prepare()/sqlite3_step()/sqlite3_finalize() API 的代码都应准备处理 SQLITE_SCHEMA 错误。下面给出一个例子：
    int rc;
    sqlite3_stmt *pStmt;
    char zSql[] = "SELECT .....";
    do {
      /* Compile the statement from SQL. Assume success. */
      sqlite3_prepare(pDb, zSql, -1, &pStmt, 0);
      while( SQLITE_ROW==sqlite3_step(pStmt) ){
        /* Do something with the row of available data */
      }
      /* Finalize the statement. If an SQLITE_SCHEMA error has
      ** occured, then the above call to sqlite3_step() will have
      ** returned SQLITE_ERROR. sqlite3_finalize() will return
      ** SQLITE_SCHEMA. In this case the loop will execute again.
      */
      rc = sqlite3_finalize(pStmt);
    } while( rc==SQLITE_SCHEMA );
**如何在字符串中使用单引号(')？**
SQL 标准规定，在字符串中，单引号需要使用逃逸字符，即在一行中使用两个单引号。在这方面 SQL 用起来类似 Pascal 语言。 SQLite 尊循标准。如：
    INSERT INTO xyz VALUES('5 O''clock');
Sqlite中如何返回本地化当前时间？
在做ClinicOS的时候遇到一个问题，在保存病历登记时间时，我使用了“CURRENT_TIMESTAMP”，但这有个问题，它返回的是UTC Time，这对我们中国人没啥用，一直希望能想办法将它转为localtime。今天刚好有空，所以去查了查Sqlite的Mail List，果然也有人遇到了这个问题，我从一篇名为《translate time comparison statement》（http://www.mail-archive.com/sqlite-users@sqlite.org /msg12350.html）中看到这样的回复：
# 二十.如何更新表中数据
update contact set lastname=’江南七怪’where id = 1028
update contact set lastname='江南七怪', mobile='13912345678' where id=1028;
# 二十一.如何一次插入多个数据
Insert into SAMPLE(PRJNUM, PRJNAME, EMYNUM, EMYNAME, SALCATEGORY, SALPACKAGE)values(100001, 'TPMS', 200001, 'Johnson', 'A', 2000), (100001, 'TPMS', 200002,'Christine', 'B', 3000), (100001, 'TPMS', 200003, 'Kevin', 'C', 4000), (100002,'TCT', 200001, 'Johnson', 'A', 2000), (100002, 'TCT', 200004, 'Apple', 'B',3000);

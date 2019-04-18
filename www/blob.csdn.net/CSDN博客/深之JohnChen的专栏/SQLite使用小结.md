# SQLite使用小结 - 深之JohnChen的专栏 - CSDN博客

2010年08月28日 15:08:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：32941标签：[sqlite																[数据库](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=sqlite&t=blog)
个人分类：[数据库编程](https://blog.csdn.net/byxdaz/article/category/184468)


一、SQLite简介

SQLite是一款轻型的数据库，是遵守ACID的关联式数据库管理系统，它的设计目标是嵌入式的，而且目前已经在很多嵌入式产品中使用了它，它占用资源非常的低，在嵌入式设备中，可能只需要几百K的内存就够了。它能够支持Windows/Linux/Unix等等主流的操作系统，同时能够跟很多程序语言相结合，比如Tcl、PHP、Java等，还有ODBC接口，同样比起Mysql、PostgreSQL这两款开源世界著名的数据库管理系统来讲，它的处理速度比他们都快。

SQLite虽然很小巧，但是支持的SQL语句不会逊色于其他开源数据库，它支持的SQL包括：

ATTACH DATABASE 

BEGIN TRANSACTION 

comment 

COMMIT TRANSACTION 

COPY 

CREATE INDEX 

CREATE TABLE 

CREATE TRIGGER 

CREATE VIEW 

DELETE 

DETACH DATABASE 

DROP INDEX 

DROP TABLE 

DROP TRIGGER 

DROP VIEW 

END TRANSACTION 

EXPLAIN 

expression 

INSERT 

ON CONFLICT clause 

PRAGMA 

REPLACE 

ROLLBACK TRANSACTION 

SELECT 

UPDATE 

　　同时它还支持事务处理功能等等。也有人说它象Microsoft的Access，有时候真的觉得有点象，但是事实上它们区别很大。比如SQLite 支持跨平台，操作简单，能够使用很多语言直接创建数据库，而不象Access一样需要Office的支持。如果你是个很小型的应用，或者你想做嵌入式开发，没有合适的数据库系统，那么现在你可以考虑使用SQLite。目前它的最新版本是 3.2.2，它的官方网站是：http://www.sqlite.org或者http://www.sqlite.com.cn，能在上面获得源代码和文档。同时因为数据库结构简单，系统源代码也不是很多，也适合想研究数据库系统开发的专业人士

　　下面是访问SQLite官方网站: http://www.sqlite.org/ 时第一眼看到关于SQLite的特性. 

1. ACID事务

2. 零配置–无需安装和管理配置

3. 储存在单一磁盘文件中的一个完整的数据库

4. 数据库文件可以在不同字节顺序的机器间自由的共享

5. 支持数据库大小至2TB 

6. 足够小, 大致3万行C代码, 250K 

7. 比一些流行的数据库在大部分普通数据库操作要快

8. 简单, 轻松的API 

9. 包含TCL绑定, 同时通过Wrapper支持其他语言的绑定

10. 良好注释的源代码, 并且有着90%以上的测试覆盖率

11. 独立: 没有额外依赖

12. Source完全的Open, 你可以用于任何用途, 包括出售它

13. 支持多种开发语言,C, PHP, Perl, Java, ASP.NET,Python 

2、SQLite类型

SQLite的数据类型

　　首先你会接触到一个让你惊讶的名词: Typelessness(无类型). 对! SQLite是无类型的. 这意味着你可以保存任何类型的数据到你所想要保存的任何表的任何列中, 无论这列声明的数据类型是什么(只有在一种情况下不是, 稍后解释). 对于SQLite来说对字段不指定类型是完全有效的. 如: 

Create Table ex1(a, b, c); 

　　诚然SQLite允许忽略数据类型, 但是仍然建议在你的Create Table语句中指定数据类型. 因为数据类型对于你和其他的程序员交流, 或者你准备换掉你的数据库引擎. SQLite支持常见的数据类型, 如: 

CREATE TABLE ex2( 

a VARCHAR(10), 

b NVARCHAR(15), 

c TEXT, 

d INTEGER, 

e FLOAT, 

f BOOLEAN, 

g CLOB, 

h BLOB, 

i TIMESTAMP, 

j NUMERIC(10,5) 

k VARYING CHARACTER (24), 

l NATIONAL VARYING CHARACTER(16) 

); 

　　前面提到在某种情况下, SQLite的字段并不是无类型的. 即在字段类型为”Integer Primary Key”时. 

二**、SQLite的简单使用**

SQLite可以到官方站点下载[http://www.sqlite.org/download.html](http://www.sqlite.org/download.html)
包括：Linux,Mac OS X, Windows下的已编译文件以及源代码、帮助文档。windows版的下载地址是：http://www.sqlite.org/sqlite-3_6_23_1.zip，下载后解压就可以了。这个文件是工作在命令行下的。双击它，就直接进入命令行了。

打开之后是这样显示的：

SQLite version 3.6.23.1

Enter ".help" for instructions

Enter SQL statements terminated with a ";"

sqlite>

**1、创建数据库**

按理说第一步是创建一个数据库，我是学电子的，对计算机不了解，所以我不知道mysql是如何存储数据库的。但sqlite将一个数据库存储为一个文件。我们先进下cmd,关掉原来的sqlite3。

在命令提示符下：

sqlite3 newsql.db

然后就会自动跳到sqlite>命令提示符下。我记得在linux下用时候会在当前目录下出现newsql.db文件。但在我所用版本的windows下没有出现。然后我做了些尝试得到如下结果：

sqlite3 newsql.db

.quit

注意：.quit是在sqlite提示符下输入的，代表退出。看目录下还是没有出现数据库文件。

sqlite3 newsql.db

;

.quit

出现了newsql.db文件。冒号加回车，在sqlite中，代表执行一条语句的意思，虽然我只输入了一个冒号加回车，没有输入任何的语句，但结果已是不同。

**2、创建一个表**

create table mytable(entry1 varchar(10),entry2 int);

不要忘了加冒号。冒号代表一条语句输入完毕。

mytable是我创建的表名。create 和table都是系统关键字。entry1,entry2是两个表项。

varchar(10) int是类型。根据我读到的内容，sqlite是不区分类型，但是我们还是要在创建表时，给他一个类型，以便于将这些代码移植到其他的数据库里面时更加的方便。

**3、向表中插入一条记录**

insert into mytable values("hello world",10);

插入完了之后才发现是不是超出定义的大小了？我定义的entry1项是varchar(10)型的，说实在的，我不知这个类型确切来讲是什么意思，我猜应该是10个字符的字符串数组吧。如果那样的话我是一定超出了。但既然sqlite是不区分类型的，我想应该没有问题吧。于是我急于看看是不是这样...

**4、查询表中内容**

select * from mytable;

执行这条语句，会列出mytable中的所有内容。

结果为：

sqlite> select * from mytable;

hello world|10

可见还是都插入进去了。

sqlite> insert into mytable values("goodbye cruel world",20);

sqlite> select * from mytable;

hello world|10

goodbye cruel world|20

也就是说，第一个条目的字符串完全不受限制。

**5、sqlite3 newsql.db的规则**

我们现在退出，然后重新打开数据库：

sqlite> .quit

F:/sqlite>sqlite3 newsql.db

SQLite version 3.6.23.1

Enter ".help" for instructions

Enter SQL statements terminated with a ";"

sqlite> select * from mytable;

hello world|10

goodbye cruel world|20

可见sqlite3 newsql.db这个命令规则为：打开名为newsql.db这个数据库，如果不存在则创建一个。

**6、查询一个数据库中所有的表名**

sqlite数据库中有一个系统建立的表，名为sqlite_master，查询这个表可以得到所有的表。

sqlite> create table my2ndtable(theonlyentry int);

sqlite> insert into my2ndtable values(30);

sqlite> select * from sqlite_master;

table|mytable|mytable|2|CREATE TABLE mytable(entry1 varchar(10),entry2 int)

table|my2ndtable|my2ndtable|3|CREATE TABLE my2ndtable(theonlyentry int)

对于这个表的定义，官方网站的FAQ中给出如下：

CREATE TABLE sqlite_master ( type TEXT, name TEXT, tbl_name TEXT, rootpage INTEGER, sql TEXT );第一个字段类型显然会一直是table,第二个字段是名称分别是mytable和my2ndtable，见上面的结果。第三个字段表名，没弄懂是什么意，想必是所在的表的名字，但是一个表的名字和所在的表名不是一样的吗？第四个字段rootpage，我也不知指什么，这个系统的学过数据库的人应该能知道，有路过的还望告之。第五个字段是创建表的使用的sql语句吧。

**7、sqlite的输出模式**

默认的输出格式是“列表”。在列表模式下，每条查询结果记录被写在一行中并且每列之间以一个字符串分割符隔开。默认的分隔符是一个管道符号（“|”）。列表符号在当你输出查询结果到另外一个符号处理的程序（如AWK）中去是尤为有用。

sqlite> .mode list

sqlite> select * from mytable;

hello world|10

goodbye cruel world|20

sqlite>

这是正常的模式。

sqlite> .mode csv

sqlite> select * from mytable;

"hello world",10

"goodbye cruel world",20

变化是什么？字符串被加上了引号。

sqlite> .mode column

sqlite> select * from mytable;

hello world  10

goodbye cru  20

哎呀，第二个字符串被截断了。

.mode MODE ?TABLE?__  Set output mode where MODE is one of:

____________  csv___ Comma-separated values

____________  column_  Left-aligned columns._ (See .width)

____________  html__  HTML <table> code

____________  insert_  SQL insert statements for TABLE

____________  line__  One value per line

____________  list__  Values delimited by .separator string

____________  tabs__  Tab-separated values

____________  tcl___ TCL list elements

这些来自.help命令的输出结果。

sqlite> .mode html

sqlite> select * from mytable;

<TR><TD>hello world</TD>

<TD>10</TD>

</TR>

<TR><TD>goodbye cruel world</TD>

<TD>20</TD>

</TR>

**.mode html是一种较新的输出方法。**

**另外，我们也可以把输出结果输出到文件：**

sqlite> .output output.txt

sqlite> select * from mytable;

sqlite> .exit

F:/sqlite>type output.txt

hello world|10

goodbye cruel world|20

**8、查看数据库中所有的表**

sqlite> .tables

my2ndtable  mytable

**9、查看所有的表的创建语句**

sqlite> .schema

CREATE TABLE my2ndtable(theonlyentry int);

CREATE TABLE mytable(entry1 varchar(10),entry2 int);

sqlite> .schema mytable

CREATE TABLE mytable(entry1 varchar(10),entry2 int);

**10、数据库导出和导入**

我们可以利用这个功能做一个简单的备份，或是说创建一个同样的数据库。

第一步，把数据库倒出来：

cmd命令提示符下：

F:/sqlite>sqlite3 newsql.db ".dump" >a.sql

此语句将数据库导出成a.sql数据库语句文件，执行这个文件就可以创建一个一模一样数据库：

F:/sqlite>sqlite3 copied.db

SQLite version 3.6.23.1

Enter ".help" for instructions

Enter SQL statements terminated with a ";"

sqlite> select * from mytable;

hello world|10

goodbye cruel world|20

11、删除记录
delete from mytable where entry2=10;
可以删掉mytable中所有entry2项为10的条目。
注意：不是delete * from mytable,而delete from mytable.没有*.

**三、SQLite3 C/C++ 开发接口简介（API函数）**

SQLite3是SQLite一个全新的版本,它虽然是在SQLite 2.8.13的代码基础之上开发的,但是使用了和之前的版本不兼容的数据库格式和API. SQLite3是为了满足以下的需求而开发的: 
- 支持UTF-16编码. 
- 用户自定义的文本排序方法. 
- 可以对BLOBs字段建立索引.

因此为了支持这些特性我改变了数据库的格式,建立了一个与之前版本不兼容的3.0版. 至于其他的兼容性的改变,例如全新的API等等,都将在理论介绍之后向你说明,这样可以使你最快的一次性摆脱兼容性问题. 

3.0版的和2.X版的API非常相似,但是有一些重要的改变需要注意. 所有API接口函数和数据结构的前缀都由"sqlite_"改为了"sqlite3_". 这是为了避免同时使用SQLite 2.X和SQLite 3.0这两个版本的时候发生链接冲突. 

由于对于C语言应该用什么数据类型来存放UTF-16编码的字符串并没有一致的规范. 因此SQLite使用了普通的void* 类型来指向UTF-16编码的字符串. 客户端使用过程中可以把void*映射成适合他们的系统的任何数据类型. 

SQLite 3.0一共有83个API函数,此外还有一些数据结构和预定义(#defines). (完整的API介绍请参看另一份文档.) 不过你们可以放心,这些接口使用起来不会像它的数量所暗示的那么复杂. 最简单的程序仍然使用三个函数就可以完成: sqlite3_open(), sqlite3_exec(), 和 sqlite3_close(). 要是想更好的控制数据库引擎的执行,可以使用提供的sqlite3_prepare()函数把SQL语句编译成字节码,然后在使用sqlite3_step()函数来执行编译后的字节码. 以sqlite3_column_开头的一组API函数用来获取查询结果集中的信息. 许多接口函数都是成对出现的,同时有UTF-8和UTF-16两个版本. 并且提供了一组函数用来执行用户自定义的SQL函数和文本排序函数. 

#### *2.1 如何打开关闭数据库*
typedef struct sqlite3 sqlite3;int sqlite3_open(const char*, sqlite3**);int sqlite3_open16(const void*, sqlite3**);int sqlite3_close(sqlite3*);const char *sqlite3_errmsg(sqlite3*);const void *sqlite3_errmsg16(sqlite3*);int sqlite3_errcode(sqlite3*);
sqlite3_open() 函数返回一个整数错误代码,而不是像第二版中一样返回一个指向sqlite3结构体的指针. sqlite3_open() 和 sqlite3_open16() 的不同之处在于sqlite3_open16() 使用UTF-16编码(使用本地主机字节顺序)传递数据库文件名. 如果要创建新数据库, sqlite3_open16() 将内部文本转换为UTF-16编码, 反之sqlite3_open() 将文本转换为UTF-8编码. 

打开或者创建数据库的命令会被缓存,直到这个数据库真正被调用的时候才会被执行. 而且允许使用PRAGMA声明来设置如本地文本编码或默认内存页面大小等选项和参数. 

sqlite3_errcode() 通常用来获取最近调用的API接口返回的错误代码. sqlite3_errmsg() 则用来得到这些错误代码所对应的文字说明. 这些错误信息将以 UTF-8 的编码返回,并且在下一次调用任何SQLite API函数的时候被清除. sqlite3_errmsg16() 和 sqlite3_errmsg() 大体上相同,除了返回的错误信息将以 UTF-16 本机字节顺序编码. 

SQLite3的错误代码相比SQLite2没有任何的改变,它们分别是: 
#define SQLITE_OK0/* Successful result */#define SQLITE_ERROR1/* SQL error or missing database */#define SQLITE_INTERNAL2/* An internal logic error in SQLite */#define SQLITE_PERM3/* Access permission denied */#define SQLITE_ABORT4/* Callback routine requested an abort */#define SQLITE_BUSY5/* The database file is locked */#define SQLITE_LOCKED6/* A table in the database is locked */#define SQLITE_NOMEM7/* A malloc() failed */#define SQLITE_READONLY8/* Attempt to write a readonly database */#define SQLITE_INTERRUPT9/* Operation terminated by sqlite_interrupt() */#define SQLITE_IOERR10/* Some kind of disk I/O error occurred */#define SQLITE_CORRUPT11/* The database disk image is malformed */#define SQLITE_NOTFOUND12/* (Internal Only) Table or record not found */#define SQLITE_FULL13/* Insertion failed because database is full */#define SQLITE_CANTOPEN14/* Unable to open the database file */#define SQLITE_PROTOCOL15/* Database lock protocol error */#define SQLITE_EMPTY16/* (Internal Only) Database table is empty */#define SQLITE_SCHEMA17/* The database schema changed */#define SQLITE_TOOBIG18/* Too much data for one row of a table */#define SQLITE_CONSTRAINT19/* Abort due to contraint violation */#define SQLITE_MISMATCH20/* Data type mismatch */#define SQLITE_MISUSE21/* Library used incorrectly */#define SQLITE_NOLFS22/* Uses OS features not supported on host */#define SQLITE_AUTH23/* Authorization denied */#define SQLITE_ROW100/* sqlite_step() has another row ready */#define SQLITE_DONE101/* sqlite_step() has finished executing */
2.2 执行 SQL 语句

       typedef int (*sqlite_callback)(void*,int,char**, char**);
       int sqlite3_exec(sqlite3*, const char *sql, sqlite_callback, void*, char**);

sqlite3_exec 函数依然像它在SQLite2中一样承担着很多的工作. 该函数的第二个参数中可以编译和执行零个或多个SQL语句. 查询的结果返回给回调函数. 更多地信息可以查看API 参考.

在SQLite3里,sqlite3_exec一般是被准备SQL语句接口封装起来使用的.

       typedef struct sqlite3_stmt sqlite3_stmt;
       int sqlite3_prepare(sqlite3*, const char*, int, sqlite3_stmt**, const char**);
       int sqlite3_prepare16(sqlite3*, const void*, int, sqlite3_stmt**, const void**);
       int sqlite3_finalize(sqlite3_stmt*);
       int sqlite3_reset(sqlite3_stmt*);

sqlite3_prepare 接口把一条SQL语句编译成字节码留给后面的执行函数. 使用该接口访问数据库是当前比较好的的一种方法.

sqlite3_prepare() 处理的SQL语句应该是UTF-8编码的. 而sqlite3_prepare16() 则要求是UTF-16编码的. 输入的参数中只有第一个SQL语句会被编译. 第四个参数则用来指向输入参数中下一个需要编译的SQL语句存放的SQLite statement对象的指针, 任何时候如果调用 sqlite3_finalize() 将销毁一个准备好的SQL声明. 在数据库关闭之前，所有准备好的声明都必须被释放销毁. sqlite3_reset() 函数用来重置一个SQL声明的状态，使得它可以被再次执行.

SQL声明可以包含一些型如"?" 或 "?nnn" 或 ":aaa"的标记，其中"nnn" 是一个整数，"aaa" 是一个字符串. 这些标记代表一些不确定的字符值（或者说是通配符），可以在后面用sqlite3_bind 接口来填充这些值. 每一个通配符都被分配了一个编号（由它在SQL声明中的位置决定，从1开始），此外也可以用 "nnn" 来表示 "?nnn" 这种情况. 允许相同的通配符在同一个SQL声明中出现多次, 在这种情况下所有相同的通配符都会被替换成相同的值. 没有被绑定的通配符将自动取NULL值.

       int sqlite3_bind_blob(sqlite3_stmt*, int, const void*, int n, void(*)(void*));
       int sqlite3_bind_double(sqlite3_stmt*, int, double);
       int sqlite3_bind_int(sqlite3_stmt*, int, int);
       int sqlite3_bind_int64(sqlite3_stmt*, int, long long int);
       int sqlite3_bind_null(sqlite3_stmt*, int);
       int sqlite3_bind_text(sqlite3_stmt*, int, const char*, int n, void(*)(void*));
       int sqlite3_bind_text16(sqlite3_stmt*, int, const void*, int n, void(*)(void*));
       int sqlite3_bind_value(sqlite3_stmt*, int, const sqlite3_value*);

以上是 sqlite3_bind 所包含的全部接口，它们是用来给SQL声明中的通配符赋值的. 没有绑定的通配符则被认为是空值. 绑定上的值不会被sqlite3_reset()函数重置. 但是在调用了sqlite3_reset()之后所有的通配符都可以被重新赋值.

在SQL声明准备好之后(其中绑定的步骤是可选的), 需要调用以下的方法来执行:

       int sqlite3_step(sqlite3_stmt*);

如果SQL返回了一个单行结果集，sqlite3_step() 函数将返回 SQLITE_ROW , 如果SQL语句执行成功或者正常将返回 SQLITE_DONE , 否则将返回错误代码. 如果不能打开数据库文件则会返回 SQLITE_BUSY . 如果函数的返回值是 SQLITE_ROW, 那么下边的这些方法可以用来获得记录集行中的数据:

       const void *sqlite3_column_blob(sqlite3_stmt*, int iCol);
       int sqlite3_column_bytes(sqlite3_stmt*, int iCol);
       int sqlite3_column_bytes16(sqlite3_stmt*, int iCol);
       int sqlite3_column_count(sqlite3_stmt*);
       const char *sqlite3_column_decltype(sqlite3_stmt *, int iCol);
       const void *sqlite3_column_decltype16(sqlite3_stmt *, int iCol);
       double sqlite3_column_double(sqlite3_stmt*, int iCol);
       int sqlite3_column_int(sqlite3_stmt*, int iCol);
       long long int sqlite3_column_int64(sqlite3_stmt*, int iCol);
       const char *sqlite3_column_name(sqlite3_stmt*, int iCol);
       const void *sqlite3_column_name16(sqlite3_stmt*, int iCol);
       const unsigned char *sqlite3_column_text(sqlite3_stmt*, int iCol);
       const void *sqlite3_column_text16(sqlite3_stmt*, int iCol);
       int sqlite3_column_type(sqlite3_stmt*, int iCol);

sqlite3_column_count()函数返回结果集中包含的列数. sqlite3_column_count() 可以在执行了 sqlite3_prepare()之后的任何时刻调用. sqlite3_data_count()除了必需要在sqlite3_step()之后调用之外，其他跟sqlite3_column_count() 大同小异. 如果调用sqlite3_step() 返回值是 SQLITE_DONE 或者一个错误代码, 则此时调用sqlite3_data_count() 将返回 0 ，然而 sqlite3_column_count() 仍然会返回结果集中包含的列数.

返回的记录集通过使用其它的几个 sqlite3_column_***() 函数来提取, 所有的这些函数都把列的编号作为第二个参数. 列编号从左到右以零起始. 请注意它和之前那些从1起始的参数的不同.

sqlite3_column_type()函数返回第N列的值的数据类型. 具体的返回值如下:

       #define SQLITE_INTEGER  1
       #define SQLITE_FLOAT    2
       #define SQLITE_TEXT     3
       #define SQLITE_BLOB     4
       #define SQLITE_NULL     5

sqlite3_column_decltype() 则用来返回该列在 CREATE TABLE 语句中声明的类型. 它可以用在当返回类型是空字符串的时候. sqlite3_column_name() 返回第N列的字段名. sqlite3_column_bytes() 用来返回 UTF-8 编码的BLOBs列的字节数或者TEXT字符串的字节数. sqlite3_column_bytes16() 对于BLOBs列返回同样的结果，但是对于TEXT字符串则按 UTF-16 的编码来计算字节数. sqlite3_column_blob() 返回 BLOB 数据. sqlite3_column_text() 返回 UTF-8 编码的 TEXT 数据. sqlite3_column_text16() 返回 UTF-16 编码的 TEXT 数据. sqlite3_column_int() 以本地主机的整数格式返回一个整数值. sqlite3_column_int64() 返回一个64位的整数. 最后, sqlite3_column_double() 返回浮点数.

不一定非要按照sqlite3_column_type()接口返回的数据类型来获取数据. 数据类型不同时软件将自动转换. 

2.3核心对象和接口

SQL数据库引擎的最主要任务是解析SQL语句。为了达成这个目的，开发者需要了解两个对象：

* 数据库连接对象：sqlite3

* 预处理语句对象：sqlite3_stmt

严格来讲，预处理语句对象并不是必须的，因为能够使用sqlite_exec或者sqlite3_get_table这些便于使用的封装接口，而这些接口封装并隐藏了预处理语句对象。尽管如此，对预处理对象的理解有助于我们更充分的使用SQLite。

数据库连接对象和预处理对象是由下列的一组C/C++接口调用操纵的：

* sqlite3_open()

* sqlite3_prepare()

* sqlite3_step()

* sqlite3_column()

* sqlite3_finalize()

* sqlite3_close()

这6个C/C++接口例程和上述的两个对象构成了SQLite的核心功能。开发者对于它们的理解能够更好的使用SQLite。

注意，这个接口例程列表更多是概念上的意义而不是实际的接口。许多这些接口都出现在各个版本之中。例如，上述列表中的sqlite3_open() 例程实际上有三个不同的接口以略微不同的方式实现相同的功能：slqite3_open()，sqlite3_open16()和 sqlite3_open_v2()。列表中的实际上并不存在sqlite3_column()这个接口。显示在列表中的“sqlite3_column()”仅仅是一个占位，表示一整套用于从表中查询出各种数据类型的列记录接口。

这里说明下核心接口的主要功能：

* sqlite3_open() 该接口打开与一个SQLite数据库文件的连接并返回一个数据库连接对象。这通常是应用程序调用的第一个SQLite API接口而且也是调用其他SQLite API接口前需要调用的接口。许多SQLite接口需要一个指向数据库连接对象的指针作为它们的第一个参数，因而这些接口也可以理解成是数据库连接对象的操作接口。该接口就是创建了这样一个数据库连接对象。

* sqlite3_prepare() 该接口把一个SQL语句文本转换成一个预处理语句对象并返回一个指向该对象的指针。这个接口需要一个由先前调用sqlite3_open()返回的数据库连接对象指针以及一个预处理的SQL语句文本字符串为参数。这个API并不实际解析SQL语句，仅仅是为后续的解析而对SQL语句进行的预处理。

注意新的应用中不建议使用sqlite3_prepare()，而应该使用另一个接口sqlite3_prepare_v2()。

* sqlite3_step() 该接口用于解析一个由先前通过sqlite3_prepare()接口创建的预处理语句，直至返回第一列结果为止。通过再次调用 sqlite3_step()可以返回下一列的结果，继续不断地调用sqlite3_step()直至整个语句完成为止。对于那些并不返回结果的语句(例如：INSERT，UPDATE，DELETE语句)一次调用sqlite3_step()就完成了语句的处理。

* sqlite3_column() 该接口返回一个由sqlite3_step()解析的预处理语句结果集中当前行的某一列数据。每次执行sqlite3_step()都返回一个新的结果集中的一行。可以多次调用sqlite3_column()接口返回那一行中所有列的数据。就像上面所说的那样，SQLite API中并没有sqlite3_column()这样的接口。取而代之的是一组用于从结果集中查询出各个列项各种数据类型数据的函数接口。在这组函数接口中，有些接口返回结果集的大小，有些返回结果集的列数。

＊sqlite3_column_blob()

＊sqlite3_column_bytes()

＊sqlite3_column_bytes16()

＊sqlite3_column_count()

＊sqlite3_column_double()

＊sqlite3_column_int()

＊sqlite3_column_int64()

＊sqlite3_column_text()

＊sqlite3_column_text16()

＊sqlite3_column_type()

＊sqlite3_column_value()

* sqlite3_finalize() 该接口销毁之前调用sqlite3_prepare()创建的预处理语句。每一个预处理语句都必须调用这个接口进行销毁以避免内存泄

漏。

* sqlite3_close() 该接口关闭一个由之前调用sqlite3_open()创建的数据库连接。所有与该连接相关的预处理语句都必须在关闭连接之前销毁。

2.4扩展SQLite

SQLite包含一些可用于扩展其功能的一些其他接口，这些接口包括：

＊ sqlite3_create_collation()

＊ sqlite3_create_function()

＊ sqlite3_create_module()

sqlite3_create_collation()接口用于为索引文本创建新的对照序列。sqlite3_create_module()接口用于注册新的续表实现接口。

sqlite3_create_function()接口创建新的SQL函数-即可以是单一的也可以是组合的接口。新的函数实现通常利用下列的辅助接口：

＊ sqlite3_aggregate_context()

＊ sqlite3_result()

＊ sqlite3_user_data()

＊ sqlite3_value()

SQLite中所有内建的SQL函数接口也是通过这些相同的接口实现的。查看SQLite源代码，尤其是date.c和func.c两个文件就有许多这方面的例子。

四、在VC下使用SQLite

#include "../sqlite3_lib/sqlite3.h"

#pragma comment(lib, "../sqlite3_lib/sqlite3.lib")

static int _sql_callback(void * notused, int argc, char ** argv, char ** szColName)

{

int i;

for ( i=0; i < argc; i++ )

{

printf( "%s = %s/n", szColName[i], argv[i] == 0 ? "NUL" : argv[i] );

}

return 0;

}

int main(int argc, char * argv[])

{

const char * sSQL1 = "create table users(userid varchar(20) PRIMARY KEY, age int, birthday datetime);";

const char * sSQL2 = "insert into users values('wang',20,'1989-5-4');";

const char * sSQL3 = "select * from users;";

sqlite3 * db = 0;

char * pErrMsg = 0;

int ret = 0;

// 连接数据库

ret = sqlite3_open("./test.db", &db);

if ( ret != SQLITE_OK )

{

fprintf(stderr, "无法打开数据库: %s", sqlite3_errmsg(db));

return(1);

}

printf("数据库连接成功!/n");

// 执行建表SQL

sqlite3_exec( db, sSQL1, 0, 0, &pErrMsg );

if ( ret != SQLITE_OK )

{

fprintf(stderr, "SQL error: %s/n", pErrMsg);

sqlite3_free(pErrMsg);

}

// 执行插入记录SQL

sqlite3_exec( db, sSQL2, 0, 0, &pErrMsg);

// 查询数据表

sqlite3_exec( db, sSQL3, _sql_callback, 0, &pErrMsg);

// 关闭数据库

sqlite3_close(db);

db = 0;

return 0;

}

#### *使用事务*
在上面的例子中，我们向数据库里插入了10条数据，然后再从数据库里读出来。细心的你不知道有没有发现，在执行Insert的时候，并没有使用同一个事 务。在很多情况下，我们需要使用事务来保证对数据库操作的原子性。Sqlite是支持事务的，而且对事务的使用非常简单：使用sql语句”begin;” 表示事务开始，”rollback;”表示事务的回滚，”commit;”表示事务的提交。下面我们对上面例子中的代码作一下修改，给Insert操作添 加事务支持：//... ...

bool is_succed = true;

sqlite3_exec(conn, "begin;", 0, 0, 0);// 
开启事务//添加10条记录for (int i = 0; i < 10; i++)

{

// 执行SQL

sprintf(sql, "INSERT INTO [test_for_cpp] ([id], [name], [age]) /

VALUES (%d, '%s', %d)", i, "JGood", i);

if (SQLITE_OK != sqlite3_exec(conn, sql, 0, 0, &err_msg))

{

is_succed = false;

break;

}

}

if (is_succed)

sqlite3_exec(conn, "commit;", 0, 0, 0);// 提交事务else

sqlite3_exec(conn, "rollback;", 0, 0, 0);// 回滚事务//... ...

使用sql参数
基本上，使用sqlite3_open, sqlite3_close, sqlite3_exec这三个函数，可以完成大大部分的工作。但还不完善。上面的例子中，都是直接以sql语句的形式来操作数据库，这样很容易被注入。所以有必要使用sql参数。**sqlite3_prepare****sqlite3_bind_*****sqlite3_step****sqlite3_column_*****struct sqlite3_stmt****sqlite3_finalize**    sqlite3_prepare用来编译sql语句。sql语句被执行之前，必须先编译成字节码。sqlite3_stmt是一个结构体，表示sql语句 编译后的字节码。sqlite3_step用来执行编译后的sql语句。sqlite3_bind_*用于将sql参数绑定到sql语句。 sqlite3_column_*用于从查询的结果中获取数据。sqlite3_finalize用来释放sqlite3_stmt对象。代码最能说明函 数的功能，下面就用一个例子来演示吧~~// ----------------------------------------------

// [http://blog.csdn.net/JGood](http://blog.csdn.net/JGood)// sqlite3_prepare, sqlite3_bind_*, sqlite3_step, sqlite3_column_*, sqlite3_column_type

// sqlite3_stmt, sqlite3_finalize, sqlite3_reset

// 查询// ----------------------------------------------

sqlite3 *conn = NULL;

sqlite3_stmt *stmt = NULL;

const char *err_msg = NULL;

// 列数据类型char col_types[][10] = { "", "Integer", "Float", "Text", "Blob", "NULL" };

sqlite3_open("test.db", &conn);

sqlite3_prepare(conn, "SELECT * FROM [test_for_cpp] WHERE [id]>?", -1, &stmt, &err_msg);

sqlite3_bind_int(stmt, 1, 5);

while (SQLITE_ROW == sqlite3_step(stmt))

{

int col_count = sqlite3_column_count(stmt); // 结果集中列的数量const char *col_0_name = sqlite3_column_name(stmt, 0); // 获取列名int id = sqlite3_column_int(stmt, 0);

int id_type = sqlite3_column_type(stmt, 0); // 获取列数据类型const char *col_2_name = sqlite3_column_name(stmt, 2);

int age = sqlite3_column_int(stmt, 2);

int age_type = sqlite3_column_type(stmt, 2);

const char *col_1_name = sqlite3_column_name(stmt, 1);

char name[80];

strncpy(name, (const char *)sqlite3_column_text(stmt, 1), 80);

int name_type = sqlite3_column_type(stmt, 1);

// 打印结果printf("col_count: %d, %s = %d(%s), %s = %s(%s), %s = %d(%s)/n",

col_count, col_0_name, id, col_types[id_type], col_2_name, name,

col_types[name_type], col_1_name, age, col_types[age_type]);

}

sqlite3_finalize(stmt); // 释放sqlite3_stmt 

sqlite3_close(conn);这段代码查询id号大于5的所有记录.[](https://p-blog.csdn.net/images/p_blog_csdn_net/JGood/601714/o_img_1.jpg)*其他函数*在上面的例子中，还使用了其他的一些函数，如：sqlite3_column_count用于获取结果集中列的数 量；sqlite3_column_name用于获取列的名称；sqlite3_column_type用于获取列的数据类 型；sqlite3_errcode用于获取最近一次操作出错的错误代码；sqlite3_errmsg用于获取最近一次操作出错的错误说明。 sqlite的api中还有很多的函数，有了上面的基础，
相信你通过查询官方的文档，能迅速掌握本文未介绍的api。

五、数据库管理工具

sqlite 管理工具 Sqlite Developer

Sqlite Developer是SharpPlus出品的一款强大数据库管理软件。支持对sqlite3数据库的管理。

下载：[http://dl.pconline.com.cn/download/60827.html](http://dl.pconline.com.cn/download/60827.html)

 Sqlite Developer 注册码[：http://download.csdn.net/source/2755962](http://download.csdn.net/source/2755962)

常见问题：

1、创建数据
如果不往数据库里面添加任何的表，这个数据库等于没有建立，不会在硬盘上产生任何文件，如果数据库已经存在，则会打开这个数据库。

2、如何通过sqlite3.dll与sqlite3.def生成sqlite3.lib文件
LIB /DEF:sqlite3.def /machine:IX86

3、如何查询当前的编码的编码格式
pragma  encoding;

4、如果删除了大量数据，而又想缩小数据库文件占用的空间，执行 VACUUM 命令
vacuum;

5、sqlite3_open打开一个数据库时，如果数据库不存在就会新生成一个数据库文件。如果接着执行其他查询语句就会失败，比如sqlite3_prepare，编程中出现明明指定了数据库而且里面也有数据，为什么查询失败了，主要是数据库名路径不对引起的。一般的做法是先检查数据库文件是否存在，如果存在就使用sqlite3_open打开数据库；否则创建一个新的数据库。

6、如何建立自动增长字段

声明为 INTEGER PRIMARY KEY 的列将会自动增长。

7、SQLite3支持何种数据类型？

NULL 

INTEGER 

REAL 

TEXT 

BLOB 

但实际上，sqlite3也接受如下的数据类型：

smallint 16 位元的整数。

interger 32 位元的整数。

decimal(p,s) p 精确值和 s 大小的十进位整数，精确值p是指全部有几个数(digits)大小值，s是指小数点後有几位数。如果没有特别指定，则系统会设为 p=5; s=0 。

float 32位元的实数。

double 64位元的实数。

char(n) n 长度的字串，n不能超过 254。

varchar(n) 长度不固定且其最大长度为 n 的字串，n不能超过 4000。

graphic(n) 和 char(n) 一样，不过其单位是两个字元 double-bytes， n不能超过127。这个形态是为了支援两个字元长度的字体，例如中文字。

vargraphic(n) 可变长度且其最大长度为 n 的双字元字串，n不能超过 2000。

date 包含了年份、月份、日期。

time 包含了小时、分钟、秒。

timestamp 包含了年、月、日、时、分、秒、千分之一秒。

8、SQLite允许向一个integer型字段中插入字符串

这是一个特性，而不是一个bug。SQLite不强制数据类型约束。任何数据都可以插入任何列。你可以向一个整型列中插入任意长度的字符串，向布尔型列中插入浮点数，或者向字符型列中插入日期型值。在 CREATE TABLE 中所指定的数据类型不会限制在该列中插入任何数据。任何列均可接受任意长度的字符串（只有一种情况除外：标志为INTEGER PRIMARY KEY的列只能存储64位整数，当向这种列中插数据除整数以外的数据时，将会产生错误。

但SQLite确实使用声明的列类型来指示你所期望的格式。所以，例如你向一个整型列中插入字符串时，SQLite会试图将该字符串转换成一个整数。如果可以转换，它将插入该整数；否则，将插入字符串。这种特性有时被称为类型或列亲和性(type or column affinity).

9、为什么SQLite不允许在同一个表不同的两行上使用0和0.0作主键？

主键必须是数值类型，将主键改为TEXT型将不起作用。

每一行必须有一个唯一的主键。对于一个数值型列， SQLite认为 '0' 和 '0.0' 是相同的，因为他们在作为整数比较时是相等的(参见上一问题)。所以，这样值就不唯一了。

10、多个应用程序或一个应用程序的多个实例可以同时访问同一个数据库文件吗？

多个进程可同时打开同一个数据库。多个进程可以同时进行SELECT 操作，但在任一时刻，只能有一个进程对数据库进行更改。

SQLite 使用读、写锁控制对数据库的访问。（在Win95/98/ME等不支持读、写锁的系统下，使用一个概率性的模拟来代替。）但使用时要注意：如果数据库文件存放于一个NFS文件系统上，这种锁机制可能不能正常工作。这是因为 fcntl() 文件锁在很多NFS上没有正确的实现。在可能有多个进程同时访问数据库的时候，应该避免将数据库文件放到NFS上。在Windows上，Microsoft的文档中说：如果使用 FAT 文件系统而没有运行 share.exe 守护进程，那么锁可能是不能正常使用的。那些在Windows上有很多经验的人告诉我：对于网络文件，文件锁的实现有好多Bug，是靠不住的。如果他们说的是对的，那么在两台或多台Windows机器间共享数据库可能会引起不期望的问题。

我们意识到，没有其它嵌入式的 SQL 数据库引擎能象 SQLite 这样处理如此多的并发。SQLite允许多个进程同时打开一个数据库，同时读一个数据库。当有任何进程想要写时，它必须在更新过程中锁住数据库文件。但那通常只是几毫秒的时间。其它进程只需等待写进程干完活结束。典型地，其它嵌入式的SQL数据库引擎同时只允许一个进程连接到数据库。

但是，Client/Server数据库引擎（如 PostgreSQL, MySQL, 或 Oracle）通常支持更高级别的并发，并且允许多个进程同时写同一个数据库。这种机制在Client/Server结构的数据库上是可能的，因为总是有一个单一的服务器进程很好地控制、协调对数据库的访问。如果你的应用程序需要很多的并发，那么你应该考虑使用一个Client/Server 结构的数据库。但经验表明，很多应用程序需要的并发，往往比其设计者所想象的少得多。

当SQLite试图访问一个被其它进程锁住的文件时，缺省的行为是返回 SQLITE_BUSY。可以在C代码中使用 sqlite3_busy_handler() 或 sqlite3_busy_timeout() API 函数调整这一行为。

11、SQLite线程安全吗？

线程是魔鬼（Threads are evil）。避免使用它们。

SQLite 是线程安全的。由于很多用户会忽略我们在上一段中给出的建议，我们做出了这种让步。但是，为了达到线程安全，SQLite在编译时必须将 SQLITE_THREADSAFE 预处理宏置为1。在Windows和Linux上，已编译的好的二进制发行版中都是这样设置的。如果不确定你所使用的库是否是线程安全的，可以调用 sqlite3_threadsafe() 接口找出。

12、在SQLite数据库中如何列出所有的表和索引？

如果你运行 sqlite3 命令行来访问你的数据库，可以键入“.tables”来获得所有表的列表。或者，你可以输入“.schema”来看整个数据库模式，包括所有的表的索引。输入这些命令，后面跟一个LIKE模式匹配可以限制显示的表。

13、SQLite数据库有已知的大小限制吗？

在 Windows 和 Unix 下，版本 2.7.4的 SQLite 可以达到 2的41次方字节 (2T 字节)。老版本的为 2的31 次方字节(2G 字节)。

SQLite 版本 2.8 限制一个记录的容量为 1M。SQLite 版本 3.0 则对单个记录容量没有限制。

表名、索引表名、视图名、触发器名和字段名没有长度限制。但 SQL 函数的名称 (由 sqlite3_create_function() API 函数创建) 不得超过 255 个字符。

14、在SQLite中，VARCHAR字段最长是多少？

SQLite 不强制 VARCHAR 的长度。你可以在 SQLITE 中声明一个 VARCHAR(10)，SQLite还是可以很高兴地允许你放入500个字符。并且这500个字符是原封不动的，它永远不会被截断。

15、在SQLite中，如何在一个表上添加或删除一列？

SQLite 有有限地 ALTER TABLE 支持。你可以使用它来在表的末尾增加一列，可更改表的名称。如果需要对表结构做更复杂的改变，则必须重新建表。重建时可以先将已存在的数据放到一个临时表中，删除原表，创建新表，然后将数据从临时表中复制回来。

如，假设有一个 t1 表，其中有 "a", "b", "c" 三列，如果要删除列 c ，以下过程描述如何做: 

BEGIN TRANSACTION; 

CREATE TEMPORARY TABLE t1_backup(a,b); 

INSERT INTO t1_backup SELECT a,b FROM t1; 

DROP TABLE t1; 

CREATE TABLE t1(a,b); 

INSERT INTO t1 SELECT a,b FROM t1_backup; 

DROP TABLE t1_backup; 

COMMIT; 

16、如何在Windows下编译SQLite3
1)先从官网获取sqlite-amalgamation-3_6_22.zip；

2)接着从官网获取sqlitedll-3_6_22.zip

下载编译好的DLL，这里有我们需要的sqlite3.def该文件

3)打开VC新建一个“Win32 Dynamic-Link Library”工程，命名为:sqlite3；

4)在接下来的对话框中选择"An empty DLL project",点 FINISH->OK；

5)将sqlite-amalgamation-3_6_22文件夹下的sqlite3.h以及sqlite3.c两个文件复制到工程文件夹下；

将sqlitedll-3_6_22文件夹下的sqlite3.def该文件复制到工程文件夹下；

6)在工程的Source Files中添加sqlite3.c文件；

7)在工程的Include Files中添加sqlite3.h文件；

8)在工程的Resource Files中添加sqlite3.def文件；

9)针对如下问题：

sqlite3.def : error LNK2001: unresolved external symbol sqlite3_column_database_name

sqlite3.def : error LNK2001: unresolved external symbol sqlite3_column_database_name16

sqlite3.def : error LNK2001: unresolved external symbol sqlite3_column_origin_name

sqlite3.def : error LNK2001: unresolved external symbol sqlite3_column_origin_name16

sqlite3.def : error LNK2001: unresolved external symbol sqlite3_column_table_name

sqlite3.def : error LNK2001: unresolved external symbol sqlite3_column_table_name16

sqlite3.def : error LNK2001: unresolved external symbol sqlite3_table_column_metadata

Debug/sqlite3.lib : fatal error LNK1120: 7 unresolved externals

在菜单【Project】-【Settings…】-【C/C++】标签下的Category【General】下

Preprocessor definitions下:

新增2个编译选项，分别是：

THREADSAFE

SQLITE_ENABLE_COLUMN_METADATA

10)往工程中添加sqlite3.def文件就是为生成sqlite3.lib文件；

sqlite3.lib是与sqlite3.dll动态链接库文件一起提供给软件开发者的；

11)解决Windows下的编译警告
e:/sqlite/code/sqlite3/sqlite3.c(15385) : warning C4047: 'function' : 'void ** ' differs in levels of indirection from 'long *'

e:/sqlite/code/sqlite3/sqlite3.c(15385) : warning C4022: 'InterlockedCompareExchange' : pointer mismatch for actual parameter 1

e:/sqlite/code/sqlite3/sqlite3.c(15385) : warning C4022: 'InterlockedCompareExchange' : pointer mismatch for actual parameter 2

e:/sqlite/code/sqlite3/sqlite3.c(15403) : warning C4047: 'function' : 'void ** ' differs in levels of indirection from 'long *'

e:/sqlite/code/sqlite3/sqlite3.c(15403) : warning C4022: 'InterlockedCompareExchange' : pointer mismatch for actual parameter 1

e:/sqlite/code/sqlite3/sqlite3.c(15403) : warning C4022: 'InterlockedCompareExchange' : pointer mismatch for actual parameter 3

e:/sqlite/code/sqlite3/sqlite3.c(15403) : warning C4047: '==' : 'void *' differs in levels of indirection from 'const int '

e:/sqlite/code/sqlite3/sqlite3.c(65536) : warning C4049: compiler limit : terminating line number emission

warning C4761: integral size mismatch in argument; conversion supplied

warning C4761: integral size mismatch in argument; conversion supplied

warning C4761: integral size mismatch in argument; conversion supplied

VC2005 + SQLite 3.6.3 编译、测试开发

http://blog.csdn.net/Dreamcode/archive/2009/08/21/4467277.aspx

17、SQLite一条SQL语句插入多条记录,批量插入
用SQLite才发现这个语法并非标准SQL，故而SQLite并不支持。网络上推荐的方法：
INSERT INTO TABLE(col1, col2) SELECT val11, val12 UNION ALL SELECT val21, val22 ;

这样的写法是属于复合SQL语句，表示先把两个SELECT的结果集进行无删减的联合，再把联合结果插入到TABLE中。

18、SQLite 插入大量数据慢(多次insert)的解决方法 
sqlite 插入数据很慢的原因：sqlite在没有显式使用事务的时候会为每条insert都使用事务操作，而sqlite数据库是以文件的形式存在磁盘中，就相当于每次访问时都要打开一次文件，如果对数据进行大量的操作，时间都耗费在I/O操作上，所以很慢。
解决方法是显式使用事务的形式提交：因为我们开始事务后，进行的大量操作的语句都保存在内存中，当提交时才全部写入数据库，此时，数据库文件也就只用打开一次。
[https://www.cnblogs.com/likebeta/archive/2012/06/15/2551466.html](https://www.cnblogs.com/likebeta/archive/2012/06/15/2551466.html)

参考资料：

sqlite 中文社区：http://www.sqlite.com.cn/

sqlite 官方网站：[http://www.sqlite.org/](http://www.sqlite.org/)


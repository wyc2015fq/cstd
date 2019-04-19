# 2010年SQLite学习笔记之一 - 2019跟着小虎玩着去软考 - CSDN博客
2010年02月20日 14:54:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：5859
**2010****年****SQLite****学习笔记之一**
# 一．如何获取SQLite最新版本
官方站点：[http://www.sqlite.org/](http://www.sqlite.org/)
从[http://www.sqlite.org/](http://www.sqlite.org/)网站的Download页面获取
即如下页面下载
http://www.sqlite.org/download.html
# 二．Windows下的SQLite的源代码是哪个软件压缩包？
[sqlite-amalgamation-3_6_22.zip](http://www.sqlite.org/sqlite-amalgamation-3_6_22.zip)是SQLite的windows下源码文件
# 三．Window下的SQLite命令行工具
[sqlite-3_6_22.zip](http://www.sqlite.org/sqlite-3_6_22.zip)
# 四．Window下的SQLite开发库，即动态链接库以及DEF文件
[sqlitedll-3_6_22.zip](http://www.sqlite.org/sqlitedll-3_6_22.zip)
# 五．SQLite最新版本是：3.6.22
# 六．b<=a && a <= c 的SQL语句如何写
在SQLite中, 表达式"a BETWEEN b AND c"等于表达式 "a >= b AND a <= c",在比较表达式时,a可以是具有任何亲和性
# 七．a的值是x,y,z其中一个值，SQL语句如何写
SQLite把表达式 "a IN (x, y, z)" 和 "a = z OR a = y OR a = z"视为相等. 
# 八．如何创建制定目录的数据库
sqlite3 D:/Project/SyncML/Lib/debug/atsync.db
# 九．创建数据的注意事项
**如果不往数据库里面添加任何的表，这个数据库等于没有建立，不会在硬盘上产生任何文件，如果数据库已经存在，则会打开这个数据库**。
# 十．如何添加一张数据表
create table student(name varchar(10), age smallint);
# 十一．如何往数据表中添加数据
insert into student values('张三', 20);
# 十二．如何通过sqlite3.dll与sqlite3.def生成sqlite3.lib文件
LIB /DEF:sqlite3.def /machine:IX86
# 十三．如何查询SQLite工具的软件版本
也就是sqlite3.exe应用程序小工具的版本号
方法一：进入sqlite3.exe所在目录，在命令行执行如下命令
sqlite3 –version
方法二：
select sqlite_version();
# 十四．如何在字符串中使用单引号(')？
SQL 标准规定，在字符串中，单引号需要使用逃逸字符，即在一行中使用两个单引号
# 十五．如果删除了大量数据，而又想缩小数据库文件占用的空间，执行 VACUUM 命令
vacuum;
# 十六．在SQLite中，如何在一个表上添加或删除一列？
SQLite 有有限地 ALTER TABLE 支持。你可以使用它来在表的末尾增加一列，可更改表的名称。如果需要对表结构做更复杂的改变，则必须重新建表。重建时可以先将已存在的数据放到一个临时表中，删除原表，创建新表，然后将数据从临时表中复制回来。
如，假设有一个 t1 表，其中有 "a", "b", "c" 三列，如果要删除列 c ，以下过程描述如何做:
开始事物处理
BEGIN TRANSACTION;
创建临时表格t1-backup
CREATE TEMPORARY TABLE t1_backup(a,b);
将数据库表t1中的所有数据拷贝到表t1-backup中
INSERT INTO t1_backup SELECT a,b FROM t1;
删除表格t1
DROP TABLE t1;
创建表格t1
CREATE TABLE t1(a,b);
将数据库表t1-backup中的所有数据拷贝到表t1中
INSERT INTO t1 SELECT a,b FROM t1_backup;
删除备份表格t1-backup
DROP TABLE t1_backup;
事物提交
COMMIT;
# 十七．如何查询当前的编码的编码格式
pragmaencoding;
**十八．****SQLite****支持哪些数据类型些？**
NULL 值为NULL
INTEGER 值为带符号的整型，根据类别用1，2，3，4，6，8字节存储
REAL 值为浮点型，8字节存储
TEXT 值为text字符串，使用数据库编码(UTF-8, UTF-16BE or UTF-16-LE)存储
BLOB 值为二进制数据，具体看实际输入
但实际上，sqlite3也接受如下的数据类型：
smallint  16 位元的整数
interger  32 位元的整数
decimal(p,s)  p 精确值和 s 大小的十进位整数，精确值p是指全部有几个数(digits)大小值，s是指小数点後有几位数。如果没有特别指定，则系统会设为 p=5; s=0 。
float   32位元的实数。
double   64位元的实数。
char(n)   n 长度的字串，n不能超过 254。
varchar(n)  长度不固定且其最大长度为 n 的字串，n不能超过 4000。
graphic(n)  和 char(n) 一样，不过其单位是两个字元 double-bytes， n不能超过127。这个形态是为了支援两个字元长度的字体，例如中文字。
vargraphic(n)  可变长度且其最大长度为 n 的双字元字串，n不能超过 2000。
date   包含了年份、月份、日期。
time   包含了小时、分钟、秒。
timestamp  包含了年、月、日、时、分、秒、千分之一秒。
# 十九．如果将某个字段设置为INTEGER PRIMARY KEY属性，有什么特性？
如果将声明表的一列设置为 INTEGER PRIMARY KEY，则具有：
1．每当你在该列上插入一NULL值时， NULL自动被转换为一个比该列中最大值大1的一个整数；
2．如果表是空的，将会是1；
**注意**该整数会比表中该列上的插入之前的最大值大1。该键值在当前的表中是唯一的。但有可能与已从表中删除的值重叠。要想建立在整个表的生命周期中唯一的键值，需要在 INTEGER PRIMARY KEY 上增加AUTOINCREMENT声明。那么，新的键值将会比该表中曾能存在过的最大值大1。
# 二十．字段声明中有AUTOINCREMENT属性，有什么与众不同的含义？
要想建立在整个表的生命周期中唯一的键值，需要在 INTEGER PRIMARY KEY 上增加AUTOINCREMENT声明。那么，新的键值将会比该表中曾能存在过的最大值大1。

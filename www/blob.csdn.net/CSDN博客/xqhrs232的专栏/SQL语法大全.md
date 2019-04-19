# SQL语法大全 - xqhrs232的专栏 - CSDN博客
2011年01月11日 17:05:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：788标签：[sql																[table																[insert																[数据库																[join																[branch](https://so.csdn.net/so/search/s.do?q=branch&t=blog)](https://so.csdn.net/so/search/s.do?q=join&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=insert&t=blog)](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=sql&t=blog)
个人分类：[SQLServer/Oracle数据库技术](https://blog.csdn.net/xqhrs232/article/category/906969)
原文地址::[http://www.qszx.org/jsj/Article/ShowArticle.asp?ArticleID=136&Page=1](http://www.qszx.org/jsj/Article/ShowArticle.asp?ArticleID=136&Page=1)
1. ASP与Access数据库连接：
<%@ language=VBscript%>
<%
dim conn,mdbfile
mdbfile=server.mappath("数据库名称.mdb")
set conn=server.createobject("adodb.connection")
conn.open "driver={microsoft access driver (*.mdb)};uid=admin;pwd=数据库密码;dbq="&mdbfile
%>
2. ASP与SQL数据库连接：
<%@ language=VBscript%>
<%
dim conn
set conn=server.createobject("ADODB.connection")
con.open "PROVIDER=SQLOLEDB;DATA SOURCE=SQL服务器名称或IP地址;UID=sa;PWD=数据库密码;DATABASE=数据库名称
%>
建立记录集对象：
set rs=server.createobject("adodb.recordset")
rs.open SQL语句,conn,3,2
3. SQL常用命令使用方法：
(1) 数据记录筛选：
sql="select * from 数据表 where 字段名=字段值 order by 字段名 [desc]"
sql="select * from 数据表 where 字段名 like /'%字段值%/' order by 字段名 [desc]"
sql="select top 10 * from 数据表 where 字段名 order by 字段名 [desc]"
sql="select * from 数据表 where 字段名 in (/'值1/',/'值2/',/'值3/')"
sql="select * from 数据表 where 字段名 between 值1 and 值2"
(2) 更新数据记录：
sql="update 数据表 set 字段名=字段值 where 条件表达式"
sql="update 数据表 set 字段1=值1,字段2=值2 …… 字段n=值n where 条件表达式"
(3) 删除数据记录：
sql="delete from 数据表 where 条件表达式"
sql="delete from 数据表"  (将数据表所有记录删除)
(4) 添加数据记录：
sql="insert into 数据表 (字段1,字段2,字段3 …) values (值1,值2,值3 …)"
sql="insert into 目标数据表 select * from 源数据表"  (把源数据表的记录添加到目标数据表)
(5) 数据记录统计函数：
AVG(字段名) 得出一个表格栏平均值
COUNT(*|字段名) 对数据行数的统计或对某一栏有值的数据行数统计
MAX(字段名) 取得一个表格栏最大的值
MIN(字段名) 取得一个表格栏最小的值
SUM(字段名) 把数据栏的值相加
引用以上函数的方法：
sql="select sum(字段名) as 别名 from 数据表 where 条件表达式"
set rs=conn.excute(sql)
用 rs("别名") 获取统的计值，其它函数运用同上。
(5) 数据表的建立和删除：
CREATE TABLE 数据表名称(字段1 类型1(长度),字段2 类型2(长度) …… )
例：CREATE TABLE tab01(name varchar(50),datetime default now())
DROP TABLE 数据表名称 (永久性删除一个数据表)
4. 记录集对象的方法：
rs.movenext              将记录指针从当前的位置向下移一行
rs.moveprevious          将记录指针从当前的位置向上移一行
rs.movefirst             将记录指针移到数据表第一行
rs.movelast              将记录指针移到数据表最后一行
rs.absoluteposition=N    将记录指针移到数据表第N行
rs.absolutepage=N        将记录指针移到第N页的第一行
rs.pagesize=N            设置每页为N条记录
rs.pagecount             根据 pagesize 的设置返回总页数
rs.recordcount           返回记录总数
rs.bof                   返回记录指针是否超出数据表首端，true表示是，false为否
rs.eof                   返回记录指针是否超出数据表末端，true表示是，false为否
rs.delete                删除当前记录，但记录指针不会向下移动
rs.addnew                添加记录到数据表末端
rs.update                更新数据表记录
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
Recordset对象方法
Open方法
recordset.Open Source,ActiveConnection,CursorType,LockType,Options
Source
Recordset对象可以通过Source属性来连接Command对象。Source参数可以是一个Command对象名称、一段SQL命令、一个指定的数据表名称或是一个Stored Procedure。假如省略这个参数，系统则采用Recordset对象的Source属性。
ActiveConnection
Recordset对象可以通过ActiveConnection属性来连接Connection对象。这里的ActiveConnection可以是一个Connection对象或是一串包含数据库连接信息（ConnectionString）的字符串参数。
CursorType
Recordset对象Open方法的CursorType参数表示将以什么样的游标类型启动数据，包括adOpenForwardOnly、adOpenKeyset、adOpenDynamic及adOpenStatic，分述如下：
--------------------------------------------------------------
常数                  常数值           说明
-------------------------------------------------------------
adOpenForwardOnly       0         缺省值，启动一个只能向前移动的游标（Forward Only）。
adOpenKeyset            1         启动一个Keyset类型的游标。
adOpenDynamic           2         启动一个Dynamic类型的游标。
adOpenStatic            3         启动一个Static类型的游标。
-------------------------------------------------------------
以上几个游标类型将直接影响到Recordset对象所有的属性和方法，以下列表说明他们之间的区别。
-------------------------------------------------------------
Recordset属性     adOpenForwardOnly    adOpenKeyset    adOpenDynamic    adOpenStatic
-------------------------------------------------------------
AbsolutePage           不支持            不支持          可读写            可读写
AbsolutePosition       不支持            不支持          可读写            可读写
ActiveConnection       可读写            可读写          可读写            可读写
BOF                    只读              只读            只读              只读
Bookmark               不支持            不支持          可读写            可读写
CacheSize              可读写            可读写          可读写            可读写
CursorLocation         可读写            可读写          可读写            可读写
CursorType             可读写            可读写          可读写            可读写
EditMode               只读              只读            只读              只读
EOF                    只读              只读            只读              只读
Filter                 可读写            可读写          可读写            可读写
LockType               可读写            可读写          可读写            可读写
MarshalOptions         可读写            可读写          可读写            可读写
MaxRecords             可读写            可读写          可读写            可读写
PageCount              不支持            不支持          只读              只
PageSize               可读写            可读写          可读写            可读写
RecordCount            不支持            不支持          只读              只读
Source                 可读写            可读写          可读写            可读写
State                  只读              只读            只读              只读
Status                 只读              只读            只读              只读
AddNew                 支持              支持            支持              支持
CancelBatch            支持              支持            支持              支持
CancelUpdate           支持              支持            支持              支持
Clone                  不支持            不支持
Close                  支持              支持            支持              支持
Delete                 支持              支持            支持              支持
GetRows                支持              支持            支持              支持
Move                   不支持            支持            支持              支持
MoveFirst              支持              支持            支持              支持
MoveLast               不支持            支持            支持              支持
MoveNext               支持              支持            支持              支持
MovePrevious           不支持            支持            支持              支持
NextRecordset          支持              支持            支持              支持
Open                   支持              支持            支持              支持
Requery                支持              支持            支持              支持
Resync                 不支持            不支持          支持              支持
Supports               支持              支持            支持              支持
Update                 支持              支持            支持              支持
UpdateBatch            支持              支持            支持              支持
--------------------------------------------------------------
其中NextRecordset方法并不适用于Microsoft Access数据库。
LockType
Recordset对象Open方法的LockType参数表示要采用的Lock类型，如果忽略这个参数，那么系统会以Recordset对象的LockType属性为预设值。LockType参数包含adLockReadOnly、adLockPrssimistic、adLockOptimistic及adLockBatchOptimistic等，分述如下：
-------------------------------------------------------------
常数                  常数值              说明
--------------------------------------------------------------
adLockReadOnly           1           缺省值，Recordset对象以只读方式启动，无法运行AddNew、Update及Delete等方法
adLockPrssimistic        2           当数据源正在更新时，系统会暂时锁住其他用户的动作，以保持数据一致性。
adLockOptimistic         3           当数据源正在更新时，系统并不会锁住其他用户的动作，其他用户可以对数据进行增、删、改的操作。
adLockBatchOptimistic    4           当数据源正在更新时，其他用户必须将CursorLocation属性改为adUdeClientBatch才能对数据进行增、
                                     删、改的操作。  
DB2 提供了关连式资料库的查询语言 ＳＱＬ (Structured Query Language)，是一种非常口语化、既易学又易懂的语法。此一语言几乎是每个资料库系统都必须提供的，用以表示关连式的操作，包含了资料的定义（ＤＤＬ）以及资料的处理（ＤＭＬ）。SQL原来拼成SEQUEL，这语言的原型以“系统 R“的名字在 IBM 圣荷西实验室完成，经过IBM内部及其他的许多使用性及效率测试，其结果相当令人满意，并决定在系统R 的技术基础发展出来 IBM 的产品。而且美国国家标准学会（ANSI）及国际标准化组织（ISO）在1987遵循一个几乎是以
 IBM SQL 为基础的标准关连式资料语言定义。
一、资料定义 ＤＤＬ（Data Definition Language)
资料定语言是指对资料的格式和形态下定义的语言，他是每个资料库要建立时候时首先要面对的，举凡资料分哪些表格关系、表格内的有什麽栏位主键、表格和表格之间互相参考的关系等等，都是在开始的时候所必须规划好的。
１、建表格：
CREATE TABLE table_name( 
column1 DATATYPE [NOT NULL] [NOT NULL PRIMARY KEY], 
column2 DATATYPE [NOT NULL],
...）
说明：　
DATATYPE --是资料的格式，详见表。
NUT NULL --可不可以允许资料有空的（尚未有资料填入）。
PRIMARY KEY --是本表的主键。 
--------------------------------------------------------------------------------
--  作者：belin2000
--  发布时间：2004-6-8 22:08:51
--  
２、更改表格　
ALTER TABLE table_name 
ADD COLUMN column_name DATATYPE 
说明：增加一个栏位（没有删除某个栏位的语法。
ALTER TABLE table_name
ADD PRIMARY KEY (column_name)
说明：更改表得的定义把某个栏位设为主键。
ALTER TABLE table_name
DROP PRIMARY KEY (column_name)
说明：把主键的定义删除。
３、建立索引　
CREATE INDEX index_name ON table_name (column_name)
说明：对某个表格的栏位建立索引以增加查询时的速度。
４、删除　
DROP table_name
DROP index_name
二、的资料形态 DATATYPEs
smallint
16 位元的整数。
interger
32 位元的整数。
decimal(p,s)
p 精确值和 s 大小的十进位整数，精确值p是指全部有几个数(digits)大小值，s是指小数
点後有几位数。如果没有特别指定，则系统会设为 p=5; s=0 。 
float
32位元的实数。
double
64位元的实数。
char(n)
n 长度的字串，n不能超过 254。
varchar(n)
长度不固定且其最大长度为 n 的字串，n不能超过 4000。
graphic(n)
和 char(n) 一样，不过其单位是两个字元 double-bytes， n不能超过127。这个形态是为
了支援两个字元长度的字体，例如中文字。
vargraphic(n)
可变长度且其最大长度为 n 的双字元字串，n不能超过 2000。
date
包含了 年份、月份、日期。
time
包含了 小时、分钟、秒。
timestamp
包含了 年、月、日、时、分、秒、千分之一秒。
三、资料操作 ＤＭＬ （Data Manipulation Language)
资料定义好之後接下来的就是资料的操作。资料的操作不外乎增加资料（insert)、查询资料（query）、更改资料（update) 、删除资料（delete）四种模式，以下分 别介绍他们的语法：
１、增加资料：
INSERT INTO table_name (column1,column2,...)
valueS ( value1,value2, ...)
说明：
1.若没有指定column 系统则会按表格内的栏位顺序填入资料。
2.栏位的资料形态和所填入的资料必须吻合。
3.table_name 也可以是景观 view_name。
INSERT INTO table_name (column1,column2,...)
SELECT columnx,columny,... FROM another_table
说明：也可以经过一个子查询（subquery）把别的表格的资料填入。
２、查询资料：
基本查询
SELECT column1,columns2,...
FROM table_name
说明：把table_name 的特定栏位资料全部列出来
SELECT *
FROM table_name
WHERE column1 = xxx 
[AND column2 > yyy] [OR column3 <> zzz]
说明：
1./'*/'表示全部的栏位都列出来。
2.WHERE 之後是接条件式，把符合条件的资料列出来。
SELECT column1,column2
FROM table_name
ORDER BY column2 [DESC]
说明：ORDER BY 是指定以某个栏位做排序，[DESC]是指从大到小排列，若没有指明，则是从小到大
排列
组合查询
组合查询是指所查询得资料来源并不只有单一的表格，而是联合一个以上的
表格才能够得到结果的。
SELECT *
FROM table1,table2
WHERE table1.colum1=table2.column1
说明：
1.查询两个表格中其中 column1 值相同的资料。
2.当然两个表格相互比较的栏位，其资料形态必须相同。
3.一个复杂的查询其动用到的表格可能会很多个。
整合性的查询：
SELECT COUNT (*) 
FROM table_name
WHERE column_name = xxx
说明：
查询符合条件的资料共有几笔。
SELECT SUM(column1)
FROM table_name
说明：
1.计算出总和，所选的栏位必须是可数的数字形态。
2.除此以外还有 AVG() 是计算平均、MAX()、MIN()计算最大最小值的整合性查询。
SELECT column1,AVG(column2)
FROM table_name
GROUP BY column1
HAVING AVG(column2) > xxx
说明：
1.GROUP BY: 以column1 为一组计算 column2 的平均值必须和 AVG、SUM等整合性查询的关键字
一起使用。 
2.HAVING : 必须和 GROUP BY 一起使用作为整合性的限制。
复合性的查询
SELECT *
FROM table_name1
WHERE EXISTS (
SELECT *
FROM table_name2
WHERE conditions )
说明：
1.WHERE 的 conditions 可以是另外一个的 query。
2.EXISTS 在此是指存在与否。
SELECT *
FROM table_name1
WHERE column1 IN (
SELECT column1 
FROM table_name2
WHERE conditions )
说明：　
1. IN 後面接的是一个集合，表示column1 存在集合里面。
2. SELECT 出来的资料形态必须符合 column1。 
其他查询
SELECT *
FROM table_name1
WHERE column1 LIKE /'x%/' 
说明：LIKE 必须和後面的/'x%/' 相呼应表示以 x为开头的字串。
SELECT *
FROM table_name1
WHERE column1 IN (/'xxx/',/'yyy/',..)
说明：IN 後面接的是一个集合，表示column1 存在集合里面。
SELECT *
FROM table_name1
WHERE column1 BETWEEN xx AND yy
说明：BETWEEN 表示 column1 的值介於 xx 和 yy 之间。 
３、更改资料：
UPDATE table_name
SET column1=/'xxx/'
WHERE conditoins
说明：
1.更改某个栏位设定其值为/'xxx/'。
2.conditions 是所要符合的条件、若没有 WHERE 则整个 table 的那个栏位都会全部被更改。
４、删除资料：
DELETE FROM table_name
WHERE conditions
说明：删除符合条件的资料。
说明：关于WHERE条件后面如果包含有日期的比较，不同数据库有不同的表达式。具体如下：
(1)如果是ACCESS数据库，则为：WHERE mydate>#2000-01-01# 
(2)如果是ORACLE数据库，则为：WHERE mydate>cast(/'2000-01-01/' as date)
或：WHERE mydate>to_date(/'2000-01-01/',/'yyyy-mm-dd/')
在Delphi中写成：
thedate=/'2000-01-01/';
query1.SQL.add(/'select * from abc where mydate>cast(/'+/'/'/'/'+thedate+/'/'/'/'+/' as date)/');
如果比较日期时间型，则为：
WHERE mydatetime>to_date(/'2000-01-01 10:00:01/',/'yyyy-mm-dd hh24:mi:ss/')   
练掌握SQL是数据库用户的宝贵财 富。在本文中，我们将引导你掌握四条最基本的数据操作语句-SQL的核心功能-来依次介绍比较操作符、选择断言以及三值逻辑。当你完成这些学习后，显然你已经开始算是精通SQL了。
在我们开始之前，先使用CREATE TABLE语句来创建一个表（如图1所示）。DDL语句对数据库对象如表、列和视进行定义。它们并不对表中的行进行处理，这是因为DDL语句并不处理数据库中实际的数据。这些工作由另一类SQL语句-数据操作语言（DML）语句进行处理。
SQL中有四种基本的DML操作：INSERT，SELECT，UPDATE和DELETE。由于这是大多数SQL用户经常用到的，我们有必要在此对它们进行一一说明。在图1中我们给出了一个名为EMPLOYEES的表。其中的每一行对应一个特定的雇员记录。请熟悉这张表，我们在后面的例子中将要用到它。
连接查询 
通过连接运算符可以实现多个表查询。连接是关系数据库模型的主要特点，也是它区别于其它类型数据库管理系统的一个标志。
在关系数据库管理系统中，表建立时各数据之间的关系不必确定，常把一个实体的所有信息存放在
一个表中。当检索数据时，通过连接操作查询出存放在多个表中的不同实体的信息。连接操作给用户带
来很大的灵活性，他们可以在任何时候增加新的数据类型。为不同实体创建新的表，尔后通过连接进行查询。
连接可以在SELECT 语句的FROM子句或WHERE子句中建立，似是而非在FROM子句中指出连接时有助于
将连接操作与WHERE子句中的搜索条件区分开来。所以，在Transact-SQL中推荐使用这种方法。
SQL-92标准所定义的FROM子句的连接语法格式为：
FROM join_table join_type join_table
[ON (join_condition)]
其中join_table指出参与连接操作的表名，连接可以对同一个表操作，也可以对多表操作，对同一
个表操作的连接又称做自连接。
join_type 指出连接类型，可分为三种：内连接、外连接和交叉连接。内连接(INNER JOIN)使用比
较运算符进行表间某(些)列数据的比较操作，并列出这些表中与连接条件相匹配的数据行。根据所使用
的比较方式不同，内连接又分为等值连接、自然连接和不等连接三种。
外连接分为左外连接(LEFT OUTER JOIN或LEFT JOIN)、右外连接(RIGHT OUTER JOIN或RIGHT JOIN)
和全外连接(FULL OUTER JOIN或FULL JOIN)三种。与内连接不同的是，外连接不只列出与连接条件相匹
配的行，而是列出左表(左外连接时)、右表(右外连接时)或两个表(全外连接时)中所有符合搜索条件的数据行。
交叉连接(CROSS JOIN)没有WHERE 子句，它返回连接表中所有数据行的笛卡尔积，其结果集合中的
数据行数等于第一个表中符合查询条件的数据行数乘以第二个表中符合查询条件的数据行数。
连接操作中的ON (join_condition) 子句指出连接条件，它由被连接表中的列和比较运算符、逻辑
运算符等构成。
无论哪种连接都不能对text、ntext和image数据类型列进行直接连接，但可以对这三种列进行间接
连接。例如：
SELECT p1.pub_id,p2.pub_id,p1.pr_info
FROM pub_info AS p1 INNER JOIN pub_info AS p2
ON DATALENGTH(p1.pr_info)=DATALENGTH(p2.pr_info)
(一)内连接
内连接查询操作列出与连接条件匹配的数据行，它使用比较运算符比较被连接列的列值。内连接分三种：
1、等值连接：在连接条件中使用等于号(=)运算符比较被连接列的列值，其查询结果中列出被连接
表中的所有列，包括其中的重复列。
2、不等连接： 在连接条件使用除等于运算符以外的其它比较运算符比较被连接的列的列值。这些
运算符包括>、>=、<=、<、!>、!<和<>。
3、自然连接：在连接条件中使用等于(=)运算符比较被连接列的列值，但它使用选择列表指出查询
结果集合中所包括的列，并删除连接表中的重复列。
例，下面使用等值连接列出authors和publishers表中位于同一城市的作者和出版社：
SELECT *
FROM authors AS a INNER JOIN publishers AS p
ON a.city=p.city
又如使用自然连接，在选择列表中删除authors 和publishers 表中重复列(city和state)：
SELECT a.*,p.pub_id,p.pub_name,p.country
FROM authors AS a INNER JOIN publishers AS p
ON a.city=p.city
(二)外连接
内连接时，返回查询结果集合中的仅是符合查询条件( WHERE 搜索条件或 HAVING 条件)和连接条件
的行。而采用外连接时，它返回到查询结果集合中的不仅包含符合连接条件的行，而且还包括左表(左外
连接时)、右表(右外连接时)或两个边接表(全外连接)中的所有数据行。
如下面使用左外连接将论坛内容和作者信息连接起来：
SELECT a.*,b.* FROM luntan LEFT JOIN usertable as b
ON a.username=b.username
下面使用全外连接将city表中的所有作者以及user表中的所有作者，以及他们所在的城市：
SELECT a.*,b.*
FROM city as a FULL OUTER JOIN user as b
ON a.username=b.username
--------------------------------------------------------------------------------
--  作者：belin2000
--  发布时间：2004-6-8 22:09:38
--  
三)交叉连接
交叉连接不带WHERE 子句，它返回被连接的两个表所有数据行的笛卡尔积，返回到结果集合中的数
据行数等于第一个表中符合查询条件的数据行数乘以第二个表中符合查询条件的数据行数。
例，titles表中有6类图书，而publishers表中有8家出版社，则下列交叉连接检索到的记录数将等
于6*8=48行。
SELECT type,pub_name
FROM titles CROSS JOIN publishers
ORDER BY type
UNION运算符可以将两个或两个以上上SELECT语句的查询结果集合合并成一个结果集合显示，即执行联合查询。UNION的语法格式为：
select_statement
UNION [ALL] selectstatement
[UNION [ALL] selectstatement][…n]
其中selectstatement为待联合的SELECT查询语句。
ALL选项表示将所有行合并到结果集合中。不指定该项时，被联合查询结果集合中的重复行将只保留一
行。
联合查询时，查询结果的列标题为第一个查询语句的列标题。因此，要定义列标题必须在第一个查询语
句中定义。要对联合查询结果排序时，也必须使用第一查询语句中的列名、列标题或者列序号。
在使用UNION 运算符时，应保证每个联合查询语句的选择列表中有相同数量的表达式，并且每个查询选
择表达式应具有相同的数据类型，或是可以自动将它们转换为相同的数据类型。在自动转换时，对于数值类
型，系统将低精度的数据类型转换为高精度的数据类型。
在包括多个查询的UNION语句中，其执行顺序是自左至右，使用括号可以改变这一执行顺序。例如：
查询1 UNION (查询2 UNION 查询3)
　　INSERT语句
　　用户可以用INSERT语句将一行记录插入到指定的一个表中。例如，要将雇员John Smith的记录插入到本例的表中，可以使用如下语句：
　　INSERT INTO EMPLOYEES valueS
　　 (/'Smith/',/'John/',/'1980-06-10/',
　　 /'Los Angles/',16,45000);
　　通过这样的INSERT语句，系统将试着将这些值填入到相应的列中。这些列按照我们创建表时定义的顺序排列。在本例中，第一个值“Smith”将填到第一个列LAST_NAME中；第二个值“John”将填到第二列FIRST_NAME中……以此类推。
　　我们说过系统会“试着”将值填入，除了执行规则之外它还要进行类型检查。如果类型不符（如将一个字符串填入到类型为数字的列中），系统将拒绝这一次操作并返回一个错误信息。
　　如果SQL拒绝了你所填入的一列值，语句中其他各列的值也不会填入。这是因为SQL提供对事务的支持。一次事务将数据库从一种一致性转移到另一种一致性。如果事务的某一部分失败，则整个事务都会失败，系统将会被恢复（或称之为回退）到此事务之前的状态。
　　 回到原来的INSERT的例子，请注意所有的整形十进制数都不需要用单引号引起来，而字符串和日期类型的值都要用单引号来区别。为了增加可读性而在数字间插入逗号将会引起错误。记住，在SQL中逗号是元素的分隔符。
　　同样要注意输入文字值时要使用单引号。双引号用来封装限界标识符。
　　对于日期类型，我们必须使用SQL标准日期格式（yyyy-mm-dd），但是在系统中可以进行定义，以接受其他的格式。当然，2000年临近，请你最好还是使用四位来表示年份。
　　既然你已经理解了INSERT语句是怎样工作的了，让我们转到EMPLOYEES表中的其他部分：
　　INSERT INTO EMPLOYEES valueS
　　 (/'Bunyan/',/'Paul/',/'1970-07-04/',
　　 /'Boston/',12,70000);
　　INSERT INTO EMPLOYEES valueS
　　 (/'John/',/'Adams/',/'1992-01-21/',
　　 /'Boston/',20,100000);
　　INSERT INTO EMPLOYEES valueS
　　 (/'Smith/',/'Pocahontas/',/'1976-04-06/',
　　 /'Los Angles/',12,100000);
　　INSERT INTO EMPLOYEES valueS
　　 (/'Smith/',/'Bessie/',/'1940-05-02/',
　　 /'Boston/',5,200000);
　　INSERT INTO EMPLOYEES valueS
　　 (/'Jones/',/'Davy/',/'1970-10-10/',
　　 /'Boston/',8,45000);
　　INSERT INTO EMPLOYEES valueS
　　 (/'Jones/',/'Indiana/',/'1992-02-01/',
　　 /'Chicago/',NULL,NULL);
　　在最后一项中，我们不知道Jones先生的工薪级别和年薪，所以我们输入NULL（不要引号）。NULL是SQL中的一种特殊情况，我们以后将进行详细的讨论。现在我们只需认为NULL表示一种未知的值。
　　有时，像我们刚才所讨论的情况，我们可能希望对某一些而不是全部的列进行赋值。除了对要省略的列输入NULL外，还可以采用另外一种INSERT语句，如下：
　　INSERT INTO EMPLOYEES(
　　 FIRST_NAME, LAST_NAME
 HIRE_DATE, BRANCH_OFFICE)
　　value(
　　 /'Indiana/',/'Jones/',
　　 /'1992-02-01/',/'Indianapolis/');
　　这样，我们先在表名之后列出一系列列名。未列出的列中将自动填入缺省值，如果没有设置缺省值则填入NULL。请注意我们改变了列的顺序，而值的顺序要对应新的列的顺序。如果该语句中省略了FIRST_NAME和LAST_NAME项（这两项规定不能为空），SQL操作将失败。
　　让我们来看一看上述INSERT语句的语法图：
　　INSERT INTO table
　　 [(column { ,column})]
　　valueS
　　 (columnvalue [{,columnvalue}]);
　　和前一篇文章中一样，我们用方括号来表示可选项，大括号表示可以重复任意次数的项（不能在实际的SQL语句中使用这些特殊字符）。value子句和可选的列名列表中必须使用圆括号。
　　SELECT语句
　　SELECT语句可以从一个或多个表中选取特定的行和列。因为查询和检索数据是数据库管理中最重要的功能，所以SELECT语句在SQL中是工作量最大的部分。实际上，仅仅是访问数据库来分析数据并生成报表的人可以对其他SQL语句一窍不通。
　　SELECT语句的结果通常是生成另外一个表。在执行过程中系统根据用户的标准从数据库中选出匹配的行和列，并将结果放到临时的表中。在直接SQL（direct SQL）中，它将结果显示在终端的显示屏上，或者将结果送到打印机或文件中。也可以结合其他SQL语句来将结果放到一个已知名称的表中。
　　SELECT语句功能强大。虽然表面上看来它只用来完成本文第一部分中提到的关系代数运算“选择”（或称“限制”），但实际上它也可以完成其他两种关系运算-“投影”和“连接”，SELECT语句还可以完成聚合计算并对数据进行排序。
　　SELECT语句最简单的语法如下：
　　SELECT columns FROM tables;
　　当我们以这种形式执行一条SELECT语句时，系统返回由所选择的列以及用户选择的表中所有指定的行组成的一个结果表。这就是实现关系投影运算的一个形式。
　　让我们看一下使用图1中EMPLOYEES表的一些例子（这个表是我们以后所有SELECT语句实例都要使用的。而我们在图2和图3中给出了查询的实际结果。我们将在其他的例子中使用这些结果）。
　　假设你想查看雇员工作部门的列表。那下面就是你所需要编写的SQL查询：
　　SELECT BRANCH_OFFICE FROM EMPLOYEES;
　　以上SELECT语句的执行将产生如图2中表2所示的结果。
　　由于我们在SELECT语句中只指定了一个列，所以我们的结果表中也只有一个列。注意结果表中具有重复的行，这是因为有多个雇员在同一部门工作（记住SQL从所选的所有行中将值返回）。要消除结果中的重复行，只要在SELECT语句中加上DISTINCT子句：
　　SELECT DISTINCT BRANCH_OFFICE
　　FROM EMPLOYEES;
　　这次查询的结果如表3所示。
　　现在已经消除了重复的行，但结果并不是按照顺序排列的。如果你希望以字母表顺序将结果列出又该怎么做呢？只要使用ORDER BY子句就可以按照升序或降序来排列结果：
　　SELECT DISTINCT BRANCH_OFFICE
　　FROM EMPLOYEES
　　ORDER BY BRANCH_OFFICE ASC;
　　这一查询的结果如表4所示。请注意在ORDER BY之后是如何放置列名BRANCH _OFFICE的，这就是我们想要对其进行排序的列。为什么即使是结果表中只有一个列时我们也必须指出列名呢？这是因为我们还能够按照表中其他列进行排序，即使它们并不显示出来。列名BRANCH_ OFFICE之后的关键字ASC表示按照升序排列。如果你希望以降序排列，那么可以用关键字DESC。
　　同样我们应该指出ORDER BY子句只将临时表中的结果进行排序；并不影响原来的表。
　　假设我们希望得到按部门排序并从工资最高的雇员到工资最低的雇员排列的列表。除了工资括号中的内容，我们还希望看到按照聘用时间从最近聘用的雇员开始列出的列表。以下是你将要用到的语句：
　　SELECT BRANCH_OFFICE,FIRST_NAME,
　　 LAST_NAME,SALARY,HIRE_DATE
　　FROM EMPLOYEES
　　ORDER BY SALARY DESC,
　　 HIRE_DATE DESC;
　　这里我们进行了多列的选择和排序。排序的优先级由语句中的列名顺序所决定。SQL将先对列出的第一个列进行排序。如果在第一个列中出现了重复的行时，这些行将被按照第二列进行排序，如果在第二列中又出现了重复的行时，这些行又将被按照第三列进行排序……如此类推。这次查询的结果如表5所示。
　　将一个很长的表中的所有列名写出来是一件相当麻烦的事，所以SQL允许在选择表中所有的列时使用*号：
　　SELECT * FROM EMPLOYEES;
　　这次查询返回整个EMPLOYEES表，如表1所示。
　　 下面我们对开始时给出的SELECT语句的语法进行一下更新（竖直线表示一个可选项，允许在其中选择一项。）：
　　SELECT [DISTINCT]
　　 (column [{, columns}])| *
　　FROM table [ {, table}]
　　[ORDER BY column [ASC] | DESC
　　 [ {, column [ASC] | DESC }]];
　　定义选择标准
　　在我们目前所介绍的SELECT语句中，我们对结果表中的列作出了选择但返回的是表中所有的行。让我们看一下如何对SELECT语句进行限制使得它只返回希望得到的行：
　　SELECT columns FROM tables [WHERE predicates];
　　WHERE子句对条件进行了设置，只有满足条件的行才被包括到结果表中。这些条件由断言（predicate）进行指定（断言指出了关于某件事情的一种可能的事实）。如果该断言对于某个给定的行成立，该行将被包括到结果表中，否则该行被忽略。在SQL语句中断言通常通过比较来表示。例如，假如你需要查询所有姓为Jones的职员，则可以使用以下SELECT语句：
　　SELECT * FROM EMPLOYEES
　　WHERE LAST_NAME = /'Jones/';
　　LAST_NAME = /'Jones/'部分就是断言。在执行该语句时，SQL将每一行的LAST_NAME列与“Jones”进行比较。如果某一职员的姓为“Jones”，即断言成立，该职员的信息将被包括到结果表中（见表6）。
　　使用最多的六种比较
　　我们上例中的断言包括一种基于“等值”的比较（LAST_NAME = /'Jones/'），但是SQL断言还可以包含其他几种类型的比较。其中最常用的为：
　　等于 =
　　不等于 <>
　　小于 <
　　大于 >
　　小于或等于 <=
　　大于或等于 >=
　　下面给出了不是基于等值比较的一个例子：
　　SELECT * FROM EMPLOYEES
　　WHERE SALARY > 50000;
　　这一查询将返回年薪高于＄50,000.00的职员（参见表7）。
　　逻辑连接符
　　有时我们需要定义一条不止一种断言的SELECT语句。举例来说，如果你仅仅想查看Davy Jones的信息的话，表6中的结果将是不正确的。为了进一步定义一个WHERE子句，用户可以使用逻辑连接符AND，OR和NOT。为了只得到职员Davy Jones的记录，用户可以输入如下语句：
　　SELECT * FROM EMPLOYEES
　　WHERE LAST_NAME = /'Jones/' AND FIRST_NAME = /'Davy/';
　　在本例中，我们通过逻辑连接符AND将两个断言连接起来。只有两个断言都满足时整个表达式才会满足。如果用户需要定义一个SELECT语句来使得当其中任何一项成立就满足条件时，可以使用OR连接符：
　　SELECT * FROM EMPLOYEES
　　WHERE LAST_NAME = /'Jones/' OR LAST_NAME = /'Smith/';
　　有时定义一个断言的最好方法是通过相反的描述来说明。如果你想要查看除了Boston办事处的职员以外的其他所有职员的信息时，你可以进行如下的查询：
　　SELECT * FROM EMPLOYEES
　　WHERE NOT(BRANCH_OFFICE = /'Boston/');
　　关键字NOT后面跟着用圆括号括起来的比较表达式。其结果是对结果取否定。如果某一职员所在部门的办事处在Boston，括号内的表达式返回true，但是NOT操作符将该值取反，所以该行将不被选中。
　　断言可以与其他的断言嵌套使用。为了保证它们以正确的顺序进行求值，可以用括号将它们括起来：
　　SELECT * FROM EMPLOYEES
　　WHERE (LAST_NAME = /'Jones/'
　　AND FIRST_NAME = /'Indiana/')
　　OR (LAST_NAME = /'Smith/'
　　AND FIRST_NAME = /'Bessie/');
　　SQL沿用数学上标准的表达式求值的约定-圆括号内的表达式将最先进行求值，其他表达式将从左到右进行求值。


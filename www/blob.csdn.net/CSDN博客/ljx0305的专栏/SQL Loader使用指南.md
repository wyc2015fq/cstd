# SQL*Loader使用指南 - ljx0305的专栏 - CSDN博客
2009年02月25日 15:41:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：523
Oracle 的SQL*LOADER可以将外部数据加载到数据库表中。下面是SQL*LOADER的基本特点：
1）能装入不同数据类型文件及多个数据文件的数据
2）可装入固定格式，自由定界以及可度长格式的数据
3）可以装入二进制，压缩十进制数据
4）一次可对多个表装入数据
5）连接多个物理记录装到一个记录中
6）对一单记录分解再装入到表中
7）可以用 数对制定列生成唯一的KEY
8）可对磁盘或 磁带数据文件装入制表中
9）提供装入错误报告
10）可以将文件中的整型字符串，自动转成压缩十进制并装入列表中。
1.2控制文件
控制文件是用一种语言写的文本文件，这个文本文件能被SQL*LOADER识别。SQL*LOADER根据控制文件可以找到需要加载的数据。并且分析和解释这些数据。控制文件由三个部分组成：
l 全局选件，行，跳过的记录数等；
l INFILE子句指定的输入数据；
l 数据特性说明。
1.3输入文件
对于 SQL*Loader, 除控制文件外就是输入数据。SQL*Loader可从一个或多个指定的文件中读出数据。如果数据是在控制文件中指定，就要在控制文件中写成 INFILE * 格式。当数据固定的格式（长度一样）时且是在文件中得到时，要用INFILE "fix n"
load data
infile 'example.dat' "fix 11"
into table example
fields terminated by ',' optionally enclosed by '"'
(col1 char(5),
col2 char(7))
example.dat:
001, cd, 0002,fghi,
00003,lmn,
1, "pqrs",
0005,uvwx,
当数据是可变格式（长度不一样）时且是在文件中得到时，要用INFILE "var n"。如：
load data
infile 'example.dat' "var 3"
into table example
fields terminated by ',' optionally enclosed by '"'
(col1 char(5),
col2 char(7))
example.dat:
009hello,cd,010world,im,
012my,name is,
1.4坏文件
坏文件包含那些被SQL*Loader拒绝的记录。被拒绝的记录可能是不符合要求的记录。
坏文件的名字由 SQL*Loader命令的BADFILE 参数来给定。
1.5日志文件及日志信息
当SQL*Loader 开始执行后，它就自动建立 日志文件。日志文件包含有加载的总结，加载中的错误信息等。
控制文件语法
控制文件的格式如下：
OPTIONS （ { [SKIP=integer] [ LOAD = integer ]
[ERRORS = integer] [ROWS=integer]
[BINDSIZE=integer] [SILENT=(ALL|FEEDBACK|ERROR|DISCARD) ] )
LOAD[DATA]
[ { INFILE | INDDN } {file | * }
[STREAM | RECORD | FIXED length [BLOCKSIZE size]|
VARIABLE [length] ]
[ { BADFILE | BADDN } file ]
{DISCARDS | DISCARDMAX} integr ]
[ {INDDN | INFILE} . . . ]
[ APPEND | REPLACE | INSERT ]
[RECLENT integer]
[ { CONCATENATE integer |
CONTINUEIF { [THIS | NEXT] (start[: end])LAST }
Operator { 'string' | X 'hex' } } ]
INTO TABLE [user.]table
[APPEND | REPLACE|INSERT]
[WHEN condition [AND condition]...]
[FIELDS [delimiter] ]
(
column {
RECNUM | CONSTANT value |
SEQUENCE ( { integer | MAX |COUNT} [, increment] ) |
[POSITION ( { start [end] | * [ + integer] }
) ]
datatype
[TERMINATED [ BY ] {WHITESPACE| [X] 'character' } ]
[ [OPTIONALLY] ENCLOSE[BY] [X]'charcter']
[NULLIF condition ]
[DEFAULTIF condotion]
}
[ ,...]
)
[INTO TABLE...]
[BEGINDATA]
1）要加载的数据文件：
1．INFILE 和INDDN是同义词，它们后面都是要加载的数据文件。如果用 * 则表示数据就在控制文件内。在INFILE 后可以跟几个文件。
2．STRAM 表示一次读一个字节的数据。新行代表新物理记录（逻辑记录可由几个物理记录组成）。
3．RECORD 使用宿主操作系统文件及记录管理系统。如果数据在控制文件中则使用这种方法。
3． FIXED length 要读的记录长度为length字节，
4． VARIABLE 被读的记录中前两个字节包含的长度，length 记录可能的长度。缺伤为8k字节。
5． BADFILE和BADDN同义。Oracle 不能加载数据到数据库的那些记录。
6． DISCARDFILE和DISCARDDN是同义词。记录没有通过的数据。
7． DISCARDS和DISCARDMAX是同义词。Integer 为最大放弃的文件个数。
2）加载的方法：
1．APPEND 给表添加行。
2．INSERT 给空表增加行（如果表中有记录则退出）。
3．REPLACE 先清空表在加载数据。
4． RECLEN 用于两种情况，1）SQLLDR不能自动计算记录长度，2）或用户想看坏文件的完整记录时。对于后一种，Oracle只能按常规把坏记录部分写到错误的地方。如果看整条记录，则可以将整条记录写到坏文件中。
3）指定最大的记录长度：
1． CONCATENATE 允许用户设定一个整数，表示要组合逻辑记录的数目。
4）建立逻辑记录：
1．THIS 检查当前记录条件，如果为真则连接下一个记录。
2．NEXT 检查下一个记录条件。如果为真，则连接下一个记录到当前记录来。
2． Start: end 表示要检查在THIS或NEXT字串是否存在继续串的列，以确定是否进行连接。如：continueif next(1-3)='WAG' 或continueif next(1-3)=X'0d03if'
5）指定要加载的表：
1．INTO TABLE 要加的表名。
2．WHEN 和select WHERE类似。用来检查记录的情况，如：when(3-5)='SSM' and (22)='*"
6）介绍并括起记录中的字段：
1． FIELDS给出记录中字段的分隔符，FIELDS格式为：
FIELDS [TERMIALED [BY] {WHITESPACE | [X] 'charcter'} ]
[ [ OPTIONALLY] ENCLOSE [BY] [X]'charcter' ]
TERMINATED 读完前一个字段即开始读下一个字段直到介绍。
WHITESPACE 是指结束符是空格的意思。包括空格、Tab、换行符、换页符及回车符。如果是要判断但字符，可以用单引号括起，如X'1B'等。
OPTIONALLY ENCLOSED 表示数据应由特殊字符括起来。也可以括在TERMINATED字符内。使用OPTIONALLY要同时用TERMINLATED。
ENCLOSED 指两个分界符内的数据。如果同时用 ENCLOSED和TERMINAED ，则它们的顺序决定计算的顺序。
7）定义列：
column 是表列名。列的取值可以是：
BECHUM 表示逻辑记录数。第一个记录为1,第2个记录为2。
CONSTANT 表示赋予常数。
SEQUENCE 表示序列可以从任意序号开始，格式为：
SEQUENCE （ { integer | MAX |COUNT} [,increment]
POSITION 给出列在逻辑记录中的位置。可以是绝对的，或相对前一列的值。格式为：
POSITION ( {start[end] | * [+integer] } )
Start 开始位置
* 表示前字段之后立刻开始。
+ 从前列开始向后条的位置数。
8）定义数据类型：
可以定义14种数据类型：
CHAR
DATE
DECIMAL EXTERNAL
DECIMAL
DOUBLE
FLOAT
FLOAT EXTERNAL
GRAPHIC EXTERNAL
INTEGER
INTEGER EXTERNAL
SMALLINT
VARCHAR
VARGRAPHIC
1．字符类型数据
CHAR[ (length)] [delimiter]
length缺省为 1.
2.日期类型数据
DATE [ ( length)]['date_format' [delimiter]
使用to_date函数来限制。
3．字符格式中的十进制
DECIMAL EXTERNAL [(length)] [delimiter]
用于常规格式的十进制数（不是二进制=> 一个位等于一个bit）。
4．压缩十进制格式数据
DECIMAL （digtial [,divcision]）
5．双精度符点二进制
DOUBLE
6．普通符点二进制
FLOAT
7．字符格式符点数
FLOAT EXTERNAL [ (length) ] [delimiter]
8．双字节字符串数据
GRAPHIC [ (legth)]
9．双字节字符串数据
GRAPHIC EXTERNAL[ (legth)]
10．常规全字二进制整数
INTEGER
11．字符格式整数
INTEGER EXTERNAL
12．常规全字二进制数据
SMALLINT
13．可变长度字符串
VARCHAR
14．可变双字节字符串数据
VARGRAPHIC
2.2写控制文件CTL
1． 各数据文件的文件名;
2．各数据文件格式;
3．各数据文件里各数据记录字段的属性；
4．接受数据的ORACLE表列的属性；
5．数据定义；
6．其它
数据文件的要求：
数据类型的指定
CHAR 字符型
INTEGER EXTERNAL 整型
DECIMAL EXTERNAL 浮点型
3.1数据文件的内容
可以在OS下的一个文件；或跟在控制文件下的具体数据。数据文件可以是：
1、 二进制与字符格式：LOADER可以把二进制文件读（当成字符读）列表中
2、 固定格式：记录中的数据、数据类型、 数据长度固定。
3、 可变格式：每个记录至少有一个可变长数据字段，一个记录可以是一个连续的字符串。
数据段的分界（如姓名、年龄）如用“，”作字段的 分 ；用，"’作数据
括号等
4、 LOADER可以使用多个连续字段的物理记录组成一个逻辑记录，记录文件运行情况文件：包括以下内容：
1、 运行日期：软件版本号
2、 全部输入，输出文件名；对命令行的展示信息，补充信息，
3、 对每个装入信息报告：如表名，装入情况；对初始装入， 加截入或更新装
入的选择情况，栏信息
4、 数据错误报告：错误码；放弃记录报告
5、 每个装X报告：装入行；装入行数，可能跳过行数；可能拒绝行数；可能放
弃行数等
6、 统计概要：使用空间（包大小，长度）；读入记录数，装入记录数，跳过记
录数；拒绝记录数，放弃记录数；运行时间等。
==========================================================================================================
sql load的一点小总结  
sqlldr userid=lgone/tiger control=a.ctl  
LOAD DATA  
INFILE 't.dat' // 要导入的文件  
// INFILE 'tt.date' // 导入多个文件  
// INFILE * // 要导入的内容就在control文件里 下面的BEGINDATA后面就是导入的内容  
INTO TABLE table_name // 指定装入的表  
BADFILE 'c:/bad.txt' // 指定坏文件地址  
************* 以下是4种装入表的方式  
APPEND // 原先的表有数据 就加在后面  
// INSERT // 装载空表 如果原先的表有数据 sqlloader会停止 默认值  
// REPLACE // 原先的表有数据 原先的数据会全部删除  
// TRUNCATE // 指定的内容和replace的相同 会用truncate语句删除现存数据  
************* 指定的TERMINATED可以在表的开头 也可在表的内部字段部分  
FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"'  
// 装载这种数据: 10,lg,"""lg""","lg,lg"  
// 在表中结果: 10 lg "lg" lg,lg  
// TERMINATED BY X '09' // 以十六进制格式 '09' 表示的  
// TERMINATED BY WRITESPACE // 装载这种数据: 10 lg lg  
TRAILING NULLCOLS ************* 表的字段没有对应的值时允许为空  
************* 下面是表的字段  
(  
col_1 , col_2 ,col_filler FILLER // FILLER 关键字 此列的数值不会被装载  
// 如: lg,lg,not 结果 lg lg  
)  
// 当没声明FIELDS TERMINATED BY ',' 时  
// (  
// col_1 [interger external] TERMINATED BY ',' ,  
// col_2 [date "dd-mon-yyy"] TERMINATED BY ',' ,  
// col_3 [char] TERMINATED BY ',' OPTIONALLY ENCLOSED BY 'lg'  
// )  
// 当没声明FIELDS TERMINATED BY ','用位置告诉字段装载数据  
// (  
// col_1 position(1:2),  
// col_2 position(3:10),  
// col_3 position(*:16), // 这个字段的开始位置在前一字段的结束位置  
// col_4 position(1:16),  
// col_5 position(3:10) char(8) // 指定字段的类型  
// )  
BEGINDATA // 对应开始的 INFILE * 要导入的内容就在control文件里  
10,Sql,what  
20,lg,show  
=====================================================================================  
//////////// 注意begindata后的数值前面不能有空格  
1 ***** 普通装载  
LOAD DATA  
INFILE *  
INTO TABLE DEPT  
REPLACE  
FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"'  
(DEPTNO,  
DNAME,  
LOC  
)  
BEGINDATA  
10,Sales,"""USA"""  
20,Accounting,"Virginia,USA"  
30,Consulting,Virginia  
40,Finance,Virginia  
50,"Finance","",Virginia // loc 列将为空  
60,"Finance",,Virginia // loc 列将为空  
2 ***** FIELDS TERMINATED BY WHITESPACE 和 FIELDS TERMINATED BY x'09' 的情况  
LOAD DATA  
INFILE *  
INTO TABLE DEPT  
REPLACE  
FIELDS TERMINATED BY WHITESPACE  
-- FIELDS TERMINATED BY x'09'  
(DEPTNO,  
DNAME,  
LOC  
)  
BEGINDATA  
10 Sales Virginia  
3 ***** 指定不装载那一列  
LOAD DATA  
INFILE *  
INTO TABLE DEPT  
REPLACE  
FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"'  
( DEPTNO,  
FILLER_1 FILLER, // 下面的 "Something Not To Be Loaded" 将不会被装载  
DNAME,  
LOC  
)  
BEGINDATA  
20,Something Not To Be Loaded,Accounting,"Virginia,USA"  
4 ***** position的列子  
LOAD DATA  
INFILE *  
INTO TABLE DEPT  
REPLACE  
( DEPTNO position(1:2),  
DNAME position(*:16), // 这个字段的开始位置在前一字段的结束位置  
LOC position(*:29),  
ENTIRE_LINE position(1:29)  
)  
BEGINDATA  
10Accounting Virginia,USA  
5 ***** 使用函数 日期的一种表达 TRAILING NULLCOLS的使用  
LOAD DATA  
INFILE *  
INTO TABLE DEPT  
REPLACE  
FIELDS TERMINATED BY ','  
TRAILING NULLCOLS // 其实下面的ENTIRE_LINE在BEGINDATA后面的数据中是没有直接对应  
// 的列的值的 如果第一行改为 10,Sales,Virginia,1/5/2000,, 就不用TRAILING NULLCOLS了  
(DEPTNO,  
DNAME "upper(:dname)", // 使用函数  
LOC "upper(:loc)",  
LAST_UPDATED date 'dd/mm/yyyy', // 日期的一种表达方式 还有'dd-mon-yyyy' 等  
ENTIRE_LINE ":deptno||:dname||:loc||:last_updated"  
)  
BEGINDATA  
10,Sales,Virginia,1/5/2000  
20,Accounting,Virginia,21/6/1999  
30,Consulting,Virginia,5/1/2000  
40,Finance,Virginia,15/3/2001  
6 ***** 使用自定义的函数 // 解决的时间问题  
create or replace  
function my_to_date( p_string in varchar2 ) return date  
as  
type fmtArray is table of varchar2(25);  
l_fmts fmtArray := fmtArray( 'dd-mon-yyyy', 'dd-month-yyyy',  
'dd/mm/yyyy',  
'dd/mm/yyyy hh24:mi:ss' );  
l_return date;  
begin  
for i in 1 .. l_fmts.count  
loop  
begin  
l_return := to_date( p_string, l_fmts(i) );  
exception  
when others then null;  
end;  
EXIT when l_return is not null;  
end loop;  
if ( l_return is null )  
then  
l_return :=  
new_time( to_date('01011970','ddmmyyyy') + 1/24/60/60 *  
p_string, 'GMT', 'EST' );  
end if;  
return l_return;  
end;  
/  
LOAD DATA  
INFILE *  
INTO TABLE DEPT  
REPLACE  
FIELDS TERMINATED BY ','  
TRAILING NULLCOLS  
(DEPTNO,  
DNAME "upper(:dname)",  
LOC "upper(:loc)",  
LAST_UPDATED "my_to_date( :last_updated )" // 使用自定义的函数  
)  
BEGINDATA  
10,Sales,Virginia,01-april-2001  
20,Accounting,Virginia,13/04/2001  
30,Consulting,Virginia,14/04/2001 12:02:02  
40,Finance,Virginia,987268297  
50,Finance,Virginia,02-apr-2001  
60,Finance,Virginia,Not a date  
7 ***** 合并多行记录为一行记录  
LOAD DATA  
INFILE *  
concatenate 3 // 通过关键字concatenate 把几行的记录看成一行记录  
INTO TABLE DEPT  
replace  
FIELDS TERMINATED BY ','  
(DEPTNO,  
DNAME "upper(:dname)",  
LOC "upper(:loc)",  
LAST_UPDATED date 'dd/mm/yyyy'  
)  
BEGINDATA  
10,Sales, // 其实这3行看成一行 10,Sales,Virginia,1/5/2000  
Virginia,  
1/5/2000  
// 这列子用 continueif list="," 也可以  
告诉sqlldr在每行的末尾找逗号 找到逗号就把下一行附加到上一行  
LOAD DATA  
INFILE *  
continueif this(1:1) = '-' // 找每行的开始是否有连接字符 - 有就把下一行连接为一行  
// 如 -10,Sales,Virginia,  
// 1/5/2000 就是一行 10,Sales,Virginia,1/5/2000  
// 其中1:1 表示从第一行开始 并在第一行结束 还有continueif next 但continueif list最理想  
INTO TABLE DEPT  
replace  
FIELDS TERMINATED BY ','  
(DEPTNO,  
DNAME "upper(:dname)",  
LOC "upper(:loc)",  
LAST_UPDATED date 'dd/mm/yyyy'  
)  
BEGINDATA // 但是好象不能象右面的那样使用  
-10,Sales,Virginia, -10,Sales,Virginia,  
1/5/2000 1/5/2000  
-40, 40,Finance,Virginia,13/04/2001  
Finance,Virginia,13/04/2001
8 ***** 载入每行的行号  
load data  
infile *  
into table t  
replace  
( seqno RECNUM //载入每行的行号  
text Position(1:1024))  
BEGINDATA  
fsdfasj //自动分配一行号给载入 表t 的seqno字段 此行为 1  
fasdjfasdfl // 此行为 2 ...  
9 ***** 载入有换行符的数据  
注意: unix 和 windows 不同 //n & /n  
< 1 > 使用一个非换行符的字符  
LOAD DATA  
INFILE *  
INTO TABLE DEPT  
REPLACE  
FIELDS TERMINATED BY ','  
TRAILING NULLCOLS  
(DEPTNO,  
DNAME "upper(:dname)",  
LOC "upper(:loc)",  
LAST_UPDATED "my_to_date( :last_updated )",  
COMMENTS "replace(:comments,'/n',chr(10))" // replace 的使用帮助转换换行符  
)  
BEGINDATA  
10,Sales,Virginia,01-april-2001,This is the Sales/nOffice in Virginia  
20,Accounting,Virginia,13/04/2001,This is the Accounting/nOffice in Virginia  
30,Consulting,Virginia,14/04/2001 12:02:02,This is the Consulting/nOffice in Virginia  
40,Finance,Virginia,987268297,This is the Finance/nOffice in Virginia  
< 2 > 使用fix属性  
LOAD DATA  
INFILE demo17.dat "fix 101"  
INTO TABLE DEPT  
REPLACE  
FIELDS TERMINATED BY ','  
TRAILING NULLCOLS  
(DEPTNO,  
DNAME "upper(:dname)",  
LOC "upper(:loc)",  
LAST_UPDATED "my_to_date( :last_updated )",  
COMMENTS  
)  
demo17.dat  
10,Sales,Virginia,01-april-2001,This is the Sales  
Office in Virginia  
20,Accounting,Virginia,13/04/2001,This is the Accounting  
Office in Virginia  
30,Consulting,Virginia,14/04/2001 12:02:02,This is the Consulting  
Office in Virginia  
40,Finance,Virginia,987268297,This is the Finance  
Office in Virginia  
// 这样装载会把换行符装入数据库 下面的方法就不会 但要求数据的格式不同  
LOAD DATA  
INFILE demo18.dat "fix 101"  
INTO TABLE DEPT  
REPLACE  
FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"'  
TRAILING NULLCOLS  
(DEPTNO,  
DNAME "upper(:dname)",  
LOC "upper(:loc)",  
LAST_UPDATED "my_to_date( :last_updated )",  
COMMENTS  
)  
demo18.dat  
10,Sales,Virginia,01-april-2001,"This is the Sales  
Office in Virginia"  
20,Accounting,Virginia,13/04/2001,"This is the Accounting  
Office in Virginia"  
30,Consulting,Virginia,14/04/2001 12:02:02,"This is the Consulting  
Office in Virginia"  
40,Finance,Virginia,987268297,"This is the Finance  
Office in Virginia"  
< 3 > 使用var属性  
LOAD DATA  
INFILE demo19.dat "var 3"  
// 3 告诉每个记录的前3个字节表示记录的长度 如第一个记录的 071 表示此记录有 71 个字节  
INTO TABLE DEPT  
REPLACE  
FIELDS TERMINATED BY ','  
TRAILING NULLCOLS  
(DEPTNO,  
DNAME "upper(:dname)",  
LOC "upper(:loc)",  
LAST_UPDATED "my_to_date( :last_updated )",  
COMMENTS  
)  
demo19.dat  
07110,Sales,Virginia,01-april-2001,This is the Sales  
Office in Virginia  
07820,Accounting,Virginia,13/04/2001,This is the Accounting  
Office in Virginia  
08730,Consulting,Virginia,14/04/2001 12:02:02,This is the Consulting  
Office in Virginia  
07140,Finance,Virginia,987268297,This is the Finance  
Office in Virginia  
< 4 > 使用str属性  
// 最灵活的一中 可定义一个新的行结尾符 win 回车换行 : chr(13)||chr(10)  
此列中记录是以 a|/r/n 结束的  
select utl_raw.cast_to_raw('|'||chr(13)||chr(10)) from dual;  
结果 7C0D0A  
LOAD DATA  
INFILE demo20.dat "str X'7C0D0A'"  
INTO TABLE DEPT  
REPLACE  
FIELDS TERMINATED BY ','  
TRAILING NULLCOLS  
(DEPTNO,  
DNAME "upper(:dname)",  
LOC "upper(:loc)",  
LAST_UPDATED "my_to_date( :last_updated )",  
COMMENTS  
)  
demo20.dat  
10,Sales,Virginia,01-april-2001,This is the Sales  
Office in Virginia|  
20,Accounting,Virginia,13/04/2001,This is the Accounting  
Office in Virginia|  
30,Consulting,Virginia,14/04/2001 12:02:02,This is the Consulting  
Office in Virginia|  
40,Finance,Virginia,987268297,This is the Finance  
Office in Virginia|  
==============================================================================  
象这样的数据 用 nullif 子句  
10-jan-200002350Flipper seemed unusually hungry today.  
10510-jan-200009945Sdivad over three meals.  
id position(1:3) nullif id=blanks // 这里可以是blanks 或者别的表达式  
// 下面是另一个列子 第一行的 1 在数据库中将成为 null  
LOAD DATA  
INFILE *  
INTO TABLE T  
REPLACE  
(n position(1:2) integer external nullif n='1',  
v position(3:8)  
)  
BEGINDATA  
1 10  
20lg  
------------------------------------------------------------  
如果是英文的日志 格式，可能需要修改环境变量 nls_lang or nls_date_format
==========================================================================================================
Oracle SQL*Loader 使用指南（转载）
SQL*Loader是Oracle数据库导入外部数据的一个工具.它和DB2的Load工具相似,但有更多的选择，它支持变化的加载模式，可选的加载及多表加载.
如何使用 SQL*Loader 工具
我们可以用Oracle的sqlldr工具来导入数据。例如:
sqlldr scott/tiger control=loader.ctl
控制文件(loader.ctl) 将加载一个外部数据文件（含分隔符). loader.ctl如下:
load data
infile 'c:/data/mydata.csv'
into table emp
fields terminated by "," optionally enclosed by '"'
( empno, empname, sal, deptno )
mydata.csv 如下:
10001,"Scott Tiger", 1000, 40
10002,"Frank Naude", 500, 20
下面是一个指定记录长度的示例控制文件。"*" 代表数据文件与此文件同名，即在后面使用BEGINDATA段来标识数据。
load data
infile *
replace
into table departments
( dept position (02:05) char(4),
deptname position (08:27) char(20)
)
begindata
COSC COMPUTER SCIENCE
ENGL ENGLISH LITERATURE
MATH MATHEMATICS
POLY POLITICAL SCIENCE
Unloader这样的工具
Oracle 没有提供将数据导出到一个文件的工具。但是，我们可以用SQL*Plus的select 及 format 数据来输出到一个文件：
set echo off newpage 0 space 0 pagesize 0 feed off head off trimspool on
spool oradata.txt
select col1 || ',' || col2 || ',' || col3
from tab1
where col2 = 'XYZ';
spool off
另外，也可以使用使用 UTL_FILE PL/SQL 包处理:
rem Remember to update initSID.ora, utl_file_dir='c:/oradata' parameter
declare
fp utl_file.file_type;
begin
fp := utl_file.fopen('c:/oradata','tab1.txt','w');
utl_file.putf(fp, '%s, %s/n', 'TextField', 55);
utl_file.fclose(fp);
end;
/
当然你也可以使用第三方工具，如SQLWays ,TOAD for Quest等。
加载可变长度或指定长度的记录
如：
LOAD DATA
INFILE *
INTO TABLE load_delimited_data
FIELDS TERMINATED BY "," OPTIONALLY ENCLOSED BY '"'
TRAILING NULLCOLS
( data1,
data2
)
BEGINDATA
11111,AAAAAAAAAA
22222,"A,B,C,D,"
下面是导入固定位置（固定长度）数据示例：
LOAD DATA
INFILE *
INTO TABLE load_positional_data
( data1 POSITION(1:5),
data2 POSITION(6:15)
)
BEGINDATA
11111AAAAAAAAAA
22222BBBBBBBBBB
跳过数据行：
可以用 "SKIP n" 关键字来指定导入时可以跳过多少行数据。如：
LOAD DATA
INFILE *
INTO TABLE load_positional_data
SKIP 5
( data1 POSITION(1:5),
data2 POSITION(6:15)
)
BEGINDATA
11111AAAAAAAAAA
22222BBBBBBBBBB
导入数据时修改数据：
在导入数据到数据库时，可以修改数据。注意，这仅适合于常规导入，并不适合 direct导入方式.如：
LOAD DATA
INFILE *
INTO TABLE modified_data
( rec_no "my_db_sequence.nextval",
region CONSTANT '31',
time_loaded "to_char(SYSDATE, 'HH24:MI')",
data1 POSITION(1:5) ":data1/100",
data2 POSITION(6:15) "upper(:data2)",
data3 POSITION(16:22)"to_date(:data3, 'YYMMDD')"
)
BEGINDATA
11111AAAAAAAAAA991201
22222BBBBBBBBBB990112
LOAD DATA
INFILE 'mail_orders.txt'
BADFILE 'bad_orders.txt'
APPEND
INTO TABLE mailing_list
FIELDS TERMINATED BY ","
( addr,
city,
state,
zipcode,
mailing_addr "decode(:mailing_addr, null, :addr, :mailing_addr)",
mailing_city "decode(:mailing_city, null, :city, :mailing_city)",
mailing_state
)
将数据导入多个表：
如:
LOAD DATA
INFILE *
REPLACE
INTO TABLE emp
WHEN empno != ' '
( empno POSITION(1:4) INTEGER EXTERNAL,
ename POSITION(6:15) CHAR,
deptno POSITION(17:18) CHAR,
mgr POSITION(20:23) INTEGER EXTERNAL
)
INTO TABLE proj
WHEN projno != ' '
( projno POSITION(25:27) INTEGER EXTERNAL,
empno POSITION(1:4) INTEGER EXTERNAL
)
导入选定的记录：
如下例： (01) 代表第一个字符, (30:37) 代表30到37之间的字符:
LOAD DATA
INFILE 'mydata.dat' BADFILE 'mydata.bad' DISCARDFILE 'mydata.dis'
APPEND
INTO TABLE my_selective_table
WHEN (01) <> 'H' and (01) <> 'T' and (30:37) = '19991217'
(
region CONSTANT '31',
service_key POSITION(01:11) INTEGER EXTERNAL,
call_b_no POSITION(12:29) CHAR
)
导入时跳过某些字段：
可用 POSTION(x:y) 来分隔数据. 在Oracle8i中可以通过指定 FILLER 字段实现。FILLER 字段用来跳过、忽略导入数据文件中的字段.如：
LOAD DATA
TRUNCATE INTO TABLE T1
FIELDS TERMINATED BY ','
( field1,
field2 FILLER,
field3
)
导入多行记录：
可以使用下面两个选项之一来实现将多行数据导入为一个记录:
CONCATENATE: - use when SQL*Loader should combine the same number of physical records together to form one logical record.
CONTINUEIF - use if a condition indicates that multiple records should be treated as one. Eg. by having a '#' character in column 1.
SQL*Loader 数据的提交：
一般情况下是在导入数据文件数据后提交的。
也可以通过指定 ROWS= 参数来指定每次提交记录数。
提高 SQL*Loader 的性能：
1) 一个简单而容易忽略的问题是，没有对导入的表使用任何索引和/或约束(主键)。如果这样做，甚至在使用ROWS=参数时，会很明显降低数据库导入性能。
2) 可以添加 DIRECT=TRUE来提高导入数据的性能。当然，在很多情况下，不能使用此参数。
3) 通过指定 UNRECOVERABLE选项，可以关闭数据库的日志。这个选项只能和 direct 一起使用。
4) 可以同时运行多个导入任务.
常规导入与direct导入方式的区别：
常规导入可以通过使用 INSERT语句来导入数据。Direct导入可以跳过数据库的相关逻辑(DIRECT=TRUE)，而直接将数据导入到数据文件中。
引用:http://blog.chinaunix.net/u/7040/showart_287430.html

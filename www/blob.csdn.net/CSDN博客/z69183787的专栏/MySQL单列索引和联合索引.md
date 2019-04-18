# MySQL单列索引和联合索引 - z69183787的专栏 - CSDN博客
2015年06月03日 13:10:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1564

MySQL单列索引和联合索引
所有的MySQL列类型能被索引。在相关的列上的使用索引是改进SELECT操作性能的最好方法。
一个表最多可有16个索引。最大索引长度是256个字节，尽管这可以在编译MySQL时被改变。
对于CHAR和VARCHAR列，你可以索引列的前缀。这更快并且比索引整个列需要较少的磁盘空间。在CREATE TABLE语句中索引列前缀的语法看起来像这样：
KEY index_name (col_name(length))
下面的例子为name列的头10个字符创建一个索引：
- 
CREATETABLE test (
- 
nameCHAR(200) NOTNULL,
- 
KEY index_name (name(10)));
对于BLOB和TEXT列，你必须索引列的前缀，你不能索引列的全部。
多列索引
MySQL能在多个列上创建索引。一个索引可以由最多15个列组成。（在CHAR和VARCHAR列上，你也可以使用列的前缀作为一个索引的部分)。
一个多重列索引可以认为是包含通过合并(concatenate)索引列值创建的值的一个排序数组。
当你为在一个WHERE子句索引的第一列指定已知的数量时，MySQL以这种方式使用多重列索引使得查询非常快速，即使你不为其他列指定值。
假定一张表使用下列说明创建：
- 
CREATETABLE test (
- 
id INTNOTNULL,
- 
last_name CHAR(30) NOTNULL,
- 
first_name CHAR(30) NOTNULL,
- 
PRIMARYKEY (id),
- 
INDEXname (last_name,first_name));
那么索引name是一个在last_name和first_name上的索引，这个索引将被用于在last_name或last_name和first_name的一个已知范围内指定值的查询，因此，name索引将使用在下列查询中：
- 
SELECT * FROM test WHERE last_name=”Widenius”;
- 
- 
SELECT * FROM test WHERE last_name=”Widenius”
- 
AND first_name=”Michael”;
- 
- 
SELECT * FROM test WHERE last_name=”Widenius”
- 
AND (first_name=”Michael” OR first_name=”Monty”);
- 
- 
SELECT * FROM test WHERE last_name=”Widenius”
- 
AND first_name >=”M” AND first_name < "N";
然而，name索引将不用在下列询问中：
- 
SELECT * FROM test WHERE first_name=”Michael”;
- 
- 
SELECT * FROM test WHERE last_name=”Widenius”
- 
OR first_name=”Michael”;
   联合索引又叫复合索引。对于复合索引：Mysql从左到右的使用索引中的字段，一个查询可以只使用索引中的一部份，但只能是最左侧部分。例如索引是key index （a,b,c）。 可以支持a | a,b| a,b,c 3种组合进行查找，但不支持 b,c进行查找 .当最左侧字段是常量引用时，索引就十分有效。
    两个或更多个列上的索引被称作复合索引。
    利用索引中的附加列，您可以缩小搜索的范围，但使用一个具有两列的索引 不同于使用两个单独的索引。复合索引的结构与电话簿类似，人名由姓和名构成，电话簿首先按姓氏对进行排序，然后按名字对有相同姓氏的人进行排序。如果您知 道姓，电话簿将非常有用；如果您知道姓和名，电话簿则更为有用，但如果您只知道名不姓，电话簿将没有用处。
    所以说创建复合索引时，应该仔细考虑列的顺序。对索引中的所有列执行搜索或仅对前几列执行搜索时，复合索引非常有用；仅对后面的任意列执行搜索时，复合索引则没有用处。
    如：建立 姓名、年龄、性别的复合索引。
    create table test（
    a int,
    b int,
    c int,
    KEY a（a,b,c）
    ）；
    优： select * from test where a=10 and b>50
    差： select * from test where a50
    优： select * from test order by a
    差： select * from test order by b
    差： select * from test order by c
    优： select * from test where a=10 order by a
    优： select * from test where a=10 order by b
    差： select * from test where a=10 order by c
    优： select * from test where a>10 order by a
    差： select * from test where a>10 order by b
    差： select * from test where a>10 order by c
    优： select * from test where a=10 and b=10 order by a
    优： select * from test where a=10 and b=10 order by b
    优： select * from test where a=10 and b=10 order by c
    优： select * from test where a=10 and b=10 order by a
    优： select * from test where a=10 and b>10 order by b
    差： select * from test where a=10 and b>10 order by c
![](http://linux.chinaitlab.com/UploadFiles_7565/201305/2013050416092592.jpg)
[](http://linux.chinaitlab.com/UploadFiles_7565/201305/2013050416092592.jpg)
[](http://linux.chinaitlab.com/UploadFiles_7565/201305/2013050416092592.jpg)
    索引原则
    1.索引越少越好
    原因：主要在修改数据时，第个索引都要进行更新，降低写速度。
    2.最窄的字段放在键的左边
    3.避免file sort排序，临时表和表扫描。

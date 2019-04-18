# MySQL load语句详细介绍 - z69183787的专栏 - CSDN博客
2017年07月26日 14:32:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：359
个人分类：[Mysql-基础知识](https://blog.csdn.net/z69183787/article/category/3258085)
**load的语法**
复制代码代码如下:
LOAD DATA [LOW_PRIORITY | CONCURRENT] [LOCAL] INFILE 'file_name.txt'
    [REPLACE | IGNORE]
    INTO TABLE tbl_name
    [FIELDS
        [TERMINATED BY 'string']
        [[OPTIONALLY] ENCLOSED BY 'char']
        [ESCAPED BY 'char' ]
    ]
    [LINES
        [STARTING BY 'string']
        [TERMINATED BY 'string']
    ]
    [IGNORE number LINES]
    [(col_name_or_user_var,...)]
    [SET col_name = expr,...)]
LOAD DATA INFILE语句用于高速地从一个文本文件中读取行，并装入一个表中。文件名称必须为一个文字字符串。
由character_set_database系统变量指示的字符集被用于解释文件中的信息。SET NAMES和character_set_client的设置不会影响对输入的解释。
**典型的示例**
复制代码代码如下:
LOAD DATA LOCAL INFILE 'data.txt' INTO TABLE tbl_name 
FIELDS TERMINATED BY ',' 
OPTIONALLY ENCLOSED BY '"' 
LINES TERMINATED BY '\n'
如果您只想载入一个表的部分列，则应指定一个列清单：
复制代码代码如下:
LOAD DATA LOCAL INFILE 'persondata.txt' INTO TABLE persondata (col1,col2);
**文件的路径**
如果指定了LOCAL，则被认为与连接的客户端有关：
 1.如果指定了LOCAL，则文件会被客户主机上的客户端读取，并被发送到服务器。文件会被给予一个完整的路径名称，以指定确切的位置。如果给定的是一个相对的路径名称，则此名称会被理解为相对于启动客户端时所在的目录。
 2.如果LOCAL没有被指定，则文件必须位于服务器主机上，并且被服务器直接读取。
当在服务器主机上为文件定位时，服务器使用以下规则：
1）.如果给定了一个绝对的路径名称，则服务器使用此路径名称。
2）.如果给定了带有一个或多个引导组件的相对路径名称，则服务器会搜索相对于服务器数据目录的文件。
3）.如果给定了一个不带引导组件的文件名称，则服务器会在默认数据库的数据库目录中寻找文件。
注意，这些规则意味着名为./myfile.txt的文件会从服务器数据目录中被读取，而名为myfile.txt的同样的文件会从默认数据库的数据库目录中读取。
**从客户端使用绝对路径load数据**
复制代码代码如下:
LOAD DATA LOCAL INFILE '/import/data.txt' INTO TABLE db2.my_table;
**从服务器里使用相对路径load数据**
下面的LOAD DATA语句会从db1数据库目录中读取文件data.txt，因为db1是当前数据库。即使语句明确把文件载入到db2数据库中的表里，也会从db1目录中读取。
复制代码代码如下:
USE db1;
LOAD DATA INFILE 'data.txt' INTO TABLE db2.my_table;
**IGNORE number LINES选项**
IGNORE number LINES选项可以被用于在文件的开始处忽略行。
您可以使用IGNORE 1 LINES来跳过一个包含列名称的起始标题行：
复制代码代码如下:
LOAD DATA INFILE '/tmp/test.txt'  INTO TABLE test IGNORE 1 LINES;
**REPLACE和IGNORE**
有些输入记录把原有的记录复制到唯一关键字值上。REPLACE和IGNORE关键字用于控制这些输入记录的操作。
如果您指定了REPLACE，则输入行会替换原有行（换句话说，与原有行一样，对一个主索引或唯一索引具有相同值的行）。
如果您指定IGNORE，则把原有行复制到唯一关键字值的输入行被跳过。
如果您这两个选项都不指定，则运行情况根据LOCAL关键词是否被指定而定。不使用LOCAL时，当出现重复关键字值时，会发生错误，并且剩下的文本文件被忽略。使用LOCAL时，默认的运行情况和IGNORE被指定时的情况相同；这是因为在运行中间，服务器没有办法中止文件的传输
**索引的影响**
如果您对一个空的MyISAM表使用LOAD DATA INFILE，则所有的非唯一索引会被创建在一个独立批中（对于REPAIR TABLE）。当您有许多索引时，这通常会使LOAD DATA INFILE大大加快。通常，LOAD DATA INFILE的速度会非常快，但是在某些极端情况下，您可以在把文件载入到表中之前使用ALTER TABLE...DISABLE KEYS关闭LOAD DATA INFILE，或者在载入文件之后使用ALTER TABLE...ENABLE KEYS再次创建索引，使创建索引的速度更快。
**FIELDS和LINES的默认值**
如果您不指定FIELDS子句，则默认值为假设您写下如下语句时的值：
复制代码代码如下:
FIELDS TERMINATED BY '\t' ENCLOSED BY '' ESCAPED BY '\\'
如果您不指定LINES子句，则默认值为假设您写下如下语句时的值：
复制代码代码如下:
LINES TERMINATED BY '\n' STARTING BY ''
换句话说，当读取输入值时，默认值会使LOAD DATA INFILE按如下方式运行：
复制代码代码如下:
在新行处寻找行的边界。
不会跳过任何行前缀。
在制表符处把行分解为字段。
不希望字段被包含在任何引号字符之中。
出现制表符、新行、或在‘\'前有‘\'时，理解为作为字段值一部分的文字字符。
相反的，当编写输出值时，默认值会使SELECT...INTO OUTFILE按如下方式运行：
复制代码代码如下:
在字段之间写入制表符。
不把字段包含在任何引号字符中。
当字段值中出现制表符、新行或‘\'时，使用‘\'进行转义。
在行的末端写入新行。
注意，要写入FIELDS ESCAPED BY ‘\\'，您必须为待读取的值指定两个反斜杠，作为一个单反斜杠使用。
备注:如果您已经在Windows系统中生成了文本文件，您可能必须使用LINES TERMINATED BY ‘\r\n'来正确地读取文件，因为Windows程序通常使用两个字符作为一个行终止符。部分程序，当编写文件时，可能会使用\r作为行终止符。要读取这样的文件，应使用LINES TERMINATED BY ‘\r'。
**STARTING  LINES选项**
如果所有您希望读入的行都含有一个您希望忽略的共用前缀，则您可以使用'prefix_string'来跳过前缀（和前缀前的字符）。如果某行不包括前缀，则整个行被跳过。注释：prefix_string会出现在一行的中间。
以下面的test.txt为文件源
复制代码代码如下:
xxx"row",1
something xxx"row",2
使用以下sql导入数据
复制代码代码如下:
LOAD DATA INFILE '/tmp/test.txt' INTO TABLE test LINES STARTING BY "xxx";
最后并只得到数据("row",1)和("row",2)。
**TERMINATED LINES选项**
如果jokes被由%%组成的行分隔，要读取包含jokes的文件，您可以这么操作：
复制代码代码如下:
LOAD DATA INFILE '/tmp/jokes.txt' INTO TABLE jokes FIELDS TERMINATED BY '' LINES TERMINATED BY '\n%%\n' (joke);
**TERMINATED,ENCLOSED,ESCAPED FIELD选项**
TERMINATED用于控制字段的分隔符，可以为多个字符。
ENCLOSED BY用于用于控制字段的引号，必须为单一字符,如果您忽略了词语OPTIONALLY，则所有的字段都被包含在ENCLOSED BY字符串中,如果您指定了OPTINALLY，则ENCLOSED BY字符只被用于包含具有字符串数据类型（比如CHAR, BINARY, TEXT或ENUM）的列中的值.
SELECT...INTO OUTFILE导出数据,ENCLOSED BY '"',忽略OPTIONALLY
复制代码代码如下:
"1","a string","100.20"
SELECT...INTO OUTFILE导出数据,ENCLOSED BY '"',指定OPTIONALLY
复制代码代码如下:
1,"a string",100.20
ESCAPED BY用于转义，FIELDS ESCAPED BY值必须为单一字符。
如果FIELDS ESCAPED BY字符为空字符，则没有字符被转义，并且NULL被作为NULL输出，而不是\N。去指定一个空的转义符不是一个好办法，特别是如果数据的字段值包含任何刚给定的清单中的字符时，更不能这么做。
如果在字段值内出现ENCLOSED BY字符，则通过使用ESCAPED BY字符作为前缀，对ENCLOSED BY字符进行转义。

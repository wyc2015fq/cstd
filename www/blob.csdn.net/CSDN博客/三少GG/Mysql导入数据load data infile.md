# Mysql导入数据load data infile - 三少GG - CSDN博客
2013年07月28日 14:45:40[三少GG](https://me.csdn.net/scut1135)阅读数：2821

mysql有一个高效导入方法，那就是load data infile 下面来看案例说明
基本语法：
load data  [low_priority] [local] infile 'file_name txt' [replace | ignore]
into table tbl_name
[fields
[terminated by't']
[OPTIONALLY] enclosed by '']
[escaped by'\' ]]
[lines terminated by'n']
[ignore number lines]
[(col_name,   )]
load data infile语句从一个文本文件中以很高的速度读入一个表中。使用这个命令之前，mysqld进程（服务）必须已经在运行。为了安全原因，当读取位于服务器上的文本文件时，文件必须处于数据库目录或可被所有人读取。另外，为了对服务器上文件使用load
 data infile，在服务器主机上你必须有file的权限。
1  如果你指定关键词low_priority，那么MySQL将会等到没有其他人读这个表的时候，才把插入数据。可以使用如下的命令：
**load data  low_priority infile "/home/mark/data sql" into table Orders;**
2  如果指定local关键词，则表明从客户主机读文件。如果local没指定，文件必须位于服务器上。
3  replace和ignore关键词控制对现有的唯一键记录的重复的处理。如果你指定replace，新行将代替有相同的唯一键值的现有行。如果你指定ignore，跳过有唯一键的现有行的重复行的输入。如果你不指定任何一个选项，当找到重复键时，出现一个错误，并且文本文件的余下部分被忽略。例如：
**load data  low_priority infile "/home/mark/data sql" replace into table Orders;**
4 分隔符
（1） fields关键字指定了文件记段的分割格式，如果用到这个关键字，MySQL剖析器希望看到至少有下面的一个选项：
terminated by分隔符：意思是以什么字符作为分隔符
enclosed by字段括起字符
escaped by转义字符
terminated by描述字段的分隔符，默认情况下是tab字符（\t）
enclosed by描述的是字段的括起字符。
escaped by描述的转义字符。默认的是反斜杠（backslash：\ ）  
**例如：load data infile "/home/mark/Orders txt" replace into table Orders fields terminated by',' enclosed by '"';**
（2）lines 关键字指定了每条记录的分隔符默认为'\n'即为换行符
如果两个字段都指定了那fields必须在lines之前。如果不指定fields关键字缺省值与如果你这样写的相同：fields
 terminated by'\t' enclosed by ’ '' ‘ escaped by'\\'
如果你不指定一个lines子句，缺省值与如果你这样写的相同：lines terminated by'\n'
例如：load data infile "/jiaoben/load.txt" replace into table test fields terminated by ',' lines terminated by '/n';
5 load data infile可以按指定的列把文件导入到数据库中。 当我们要把数据的一部分内容导入的时候，，需要加入一些栏目（列/字段/field）到MySQL数据库中，以适应一些额外的需要。比方说，我们要从Access数据库升级到MySQL数据库的时候
下面的例子显示了如何向指定的栏目(field)中导入数据：
**load data infile "/home/Order txt" into table Orders(Order_Number, Order_Date, Customer_ID);**
6 当在服务器主机上寻找文件时，服务器使用下列规则：
（1）如果给出一个绝对路径名，服务器使用该路径名。
（2）如果给出一个有一个或多个前置部件的相对路径名，服务器相对服务器的数据目录搜索文件。
（3）如果给出一个没有前置部件的一个文件名，服务器在当前数据库的数据库目录寻找文件。
例如： /myfile txt”给出的文件是从服务器的数据目录读取，而作为“myfile txt”给出的一个文件是从当前数据库的数据库目录下读取。
**注意：字段中的空值用\N表示**
我的文章一般浅显易懂，不会搞那么深入让大家很难理解。（其实我水平也不咋样)
LOAD DATA INFILE 一直被认为是MySQL很强大的一个数据导入工具，因为他速度非常的快。
不过有几个问题一定要注意
1、编码。
2、灵活导入导出。
我来举两个例子说明一下。
一、关于编码
我们的示例文本文件：
"我爱你","20","相貌平常，经常耍流氓！哈哈"
"李奎","21","相貌平常，经常耍流氓！哈哈"
"王二米","20","相貌平常，经常耍流氓！哈哈"
"老三","24","很强"
"老四","34","XXXXX"
"老五","52","***%*￥*￥*￥*￥"
"小猫","45","中间省略。。。"
"小狗","12","就会叫"
"小妹","21","PP的很"
"小坏蛋","52","表里不一"
"上帝他爷","96","非常英俊"
"MM来了","10","。。。"
"歌颂党","20","社会主义好"
"人民好","20","的确是好"
"老高","10","学习很好"
"斜三","60","眼睛斜了"
"中华之子","100","威武的不行了"
"大米","63","我爱吃"
"苹果","15","好吃"
我们的示例表结构：
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Table | Create Table                                                                                                                                                                                                                                                         
 |
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| t0    | CREATE TABLE `t0` (
  `id` bigint(20) unsigned NOT NULL auto_increment,
  `name` char(20) NOT NULL,
  `age` tinyint(3) unsigned NOT NULL,
  `description` text NOT NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `idx_name` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 | 
+-------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
1 row in set (0.00 sec)
我们把这个文本文件从WINDOWS 下COPY到LINUX下看看
mysql> load data infile '/tmp/t0.txt' ignore into table t0 character set gbk fields terminated by ',' enclosed by '"' lines terminated by '\n' (`name`,`age`,`description`);
Query OK, 19 rows affected (0.01 sec)
Records: 19  Deleted: 0  Skipped: 0  Warnings: 0
mysql> select * from t0;
+----+----------+-----+----------------------------+
| id | name     | age | description                |
+----+----------+-----+----------------------------+
|  1 | 我爱你   |  20 | 相貌平常，经常耍流氓！哈哈 | 
|  2 | 李奎     |  21 | 相貌平常，经常耍流氓！哈哈 | 
|  3 | 王二米   |  20 | 相貌平常，经常耍流氓！哈哈 | 
|  4 | 老三     |  24 | 很强                       | 
|  5 | 老四     |  34 | XXXXX                      | 
|  6 | 老五     |  52 | ***%*￥*￥*￥*￥           | 
|  7 | 小猫     |  45 | 中间省略。。。             | 
|  8 | 小狗     |  12 | 就会叫                     | 
|  9 | 小妹     |  21 | PP的很                     | 
| 10 | 小坏蛋   |  52 | 表里不一                   | 
| 11 | 上帝他爷 |  96 | 非常英俊                   | 
| 12 | MM来了   |  10 | 。。。                     | 
| 13 | 歌颂党   |  20 | 社会主义好                 | 
| 14 | 人民好   |  20 | 的确是好                   | 
| 15 | 老高     |  10 | 学习很好                   | 
| 16 | 斜三     |  60 | 眼睛斜了                   | 
| 17 | 中华之子 | 100 | 威武的不行了               | 
| 18 | 大米     |  63 | 我爱吃                     | 
| 19 | 苹果     |  15 | 好吃                       | 
+----+----------+-----+----------------------------+
19 rows in set (0.00 sec)
我来说明一下相关的参数
关于我的导入语句，我现在直说两个，其他的参考手册。
character set gbk;
这个字符集一定要写，要不然就会乱码或者只导入一部分数据。
ignore into table 
因为name 列加了唯一索引，加这个是为了避免重复数据插入报错。
加入我们再次运行这个导入语句就会发现
Query OK, 0 rows affected (0.00 sec)
Records: 19  Deleted: 0  Skipped: 19  Warnings: 0
没有任何值导入，因为里面已经有了相同的值。
这里也可以用replace into table
MySQL会把相同的先干掉，再插入新的值。
mysql> load data infile '/tmp/t0.txt' replace into table t0 character set gbk fields terminated by ',' enclosed by '"' lines terminated by '\n' (`name`,`age`,`description`);
Query OK, 38 rows affected (0.00 sec)
Records: 19  Deleted: 19  Skipped: 0  Warnings: 0
mysql> select * from t0;
+----+----------+-----+----------------------------+
| id | name     | age | description                |
+----+----------+-----+----------------------------+
| 20 | 我爱你   |  20 | 相貌平常，经常耍流氓！哈哈 | 
| 21 | 李奎     |  21 | 相貌平常，经常耍流氓！哈哈 | 
| 22 | 王二米   |  20 | 相貌平常，经常耍流氓！哈哈 | 
| 23 | 老三     |  24 | 很强                       | 
| 24 | 老四     |  34 | XXXXX                      | 
| 25 | 老五     |  52 | ***%*￥*￥*￥*￥           | 
| 26 | 小猫     |  45 | 中间省略。。。             | 
| 27 | 小狗     |  12 | 就会叫                     | 
| 28 | 小妹     |  21 | PP的很                     | 
| 29 | 小坏蛋   |  52 | 表里不一                   | 
| 30 | 上帝他爷 |  96 | 非常英俊                   | 
| 31 | MM来了   |  10 | 。。。                     | 
| 32 | 歌颂党   |  20 | 社会主义好                 | 
| 33 | 人民好   |  20 | 的确是好                   | 
| 34 | 老高     |  10 | 学习很好                   | 
| 35 | 斜三     |  60 | 眼睛斜了                   | 
| 36 | 中华之子 | 100 | 威武的不行了               | 
| 37 | 大米     |  63 | 我爱吃                     | 
| 38 | 苹果     |  15 | 好吃                       | 
+----+----------+-----+----------------------------+
19 rows in set (0.00 sec)
 (`name`,`age`,`description`);
这些也就是具体的表属性了，指明这个就可以导入想要的数据。
2、关于灵活性，其实也就是一个记录功能
如果想在导入的时候记录一下导入的具体时间怎么办？
我们来看看
先加一个时间属性记录导入时间。
mysql> alter table t0 add update_time timestamp not null;
Query OK, 19 rows affected (0.00 sec)
Records: 19  Duplicates: 0  Warnings: 0
干掉唯一索引
mysql> alter table t0 drop index idx_name;
Query OK, 19 rows affected (0.00 sec)
Records: 19  Duplicates: 0  Warnings: 0
mysql> load data infile '/tmp/t0.txt' into table t0 character set gbk fields terminated by ',' enclosed by '"' lines terminated by '\n' (`name`,`age`,`description`) set update_time=current_timestamp;
Query OK, 19 rows affected (0.00 sec)
Records: 19  Deleted: 0  Skipped: 0  Warnings: 0
mysql> select * from t0;
+----+----------+-----+----------------------------+---------------------+
| id | name     | age | description                | update_time         |
+----+----------+-----+----------------------------+---------------------+
| 20 | 我爱你   |  20 | 相貌平常，经常耍流氓！哈哈 | 0000-00-00 00:00:00 | 
…………
| 24 | 老四     |  34 | XXXXX                      | 0000-00-00 00:00:00 | 
| 25 | 老五     |  52 | ***%*￥*￥*￥*￥           | 0000-00-00 00:00:00 | 
…………
| 35 | 斜三     |  60 | 眼睛斜了                   | 0000-00-00 00:00:00 | 
| 36 | 中华之子 | 100 | 威武的不行了               | 0000-00-00 00:00:00 | 
…………
| 60 | 王二米   |  20 | 相貌平常，经常耍流氓！哈哈 | 2008-06-30 14:58:37 | 
…………
| 68 | 上帝他爷 |  96 | 非常英俊                   | 2008-06-30 14:58:37 | 
| 69 | MM来了   |  10 | 。。。                     | 2008-06-30 14:58:37 | 
…………
| 75 | 大米     |  63 | 我爱吃                     | 2008-06-30 14:58:37 | 
| 76 | 苹果     |  15 | 好吃                       | 2008-06-30 14:58:37 | 
+----+----------+-----+----------------------------+---------------------+
38 rows in set (0.00 sec)
新导入的19条记录时间被记录了下来。
只是之前的数据库没有记录，不过现在不需要这些重复数据了。
干掉他就可以了
mysql> alter table t0 order by id desc;
Query OK, 38 rows affected (0.01 sec)
Records: 38  Duplicates: 0  Warnings: 0
mysql> alter ignore table t0 add unique index idx_name (`name`);
Query OK, 38 rows affected (0.00 sec)
Records: 38  Duplicates: 19  Warnings: 0
mysql> alter table t0 order by id asc;
Query OK, 19 rows affected (0.01 sec)
Records: 19  Duplicates: 0  Warnings: 0
mysql> select * from t0;
+----+----------+-----+----------------------------+---------------------+
| id | name     | age | description                | update_time         |
+----+----------+-----+----------------------------+---------------------+
| 58 | 我爱你   |  20 | 相貌平常，经常耍流氓！哈哈 | 2008-06-30 14:58:37 | 
| 59 | 李奎     |  21 | 相貌平常，经常耍流氓！哈哈 | 2008-06-30 14:58:37 | 
| 60 | 王二米   |  20 | 相貌平常，经常耍流氓！哈哈 | 2008-06-30 14:58:37 | 
| 61 | 老三     |  24 | 很强                       | 2008-06-30 14:58:37 | 
| 62 | 老四     |  34 | XXXXX                      | 2008-06-30 14:58:37 | 
| 63 | 老五     |  52 | ***%*￥*￥*￥*￥           | 2008-06-30 14:58:37 | 
| 64 | 小猫     |  45 | 中间省略。。。             | 2008-06-30 14:58:37 | 
| 65 | 小狗     |  12 | 就会叫                     | 2008-06-30 14:58:37 | 
| 66 | 小妹     |  21 | PP的很                     | 2008-06-30 14:58:37 | 
| 67 | 小坏蛋   |  52 | 表里不一                   | 2008-06-30 14:58:37 | 
| 68 | 上帝他爷 |  96 | 非常英俊                   | 2008-06-30 14:58:37 | 
| 69 | MM来了   |  10 | 。。。                     | 2008-06-30 14:58:37 | 
| 70 | 歌颂党   |  20 | 社会主义好                 | 2008-06-30 14:58:37 | 
| 71 | 人民好   |  20 | 的确是好                   | 2008-06-30 14:58:37 | 
| 72 | 老高     |  10 | 学习很好                   | 2008-06-30 14:58:37 | 
| 73 | 斜三     |  60 | 眼睛斜了                   | 2008-06-30 14:58:37 | 
| 74 | 中华之子 | 100 | 威武的不行了               | 2008-06-30 14:58:37 | 
| 75 | 大米     |  63 | 我爱吃                     | 2008-06-30 14:58:37 | 
| 76 | 苹果     |  15 | 好吃                       | 2008-06-30 14:58:37 | 
+----+----------+-----+----------------------------+---------------------+
19 rows in set (0.00 sec)
现在是达到了目的了，为啥中途要干掉唯一索引呢？因为set 语法 再有IGNORE 的时候会忽略掉。
# MySQL数据导入导出方法与工具mysqlimport(1)
2005-10-17 11:19 David Euler www.w3sky.com [我要评论(0)](http://database.51cto.com/art/200510/8105.htm#commment) 字号：[T]()
 | [T]()
![一键收藏，随时查看，分享好友！](http://images.51cto.com/images/art/newart1012/images/Fav.gif)
本文讲解MySQL数据导入导出方法与工具mysqlimport的使用
AD： [2013云计算架构师峰会超低价抢票中](http://wot.51cto.com/cloud2013/)
**1.mysqlimport的语法介绍：**
mysqlimport位于mysql/bin目录中，是mysql的一个载入（或者说导入）数据的一个非常有效的工具。这是一个命令行工具。有两个参数以及大量的选项可供选择。这个工具把一个文本文件（text file）导入到你指定的数据库和表中。比方说我们要从文件Customers.txt中把数据导入到数据库Meet_A_Geek中的表Custermers中：
mysqlimport Meet_A_Geek Customers.txt 
注意：这里Customers.txt是我们要导入数据的文本文件， 而Meet_A_Geek是我们要操作的数据库， 数据库中的表名是Customers，这里文本文件的数据格式必须与Customers表中的记录格式一致，否则mysqlimport命令将会出错。
其中表的名字是导入文件的第一个句号（.）前面文件字符串，另外一个例子： 
mysqlimport Meet_A_Geek Cus.to.mers.txt 
那么我们将把文件中的内容导入到数据库Meet_A_Geek 中的Cus表中。 
上面的例子中，都只用到两个参数，并没有用到更多的选项，下面介绍mysqlimport的选项
2.mysqlimport的常用选项介绍: 
选项 功能 
-d or --delete 新数据导入数据表中之前删除数据数据表中的所有信息 
-f or --force 不管是否遇到错误，mysqlimport将强制继续插入数据 
-i or --ignore mysqlimport跳过或者忽略那些有相同唯一 
关键字的行， 导入文件中的数据将被忽略。 
-l or -lock-tables 数据被插入之前锁住表，这样就防止了， 你在更新数据库时，用户的查询和更新受到影响。 
-r or -replace 这个选项与－i选项的作用相反；此选项将替代 表中有相同唯一关键字的记录。 
--fields-enclosed- by= char 指定文本文件中数据的记录时以什么括起的， 很多情况下 数据以双引号括起。 默认的情况下数据是没有被字符括起的。
--fields-terminated- by=char 指定各个数据的值之间的分隔符，在句号分隔的文件中， 分隔符是句号。您可以用此选项指定数据之间的分隔符。 默认的分隔符是跳格符（Tab）
--lines-terminated- by=str 此选项指定文本文件中行与行之间数据的分隔字符串 或者字符。 默认的情况下mysqlimport以newline为行分隔符。 您可以选择用一个字符串来替代一个单个的字符： 一个新行或者一个回车。
mysqlimport命令常用的选项还有-v 显示版本（version）， -p 提示输入密码（password）等。 
**3.例子：导入一个以逗号为分隔符的文件**
文件中行的记录格式是这样的： 
"1", "ORD89876", "1 Dozen Roses", "19991226" 
我们的任务是要把这个文件里面的数据导入到数据库Meet_A_Geek中的表格Orders中， 我们使用这个命令:
bin/mysqlimport –prl –fields-enclosed-by=" –fields-terminated-by=, Meet_A_Geek Orders.txt
这个命令可能看起来很不爽，不过当你熟悉了之后，这是非常简单的。第一部分，bin/mysqlimport,告诉操作系统你要运行的命令是mysql/bin目录下的mysqlimport，选项p是要求输入密码，这样就要求你在改动数据库之前输入密码，操作起来会更安全。 我们用了r选项是因为我们想要把表中的唯一关键字与文件记录中有重复唯一关键字的记录替换成文件中的数据。我们表单中的数据不是最新的，需要用文件中的数据去更新，因而就用r这个选项，替代数据库中已经有的记录。l选项的作用是在我们插入数据的时候锁住表，这样就阻止了用户在我们更新表的时候对表进行查询或者更改的操作。
批处理导入文件，从sql文件导入数据到数据库中 ，批处理是一种非交互式运行mysql程序的方法，如同您在mysql中使用的命令一样，你仍然将使用这些命令。
为了实现批处理，您重定向一个文件到mysql程序中，首先我们需要一个文本文件，这个文本文件包含有与我们在mysql中输入的命令相同的文本。 //www.w3sky.com
比如我们要插入一些数据，使用包含下面文本的文件(文件名为New_Data.sql,当然我们也可以取名为New_Data.txt及任何其他的合法名字，并不一定要以后缀sql结尾)：
USE Meet_A_Geek; 
INSERT INTO Customers (Customer_ID, Last_Name) VALUES(NULL, "Block"); 
INSERT INTO Customers (Customer_ID, Last_Name) VALUES(NULL, "Newton"); 
INSERT INTO Customers (Customer_ID, Last_Name) VALUES(NULL, "Simmons"); 
注意上面的这些句子的语法都必须是正确的，并且每个句子以分号结束。 上面的USE命令选择数据库，INSERT命令插入数据。
下面我们要把上面的文件导入到数据库中，导入之前要确认数据库已经在运行，即是mysqld进程（或者说服务，Windows NT下面称为”服务“，unix下面为”进程“）已经在运行。
然后运行下面的命令： 
bin/mysql –p < /home/mark/New_Data.sql 
接着按提示输入密码，如果上面的文件中的语句没有错误，那么这些数据就被导入到了数据库中。 
命令行中使用LOAD DATA INFILE 从文件中导入数据到数据库: 
现在您可能会问自己，"究竟为什么我要输入所有的这些SQL语句到文件中，然后通过程序运行它们呢？” 这样看起来好像需要大量的工作。很好，你这样想很可能就对了。但是假如你有从所有这些命令中产生的log记录呢？现在这样就很棒，嗯，大多数数据库都会自动产生数据库中的事件记录的log。而大部分log都包含有用过的原始的SQL命令。因此，如果您不能从您现在的数据库中导出数据到新的mysql数据库中使用，那么您可以使用log和mysql的批处理特性，来快速且方便地导入您地数据。当然，这样就省去了打字的麻烦。
LOAD DATA INFILE 
这是我们要介绍的最后一个导入数据到MySQL数据库中的方法。这个命令与mysqlimport非常相似，但这个方法可以在mysql命令行中使用。也就是说您可以在所有使用API的程序中使用这个命令。使用这种方法，您就可以在应用程序中导入您想要导入的数据。 使用这个命令之前，mysqld进程（服务）必须已经在运行。
启动mysql命令行： 
bin/mysql –p 
按提示输入密码，成功进入mysql命令行之后，输入下面的命令： 
USE Meet_A_Geek; 
LOAD DATA INFILE "/home/mark/data.sql" INTO TABLE Orders; 
简单的讲，这样将会把文件data.sql中的内容导入到表Orders中，如mysqlimport工具一样，这个命令也有一些可以选择的参数。比如您需要把自己的电脑上的数据导入到远程的数据库服务器中，您可以使用下面的命令：
LOAD DATA LOCAL INFILE "C:\MyDocs\SQL.txt" INTO TABLE Orders; 
上面的LOCAL参数表示文件是本地的文件，服务器是您所登陆的服务器。 这样就省去了使用ftp来上传文件到服务器，MySQL替你完成了. 您也可以设置插入语句的优先级，如果您要把它标记为低优先级（LOW_PRIORITY），那么MySQL将会等到没有其他人读这个表的时候，才把插入数据。可以使用如下的命令：
LOAD DATA LOW_PRIORITY INFILE "/home/mark/data.sql" INTO TABLE Orders; 
您也可以指定是否在插入数据的时候，取代或者忽略文件与数据表中重复的键值。替代重复的键值的语法：
LOAD DATA LOW_PRIORITY INFILE "/home/mark/data.sql" REPLACE INTO TABLE Orders; 
上面的句子看起来有点笨拙，但却把关键字放在了让您的剖析器可以理解的地方。 //from www.w3sky.com
下面的一对选项描述了文件的记录格式，这些选项也是在mysqlimport工具中可以用的。他们在这里看起来有点不同。首先，要用到FIELDS关键字，如果用到这个关键字，MySQL剖析器希望看到至少有下面的一个选项：
TERMINATED BY character 
ENCLOSED BY character 
ESCAPED BY character 
这些关键字与它们的参数跟mysqlimport中的用法是一样的. The TERMINATED BY 描述字段的分隔符，默认情况下是tab字符（\t）
ENCLOSED BY描述的是字段的括起字符。比方以引号括起每一个字段。 
ESCAPED BY 描述的转义字符。默认的是反些杠（backslash：\ ）. 
下面仍然使用前面的mysqlimport命令的例子，用LOAD DATA INFILE语句把同样的文件导入到数据库中:
LOAD DATA INFILE "/home/mark/Orders.txt" REPLACE INTO TABLE Orders FIELDS TERMINATED BY ',' ENCLOSED BY '"';
LOAD DATA INFILE语句中有一个mysqlimport工具中没有特点： 
LOAD DATA INFILE 可以按指定的列把文件导入到数据库中。 当我们要把数据的一部分内容导入的时候，这个特点就很重要。比方说，我们要从Access数据库升级到MySQL数据库的时候，需要加入一些栏目（列/字段/field）到MySQL数据库中，以适应一些额外的需要。
这个时候，我们的Access数据库中的数据仍然是可用的，但是因为这些数据的栏目(field)与MySQL中的不再匹配，因此而无法再使用mysqlimport工具。尽管如此，我们仍然可以使用LOAD DATA INFILE，下面的例子显示了如何向指定的栏目(field)中导入数据：
LOAD DATA INFILE "/home/Order.txt" INTO TABLE Orders(Order_Number, Order_Date, Customer_ID);
如您所见，我们可以指定需要的栏目（fields）。这些指定的字段依然是以括号括起，由逗号分隔的，如果您遗漏了其中任何一个，MySQL将会提醒您
导出数据的方法：Methods of Exporting Data 
您可以看到MySQL有很多可以导入数据的方法，然而这些只是数据传输中的一半。另外的一般是从MySQL数据库中导出数据。有许多的原因我们需要导出数据。一个重要的原因是用于备份数据库。数据的造价常常是昂贵的，需要谨慎处理它们。经常地备份可以帮助防止宝贵数据地丢失；另外一个原因是，也许您希望导出数据来共享。 在这个信息技术不断成长的世界中，共享数据变得越来越常见。
比方说Macmillan USA维护护着一个将要出版的书籍的大型数据库。这个数据库在许多书店之间共享，这样他们就知道哪些书将会很快出版。医院越来越走向采用无纸病历记录，这样这些病历可以随时跟着你。世界变得越来越小，信息也被共享得越来越多。有很多中导出数据得方法，它们都跟导入数据很相似。因为，毕竟，这些都只是一种透视得方式。从数据库导出的数据就是从另一端导入的数据。这里我们并不讨论其他的数据库各种各样的导出数据的方法，您将学会如何用MySQL来实现数据导出。
使用mysqldump: 
(mysqldump命令位于mysql/bin/目录中) 
mysqldump工具很多方面类似相反作用的工具mysqlimport。它们有一些同样的选项。但mysqldump能够做更多的事情。它可以把整个数据库装载到一个单独的文本文件中。这个文件包含有所有重建您的数据库所需要的SQL命令。这个命令取得所有的模式（Schema，后面有解释）并且将其转换成DDL语法（CREATE语句，即数据库定义语句），取得所有的数据，并且从这些数据中创建INSERT语句。这个工具将您的数据库中所有的设计倒转。因为所有的东西都被包含到了一个文本文件中。这个文本文件可以用一个简单的批处理和一个合适SQL语句导回到MySQL中。这个工具令人难以置信地简单而快速。决不会有半点让人头疼地地方。
因此，如果您像装载整个数据库Meet_A_Geek的内容到一个文件中，可以使用下面的命令：
bin/mysqldump –p Meet_A_Geek > MeetAGeek_Dump_File.txt 
这个语句也允许您指定一个表进行dump(备份/导出/装载?)。如果您只是希望把数据库Meet_A_Geek中的表Orders中的整个内容导出到一个文件，可以使用下面的命令：
bin/mysqldump –p Meet_A_Geek Orders >MeetAGeek_Orders.txt 
这个非常的灵活，您甚至可以使用WHERE从句来选择您需要的记录导出到文件中。要达到这样的目的，可以使用类似于下面的命令：
bin/mysqldump –p –where="Order_ID > 2000" Meet_A_Geek Orders > Special_Dump.txt 
mysqldump工具有大量的选项，部分选项如下： 
选项/Option 作用/Action Performed 
--add-drop-table 
这个选项将会在每一个表的前面加上DROP TABLE IF
EXISTS语句，这样可以保证导回MySQL数据库的时候不会出错，因为每次导回的时候，都会首先检查表是否存在，存在就删除 
--add-locks 
这个选项会在INSERT语句中捆上一个LOCK TABLE和UNLOCK
TABLE语句。这就防止在这些记录被再次导入数据库时其他用户对表进行的操作 
-c or - complete_insert 
这个选项使得mysqldump命令给每一个产生INSERT语句加上列（field）的名字。当把数据导出导另外一个数据库时这个选项很有用。 
--delayed-insert 在INSERT命令中加入DELAY选项 
-F or -flush-logs 使用这个选项，在执行导出之前将会刷新MySQL服务器的log. 
-f or -force 使用这个选项，即使有错误发生，仍然继续导出 
--full 这个选项把附加信息也加到CREATE TABLE的语句中 
-l or -lock-tables 使用这个选项，导出表的时候服务器将会给表加锁。 
-t or -no-create- info 
这个选项使的mysqldump命令不创建CREATE TABLE语句，这个选项在您只需要数据而不需要DDL（数据库定义语句）时很方便。 
-d or -no-data 这个选项使的mysqldump命令不创建INSERT语句。 
在您只需要DDL语句时，可以使用这个选项。 
--opt 此选项将打开所有会提高文件导出速度和创造一个可以更快导入的文件的选项。 //from www.w3sky.com
-q or -quick 这个选项使得MySQL不会把整个导出的内容读入内存再执行导出，而是在读到的时候就写入导文件中。 
-T path or -tab = path 这个选项将会创建两个文件，一个文件包含DDL语句或者表创建语句，另一个文件包含数据。DDL文件被命名为table_name.sql,数据文件被命名为table_name.txt.路径名是存放这两个文件的目录。目录必须已经存在，并且命令的使用者有对文件的特权。
-w "WHERE Clause" or -where = "Where clause " 
如前面所讲的，您可以使用这一选项来过筛选将要放到 导出文件的数据。 
假定您需要为一个表单中要用到的帐号建立一个文件，经理要看今年（2004年）所有的订单（Orders），它们并不对DDL感兴趣，并且需要文件有逗号分隔，因为这样就很容易导入到Excel中。 为了完成这个人物，您可以使用下面的句子：
bin/mysqldump –p –where "Order_Date >='2000-01-01'" 
–tab = /home/mark –no-create-info –fields-terminated-by=, Meet_A_Geek Orders 
这将会得到您想要的结果。 
schema：模式 
The set of statements, expressed in data definition language, that completely describe the structure of a data base. 一组以数据定义语言来表达的语句集,该语句集完整地描述了数据库的结构。
SELECT INTO OUTFILE ： 
如果您觉得mysqldump工具不够酷，就使用SELECT INTO OUTFILE吧, MySQL同样提供一个跟LOAD DATA INFILE命令有相反作用的命令，这就是SELECT INTO OUTFILE 命令，这两个命令有很多的相似之处。首先，它们有所有的选项几乎相同。现在您需要完成前面用mysqldump完成的功能，可以依照下面的步骤进行操作：
1. 确保mysqld进程（服务）已经在运行 
2. cd /usr/local/mysql 
3. bin/mysqladmin ping ；// 如果这个句子通不过，可以用这个：mysqladmin -u root -p ping mysqladmin ping用于检测mysqld的状态，is alive说明正在运行，出错则可能需要用户名和密码。
4. 启动MySQL 监听程序. 
5. bin/mysql –p Meet_A_Geek；// 进入mysql命令行，并且打开数据库Meet_A_Geek,需要输入密码 
6. 在命令行中，输入一下命令: 
SELECT * INTO OUTFILE '/home/mark/Orders.txt'
FIELDS 
TERMINATED BY = ',' 
FROM Orders 
WHERE Order_Date >= '2000-01-01'
在你按了Return（回车）之后，文件就创建了。这个句子就像一个规则的SELECT语句，只是把想屏幕的输出重定向到了文件中。这意味这您可以使用JOIN来实现多表的高级查询。这个特点也可以被用作一个报表产生器。
比方说，您可以组合这一章中讨论的方法来产生一个非常有趣的查询，试试这个： 
在mysql目录建立一个名为Report_G.rpt 的文本文件，加入下面的行： 
USE Meet_A_Geek; 
INSERT INTO Customers (Customer_ID, Last_Name, First_Name) 
VALUES (NULL, "Kinnard", "Vicky"); 
INSERT INTO Customers (Customer_ID, Last_Name, First_Name) 
VALUES (NULL, "Kinnard", "Steven"); 
INSERT INTO Customers (Customer_ID, Last_Name, First_Name) 
VALUES (NULL, "Brown", "Sam"); 
SELECT Last_Name INTO OUTFILE '/home/mark/Report.rpt' 
FROM Customers WHERE Customer_ID > 1; 
然后确认 mysql进程在运行，并且您在mysql目录中， 输入下面的命令： 
bin/mysql < Report_G.rpt检查您命名作为输出的文件，这个文件将会包含所有您在Customers表中输入的顾客的姓。 如您所见，您可以使用今天学到的导入/导出（import/export）的方法来帮助得到报表。
翻译声明： 本文内容来自Sam's Teach Yourself MySQL in 21 Days一书的部分内容，by Mark Maslakowski 英文原文版权属原作者所有，中文的部分翻译有略有增删;原书讲的过于清楚的地方有删，讲的不清楚的地方有增；如果有翻译的不妥或者不正确的地方，请指正。
翻译者：David Euler,SCU. de_euler-david@www.yahoo.com.cn
# 通过mysqlimport定时将远程文本文件导入mysql
个人分类：[mysql](http://space.itpub.net/15480802/spacelist-blog-itemtypeid-84815)
文本文件存放在服务器a上，且文本文件的命名方式为yyyymmddhh，每小时导入24小时前生成的文件，而[**mysql**]()[**安装**]()在b上
首先在a上，从现有的文本文件截取所需的列(使用cut命令)，然后传输到b。
b这台[**服务器**]()既不能连接ftp服务器，也不能与其他机器配置用户等价性，因此向它传输文件只能使用scp，为了避免人机交互输入密码，采用了autoexpect命令
[root@justin]# autoexpect scp /var/www/data/info.txt[root@b:/var/lib/mysql/market](mailto:root@b:/var/lib/mysql/market)
autoexpect started, file is script.exp
[root@b's](mailto:root@b' rel=)password:
生成script.exp
[root@justin]# more generate_mysql_info.sh
#!/bin/bash
folder=/var/www/data/
#get the past time value
date1=`date --date='24 hour ago'  +%Y%m%d%H`
cat "$folder""$date1" | cut -d '"' -f 19,20,21,31,32,33,34,35,36,37,41,42,43,61,62,63,64 | cut -d ',' -f 2,4,5,6,8,10 > info.txt
cd $folder
./script.exp
[root@justin]# crontab -l | grep generate
#generate and scp the info.txt to b for testing
30 * * * * sh /var/www/data/generate_mysql_info.sh >> /var/www/data/generate_mysql_info.log 2>&1
传输到64之后，然后调用mysqlimport导入mysql
字段中包含中文，[**数据库**]()采用utf8字符集，为避免乱码，mysql建表语句后面加上 DEFAULT CHARSET=utf8
[root@db market]# more import_into_mysql.sh
#!/bin/bash
cd /var/lib/mysql/market
mysqlimport --user=root --password=******* --fields-enclosed-by='"' --fields-terminated-by=',' --default-character-set='utf8'  market info.txt  
[root@db market]# crontab -l
#import the info.txt into market.info
50 * * * * sh /var/lib/mysql/market/import_into_mysql.sh >> /var/lib/mysql/market/import_into_mysql.log 2>&1
至此job完成

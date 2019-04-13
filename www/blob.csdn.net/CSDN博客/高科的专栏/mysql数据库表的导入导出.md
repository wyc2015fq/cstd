
# mysql数据库表的导入导出 - 高科的专栏 - CSDN博客

2013年01月25日 00:47:56[高科](https://me.csdn.net/pbymw8iwm)阅读数：5765


MySQL写入数据通常用insert语句，如
复制代码 代码如下:
insert into person values(张三，20)，（李四，21），（王五，70）…;
但有时为了更快速地插入大批量数据或交换数据，需要从文本中导入数据或导出数据到文本。
一、 建立测试表，准备数据
首先建立一个用于测试的表示学生信息的表，字段有id、姓名、年龄、城市、薪水。Id和姓名不
能为空。
复制代码 代码如下:
create table person(
id int not null auto_increment,
name varchar(40) not null,
city varchar(20),
salary int,
primary key(id)
)engine=innodb charset=gb2312;
接着写一个用于导入的文本文件：c:\data.txt。
张三 31 北京 3000
李四 25 杭州 4000
王五 45 \N 4500
小明 29 天津 \N
每一项之间用Tab键进行分隔，如果该字段为NULL，则用\N表示。
二、 导入数据
输入命令，进行导入。
mysql>load data local infile “c:/data.txt”  into table person(name,age,city,salary);
其中local表示本地。执行后，可以看到NULL数据也被正确地导入。
三、 导出数据
现在将这个表导出为文本文件：c:\data_out.txt。
复制代码 代码如下:
mysql>select name,age,city,salary into outfile “c:/data_out.txt” lines terminated by “\r\n” from person;
其中lines terminated by “\r\n”表示每一行（即每一条记录）用\r\n分隔，\r\n是window系
统的换行符。导出的data_out.txt与data.txt的内容完全一样。
四、 运行环境
Windows vista home basic
MySQL 5.1.34-community
五、 注意
字段之间的分隔和记录（行）之间的分隔默认是\t(即Tab)和\n。但可以改变，如：
FIELDS TERMINATED BY ',' --字段用,进行分隔
LINES TERMINATED BY ';' --记录用; 进行分隔
另外要注意其它操作系统的换行符与windows可能不相同。
详细出处参考：[http://www.jb51.net/article/21117.htm](http://www.jb51.net/article/21117.htm)
不过，有可能产生中文乱码，所以你的数据库txt文件最好是保存成utf8的格式
在运行数据库的时候
mysql> set names utf8;
mysql>load data local infile "~/vote.txt" into table SYSARG_DB.t_sysarg_vote_for_works;
OK

导出数据库的表结构：
mysqldump  -uroot -p123  ron globalmarket > globalmarket.sql


mysql中把一个表的数据导入到另外一个表中的命令:
insert into table1 select * from table2





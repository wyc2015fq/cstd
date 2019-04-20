# 小白专属mysql入门 - Spark高级玩法 - CSDN博客
2018年05月23日 00:06:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：845
MYSQL-深入到精通mysql教程，请进入知识星球-spark技术学院获取
## 基本操作语句创建查看
1，创建数据库： 
进入MySql数据库环境后，就可以使用CREATE DATABASE语句创建自己的数据库了。
- 
`CREATE DATABASE 数据库名；`
- 
`>create database study;`
注1：数据库名可以由任意字母、数字、下面(_)和美元符号($)组成，但不能由单独的数字组成，也不能为MySQL关键字，而且长度还不能超过64个字符。在windows系统下，数据库名不区分大小写，在UNIX、Linux系统下，数据库名是区分大小写的，但是MySQL语句不区分大小写。 
    注意2：完整的MySQL语句必须以‘；’结尾的语句。 
2，SHOW DATABASES；
- 
`show databases;`
3,创建表
- 
`CREATE [TEMPORARY]TABLE[IF NOT EXISTS] tbl_name[(create_definition...)][table_options][select_statement]`
- 
`>create table if not exists stu_info `
方括号([])中的参数均为可选对数。 
其中，最常用的是create_definition，它是用来定义数据表的字段。语法如下：
- 
`col_name type[NOT NULL|NULL][DEFAULT default_value][AUTO_INCREMENT][UNIQUE_KEY][PRIMARY_KEY][COMMENT 'String'][reference_definition]`
方括号中是可选参数： 
注：需要在创建数据库前指定一个支持中文字符的字符集编码，推荐使用UTF-8。
- 
`create database school character set utf8 collate utf8_general_ci;`
创建下表： 
- 
`create table student(`
- 
`name varchar(20) not null comment '姓名',`
- 
`sid int(10) not null primary key comment '学员',`
- 
`majora varchar(50) not null default '' comment '专业',`
- 
`tel varchar(11) not null unique key comment '手机号',`
- 
`birthday date not null comment '出生日期'`
- 
`);`
4,SHOW语句
- 
`SHOW TABLES FROM 数据库名；`
注：查询操作默认是在当前数据库中进行的，如果不是当前使用数据库，必须指定数据库名。
- 
`SHOW COLUMNS FROM 数据表名 FROM 数据库名;`
- 
`SHOW COLUMNS FROM 数据库名字.数据表名字;`
5,DESCRIBE 语句
- 
`DESCRIBE 数据库名.数据表名;`
- 
`DESCRIBE 数据库.数据表 字段;`
## 操作表与字段
MySQL用ALTER TABLE 语句来修改数据表的结构，包括重命名数据表名称、重命名字段名称、修改字段类型、修改字段默认值、增加和删除字段等。 
注：在执行修改语句之前，都要用USE语句选择对应的数据库，否则应用'数据库.数据表'代替'数据表名'。 
1，重命名数据表名称 
语法格式
- 
`ALTER TABLE 原数据表名 RENAME 新数据表名；`
2,重命名字段名称 
语法格式
- 
`ALTER TABLE 数据表名 CHANGE 原字段名 字段定义；`
- 
`eg:`
- 
`ALTER TABLE stu CHANGE name new_name varchar(20) not null comment '姓名';`
注：这里只是修改了字段的名字，并没有修改字段类型。 
3，修改字段类型 
语法结构
- 
`ALTER TABLE 数据表名 CHANGE 字段名 字段定义；`
- 
`eg:`
- 
`ALTER TABLE stu change new_name new_name char(20) not null comment '姓名';`
注：这里只是修改了字段类型，其余定义未改变。 
3，修改字段的默认值 
语法格式
- 
`ALTER TABLE 数据表名 CHANGE 字段名 字段定义；`
- 
`eg:`
- 
`ALTER TABLE stu CHANGE major major varchar(50) not null default '电子信息工程' comment '专业';`
4，增加和删除字段 
语法格式：
- 
`ALTER TABLE 数据表名 ADD 增加的字段定义；`
- 
`eg:`
- 
`ALTER TABLE stu ADD email varchar(50) not null comment '电子邮箱';`
注：其中，增加的字段定义的语法格式与CHANGE TABLE 语句中字段的定义一样。
- 
`ALTER TABLE stu DROP email;`
5，修改字段顺序
- 
`ALTER TABLE student CHANGE name after sid;`
6，删除表结构 
语法结构
- 
`DROP TABLE [IF EXISTS] 数据表[,数据表,...]；`
- 
`eg:`
- 
`DROP TABLE IF EXISTS stu;`
通过使用[IF EXISTS] 选项，可以在删除一个不存在的表的时候得到一些警告信息。通过使用SHOW WARNINGS 语句可以查看该警告信息。
## 备份与恢复
数据库在运行时，由于自身或者外部的原因，难免会出现这样那样的问题。因此，数据库的备份和恢复显得尤为重要。 
1，备份 
mysql的bin目录下，有一个名为mysqldump的可执行文件，将该bin目录添加到环境变量中，可以利用他在“shell”环境下来备份数据库。
- 
`mysqldump --opt 要备份的数据库名 -h mysql数据库所在的域名或者IP地址 -u账户名 -p[密码] -r备份文件的路径;`
- 
`或者`
- 
`mysqldump --opt 要备份的数据库名 -h mysql数据库所在的域名或者IP地址 -u账户名 -p[密码]>备份文件的路径;`
- 
- 
`eg:`
- 
`mysqldump --opt study -h 127.0.0.1 -u root -p -r /opt/datas/backup_mysql;`
2，通过使用mysql程序备份的文件，可以将数据库恢复到指定的状态。 
语法格式：
- 
`mysql -h mysql数据库所在的域名 -u账户名 -p要恢复的数据库名<备份的路径；`
- 
`eg:`
- 
`mysql-h 127.0.0.1 -u root -p study </opt/datas/backup_mysql`
没有提示.
## 查询语句
1，简单查询语句
- 
`SELECT 字段名[,字段名...] FROM 数据表名 WHERE 条件表达式；`
该语句的含义是，从指定的数据表中，查找满足查询条件的指定字段的记录。其中，条件表达式可以是包含'>','<','>=','<=','=','!='的逻辑比较语句，也可以包含'and','or',这样的组合逻辑语句。如果不使用USE指定数据库，那么在使用'数据表名'的时候应该使用'数据库.数据表'代替。 
2，带结果排序的SELECT查询 
语法格式
- 
`SELECT 字段名[,字段名...] FROM 数据表名 `
- 
`WHERE 条件表达式 `
- 
`ORDER BY 字段名[,字段名...] [ASC|DESC]；`
3，限定结果条数的查询 
语法格式
- 
`SELECT 字段名[,字段名...] FROM 数据表名 `
- 
`WHERE 条件表达式 `
- 
`LIMIT [OFFSET,] row_count;`
注：参数OFFSET可以省略。此时默认为0. 
一般我们总是先排序再从指定的位置记录。
- 
`SELECT 字段名[,字段名...] FROM 数据表名 `
- 
`WHERE 条件表达式 `
- 
`ORDER BY 字段名[,字段名...] [ASC|DESC]`
- 
`LIMIT [OFFSET,] row_count;`
## 数据导入与导出
fields terminated by','可以指定两个字段之间隔开的方式。 
lines terminated by '
'，设置行结尾的方式 
1，导入数据
- 
`LOAD DATA LOCAL INFILE '/opt/datas/user.txt' INTO TABLE student(sid,name,tel,birthday);`
2,导出数据
- 
`select name,sid,tel,birthday  `
- 
`into outfile '/opt/datas/text.txt'  `
- 
`lines terminated by '/r/n' `
- 
`from student;`
## 插入语句
包括INSERT...VALUES和INSTER...SET两种语法形式。 
1 INSERT ...VALUES语句
- 
`INSERT INTO [数据表名(字段名，...)] VALUES(字段名，...),INSERT INTO student(name,sid,tel,birthday) values(bigdata,100,13283886131,2013-4-6);;`
- 
`eg:`
- 
`INSERT INTO student(name,sid,tel,birthday) values(bigdata,100,13283886131,2013-4-6);`
2 INSERT ...SET语句
- 
`INSERT INTO 数据表名 SET 字段名=字段值，...;`
- 
`eg:`
- 
`INSERT INTO student SET name='yarn',sid=099,tel='12345678901',birthday='2015-11-1';`
## 更新语句
- 
`UPDATE 数据表名 SET 字段名=新字段值，...`
- 
`[WHERE 条件查询]`
- 
`ORDER BY 字段名[,字段名...] [ASC|DESC]；`
- 
`eg:`
- 
`UPDATE student SET major='tech'  `
- 
`WHERE year(birthday)>1992 `
- 
`ORDER BY birthday ASC LIMIT 2;`
如果通过WHERE语句查询出的记录有多条，那么也可以使用ORDER语句进行排序。按照排序结果依次更新，还可以使用LIMIT子语句限制更新的条数。
## 删除语句
- 
`DELETE FROM 数据表名 `
- 
`[WHERE 查询条件]`
- 
`ORDER BY 字段名[,字段名...] [ASC|DESC]`
- 
`LIMIT [OFFSET,] row_count;`
如果没有WHERE子语句，则表中的所有数据将被删除。 
也可以使用ORDER BY语句进行排序，按照排序结果依次删除记录，还可以使用LIMIT子语句限制删除记录的条数 
## 高级查询语句
1，聚合函数 
DISTINCT 关键字可以去重。 
例如指令
- 
`SELECT COUNT(DISTINCT mark) FROM student`
- 
`WHERE mark >=80;`
## 分组查询
有时候需要对表中的记录按照字段进行分组，然后对每个分组分别运用聚合函数进行查询或者计算。 
语法结构如下：
- 
`SELECT [聚合函数] 字段名[,字段名...] FROM 数据表名`
- 
`WHERE 查询条件`
- 
`GROUP BY 字段名`
- 
`eg:`
- 
`SELECT mark,COUNT(DISTINCT mark) FROM student `
- 
`WHERE mark>=80 `
- 
`GROUP BY mark;`
GROUP BY语句也常与HAVING语句进行组合。
- 
`SELECT [聚合函数] 字段名[,字段名...] FROM 数据表名`
- 
`WHERE 查询条件`
- 
`GROUP BY 字段名`
- 
`HAVING  过滤条件;`
- 
`eg:`
- 
`SELECT mark,COUNT(DISTINCT mark) FROM student `
- 
`WHERE mark>=80  `
- 
`GROUP BY mark `
- 
`HAVING  mark>90;`
HAVING子语句与WHERE子语句的区别在于，WHERE在分组前对记录进行过滤，而HAVING在分组后才对记录进行过滤。 
## 联合查询
使用UNION语句将两个或者两个以上的SELECT语句的查询结果进行合并成一个结果显示。
- 
`SELECT 语句`
- 
`UNION [ALL]`
- 
`SELECT 语句`
- 
`[UNION [ALL]`
- 
`SELECT 语句`
- 
`]...`
- 
`ORDER BY 字段名[,字段名...] [ASC|DESC]`
- 
`;`
- 
- 
`eg:`
- 
`select * from student`
- 
`union ALL`
- 
`select * from student1;//字段数必须相同，字段类型顺序也尽量相同`
如果ALL参数被省略，则联合查询的结果集合中的重复行将只保留一行，否则将保留所有重复行。需要提醒的是，使用UNION运算符时应保证每个联合查询的语句的选择字段具有相同的数据类型，或者可以自动将它们转为相同的数据类型。 
注：联合查询时，查询的结果字段标题为第一个SELECT语句的字段标题。如果要对联合查询结果进行排序时，也必须使用第一个SELECT语句中的字段名。
## 连接查询
在关系型数据库中，一个数据库包括了多张数据表，表与表直接通过外键建立联系。通过连接查询，可以查询出存放在多张数据表中的信息。连接类型可以分为：交叉连接，内连接，外链接。
- 
`SELECT 字段名[,字段名...] FROM 数据表1名`
- 
`连接类型 数据表2名`
- 
`[ON 连接条件]；`
连接类型：CROSS JOIN；INNER JOIN 
1，交叉连接 
交叉连接（CROSS JOIN）返回连接表中所有数据行的笛卡尔积，其结果集合中的数据行等于第一个表中符合查询条件的数据行数乘以第二个表中符合查询条件的数据行数。
- 
`select *from student cross join student1 `
- 
`[on student.name=student1.name];`
2，内连接 
内连接(INNER JOIN)使用比较运算符进行表间某些字段数据的比较，并列出这些表中与连接条件相匹配的数据行。根据所使用的比较方式不同，内连接又分为等值连接，不等值连接，和自然连接三种。 
MySQL内连接的数据记录中，不会存在字段为NULL的情况。可以简单地认为，内链接的结果就是在左连接或者右连接的结果中剔除存在字段为NULL的记录后所得到的结果
- 
`select student.name,student1.name from student `
- 
`inner join student1 `
- 
`on student.sid==student1.sid;`
 多对多关系： 
 ``` 
 UPDATE student SET name='zkfc' 
 where name='spark';
select * from student  
inner join student1  
on student.name=student1.name; 
 ``` 
3，外连接 
外连接包括左连接(LEFT JOIN)，右连接(RIGHT JOIN).与内连接不同的是，外连接不止列出了与连接条件相匹配的行还列出了坐标，右表或者两个表中所有符合搜索条件的数据行。MySQL支持左连接，右连接，但是并不支持全连接。
- 
`select student.name,student1.name from student `
- 
`left join student1 `
- 
`on student.sid=student1.sid;`
## 子查询
有时进行查询，需要的条件是另外一条SELECT语句的结果，这个时候就要使用子查询。用子查询的关键字主要包括IN,NOT IN,=,!=,EXISTS,NOT EXISTS等。 
例子：
- 
`SELECT *FROM student `
- 
`WHERE sid IN  `
- 
`(SELECT sid FROM student1);`
**推荐阅读：**
1，[第5篇：数据库系统的实现](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484541&idx=1&sn=5ca6b67ec8c6057d560ec9d7058369aa&chksm=9f38e755a84f6e43ab62a905cc41472412574239dabc975f51e213b1866773b3c7f3b06ae464&scene=21#wechat_redirect)
2，[MySQL的索引是什么？怎么优化？](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484680&idx=1&sn=4c123ba1e7202eb65d0788863d29729d&chksm=9f38e620a84f6f3664aedbb1dfa0ede6c35fff825583f332df01b565506f39527bfaf9c0bfad&scene=21#wechat_redirect)
3，[Spark设计理念和基本架构](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484862&idx=1&sn=ffb4576cd9627a783968bef202fb3e89&chksm=9f38e696a84f6f80bccb12f3bed66c049bea288a419f5375102eab84fdd73c45b7ec6625eb0d&scene=21#wechat_redirect)
4，[如何准备Java初级和高级的技术面试](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484877&idx=1&sn=623c80aef584808391cb6a590428a5c8&chksm=9f38e6e5a84f6ff3c1ee7397a17b570ee0a07801d5006132482ba0f9e3963765e4d5165d6253&scene=21#wechat_redirect)

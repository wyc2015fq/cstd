# MySql数据库导出csv - z69183787的专栏 - CSDN博客
2017年07月25日 16:44:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：300
个人分类：[Mysql-基础知识](https://blog.csdn.net/z69183787/article/category/3258085)
**MySql数据库导出csv文件命令：**
mysql> select first_name,last_name,email from account into outfile 'e://output1.csv' fields terminated by ','optionally enclosed by ''lines terminated by '/n';
csv文件效果：
|sunny|Grigoryan|lovechoosesun@gmail.com||||||
|----|----|----|----|----|----|----|----|
|Jon|Siegal|sun@riliantech.net||||||
|Joe|Siegal|[zhao@gmail.com](mailto:zhao@gmail.com)||||||
|alejandro|medina|[wei@gmail.com](mailto:wei@gmail.com)||||||
**cvs文件导入MySql数据库命令: **
mysql> load data local infile 'e://input1.csv' into table test1 fields termin
ated by ','lines terminated by '/n'(first_name,last_name,email);
Query OK, 1 row affected, 1 warning (0.00 sec)
Records: 69  Deleted: 0  Skipped: 68  Warnings: 0
mysql> select * from test1;
+----+------------+-----------+--------------------------+
| id | first_name | last_name | email                    |
+----+------------+-----------+--------------------------+
 | 0 | sunny      | Grigoryan | lovechoosesun@gmail.com
+----+------------+-----------+--------------------------+
**FIELDS TERMINATED BY ---- 字段终止字符 OPTIONALLY ENCLOSED BY ---- 封套符 LINES TERMINATED BY ---- 行终止符**
通过mysql客户端shell连接到服务器，选择使用的数据库，输入sql代码： 
select * from test_info   
into outfile '/tmp/test.csv'   
fields terminated by ',' optionally enclosed by '"' escaped by '"'   
lines terminated by '\r\n'; 
里面最关键的部分就是格式参数
这个参数是根据RFC4180文档设置的，该文档全称Common Format and MIME Type for Comma-Separated Values (CSV) Files，其中详细描述了CSV格式，其要点包括：
(1)字段之间以逗号分隔，数据行之间以\r\n分隔；
(2)字符串以半角双引号包围，字符串本身的双引号用两个双引号表示。
通过执行上述代码，便可以将需要的数据以csv格式导出到执行的文件中。
另外，MySQL中导入CSV格式数据的sql代码如下:
load data infile '/tmp/test.csv'  
into table test_info   
fields terminated by ','  optionally enclosed by '"' escaped by '"'  
lines terminated by '\r\n';
当csv文件中的每一行记录的列数小于数据库表时，以下语句将十分有用：
load data infile "/tmp/appleAppV2_search_day_20110525.csv" into table apple_search_log fields terminated by ';' lines terminated by '/n' (apptypeId,keyword,searchTime,model) ;
蓝色字部分是亮点。如果数据表包含自增字段，例如自增的ID， 这个语句将十分有用。

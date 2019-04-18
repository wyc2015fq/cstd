# com.mysql.jdbc.MysqlDataTruncation: Data truncation: Data too long for column 'title' at row 1 - z69183787的专栏 - CSDN博客
2015年05月22日 13:02:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2175
做实验用到wiki的数据，想要把xml格式的内容转存到数据库中，结果碰到了好多问题。
com.mysql.jdbc.MysqlDataTruncation: Data truncation: Data too long for column 'title' at row 1
从页面上输入中文保存到MySQL数据库时候就会出现上面的问题，并且输入的中文不是过长。这种情况下应该是字符集的问题。
建表语句为：
create table article
(
   id int primary key auto_increment,
   pid int,
   rootid int,
   title varchar(255),
   content text,
   pdate datetime,
   isleaf int
)engine innodb default charset=gbk;
用命令：show table status like 'article'; 就可以看到表的状态信息，其中的Collation项为：gbk_chinese_ci，因此MYSQL JDBC连接参数要设置为一致：
String url = "jdbc:mysql://localhost/bbs? user=root&password=1234&useUnicode=true&characterEncoding=gbk";
Connection conn = DriverManager.getConnection(url);
后来发现我确实是在数据库中设置的text长度不够，mysql中text类型是可变长度的字符串，最多65535个字符；把字段类型改成MEDIUMTEXT（最多存放16777215个字符）或者LONGTEXT（最多存放4294967295个字符）就解决了。

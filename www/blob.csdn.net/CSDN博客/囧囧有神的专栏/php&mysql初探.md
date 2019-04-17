# php&mysql初探 - 囧囧有神的专栏 - CSDN博客





2012年05月14日 11:52:53[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：1413标签：[mysql																[php																[数据库																[database																[sql																[query](https://so.csdn.net/so/search/s.do?q=query&t=blog)
个人分类：[linux																[语言及算法](https://blog.csdn.net/liujiyong7/article/category/1098491)](https://blog.csdn.net/liujiyong7/article/category/1098485)





1.连接数据库

shell：

Mysql -u$user -p [$database]

Php:

    $con = mysql_connect($Host,$DbaUser,$DbaPassword);

2.展示数据库，选择数据库

show databases; 


show tables;

describe table;


Shell:

Use mysql

Php:

$db_selected = mysql_select_db($Database,$con);

3.查询数据

Mysql 子命令:

Select * from table where

Php:

$sql = "select * from showlimits_$Dtime order by R_RATE desc";

$result = mysql_query($sql,$con);




允许某台主机访问数据库

mysql子命令下：

GRANT ALL PRIVILEGES ON *.* TO 'root'@'10.26.170.14' IDENTIFIED BY 'mypassword' WITH GRANT OPTION;

flush privileges;

mysql 数据类型：http://www.cnblogs.com/bukudekong/archive/2011/06/27/2091590.html 




select now(); 可获取当前时间




phpadmin 安装过程：http://www.51099.com/comp/damy/20101012/353921.html](https://so.csdn.net/so/search/s.do?q=sql&t=blog)](https://so.csdn.net/so/search/s.do?q=database&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)





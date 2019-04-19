# 如何解决PHP里大量数据循环时内存耗尽的问题 - 文章 - 伯乐在线
原文出处： [WEB骇客](http://www.webhek.com/php-buffered-and-unbuffered-queries)
最近在开发一个PHP程序时遇到了下面的错误：
> 
PHP Fatal error: Allowed memory size of 268 435 456 bytes exhausted
错误信息显示允许的最大内存已经耗尽。遇到这样的错误起初让我很诧异，但转眼一想，也不奇怪，因为我正在开发的这个程序是要用一个`foreach`循环语句在一个有4万条记录的表里全表搜索具有特定特征的数据，也就是说，一次要把4万条数据取出，然后逐条检查每天数据。可想而知，4万条数据全部加载到内存中，内存不爆才怪。
毕竟编程这么多年，我隐约记得PHP里提供有非一次全部加载数据的API，是像处理流媒体那样，随用随取随丢、数据并不会积累在内存的查询方法。经过简单的搜索，果然在官方网站上找到的正确的用法。
这个问题在PHP的官方网站上叫[缓冲查询和非缓冲查询(Buffered and Unbuffered queries)](http://php.net/manual/zh/mysqlinfo.concepts.buffering.php)。PHP的查询缺省模式是缓冲模式。也就是说，查询数据结果会一次全部提取到内存里供PHP程序处理。这样给了PHP程序额外的功能，比如说，计算行数，将指针指向某一行等。更重要的是程序可以对数据集反复进行二次查询和过滤等操作。但这种缓冲查询模式的缺陷就是消耗内存，也就是用空间换速度。
相对的，另外一种PHP查询模式是非缓冲查询，数据库服务器会一条一条的返回数据，而不是一次全部返回，这样的结果就是PHP程序消耗较少的内存，但却增加了数据库服务器的压力，因为数据库会一直等待PHP来取数据，一直到数据全部取完。
很显然，缓冲查询模式适用于小数据量查询，而非缓冲查询适应于大数据量查询。
对于PHP的缓冲模式查询大家都知道，下面列举的例子是如何执行非缓冲查询API。
## 非缓冲查询方法一: mysqli

PHP
```
<?php
$mysqli  = new mysqli("localhost", "my_user", "my_password", "world");
$uresult = $mysqli->query("SELECT Name FROM City", MYSQLI_USE_RESULT);
if ($uresult) {
   while ($row = $uresult->fetch_assoc()) {
       echo $row['Name'] . PHP_EOL;
   }
}
$uresult->close();
?>
```
## 非缓冲查询方法二: pdo_mysql

PHP
```
<?php
$pdo = new PDO("mysql:host=localhost;dbname=world", 'my_user', 'my_pass');
$pdo->setAttribute(PDO::MYSQL_ATTR_USE_BUFFERED_QUERY, false);
$uresult = $pdo->query("SELECT Name FROM City");
if ($uresult) {
   while ($row = $uresult->fetch(PDO::FETCH_ASSOC)) {
       echo $row['Name'] . PHP_EOL;
   }
}
?>
```
## 非缓冲查询方法三: mysql

PHP
```
<?php
$conn = mysql_connect("localhost", "my_user", "my_pass");
$db   = mysql_select_db("world");
$uresult = mysql_unbuffered_query("SELECT Name FROM City");
if ($uresult) {
   while ($row = mysql_fetch_assoc($uresult)) {
       echo $row['Name'] . PHP_EOL;
   }
}
?>
```

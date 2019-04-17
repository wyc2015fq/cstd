# php中对Mysql数据库的访问操作 - 在思索中前行！ - CSDN博客





2014年06月19日 11:54:46[_Tham](https://me.csdn.net/txl16211)阅读数：842








一： 
PHP-MySQL 是 PHP 操作 MySQL 资料库最原始的 Extension ，PHP-MySQLi 的 i 代表 Improvement ，提更了相对进阶的功能，就 Extension 而言，本身也增加了安全性。而 PDO (PHP Data Object) 则是提供了一个
 Abstraction Layer 来操作资料库，用讲的其实看不出来有有什麽差别，所以就直接看程序吧… 

首先，先来看一段用 PHP-MySQL 写成的程式码，这类的范例常用在世界各地：



<?php 

mysql_connect($db_host, $db_user, $db_password); 

mysql_select_db($dn_name); 

$result = mysql_query("SELECT `name` FROM `users` WHERE `location` = '$location'"); 

while ($row = mysql_fetch_array($result, MYSQL_ASSOC)) 

{ 

echo $row['name']; 

} 

mysql_free_result($result); 

?> 


乍看之下没什麽问题，但其实背后有些学问… 

这种方式不能 Bind Column ，以前例的 SQL 叙述来说，$location 的地方容易被 SQL Injection。后来于是发展出了 mysql_escape_string() (备注：5.3.0之后弃用) 以及 mysql_real_escape_string() 来解决这个问题，不过这麽一搞，整个叙述会变得複杂且丑陋，而且如果栏位多了，可以想见会是怎样的情形… 

<?php 

$query = sprintf("SELECT * FROM users WHERE user='%s' AND password='%s'", 

mysql_real_escape_string($user), 

mysql_real_escape_string($password)); 

mysql_query($query); 

?> 


在 PHP-MySQLi 中有了不少进步，除了透过 Bind Column 来解决上述问题，而且也多援 Transaction, Multi Query ，并且同时提供了 Object oriented style (下面这段
 PHP-MySQLi 范例的写法) 和 Procedural style (上面 PHP-MySQL 范例的写法)两种写法…等等。

<?php 

$mysqli = new mysqli($db_host, $db_user, $db_password, $db_name); 

$sql = "INSERT INTO `users` (id, name, gender, location) VALUES (?, ?, ?, ?)"; 

$stmt = $mysqli->prepare($sql); 

$stmt->bind_param('dsss', $source_id, $source_name, $source_gender, $source_location); 

$stmt->execute(); 

$stmt->bind_result($id, $name, $gender, $location); 

while ($stmt->fetch()) 

{ 

echo $id . $name . $gender . $location; 

} 

$stmt->close(); 

$mysqli->close(); 

?> 


但看到这边又发现了一些缺点，例如得 Bind Result，这个就有点多馀，不过这其实无关紧要，因为最大的问题还是在于这不是一个抽象(Abstraction)的方法，所以当后端更换资料库的时候，就是痛苦的开始… 

于是 PDO 就出现了(备注：目前 Ubuntu 和 Debian 来说，PDO 并没有直接的套件可以安装，而是必须透过 PECL 安装)。 

roga@carlisten-lx:~$ pecl search pdo 

======================================= 

Package Stable/(Latest) Local 

PDO 1.0.3 (stable) PHP Data Objects Interface. 

PDO_4D 0.3 (beta) PDO driver for 4D-SQL database 

PDO_DBLIB 1.0 (stable) FreeTDS/Sybase/MSSQL driver for PDO 

PDO_FIREBIRD 0.2 (beta) Firebird/InterBase 6 driver for PDO 

PDO_IBM 1.3.2 (stable) PDO driver for IBM databases 

PDO_INFORMIX 1.2.6 (stable) PDO driver for IBM Informix INFORMIX databases 

PDO_MYSQL 1.0.2 (stable) MySQL driver for PDO 

PDO_OCI 1.0 (stable) Oracle Call Interface driver for PDO 

PDO_ODBC 1.0.1 (stable) ODBC v3 Interface driver for PDO 

PDO_PGSQL 1.0.2 (stable) PostgreSQL driver for PDO 

PDO_SQLITE 1.0.1 (stable) SQLite v3 Interface driver for PDO 

pdo_user 0.3.0 (beta) Userspace driver for PDO 


当透过 PECL 安装装好后，就可以透过以下方式来操作资料库： 

<?php 

$dsn = "mysql:host=$db_host;dbname=$db_name"; 

$dbh = new PDO($dsn, $db_user, $db_password); 

$sql = "SELECT `name`, `location` FROM `users` WHERE `location` = ? , `name` = ?"; 

$sth = $dbh->prepare($sql); 

$sth->execute(array($location, $name)); 

$result = $sth->fetch(PDO::FETCH_OBJ); 

echo $result->name . $result->location; 

$dbh = NULL; 

?> 

乍看之下，PDO 的程式码好像也没有比较短，那到底好处是什麽呢？ 

1. PDO 连接资料库时透过 Connection String 来决定连接何种资料库。 

2. PDO 可以透过 PDO::setAttribute 来决定连线时的设定，像是 Persistent Connection, 回传错误的方式(Exception, E_WARNING, NULL)。甚至是回传栏位名称的大小写…等等。 

2. PDO 支援 Bind Column 的功能，除了基本的 Prepare, Execute 以外，也可以 Bind 单一栏位，并且指定栏位型态。 

4. PDO 是 Abstraction Layer 所以就算更换储存媒介，需要花的功夫比起来是最少的。 

可惜的是，儘管这些东西都已经出现很久了，但还是不够大众化。我想或许是肇因于大家习惯看坊间的书籍学习，但那些书本往往只会介绍最简单最传统的方式。导致很多人还是在用 MySQL 这种方直接连资料库。 

不过，目前来说我个人还是最喜爱透过 DBI 来连接资料库，像是 ActiveRecord 以及 Propel ORM(Object-Relational Mapping)。 

例如说以 ActiveRecord 为例，如果要实现这样的 SQL 叙述… 

INSERT INTO `users` (id, name, gender, location) VALUES(1, 'roga', 'male', 'tpe') 

以 PDO 来写是： 

<?php 

$sql = "INSERT INTO `users` (id, name, gender, location) VALUES(?, ?, ?, ?)"; 

$sth = $dbh->prepare($sql); 

$sth->execute(array(1, 'roga', 'male', 'tpe')); 

?> 


但以 ActiveRecord 来说的话，则是： 

<?php 

$user = new User(); 

$user->id = 1; 

$user->name = 'roga'; 

$user->gender = 'male'; 

$user->location = 'tpe'; 

$user->save(); 

?> 


后者在语法上是不是简洁很多呢，而且也大幅降低对 SQL 语言的依赖性！(不同资料库对 SQL 实作的问题可参考 Comparison of different SQL implementations) 


mysql是非持继连接函数而mysqli是永远连接函数。也就是说 

mysql每次链接都会打开一个连接的进程而mysqli多次运行mysqli将使用同一连接进程,从而减少了服务器的开销 

有些朋友在编程的时候，使用new mysqli('localhost', usenamer', 'password', 'databasename');总是报 

错，Fatal error: Class 'mysqli' not found in d:\... 

mysqli类不是php自带的吗？ 

不是默认开启的，win下要改php.ini,去掉php_mysqli.dll前的;,linux下要把mysqli编译进去。 

一:Mysqli.dll是一个允许以对象的方式或者过程操作数据库的，它的使用方式也很容易。这里就几个常见的操作和 mysql.dll做一个对比。 

　　１:  mysql.dll(可以理解为函数式的方式)： 


复制代码代码如下:



　　$conn = mysql_connect('localhost', 'user', 'password');　//连接mysql数据库 

　　mysql_select_db('data_base'); //选择数据库 


　　$result = mysql_query('select * from data_base');//这里有第二个可选参数，指定打开的连接 

　　$row = mysql_fetch_row( $result ) ) //为了简单，这里只取一行数据 

　　echo $row[0]; //输出第一个字段的值 


　　mysqli也有过程式的方式，只不过开始贯以mysqli的前缀，其他都差不多。如果mysqli以过程式的方式操作的话， 有些函数必须指定资源，比如说 mysqli_query(资源标识,SQL语句),并且资源标识的参数是放在前面的，而 mysql_query(SQL语句,'可选')的资源标识是放在后面的，并且可以不指定，它默认是上一个打开的连接或资源。 
　　２  mysqli.dll(对象方式)： 

复制代码代码如下:



　　$conn = new mysqli('localhost', 'user', 'password','data_base'); 

　　//这里的连接是new出来的，最后一个参数是直接指定数据库，不用                   mysql_select_db()了 

　　//也可以构造时候不指定，然后 $conn -> select_db('data_base') 

　　$result = $conn -> query( 'select * from data_base' ); 

　　$row = $result -> fetch_row(); //取一行数据 

　　echo row[0]; //输出第一个字段的值 


　　二：mysql_fetch_row(),mysql_fetch_array() 
　　这两个函数，返回的都是一个数组，区别就是第一个函数返回的数组是只包含值，我们只能$row[0], 
      $row[1],这样以数组下标来读取数据，而mysql_fetch_array()返回的数组既包含第一种，也包含键值 
      对的形式，我们可以这样读取数据，（假如数据库的字段是 username,passwd）: 
　　$row['username'], $row['passwd'] 
　　而且，如果用($row as $kay => $value)来操作的话，还以直接取得数据库的字段名称。 
　　更主要的是mysqli是php5提供的新函数库，(i)表示改进，其执行速度更快.






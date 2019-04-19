# 常用PHP MySQL函数 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年05月31日 15:56:17[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：285
## 配置选项
MySQL 函数的行为受到 php.ini 中设置的影响。
- mysql.allow_persistent "1" 是否允许 MySQL 的持久连接。
- mysql.max_persistent "-1" 每个进程中最大的持久连接数目。
- mysql.max_links "-1" 每个进程中最大的连接数，包括持久连接。
- mysql.default_port NULL 指定默认连接数据库的 TCP 端口号。
- mysql.default_host NULL 默认的服务器地址。不适用于SQL安全模式。
- mysql.default_user NULL 默认使用的用户名。不适用于SQL安全模式。
- mysql.default_password NULL 默认使用的密码。不适用于SQL安全模式。
- mysql.connect_timeout "60" 连接超时秒数。
在 MySQL 模块中使用了两种资源类型。第一种是数据库的连接句柄，第二种是 SQL 查询返回的结果集。
## MySQL 函数
### mysql_affected_rows()
返回前一次 MySQL 操作所影响的记录行数，如果最近一次查询失败的话，函数返回 -1。
### mysql_client_encoding()
返回当前连接的字符集名称(character_set 变量的值)
### mysql_close()
关闭非持久的 MySQL 连接。成功返回 true，失败返回 false。
通常不需要使用 mysql_close()，因为已打开的非持久连接会在脚本执行完毕后自动关闭。
注：mysql_close() 不会关闭由 mysql_pconnect() 建立的持久连接。
### mysql_connect()
打开非持久的 MySQL 连接。成功返回一个 MySQL 连接标识，失败返回 FALSE。
mysql_connect(server,user,pwd,newlink,clientflag)
其中：
- newlink：可选。如果用同样的参数第二次调用 mysql_connect()，将不会建立新连接，而将返回已经打开的连接标识。参数 new_link 改变此行为并使 mysql_connect() 总是打开新的连接，甚至当 mysql_connect() 曾在前面用同样的参数调用过。
- clientflag 可选。client_flags 参数可以是以下常量的组合：
- MYSQL_CLIENT_SSL - 使用 SSL 加密
- MYSQL_CLIENT_COMPRESS - 使用压缩协议
- MYSQL_CLIENT_IGNORE_SPACE - 允许函数名后的间隔
- MYSQL_CLIENT_INTERACTIVE - 允许关闭连接之前的交互超时非活动时间
### mysql_pconnect()
打开一个到 MySQL 服务器的持久连接。出错返回 FALSE。
mysql_pconnect() 和 mysql_connect() 非常相似，但有两个主要区别：
- 当连接的时候本函数将先尝试寻找一个在同一个主机上用同样的用户名和密码已经打开的（持久）连接，如果找到，则返回此连接标识而不打开新连接。
- 其次，当脚本执行完毕后到 SQL 服务器的连接不会被关闭，此连接将保持打开以备以后使用。
### mysql_select_db()
选择 MySQL 数据库。如果成功，则该函数返回 true。如果失败，则返回 false。
### mysql_stat()
返回 MySQL 服务器的当前系统状态。如果成功，则该函数返回状态。如果失败，则返回 false。要得到其它状态变量的完整列表，可使用 SQL 命令 SHOW STATUS。
### mysql_ping()
Ping 一个服务器连接，如果没有连接则重新连接。如果存在连接，则返回 true。如果失败，则返回 false。
本函数可用于空闲很久的脚本来检查服务器是否关闭了连接。
### mysql_query()
执行一条 MySQL 查询。
mysql_query() 仅对 SELECT，SHOW，EXPLAIN 或 DESCRIBE 语句返回一个资源标识符，如果查询执行不正确则返回 FALSE。对于其它类型的 SQL 语句，mysql_query() 在执行成功时返回 TRUE，出错时返回 FALSE。
函数自动对记录集进行读取和缓存。如需运行非缓存查询，请使用 mysql_unbuffered_query()。
### mysql_real_escape_string()
转义 SQL 语句中字符串中的特殊字符。如果成功，则该函数返回被转义的字符串。如果失败，则返回 false。可使用本函数来预防数据库攻击。
**<?php**$con=mysql_connect("localhost","hello","321");if(!$con){    die('Could not connect: '.mysql_error());}*// 获得用户名和密码的代码**// 转义用户名和密码，以便在 SQL 中使用*$user=mysql_real_escape_string($user);$pwd=mysql_real_escape_string($pwd);$sql="SELECT * FROM users WHERE user='"  .$user.  "' AND password='"  .$pwd.  "'"*// 更多代码*mysql_close($con);
数据库攻击。本例演示如果我们不对用户名和密码应用 mysql_real_escape_string() 函数会发生什么：
**<?php**$con=mysql_connect("localhost","hello","321");if(!$con){die('Could not connect: '.mysql_error());}$sql="SELECT * FROM users WHERE user='**{$_POST['user']}**' AND password='**{$_POST['pwd']}**'";mysql_query($sql);*// 不检查用户名和密码**// 可以是用户输入的任何内容，比如：*$_POST['user']='john';  $_POST['pwd']="' OR ''='";*// 一些代码...*mysql_close($con);**?>**
那么 SQL 查询会成为这样：
SELECT ***** FROM **users**
WHERE user='john' AND password='' OR ''=''
这意味着任何用户无需输入合法的密码即可登陆。
预防数据库攻击的正确做法：
**<?php****function** check_input($value){*    // 去除斜杠*    if(get_magic_quotes_gpc()){        $value=stripslashes($value);    }*    // 如果不是数字则加引号*    if(!is_numeric($value)){        $value="'".mysql_real_escape_string($value)."'";    }    return $value;}$con=mysql_connect("localhost","hello","321");if(!$con){die('Could not connect: '.mysql_error());}*// 进行安全的 SQL*$user= check_input($_POST['user']);$pwd= check_input($_POST['pwd']);$sql="SELECT * FROM users WHERE
user=**$user** AND password=**$pwd**";mysql_query($sql);mysql_close($con);**?>**
### mysql_result()
根据行号和字段返回结果集中一个字段的值。如果成功，则该函数返回字段值。如果失败，则返回 false。
### mysql_data_seek()
移动内部结果的指针。成功返回 true，失败返回 false。
mysql_data_seek(data,row)
- data 必需。返回类型为 resource 的结果集。该结果集从 mysql_query() 调用中得到。
- row 必需。想要设定的新的结果集指针的行数。0 指示第一个记录。
mysql_data_seek() 将 data 参数指定的 MySQL 结果内部的行指针移动到指定的行号。
接着调用 mysql_fetch_row() 将返回那一行。
row 从 0 开始。row 的取值范围应该从 0 到 mysql_num_rows - 1。
但是如果结果集为空（mysql_num_rows() == 0），要将指针移动到 0 会失败并发出 E_WARNING 级的错误，mysql_data_seek() 将返回 false。
注：mysql_data_seek() 只能和 mysql_query() 一起使用，而不能用于 mysql_unbuffered_query()。
### [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8PHP_MySQL%E5%87%BD%E6%95%B0&action=edit&section=15)]mysql_errno()
返回上一个 MySQL 函数的错误号码，如果没有出错则返回 0。
### [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8PHP_MySQL%E5%87%BD%E6%95%B0&action=edit&section=16)]mysql_error()
返回上一个 MySQL 函数的错误文本，如果没有出错则返回 *（空字符串）。*
### [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8PHP_MySQL%E5%87%BD%E6%95%B0&action=edit&section=17)]mysql_fetch_array()
从结果集中取得一行作为关联数组，或数字数组，或二者兼有。如果没有更多行则返回 false。
mysql_fetch_array(data,array_type)
其中：
- array_type 可选。规定返回哪种结果。可能的值：MYSQL_ASSOC - 关联数组；MYSQL_NUM - 数字数组；MYSQL_BOTH - 默认。同时产生关联和数字数组
mysql_fetch_array() 是 mysql_fetch_row() 的扩展版本。除了将数据以数字索引方式储存在数组中之外，还可以将数据作为关联索引储存，用字段名作为键名。注：返回的字段名是区分大小写的。
### [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8PHP_MySQL%E5%87%BD%E6%95%B0&action=edit&section=18)]mysql_fetch_assoc()
从结果集中取得一行作为关联数组。如果没有更多行，则返回 false。注：本函数返回的字段名是区分大小写的。
mysql_fetch_assoc() 和用 mysql_fetch_array() 加上第二个可选参数 MYSQL_ASSOC 完全相同。
### [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8PHP_MySQL%E5%87%BD%E6%95%B0&action=edit&section=19)]mysql_fetch_object()
从结果集中取得一行作为对象。
若成功的话，本函数从 mysql_query() 获得一行，并返回一个对象。如果失败或没有更多的行，则返回 false。
每个随后对 mysql_fetch_object() 的调用都会返回记录集中的下一行。
注：mysql_fetch_object() 与 mysql_fetch_array() 类似，只有一点区别 - 返回的是对象而不是数组。也意味着只能通过字段名来访问数组，而不是偏移量。
### [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8PHP_MySQL%E5%87%BD%E6%95%B0&action=edit&section=20)]mysql_fetch_row()
从结果集中取得一行作为数字数组。依次调用 mysql_fetch_row() 将返回结果集中的下一行，如果没有更多行则返回 FALSE。
### [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8PHP_MySQL%E5%87%BD%E6%95%B0&action=edit&section=21)]mysql_field_name()
取得结果中指定字段的字段名。如果成功，则返回字段的名称，如果失败，则返回 false。注：返回的字段名是区分大小写的。
### [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8PHP_MySQL%E5%87%BD%E6%95%B0&action=edit&section=22)]mysql_free_result()
释放结果内存。如果成功返回 true，如果失败，则返回 false。
注：mysql_free_result() 仅需要在返回很大的结果集时会占用很多内存时调用。在脚本结束后所有关联的内存都会被自动释放。
### [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8PHP_MySQL%E5%87%BD%E6%95%B0&action=edit&section=23)]mysql_insert_id()
返回上一步 INSERT 操作产生的 ID。如果上一查询没有产生 AUTO_INCREMENT 的 ID，则 mysql_insert_id() 返回 0。
### [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8PHP_MySQL%E5%87%BD%E6%95%B0&action=edit&section=24)]mysql_num_fields()
返回结果集中字段的数目。如果失败，则返回 false。
### [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8PHP_MySQL%E5%87%BD%E6%95%B0&action=edit&section=25)]mysql_num_rows()
返回结果集的行数。此命令仅对 SELECT 语句有效。要取得被 INSERT，UPDATE 或者 DELETE 查询所影响到的行的数目，用 mysql_affected_rows()。
### [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8PHP_MySQL%E5%87%BD%E6%95%B0&action=edit&section=26)]mysql_fetch_field()
从结果集中取得列信息并作为对象返回。从查询结果中取得字段的信息。如果没有指定字段偏移量，则提取下一个尚未被 mysql_fetch_field() 取得的字段。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%B8%B8%E7%94%A8PHP_MySQL%E5%87%BD%E6%95%B0&action=edit&section=27)]艺搜参考
[http://www.w3school.com.cn/php/php_ref_mysql.asp](http://www.w3school.com.cn/php/php_ref_mysql.asp)

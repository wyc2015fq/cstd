# PHP 安全性漫谈 - 文章 - 伯乐在线
原文出处： [彭长霖](http://www.tingm.cc/doku.php?id=php:php%E5%AE%89%E5%85%A8%E6%80%A7%E6%BC%AB%E8%B0%88)
本文所讨论的安全性环境是在Linux+Apache+Mysql+PHP。超出此范围的安全性问题不在本文范畴之内
# 一、apache server安全性设置
#### 1、以Nobody用户运行
一般情况下，Apache是由Root 来安装和运行的。如果Apache Server进程具有Root用户特权，那么它将给系统的安全构成很大的威胁，应确保Apache Server进程以最可能低的权限用户来运行。通过修改httpd.conf文件中的下列选项，以Nobody用户运行Apache 达到相对安全的目的。
User nobody
Group# -1
#### 2、ServerRoot目录的权限
为了确保所有的配置是适当的和安全的，需要严格控制Apache 主目录的访问权限，使非超级用户不能修改该目录中的内容。Apache 的主目录对应于Apache Server配置文件httpd.conf的Server Root控制项中，应为：
Server Root /usr/local/apache
#### 3、SSI的配置
在配置文件access.conf 或httpd.conf中的确Options指令处加入Includes NO EXEC选项，用以禁用Apache Server 中的执行功能。避免用户直接执行Apache 服务器中的执行程序，而造成服务器系统的公开化。
Options Includes Noexec
#### 4、阻止用户修改系统设置
在Apache 服务器的配置文件中进行以下的设置，阻止用户建立、修改 .htaccess文件，防止用户超越能定义的系统安全特性。
AllowOveride None
Options None
Allow from all
然后再分别对特定的目录进行适当的配置。
#### 5、改变Apache 服务器的缺省访问特性
Apache 的默认设置只能保障一定程度的安全，如果服务器能够通过正常的映射规则找到文件，那么客户端便会获取该文件，如http://local host/~ root/ 将允许用户访问整个文件系统。在服务器文件中加入如下内容：
order deny,ellow
Deny from all
将禁止对文件系统的缺省访问。
#### 6、CGI脚本的安全考虑
CGI脚本是一系列可以通过Web服务器来运行的程序。为了保证系统的安全性，应确保CGI的作者是可信的。对CGI而言，最好将其限制在一个特定的目 录下，如cgi-bin之下，便于管理；另外应该保证CGI目录下的文件是不可写的，避免一些欺骗性的程序驻留或混迹其中；如果能够给用户提供一个安全性 良好的CGI程序的模块作为参考，也许会减少许多不必要的麻烦和安全隐患；除去CGI目录下的所有非业务应用的脚本，以防异常的信息泄漏。
#### 7、SSL链接加密
以上这些常用的举措可以给Apache Server 一个基本的安全运行环境，显然在具体实施上还要做进一步的细化分解，制定出符合实际应用的安全配置方案。
# 二、PHP安全性设置
服务器并不能阻止所有的安全问题，例如程序漏洞问题、用户输入表单问题、PHP文件权限问题等。 也可以通过一些手段来迷惑黑客或者别有用心者。
#### 1、程序代码漏洞问题
很多 PHP 程序所存在的重大弱点并不是 PHP 语言本身的问题，而是编程者的安全意识不高而导致的。因此，必须时时注意每一段代码可能存在的问题，去发现非正确数据提交时可能造成的影响。

PHP
```
<?php 
    unlink ($evil_var); 
    fwrite ($fp, $evil_var); 
    system ($evil_var); 
    exec ($evil_var); 
?>
```
必须时常留意你的代码，以确保每一个从客户端提交的变量都经过适当的检查，然后问自己以下一些问题：
此脚本是否只能影响所预期的文件？
非正常的数据被提交后能否产生作用？
此脚本能用于计划外的用途吗？
此脚本能否和其它脚本结合起来做坏事？
是否所有的事务都被充分记录了？
在写代码的时候问自己这些问题，否则以后可能要为了增加安全性而重写代码了。注意了这些问题的话，也许还不完全能保证系统的安全，但是至少可以提高安全性。
还可以考虑关闭 register_globals，magic_quotes 或者其它使编程更方便但会使某个变量的合法性，来源和其值被搞乱的设置。
#### 2、用户输入表单问题
验证用户输入的任何数据，保证PHP代码的安全。
注意1：JS只是为了提高来访用户的体验而产生的，而不是验证的工具。因为任何一个来访的用户都可能会，也有可能无意间就禁用了客户端脚本的执行，从而跳过这层验证。所以我们必须在PHP的服务器端程序上检验这些数据。
注意2：不要使用$_SERVER[‘HTTP_REFERER’]这个超级变量来检查数据的来源地址，一个很小的菜鸟黑客都会利用工具来伪造这个变量的数据，尽可能利用Md5，或者rand等函数来产生一个令牌，验证来源的时候，验证这个令牌是否匹配。
#### 3、PHP文件权限问题
PHP 被设计为以用户级别来访问文件系统，所以完全有可能通过编写一段 PHP 代码来读取系统文件如 /etc/passwd，更改网络连接以及发送大量打印任务等等。因此必须确保 PHP 代码读取和写入的是合适的文件。 请看下面的代码，用户想要删除自己主目录中的一个文件。假设此情形是通过 web 界面来管理文件系统，因此 Apache 用户有权删除用户目录下的文件。

PHP
```
<?php 
    $username = $_POST['user_submitted_name']; 
    $homedir = "/home/$username"; 
    $file_to_delete = "$userfile"; 
    unlink ("$homedir/$userfile"); 
    echo "$file_to_delete has been deleted!"; 
?>
```
既然 username 变量可以通过用户表单来提交，那就可以提交别人的用户名和文件名，并删除该文件。这种情况下，就要考虑其它方式的认证：
-只给 PHP 的 web 用户很有限的权限。 -检查所有提交上来的变量。 -以下是更加安全的文件名和变量的验证和检查：

PHP
```
<?php 
    $username = $_SERVER['REMOTE_USER']; 
    $homedir = "/home/$username"; 
 
    if (!ereg('^[^./][^/]*$', $userfile)) 
        die('bad filename'); 
 
    if (!ereg('^[^./][^/]*$', $username)) 
        die('bad username'); 
?>
```
#### 4、隐藏PHP扩展名
一般而言，通过隐藏的手段提高安全性被认为是作用不大的做法。但某些情况下，尽可能的多增加一份安全性都是值得的。
一些简单的方法可以帮助隐藏 PHP，这样做可以提高攻击者发现系统弱点的难度。在 php.ini 文件里设置 expose_php = off ，可以减少他们能获得的有用信息。
另一个策略就是让 web 服务器用 PHP 解析不同扩展名。无论是通过 .htaccess 文件还是 Apache 的配置文件，都可以设置能误导攻击者的文件扩展名：
# 使PHP看上去像其它的编程语言
AddType application/x-httpd-php .asp .py .pl
# 使 PHP 看上去像未知的文件类型
AddType application/x-httpd-php .bop .foo .133t
# 使 PHP 代码看上去像HTML页面
AddType application/x-httpd-php .htm .html
要让此方法生效，必须把 PHP 文件的扩展名改为以上的扩展名。这样就通过隐藏来提高了安全性，虽然防御能力很低而且有些缺点。
# 三、Mysql数据库安全性设置
PHP 本身并不能保护数据库的安全。下面的章节只是讲述怎样用 PHP 脚本对数据库进行基本的访问和操作。记住一条简单的原则：深入防御。保护数据库的措施越多，攻击者就越难获得和使用数据库内的信息。正确地设计和应用数据库可以减少被攻击的担忧。
#### 1、数据库设计问题
应用程序永远不要使用数据库所有者或超级用户帐号来连接数据库，因为这些帐号可以执行任意的操作，比如说修改数据库结构（例如删除一个表）或者清空整个数据库的内容。以下截图的用户设置是危险的。
![20120628154434577](http://jbcdn2.b0.upaiyun.com/2016/04/b464840217e62688e048e483a06ff378.jpg)
![20120628154434222](http://jbcdn2.b0.upaiyun.com/2016/04/4a3f82a98ed6abe59f19085241cc7d74.jpg)
应该为程序的每个方面创建不同的数据库帐号，并赋予对数据库对象的极有限的权限。仅分配给能完成其功能所需的权限，避免同一个用户可以完成另一个用户的事情。这样即使攻击者利用程序漏洞取得了数据库的访问权限，也最多只能做到和该程序一样的影响范围。
##### 2.数据库连接问题
把连接建立在 SSL 加密技术上可以增加客户端和服务器端通信的安全性，或者 SSH 也可以用于加密客户端和数据库之间的连接。如果使用了这些技术的话，攻击者要监视服务器的通信或者得到数据库的信息是很困难的。
#### 3.数据库数据的加密
SSL/SSH 能保护客户端和服务器端交换的数据，但 SSL/SSH 并不能保护数据库中已有的数据。SSL 只是一个加密网络数据流的协议。
如果攻击者取得了直接访问数据库的许可（绕过 web 服务器），敏感数据就可能暴露或者被滥用，除非数据库自己保护了这些信息。对数据库内的数据加密是减少这类风险的有效途径，但是只有很少的数据库提供这些加密功能。
对于这个问题，有一个简单的解决办法，就是创建自己的加密机制，然后把它用在 PHP 程序内，最常见的例子就是把密码经过 MD5 加密后的散列存进数据库来代替原来的明文密码。

PHP
```
<?php 
$query = sprintf("INSERT INTO users(name,pwd) VALUES('%s','%s');", 
addslashes($username), md5($password)); 
$result = pg_query($connection, $query); 
$query = sprintf("SELECT 1 FROM users WHERE name='%s' AND pwd='%s';", 
addslashes($username), md5($password)); 
$result = pg_query($connection, $query); 
if (pg_num_rows($result) > 0) { 
echo 'Welcome, $username!'; 
} else { 
echo 'Authentication failed for $username.'; 
} 
?>
```
#### 4、SQL注入问题
直接 SQL 命令注入就是攻击者常用的一种创建或修改已有 SQL 语句的技术，从而达到取得隐藏数据，或覆盖关键的值，甚至执行数据库主机操作系统命令的目的。这是通过应用程序取得用户输入并与静态参数组合成 SQL 查询来实现的。下面将会给出一些真实的例子。

PHP
```
<?php 
$query = "SELECT id, name, inserted, size FROM products 
WHERE size = '$size' 
ORDER BY $order LIMIT $limit, $offset;"; 
$result = odbc_exec($conn, $query); 
?>
```
可以在原来的查询的基础上添加另一个 SELECT 查询来获得密码： union select ‘1’, concat(uname||’-‘||passwd) as name, ‘1971-01-01’, ‘0’ from usertable; 假如上述语句（使用 ‘ 和 –）被加入到 $query 中的任意一个变量的话，那么就麻烦了。
这些攻击总是建立在发掘安全意识不强的代码上的。所以，永远不要信任外界输入的数据，特别是来自于客户端的，包括选择框、表单隐藏域和 cookie。就如上面的第一个例子那样，就算是正常的查询也有可能造成灾难。
永远不要使用超级用户或所有者帐号去连接数据库。要用权限被严格限制的帐号。 检查输入的数据是否具有所期望的数据格式。PHP 有很多可以用于检查输入的函数，从简单的变量函数和字符类型函数（比如 is_numeric()，ctype_digit()）到复杂的 Perl 兼容正则表达式函数都可以完成这个工作。
如果程序等待输入一个数字，可以考虑使用 is_numeric() 来检查，或者直接使用 settype() 来转换它的类型，也可以用 sprintf() 把它格式化为数字。
一个更安全的防止SQL注入的分页显示方法：

PHP
```
<?php 
settype($offset, 'integer'); 
$query = "SELECT id, name FROM products ORDER BY name LIMIT 20 OFFSET $offset;"; 
$query = sprintf("SELECT id, name FROM products ORDER BY name LIMIT 20 OFFSET %d;", 
$offset); 
?>
```

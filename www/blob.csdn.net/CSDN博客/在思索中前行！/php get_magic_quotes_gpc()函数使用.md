# php get_magic_quotes_gpc()函数使用 - 在思索中前行！ - CSDN博客





2014年06月19日 11:52:38[_Tham](https://me.csdn.net/txl16211)阅读数：680标签：[mysql																[数据解析](https://so.csdn.net/so/search/s.do?q=数据解析&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[Database mysql](https://blog.csdn.net/txl16211/article/category/2333871)











magic_quotes_gpc函数在php中的作用是判断解析用户提示的数据，如包括有:post、get、cookie过来的数据增加转义字符“\”，以确保这些数据不会引起程序，特别是数据库语句因为特殊字符引起的污染而出现致命的错误


在magic_quotes_gpc=On的情况下，如果输入的数据有

单引号（’）、双引号（”）、反斜线（\）与 NULL（NULL 字符）等字符都会被加上反斜线。这些转义是必须的，如果这个选项为off，那么我们就必须调用addslashes这个函数来为字符串增加转义。

正是因为这个选项必须为On，但是又让用户进行配置的矛盾，在PHP6中删除了这个选项，一切的编程都需要在magic_quotes_gpc=Off下进行了。在这样的环境下如果不对用户的数据进行转义，后果不仅仅是程序错误而已了。同样的会引起数据库被注入攻击的危险。所以从现在开始大家都不要再依赖这个设置为On了，以免有一天你的服务器需要更新到PHP6而导致你的程序不能正常工作。
|代码如下|复制代码|
|----|----|
|当magic_quotes_gpc=On的时候，函数get_magic_quotes_gpc()就会返回1当magic_quotes_gpc=Off的时候，函数get_magic_quotes_gpc()就会返回0| |

因此可以看出这个get_magic_quotes_gpc()函数的作用就是得到环境变量magic_quotes_gpc的值。既然在PHP6中删除了magic_quotes_gpc这个选项，那么在PHP6中这个函数我想也已经不复存在了。



php 判断是否开启get_magic_quotes_gpc功能了，以方便我们是否决定使用addslashes这个函数了。
|代码如下|复制代码|
|----|----|
|function SQLString($c, $t){ $c=(!get_magic_quotes_gpc())?addslashes($c):$c; switch($t){  case 'text':   $c=($c!='')?"'".$c."'":'NULL';   break;  case 'search':   $c="'%%".$c."%%'";   break;  case 'int':   $c=($c!='')?intval($c):'0';   break; } return $c;}| |

预防数据库攻击的正确做法
|代码如下|复制代码|
|----|----|
|function check_input($value){// 去除斜杠if (get_magic_quotes_gpc()){$value = stripslashes($value);}// 如果不是数字则加引号if (!is_numeric($value)){$value = “‘” . mysql_real_escape_string($value) . “‘”;}return $value;}$con = mysql_connect(“localhost”, “hello”, “321″);if (!$con){die(‘Could not connect: ‘ . mysql_error());}// 进行安全的 SQL$user = check_input($_POST['user']);$pwd = check_input($_POST['pwd']);$sql = “SELECT * FROM users WHEREuser=$user AND password=$pwd”;mysql_query($sql);mysql_close($con);?>| |






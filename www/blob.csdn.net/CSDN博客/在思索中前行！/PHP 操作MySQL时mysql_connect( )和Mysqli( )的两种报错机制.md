# PHP 操作MySQL时mysql_connect( )和Mysqli( )的两种报错机制 - 在思索中前行！ - CSDN博客





2014年10月20日 19:46:19[_Tham](https://me.csdn.net/txl16211)阅读数：2677








    刚开始使用PHP连接MySQL数据库的时候，如果数据库连接不成功或者，对MySQL数据库进行增删改查等操作的时候，SQL语句存在错误，而在执行PHP文件的时候，浏览器并不会抛出错误的原因，一般是空白显示。此时打开数据库查看，数据库并无变化，可知这些PHP语句根本没有执行任何操作。

    Mysql_connect("主机地址","用户名","密码","数据库"),采用该方法连接数据库的时候基本操作见下



```php
$connection =  mysql_connect('localhost', 'root', 'txl');
if($connection)
{
	echo "connection Success !"."<br/>";
}
else
die(mysql_error()); //使用MySQL_error()报错
$con=mysql_select_db('test',$connection);
$sql="SELECT id FROM test LIMIT 100";

//$link = mysql_connect("localhost", "mysql_user", "mysql_password");
//mysql_select_db("database", $link);

$result = mysql_query("SELECT id FROM test ", $connection);
$num_rows = mysql_num_rows($result);

// echo "$num_rows Rows\n";
if ($result) {

    printf("Select returned %d rows.\n", $num_rows );

    /* free result set */

 //  $result->close();
}else
    die(mysql_error());
```
   new Mysqli()方法是PHP连接MySQL的拓展，采用了面向对象的思想，它不再支持mysql_error()这一种方法，提示MySQL操作错误



```php
$connection = new mysqli('localhost', 'root', 'txl', 'test');
    try{
        $result = $connection->query('select * from st');

        if(!$result){
            echo $connection->error.":".$connection->erron;
            throw new Exception( $connection->erron.":".$connection->error);

        }
    }catch( Exception $e){
      echo $e->getMessage();
  }
```
采用新的try - catch(Exception $e) 新的机制捕获错误，并echo相应的错误信息。
            



# PHP 设计模式 笔记与总结（4）PHP 链式操作的实现 - weixin_33985507的博客 - CSDN博客
2015年04月09日 22:11:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
PHP 链式操作的实现
$db->where()->limit()->order();
在 Common 下创建 Database.php。
链式操作最核心的地方在于：在方法的最后 return $this;
Database.php:
```
<?php
namespace Common;
class Database{
    function where($where){
        return $this;   //链式方法最核心的地方在于：在每一个方法之后 return $this
    }
    function order($order){
        return $this;
    }
    function limit($limit){
        return $this;
    }
}
```
index.php:
```
<?php
define('BASEDIR',__DIR__); //定义根目录常量
include BASEDIR.'/Common/Loader.php';
spl_autoload_register('\\Common\\Loader::autoload');
$db = new \Common\Database();  
//传统的操作需要多行代码实现
//$db->where('id = 1');
//$db->where('name = 2');
//$db->order('id desc');
//$db->limit(10);
//使用链式操作，一行代码解决问题
$db->where('id = 1')->where('name = 2')->order('id desc')->limit(10);
```
在使用链式操作时，ide（netbeans 会给出自动提示）：
![](https://images0.cnblogs.com/blog2015/534303/201504/092210081182148.png)

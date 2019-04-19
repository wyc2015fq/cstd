# mvc学习总结 - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2012年07月17日 17:39:06[baofenny](https://me.csdn.net/jiaminbao)阅读数：307标签：[mvc																[action																[include																[file																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=action&t=blog)](https://so.csdn.net/so/search/s.do?q=mvc&t=blog)
个人分类：[学习实例开发](https://blog.csdn.net/jiaminbao/article/category/1158383)
用mvc做个人站点personal2-------------------
用mvc用网站开发，首先了解mvc的概念
m----模板
v---视图
c----控制器
个人站点制作首先建立一个单入口文件index.php;大致代码如下：
```php
<?php
include('conn.php');
$c = isset($_GET['action']) ? $_GET['action'] : 'index';
$do = isset($_GET['do']) ? $_GET['do'] : 'index';
if(!file_exists('c/'.$c.'.php')) {
	echo '404 not find';
 	exit();
}
include ('c/'.$c.'.php');
$c ='c_'.$c;
$app = new $c;
$app->$do();
?>
```

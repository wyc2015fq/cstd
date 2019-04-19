# mvc建站心得 - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2012年07月17日 18:09:10[baofenny](https://me.csdn.net/jiaminbao)阅读数：677
个人分类：[学习心得](https://blog.csdn.net/jiaminbao/article/category/1158385)
后台管理里面删除功能的设计：
1.在视图c控制器下面不要出现任何输出代码，包括SQL查询语句也不能出现。
```php
$sql='DELETE FROM article WHERE id =\''.$id.'\'';
		if(mysql_query($sql)) {
			$msg = $id."内容删除成功";	
		} else {
			$msg = $id."删除失败";
		}
    }
```
例如如此。。。。。
2.在接收来自URL的数据用$_GET[ '' ]接收时，最好用
```php
intval($_GET['id']);
```
来处理下，以保证接收数据的安全性;
3.
```php
$id = isset($_GET['id']) ? intval($_GET['id']) : 0;
```
这个语句通常用来处理判断接收的信息;于网站安全性考虑，在运算符“：”后面0是比较合理的选择;
4.
```php
$sql='DELETE FROM article WHERE id =\''.$id.'\'';//$_GET['id']
```
这个语句注意两点：a. $sql一般用小写，最好不要用大写；
                                    b. $_GET['id']与\''.$id.'\''比较写后面的是比较好的选择，这在网站安全性方面是理智的；
5.
```php
<?php
if($yes) {
	echo $msg.'<br/>';
	echo '<a href="?admin=article">return</a>';
}else{
?>
    <h1><font color="#FF0000">你确定要删除吗？</font></h1>
    <br/><br/>
	<div class="padd">
    </div>
    <a href="?admin=article_del&id=<?php echo $id;?>&yes=1">yes</a>
    <a href="?admin=article">no</a>
<?php
}
?>
```
此语句用if....else....来判断视图页面的显示效果，牢记此种方法；
6.在控制器使用方面，可以用少量的文件来实现同样的功能，就选择少量的控制器，比如：
此personal2的制作中c下面的控制器可以有add(添加)，del(删除)，edit(编辑)；可以同时写在一个类里面保存在一个文件中。然后实例化类调用不同的类中方法；
例如：
```php
class ad_article{
function  add(){
}
function  edit() {
}
function  add(){
}
```
然后用单入口文件index.php调用
```php
<?php
include('conn.php');
$c = isset($_GET['admin']) ? $_GET['admin'] : 'admin';
$do = isset($_GET['do']) ? $_GET['do'] : 'index';
if(!file_exists('c/admin/'.$c.'.php')) {
	echo '404 not find';
 	exit();
}
include ('c/admin/'.$c.'.php');
$c ='ad_'.$c;
$app = new $c;
$app->$do();
?>
```
里面分类用$do代表调用的不同方法；
[](http://127.0.1.3/jmb/003/personal2/admin.php?admin=article&do=add)
```php
http://127.0.1.3/jmb/003/personal2/admin.php?admin=article
http://127.0.1.3/jmb/003/personal2/admin.php?admin=article_del&id=2
http://127.0.1.3/jmb/003/personal2/admin.php?admin=article_edit&id=2
http://127.0.1.3/jmb/003/personal2/admin.php?admin=article_add
http://127.0.1.3/jmb/003/personal2/admin.php?admin=article&do=add
```
另外查看Url也是一种解决问题的方法；牢记～～～
7.程序优化：
           一切要用到的变量都要用$id = isset($_GET['id'])?intval($_GET['id']):0;方式书写；
          网站的程序哪些地方是重复的，可以合并为一个控制器。
8.如果有错误提示是没用定义的变量，就用一些if($_post)来判断是否接收到了；
if($_POST) {
  $classid = $_POST['classid'];
  $title = $_POST['title'];
  $content = $_POST['content'];//debug
  $sql = "INSERT INTO `dev_personal`.`article` (`id`, `classid`, `title`, `content`, `lasttime`, `count`) VALUES (NULL, '".$classid."', '".$title."', '".$content."', now(), 0);";
 }
————————————————————————————————————————————
$id = isset($_GET['id']) ? intval($_GET['id']) : 0;
 $yes = isset($_GET['yes']) ? intval($_GET['yes']) : 0;
 if($yes){//------------------------切记----------

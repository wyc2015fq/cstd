# PHP 从基础开始 ——重要知识点笔记 - 在思索中前行！ - CSDN博客





2014年12月10日 22:19:30[_Tham](https://me.csdn.net/txl16211)阅读数：1177








## PHP static 关键词

通常，当函数完成/执行后，会删除所有变量。不过，有时我需要不删除某个局部变量。实现这一点需要更进一步的工作。

要完成这一点，请在您首次声明变量时使用 static 关键词：

实例


<?php
function myTest() {
  static $x=0;
  echo $x;
  $x++;
}
myTest();
myTest();
myTest();
?>


然而，每当函数被调用时，这个变量所存储的信息都是函数最后一次被调用时所包含的信息。

注释：该static变量仍然是函数的局部变量(局部函数外部无法使用改变量)。PHP中常量的声明使用define(varname
 , value)，被定义为常量后，全局都可以使用。

PHP var_dump() 会返回变量的数据类型和值：




## $GLOBALS — 引用全局作用域中可用的全部变量



$GLOBALS 这种全局变量用于在 PHP 脚本中的任意位置访问全局变量（从函数或方法中均可）。

PHP 在名为 $GLOBALS[index] 的数组中存储了所有全局变量。变量的名字就是数组的键。

下面的例子展示了如何使用超级全局变量 $GLOBALS：实例
<?php 
$x = 75; 
$y = 25;
function addition() { 
  $GLOBALS['z'] = $GLOBALS['x'] + $GLOBALS['y']; 
}
addition(); 
echo $z; 
?>
/*$x , $y 如果不使用$GLOBALS['index']，则在addtion()函数中是无法使用php脚本中的$x ，$y的值，由于
 z 是 $GLOBALS 数组中的变量，因此在函数之外也可以访问它。



## PHP foreach 循环



foreach 循环只适用于数组，并用于遍历数组中的每个键/值对。

### 语法
foreach ($array as $value) {
  code to be executed;
}

每进行一次循环迭代，当前数组元素的值就会被赋值给 $value 变量，并且数组指针会逐一地移动，直到到达最后一个数组元素。

下面的例子演示的循环将输出给定数组（$colors）的值：

### 实例
<?php 
$colors = array("red","green","blue","yellow"); 
foreach ($colors as $value) {
  echo "$value <br>";
}
?>
## 在 PHP 中创建数组（索引，关联，多维）

在 PHP 中， array() 函数用于创建数组：
array();
在 PHP 中，有三种数组类型：
- 索引数组 - 带有数字索引的数组
- 关联数组 - 带有指定键的数组
- 多维数组 - 包含一个或多个数组的数组




## PHP 索引数组

有两种创建索引数组的方法：

索引是自动分配的（索引从 0 开始）：
$cars=array("Volvo","BMW","SAAB");
或者也可以手动分配索引：
$cars[0]="Volvo";
$cars[1]="BMW";
$cars[2]="SAAB";
## 遍历索引数组

如需遍历并输出索引数组的所有值，您可以使用 for 循环，就像这样：实例
<?php
$cars=array("Volvo","BMW","SAAB");
$arrlength=count($cars);
for($x=0;$x<$arrlength;$x++) {
  echo $cars[$x];
  echo "<br>";
}
?>



## PHP 关联数组



关联数组是使用您分配给数组的指定键的数组。

有两种创建关联数组的方法：
$age=array("Peter"=>"35","Ben"=>"37","Joe"=>"43");

或者：
$age['Peter']="35";
$age['Ben']="37";
$age['Joe']="43";
## 遍历关联数组

如需遍历并输出关联数组的所有值，您可以使用 foreach 循环，就像这样：

### 实例
<?php
$age=array("Bill"=>"35","Steve"=>"37","Peter"=>"43");
foreach($age as $x=>$x_value) {
  echo "Key=" . $x . ", Value=" . $x_value;
  echo "<br>";
}
?>




## PHP - 多维数组

多维数组指的是包含一个或多个数组的数组。

PHP 能理解两、三、四或五级甚至更多级的多维数组。不过，超过三级深的数组对于大多数人难于管理。

注释：数组的维度指示您需要选择元素的索引数。
- 对于二维数组，您需要两个索引来选取元素
- 对于三维数组，您需要三个索引来选取元素



## PHP - 两维数组

两维数组是数组的数组（三维数组是数组的数组的数组）。




我们能够在两维数组中存储上表中的数据，就像这样：
$cars = array
  (
  array("Volvo",22,18),
  array("BMW",15,13),
  array("Saab",5,2),
  array("Land Rover",17,15)
  );

现在这个两维数组包含了四个数组，并且它有两个索引（下标）：行和列。

如需访问 $cars 数组中的元素，我们必须使用两个索引（行和列）：

### 实例
<?php
echo $cars[0][0].": 库存：".$cars[0][1].", 销量：".$cars[0][2].".<br>";
echo $cars[1][0].": 库存：".$cars[1][1].", 销量：".$cars[1][2].".<br>";
echo $cars[2][0].": 库存：".$cars[2][1].", 销量：".$cars[2][2].".<br>";
echo $cars[3][0].": 库存：".$cars[3][1].", 销量：".$cars[3][2].".<br>";
?>
我们也可以在 For 循环中使用另一个 For 循环，来获得 $cars 数组中的元素（我们仍需使用两个索引）：

### 实例

<?php
for ($row = 0; $row < 4; $row++) {
  echo "<p><b>Row number $row</b></p>";
  echo "<ul>";
  for ($col = 0; $col < 3; $col++) {
    echo "<li>".$cars[$row][$col]."</li>";
  }
  echo "</ul>";
}
?>


在C中遍历二维数组我们一般用二重for循环，相应地，PHP中的foreach也可以嵌套使用，分析一下下面的程序你就明白了： 
PHP代码:

```php
<?php
$arr=array( 
             "row1"=>array(100,200,300,400),
             "row2"=>array("num"=>100,"name"=>"Liuxy","score"=>98)
           );

foreach ($arr as $key=>$value)
{
    echo "$key=>$value";
    echo "<br>";
    foreach ($value as $k=>$var) echo "$k=>$var<br>";
}
?>
```
PHP的数组元素是可以动态增长的，这是C里面不敢想的。为数组增加一个元素很简单，同访问数组元素一样，只不过键名是新的或者为空，如 $arr[new]=3;
 或 $arr[]=3，这样做的结果是PHP为数组$arr增加一个键名为new的元素其值为3，若键名为空，则以当前整型键名的最大值加1作为默认的键名分配给新的元素。PHP的这种动态增长数组有时候很方便，但也带来隐患，比如当我们企图修改已存在的元素值而写错了键名时，就变成了新增一个元素，而这样的逻辑错误，系统是不会报错的。
可以增长就可以缩减，要想删除一个数组元素，可以使用unset函数，如删除上面新增的那个元素，可以写成 unset($arr[new])。也可以使用unset函数删除整个数组结构，如unset($arr)，要注意这同逐个删除全部数组元素不同，后者还保留有数组的结构。形象地说，后者是人去楼空，但楼还在，前者连楼也拆除了。有意思的是删除数组全部元素后如果新增一个元素，它的默认键名将延续之前数组的最大键名递增，要想从0开始，可以使用array_values函数重置，如$arr=array_values($arr)。





## **关于Html文件中插入php代码，及php文件中使用html代码的深入理解**

在HTML嵌入PHP脚本”和“在PHP中嵌入HTML”是两个不同的概念Web服务器设置中，默认是不把HTML拿去解析的.
如果在服务器中修改设置，也可以，但你觉得对每一个HTML进行解析，有必要吗？效率会很低下的。
因为在配置apache/Nginx时，php的扩展名一般配置为php或php3，只有当文件扩展名为php或php3时，才会通过php服务器端的解析，否则只当是html语言，不进行php脚本的解析，也就无法调用php的代码！APACHE在Apache的httpd.conf中加入以下语句：
AddType application/x-httpd-htm .htm
Action application/x-httpd-htm "/php5/php.exe"在后面还可以加上别的 如果这样 
AddType application/x-httpd-php .php .html .img

那以.img为后缀的文件都会被解释器跑下的。这样做的目的可以隐藏脚本类型,但是没有太大必要。


这样就可以让php解释htm中的php代码了。
因为你的HTML内容中夹带PHP请求语句（如<?php .....?>）,需要PHP引擎来语法解析，而PHP服务的默认文件后缀为.php和.php3, 但是.htm和.html后缀的文件是不会提交PHP引擎解析。
php文件中夹杂html代码却是没有问题的，因为php引擎只会解释php标签<?php ... ?>内的php代码语句，而html代码不会进行任何的处理，直接html送给浏览器。

[Nginx如何配置可以让.html后缀的文件当php动态文件执行](http://www.cnblogs.com/lost-1987/articles/2720186.html)


有两种方式修改nginx配置文件可以实现。

方式一：打开你的网站的nginx配置文件，然后找到：“location ~ \.php$ {”，再把其中的\.php修改为：“\.php|\.html”，保存后重启nginx即可。

方式二：同上，打开配置文件找到：“location ~ \.php$ {”，然后把location整段复制，在下面粘帖上，再把\.php修改为\.html，保存后重启nginx即可生效。

上述两种方式的配置示例代码如下：location ~ \.php|\.html$ {

            fastcgi_pass 127.0.0.1:9000; 

            fastcgi_index index.php; 

            fastcgi_param SCRIPT_FILENAME /webs$fastcgi_script_name; 

            include fastcgi_params; }

示例代码二(添加html重复段)： location ~ \.html$ { 

            fastcgi_pass 127.0.0.1:9000; 

    fastcgi_index index.php; 

    fastcgi_param SCRIPT_FILENAME /webs$fastcgi_script_name; 

    include fastcgi_params; }


## **PHP 日期和时间**

Date() 函数返回Unix 时间戳。Unix 时间戳包含 Unix 纪元（1970 年 1 月 1 日 00:00:00 GMT）与指定时间之间的秒数。


PHP Date() 函数把时间戳格式化为更易读的日期和时间。

语法

date(format,timestamp)

参数        描述

format        必需。规定时间戳的格式。

timestamp可选。规定时间戳。默认是当前时间和日期。

注释：时间戳是一种字符序列，它表示具体事件发生的日期和事件。


date() 函数的格式参数是必需的，它们规定如何格式化日期或时间，否则返回的是默认时间戳（秒数）。

下面列出了一些常用于日期的字符：

d - 表示月里的某天（01-31）

m - 表示月（01-12）

Y - 表示年（四位数）

l - 表示周里的某天

h - 带有首位零的 12 小时小时格式

i - 带有首位零的分钟

s - 带有首位零的秒（00 -59）

a - 小写的午前和午后（am 或 pm）

其他字符，比如 "/", "." 或 "-" 也可被插入字符中，以增加其他格式。



### 实例
<?php
echo "今天是 " . date("Y/m/d") . "<br>";
echo "今天是 " . date("Y.m.d") . "<br>";
echo "今天是 " . date("Y-m-d") . "<br>";
echo "今天是 " . date("l-y-m-d h:i:s a");
?>


获得时区

如果从代码返回的不是正确的时间，有可能是因为您的服务器位于其他国家或者被设置为不同时区。因此，如果您需要基于具体位置的准确时间，您可以设置要用的时区。


date_default_timezone_set("Asia/Shanghai"); // PRC 表示中国



## 通过 PHP mktime() 创建日期

date() 函数中可选的时间戳参数规定时间戳。如果您未规定时间戳，将使用当前日期和时间（正如上例中那样）。

mktime() 函数返回日期的 Unix 时间戳。Unix 时间戳包含 Unix 纪元（1970 年 1 月 1 日 00:00:00 GMT）与指定时间之间的秒数。

### 语法
mktime(hour,minute,second,month,day,year)
下面的例子使用 mktime() 函数中的一系列参数来创建日期和时间：

### 实例
<?php
$d=mktime(9, 12, 31, 6, 10, 2015);
echo "创建日期是 " . date("Y-m-d h:i:sa", $d);
?>
## 通过 PHP strtotime() 用字符串来创建日期

PHP strtotime() 函数用于把人类可读的字符串转换为 Unix 时间。
strtotime(time,now)
下面的例子通过 strtotime() 函数创建日期和时间：

### 实例

<?php
$d=strtotime("tomorrow");
echo date("Y-m-d h:i:sa", $d) . "<br>";

$d=strtotime("next Saturday");
echo date("Y-m-d h:i:sa", $d) . "<br>";

$d=strtotime("+3 Months");
echo date("Y-m-d h:i:sa", $d) . "<br>";
?>
下例输出下周六的日期：
<?php
$startdate = strtotime("Saturday");
$enddate = strtotime("+6 weeks",$startdate);

while ($startdate < $enddate) {
  echo date("M d", $startdate),"<br>";
  $startdate = strtotime("+1 week", $startdate);
}
?>
下例输出七月四日之前的天数：
<?php
$d1=strtotime("December 31");
$d2=ceil(($d1-time())/60/60/24);
echo "距离十二月三十一日还有：" . $d2 ." 天。";
?>



## **PHP include 和 require 语句**
通过 include 或 require 语句，可以将 PHP 文件的内容插入另一个 PHP 文件（在服务器执行它之前）。
include （或 require）语句会获取指定文件中存在的所有文本/代码/标记，并复制到使用 include 语句的文件中。

包含文件很有用，如果您需要在网站的多张页面上引用相同的 PHP、HTML 或文本的话。


include 和 require 语句是相同的，除了错误处理方面：
require 会生成致命错误（E_COMPILE_ERROR）并停止脚本

include 只生成警告（E_WARNING），并且脚本会继续

因此，如果您希望继续执行，并向用户输出结果，即使包含文件已丢失，那么请使用 include。否则，在框架、CMS 或者复杂的 PHP 应用程序编程中，请始终使用 require 向执行流引用关键文件。这有助于提高应用程序的安全性和完整性，在某个关键文件意外丢失的情况下。
包含文件省去了大量的工作。这意味着您可以为所有页面创建标准页头、页脚或者菜单文件。然后，在页头需要更新时，您只需更新这个页头包含文件即可。


include_once 语句在脚本执行期间包含并运行指定文件。此行为和 include 语句类似，唯一区别是如果该文件中已经被包含过，则不会再次包含。如同此语句名字暗示的那样，只会包含一次。

include_once 可以用于在脚本执行期间同一个文件有可能被包含超过一次的情况下，想确保它只被包含一次以避免函数重定义，变量重新赋值等问题。

## **PHP打开文件目录的相对路径设置“$_SERVER[ 'DOCUMENT_ROOT' ] ”**

$_SERVER[ 'DOCUMENT_ROOT' ]是web服务器的根文档目录，我们在打开文件时，一般为安全性考虑需要隐藏服务器目录的名称，因此打开的文件不能放在服务器下，也不推荐使用绝对路径（不便维护）


 $DOCUMENT_ROOT = $_SERVER['DOCUMENT_ROOT'];


$fp = fopen("$DOCUMENT_ROOT/../orders/orders.txt", 'ab');


考虑到可以利用 $_SERVER['DOCUMENT_ROOT']，因此设置一个orders的目录与web服务器的根目录位于同一文件夹，然后以这样的方式操作。其中“ ../ ”表示web服务器根目录的父目录。










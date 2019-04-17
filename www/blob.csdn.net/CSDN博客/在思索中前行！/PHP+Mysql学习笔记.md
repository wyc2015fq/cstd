# PHP+Mysql学习笔记 - 在思索中前行！ - CSDN博客





2014年06月24日 21:43:39[_Tham](https://me.csdn.net/txl16211)阅读数：587








1.      PHP is Hypertext Preproocessor。

2.      Hello World：
|<?phpEcho ‘Hello World!’;?>|
|----|
3.      标记风格：

a)       <?php ?>

b)       <script language=”php”></script>

c)       <?    ?>  短标签
4.      注释：

a)       //

b)       /**/

c)       #

5.      数据类型：

a)       4种标量类型

                   i.           boolean：取值true和false；

                  ii.           integer

                iii.            float，也成为double

                iv.            string：单引号(不显示变量值)、双引号(显示变量值)、界定符（<<<，显示变量值）

b)       2种复合类型

                   i.           array：eg：myarray(1,2);或者myarray(key1=>value1,key2=>value2);或者myarray[k1]=v1;

                  ii.           对象类型object：

class MyClass

{

       public$var1;                              //属性

       functionmyFun($arg1,$arg2){}       //方法

       functionmyFun($arg1,$arg2){} //构造方法

}

使用new创建对象：$obj = new MyClass();

c)       2种特殊类型

                   i.           资源类型：例如，数据库连接、查询，文件句柄等

                  ii.           NULL类型，不区分大小写null=NULL,unset()函数删除变量值后为null

6.      常量和变量：

a)       常量：使用define()函数实现，eg：define(‘NAME’,’姓名’);预定义的常量又称之为魔术常量，例如_METHOD_表示方法的名字。

b)       变量：使用$定义变量并获得变量值。预定义变量也有很多，例如_COOKIE_表示用户会话。打印变量：var_dump($arg);

7.      表达式：

a)       算数运算符：+-*/%

b)       赋值运算符：=

c)       逻辑运算符：&&(and)、||(or)、xor、!

d)       比较运算符：===表示全等，即两边的值相等且类型相等

e)       位运算符：&|~^<< >>

f)       递增递减：++--

g)       三元运算符：?:

h)       字符串运算符：$a=’你’;$b=’好’; $e=$a.$b;通过.连接字符串，输出“你好”。此外，使用{}和双引号同样可以得到这样的效果。$e=”{$a}{$b}”;点运算符首先将不是字符型的变量转换成字符型，再进行运算。

i)       数组运算符：+、==、===、!=或<>、!==

j)       类型运算符：instanceof

k)       错误控制运算符：@控制错误提示不出现

8.      PHP流程控制：

a)       If…else

b)       Switch

c)       While、for、foreach(用于遍历数组和对象)

9.      数组：

a)       索引数组：使用数字作为key

b)       关联数组：使用字符串作为key。

$myarray = array(‘name’=>’姓名’);

10.   索引数组遍历：

       a)    for:|$myarray = array(‘Mo’,19);$size = count($myarray);For($i = 0; $i < $size; ++$i){    Echo $i.’=>’.$myarray[$i].’<br>’;}|
|----|






b)    while:|$myarray = array(‘Mo’,19);$size = count($myarray);while($i<$size){    Echo $i.’=>’.$myarray[$i].’<br>’;}|
|----|








c)    foreach:|$myarray = array(‘Mo’,19);Foreach($myarray as $key =>$value){    Echo $key.’=>’.$value.’<br>’;}输出：0=>Mo      1=>19|
|----|









d)    list():仅用于索引数组|$myarray = array(‘Mo’,19);List($nickname,$age) = $myarray;Echo $nickname.’ ’.$age.’ ’;|
|----|





e)    each():返回key-map键值对（4个单元的数组，键名分别为0、1、1key、value，其中，0和key包含键名，1和value包含数组元素的值），并将指针向前移动一步。|$myarray = array(‘Mo’,19);$array = each($myarray);Echo $nickname.’ ’.$age.’ ’;|
|----|





f)     混合使用list和each|$myarray = array(‘Mo’,19);While($list($key,$value)=each($myarray)){    Echo $key.’=>’.$value.’<br>’;}|
|----|







11.   关联数组遍历：

a)    foreach:|$myarray = array(‘name’=>‘Mo’,’age’=>19);Foreach($myarray as $key =>$value){    Echo $key.’=>’.$value.’<br>’;}输出：0=>Mo      1=>19|
|----|









b)    混合使用list和each|$myarray = array(‘Mo’,19);While($list($key,$value)=each($myarray)){    Echo $key.’=>’.$value.’<br>’;}|
|----|







12.   数组的操作：

a)       检查数组中是否含有特定值：array_search

b)       数组合并：array_merge

c)       数组分割：array_chunk

d)       统计数组中值出现的次数：array_count_valuesc

e)       计算数组中所有值的和：array_sum

f)       删除重复的中：array_unique

g)       数组中的元素数目：count

h)       数组正向排序：sort

i)       逆向排序：rsort

j)       元素顺序反转：array_reverse

k)       用给定的值填充数组：array_pad

l)       计算多个数组的交集：array_intersect

13.   预定义数组变量：

a)       $_SERVER:服务器变量

b)       $_EVE:环境变量

c)       $_GET:获取HTTP GET方式传递的变量

d)       $_POST:获取HTTP POST方式传递的变量

e)       $_SESSION:会话变量

f)       $_COOKIE:cookie变量

g)       $_Request:请求变量，包括$_GET,$_POST和$_COOKIE

h)       $_FILES:文件上传上传变量

i)       $_GLOBALS:全局变量的集合

14.   PHP函数：

函数定义：

function 函数名(参数1，参数2)

{

    函数体；

}

参数传递：$传值，&$传引用

函数中引用全局变量：1.使用global修饰变量2.使用$GLOBALS数组引用

静态变量：使用static修饰

15.   PHP与面向对象

a)       类定义：class 类名{public$name; public function(){}}

b)       类加载：require或者include

c)       类的实例化：变量名 = new 类名();

d)       类内部访问该类静态方法：self::静态方法名；类内部访问父类的静态方法：parent::静态方法名。

e)       构造方法：__construct()、析构方法：__destruct()

16.   PHP与MySQL:

a)       连接：mysql_connect()

b)       关闭：mysql_close()

c)       选择数据库：mysql_select_db()

d)       查询：mysql_query()

                   i.           Mysql_fetch_row:将查询返回的结果集作为枚举数组，如果没有，返回false

                  ii.           Mysql_fetch_assoc:将结果集的一行作为关联数组

                iii.           Mysql_fetch_array：结合上述两者，可以通过参数选择

扩展库mysqli（采用面向对象技术）：

a)       连接：$mysqli = newmysqli($host,$username,$password,$dbname);

b)       查询：mysqli->query(sql);

17.   Smarty模板技术：

a)       作用：分离PHP和HTML代码

b)       两个文件：

                   i.           模板文件：含有定界符的HTML文件，执行时变量替换，后缀名为tpl或者html。

                  ii.           PHP文件：实现程序逻辑，通过assign方法给模板文件中的变量赋值

c)       定界符：默认为{}。注释：{*注释*}

d)       变量：

                   i.           PHP分配变量：

1.       $smarty->assign(‘girl’,$girl);             //给模板变量赋值

2.       $smarty->display(‘xxx.html’);    //调用并显示模板

ii．          PHP配置文件中的变量：配置的全局变量

iii．        Smarty保留变量：如get、post、request等



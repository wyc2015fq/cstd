# PHP入门教程 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月02日 15:08:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：70
个人分类：[纯编程](https://blog.csdn.net/leixiaohua1020/article/category/1647545)










**十天学会PHP**



**下面简单介绍一下PHP的语法。**

 1、嵌入方法：

类似ASP的<%，PHP可以是<?php或者是<?，结束符号是?>，当然您也可以自己指定。

 2、引用文件：

引用文件的方法有两种：require及 include。

 require 的使用方法如require("MyRequireFile.php");。这个函数通常放在 PHP程序的最前面，PHP程序在执行前，就会先读入 require所指定引入的文件，使它变成 PHP程序网页的一部份。常用的函数，亦可以这个方法将它引入网页中。

 include 使用方法如include("MyIncludeFile.php");。这个函数一般是放在流程控制的处理部分中。PHP程序网页在读到include的文件时，才将它读进来。这种方式，可以把程序执行时的流程简单化。

 3、注释方法：

```php
<?php 

  echo "这是第一种例子。\n" ; //本例是 C++ 语法的注释

  /* 本例采用多行的

  注释方式 */

  echo "这是第二种例子。\n" ;

  echo "这是第三种例子。\n" ; #本例使用 UNIX Shell 语法注释

  ?>
```



 4、变量类型：

```php
$mystring = "我是字符串" ;

  $NewLine = "换行了\n" ;

  $int1 = 38 ; 

  $float1 = 1.732 ; 

  $float2 = 1.4E+2 ;

  $MyArray1 = array( "子" , "丑" , "寅" , "卯" );
```



这里引出两个问题，首先PHP变量以$开头，第二PHP语句以;结尾，可能ASP程序员会不适应。这两个遗漏也是程序上大多错误所在。



 5、运算符号：

数学运算：符号意义

 + 加法运算

 - 减法运算

 * 乘法运算

 / 除法运算

 % 取余数

 ++ 累加

 -- 递减



字符串运算：

运算符号只有一个，就是英文的句号。它可以将字符串连接起来，变成合并的新字符串。类似ASP中的&

```php
<?
  $a = "PHP 4" ; 
  $b = "功能强大" ;
  echo $a.$b; 
  ?>
```



这里也引出两个问题，首先PHP中输出语句是echo，第二类似ASP中的<%=变量%>，PHP中也可以<?=变量? >。



逻辑运算：

符号意义

 < 小于

 > 大于

 <= 小于或等于

 >= 大于或等于

 == 等于

 != 不等于

 && 而且 (And)

 and 而且 (And)

 || 或者 (Or)

 or 或者 (Or)

 xor 异或 (Xor)

 ! 不 (Not)

今天就说到这里，明天说一下流程控制。



**学习目的：掌握php的流程控制**



** 1、if..else循环有三种结构**

第一种是只有用到 if条件，当作单纯的判断。解释成 "若发生了某事则怎样处理"。语法如下：

 if (expr) { statement }

其中的 expr为判断的条件，通常都是用逻辑运算符号当判断的条件。而 statement为符合条件的执行部分程序，若程序只有一行，可以省略大括号 {}。

范例：本例省略大括号。



```php
<?php 
  if ($state==1)echo "哈哈" ;
  ?>
```



这里特别注意的是，判断是否相等是==而不是=，ASP程序员可能常犯这个错误，=是赋值。

范例：本例的执行部分有三行，不可省略大括号。

```php
<?php 

  if ($state==1) { 
  echo "哈哈 ;
  echo "<br>"; 
  } 
  ?>
```


第两种是除了 if之外，加上了 else的条件，可解释成 "若发生了某事则怎样处理，否则该如何解决"。语法如下

 if (expr) { statement1 }else { statement2 }

范例：上面的例子来修改成更完整的处理。其中的 else 由于只有一行执行的指令，因此不用加上大括号。

```php
<?php 
  if ($state==1) { 
  echo "哈哈" ;
  echo "<br>";
  } 
  else{
  echo "呵呵";
  echo "<br>";
  } 
  ?>
```


 第三种就是递归的 if..else循环，通常用在多种决策判断时。它将数个 if..else拿来合并运用处理。

直接看下面的例子



```php
<?php 
  if ( $a > $b ) { 
  echo "a 比 b大" ; 
  } elseif ( $a == $b ) {
  echo "a 等于 b" ;
  } else { 
  echo "a 比 b小" ; 
  } 
  ?>
```



上例只用二层的 if..else循环，用来比较 a和 b两个变量。实际要使用这种递归 if..else循环时，请小心使用，因为太多层的循环容易使设计的逻辑出问题，或者少打了大括号等，都会造成程序出现莫名其妙的问题。



** 2、 for循环就单纯只有一种，没有变化，它的语法如下**

 for (expr1; expr2; expr3) {statement }

其中的 expr1为条件的初始值。expr2为判断的条件，通常都是用逻辑运算符号 (logicaloperators)当判断的条件。expr3为执行 statement后要执行的部份，用来改变条件，供下次的循环判断，如加一..等等。而 statement为符合条件的执行部分程序，若程序只有一行，可以省略大括号 {}。

下例是用 for循环写的的例子。

```php
<?php 
  for ( $i = 1 ; $i <= 10 ;$i ++) {
  echo "这是第".$i."次循环<br>" ;
  } 
  ?>
```



**3****、 switch循环，通常处理复合式的条件判断，每个子条件，都是 case指令部分。在实作上若使用许多类似的 if 指令，可以将它综合成 switch循环。**

语法如下

 switch (expr) { case expr1:statement1; break; case expr2: statement2; break; default: statementN; break; }

其中的 expr条件，通常为变量名称。而 case后的 exprN，通常表示变量值。冒号后则为符合该条件要执行的部分。注意要用 break跳离循环。



```php
<?php 
  switch ( date ("D" )) {
  case "Mon" : 
  echo "今天星期一" ;
  break; 
  case "Tue" : 
  echo "今天星期二" ;
  break; 
  case "Wed" : 
  echo "今天星期三" ;
  break; 
  case "Thu" : 
  echo "今天星期四" ;
  break; 
  case "Fri" : 
  echo "今天星期五" ;
  break; 
  default: 
  echo "今天放假" ;
  break; 
  } 
?>
```





这里需要注意的是break;别遗漏了，default，省略是可以的。

很明显的，上述的例子用 if循环就很麻烦了。当然在设计时，要将出现机率最大的条件放在最前面，最少出现的条件放在最后面，可以增加程序的执行效率。上例由于每天出现的机率相同，所以不用注意条件的顺序。

今天就说到这里，明天开始说数据库的使用。



**学习目的：学会连接数据库**

PHP简直就是一个函数库，丰富的函数使PHP的某些地方相当简单。建议大家down一本PHP的函数手册，总用的到。

我这里就简单说一下连接MYSQL数据库。

**1、mysql_connect**

打开 MySQL服务器连接。

语法: int mysql_connect(string[hostname] [:port], string [username], string [password]);返回值:整数

本函数建立与 MySQL服务器的连接。其中所有的参数都可省略。当使用本函数却不加任何参数时，参数hostname的默认值为localhost、参数 username的默认值为 PHP 执行行程的拥有者、参数 password则为空字符串 (即没有密码)。而参数 hostname后面可以加冒号与端口号，代表使用哪个端口与 MySQL连接。当然在使用数据库时，早点使用 mysql_close()将连接关掉可以节省资源。

**2、mysql_select_db**

选择一个数据库。

语法: int mysql_select_db(stringdatabase_name, int [link_identifier]);返回值:整数

本函数选择 MySQL服务器中的数据库以供之后的资料查询作业 (query)处理。成功返回 true，失败则返回 false。

最简单的例子就是：

```php
$conn=mysql_connect ("127.0.0.1", "","");
mysql_select_db("shop");
```



连接机MY SQL数据库，打开SHOP数据库。在实际应用中应当加强点错误判断。

今天就说到这里，明天再说一下数据库的读取。

学习目的：学会读取数据



先看两个函数：

**1、mysql_query**

送出一个 query字符串。语法: int mysql_query(string query, int [link_identifier]);返回值:整数

本函数送出 query字符串供 MySQL做相关的处理或者执行。若没有指定 link_identifier参数，则程序会自动寻找最近打开的 ID。当 query查询字符串是 UPDATE、INSERT及 DELETE时，返回的可能是 true或者 false；查询的字符串是 SELECT则返回新的 ID值，当返回 false时，并不是执行成功但无返回值，而是查询的字符串有错误。



**2、mysql_fetch_object返回类资料**。语法: object mysql_fetch_object(int result, int [result_typ]);返回值:类

本函数用来将查询结果 result拆到类变量中。若 result没有资料，则返回 false值。



看一个简单的例子：

```php
<? 
$exec="select * from user";
$result=mysql_query($exec);
while($rs=mysql_fetch_object($result))
{
echo "username:".$rs->username."<br>";
}
?>
```



当然，表user中有一个username的字段，这就类似asp中的

```
<%
exec="select * from user"
set rs=server.createobject("adodb.recordset")
rs.open exec,conn,1,1
do while not rs.eof
response.write"username:"&rs("username")&"<br>"
rs.movenext 
loop 
%>
```



当然先要连接数据库，一般我们require_once('conn.php');而conn.php里面就是上一次说的连接数据库的代码。

小小的两条命令可以完成读取数据的工作了，今天说到这里下一次说数据的添加删除修改。

学习目的：学会添加删除修改数据

mysql_query($exec);

单这个语句就可以执行所有的操作了，不同的就是$exec这个sql语句

添加：$exec="insert into tablename(item1,item2) values ('".$_POST['item1']."',".$_POST['item1'].")";

删除：$exec="delete from tablenamewhere...";

修改：$exec="update tablename setitem1='".$_POST['item1']."' where ...";

说到这里就要说一下表单和php变量传递，如果表单中的一个 <inputname="item1" type="text" id="item1">

表单以POST提交的，那么处理表单文件就可以用$_POST['item1']得到变量值，同样以GET提交的就是$_GET['item1']

是不是很简单？但是通常$exec会有问题，因为可能您的SQL语句会很长，您会遗漏.连接符，或者'来包围字符型字段。

我们可以注释mysql_query($exec);语句用echo $exec;代替来输出$exec以检查正确性。如果您还不能察觉$exec有什么错误的话，可以复制这个sql语句到phpmyadmin中执行，看看它的出错信息。还有需要注意的是，我们不要使用一些敏感的字符串作为字段名字，否则很可能会出现问题，比如说date什么的。变量的命名，字段的命名遵循一点规律有的时候对自己是一种好处，初学者并不可忽视其重要性。

今天就说到这里，大家可以DOWN一个SQL语句的参考手册，再研究研究。明天继续说SESSION



**学习目的：学会SESSION的使用**

SESSION的作用很多，最多用的就是站点内页面间变量传递。在页面开始我们要session_start();开启SESSION；

然后就可以使用SESSION变量了，比如说要赋值就是：$_SESSION['item']="item1";要得到值就是$item1=$_SESSION['item'];，很简单吧。这里我们可能会使用到一些函数，比如说判断是不是某SESSION变量为空，可以这么写：empty($_SESSION['inum'])返回true or false。

下面综合一下前面所说的我们来看一个登陆程序，判断用户名密码是否正确。

登陆表单是这样：login.php

```
<table width="100%" height="100%"border="0" align="center" cellpadding="0"cellspacing="0">
<tr>
<form action="checklogin.php"method="post"><td align="center"valign="middle"><table width="400"border="0" cellpadding="5" cellspacing="1"class="tablebg">
<tr class="tdbg">
<td colspan="2"><divalign="center">Administrators Login</div></td>
</tr>
<tr class="tdbg">
<td><divalign="center">Username</div></td>
<td><div align="center">
<input name="username" type="text"id="username">
</div></td>
</tr>
<tr class="tdbg">
<td><divalign="center">Password</div></td>
<td><div align="center">
<input name="password" type="password"id="password">
</div></td>
</tr>
<tr class="tdbg">
<td colspan="2"><div align="center">
<input type="submit" name="Submit"value="Submit">
<input type="reset" name="Submit2"value="Clear">
</div></td>
</tr>
</table></td></form>
</tr>
</table>
```



处理文件是这样

```php
<?
require_once('conn.php');
session_start();
$username=$_POST['username'];
$password=$_POST['password'];
$exec="select * from admin whereusername='".$username."'";
if($result=mysql_query($exec))
{
 if($rs=mysql_fetch_object($result))
  {
    if($rs->password==$password)
    {
     $_SESSION['adminname']=$username;
     header("location:index.php");
    }
    else
    {
      echo"<script>alert('Password CheckError!');location.href='login.php';</script>";
    }
  }
  else
  {
  echo"<script>alert('Username CheckError!');location.href='login.php';</script>";
  }
  }
else
{
echo "<script>alert('Database ConnectionError!');location.href='login.php';</script>";
}

?>
```



conn.php是这样：

```php
<?
$conn=mysql_connect ("127.0.0.1", "","");
mysql_select_db("shop");
?>
```



由于 $_SESSION['adminname']=$username;我们可以这样写验证是否登陆语句的文件：checkadmin.asp

```php
<?

session_start();

if($_SESSION['adminname']=='')
{
echo "<script>alert('Please LoginFirst');location.href='login.php';</script>";
}
?>
```



呵呵，今天说到这里，明天说一下怎么弄一个分页。



**学习目的：做一个分页显示**

关键就是用到了SQL语句中的limit来限定显示的记录从几到几。我们需要一个记录当前页的变量$page，还需要总共的记录数$num



对于$page如果没有我们就让它=0，如果有<0就让它也=0，如果超过了总的页数就让他=总的页数。

```php
$execc="select count(*) from tablename ";
$resultc=mysql_query($execc);
$rsc=mysql_fetch_array($resultc);
$num=$rsc[0];
```



这样可以得到记录总数

ceil($num/10))如果一页10记录的话，这个就是总的页数



所以可以这么写

```php
if(empty($_GET['page'])){
$page=0;
}
else{
$page=$_GET['page'];
if($page<0)$page=0;
if($page>=ceil($num/10))$page=ceil($num/10)-1;//因为page是从0开始的，所以要-1
}
```

这样$exec可以这么写

$exec="select * fromtablename limit ".($page*10).",10";

//一页是10记录的



最后我们需要做的就是几个连接：

```
<a href="xxx.php?page=0">FirstPage</a>
<a href="xxx.php?page=<?=($page-1)?>">PrevPage</a>
<a href="xxx.php?page=<?=($page+1)?>">NextPage</a>
<a href="xxx.php?page=<?=ceil($num/10)-1?>">LastPage</a>
```


这是一个大致的思路，大家可以想想怎么来优化？今天说到这里，明天说一下一些注意的问题。



**学习目的：注意事项**

因为我是先学ASP的，所以再做PHP的时候会发现很多地方需要适应。



1、注意不要漏了分号

2、注意不要漏了变量前的$

3、使用SESSION的时候注意不要遗漏session_start();



如果发生错误的时候，可以采用以下方法：

1、如果是SQL语句出错，就注释了然后输出SQL语句，注意也要注释调后续的执行SQL语句

2、如果是变量为空，大多是没有传递到位，输出变量检查一下，检查一下表单的id和name

3、如果是数据库连接出错，检查是否正确打开MY SQL和是否遗漏了连接语句

4、注意缩进，排除括号不区配的错误



在做大网站的时候，我的思路是先构建数据库，确定每一个字段的作用，和表之间的关系。然后设计后台界面，从添加数据开始做起，因为添加是否成功可以直接到数据库里面验证，做好了添加再做显示的页面，最后才是两者的结合。一般来说后台就包括添加删除修改和显示，后台没有问题了，前台也没有什么大问题。前台还需要注意安全性和容错还有就是输出格式。



好了，今天说到这里，明天说一下如果用PHP上传文件和发邮件。



**学习目的：学会用PHP上传文件和发邮件**

上传文件表单必须加上enctype="multipart/form-data"

和 <input type="file"name="file">

下面看一下代码：

```php
$f=&$HTTP_POST_FILES['file'];
$dest_dir='uploads';//设定上传目录
$dest=$dest_dir.'/'.date("ymd")."_".$f['name'];//我这里设置文件名为日期加上文件名避免重复
$r=move_uploaded_file($f['tmp_name'],$dest);
chmod($dest, 0755);//设定上传的文件的属性
```



上传的文件名为date("ymd")."_".$f['name']，可以在以后插入到数据库的时候用到，PHP实际上是把你上传的文件从临时目录移动到指定目录。move_uploaded_file($f['tmp_name'],$dest);这是关键



至于发邮件就更加简单，可以使用mail()函数

```php
mail("收件人地址","主题","正文","From:发件人\r\nReply-to:发件人的地址");
```



不过mail()需要服务器的支持，在WINDOWS下还需要配置SMTP服务器，一般来说外面的LINUX空间都行。

好像上传文件和发邮件比ASP简单很多，只要调用函数就可以了。ASP还需要用到服务器的不同组件比如FSO、JMAIL什么的。





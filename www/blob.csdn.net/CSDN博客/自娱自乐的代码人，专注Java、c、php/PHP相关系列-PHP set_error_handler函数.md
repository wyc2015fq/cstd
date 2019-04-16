# PHP相关系列 - PHP set_error_handler函数 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年07月24日 17:33:29[initphp](https://me.csdn.net/initphp)阅读数：804标签：[php																[types																[function																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)
个人分类：[PHP相关系列](https://blog.csdn.net/initphp/article/category/2609095)





**set_error_handler函数：**

函数设置用户自定义的错误处理函数。该函数用于创建运行时期间的用户自己的错误处理方法。该函数会返回旧的错误处理程序，若失败，则返回 null。



**语法：**

```php
set_error_handler(error_function,error_types)
```


**参数：**

error_function ： 必需。规定发生错误时运行的函数。

error_types ： 可选。规定在哪个错误报告级别会显示用户定义的错误。默认是 "E_ALL"。



**例子：**

```php
<?php
//error handler function
function customError($errno, $errstr, $errfile, $errline)
 { 
 echo "<b>Custom error:</b> [$errno] $errstr<br />";
 echo " Error on line $errline in $errfile<br />";
 echo "Ending Script";
 die();
 }

//set error handler
set_error_handler("customError");

$test=2;

//trigger error
if ($test>1)
 {
 trigger_error("A custom error has been triggered");
 }
?>
```



**想法：**一般可以作为统一的程序错误机制处理。



参看：w3school.com.cn](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=types&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)





# PHP相关系列 - PHP异常详解 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年07月24日 14:27:18[initphp](https://me.csdn.net/initphp)阅读数：1019








**PHP的异常是什么？**

PHP 5 提供了一种新的面向对象的错误处理方法。异常处理用于在指定的错误（异常）情况发生时改变脚本的正常流程。这种情况称为异常。



**一般使用方法：**

```php
<?php
function test() {
	throw new Exception("异常啦");
}
try {
     test();
} catch (Exception $e) {
 echo $e->getMessage();
}
```
- throw new Exception('XXX') ： 抛出一个异常
- try  ： 使用异常的函数应该位于 "try" 代码块内。如果没有触发异常，则代码将照常继续执行。但是如果异常被触发，会抛出一个异常。
- 
Catch：代码块会捕获异常，并创建一个包含异常信息的对象


**自定义异常类：**

```php
class myException extends Exception {
 public function errorMessage() {
  	$errorMsg = 'Error on line '.$this->getLine().' in '.$this->getFile()
  .': <b>'.$this->getMessage().'</b> is not a valid E-Mail address';
  	return $errorMsg;
  }
}


try {
  throw new myException($email);
}catch (myException $e){
 echo $e->errorMessage();
}
```
- myException类是一个自定义的异常类，必须继承Exception
- Exception是一个PHP自带的内置的异常类，默认情况下会使用PHP默认的内置异常类，但是我们可以通过自定义的方式，自定义成满足自己需求的异常类。
- 使用自定义异常类的时候，throw new myException 抛出异常也需要抛出自定义的异常类类名
- catch的时候也需要捕获自定义的异常类
- 如果抛出了异常而不捕获处理的话，就会发生PHP代码报错，如下：

```
Fatal error: Uncaught exception 'myException' in D:\AppServ\www\cctv\trunk\index.php:12 Stack trace: #0 {main} 
thrown in D:\AppServ\www\cctv\trunk\index.php on line 12
```


**PHP默认异常类详细：**

```php
class exception
{
    protected $message = 'Unknow exception'; //自定义的异常信息
    protected $code = 0; //定义的异常代码
    protected $file; //发生异常的PHP程序名
    protected $line; //发生异常的PHP行号
    //用于传递用户自定义异常信息和用户自定义异常代码的构造函数
    function __construct($message=null,$code=0);
    final function getMessage();
    final function getCode();
    final function getFile();
    final function getLine();
    final function getTrace(); //以数组形式返回异常传递的路线
    final function getTraceAsString(); //返回格式化成字符串的getTrace函数信息
    function __toString(); //可重载，用于返回可输出的字符串
}
```
- 可以看到PHP默认异常类中有很多自定义变量和方法
- $e->getMessage 一般是获取错误信息
- $e->getCode() 一般是获取错误码
- $e->getFile() 获取错误的文件信息
- $e->getLine() 获取错误行数
- $e->getTrace() 异常经过的文件路径
- __toString() 一般可以重载，用于返回可输出的字符串

**PHP异常类的作用：**
- 捕获异常，一般可以在MYSQL链接不上，或者业务逻辑错误的时候，抛出异常，在代码最顶端接收。
- 异常分析处理。可以在出异常的时候，捕获到异常的错误信息，写入日志。
- 可以返回友好化的提示信息，例如WEB端可以返回页面形式，也可以在AJAX端返回JSON数据格式。
- 原则：如果异常抛出了，就必须捕获它，否则会PHP报错。





****




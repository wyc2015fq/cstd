# PHP命令行下的世界 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [管宜尧](http://www.jobbole.com/members/mylxsw) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
![DSC05906-Quick Preset_2758x1838](http://jbcdn2.b0.upaiyun.com/2016/12/5316cb1517edcae40acbeaaf17578494.jpg)￼
- [php命令行(CLI)参数详解](#toc_0)- [以交互式shell模式运行php](#toc_1)
- [运行内建的Web服务器](#toc_2)
- [查找PHP的配置文件](#toc_3)
- [查看类/函数/扩展信息](#toc_4)
- [语法检查](#toc_5)
- [命令行脚本开发](#toc_6)
PHP作为一门web开发语言，通常情况下我们都是在Web Server中运行PHP，使用浏览器访问，因此很少关注其命令行操作以及相关参数的使用，但是，特别是在类Unix操作系统上，PHP可以作为一门脚本语言执行与shell类似的处理任务。
## php命令行(CLI)参数详解
查看PHP的所有命令行参数，使用php -h命令。我们将会对大部分常用的命令行参数进行一一解释，以加深对PHP能力的认识，更加快捷的在服务端命令行下使用PHP或者调试各种因为对环境不熟悉而出现的问题。


```
-a               以交互式shell模式运行
-c | 指定php.ini文件所在的目录
-n               指定不使用php.ini文件
-d foo[=bar]     定义一个INI实体，key为foo，value为'bar'
-e               为调试和分析生成扩展信息
-f         解释和执行文件.
-h               打印帮助
-i               显示PHP的基本信息
-l               进行语法检查 (lint)
-m               显示编译到内核的模块
-r         运行PHP代码，不需要使用标签 ..?>
-B   在处理输入之前先执行PHP代码
-R         对输入的没一行作为PHP代码运行
-F         Parse and execute  for every input line
-E     Run PHP  after processing all input lines
-H               Hide any passed arguments from external tools.
-S : 运行内建的web服务器.
-t      指定用于内建web服务器的文档根目录
-s               输出HTML语法高亮的源码
-v               输出PHP的版本号
-w               输出去掉注释和空格的源码
-z         载入Zend扩展文件 .
args...          传递给要运行的脚本的参数. 当第一个参数以-开始或者是脚本是从标准输入读取的时候，使用--参数
--ini            显示PHP的配置文件名
--rf       显示关于函数  的信息.
--rc       显示关于类  的信息.
--re       显示关于扩展  的信息.
--rz       显示关于Zend扩展  的信息.
--ri       显示扩展  的配置信息.
```
上面列出了PHP命令所有的参数及其注释，接下来，我们将对其中比较常用的参数举例说明。
### 以交互式shell模式运行php
用过 [Python](https://www.python.org/) 的朋友对Python的交互式shell比较熟悉，在命令行下，如果我们直接输入python命令，则会进入python的交互式shell程序，接下来就可以交互式的执行一些计算任务。
在PHP命令行中，同样提供了类似的功能，使用-a参数即可进入交互shell模式。
在该shell中，我们可以执行一些简单的任务，而不需要总是新建一个php文件。
更详细的使用说明，请参考[官方文档](http://php.net/manual/en/features.commandline.interactive.php)
### 运行内建的Web服务器
从PHP 5.4.0开始，PHP的命令行模式提供了一个内建的web服务器。使用-S开始运行web服务。
假设当前我们处在目录/Users/mylxsw/codes/php/aicode/demo，在该目录中，存在index.php文件。


```
$ ls
index.php
$ cat index.php
<?php echo "Hello, PHPER!";
```
在该目录中，执行以下命令可以启动内建web服务器，并且默认以当前目录为工作目录


```
$ php -S localhost:8000
PHP 5.6.3 Development Server started at Wed Jun 10 15:49:41 2015
Listening on http://localhost:8000
Document root is /Users/mylxsw/codes/php/aicode/demo
Press Ctrl-C to quit.
```
我们另外开启一个shell窗口，请求http://localhost:8000/即可看到脚本输出


```
$ curl -is http://localhost:8000/
HTTP/1.1 200 OK
Host: localhost:8000
Connection: close
X-Powered-By: PHP/5.6.3
Content-type: text/html; 
Hello, PHPER!
```
在web服务运行的窗口，可以看到输出的日志信息
以上我们在启动内建服务器的时候，只指定了-S参数让PHP以web服务器的方式运行，这时，PHP会使用当前目录作为工作目录，因此回到当前目录下寻找请求的文件，我们还可以使用-t参数指定其它的目录作为工作目录（文档根目录）。
更多详细信息，请参考[官方文档](http://php.net/manual/en/features.commandline.webserver.php)。
### 查找PHP的配置文件
在有的时候，由于服务器上软件安装比较混乱，我们可能安装了多个版本的PHP环境，这时候，如何定位我们的PHP程序使用的是那个配置文件就比较重要了。在PHP命令行参数中，提供了–ini参数，使用该参数，可以列出当前PHP的配置文件信息。


```
$ php --ini
Configuration File (php.ini) Path: /usr/local/etc/php/5.6
Loaded Configuration File:         /usr/local/etc/php/5.6/php.ini
Scan for additional .ini files in: /usr/local/etc/php/5.6/conf.d
Additional .ini files parsed:      (none)
$ /usr/local/php/bin/php --ini
Configuration File (php.ini) Path: /usr/local/php/etc/
Loaded Configuration File:         /usr/local/php/etc/php.ini
Scan for additional .ini files in: (none)
Additional .ini files parsed:      (none)
```
上述的服务器上我们安装了两个版本的PHP，由上可以看到，使用php –ini命令可以很方便的定位当前PHP命令将会采用哪个配置文件。
### 查看类/函数/扩展信息
通常，我们可以使用php –info命令或者在在web服务器上的php程序中使用函数phpinfo()显示php的信息，然后再查找相关类、扩展或者函数的信息，这样做实在是麻烦了一些。


```
$ php --info | grep redis
redis
Registered save handlers => files user redis
This program is free software; you can redistribute it and/or modify
```
我们可以使用下列参数更加方便的查看这些信息


```
--rf       显示关于函数  的信息.
--rc       显示关于类  的信息.
--re       显示关于扩展  的信息.
--rz       显示关于Zend扩展  的信息.
--ri       显示扩展  的配置信息.
```
例如，我们希望查看扩展redis的配置信息


```
$ php --ri redis
redis
Redis Support => enabled
Redis Version => 2.2.7
```
查看redis类的信息


```
$ php --rc redis
Class [  class Redis ] {
  - Constants [19] {
    Constant [ integer REDIS_NOT_FOUND ] { 0 }
    ...
  - Methods [201] {
    ...
    Method [  public method echo ] {
    }
    ...
```
查看函数printf的信息


```
$ php --rf printf
Function [  function printf ] {
  - Parameters [2] {
    Parameter #0 [  $format ]
    Parameter #1 [  ...$args ]
  }
}
```
### 语法检查
有时候，我们只需要检查php脚本是否存在语法错误，而不需要执行它，比如在一些编辑器或者IDE中检查PHP文件是否存在语法错误。
使用-l（–syntax-check）可以只对PHP文件进行语法检查。


```
$ php -l index.php
No syntax errors detected in index.php
```
假如此时我们的index.php中存在语法错误


```
$ php -l index.php
PHP Parse error:  syntax error, unexpected 'echo' (T_ECHO) in index.php on line 3
Parse error: syntax error, unexpected 'echo' (T_ECHO) in index.php on line 3
Errors parsing index.php
```
## 命令行脚本开发
在使用PHP开发命令行脚本的时候，与开发web程序是明显不同的，在web程序中，我们可以通过改变url的参数，为PHP环境提供不同的输入，但是在命令行脚本程序中如何获取外部的输入呢？
在使用C语言开发程序时，我们通常会在main函数中提供两个可选的参数int main(int argc, char *argv[])，这两个参数就是从命令行提供的输入参数。在PHP中，提供了两个全局变量$argc和$argv用于获取命令行输入。
- $argc 包含了 $argv数组包含元素的数目
- $argv 是一个数组，包含了提供的参数，第一个参数总是脚本文件名称
假设我们有一个名为console.php的命令行脚本文件


```
<?php echo '命令行参数个数: ' . $argc . "n";
echo "命令行参数:n";
foreach ($argv as $index => $arg) {
    echo "    {$index} : {$arg}n";
}
```
在命令行下执行该脚本


```
$ php console.php hello world
命令行参数个数: 3
命令行参数:
    0 : console.php
    1 : hello
    2 : world
```
可以看到，第0个参数是我们执行的脚本名称。需要注意的是，如果提供的第一个参数是以-开头的话，需要在前面增加–，以告诉php这后面的参数是提供给我们的脚本的，而不是php执行文件的（php -r ‘var_dump($argv);’ — -h）。
另外，在脚本中，我们可以通过php_sapi_name()函数判断是否是在命令行下运行的


```
$ php -r 'echo php_sapi_name(), PHP_EOL;'
cli
```
参考文献
- [Using PHP from the command line](http://php.net/manual/en/features.commandline.php)
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2017/01/e8b93699352c7d38fa1d9d15a95ba461.jpg)![](http://jbcdn2.b0.upaiyun.com/2017/01/2197f30341190c0c9daab43dd2d4ce0c.jpg)

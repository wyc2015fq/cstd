# 从零开始创建一个 PHP 扩展 - 文章 - 伯乐在线
原文出处： [信海龙（@淘宝信海龙）](http://www.bo56.com/php7%E6%89%A9%E5%B1%95%E5%BC%80%E5%8F%91%E4%B9%8Bhello-word/)
本文是以PHP7作为基础，讲解如何从零开始创建一个PHP扩展。本文主要讲解创建一个扩展的基本步骤都有哪些。示例中，我们将实现如下功能：

PHP
```
<?php
echo say();
?>
```
输出内容：

PHP
```
$ php ./test.php
$ hello word
```
在扩展中实现一个say方法，调用say方法后，输出 hello word。
## 第一步：生成代码
PHP为我们提供了生成基本代码的工具 ext_skel。这个工具在PHP源代码的./ext目录下。

PHP
```
$ cd php_src/ext/
$ ./ext_skel --extname=say
```
extname参数的值就是扩展名称。执行ext_skel命令后，这样在当前目录下会生成一个与扩展名一样的目录。
## 第二步，修改config.m4配置文件
config.m4的作用就是配合phpize工具生成configure文件。configure文件是用于环境检测的。检测扩展编译运行所需的环境是否满足。现在我们开始修改config.m4文件。

PHP
```
$ cd ./say
$ vim ./config.m4
```
打开，config.m4文件后，你会发现这样一段文字。

PHP
```
dnl If your extension references something external, use with:
 dnl PHP_ARG_WITH(say, for say support,
 dnl Make sure that the comment is aligned:
 dnl [  --with-say             Include say support])
 dnl Otherwise use enable:
 dnl PHP_ARG_ENABLE(say, whether to enable say support,
 dnl Make sure that the comment is aligned:
 dnl [  --enable-say           Enable say support])
```
其中，dnl 是注释符号。上面的代码说，如果你所编写的扩展如果依赖其它的扩展或者lib库，需要去掉PHP_ARG_WITH相关代码的注释。否则，去掉 PHP_ARG_ENABLE 相关代码段的注释。我们编写的扩展不需要依赖其他的扩展和lib库。因此，我们去掉PHP_ARG_ENABLE前面的注释。去掉注释后的代码如下：

PHP
```
dnl If your extension references something external, use with:
 dnl PHP_ARG_WITH(say, for say support,
 dnl Make sure that the comment is aligned:
 dnl [  --with-say             Include say support])
 dnl Otherwise use enable:
 PHP_ARG_ENABLE(say, whether to enable say support,
 Make sure that the comment is aligned:
 [  --enable-say           Enable say support])
```
## 第三步，代码实现
修改say.c文件。实现say方法。
找到PHP_FUNCTION(confirm_say_compiled)，在其上面增加如下代码：

PHP
```
PHP_FUNCTION(say)
{
        zend_string *strg;
        strg = strpprintf(0, "hello word");
        RETURN_STR(strg);
}
```
找到 PHP_FE(confirm_say_compiled, 在上面增加如下代码：

PHP
```
PHP_FE(say, NULL)
```
修改后的代码如下：

PHP
```
const zend_function_entry say_functions[] = {
     PHP_FE(say, NULL)       /* For testing, remove later. */
     PHP_FE(confirm_say_compiled,    NULL)       /* For testing, remove later. */
     PHP_FE_END  /* Must be the last line in say_functions[] */
 };
 /* }}} */
```
## 第四步，编译安装
编译扩展的步骤如下：

PHP
```
$ phpize
$ ./configure
$ make
```
修改php.ini文件，增加如下代码：

PHP
```
[say]
extension = say.so
```
然后执行，php -m 命令。在输出的内容中，你会看到say字样。
## 第五步，调用测试
自己写一个脚本，调用say方法。看输出的内容是否符合预期。
## 完整代码下载
[完整的扩展源代码下载(tar.gz格式)。](http://www.bo56.com/download/php7_ext/say.tar.gz)
[完整的扩展源代码下载(zip格式)。](http://www.bo56.com/download/php7_ext/say.zip)

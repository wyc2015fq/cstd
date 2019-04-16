# PHP相关系列 - PHP CLI - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年10月12日 16:34:41[initphp](https://me.csdn.net/initphp)阅读数：998标签：[php																[脚本																[shell																[zend																[input](https://so.csdn.net/so/search/s.do?q=input&t=blog)
个人分类：[PHP相关系列](https://blog.csdn.net/initphp/article/category/2609095)





                
所有的PHP发行版，不论是编译自源代码的版本还是预创建的版本，都在默认情况下带有一个PHP可执行文件。这个可执行文件可以被用来运行命令行的PHP程序。

    要在你的系统上找到这个可执行文件，就要遵照下面的步骤：

        在Windows操作系统里，它被放在PHP主安装目录下，文件名是php.exe或者（在老版本的PHP里）是php-cli.exe。

        在Linux操作系统里，它被保存在PHP安装目录的bin/子目录下。

    不论是在哪一个操作系统里，你都需要对它进行测试，以保证它能够正常运行，方法是用-v参数调用它：

shell> /path/to/php -v

PHP 5.0.0 (cli) (built: Jun 1 2005 18:32:10)

Copyright (c) 1997-2004 The PHP Group

Zend Engine v2.0.0, Copyright (c) 1998-2004 Zend Technologies

它应该会返回PHP的版本号。

**一个简单的PHP CLI程序**

    一旦找到了这个CLI可执行文件，你就可以用一个简单的程序来使用一下。创建一个简单的文本文件，其中包含有以下PHP代码，并把它保存为hello.php：

<?php

echo "Hello from the CLI";

?>

    现在，试着在命令行提示符下运行这个程序，方法是调用CLI可执行文件并提供脚本的文件名：

shell> /path/to/phphello.php

Hello from the CLI

**使用标准的输入和输出**

    PHP CLI会定义三个常量，以便让在命令行提示符下与解释器进行交互操作更加容易。这些常量见表格A。


**表格A**
|常量|说明|
|----|----|
|STDIN|标准的输入设备|
|STDOUT|标准的输出设备|
|STDERR|标准的错误设备|

    你可以在自己的PHP脚本里使用这三个常量，以接受用户的输入，或者显示处理和计算的结果。要更好地理解这一点，可以看看下面的脚本（列表A）：

**列表A**

<?php

// ask for input

fwrite(STDOUT, "Enter your name: ");

// get input

$name = trim(fgets(STDIN));

// write input back

fwrite(STDOUT, "Hello, $name!");

?>

Look what happens when you run it:

shell> /path/to/phphello.php

Enter your name: Joe

Hello, Joe!

    在这个脚本里，fwrite()函数首先会向标准的输出设备写一条消息，询问用户的姓名。然后它会把从标准输入设备获得的用户输入信息读取到一个PHP变量里，并它把合并成为一个字符串。然后就用fwrite()把这个字符串打印输出到标准的输出设备上。

**使用命令行自变量**

    在命令行里输入程序参数来更改其运行方式是很常见的做法。你也可以对CLI程序这样做。PHP CLI带有两个特殊的变量，专门用来达到这个目的：一个是$argv变量，它通过命令行把传递给PHP脚本的参数保存为单独的数组元素；另一个是$ argc变量，它用来保存$argv数组里元素的个数。

    用PHP脚本编写一段读取$argv并处理它所含参数的代码是很简单的。试试列表B里的示例脚本，看看它是如何工作的：

**列表B**

<?php

print_r($argv);

?>

Run this script by passing it some arbitrary values, and check the output:

shell> /path/to/phptest.php chocolate 276 "killer tie, dude!"

Array

( [0] => test.php

[1] => chocolate

[2] => 276

[3] => killer tie, dude!

)

    正如你可以从输出的结果看到的，传递给test.php的值会自动地作为数组元素出现在$argv里。要注意的是，$argvis的第一个自变量总是脚本自己的名称。

    下面是一个更加复杂的例子（列表C）：

**列表C**

<?php

// check for all required arguments

// first argument is always name of script!

if ($argc != 4) {

die("Usage: book.php <check-in-date> <num-nights> <room-type> ");

}

// remove first argument

array_shift($argv);

// get and use remaining arguments

$checkin = $argv[0];

$nights = $argv[1];

$type = $argv[2];

echo "You have requested a $type room for $nights nights, checking in on $checkin. Thank you for your order! ";

?>

    下面是其用法的示例：

shell> /path/to/phpbook.php 21/05/2005 7 single

You have requested a single room for 7 nights, checking in on 21/05/2005. Thank you for your order!

    在这里，脚本首先会检查$argc，以确保自变量的数量符合要求。它然后会从$argv里提取出每一个自变量，把它们打印输出到标准的输出设备上。

*注意：你可以用*[*Console_Getopt*](http://www.melonfire.com/community/columns/trog/article.php?id=259)*PEAR类向PHP增加更加复杂的命令行参数。*

**使用CLI参数**

    除了用命令行传递PHP脚本参数，你还可以传递PHP CLI参数以更改其工作方式。表格B就是一些重要参数的列表：

**表格B**
|参数|说明|
|----|----|
|-a|交互式运行Run interactively|
|-c|path 从path读取php的.ini文件|
|-n|不用读取php的.ini文件就直接运行|
|-m|列出经过编译的模块|
|-i|显示有关PHP构建的信息|
|-l|检查PHP脚本的句法|
|-s|以彩色方式显示源代码|
|-w|显示去掉注释之后的源代码|
|-h|显示帮助|

**交互模式**

    你还可以以交互方式使用PHP CLI，也就是输入命令，马上获得结果。要得到这种效果，只需要使用一个参数调用CLI可执行文件就行了，就像下面这样：

shell> /path/to/php -a

    你会看到一个空行，你可以在里面输入PHP代码。看看：

shell> /path/to/php -a

Interactive mode enabled

<?php

echo mktime();

1121187283

echo 2+2;

4

exit();

shell>

    或者，你可以不使用-a参数就调用CLI可执行文件，直接输入完整的脚本或者代码段。用<Ctrl>-D来结束代码段，并让CLI来执行它。见下面的例子：

shell> /path/to/php

<?php

echo date("d-M-Y h:i:s", time());

?>

<Ctrl-D>

12-Jul-2005 06:54:04

    这就是PHP的命令行，现在你应该已经对PHP CLI有了足够的了解，并开始使用它了。](https://so.csdn.net/so/search/s.do?q=zend&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)





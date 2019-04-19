# PHP 中使用命令行工具 在 Web 应用程序中运行 shell 命令 - sxf_123456的博客 - CSDN博客
2017年04月24日 20:55:48[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：592
> 
学习如何更好地集成脚本和命令行工具。本文考察如何使用 `shell_exec()`、`exec()`、`passthru()` 和 `system()`；安全地将信息传递到命令行；以及安全地从命令行获取信息。
如果您使用过 PHP，您就会发现它是创建特性丰富的 Web 页面的出色工具。作为一大脚本语言，PHP：
- 容易学习。
- 有许多强大的框架（比如 CakePHP 和 CodeIgniter），让您能够像 Rails 程序员一样高效。
- 能够与 MySQL、PostgreSQL、Microsoft® SQL Server，甚至 Oracle 通信。
- 能够轻松地与 JavaScript 框架集成，比如 script.aculo.us 和 jQuery。
但有时候，您想做更多的事情，或必须做更多的事情。我的意思是您必须直接与 PHP 运行的服务器的文件系统打交道。您最终需要处理文件系统上的文件，了解运行的进程或执行其他任务。
首先，您对在 PHP 使用 `file()` 命令打开文件很满意。但是在某种程度上，完成某些事情的唯一途径是在服务器上运行 shell 命令并获得特定的输出。例如，您可能想知道特定目录包含多少个文件。或者您想知道向某组日志文件写了多少行内容。或者您想操作这些文件，将它们复制到另一个目录，或使用 `rsync` 将它们发送到另一个位置。
在 “[PHP 命令行？是的，您可以！](http://www.ibm.com/developerworks/cn/opensource/os-php-command/)” 这篇文章中，Roger
 McCoy 演示了如何从命令行直接使用 PHP —— 不需任何 Web 浏览器。在这篇文章中，我从另一个角度看待相同的主题，向您展示如何紧密地与底层 shell 命令集成，以及将返回值包含到您的界面和进程中。
仅当您运行在 Linux®、Berkeley Software Distribution (BSD) 或一些其他 UNIX® 版本上时，这些操作才有效。我假设您运行在 Linux-Apache-MySQL-PHP (LAMP) 堆栈上。如果您运行其他版本的 UNIX，具体细节可能不同，因为在每个版本中命令行的可用性都不同。我知道很多人还在 Mac OS X（运行某个版本的 BSD）从事开发，因此我尽量保持示例命令的通用性，确保移植方便。
命令行概述
PHP Command Line Interface (CLI) Server Application Programming Interface (SAPI) 在 PHP V4.2.0 开始发布，用于试验目的。到 V4.3.0 时，已经受到完整支持并且默认启用。PHP CLI SAPI 允许您开发 PHP 支持的 shell 脚本，甚至是基于桌面的脚本。事实上，可以用 PHP 创建可直接从命令行运行的工具。采用这种方式，PHP 开发人员可以像 Perl、AWK、Ruby 或 shell 程序员一样高效。
本文探究构建到 PHP 中的工具，让您了解 PHP 运行的底层 shell 环境和文件系统。PHP 为执行外部命令提供大量函数，其中包括 `shell_exec()`、`exec()`、`passthru()` 和 `system()`。这些命令是相似的，但为您运行的外部程序提供不同的界面。所有这些命令都衍生一个子进程，用于运行您指定的命令或脚本，并且每个子进程会在命令输出写到标准输出
 (`stdout`) 时捕捉它们。
`shell_exec()`
`shell_exec()` 命令行实际上仅是反撇号 (`) 操作符的变体。如果您编写过 shell 或 Perl 脚本，您就知道可以在反撇号操作符内部捕捉其他命令的输出。例如，清单 1 显示了如何使用反撇号在当前目录中获取每个文本（.txt）的单词计数。
清单 1. 使用反撇号计算单词数量
#! /bin/sh
number_of_words=`wc -w *.txt`
echo $number_of_words
#result would be something like:
#165 readme.txt 388 results.txt 588 summary.txt
#and so on....
在您的 PHP 脚本中，您可以在 `shell_exec()` 中运行这个简单的命令，如清单 2 所示，并获取想要的结果。这里假设在同一个目录下有一些文本文件。
清单 2. 在 `shell_exec()` 中运行相同的命令
<?php
$results = shell_exec('wc -w *.txt');
echo $results;
?>
在图 1 中可以看到，获得的结果与从 shell 脚本得到的一样。这是因为 `shell_exec()` 允许您通过 shell 运行外部程序，然后以字符串的形式返回结果。
图 1. 通过 `shell_exec()` 运行
 shell 命令的结果
![shell_exec()](http://www.ibm.com/developerworks/cn/opensource/os-php-commandline/figure1.jpg)
在本文的后面部分，您将学习如何使用 PHP 为这些脚本传递参数。现在您可以将它看作运行 shell 命令的一种方法，但要记住您只能看到标准输出。如果命令或脚本出现错误，您将看不到标准的错误 (`stderr`)，除非您通过竖线将它添加到 `stdout`。
`passthru()`
`passthru()` 允许您运行外部程序，并在屏幕上显示结果。您不需要使用 `echo` 或 `return` 来查看结果；它们会显示在浏览器上。您可以添加可选的参数，即保存从外部程序返回的代码的变量，比如表示成功的
 0，这为调试提供更好的机制。
在清单 6 中，我使用 `passthru()` 命令运行在前面小节运行的单词计数脚本。如您所见，我还添加一个包含返回代码的 `$returnval` 变量。
清单 6. 使用 `passthru()` 命令运行单词计数脚本
<?php
passthru('wc -w *.txt | head -5',$returnval);
echo "<hr/>".$returnval;
?>
注意，我不需要使用 `echo` 返回任何东西。结果会直接显示在屏幕上，如下所示。
图 3. 使用 `return` 代码运行 `passthru()` 命令的结果
![passthru()](http://www.ibm.com/developerworks/cn/opensource/os-php-commandline/figure3.jpg)
`exec()`
`exec()` 命令与 `shell_exec()` 相似，不同之处是它返回输出的最后一行，并且可选地用命令的完整输出和错误代码填充数组。清单
 8 展示了当运行 `exec()` 而不捕捉数据数组中的数据时发生的事情。
清单 8. 运行 `exec()` 而不捕捉数据数组中的数据
<?php
$results = exec('wc -w *.txt | head -5');
echo $results;
#would print out just the last line or results, i.e.:
#3847 myfile.txt
?>
为了捕捉数组中的结果，要将该数组的名称作为第二个参数添加到 `exec()`。我在清单 9 中执行了这个步骤，并以`$data` 作为数组的名称。
清单 9. 从 `exec()` 捕捉数据数组的结果
<?php
$results = exec('wc -w *.txt | head -5',$data);
print_r($data);
#would print out the data array:
#Array ( [0]=> 555 text1.txt [1] => 283 text2.txt) 
?>
在捕捉数组中的结果之后，您可以对每行进行一些处理。例如，您可以在第一个空格处进行划分，将分离的值存储在数据库表中，或对每个行应用特定的格式或标记。
`system()`
如清单 10 所示，`system()` 命令是一种混合体。它像 `passthru()` 一样直接输出从外部程序接收到的任何东西。它还像 `exec()` 一样返回最后一行，并使返回代码可用。
清单 10. `system()` 命令
<?php
system('wc -w *.txt | head -5');
#would print out:
#123 file1.txt 332 file2.txt 444 file3.txt
#and so on
?>
![](http://www.ibm.com/i/v14/rules/blue_rule.gif)
![](http://www.ibm.com/i/c.gif)|![](http://www.ibm.com/i/c.gif)|![](http://www.ibm.com/i/v14/icons/u_bold.gif)|[回页首](http://www.ibm.com/developerworks/cn/opensource/os-php-commandline/#main)||![](http://www.ibm.com/i/v14/icons/u_bold.gif)|[回页首](http://www.ibm.com/developerworks/cn/opensource/os-php-commandline/#main)|
|----|----|----|
|![](http://www.ibm.com/i/v14/icons/u_bold.gif)|[回页首](http://www.ibm.com/developerworks/cn/opensource/os-php-commandline/#main)| |
一些例子
现在您已经了解如何使用这些 PHP 命令，但可能仍然有一些疑问。例如，什么时候应该使用哪个命令？这完全由您的需求决定。
大多数情况下，我使用 `exec()` 命令和数据数组处理所有东西。或者对更简单的命令使用 `shell_exec()`，尤其是不关心结果时。如果仅需返回一个
 shell 脚本，我就使用 `passthru()`。通常，我在不同的场合中使用不同的函数，并且有时它们是可以互换的。这完全取决于我的心情和要实现的目的。
您可能提问的另一个问题是 “它们的长处是什么？”。如果您没有头绪，或者一个项目非常适合使用 shell 命令，但不知道如何使用，那么我在这里提供一些见解。
如果您正在编写一个提供各种备份或文件传输功能的应用程序，您可以选择使用 `shell_exec()` 或这里提供的其他命令之一运行 `rsync` 支持的
 shell 脚本。您可以编写 shell 脚本使其包含必要的 `rsync` 命令，然后使用`passthru()` 根据用户的命令或 `cron` 作业执行它。
例如，一位用户在您的应用程序中有适当的权限（比如管理员权限），他想将 50 个 PDF 文件从一个服务器发送到另一个服务器。那么，该用户需要在应用程序中导航到正确的位置，单击 Transfer，选择需要发送的 PDF，然后单击 Submit。在这个过程中，该表单应该有一个 PHP 脚本，它使用返回选项变量通过 `passthru()` 运行 `rsync` 脚本，这样您就知道是否发生问题，如下所示。
清单 11. 通过 `passthru()` 运行 `rsync` 脚本的示例
 PHP 脚本
<?php
passthru('xfer_rsync.sh',$returnvalue);
if ($returnvalue != 0){
    //we have a problem!
    //add error code here
}else{
    //we are okay
    //redirect to some other page
}
?>
如果您的应用程序需要列出进程或文件，或关于这些进程或文件的数据，您可以使用本文总结的命令之一轻松实现这个目的。例如，一个简单的 `grep` 命令能够帮助您找到匹配特定搜索条件的文件。将它与 `exec()` 命令一起使用可以将结果保存到一个数组中，这允许您构建一个
 HTML 表或表单，它们又进一步允许您运行其他命令。
到目前为止，我讨论了用户生成的事件 —— 用户只要按下按钮或单击链接，PHP 就运行相应的脚本。您还可以将独立的 PHP 脚本和 `cron` 或其他日程安排程序一起使用，从而实现一些有趣的效果。例如，如果您一个备份脚本，您可以通过 `cron` 运行它，或者将它打包到
 PHP 脚本后在运行。为什么要这样做？这似乎是多余的，不是吗？不是这样的 —— 您需要这样考虑，您可以通过 `exec()` 或 `passthru()` 运行备份脚本，然后根据返回代码执行一些行为。如果出现错误，您可以将其记录到错误日志或数据库中，或发送一封警告电子邮件。如果脚本成功，您可以将原始的输出转储到数据库（例如，`rsync` 有一个详尽（verbose）模式，对随后诊断问题十分有用）。
![](http://www.ibm.com/i/v14/rules/blue_rule.gif)
![](http://www.ibm.com/i/c.gif)|![](http://www.ibm.com/i/c.gif)||||||
|----|----|----|
||| |
安全
我们在这里简要讨论一下安全性：如果您接受用户输入并将信息传递到 shell，那么最好过滤用户输入。删除您认为有害的命令和不允许的内容，比如 `sudo`（作为超级用户运行）或 `rm`（删除）。事实上，您可能不希望用户发送开放的请求，而是让他们从列表中选择。
例如，您运行一个接受文件列表作为参数的传输程序，您应该通过一系列复选框列出所有文件。用户可以选择和取消选择文件，并通过单击 Submit 激活 `rsync` shell 脚本。用户不能自己输入文件或使用正则表达式。

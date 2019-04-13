
# 1.5 输出Hello World -  芝麻软件工作室的专栏 - CSDN博客


2016年06月30日 09:56:05[seven-soft](https://me.csdn.net/softn)阅读数：209个人分类：[Python																](https://blog.csdn.net/softn/article/category/6290759)



现在，我们将看到在Python中如何运行一个传统的“Hello World”程序。这将教你如何写、保存和运行Python程序。
使用Python运行你的程序有两种方法——使用交互式解释器提示符或使用一个源文件。现在,我们将看到如何使用这两种方法。
## 使用解释器提示符
在您的操作系统中打开终端（如上节所示），然后，输入“python”按回车键，打开Python提示符。
一旦你启动python，您应该看到“>>>”，这被称为“Python解释器提示符”，你可以开始输入的东西。
在Python解释器提示符下，输入print('Hello World')

后按回车键。您应该看到输出了单词“Hello World”。
当使用一个Mac OS X计算机，下面是你将看到的一个例子。Python软件的细节会根据你的电脑不同而有所不同，但从提示符（即从“>>>”开始）与操作系统无关，应该是相同。
$ python
Python 3.3.0 (default, Oct 22 2012, 12:20:36)
[GCC 4.2.1 Compatible Apple Clang 4.0 ((tags/Apple/clang-421.0.60))] on darwin
Type "help", "copyright", "credits" or "license" for more information.
>>> print('hello world')
hello world
>>>
注意，Python让你的代码行立即输出了!你刚才输入的是一个Python 语句。我们使用print打印出(不出所料)你提供给它的任何值。在这里,我们提供的是文本“Hello World”，并立即打印到屏幕上。
## 如何解释器提示符
如果你正在使用一个Linux或Unix shell，您可以通过按下“ctrl - d’或输入“exit()“（注意：记得包含括号“()”），然后输入回车键。如果您使用的是Windows命令行提示符，按“ctrl - z”键再按“回车”键，退出解释器提示符。


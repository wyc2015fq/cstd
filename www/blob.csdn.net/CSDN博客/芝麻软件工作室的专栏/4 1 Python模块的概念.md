
# 4.1 Python模块的概念 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月01日 07:39:40[seven-soft](https://me.csdn.net/softn)阅读数：145


您已经看到如何通过一次定义函数在程序中重用代码。如果你想在其它程序中重用一定数量的函数，你将写什么？正如你可能已经猜到了，答案是模块。
编写模块有各种各样的方法，但是最简单的方法是创建一个以.py 为扩展名、包含函数和变量的文件。
编写模块的另一种方式是使用编写Python解释器本身的本机语言，例如，你可以使用（C语言：[http://see.xidian.edu.cn/cpp/](http://see.xidian.edu.cn/cpp/)）编写模块，当它们被编译后，当使用标准的Python解释器时，在你Python代码中可以使用（这些模块）。
一个模块可以因另一个程序使用其功能而被导入(import)。同样，我们可以使用Python标准库。首先 ，我们将看到如何使用标准库模块。
使用标准库模块举例（保存为 using_sys.py）：importsys
print('命令行参数是：')
foriinsys.argv:
print(i)
print('
PYTHONPATH在', sys.path, '
')

使用$ python using_sys.py we are arguments语句执行该文件，输出：
命令行参数是：
using_sys.py
we
are
arguments
PYTHONPATH在[<nowiki>''</nowiki>, 'C:\Windows\system32\python30.zip',
'C:\Python30\DLLs', 'C:\Python30\lib',
'C:\Python30\lib\plat-win', 'C:\Python30',
'C:\Python30\lib\site-packages']
它是如何工作的：
首先，我们使用import语句导入sys（系统）模块。基本上，这意味着我们我们想告诉Python，我们想使用这个模块。sys模块包含了与Python解释器和其环境--即system系统--有关的函数。
当Python执行import sys 语句时，它查找sys模块。在这里，它是一个内建模块，因此，Python知道到去哪里找到它。
如果它不是一个编译的，也就是用Python写的模块，那么，Python解释器将在sys.path变量列表中的目录中搜索。如果模块被发现，那么，模块中的代码将运行，对你来说，使用模块变为有效。
注意，初始化只有在我们第一次导入一个模块时完成。
在sys模块中的argv变量是通过点符号访问的，例如，例如，sys.argv。它清楚地表明，这个名字是sys模块的一部分。这种方法的另一个优点是，这个名字与你的程序中使用的任何argv变量都不冲突。
sys.argv变量一个字符串列表(list)（列表将在后面章节中详细解释）。具体来说，sys.argv包含命令行参数，也就是使用命令行向你的程序传递参数的列表。
如果您正在使用IDE编写并运行这些程序，在菜单中寻找一种方法来指定命令行参数传递给你的程序。
这里，当我们执行python using_sys.py we are arguments时，我们使用 python命令和其后的传递给程序的参数运行using_sys.py模块。Python把命令行参数存储在 sys.argv变量中供我们使用。
记住，运行脚本的名字通常是sys.argv列表中的第一个参数。因此，在这里将有'using_sys.py'作为sys.argv[0]，'we'作为sys.argv[1]，'are'作为sys.argv[2]和'arguments'作为sys.argv[3]。注意，Python从0而不是1开始数数。
sys.path包含被导入的模块所在的目录名列表。观察到sys.path就是的第一个字符串是空的——这个空字符串表示当前目录是和PYTHONPATH环境变量相同的sys.path变量的一部分。这意味着你可以直接导入位于当前目录中的模块。否则，你将不得不把你的模块存放在sys.path列表中的一个目录中。
请注意，当前目录是程序启动的目录。运行import os; print(os.getcwd())找到你的程序的当前目录。


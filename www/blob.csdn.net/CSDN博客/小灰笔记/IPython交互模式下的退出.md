# IPython交互模式下的退出 - 小灰笔记 - CSDN博客





2017年04月03日 23:07:00[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：11560
个人分类：[Python](https://blog.csdn.net/grey_csdn/article/category/6639459)








如果进入了shell交互窗口，python的退出方式只能够用函数不能够用命令，这有时候让人感觉到很不习惯。因为函数会比命令多一个括号的输入，这让人有点反感。而Linux的终端、DOS CMD窗口、MATLAB等各种使用基本上都是一个exit退出命令即可。相比之下，Python的这种方式确实是让我觉得有点不舒服。
尽管有着使用上的不便利，但是使用时间长了还是能够养成使用Python的时候加个括号实现相应的退出。换了IPython之后，感觉又有一种回到Linux终端的感觉，刚刚试了一下，居然也支持命令式退出。

交互记录：

E:\WorkSpace\05_PythonDataAnalysis\chapter05>ipython

Python 2.7.11 | 64-bit | (default, Jun 11 2016, 11:33:47) [MSC v.1500 64 bit (AMD64)]

Type "copyright", "credits" or "license" for more information.



IPython 4.1.2 -- An enhanced Interactive Python.

?         -> Introduction and overview of IPython's features.

%quickref -> Quick reference.

help      -> Python's own help system.

object?   -> Details about 'object', use 'object??' for extra details.



In [1]: exit()



E:\WorkSpace\05_PythonDataAnalysis\chapter05>ipython

Python 2.7.11 | 64-bit | (default, Jun 11 2016, 11:33:47) [MSC v.1500 64 bit (AMD64)]

Type "copyright", "credits" or "license" for more information.



IPython 4.1.2 -- An enhanced Interactive Python.

?         -> Introduction and overview of IPython's features.

%quickref -> Quick reference.

help      -> Python's own help system.

object?   -> Details about 'object', use 'object??' for extra details.



In [1]: exit
两种方式都能够实现从交互界面的退出，虽然只是少敲了两个符号，三次键盘，但是能够省得下几次操作的体验总归还是会很好的。
            



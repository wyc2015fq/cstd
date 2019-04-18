# Windows上实现dos命令重命名 - 小灰笔记 - CSDN博客





2017年05月08日 19:56:36[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1740








    以前我曾经想使用这个功能，但是一直没有找到相应的实现方法。加上我用的脚本语言多少也能够实现类似的功能，在工作以及生活中也就没有继续深入探索。今天偶尔间找到了这个Windows内置的dos命令，使用起来还是比较方便的。

      这个命令就是doskey，具体的功能是编辑命令，重新调用Windows命令并创建宏。

      使用示范如下：

直接调用Python：

E:\WorkSpace\01_编程语言\03_Python\demos\python_exp08>pythondemo.py

hello!

取一个别名后，执行同样的操作：

E:\WorkSpace\01_编程语言\03_Python\demos\python_exp08>doskeyp="E:\\da_workspace\\User\\python.exe" $*



E:\WorkSpace\01_编程语言\03_Python\demos\python_exp08>pdemo.py

hello!

      执行的结果相同，说明重命名成功。这在脚本的简洁性上会带来很大的帮助！





# 4.4 Python创建模块 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月01日 07:42:33[seven-soft](https://me.csdn.net/softn)阅读数：203


创建自己的模块是很容易的，你一直在这样做，始终都是！这是因为每个Python程序就是一个模块。你只需要确保它有一个.py扩展名。下面的例子会让你明白。
例子（保存为mymodule.py）：defsayhi():
print('嗨，这是我的模块在讲话。')
__version__='0.1'

上面的是模块的一个示例。正如您可以看到的，和我们通过的Python程序相比，没有什么特别的。接下来我们要看如何在我们的其它程序中使用这个模块。
另一个模块（保存为mymodule_demo.py）：importmymodule
mymodule.sayhi()
print('版本',mymodule.__version__)

如果 mymodule.py 和 mymodule_demo.py 在同一目录下，则输出：
嗨，这是我的模块在讲话。
版本 0.1
它是如何工作的：
注意，我们使用点符号（.）来访问模块的成员。Python充分重用相同的符号产生了独特的'神谕的'的感觉，这样我们不需要不断学习新的方法来做事情。
这是使用from..import语法的一个版本（保存为mymodule_demo2.py）：frommymoduleimportsayhi,__version__
sayhi()
print('版本',__version__)

mymodule_demo2.py和mymodule_demo.py的输出相同。
注意，如果在导入模块中已经有一个__version__名字的声明，这里会有一个冲突。这也可能是因为它是常见的做法--对于每个模块使用这个名字声明它的版本号。因此，总是推荐选择import语句，虽然它可能让你的程序有点长。
你还可以使用：frommymoduleimport*

这将导入所有的公共名称如 sayhi，但不会导入__version__，因为它始于双下划线。
## 模块和文件
如果说模块是按照逻辑来组织 Python 代码的方法，那么文件便是物理层上组织模块的方法。
因此，一个文件被看作是一个独立模块，一个模块也可以被看作是一个文件。模块的文件名就是模块的名字加上扩展名
 .py。与其它可以导入类(class)的语言不同，在 Python 中你导入的是模块或模块属性。


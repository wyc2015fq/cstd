# python 多级包导入机制 - walkabc - 博客园
# [python 多级包导入机制](https://www.cnblogs.com/hitfire/articles/4508980.html)
##### python的package
包通常总是一个目录，目录下为首的一个文件便是 _*init**.py。然后是一些模块文件和子目录，假如子目录中也有 __init*_.py 那么它就是这个包的子包了。差不多就像这样吧：
Package1/ __init__.py
    Module1.py
    Module2.py
    Package2/ __init__.py
       Module1.py
       Module2.py
我们可以就这样导入一个包：
import Package1
或者调入一个子模块和子包：
from Package1 import Module1
from Package1 import Package2
import Packag1.Module1
import Packag1.Package2
可以深入好几层包结构：
from Package1.Package2 import Module1
import Package1.Package2.Module1
##### _*init*_.py文件
The _*init**.py files are required to make Python treat the directories as containing packages. In the simplest case, __init**.py can just be an empty file, but it can also execute initialization code for the package or set the __all*_ variable, described later.
_*init**.py 控制着包的导入行为。假如 __init*_.py 为空，那么仅仅导入包是什么都做不了的。
>>> import Package1
>>> Package1.Module1
Traceback (most recent call last):
 File "<pyshell#1>", line 1, in ? 
 Package1.Module1
AttributeError: 'module' object has no attribute 'Module1'
我们需要在 _*init*_.py 里把 Module1 预先导入：
import Module1
测试：
>>> import Package1
>>> Package1.Module1
<module 'Package1.Module1' from 
'Module.pyc'>
_*init**.py 中还有一个重要的变量，叫做 __all*_。我们有时会使出一招"全部导入"，也就是这样：
from PackageName import *
这时 import 就会把注册在包 _*init**.py 文件中 __all*_ 列表中的子模块和子包导入到当前作用域中来。比如：
__all__ = ['Module1', 'Module2', 'Package2']
测试：
>>> from Package1 import *
>>> Module2
<module 'Package1.Module2' from 'Module.pyc'>
_*init*_.py其实就是一个普通的python文件，它会在package被导入时执行。
print ">>>in package1.__init__.py"
def say_hello():
    print "Hi, my name is Forrest!"
测试：
In [1]: import package1
>>>in package1.__init__.py
In [2]: package1.say_hello()
Hi, my name is Forrest!

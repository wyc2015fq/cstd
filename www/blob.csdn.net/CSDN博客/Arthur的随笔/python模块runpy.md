# python模块runpy - Arthur的随笔 - CSDN博客
2012年08月27日 23:49:17[largetalk](https://me.csdn.net/largetalk)阅读数：5761
[http://www.python.org/dev/peps/pep-0366/](http://www.python.org/dev/peps/pep-0366/)
[http://www.python.org/dev/peps/pep-0338/](http://www.python.org/dev/peps/pep-0338/)
The [runpy](http://docs.python.org/library/runpy.html#module-runpy) module
 is used to locate and run Python modules without importing them first. Its main use is to implement the [-m](http://docs.python.org/using/cmdline.html#cmdoption-m) command
 line switch that allows scripts to be located using the Python module namespace rather than the filesystem.
runpy模块用于：在不import python模块时，定位并执行该模块。主要用途在于实现命令行-m执行python 模块得效果，但是是在脚本中而不是文件系统上。
runpy一个就两个函数：
runpy.run_module(mod_name, init_globals=None, run_name=None, alter_sys=False)
运行指定模块代码并返回模块得全局字典。
例子：
```python
import runpy
from pprint import pprint
ttt = runpy.run_module('classproperty',  alter_sys=True)
pprint(ttt)
```
runpy.run_path(file_path, init_globals=None, run_name=None)
执行指定脚本文件并返回模块全局字典。
例子：
首先写一个脚本
```python
$ cat a.py 
print 'iiiiiiiiiiiiinnnnnnnnnnnnnn a %s'%bb
print __name__
print __file__
print __loader__
print __package__
if __name__ == '__main__':
    print 'in __main__'
```
然后
```python
import runpy
from pprint import pprint
bb = 'uuu'
ggg = runpy.run_path('a.py', init_globals={'bb':bb}, run_name='__main__')
print '====================='
pprint( ggg)
```
init_globals是传给运行module（文件）的字典，有四个全局变量是一定会传过去得：__name__, __file__, __loader__ and __package__

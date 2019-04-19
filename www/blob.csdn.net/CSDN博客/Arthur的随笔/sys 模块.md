# sys 模块 - Arthur的随笔 - CSDN博客
2013年01月30日 23:06:12[largetalk](https://me.csdn.net/largetalk)阅读数：6392
**sys.excepthook **未捕获异常处理的顶层函数
```python
import sys
def new_hook(t, v, tb):
    print 'in new hook'
    return sys.__excepthook__(t, v, tb)
sys.excepthook = new_hook
raise ValueError('asdfasdfa')
```
**sys.displayhook**:  交互python解析器时的打印函数
```python
In [1]: import sys
In [2]: def new_display(v):
   ...:     print 'in new displayhook'
   ...:     return sys.__displayhook__(v)
   ...: 
In [3]: sys.displayhook = new_display
In [4]: 1+1
in new displayhook
2
```
**sys.exc_info**  当前处理的异常信息
**sys.exc_clear **清除当前的异常信息
```python
import sys
import traceback
def a():
    raise ValueError('asdfasdf')
try:
    a()
except:
    print 'in except'
    print sys.exc_info()
    tb = sys.exc_info()[2]
    traceback.print_tb(tb)
print '------------'
print sys.exc_info()
print '############'
sys.exc_clear()
print sys.exc_info()
```
**sys.exec_prefix ** 放置平台依赖的python文件目录前缀
**sys.executable**  python可执行文件路径
**sys.exit  退**出python， 实现原来是抛出一个SystemExit异常，所以可以捕获异常而不退出
**atexit.register **注册退出时函数
```python
import sys
import atexit
def exitfunc():
    print 'do job before exit'
atexit.register(exitfunc)
try:
    sys.exit(1)
except:
    print 'aaaa'
print 'bbb'
sys.exit(2)
print 'ccccc'
```
**sys.getcheckinterval    sys.setcheckinterval **  线程切换前python指令执行条数(GIL)
**sys.getfilesytemencoding ** 文件系统编码
**sys.getrefcount ** 得到对象的引用次数， 会比你期望的多一次， 因为这次调用也算
**sys.getrecursionlimit ** 递归调用最大深度
**sys.getsizeof**  得到对象所占字节数，对于内建类型对象，返回的值是正确的，对于第三方扩展类型则不一定
**sys._getframe** 得到调用栈
```python
import sys
def a():
    try:
        for i in range(100):
            f = sys._getframe(i)
            print f.f_code.co_name, f.f_code.co_filename, f.f_lineno
    except:
        pass
def b():
    a()
def c():
    b()
c()
```
**sys.maxunicode** 为65535时，unicode是UCS-2 为1114111时， unicode是UCS-4
**sys.meta_path**  find_module调用时查找的一系列路径， 优先级高于sys.path
**sys.modules** 已经load的模块
**sys.path**
**sys.path_hooks** 对于一个路径，能创建一个finder的可执行对象列表。比如从zip中import python模块。 see [PEP302](http://www.python.org/dev/peps/pep-0302/)
**sys.prefix **  放置平台独立python文件的路径前缀
**sys.ps1 sys.ps2 sys.ps3** 交互python的提示符
**sys.py3kwarning **python 3 warning flag
**sys.settrace **设置trace调用函数， 详细例子见：http://www.doughellmann.com/PyMOTW/sys/tracing.html
**sys.settsdump**Activate dumping of VM measurements
**sys.tracebacklimit** traceback最大深度

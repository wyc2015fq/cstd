# Python内置函数(48)——__import__ - lincappu - 博客园







# [Python内置函数(48)——__import__](https://www.cnblogs.com/lincappu/p/8145098.html)





**英文文档：**

`__import__`(*name*, *globals=None*, *locals=None*, *fromlist=()*, *level=0*)

This function is invoked by the `import` statement. It can be replaced (by importing the `builtins` module and assigning to `builtins.__import__`) in order to change semantics of the `import` statement, but doing so is **strongly** discouraged as it is usually simpler to use import hooks (see [**PEP 302**](https://www.python.org/dev/peps/pep-0302)) to attain the same goals and does not cause issues with code which assumes the default import implementation is in use. Direct use of `__import__()` is also discouraged in favor of `importlib.import_module()`.

The function imports the module *name*, potentially using the given *globals* and *locals* to determine how to interpret the name in a package context. The *fromlist* gives the names of objects or submodules that should be imported from the module given by *name*. The standard implementation does not use its *locals* argument at all, and uses its *globals* only to determine the package context of the `import` statement.

*level* specifies whether to use absolute or relative imports. `0` (the default) means only perform absolute imports. Positive values for *level* indicate the number of parent directories to search relative to the directory of the module calling `__import__()` (see [**PEP 328**](https://www.python.org/dev/peps/pep-0328) for the details).

When the *name* variable is of the form `package.module`, normally, the top-level package (the name up till the first dot) is returned, *not* the module named by *name*. However, when a non-empty *fromlist* argument is given, the module named by *name* is returned.

 　　动态导出模块

**说明：**

　　1. 函数功能用于动态的导入模块，主要用于反射或者延迟加载模块。

　　2. __import__(module)相当于import module

　　先定义两个模块mian.py和index.py，两个文件在同一目录下：

```
#index.py

print ('index')

def sayHello():
    print('hello index')

def sayHelloZhCn():
    print('你好 index')
```

```
#mian.py
print ('main')

index = __import__('index')
dir(index)
index.sayHello()
index.sayHelloZhCn()
```

　　执行main.py，可以证实动态加载了index.py，__import__返回的模块也是index模块

```
C:\Users\Admin\Documents\Python3\importtest>python main.py
main
index
hello index
你好 index
```

　　3. __import__(package.module)相当于from package import name，如果fromlist不传入值，则返回package对应的模块，如果fromlist传入值，则返回package.module对应的模块。

　　先定义archives包，其中包含user和role两个模块：

```
#__index__.py
print ('archives.__index__')

def sayHello():
    print('hello archives')
```

```
#user.py
print ('user')

def sayHello():
    print('hello user')
```

```
#role.py
print ('role')

def sayHello():
    print('hello role')
```

　　结构如下：

![](https://images2015.cnblogs.com/blog/232931/201612/232931-20161204021029677-1811063826.jpg)

　　修改mian.py：

```
#main.py
print ('main')

archives = __import__('archives')
archives.sayHello()
archives.user
```

　　执行main.py，可以证实动态加载了archives包，__import__返回的模块也是archives模块

```
C:\Users\Admin\Documents\Python3\importtest>python main.py
main
archives.__index__
hello archives
Traceback (most recent call last):
  File "main.py", line 5, in <module>
    archives.user
AttributeError: module 'archives' has no attribute 'user'
```

　　修改mian.py：

```
#main.py
print ('main')

archives = __import__('archives.user')
archives.sayHello()
print(archives.user)
```

　　执行main.py，可以证实动态加载了archives包的user模块，__import__返回的模块也是archives模块

```
C:\Users\Admin\Documents\Python3\importtest>python main.py
main
archives.__index__
user
hello archives
<module 'archives.user' from 'C:\\Users\\Admin\\Documents\\Python3\\import
test\\archives\\user.py'>
```

　　修改mian.py：

```
#main.py
print ('main')

archives = __import__('archives.user',fromlist = ('user',))
archives.sayHello()
print(archives)
```

　　执行main.py，可以证实动态加载了archives包的user模块，__import__返回的模块是user模块

```
C:\Users\Admin\Documents\Python3\importtest>python main.py
main
archives.__index__
user
hello user
<module 'archives.user' from 'C:\\Users\\Admin\\Documents\\Python3\\import
test\\archives\\user.py'>
```

　　4. level参数，指定是使用绝对导入还是相对导入。 0(默认值)表示只执行绝对导入。













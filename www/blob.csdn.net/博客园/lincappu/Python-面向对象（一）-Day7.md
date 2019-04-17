# Python-面向对象（一）-Day7 - lincappu - 博客园







# [Python-面向对象（一）-Day7](https://www.cnblogs.com/lincappu/p/8157520.html)






Day7-面向对象基础 1
一、isinstance(obj, cls) 1
二、issubclass(sub, super) 1
三、异常处理 1
1、异常基础 1
2、异常种类 2
3、异常其他结构 5
4、主动触发异常 5
5、自定义异常 5
6、断言 6
四、反射 6
类也是对象 12
模块也是对象 12
五、设计模式 13
5.1、单例模式 13

# Day7-面向对象基础

## 一、isinstance(obj, cls)

检查是否obj是否是类 cls 的对象

```
class Foo(object):
    pass
 
obj = Foo()
 
isinstance(obj, Foo)
```

## 二、issubclass(sub, super)

检查sub类是否是 super 类的派生类

```python
class Foo(object):
    pass
 
class Bar(Foo):
    pass
 
issubclass(Bar, Foo)
```

## 三、异常处理

### 1、异常基础

在编程过程中为了增加友好性，在程序出现bug时一般不会将错误信息显示给用户，而是现实一个提示的页面，通俗来说就是不让用户看见大黄页！！！

```python
try:
    pass
except Exception,ex:
    pass
```

需求：将用户输入的两个数字相加

```python
while True:
    num1 = raw_input('num1:')
    num2 = raw_input('num2:')
    try:
        num1 = int(num1)
        num2 = int(num2)
        result = num1 + num2
    except Exception, e:
        print '出现异常，信息如下：'
        print e
```

### 2、异常种类

python中的异常种类非常多，每个异常专门用于处理某一项异常！！！

```python
AttributeError 试图访问一个对象没有的树形，比如foo.x，但是foo没有属性x
IOError 输入/输出异常；基本上是无法打开文件
ImportError 无法引入模块或包；基本上是路径问题或名称错误
IndentationError 语法错误（的子类） ；代码没有正确对齐
IndexError 下标索引超出序列边界，比如当x只有三个元素，却试图访问x[5]
KeyError 试图访问字典里不存在的键
KeyboardInterrupt Ctrl+C被按下
NameError 使用一个还未被赋予对象的变量
SyntaxError Python代码非法，代码不能编译(个人认为这是语法错误，写错了）
TypeError 传入对象类型与要求的不符合
UnboundLocalError 试图访问一个还未被设置的局部变量，基本上是由于另有一个同名的全局变量，
导致你以为正在访问它
ValueError 传入一个调用者不期望的值，即使值的类型是正确的


ArithmeticError
AssertionError
AttributeError
BaseException
BufferError
BytesWarning
DeprecationWarning
EnvironmentError
EOFError
Exception
FloatingPointError
FutureWarning
GeneratorExit
ImportError
ImportWarning
IndentationError
IndexError
IOError
KeyboardInterrupt
KeyError
LookupError
MemoryError
NameError
NotImplementedError
OSError
OverflowError
PendingDeprecationWarning
ReferenceError
RuntimeError
RuntimeWarning
StandardError
StopIteration
SyntaxError
SyntaxWarning
SystemError
SystemExit
TabError
TypeError
UnboundLocalError
UnicodeDecodeError
UnicodeEncodeError
UnicodeError
UnicodeTranslateError
UnicodeWarning
UserWarning
ValueError
Warning
ZeroDivisionError


dic = ["wupeiqi", 'alex']
try:
    dic[10]
except IndexError, e:
    print e

dic = {'k1':'v1'}
try:
    dic['k20']
except KeyError, e:
    print e

s1 = 'hello'
try:
    int(s1)
except ValueError, e:
    print e
```

对于上述实例，异常类只能用来处理指定的异常情况，如果非指定异常则无法处理。

```python
#未捕获到异常，程序直接报错
 
s1 = 'hello'
try:
    int(s1)
except IndexError,e:
    print e
```

所以，写程序时需要考虑到try代码块中可能出现的任意异常，可以这样写：

```python
s1 = 'hello'
try:
    int(s1)
except IndexError,e:
    print e
except KeyError,e:
    print e
except ValueError,e:
    print e
```

万能异常 在python的异常中，有一个万能异常：Exception，他可以捕获任意异常，即：

```python
s1 = 'hello'
try:
    int(s1)
except Exception,e:
    print e
```

接下来你可能要问了，既然有这个万能异常，其他异常是不是就可以忽略了！

答：当然不是，对于特殊处理或提醒的异常需要先定义，最后定义Exception来确保程序正常运行。

```python
s1 = 'hello'
try:
    int(s1)
except KeyError,e:
    print '键错误'
except IndexError,e:
    print '索引错误'
except Exception, e:
    print '错误'
```

### 3、异常其他结构

```python
try:
    # 主代码块
    pass
except KeyError,e:
    # 异常时，执行该块
    pass
else:
    # 主代码块执行完，执行该块
    pass
finally:
    # 无论异常与否，最终执行该块
    pass
```

### 4、主动触发异常

```python
try:
    raise Exception('错误了。。。')
except Exception,e:
    print e
```

### 5、自定义异常

```python
class WupeiqiException(Exception):
 
    def __init__(self, msg):
        self.message = msg
 
    def __str__(self):
        return self.message
 
try:
    raise WupeiqiException('我的异常')
except WupeiqiException,e:
    print e
```

### 6、断言

```
# assert 条件
 
assert 1 == 1
 
assert 1 == 2
```

## 四、反射

python中的反射功能是由以下四个内置函数提供：hasattr、getattr、setattr、delattr，改四个函数分别用于对对象内部执行：检查是否含有某成员、获取成员、设置成员、删除成员。
![](https://images2015.cnblogs.com/blog/425762/201509/425762-20150907094454965-329821364.jpg)

```python
class Foo(object):
 
    def __init__(self):
        self.name = 'wupeiqi'
 
    def func(self):
        return 'func'
 
obj = Foo()
 
# #### 检查是否含有成员 ####
hasattr(obj, 'name')
hasattr(obj, 'func')
 
# #### 获取成员 ####
getattr(obj, 'name')
getattr(obj, 'func')
 
# #### 设置成员 ####
setattr(obj, 'age', 18)
setattr(obj, 'show', lambda num: num + 1)
 
# #### 删除成员 ####
delattr(obj, 'name')
delattr(obj, 'func')

详细解析：

当我们要访问一个对象的成员时，应该是这样操作：

class Foo(object):
 
    def __init__(self):
        self.name = 'alex'
 
    def func(self):
        return 'func'
 
obj = Foo()
 
# 访问字段
obj.name
# 执行方法
obj.func()
   
那么问题来了？
a、上述访问对象成员的 name 和 func 是什么？ 
答：是变量名
b、obj.xxx 是什么意思？ 
答：obj.xxx 表示去obj中或类中寻找变量名 xxx，并获取对应内存地址中的内容。
c、需求：请使用其他方式获取obj对象中的name变量指向内存中的值 “alex”

class Foo(object):
 
    def __init__(self):
        self.name = 'alex'
 
# 不允许使用 obj.name
obj = Foo()


答：有两种方式，如下：

class Foo(object):

    def __init__(self):
        self.name = 'alex'

    def func(self):
        return 'func'

# 不允许使用 obj.name
obj = Foo()

print obj.__dict__['name']

class Foo(object):

    def __init__(self):
        self.name = 'alex'

    def func(self):
        return 'func'

# 不允许使用 obj.name
obj = Foo()

print getattr(obj, 'name')

d、比较三种访问方式

    obj.name
    obj.__dict__['name']
    getattr(obj, 'name')

答：第一种和其他种比，...
      第二种和第三种比，...

#!/usr/bin/env python
#coding:utf-8
from wsgiref.simple_server import make_server

class Handler(object):

    def index(self):
        return 'index'

    def news(self):
        return 'news'


def RunServer(environ, start_response):
    start_response('200 OK', [('Content-Type', 'text/html')])
    url = environ['PATH_INFO']
    temp = url.split('/')[1]
    obj = Handler()
    is_exist = hasattr(obj, temp)
    if is_exist:
        func = getattr(obj, temp)
        ret = func()
        return ret
    else:
        return '404 not found'

if __name__ == '__main__':
    httpd = make_server('', 8001, RunServer)
    print "Serving HTTP on port 8000..."
    httpd.serve_forever()
```

结论：反射是通过字符串的形式操作对象相关的成员。一切事物都是对象！！！

```python
#!/usr/bin/env python
# -*- coding:utf-8 -*-

import sys


def s1():
    print 's1'


def s2():
    print 's2'


this_module = sys.modules[__name__]

hasattr(this_module, 's1')
getattr(this_module, 's2')
```

### 类也是对象

```python
class Foo(object):
 
    staticField = "old boy"
 
    def __init__(self):
        self.name = 'wupeiqi'
 
    def func(self):
        return 'func'
 
    @staticmethod
    def bar():
        return 'bar'
 
print getattr(Foo, 'staticField')
print getattr(Foo, 'func')
print getattr(Foo, 'bar')
```

### 模块也是对象

```python
#!/usr/bin/env python
# -*- coding:utf-8 -*-

def dev():
    return 'dev'


#!/usr/bin/env python
# -*- coding:utf-8 -*-
 
"""
程序目录：
    home.py
    index.py
 
当前文件：
    index.py
"""
 
 
import home as obj
 
#obj.dev()
 
func = getattr(obj, 'dev')
func()
```

## 五、设计模式

### 5.1、单例模式

单例，顾名思义单个实例。

```python
学习单例之前，首先来回顾下面向对象的内容：

python的面向对象由两个非常重要的两个“东西”组成：类、实例

面向对象场景一：

如：创建三个游戏人物，分别是：

    苍井井，女，18，初始战斗力1000
    东尼木木，男，20，初始战斗力1800
    波多多，女，19，初始战斗力2500


# #####################  定义类  #####################
class Person:

    def __init__(self, na, gen, age, fig):
        self.name = na
        self.gender = gen
        self.age = age
        self.fight =fig

    def grassland(self):
        """注释：草丛战斗，消耗200战斗力"""

        self.fight = self.fight - 200

# #####################  创建实例  #####################

cang = Person('苍井井', '女', 18, 1000)    # 创建苍井井角色
dong = Person('东尼木木', '男', 20, 1800)  # 创建东尼木木角色
bo = Person('波多多', '女', 19, 2500)      # 创建波多多角色


面向对象场景二：

如：创建对数据库操作的公共类

    增
    删
    改
    查

# #### 定义类 ####

class DbHelper(object):

    def __init__(self):
        self.hostname = '1.1.1.1'
        self.port = 3306
        self.password = 'pwd'
        self.username = 'root'

    def fetch(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        pass

    def create(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        pass

    def remove(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        pass

    def modify(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        pass

# #### 操作类 ####

db = DbHelper()
db.create()
```

实例：结合场景二实现Web应用程序

```python
#!/usr/bin/env python
#coding:utf-8
from wsgiref.simple_server import make_server


class DbHelper(object):

    def __init__(self):
        self.hostname = '1.1.1.1'
        self.port = 3306
        self.password = 'pwd'
        self.username = 'root'

    def fetch(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        return 'fetch'

    def create(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        return 'create'

    def remove(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        return 'remove'

    def modify(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        return 'modify'


class Handler(object):

    def index(self):
        # 创建对象
        db = DbHelper()
        db.fetch()
        return 'index'

    def news(self):
        return 'news'


def RunServer(environ, start_response):
    start_response('200 OK', [('Content-Type', 'text/html')])
    url = environ['PATH_INFO']
    temp = url.split('/')[1]
    obj = Handler()
    is_exist = hasattr(obj, temp)
    if is_exist:
        func = getattr(obj, temp)
        ret = func()
        return ret
    else:
        return '404 not found'

if __name__ == '__main__':
    httpd = make_server('', 8001, RunServer)
    print "Serving HTTP on port 8001..."
    httpd.serve_forever()
```

对于上述实例，每个请求到来，都需要在内存里创建一个实例，再通过该实例执行指定的方法。

那么问题来了...如果并发量大的话，内存里就会存在非常多功能上一模一样的对象。存在这些对象肯定会消耗内存，对于这些功能相同的对象可以在内存中仅创建一个，需要时都去调用，也是极好的！！！

铛铛 铛铛 铛铛铛铛铛，单例模式出马，单例模式用来保证内存中仅存在一个实例！！！

通过面向对象的特性，构造出单例模式：

```python
# ########### 单例类定义 ###########
class Foo(object):
 
    __instance = None
 
    @staticmethod
    def singleton():
        if Foo.__instance:
            return Foo.__instance
        else:
            Foo.__instance = Foo()
            return Foo.__instance
 
# ########### 获取实例 ###########
obj = Foo.singleton()
```

对于Python单例模式，创建对象时不能再直接使用：obj = Foo()，而应该调用特殊的方法：obj = Foo.singleton() 。

```python
#!/usr/bin/env python
#coding:utf-8
from wsgiref.simple_server import make_server

# ########### 单例类定义 ###########
class DbHelper(object):

    __instance = None

    def __init__(self):
        self.hostname = '1.1.1.1'
        self.port = 3306
        self.password = 'pwd'
        self.username = 'root'

    @staticmethod
    def singleton():
        if DbHelper.__instance:
            return DbHelper.__instance
        else:
            DbHelper.__instance = DbHelper()
            return DbHelper.__instance

    def fetch(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        pass

    def create(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        pass

    def remove(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        pass

    def modify(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        pass


class Handler(object):

    def index(self):
        obj =  DbHelper.singleton()
        print id(single)
        obj.create()
        return 'index'

    def news(self):
        return 'news'


def RunServer(environ, start_response):
    start_response('200 OK', [('Content-Type', 'text/html')])
    url = environ['PATH_INFO']
    temp = url.split('/')[1]
    obj = Handler()
    is_exist = hasattr(obj, temp)
    if is_exist:
        func = getattr(obj, temp)
        ret = func()
        return ret
    else:
        return '404 not found'

if __name__ == '__main__':
    httpd = make_server('', 8001, RunServer)
    print "Serving HTTP on port 8001..."
    httpd.serve_forever()
```

总结：单利模式存在的目的是保证当前内存中仅存在单个实例，避免内存浪费！！！




















# [Python-面向对象（一）-Day7](https://www.cnblogs.com/lincappu/p/8157520.html)






Day7-面向对象基础 1
一、isinstance(obj, cls) 1
二、issubclass(sub, super) 1
三、异常处理 1
1、异常基础 1
2、异常种类 2
3、异常其他结构 5
4、主动触发异常 5
5、自定义异常 5
6、断言 6
四、反射 6
类也是对象 12
模块也是对象 12
五、设计模式 13
5.1、单例模式 13

# Day7-面向对象基础

## 一、isinstance(obj, cls)

检查是否obj是否是类 cls 的对象

```
class Foo(object):
    pass
 
obj = Foo()
 
isinstance(obj, Foo)
```

## 二、issubclass(sub, super)

检查sub类是否是 super 类的派生类

```python
class Foo(object):
    pass
 
class Bar(Foo):
    pass
 
issubclass(Bar, Foo)
```

## 三、异常处理

### 1、异常基础

在编程过程中为了增加友好性，在程序出现bug时一般不会将错误信息显示给用户，而是现实一个提示的页面，通俗来说就是不让用户看见大黄页！！！

```python
try:
    pass
except Exception,ex:
    pass
```

需求：将用户输入的两个数字相加

```python
while True:
    num1 = raw_input('num1:')
    num2 = raw_input('num2:')
    try:
        num1 = int(num1)
        num2 = int(num2)
        result = num1 + num2
    except Exception, e:
        print '出现异常，信息如下：'
        print e
```

### 2、异常种类

python中的异常种类非常多，每个异常专门用于处理某一项异常！！！

```python
AttributeError 试图访问一个对象没有的树形，比如foo.x，但是foo没有属性x
IOError 输入/输出异常；基本上是无法打开文件
ImportError 无法引入模块或包；基本上是路径问题或名称错误
IndentationError 语法错误（的子类） ；代码没有正确对齐
IndexError 下标索引超出序列边界，比如当x只有三个元素，却试图访问x[5]
KeyError 试图访问字典里不存在的键
KeyboardInterrupt Ctrl+C被按下
NameError 使用一个还未被赋予对象的变量
SyntaxError Python代码非法，代码不能编译(个人认为这是语法错误，写错了）
TypeError 传入对象类型与要求的不符合
UnboundLocalError 试图访问一个还未被设置的局部变量，基本上是由于另有一个同名的全局变量，
导致你以为正在访问它
ValueError 传入一个调用者不期望的值，即使值的类型是正确的


ArithmeticError
AssertionError
AttributeError
BaseException
BufferError
BytesWarning
DeprecationWarning
EnvironmentError
EOFError
Exception
FloatingPointError
FutureWarning
GeneratorExit
ImportError
ImportWarning
IndentationError
IndexError
IOError
KeyboardInterrupt
KeyError
LookupError
MemoryError
NameError
NotImplementedError
OSError
OverflowError
PendingDeprecationWarning
ReferenceError
RuntimeError
RuntimeWarning
StandardError
StopIteration
SyntaxError
SyntaxWarning
SystemError
SystemExit
TabError
TypeError
UnboundLocalError
UnicodeDecodeError
UnicodeEncodeError
UnicodeError
UnicodeTranslateError
UnicodeWarning
UserWarning
ValueError
Warning
ZeroDivisionError


dic = ["wupeiqi", 'alex']
try:
    dic[10]
except IndexError, e:
    print e

dic = {'k1':'v1'}
try:
    dic['k20']
except KeyError, e:
    print e

s1 = 'hello'
try:
    int(s1)
except ValueError, e:
    print e
```

对于上述实例，异常类只能用来处理指定的异常情况，如果非指定异常则无法处理。

```python
#未捕获到异常，程序直接报错
 
s1 = 'hello'
try:
    int(s1)
except IndexError,e:
    print e
```

所以，写程序时需要考虑到try代码块中可能出现的任意异常，可以这样写：

```python
s1 = 'hello'
try:
    int(s1)
except IndexError,e:
    print e
except KeyError,e:
    print e
except ValueError,e:
    print e
```

万能异常 在python的异常中，有一个万能异常：Exception，他可以捕获任意异常，即：

```python
s1 = 'hello'
try:
    int(s1)
except Exception,e:
    print e
```

接下来你可能要问了，既然有这个万能异常，其他异常是不是就可以忽略了！

答：当然不是，对于特殊处理或提醒的异常需要先定义，最后定义Exception来确保程序正常运行。

```python
s1 = 'hello'
try:
    int(s1)
except KeyError,e:
    print '键错误'
except IndexError,e:
    print '索引错误'
except Exception, e:
    print '错误'
```

### 3、异常其他结构

```python
try:
    # 主代码块
    pass
except KeyError,e:
    # 异常时，执行该块
    pass
else:
    # 主代码块执行完，执行该块
    pass
finally:
    # 无论异常与否，最终执行该块
    pass
```

### 4、主动触发异常

```python
try:
    raise Exception('错误了。。。')
except Exception,e:
    print e
```

### 5、自定义异常

```python
class WupeiqiException(Exception):
 
    def __init__(self, msg):
        self.message = msg
 
    def __str__(self):
        return self.message
 
try:
    raise WupeiqiException('我的异常')
except WupeiqiException,e:
    print e
```

### 6、断言

```
# assert 条件
 
assert 1 == 1
 
assert 1 == 2
```

## 四、反射

python中的反射功能是由以下四个内置函数提供：hasattr、getattr、setattr、delattr，改四个函数分别用于对对象内部执行：检查是否含有某成员、获取成员、设置成员、删除成员。
![](https://images2015.cnblogs.com/blog/425762/201509/425762-20150907094454965-329821364.jpg)

```python
class Foo(object):
 
    def __init__(self):
        self.name = 'wupeiqi'
 
    def func(self):
        return 'func'
 
obj = Foo()
 
# #### 检查是否含有成员 ####
hasattr(obj, 'name')
hasattr(obj, 'func')
 
# #### 获取成员 ####
getattr(obj, 'name')
getattr(obj, 'func')
 
# #### 设置成员 ####
setattr(obj, 'age', 18)
setattr(obj, 'show', lambda num: num + 1)
 
# #### 删除成员 ####
delattr(obj, 'name')
delattr(obj, 'func')

详细解析：

当我们要访问一个对象的成员时，应该是这样操作：

class Foo(object):
 
    def __init__(self):
        self.name = 'alex'
 
    def func(self):
        return 'func'
 
obj = Foo()
 
# 访问字段
obj.name
# 执行方法
obj.func()
   
那么问题来了？
a、上述访问对象成员的 name 和 func 是什么？ 
答：是变量名
b、obj.xxx 是什么意思？ 
答：obj.xxx 表示去obj中或类中寻找变量名 xxx，并获取对应内存地址中的内容。
c、需求：请使用其他方式获取obj对象中的name变量指向内存中的值 “alex”

class Foo(object):
 
    def __init__(self):
        self.name = 'alex'
 
# 不允许使用 obj.name
obj = Foo()


答：有两种方式，如下：

class Foo(object):

    def __init__(self):
        self.name = 'alex'

    def func(self):
        return 'func'

# 不允许使用 obj.name
obj = Foo()

print obj.__dict__['name']

class Foo(object):

    def __init__(self):
        self.name = 'alex'

    def func(self):
        return 'func'

# 不允许使用 obj.name
obj = Foo()

print getattr(obj, 'name')

d、比较三种访问方式

    obj.name
    obj.__dict__['name']
    getattr(obj, 'name')

答：第一种和其他种比，...
      第二种和第三种比，...

#!/usr/bin/env python
#coding:utf-8
from wsgiref.simple_server import make_server

class Handler(object):

    def index(self):
        return 'index'

    def news(self):
        return 'news'


def RunServer(environ, start_response):
    start_response('200 OK', [('Content-Type', 'text/html')])
    url = environ['PATH_INFO']
    temp = url.split('/')[1]
    obj = Handler()
    is_exist = hasattr(obj, temp)
    if is_exist:
        func = getattr(obj, temp)
        ret = func()
        return ret
    else:
        return '404 not found'

if __name__ == '__main__':
    httpd = make_server('', 8001, RunServer)
    print "Serving HTTP on port 8000..."
    httpd.serve_forever()
```

结论：反射是通过字符串的形式操作对象相关的成员。一切事物都是对象！！！

```python
#!/usr/bin/env python
# -*- coding:utf-8 -*-

import sys


def s1():
    print 's1'


def s2():
    print 's2'


this_module = sys.modules[__name__]

hasattr(this_module, 's1')
getattr(this_module, 's2')
```

### 类也是对象

```python
class Foo(object):
 
    staticField = "old boy"
 
    def __init__(self):
        self.name = 'wupeiqi'
 
    def func(self):
        return 'func'
 
    @staticmethod
    def bar():
        return 'bar'
 
print getattr(Foo, 'staticField')
print getattr(Foo, 'func')
print getattr(Foo, 'bar')
```

### 模块也是对象

```python
#!/usr/bin/env python
# -*- coding:utf-8 -*-

def dev():
    return 'dev'


#!/usr/bin/env python
# -*- coding:utf-8 -*-
 
"""
程序目录：
    home.py
    index.py
 
当前文件：
    index.py
"""
 
 
import home as obj
 
#obj.dev()
 
func = getattr(obj, 'dev')
func()
```

## 五、设计模式

### 5.1、单例模式

单例，顾名思义单个实例。

```python
学习单例之前，首先来回顾下面向对象的内容：

python的面向对象由两个非常重要的两个“东西”组成：类、实例

面向对象场景一：

如：创建三个游戏人物，分别是：

    苍井井，女，18，初始战斗力1000
    东尼木木，男，20，初始战斗力1800
    波多多，女，19，初始战斗力2500


# #####################  定义类  #####################
class Person:

    def __init__(self, na, gen, age, fig):
        self.name = na
        self.gender = gen
        self.age = age
        self.fight =fig

    def grassland(self):
        """注释：草丛战斗，消耗200战斗力"""

        self.fight = self.fight - 200

# #####################  创建实例  #####################

cang = Person('苍井井', '女', 18, 1000)    # 创建苍井井角色
dong = Person('东尼木木', '男', 20, 1800)  # 创建东尼木木角色
bo = Person('波多多', '女', 19, 2500)      # 创建波多多角色


面向对象场景二：

如：创建对数据库操作的公共类

    增
    删
    改
    查

# #### 定义类 ####

class DbHelper(object):

    def __init__(self):
        self.hostname = '1.1.1.1'
        self.port = 3306
        self.password = 'pwd'
        self.username = 'root'

    def fetch(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        pass

    def create(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        pass

    def remove(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        pass

    def modify(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        pass

# #### 操作类 ####

db = DbHelper()
db.create()
```

实例：结合场景二实现Web应用程序

```python
#!/usr/bin/env python
#coding:utf-8
from wsgiref.simple_server import make_server


class DbHelper(object):

    def __init__(self):
        self.hostname = '1.1.1.1'
        self.port = 3306
        self.password = 'pwd'
        self.username = 'root'

    def fetch(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        return 'fetch'

    def create(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        return 'create'

    def remove(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        return 'remove'

    def modify(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        return 'modify'


class Handler(object):

    def index(self):
        # 创建对象
        db = DbHelper()
        db.fetch()
        return 'index'

    def news(self):
        return 'news'


def RunServer(environ, start_response):
    start_response('200 OK', [('Content-Type', 'text/html')])
    url = environ['PATH_INFO']
    temp = url.split('/')[1]
    obj = Handler()
    is_exist = hasattr(obj, temp)
    if is_exist:
        func = getattr(obj, temp)
        ret = func()
        return ret
    else:
        return '404 not found'

if __name__ == '__main__':
    httpd = make_server('', 8001, RunServer)
    print "Serving HTTP on port 8001..."
    httpd.serve_forever()
```

对于上述实例，每个请求到来，都需要在内存里创建一个实例，再通过该实例执行指定的方法。

那么问题来了...如果并发量大的话，内存里就会存在非常多功能上一模一样的对象。存在这些对象肯定会消耗内存，对于这些功能相同的对象可以在内存中仅创建一个，需要时都去调用，也是极好的！！！

铛铛 铛铛 铛铛铛铛铛，单例模式出马，单例模式用来保证内存中仅存在一个实例！！！

通过面向对象的特性，构造出单例模式：

```python
# ########### 单例类定义 ###########
class Foo(object):
 
    __instance = None
 
    @staticmethod
    def singleton():
        if Foo.__instance:
            return Foo.__instance
        else:
            Foo.__instance = Foo()
            return Foo.__instance
 
# ########### 获取实例 ###########
obj = Foo.singleton()
```

对于Python单例模式，创建对象时不能再直接使用：obj = Foo()，而应该调用特殊的方法：obj = Foo.singleton() 。

```python
#!/usr/bin/env python
#coding:utf-8
from wsgiref.simple_server import make_server

# ########### 单例类定义 ###########
class DbHelper(object):

    __instance = None

    def __init__(self):
        self.hostname = '1.1.1.1'
        self.port = 3306
        self.password = 'pwd'
        self.username = 'root'

    @staticmethod
    def singleton():
        if DbHelper.__instance:
            return DbHelper.__instance
        else:
            DbHelper.__instance = DbHelper()
            return DbHelper.__instance

    def fetch(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        pass

    def create(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        pass

    def remove(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        pass

    def modify(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        pass


class Handler(object):

    def index(self):
        obj =  DbHelper.singleton()
        print id(single)
        obj.create()
        return 'index'

    def news(self):
        return 'news'


def RunServer(environ, start_response):
    start_response('200 OK', [('Content-Type', 'text/html')])
    url = environ['PATH_INFO']
    temp = url.split('/')[1]
    obj = Handler()
    is_exist = hasattr(obj, temp)
    if is_exist:
        func = getattr(obj, temp)
        ret = func()
        return ret
    else:
        return '404 not found'

if __name__ == '__main__':
    httpd = make_server('', 8001, RunServer)
    print "Serving HTTP on port 8001..."
    httpd.serve_forever()
```

总结：单利模式存在的目的是保证当前内存中仅存在单个实例，避免内存浪费！！！




















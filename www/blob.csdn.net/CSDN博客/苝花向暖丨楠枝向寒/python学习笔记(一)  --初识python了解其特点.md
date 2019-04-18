# python学习笔记(一)   --初识python了解其特点 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年08月04日 00:14:27[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：59标签：[python																[java																[基础知识](https://so.csdn.net/so/search/s.do?q=基础知识&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[python基础](https://blog.csdn.net/weixin_40247263/article/category/7905597)

所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)


# python特点: 

## 一：脚本解释性语言

脚本解释性语言的内部机制

在运行脚本之前，得到结果之后

1.python先将脚本编译成字节码文件(pyc,pyo)

2.python虚拟机解释并运行字节码文件

编译型语言的内部机制(C语言)

1.先将源代码编译成机器码(机器能读懂的代码)，生成可执行文件

2.运行可执行文件

编译型语言要比脚本解释性语言快的多

## 二:动态语义

python 无需声明变量类型 直接赋值即可   例如  str = "abc" ，不用像java中 String str = "abc"

但是一个变量名 一旦 引用了一个对象，它的类型就被定下来了。

例如  ：

```python
a = "123"

a = a+1

#这种情况是不允许的

#我们必须用 int(变量名)  来进行强制类型转换

a = int(a) + 1
```

## 三:缩进

```python
a = 5
if a == 5:
    print(a)
```

python的if下面一行必须缩进，并且缩进必须是4个空格，大多数编译器按tab键默认都是缩进4格，但有特殊的。

python的if 后面不用跟小括号也不用跟大括号，而是跟一个冒号，python一行结束也不用跟分号。

# python的基本编写方法

## 一：如果.py文件中包含中文，必须在第一行加上编码格式

```python
#coding=utf-8
```

否则运行时会提示语法错误

这个编码格式并不规定必须这么写，只要符合下面的正则即可

```python
coding[:=]\s*([-\w.]+)
#首先 前面与后面是什么都无所谓，中间部分必须包含 coding紧跟着冒号或者等号
#然后接着几个空格都可以,再#跟上编码格式
#如下面这种也可以
#-*- coding: utf-8 -*-
```

## 二:单行注释以 # 开头，#开头这一行全都不会执行 类似于java中的 //

```python
#这是一个注释
```

## 三:多行注释 用 两对  三个双引号 包裹住 类似于 java中的  /*  */

```python
"""
这
是
一
个
多
行
注
释
"""
```

## 四:方法的声明以及调用

stand.py

```python
# coding=utf8

""" 这是一个标准模块脚本的写作范式,此处为该脚本文档 """

# 这里给下面语句做一个注释
new_str = "这是一个全局变量"

def hello():    # 下面声明一个方法
    """
    这是
    方法
    体内部
    """
    return "hello world"

if __name__ == "__main__":  # 程序主体 相当于 java中的main函数
    print(hello())
```

hello.py

```python
# -*- coding: utf-8 -*-

# 引入类其他类 from 文件夹名 import 类名，被improt的文件会被编译成.pyc文件
from study import stand

# 类名.__doc__ 获取文档注释
print(stand.__doc__)
# 类名.变量名获取属性
print(stand.new_str)
# 类名.函数名 调用方法
print(stand.hello())

a = 5
b = 6
c = 7
"""
如果if条件语句特别长,为了方便阅读我们需要换行
同时可以在每一行结尾加上反斜杠
也可以在 a == b 两侧加上 小括号
"""
if (a == b) and (a == c) and (b == c) \
    and (a == b) and (a == c) and (b == c) \
        and (a == b) and (a == c) and (b == c):
    print('hello')
```

输出

```
这是一个标准模块脚本的写作范式,此处为该脚本文档 
这是一个全局变量
hello world
```

# 对象、变量与赋值

## 一：一切数据都是对象

例如 java中的 整型、浮点型这些基本数据类型，在python中都被称为对象，与String、List、Set、Map 等等一样

## 二：赋值

```python
d = "4"
```

过程描述：在内存中开辟一块空间来存放 "4" 这个字符串,然后把地址 赋值给了 d。这一点与java中是一样的，并且python中的字符串也是不可被更改的，如：

```python
d = "3"
```

此时并不是把 "4" 变成 了 "3"，而是新开辟了一个空间来存放 "3" 然后将 d指向 "3"，此时"4"没有变量指向就会被回收，否则会占用内存。

我们再说另外一种情况,假设此时 变量 d 还是指向 "4" ，我们此时 再让 变量 e 也指向 "4" ,如下

```python
e = "4"
```

此时 e和d 同时 指向 "4" ，注意在内存中只有一个 "4" ,这里有点像java中的常量池机制。

我们可以通过sys.getrefcount()方法来查看 某个对象 有几个变量指向它(初始值为3,我的python虚拟机版本是3.7.150.1013)

我看网上很多人说 初始值是 2 ,即有一个对象引用的时候返回的是3,不知道是不是因为版本不同的原因

如下

```python
d = "testhahahaha"
print(sys.getrefcount("testhahahaha"))
e = "testhahahaha"
print(sys.getrefcount("testhahahaha"))
f = "testhahahaha"
print(sys.getrefcount("testhahahaha"))
d = 1
print(sys.getrefcount("testhahahaha"))
e = 2
print(sys.getrefcount("testhahahaha"))
f = 3
print(sys.getrefcount("testhahahaha"))
```

显示

```
4
5
6
5
4
3
```

python中支持多重赋值 如下

```python
a, b, c = "1", "2", "3"    # python支持多重赋值
k = ("1", "2", "3")     # 此行与上一行 其实是等价的
print(a)
print(b)
print(c)
print(a, b, c)
print(k)
```

显示

```
1
2
3
1 2 3
('1', '2', '3')
```

删除 变量 对 对象的 指向 用 del

```python
del a
print(a)
```

报错
`NameError: name 'a' is not defined`
联合 删除

```python
del a, b, c
print(b)
```

报错
`NameError: name 'b' is not defined`
## 三：常用内置函数

### 1:type() 查看数据类型

```python
a = 1
b = "1"
c = (1, 2)
d = [1, 2]
print(type(a))
print(type(b))
print(type(c))
print(type(d))
```

显示

```
<class 'int'>
<class 'str'>
<class 'tuple'>
<class 'list'>
```

### 2:help(类名) 查看某个类库下所有的方法描述 

###    help(类名.函数名) 查看某个类下的某个方法的描述

```python
import sys

help(sys)
```

### 3: dir(类名) 查看某个类库下的所有方法

```python
print(dir(sys))
```

```
['__breakpointhook__', '__displayhook__', '__doc__', '__excepthook__', '__interactivehook__', '__loader__', '__name__', '__package__', '__spec__', '__stderr__', '__stdin__', '__stdout__', '_clear_type_cache', '_current_frames', '_debugmallocstats', '_enablelegacywindowsfsencoding', '_framework', '_getframe', '_git', '_home', '_xoptions', 'api_version', 'argv', 'base_exec_prefix', 'base_prefix', 'breakpointhook', 'builtin_module_names', 'byteorder', 'call_tracing', 'callstats', 'copyright', 'displayhook', 'dllhandle', 'dont_write_bytecode', 'exc_info', 'excepthook', 'exec_prefix', 'executable', 'exit', 'flags', 'float_info', 'float_repr_style', 'get_asyncgen_hooks', 'get_coroutine_origin_tracking_depth', 'get_coroutine_wrapper', 'getallocatedblocks', 'getcheckinterval', 'getdefaultencoding', 'getfilesystemencodeerrors', 'getfilesystemencoding', 'getprofile', 'getrecursionlimit', 'getrefcount', 'getsizeof', 'getswitchinterval', 'gettrace', 'getwindowsversion', 'hash_info', 'hexversion', 'implementation', 'int_info', 'intern', 'is_finalizing', 'maxsize', 'maxunicode', 'meta_path', 'modules', 'path', 'path_hooks', 'path_importer_cache', 'platform', 'prefix', 'set_asyncgen_hooks', 'set_coroutine_origin_tracking_depth', 'set_coroutine_wrapper', 'setcheckinterval', 'setprofile', 'setrecursionlimit', 'setswitchinterval', 'settrace', 'stderr', 'stdin', 'stdout', 'thread_info', 'version', 'version_info', 'warnoptions', 'winver']
```

## 4：id(变量名) 查看对象地址

```
>>> a = '123'
>>> id(a)
139645340191560
```



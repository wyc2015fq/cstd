# Python 3 学习笔记（3） - zwvista - 博客园

## [Python 3 学习笔记（3）](https://www.cnblogs.com/zwvista/p/9504055.html)

### 模块

编写模块

```
# fibo.py
# Fibonacci numbers module

def fib(n):    # write Fibonacci series up to n
    a, b = 0, 1
    while a < n:
        print(a, end=' ')
        a, b = b, a+b
    print()

def fib2(n):   # return Fibonacci series up to n
    result = []
    a, b = 0, 1
    while a < n:
        result.append(a)
        a, b = b, a+b
    return result
```

改变解释器的工作目录
[How to know/change current directory in Python shell?](https://stackoverflow.com/questions/8248397/how-to-know-change-current-directory-in-python-shell)

```
>>> import os
>>> os.getcwd()
'...'
>>> os.chdir("...")
```

导入（import） fibo 模块

```
>>> import fibo
>>> fibo.fib(1000)
0 1 1 2 3 5 8 13 21 34 55 89 144 233 377 610 987 
>>> fibo.fib2(100)
[0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89]
>>> from fibo import fib, fib2
>>> fib(500)
0 1 1 2 3 5 8 13 21 34 55 89 144 233 377 
>>> from fibo import *
>>> fib(500)
0 1 1 2 3 5 8 13 21 34 55 89 144 233 377 
>>> import fibo as fib
>>> fib.fib(500)
0 1 1 2 3 5 8 13 21 34 55 89 144 233 377 
>>> from fibo import fib as fibonacci
>>> fibonacci(500)
0 1 1 2 3 5 8 13 21 34 55 89 144 233 377
```
- 模块中可包含可执行语句，这些可执行语句只有在模块初次导入时才执行。
- 模块可导入其他模块。使用 modname.itemname 调用其他模块的函数。
- 可以将其他模块中的函数导入到本模块中。
- 使用通配符 * 时模块名不导入，以下划线打头的名字不导入。
- 模块名与其中的名字在导入时均可改名。

重新导入模块
[How do I unload (reload) a Python module?](https://stackoverflow.com/questions/437589/how-do-i-unload-reload-a-python-module)

```
>>> import foo
>>> import importlib
>>> importlib.reload(foo)
```

### 标准模块

```
>>> import fibo,sys
>>> dir(fibo)
['__builtins__', '__cached__', '__doc__', '__file__', '__loader__', '__name__', '__package__', '__spec__', 'fib', 'fib2']
>>> a = [1, 2, 3, 4, 5]
>>> dir()
['__annotations__', '__builtins__', '__doc__', '__loader__', '__name__', '__package__', '__spec__', 'a', 'fibo', 'os', 'sys']
```

### 异常

```
>>> # 使用 try ... except 处理异常
>>> while True:
    try:
        x = int(input("Please enter a number: "))
        break
    except ValueError:
        print("Oops!  That was no valid number.  Try again...")

Please enter a number: a
Oops!  That was no valid number.  Try again...
Please enter a number: 3
>>> # 异常类的继承
>>> class B(Exception):
    pass

>>> class C(B):
    pass

>>> class D(C):
    pass

>>> for cls in [B, C, D]:
    try:
        raise cls()
    except D:
        print("D")
    except C:
        print("C")
    except B:
        print("B")

B
C
D
>>> # 异常类的附带信息
>>> try:
    raise Exception('spam', 'eggs')
except Exception as inst:
    print(type(inst))    # the exception instance
    print(inst.args)     # arguments stored in .args
    print(inst)          # __str__ allows args to be printed directly,
                         # but may be overridden in exception subclasses
    x, y = inst.args     # unpack args
    print('x =', x)
    print('y =', y)

<class 'Exception'>
('spam', 'eggs')
('spam', 'eggs')
x = spam
y = eggs
>>> # else 和 finally
>>> def divide(x, y):
    try:
        result = x / y
    except ZeroDivisionError:
        print("division by zero!")
    else:
        print("result is", result)
    finally:
        print("executing finally clause")

>>> divide(2, 1)
result is 2.0
executing finally clause
>>> divide(2, 0)
division by zero!
executing finally clause
>>> divide("2", "1")
executing finally clause
Traceback (most recent call last):
  File "<pyshell#24>", line 1, in <module>
    divide("2", "1")
  File "<pyshell#21>", line 3, in divide
    result = x / y
TypeError: unsupported operand type(s) for /: 'str' and 'str'
```



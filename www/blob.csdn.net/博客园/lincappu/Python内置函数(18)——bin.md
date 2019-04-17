# Python内置函数(18)——bin - lincappu - 博客园







# [Python内置函数(18)——bin](https://www.cnblogs.com/lincappu/p/8144773.html)





**英文文档：**

`bin`(*x*)

    Convert an integer number to a binary string. The result is a valid Python expression. If *x* is not a Python [`int`](mk:@MSITStore:D:\Python\Python35-32\Doc\Python352.chm::/library/functions.html#int) object, it has to define an `__index__()` method that returns an integer.

**　　将整数转换为2进制格式字符串**

**说明：**

    1. 将一个整形数字转换成二进制字符串

```
>>> b = bin(3) 
>>> b
'0b11'
>>> type(b) #获取b的类型
<class 'str'>
```

    2. 如果参数x不是一个整数，则x必须定义一个 __index__() 方法，并且方法返回值必须是整数。

    2.1 如果对象不是整数，则报错

```
>>> class A:
    pass

>>> a = A()
>>> bin(a) 
Traceback (most recent call last):
  File "<pyshell#15>", line 1, in <module>
    bin(a)
TypeError: 'A' object cannot be interpreted as an integer
```

    2.2 如果对象定义了__index__方法，但返回值不是整数，报错

```
>>> class B:
    def __index__(self):
        return "3"

>>> b = B()
>>> bin(b)
Traceback (most recent call last):
  File "<pyshell#21>", line 1, in <module>
    bin(b)
TypeError: __index__ returned non-int (type str)
```

    2.3 对象定义了__index__方法，且返回值是整数，将__index__方法返回值转换成二进制字符串

```
>>> class C:
    def __index__(self):
        return 3

>>> c = C()
>>> bin(c)
'0b11'
```













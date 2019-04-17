# Python内置函数(20)——hex - lincappu - 博客园







# [Python内置函数(20)——hex](https://www.cnblogs.com/lincappu/p/8144785.html)





**英文文档：**

`hex`(*x*)

Convert an integer number to a lowercase hexadecimal string prefixed with “0x”, for example

If x is not a Python `int` object, it has to define an __index__() method that returns an integer.

　　将整数转换为16进制的字符串 

**说明：**

　　1. 函数功能将10进制整数转换成16进制整数。

```
>>> hex(15)
'0xf'
>>> hex(16)
'0x10'
```

　　2. 如果参数x不是整数，则它必须定义一个返回整数的__index__函数。

```
# 未定义__index__函数
>>> class Student:
    def __init__(self,name,age):
        self.name = name
        self.age = age

>>> 
>>> s = Student('Kim',10)
>>> hex(s)
Traceback (most recent call last):
  File "<pyshell#17>", line 1, in <module>
    hex(s)
TypeError: 'Student' object cannot be interpreted as an integer

# 定义__index__函数，但是返回字符串
>>> class Student:
    def __init__(self,name,age):
        self.name = name
        self.age = age
    def __index__(self):
        return self.name

>>> s = Student('Kim',10)
>>> hex(s)
Traceback (most recent call last):
  File "<pyshell#23>", line 1, in <module>
    hex(s)
TypeError: __index__ returned non-int (type str)

# 定义__index__函数，并返回整数
>>> class Student:
    def __init__(self,name,age):
        self.name = name
        self.age = age
    def __index__(self):
        return self.age

>>> s = Student('Kim',10)
>>> hex(s)
'0xa'
```













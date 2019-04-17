# Python内置函数(19)——oct - lincappu - 博客园







# [Python内置函数(19)——oct](https://www.cnblogs.com/lincappu/p/8144780.html)





**英文文档：**
- `oct`(*x*)
- 
- Convert an integer number to an octal string. The result is a valid Python expression. If *x* is not a Python [`int`](mk:@MSITStore:D:\Python\Python35-32\Doc\Python352.chm::/library/functions.html#int) object, it has to define an `__index__()` method that returns an integer.
- 
- 
- 
- 
- 

 　　将整数转换为8进制的字符串
- **说明：**
- 
- 　　1. 函数功能将一个整数转换成8进制字符串。如果传入浮点数或者字符串均会报错。

```
>>> a = oct(10)

>>> a
'0o12'
>>> type(a) # 返回结果类型是字符串
<class 'str'>

>>> oct(10.0) # 浮点数不能转换成8进制
Traceback (most recent call last):
  File "<pyshell#3>", line 1, in <module>
    oct(10.0)
TypeError: 'float' object cannot be interpreted as an integer

>>> oct('10') # 字符串不能转换成8进制
Traceback (most recent call last):
  File "<pyshell#4>", line 1, in <module>
    oct('10')
TypeError: 'str' object cannot be interpreted as an integer
```

　　2. 如果传入参数不是整数，则其必须是一个定义了__index__并返回整数函数的类的实例对象。

```
# 未定义__index__函数，不能转换
>>> class Student:
    def __init__(self,name,age):
        self.name = name
        self.age = age

        
>>> a = Student('Kim',10)
>>> oct(a)
Traceback (most recent call last):
  File "<pyshell#12>", line 1, in <module>
    oct(a)
TypeError: 'Student' object cannot be interpreted as an integer

# 定义了__index__函数，但是返回值不是int类型，不能转换
>>> class Student:
    def __init__(self,name,age):
        self.name = name
        self.age = age
    def __index__(self):
        return self.name

>>> a = Student('Kim',10)
>>> oct(a)
Traceback (most recent call last):
  File "<pyshell#18>", line 1, in <module>
    oct(a)
TypeError: __index__ returned non-int (type str)

# 定义了__index__函数，而且返回值是int类型，能转换
>>> class Student:
    def __init__(self,name,age):
        self.name = name
        self.age = age
    def __index__(self):
        return self.age

>>> a = Student('Kim',10)
>>> oct(a)
'0o12'
```













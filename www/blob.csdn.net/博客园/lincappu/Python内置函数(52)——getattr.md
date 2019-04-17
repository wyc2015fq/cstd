# Python内置函数(52)——getattr - lincappu - 博客园







# [Python内置函数(52)——getattr](https://www.cnblogs.com/lincappu/p/8145135.html)





**英文文档：**
- `getattr`(*object*, *name*[, *default*])
- 
- Return the value of the named attribute of *object*. *name* must be a string. If the string is the name of one of the object’s attributes, the result is the value of that attribute. For example, `getattr(x, 'foobar')` is equivalent to `x.foobar`. If the named attribute does not exist, *default* is returned if provided, otherwise `AttributeError` is raised.
- 
- 
- 
- 
- **　　获取对象的属性值**
- **说明：**
- 
- 
- 　　1. 函数功能是从对象object中获取名称为name的属性，等效与调用object.name。

```
#定义类Student
>>> class Student:
    def __init__(self,name):
        self.name = name

        
>>> s = Stduent('Aim')
>>> getattr(s,'name') #等效于调用s.name
'Aim'
>>> s.name
'Aim'
```

　　2. 函数第三个参数default为可选参数，如果object中含义name属性，则返回name属性的值，如果没有name属性，则返回default值，如果default未传入值，则报错。

```
#定义类Student
>>> class Student:
    def __init__(self,name):
        self.name = name

>>> getattr(s,'name') #存在属性name
'Aim'

>>> getattr(s,'age',6) #不存在属性age，但提供了默认值，返回默认值
6

>>> getattr(s,'age') #不存在属性age，未提供默认值，调用报错
Traceback (most recent call last):
  File "<pyshell#17>", line 1, in <module>
    getattr(s,'age')
AttributeError: 'Stduent' object has no attribute 'age'
```













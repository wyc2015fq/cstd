# Python内置函数(51)——hasattr - lincappu - 博客园







# [Python内置函数(51)——hasattr](https://www.cnblogs.com/lincappu/p/8145124.html)





**英文文档：**
- `hasattr`(*object*, *name*)
- 
- The arguments are an object and a string. The result is `True` if the string is the name of one of the object’s attributes, `False` if not. (This is implemented by calling `getattr(object, name)` and seeing whether it raises an `AttributeError`or not.)
- 
- 
- 
- 
- 
- **　　检查对象是否含有属性**
- **说明：**
- 
- 
- 
- 　　1. 函数功能用来检测对象object中是否含有名为name的属性，如果有则返回True，如果没有返回False

```
#定义类A
>>> class Student:
    def __init__(self,name):
        self.name = name

        
>>> s = Student('Aim')
>>> hasattr(s,'name') #a含有name属性
True
>>> hasattr(s,'age') #a不含有age属性
False
```













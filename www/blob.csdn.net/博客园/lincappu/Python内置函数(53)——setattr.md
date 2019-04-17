# Python内置函数(53)——setattr - lincappu - 博客园







# [Python内置函数(53)——setattr](https://www.cnblogs.com/lincappu/p/8145179.html)





**英文文档：**

`setattr`(*object*, *name*, *value*)

This is the counterpart of `getattr()`. The arguments are an object, a string and an arbitrary value. The string may name an existing attribute or a new attribute. The function assigns the value to the attribute, provided the object allows it. For example, `setattr(x, 'foobar', 123)` is equivalent to `x.foobar = 123`

　　设置对象的属性值

**说明：**

　　1. setattr函数和[getattr函数](http://www.cnblogs.com/sesshoumaru/p/6009598.html)是对应的。一个设置对象的属性值，一个获取对象属性值。

　　2. 函数有3个参数，功能是对参数object对象，设置名为name的属性的属性值为value值。

```
>>> class Student:
    def __init__(self,name):
        self.name = name

        
>>> a = Student('Kim')
>>> a.name
'Kim'
>>> setattr(a,'name','Bob')
>>> a.name
'Bob'
```

　　3. name属性可以是object对象的一个已经存在的属性，存在的话就会更新其属性值；如果name属性不存在，则对象将创建name名称的属性值，并存储value值。等效于调用object.name = value。

```
>>> a.age # 不存在age属性
Traceback (most recent call last):
  File "<pyshell#20>", line 1, in <module>
    a.age
AttributeError: 'Student' object has no attribute 'age'

>>> setattr(a,'age',10) # 执行后 创建 age属性
>>> a.age  # 存在age属性了
10
>>> a.age = 12 # 等效于调用object.name
>>> a.age
12
```













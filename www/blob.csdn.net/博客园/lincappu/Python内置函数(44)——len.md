# Python内置函数(44)——len - lincappu - 博客园







# [Python内置函数(44)——len](https://www.cnblogs.com/lincappu/p/8145058.html)





**英文文档：**
- `len`(*s*)
- 
- Return the length (the number of items) of an object. The argument may be a sequence (such as a string, bytes, tuple, list, or range) or a collection (such as a dictionary, set, or frozen set).
- 
- 
- 
- 
- 
- 
- **　　返回对象的长度**
- **说明：**
- 
- 　　1. 返回对象的长度，参数可以是序列(比如字符串、字节数组、元组、列表和range对象)，或者是集合(比如字典、集合、不可变集合)

```
>>> len('abcd') # 字符串
4
>>> len(bytes('abcd','utf-8')) # 字节数组
4
>>> len((1,2,3,4)) # 元组
4
>>> len([1,2,3,4]) # 列表
4
>>> len(range(1,5)) # range对象
4
>>> len({'a':1,'b':2,'c':3,'d':4}) # 字典
4
>>> len({'a','b','c','d'}) # 集合
4
>>> len(frozenset('abcd')) #不可变集合
4
```

　　2. 如果参数为其它类型，则其必须实现__len__方法，并返回整数，否则报错。

```
>>> class A:
    def __init__(self,name):
        self.name = name
    def __len__(self):
        return len(self.name)

>>> a = A('')
>>> len(a)
0
>>> a = A('Aim')
>>> len(a)
3
>>> class B:
    pass

>>> b = B()
>>> len(b)
Traceback (most recent call last):
  File "<pyshell#65>", line 1, in <module>
    len(b)
TypeError: object of type 'B' has no len()
>>> class C:
    def __len__(self):
        return 'len'

>>> c = C()
>>> len(c)
Traceback (most recent call last):
  File "<pyshell#71>", line 1, in <module>
    len(c)
TypeError: 'str' object cannot be interpreted as an integer
```













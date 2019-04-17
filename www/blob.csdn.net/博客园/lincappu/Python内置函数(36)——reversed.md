# Python内置函数(36)——reversed - lincappu - 博客园







# [Python内置函数(36)——reversed](https://www.cnblogs.com/lincappu/p/8144961.html)





**英文文档：**
- `reversed`(*seq*)
- 
- Return a reverse iterator. *seq* must be an object which has a `__reversed__()` method or supports the sequence protocol (the `__len__()` method and the `__getitem__()` method with integer arguments starting at `0`).　　

**　　反转序列生成新的可迭代对象**

**说明：**

　　1. 函数功能是反转一个序列对象，将其元素从后向前颠倒构建成一个新的迭代器。

```
>>> a = reversed(range(10)) # 传入range对象
>>> a # 类型变成迭代器
<range_iterator object at 0x035634E8>
>>> list(a)
[9, 8, 7, 6, 5, 4, 3, 2, 1, 0]

>>> a = ['a','b','c','d']
>>> a
['a', 'b', 'c', 'd']
>>> reversed(a) # 传入列表对象
<list_reverseiterator object at 0x031874D0>
>>> b = reversed(a)
>>> b # 类型变成迭代器
<list_reverseiterator object at 0x037C4EB0>
>>> list(b)
['d', 'c', 'b', 'a']
```

　　2. 如果参数不是一个序列对象，则其必须定义一个`__reversed__方法。`

```
# 类型Student没有定义__reversed__方法
>>> class Student:
    def __init__(self,name,*args):
        self.name = name
        self.scores = []
        for value in args:
            self.scores.append(value)

            
>>> a = Student('Bob',78,85,93,96)
>>> reversed(a) # 实例不能反转
Traceback (most recent call last):
  File "<pyshell#37>", line 1, in <module>
    reversed(a)
TypeError: argument to reversed() must be a sequence
>>> type(a.scores) # 列表类型
<class 'list'>


# 重新定义类型，并为其定义__reversed__方法
>>> class Student:
    def __init__(self,name,*args):
        self.name = name
        self.scores = []
        for value in args:
            self.scores.append(value)
    def __reversed__(self):
        self.scores = reversed(self.scores)

        
>>> a = Student('Bob',78,85,93,96)
>>> a.scores # 列表类型
[78, 85, 93, 96]
>>> type(a.scores)
<class 'list'>

>>> reversed(a)  # 实例变得可以反转
>>> a.scores # 反转后类型变成迭代器
<list_reverseiterator object at 0x0342F3B0>
>>> type(a.scores)
<class 'list_reverseiterator'>

>>> list(a.scores)
[96, 93, 85, 78]
```













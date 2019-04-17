# Python内置函数(28)——iter - lincappu - 博客园







# [Python内置函数(28)——iter](https://www.cnblogs.com/lincappu/p/8144842.html)





**英文文档：**

`iter`(*object*[, *sentinel*])

Return an iterator object. The first argument is interpreted very differently depending on the presence of the second argument. Without a second argument, *object* must be a collection object which supports the iteration protocol (the `__iter__()` method), or it must support the sequence protocol (the `__getitem__()` method with integer arguments starting at `0`). If it does not support either of those protocols, `TypeError` is raised. If the second argument, *sentinel*, is given, then *object* must be a callable object. The iterator created in this case will call *object* with no arguments for each call to its `__next__()` method; if the value returned is equal to *sentinel*, `StopIteration` will be raised, otherwise the value will be returned.

One useful application of the second form of `iter()` is to read lines of a file until a certain line is reached. The following example reads a file until the `readline()` method returns an empty string:

```
with open('mydata.txt') as fp:
    for line in iter(fp.readline, ''):
        process_line(line)
```

 　　根据传入的参数生成一个新的可迭代对象

**说明：**

　　1. 函数功能返回一个迭代器对象。

　　2. 当第二个参数不提供时，第一个参数必须是一个支持可迭代协议(即实现了__iter__()方法)的集合(字典、集合、不可变集合)，或者支持序列协议(即实现了__getitem__()方法，方法接收一个从0开始的整数参数)的序列(元组、列表、字符串)，否则将报错。

```
>>> a = iter({'A':1,'B':2}) #字典集合
>>> a
<dict_keyiterator object at 0x03FB8A50>
>>> next(a)
'A'
>>> next(a)
'B'
>>> next(a)
Traceback (most recent call last):
  File "<pyshell#36>", line 1, in <module>
    next(a)
StopIteration
 
>>> a = iter('abcd') #字符串序列
>>> a
<str_iterator object at 0x03FB4FB0>
>>> next(a)
'a'
>>> next(a)
'b'
>>> next(a)
'c'
>>> next(a)
'd'
>>> next(a)
Traceback (most recent call last):
  File "<pyshell#29>", line 1, in <module>
    next(a)
StopIteration
```



　　3. 当第二个参数*sentinel*提供时，第一个参数必须是一个可被调用对象。创建的迭代对象，在调用__next__方法的时候会调用这个可被调用对象，当返回值和*sentinel*值相等时，将抛出`StopIteration异常， 终止迭代。`

```
# 定义类
>>> class IterTest: 
    def __init__(self):
        self.start = 0
        self.end = 10
    def get_next_value(self):
        current = self.start
        if current < self.end:
            self.start += 1
        else:
            raise StopIteration
        return current

>>> iterTest = IterTest() #实例化类
>>> a = iter(iterTest.get_next_value,4) # iterTest.get_next_value为可调用对象，sentinel值为4
>>> a
<callable_iterator object at 0x03078D30>
>>> next(a)
0
>>> next(a)
1
>>> next(a)
2
>>> next(a)
3
>>> next(a) #迭代到4终止
Traceback (most recent call last):
  File "<pyshell#22>", line 1, in <module>
    next(a)
StopIteration
```













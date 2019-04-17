# Python内置函数(32)——all - lincappu - 博客园







# [Python内置函数(32)——all](https://www.cnblogs.com/lincappu/p/8144899.html)





**英文文档：**

`all`(*iterable*)

    Return `True` if all elements of the *iterable* are true (or if the iterable is empty). Equivalent to:

```
def all(iterable):
    for element in iterable:
        if not element:
            return False
    return True
```

 　　判断可迭代对象的每个元素都是 True 值

**说明：**

    1. 接受一个可迭代器对象为参数，当参数为空或者不为可迭代器对象是报错

```
>>> all(2) #传入数值报错
Traceback (most recent call last):
  File "<pyshell#9>", line 1, in <module>
    all(2)
TypeError: 'int' object is not iterable
```

     2. 如果可迭代对象中每个元素的逻辑值均为True时，返回True，否则返回False

```
>>> all([1,2]) #列表中每个元素逻辑值均为True，返回True
True
>>> all([0,1,2]) #列表中0的逻辑值为False，返回False
False
```

     3. 如果可迭代对象为空(元素个数为0)，返回True

```
>>> all(()) #空元组
True
>>> all({}) #空字典
True
```













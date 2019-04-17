# Python内置函数(33)——any - lincappu - 博客园







# [Python内置函数(33)——any](https://www.cnblogs.com/lincappu/p/8144909.html)





**英文文档：**

`any`(*iterable*)

    Return `True` if any element of the *iterable* is true. If the iterable is empty, return `False`. Equivalent to:

```
def any(iterable):
    for element in iterable:
        if element:
            return True
    return False
```

**　　判断可迭代对象的元素是否有 True值的元素**

**说明：**

    1. 接受一个可迭代器对象为参数，当参数为空或者不为可迭代器对象是报错

```
>>> any(2) #传入数值报错
Traceback (most recent call last):
  File "<pyshell#0>", line 1, in <module>
    any(2)
TypeError: 'int' object is not iterable
```



    2. 如果可迭代对象中其中一个元素的逻辑值为True时，返回True，全部值均为False时返回False

```
>>> any([0,1,2]) #列表元素有一个为True，则返回True
True
>>> any([0,0]) #列表元素全部为False，则返回False
False
```

    3. 如果可迭代对象为空(元素个数为0)，返回False

```
>>> any([]) #空列表
False
>>> any({}) #空字典
False
>>>
```













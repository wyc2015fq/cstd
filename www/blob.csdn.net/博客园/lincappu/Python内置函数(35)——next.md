# Python内置函数(35)——next - lincappu - 博客园







# [Python内置函数(35)——next](https://www.cnblogs.com/lincappu/p/8144938.html)





**英文文档：**
- `next`(*iterator*[, *default*])
- 
- Retrieve the next item from the *iterator* by calling its `__next__()` method. If *default* is given, it is returned if the iterator is exhausted, otherwise `StopIteration` is raised.
- 
- 
- 
- **　　返回可迭代对象中的下一个元素值**
- 
- **说明：**
- 
- 　　1. 函数必须接收一个可迭代对象参数，每次调用的时候，返回可迭代对象的下一个元素。如果所有元素均已经返回过，则抛出`StopIteration` 异常。

```
>>> a = iter('abcd')
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
  File "<pyshell#18>", line 1, in <module>
    next(a)
StopIteration
```

　　2. 函数可以接收一个可选的default参数，传入default参数后，如果可迭代对象还有元素没有返回，则依次返回其元素值，如果所有元素已经返回，则返回default指定的默认值而不抛出`StopIteration` 异常。

```
>>> a = iter('abcd')
>>> next(a,'e')
'a'
>>> next(a,'e')
'b'
>>> next(a,'e')
'c'
>>> next(a,'e')
'd'
>>> next(a,'e')
'e'
>>> next(a,'e')
'e'
```













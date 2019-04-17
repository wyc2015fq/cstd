# Python内置函数(26)——enumerate - lincappu - 博客园







# [Python内置函数(26)——enumerate](https://www.cnblogs.com/lincappu/p/8144816.html)





**英文文档：**

`enumerate`(*iterable*, *start=0*)

Return an enumerate object. *iterable* must be a sequence, an iterator, or some other object which supports iteration. The `__next__()` method of the iterator returned by `enumerate()` returns a tuple containing a count (from *start* which defaults to 0) and the values obtained from iterating over *iterable*.



```
def enumerate(sequence, start=0):
    n = start
    for elem in sequence:
        yield n, elem
        n += 1
```






- enumerate()是python的内置函数
- enumerate在字典上是枚举、列举的意思
- 对于一个可迭代的（iterable）/可遍历的对象（如列表、字符串），enumerate将其组成一个索引序列，利用它可以同时获得索引和值
- enumerate多用于在for循环中得到计数
- 
enumerate()返回的是一个enumerate对象，例如：

![](https://images2017.cnblogs.com/blog/1272773/201801/1272773-20180104092351065-1550730547.png)




```
>>> seasons = ['Spring', 'Summer', 'Fall', 'Winter']
>>> list(enumerate(seasons))
[(0, 'Spring'), (1, 'Summer'), (2, 'Fall'), (3, 'Winter')]
>>> list(enumerate(seasons, start=1)) #指定起始值
[(1, 'Spring'), (2, 'Summer'), (3, 'Fall'), (4, 'Winter')]
```













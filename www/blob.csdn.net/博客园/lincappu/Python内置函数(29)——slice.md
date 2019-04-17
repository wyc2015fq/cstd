# Python内置函数(29)——slice - lincappu - 博客园







# [Python内置函数(29)——slice](https://www.cnblogs.com/lincappu/p/8144851.html)





**英文文档：**
- *class *`slice`(*stop*)
- *class *`slice`(*start*, *stop*[, *step*])
- 
- Return a slice object representing the set of indices specified by `range(start, stop, step)`. The *start* and *step*arguments default to `None`. Slice objects have read-only data attributes `start`, `stop` and `step` which merely return the argument values (or their default). They have no other explicit functionality; however they are used by Numerical Python and other third party extensions. Slice objects are also generated when extended indexing syntax is used. For example: `a[start:stop:step]` or `a[start:stop, i]`. See `itertools.islice()` for an alternate version that returns an iterator.

 　　根据传入的参数生成一个可切片对象

**说明：**

　　1. 函数实际上是一个切片类的构造函数，返回一个切片对象。

　　2. 切片对象由3个属性start、stop、step组成，start和step默认值为None。切片对象主要用于对序列对象进行切片取对应元素。

```
>>> help(slice)
class slice(object)
 |  slice(stop)
 |  slice(start, stop[, step])
 |  
 |  Create a slice object.  This is used for extended slicing (e.g. a[0:10:2]).
 |  
 |  Methods defined here:
 |  
 |  ...#省略#
 |  ----------------------------------------------------------------------
 |  Data descriptors defined here:
 |  
 |  start
 |  
 |  step
 |  
 |  stop
 |  
 |  ----------------------------------------------------------------------
 |  Data and other attributes defined here:
 |  
 |  __hash__ = None
```

```
>>> a = list(range(10))
>>> a
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

>>> a[None:5:None] # start step显式为None
[0, 1, 2, 3, 4]
>>> a[:5:] # start step默认为None
[0, 1, 2, 3, 4]
>>> a[2:5:None] # step显式为None
[2, 3, 4]
>>> a[2:5:] # step默认为None
[2, 3, 4]
>>> a[1:10:3]
[1, 4, 7]
```

　　3. 对应切片对象的3个属性start、stop、step，slice函数也有3个对应的参数start、stop、step，其值分别会付给切片对象的start、stop、step。

```
>>> c1 = slice(5) # 定义c1
>>> c1
slice(None, 5, None)
>>> c2 = slice(2,5) # 定义c2
>>> c2
slice(2, 5, None)
>>> c3 = slice(1,10,3) # 定义c3
>>> c3
slice(1, 10, 3)
>>> a[c1] # 和a[:5:]结果相同
[0, 1, 2, 3, 4]
>>> a[c2] # 和a[2:5:]结果相同
[2, 3, 4]
>>> a[c3] # 和a[1:10:3]结果相同
[1, 4, 7]
```













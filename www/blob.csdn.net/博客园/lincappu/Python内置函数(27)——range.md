# Python内置函数(27)——range - lincappu - 博客园







# [Python内置函数(27)——range](https://www.cnblogs.com/lincappu/p/8144831.html)





**英文文档：**
- `range`(*stop*)
- `range`(*start*, *stop*[, *step*])
- 
- Rather than being a function, `range` is actually an immutable sequence type, as documented in Ranges and Sequence Types — list, tuple, range.
- 
- 

**　　根据传入的参数创建一个新的 range 对象**

**说明：**
- 　　1. range函数用于生成一个range对象，range类型是一个表示整数范围的类型。
- 　　2. 可以直接传入一个结束整数来初始化一个range类型，默认起始值为0(包含0).结束整数可以大于0，也可以小于等于0，但是小于等于0的时候生成的range对象实际是不包含任何元素的。

```
>>> a = range(5)
>>> a
range(0, 5)
>>> len(a)
5
>>> for x in a:print(x)

0
1
2
3
4

>>> b = range(0) # 传入0，空range对象
>>> len(b)
0

>>> c = range(-5)  # 传入负数，空range对象
>>> len(c)
0
```

　　3. 可以传入一个起始整数和一个结束整数来初始化一个range类型，生成的range类型包含起始整数(包含)，和结束整数(不包含)之间的所有整数。

```
>>> a = range(1,5)
>>> a
range(1, 5)
>>> for x in a:print(x)

1
2
3
4
```

　　4. 传入了起始整数和结束整数，还可以同时传入一个步进值来初始化一个range类型，生成的range类型包含起始整数(包含)，和结束整数(不包含)之间的以步进值筛选后的整数。

```
>>> a = range(1,10,3)
>>> a
range(1, 10, 3)
>>> for x in a:print(x)

1
4
7
```

　　5. 初始化range类型时起始整数和结束整数，遵循的是左臂右开原则，即包含起始整数，但不包含结束整数。

```
>>> a = range(1,5)
>>> a
range(1, 5) 
>>> for x in a:print(x) # 包含1，不包含5

1
2
3
4
```

　　6. range接收的参数都必须是整数，不能是浮点数等其它数据类型。

```
>>> a = range(3.5)
Traceback (most recent call last):
  File "<pyshell#33>", line 1, in <module>
    a = range(3.5)
TypeError: 'float' object cannot be interpreted as an integer
>>> a = range('3.5')
Traceback (most recent call last):
  File "<pyshell#34>", line 1, in <module>
    a = range('3.5')
TypeError: 'str' object cannot be interpreted as an integer
```

　　7. range实际上是一个不可变的序列类型，可以对它进行取元素、切片等序列操作，但是不能对其中元素修改值。

```
>>> a = range(1,5)

>>> a[0] # 取元素
1
>>> a[:-2] # 切片
range(1, 3)
>>> a[1] = 2 # 修改元素值
Traceback (most recent call last):
  File "<pyshell#38>", line 1, in <module>
    a[1] = 2
TypeError: 'range' object does not support item assignment
```













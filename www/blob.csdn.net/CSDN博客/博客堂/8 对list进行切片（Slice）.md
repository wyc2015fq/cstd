# 8 对list进行切片（Slice） - 博客堂 - CSDN博客





2017年03月09日 09:54:42[最小森林](https://me.csdn.net/u012052268)阅读数：671








## 8 对list进行切片（Slice）



- - [对list进行切片Slice](#8-对list进行切片slice)

- [8-0 range函数](#8-0-range函数)
- [8-1 对list进行切片Slice](#8-1-对list进行切片slice)
- [8-2 倒序切片](#8-2-倒序切片)
- [8-3 对字符串切片](#8-3-对字符串切片)




# 8-0 range（）函数

首先讲解一个函数：range（）

先列几个range（）函数的几个用法：

![image](http://images.cnitblog.com/blog/408927/201301/05113441-b7d3c0ac37af4ecb8cb5a261908e73ef.png)

函数原型：range（start， end， scan):

参数含义：

```
start:计数从start开始。默认是从0开始。例如range（5）等价于range（0， 5）;

end:技术到end结束，但不包括end.例如：range（0， 5） 是[0, 1, 2, 3, 4]没有5

scan：每次跳跃的间距，默认为1。例如：range（0， 5） 等价于 range(0, 5, 1)
```

# 8-1 对list进行切片（Slice）

切片：

取一个list的部分元素是非常常见的操作。比如，一个list如下：

```python
>>> L = ['Adam', 'Lisa', 'Bart', 'Paul']
```

取前3个元素，应该怎么做？

笨办法：

```
>>> [L[0], L[1], L[2]]
['Adam', 'Lisa', 'Bart']
```

之所以是笨办法是因为扩展一下，取前N个元素就没辙了。

取前N个元素，也就是索引为0-(N-1)的元素，可以用循环：

```
>>> r = []
>>> n = 3
>>> for i in range(n):
...     r.append(L[i])
... 
>>> r
['Adam', 'Lisa', 'Bart']
```

对这种经常取指定索引范围的操作，用循环十分繁琐，因此，Python提供了切片（Slice）操作符，能大大简化这种操作。

对应上面的问题，取前3个元素，用一行代码就可以完成切片：

```python
>>> L[0:3]
['Adam', 'Lisa', 'Bart']
```

L[0:3]表示，**从索引0开始取，直到索引3为止，但不包括索引3。** 即索引0，1，2，正好是3个元素。

如果第一个索引是0，还可以省略：

```python
>>> L[:3]
['Adam', 'Lisa', 'Bart']
```

也可以从索引1开始，取出2个元素出来：

```python
>>> L[1:3]
['Lisa', 'Bart']
```

只用一个 : ，表示从头到尾：

```python
>>> L[:]
['Adam', 'Lisa', 'Bart', 'Paul']
```

因此，L[:]实际上**复制出**了一个新list。

切片操作还可以指定**第三个参数**：

```python
>>> L[::2]
['Adam', 'Bart']
```

第三个参数表示每N个取一个，上面的 L[::2] 会每两个元素取出一个来，也就是隔一个取一个。

把list换成tuple，切片操作完全相同，只是切片的结果也变成了tuple。

任务

range()函数可以创建一个数列：

```
>>> range(1, 101)
[1, 2, 3, .., 100]
```

请利用切片，取出：
- 前10个数；
- 3的倍数；
- 不大于50的5的倍数。

答案

```
L = range(1, 101)

print(L[:10])
print(L[2::3])
print(L[4:50:5])
```

# 8-2 倒序切片

倒序切片

对于list，既然Python支持L[-1]取倒数第一个元素，那么它同样支持倒数切片，试试：

```python
>>> L = ['Adam', 'Lisa', 'Bart', 'Paul']

>>> L[-2:]
['Bart', 'Paul']

>>> L[:-2]
['Adam', 'Lisa']

>>> L[-3:-1]
['Lisa', 'Bart']

>>> L[-4:-1:2]
['Adam', 'Bart']
```

记住倒数第一个元素的索引是-1。倒序切片包含起始索引，不包含结束索引。

任务

利用倒序切片对 1 - 100 的数列取出：
- 
最后10个数；

- 
最后10个5的倍数。

- 

答案

```php
L = range(1, 101)
print(L[-10:])  #   不能用：print(L[-10:-1]) 因为结尾不包括-1
print(L[-46::5])    
# 或者：
print((L[::5])[-10:])
```

# 8-3 对字符串切片

对字符串切片

字符串 ‘xxx’和 Unicode字符串 u’xxx’也可以看成是一种list，每个元素就是一个字符。因此，字符串也可以用切片操作，只是操作结果仍是字符串：

```python
>>> 'ABCDEFG'[:3]
'ABC'
>>> 'ABCDEFG'[-3:]
'EFG'
>>> 'ABCDEFG'[::2]
'ACEG'
```

在很多编程语言中，针对字符串提供了很多各种截取函数，其实目的就是对字符串切片。Python没有针对字符串的截取函数，只需要切片一个操作就可以完成，非常简单。

任务

字符串有个方法 upper() 可以把字符变成大写字母：

```python
>>> 'abc'.upper()
'ABC'
```

但它会把所有字母都变成大写。请设计一个函数，它接受一个字符串，然后返回一个仅首字母变成大写的字符串。

提示：利用切片操作简化字符串操作。

答案：

```python
def firstCharUpper(s):
    return s[:1].upper() + s[1:]


print(firstCharUpper('hello'))
print(firstCharUpper('sunday'))
print(firstCharUpper('september'))
```




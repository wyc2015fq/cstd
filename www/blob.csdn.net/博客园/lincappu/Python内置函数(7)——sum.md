# Python内置函数(7)——sum - lincappu - 博客园







# [Python内置函数(7)——sum](https://www.cnblogs.com/lincappu/p/8144578.html)





**英文文档：**

`sum`(*iterable*[, *start*])

Sums *start* and the items of an *iterable* from left to right and returns the total. *start*defaults to `0`. The *iterable*‘s items are normally numbers, and the start value is not allowed to be a string.

**说明：**

　　1.函数的功能是对迭代类型进行求和，

要求：

1.接受对象是可迭代类型，

2.节迭代对象的所有元素是数值类型。

```
>>> sum((1,2,3,4))
10
>>> sum([1,23,34])
58
>>> sum(range(10))
```

　　2.可以传入一个可选参数 start，标示求和前的初始化值，如果传入空的可迭代数据类型，则返回初始值。

>>> sum((1,2,3,4),4)
14
>>> sum([1,2,3,4],-4)
6


















# Python内置函数(4)——min - lincappu - 博客园







# [Python内置函数(4)——min](https://www.cnblogs.com/lincappu/p/8144255.html)







**英文文档：**

`min`(*iterable*, ***[, *key*, *default*])

`min`(*arg1*, *arg2*, **args*[, *key*])

Return the smallest item in an iterable or the smallest of two or more arguments.

If one positional argument is provided, it should be an iterable. The smallest item in the iterable is returned. If two or more positional arguments are provided, the smallest of the positional arguments is returned.

There are two optional keyword-only arguments. The *key* argument specifies a one-argument ordering function like that used for `list.sort()`. The *default* argument specifies an object to return if the provided iterable is empty. If the iterable is empty and *default* is not provided, a `ValueError` is raised.

If multiple items are minimal, the function returns the first one encountered. This is consistent with other sort-stability preserving tools such as `sorted(iterable, key=keyfunc)[0]` and `heapq.nsmallest(1, iterable, key=keyfunc)`.



**说明：**



　　1. 函数功能为取传入的多个参数中的最小值，或者传入的可迭代对象元素中的最小值。默认数值型参数，取值小者；字符型参数，取字母表排序靠前者。还可以传入命名参数key，其为一个函数，用来指定取最小值的方法。default命名参数用来指定最小值不存在时返回的默认值。功能与max函数相反。

　　2. 函数至少传入两个参数，但是有只传入一个参数的例外，此时参数必须为可迭代对象，返回的是可迭代对象中的最小元素。



参考 max 函数















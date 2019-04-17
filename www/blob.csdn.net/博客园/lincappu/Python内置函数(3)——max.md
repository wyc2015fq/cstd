# Python内置函数(3)——max - lincappu - 博客园







# [Python内置函数(3)——max](https://www.cnblogs.com/lincappu/p/8144239.html)





**英文文档：**

`max`(*iterable*, ***[, *key*, *default*])

`max`(*arg1*, *arg2*, **args*[, *key*])

Return the largest item in an iterable or the largest of two or more arguments.

If one positional argument is provided, it should be an iterable. The largest item in the iterable is returned. If two or more positional arguments are provided, the largest of the positional arguments is returned.

There are two optional keyword-only arguments. The *key* argument specifies a one-argument ordering function like that used for `list.sort()`. The *default* argument specifies an object to return if the provided iterable is empty. If the iterable is empty and *default* is not provided, a `ValueError` is raised.

If multiple items are maximal, the function returns the first one encountered. This is consistent with other sort-stability preserving tools such as `sorted(iterable, key=keyfunc, reverse=True)[0]` and `heapq.nlargest(1, iterable,key=keyfunc)`.



**说明：**

　　1. 函数功能为取传入的多个参数中的最大值，或者传入的可迭代对象元素中的最大值。默认数值型参数，取值大者；字符型参数，取字母表排序靠后者。还可以传入命名参数key，其为一个函数，用来指定取最大值的方法。default命名参数用来指定最大值不存在时返回的默认值。

　　2. 函数至少传入两个参数，但是有只传入一个参数的例外，此时参数必须为可迭代对象，返回的是可迭代对象中的最大元素。

可迭代对象，字符串，类表，元组，字典、集合都可以比较，
>>> max(1,3,4)
4 
>>> max('1234')
'4'
>>> max((1,2,3,4))
4
>>> max((1,2,3,6,8))
8
>>> max([1,2,3,4])
4
>>> max({'x':1,'y':2})
'y'
>>> max({'x':1,'y':2,'z':10})
'z'


　　3.当传入参数数据类型不一致时，传入的参数会进行隐式数据类型转换，如果不能进行隐式转换就会报错，

   >>> max(1,2.4545)  # 整数和浮点数可以,有多个值时，先比较前两个再跟第三个比较，以此类推，
2.4545
>>> max(1,'a')  # 整数和字符串不可以
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: '>' not supported between instances of 'str' and 'int'
>>>
>>> max([1,3],[2,3])  # 列表和列表可以，它们是按位比较的,
[2, 3]
>>> max([1,3],[1,4])
[1, 4]
>>> max([1,3,4,5,6],[1,4,2,5,7,1])
[1, 4, 2, 5, 7, 1]
>>> max([1,3,4,5,6],[1,4,2,])
[1, 4, 2]
>>> max([1,3,4,5,6],[2])
[2]
>>>
>>> max([1,2,3],(1,3))  # 列表和元组不可以
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: '>' not supported between instances of 'tuple' and 'list'
>>>
SyntaxError: invalid syntax
>>> max({1,3,4,5})
5
>>>    
>>> max({1,3},(1,2))  # 集合和列表也不可以。
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: '>' not supported between instances of 'tuple' and 'set'


　　4. 当存在多个相同的最大值时，返回的是最先出现的那个最大值。

#定义a、b、c 3个列表
>>> a = [1,2]
>>> b = [1,1]
>>> c = [1,2]

#查看a、b、c 的id
>>> id(a)
68128320
>>> id(b)
68128680
>>> id(c)
68128240

#取最大值
>>> d = max(a,b,c)
>>> id(d)
68128320

#验证是否最大值是否是a
>>> id(a) == id(d)
True


　　5.比较的方式:

默认数值类型：取值大者

字符串类型：按字母表排序靠后者

序列类型：依次按索引位置取值比较取最大者。

还可以传入命名参数 key，指定取最大值的方法.

>>> max(-1,-5,key = abs)  # 先执行 abs 函数在执行 max 函数
-5
>>>

key 另一个作用是：原本数据类型不一致，不能进行比较的，传入适当的 key 就可以比较。通过传入的函数，将两者的数据类型转换为一致

>>> max(1,3,'5')Traceback (most recent call last):
File "<stdin>", line 1, in <module>
TypeError: '>' not supported between instances of 'str' and 'int'
>>> max(1.3,'5',key=int)
'5'

>>> max(1,3,'5')    
Traceback (most recent call last):
File "<stdin>", line 1, in <module>
TypeError: '>' not supported between instances of 'str' and 'int'
>>> max(1.3,'5',key=int)   # 整数和字符串
'5'
>>>
>>> max([1,2],(1,3),key=lambda x:x[1])  #列表和元组
(1, 3)
>>>




　6.空的可迭代对象不能进行比较，必须传入一个默认值。

>>> max([1,2],(1,3),key=lambda x:x[1])
(1, 3)
>>>
>>>
>>> max(())
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ValueError: max() arg is an empty sequence
>>> max((),default=1)
1
>>> max((),0)  #默认值必须用命名参数进行传参，否会被认为是一个比较的元素。
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: '>' not supported between instances of 'int' and 'tuple'


















# Python内置函数(23)——dict - lincappu - 博客园







# [Python内置函数(23)——dict](https://www.cnblogs.com/lincappu/p/8144802.html)





**英文文档：**

*class *`dict`(***kwarg*)

*class *`dict`(*mapping*, ***kwarg*)

*class *`dict`(*iterable*, ***kwarg*)

Return a new dictionary initialized from an optional positional argument and a possibly empty set of keyword arguments.

If no positional argument is given, an empty dictionary is created. If a positional argument is given and it is a mapping object, a dictionary is created with the same key-value pairs as the mapping object. Otherwise, the positional argument must be an iterable object. Each item in the iterable must itself be an iterable with exactly two objects. The first object of each item becomes a key in the new dictionary, and the second object the corresponding value. If a key occurs more than once, the last value for that key becomes the corresponding value in the new dictionary.

If keyword arguments are given, the keyword arguments and their values are added to the dictionary created from the positional argument. If a key being added is already present, the value from the keyword argument replaces the value from the positional argument.

 　　根据传入的参数创建一个新的字典

**说明：**

　　1. 字典类的构造函数。

　　2. 不传入任何参数时，返回空字典。

```
>>> dict()
{}
```

 　　3. 可以传入键值对创建字典。

```
>>> dict(a = 1)
{'a': 1}
>>> dict(a = 1,b = 2)
{'b': 2, 'a': 1}
```

 　　4. 可以传入映射函数创建字典。

```
>>> dict(zip(['a','b'],[1,2]))
{'b': 2, 'a': 1}
```

 　　5. 可以传入可迭代对象创建字典。

```
>>> dict((('a',1),('b',2)))
{'b': 2, 'a': 1}
```













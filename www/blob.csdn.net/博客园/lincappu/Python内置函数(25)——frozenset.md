# Python内置函数(25)——frozenset - lincappu - 博客园







# [Python内置函数(25)——frozenset](https://www.cnblogs.com/lincappu/p/8144812.html)





**英文文档：**
- *class *`frozenset`([*iterable*])
- 
- Return a new `frozenset` object, optionally with elements taken from *iterable*. `frozenset` is a built-in class. See `frozenset` and Set Types — set, frozenset for documentation about this class.
- 
- For other containers see the built-in `set`, `list`, `tuple`, and `dict` classes, as well as the `collections` module.
- 
- 　　根据传入的参数生成一个不可变的集合
- 
- **说明：**
- 　　1. 传入一个可迭代对象，生成一个新的不可变集合。
- 

```
>>> a = frozenset(range(10))
>>> a
frozenset({0, 1, 2, 3, 4, 5, 6, 7, 8, 9})
>>> b = frozenset('I am a Pythoner')
>>> b
frozenset({'y', 'I', ' ', 'r', 't', 'm', 'h', 'o', 'a', 'e', 'n', 'P'})
```

　　2. 不传入参数时，生成的空的不可变集合。

```
>>> c = frozenset()
>>> c
frozenset()
```













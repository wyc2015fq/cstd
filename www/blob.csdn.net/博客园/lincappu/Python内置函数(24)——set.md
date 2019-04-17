# Python内置函数(24)——set - lincappu - 博客园







# [Python内置函数(24)——set](https://www.cnblogs.com/lincappu/p/8144804.html)





**英文文档：**
- *class *`set`([*iterable*])
- 
- Return a new `set` object, optionally with elements taken from *iterable*. `set` is a built-in class. See `set` and Set Types — set, frozenset for documentation about this class.
- 
- For other containers see the built-in `frozenset`, `list`, `tuple`, and `dict` classes, as well as the `collections` module.

**　　根据传入的参数生成一个新的集合**

**说明：**

　　1.  传入一个可迭代对象，生成一个新的集合。

```
>>> a = set(range(10))
>>> a
{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
```

　　2.  不传入参数时，生成一个新的空集合。

```
>>> a = set()
>>> a
set()
```

　　3. 返回的集合是可以修改的。

```
>>> a = set()
>>> a
set()
>>> a.add(1)
>>> a
{1}
```













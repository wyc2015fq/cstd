# python3 sorted 如何自定义排序标准 - Keith - CSDN博客





2018年04月15日 20:42:31[ke1th](https://me.csdn.net/u012436149)阅读数：3804








在 python2 中，如果想要自定义评价标准的话，可以这么做

```python
def cmp(a, b):
  # 如果逻辑上认为 a < b ，返回 -1
  # 如果逻辑上认为 a > b , 返回 1
  # 如果逻辑上认为 a == b, 返回 0 
  pass

a = [2,3,1,2]
a = sorted(a, cmp)
```

但是在 `python3` 中，`cmp` 这个参数已经被移除了，那么在 `python3` 中应该怎么实现 `python2` 的 `cmp` 功能呢？ 

```python
import functools
def cmp(a, b):
    if b < a:
        return -1
    if a < b:
        return 1
    return 0
a = [1, 2, 5, 4]
print(sorted(a, key=functools.cmp_to_key(cmp)))
```

上面这个方法实现了降序排列，因为 `-1` 代表我们逻辑上认为 `a<b` ，而实际上 `b<a` 。

追溯 `cmp_to_key` 的源码，发现是这样的

```python
def cmp_to_key(mycmp):
    """Convert a cmp= function into a key= function"""
    class K(object):
        __slots__ = ['obj']
        def __init__(self, obj):
            self.obj = obj
        def __lt__(self, other):
            return mycmp(self.obj, other.obj) < 0
        def __gt__(self, other):
            return mycmp(self.obj, other.obj) > 0
        def __eq__(self, other):
            return mycmp(self.obj, other.obj) == 0
        def __le__(self, other):
            return mycmp(self.obj, other.obj) <= 0
        def __ge__(self, other):
            return mycmp(self.obj, other.obj) >= 0
        __hash__ = None
    return K
```

返回的是一个类，在 `sorted` 内部，类接收一个参数构造一个实例，然后实例通过重载的方法来进行比较。

```python
k1 = K(1)
k2 = K(2)

# 问题，k1，k2 谁是 self，谁是 other
# k1 是 self， k2 是 other
print(k1 < k2)
```




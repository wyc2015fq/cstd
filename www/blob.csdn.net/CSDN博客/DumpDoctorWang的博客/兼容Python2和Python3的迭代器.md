# 兼容Python2和Python3的迭代器 - DumpDoctorWang的博客 - CSDN博客





2019年02月15日 20:58:40[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：27








除了内置的迭代器类型，任何一个含有__iter__函数、next（Python2）或者__next__函数的类都是迭代器类型。__iter__函数返回一个可迭代的对象；next（Python2）或者__next__函数返回下一个值。

例如，编写一个整数迭代器，从1迭代到100。

# Python2

```python
# coding=utf8

class IntegerIterator:
    def __init__(self):
        self._num = 0

    def __iter__(self):
        return self

    def next(self):
        if self._num == 100:
            raise StopIteration
        self._num += 1
        return self._num

if __name__ == '__main__':
    for a in IntegerIterator():
        print(a)
```

# Python3

```python
# coding=utf8

class IntegerIterator:
    def __init__(self):
        self._num = 0

    def __iter__(self):
        return self

    def __next__(self):
        if self._num == 100:
            raise StopIteration
        self._num += 1
        return self._num

if __name__ == '__main__':
    for a in IntegerIterator():
        print(a)
```

和Python2相比，next函数变成了__next__函数。

# 兼容Python2和Python3的迭代器

要编写兼容Python2和Python3的迭代器，就很简单：

```python
# coding=utf8

class IntegerIterator:
    def __init__(self):
        self._num = 0

    def __iter__(self):
        return self

    def __next__(self):
        if self._num == 100:
            raise StopIteration
        self._num += 1
        return self._num

    next = __next__


if __name__ == '__main__':
    for a in IntegerIterator():
        print(a)
```

定义`__next__`函数，再定义`next`指向`__next__`。这样的代码就不会报错误：

```
# Python2
TypeError: instance has no next() method
# Python3
TypeError: iter() returned non-iterator of type '***'
```




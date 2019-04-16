# python3与python2中map函数的区别 - 一个跳popping的quant的博客 - CSDN博客





2018年07月22日 16:03:35[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：1218
所属专栏：[学python的路上](https://blog.csdn.net/column/details/20561.html)









### 先看一下python2中的map函数：

```python
def map(function, sequence, *sequence_1): # real signature unknown; restored from __doc__
    """
    map(function, sequence[, sequence, ...]) -> list
    
    Return a list of the results of applying the function to the items of
    the argument sequence(s).  If more than one sequence is given, the
    function is called with an argument list consisting of the corresponding
    item of each sequence, substituting None for missing values when not all
    sequences have the same length.  If the function is None, return a list of
    the items of the sequence (or a list of tuples if more than one sequence).
    """
    return []
```

也即是将一个function作用于sequence中每个元素身上，最后返回这个被function作用后的list。

### 再看一下python3中的map函数：

```python
class map(object):
    """
    map(func, *iterables) --> map object
    
    Make an iterator that computes the function using arguments from
    each of the iterables.  Stops when the shortest iterable is exhausted.
    """
    def __getattribute__(self, *args, **kwargs): # real signature unknown
        """ Return getattr(self, name). """
        pass

    def __init__(self, func, *iterables): # real signature unknown; restored from __doc__
        pass

    def __iter__(self, *args, **kwargs): # real signature unknown
        """ Implement iter(self). """
        pass

    @staticmethod # known case of __new__
    def __new__(*args, **kwargs): # real signature unknown
        """ Create and return a new object.  See help(type) for accurate signature. """
        pass

    def __next__(self, *args, **kwargs): # real signature unknown
        """ Implement next(self). """
        pass

    def __reduce__(self, *args, **kwargs): # real signature unknown
        """ Return state information for pickling. """
        pass
```

在python3中map被封装成了一个类，功能依旧是将function作用于要被遍历的序列，但是最后返回的结果就是一个对象了。



### 通过代码举一个将int转换为float的例子：

```python
if __name__ == '__main__':
    x = [1, 2, 3, 4, 5]
    y = map(float, x)
    print(y)
```

python2中：

```python
[1.0, 2.0, 3.0, 4.0, 5.0]
```

python3中：

```
<map object at 0x000001FB6DAF0A58>
```

如何将python3中的对象转换为python2中的形式呢？

只需要通过list作用于map即可：

```python
if __name__ == '__main__':
    x = [1, 2, 3, 4, 5]
    y = list(map(float, x))
    print(y)
```

```python
[1.0, 2.0, 3.0, 4.0, 5.0]
```








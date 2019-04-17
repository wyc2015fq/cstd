# 【python】itertools.chain类解析 - zkq_1986的博客 - CSDN博客





2018年12月12日 09:26:42[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：139








```python
import itertools
b=[[1, 2, 3]] +[[4, 5, 6, 7, 8, 9]]
a=itertools.chain(*map(list,b))

for e in a:
    print(e)
```

输出：

1

2

3

4

5

6

7

8

9



itertools中chain类的代码片段。

class chain(object):
    """
    chain(*iterables) --> chain object   #需要多个可迭代器
    
    Return a chain object whose .__next__() method returns elements from the
    first iterable until it is exhausted, then elements from the next
    iterable, until all of the iterables are exhausted.
    """



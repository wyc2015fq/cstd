# python 小工具总结 - Keith - CSDN博客





2017年07月04日 17:15:34[ke1th](https://me.csdn.net/u012436149)阅读数：993








> 
总结一些python编程中可能会用到的一些小工具


## namedtuple

> 
给tuple 起个名字


```python
def namedtuple(typename, field_names, verbose=False, rename=False):
  """Returns a new subclass of tuple with named fields.

    >>> Point = namedtuple('Point', ['x', 'y'])
    >>> Point.__doc__                   # docstring for the new class
    'Point(x, y)'
    >>> p = Point(11, y=22)             # instantiate with positional args or keywords
    >>> p[0] + p[1]                     # indexable like a plain tuple
    33
    >>> x, y = p                        # unpack like a regular tuple
    >>> x, y
    (11, 22)
    >>> p.x + p.y                       # fields also accessible by name
    33
    >>> d = p._asdict()                 # convert to a dictionary
    >>> d['x']
    11
    >>> Point(**d)                      # convert from a dictionary
    Point(x=11, y=22)
    >>> p._replace(x=100)               # _replace() is like str.replace() but targets named fields
    Point(x=100, y=22)

    """

from collections import namedtuple
NamedTuple = namedtuple("NamedTuple", ['output', 'stride']) # 返回一个类

tup1 = NamedTuple(output=54, stride=2) 
print(tup1[1], tup1)
```

## OrderedDict

```python
# 记录了插入元素顺序的 dict
from collections import OrderedDict

ordered_dict = OrderedDict()

ordered_dict['he'] = 'hello'
ordered_dict['be'] = 'bello'
print(ordered_dict)
# OrderedDict([('he', 'hello'), ('be', 'bello')])
# 遍历方法与 dict 一致
```




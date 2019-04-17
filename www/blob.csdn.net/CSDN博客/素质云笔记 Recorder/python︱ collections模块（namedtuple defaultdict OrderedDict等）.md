# python︱ collections模块（namedtuple/defaultdict/OrderedDict等） - 素质云笔记/Recorder... - CSDN博客





2018年11月27日 11:06:01[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：211








collections有的功能：

```
['deque', 'defaultdict', 'namedtuple',  'UserDict', 'UserList', 'UserString',\
 'Counter', 'OrderedDict', 'ChainMap', 'Awaitable', 'Coroutine', 'AsyncIterable', \
 'AsyncIterator', 'AsyncGenerator', 'Hashable',  'Iterable', 'Iterator', 'Generator', 'Reversible', \
 'Sized', 'Container', 'Callable', 'Collection', 'Set', 'MutableSet', 'Mapping', 'MutableMapping', \
 'MappingView', 'KeysView', 'ItemsView', 'ValuesView', 'Sequence', 'MutableSequence', 'ByteString']
```

高效/便捷。

官方：[python - collections](https://docs.python.org/3.6/library/collections.html)




### 文章目录
- [1 namedtuple - 可命名的tuple](#1_namedtuple__tuple_18)
- [1.1 namedtuple 新建](#11_namedtuple__42)
- [1.2 namedtuple支持通过属性访问](#12_namedtuple_76)
- [1.3 namedtuple转OrderedDict](#13_namedtupleOrderedDict_96)
- [1.4 其他一些属性](#14__108)
- [1.5 实践的例子](#15__119)
- [2 deque - 高效有序的list](#2_deque__list_153)
- [3 defaultdict - 防dict报错](#3_defaultdict__dict_193)
- [情况一：报错返回默认值](#_199)
- [情况二：未定义key，一键定义](#key_216)
- [4 OrderedDict - 有序的dict](#4_OrderedDict__dict_263)
- [5 ChainMap - dict合并](#5_ChainMap__dict_379)
- [6 Counter - 计数](#6_Counter___436)
- [参考：](#_552)




# 1 namedtuple - 可命名的tuple

```
>>> from collections import namedtuple
>>> Point = namedtuple('Point', ['x', 'y'])
>>> p = Point(1, 2)
>>> p.x
1
>>> p.y
2
```

给[‘x’,‘y’]这个tuple命名为`point`，这个tuple中，第一个空位命名为`'x'`，第二个为`'y'`。

```
>>> isinstance(p, Point)
True
>>> isinstance(p, tuple)
True
```

## 1.1 namedtuple 新建

```
from collections import namedtuple

Animal = namedtuple('Animal', 'name age type')
perry = Animal(name="perry", age=31, type="cat")

print(perry)
## 输出: Animal(name='perry', age=31, type='cat')

print(perry.name)
## 输出: 'perry'

print(perry[0])
## 输出: perry
```

在上面的例子中，我们的元组名称是Animal，字段名称是’name’，‘age’和’type’。

要记住它是一个元组，属性值在namedtuple中是不可变的，所以下面的代码不能工作：

```
from collections import namedtuple

Animal = namedtuple('Animal', 'name age type')
perry = Animal(name="perry", age=31, type="cat")
perry.age = 42

## 输出:
## Traceback (most recent call last):
##     File "", line 1, in
## AttributeError: can't set attribute
```

## 1.2 namedtuple支持通过属性访问

namedtuple既支持tupleindex的访问方式，也支持通过属性访问

```
>>> p[0] + p[1] 
33
>>> x, y = p
>>> x, y 
(11, 22)
>>> p.x + p.y
33
```

namedtuple和dict的互转，严格说是与OrderedDict互转，因为_asdict返回的是一个OrderedDict

```
>>> d = p._asdict() 
OrderedDict([('x', 11), ('y', 22)])
```

## 1.3 namedtuple转OrderedDict

```
from collections import namedtuple

Animal = namedtuple('Animal', 'name age type')
perry = Animal(name="Perry", age=31, type="cat")
print(perry._asdict())

## 输出: OrderedDict([('name', 'Perry'), ('age', 31), ...
```

## 1.4 其他一些属性

```
>>> p2._replace(y=100)    # 替换元素值
Point(x=11, y=100, z=33)
>>> p._fields             # 查看对象字段
('x', 'y', 'z')
>>> Point._make(range(3)) # 通过一个序列或者可迭代对象创建一个对象
Point(x=0, y=1, z=2)
```

## 1.5 实践的例子

```
# -*- coding: utf-8 -*-
"""
比如我们用户拥有一个这样的数据结构，每一个对象是拥有三个元素的tuple。
使用namedtuple方法就可以方便的通过tuple来生成可读性更高也更好用的数据结构。
"""
from collections import namedtuple

websites = [
    ('Sohu', 'http://www.google.com/', u'张朝阳'),
    ('Sina', 'http://www.sina.com.cn/', u'王志东'),
    ('163', 'http://www.163.com/', u'丁磊')
]

Website = namedtuple('Website', ['name', 'url', 'founder'])

for website in websites:
    website = Website._make(website)
    print website


# Result:
Website(name='Sohu', url='http://www.google.com/', founder=u'\u5f20\u671d\u9633')
Website(name='Sina', url='http://www.sina.com.cn/', founder=u'\u738b\u5fd7\u4e1c')
Website(name='163', url='http://www.163.com/', founder=u'\u4e01\u78ca')
```

# 2 deque - 高效有序的list

双端队列，你可以从头/尾两端添加或删除元素。

使用list存储数据时，按索引访问元素很快，但是插入和删除元素就很慢了，因为list是线性存储，数据量大的时候，插入和删除效率很低。

deque是为了高效实现插入和删除操作的双向列表，适合用于队列和栈：

```
>>> from collections import deque
>>> q = deque(['a', 'b', 'c'])
>>> q.append('x')
>>> q.appendleft('y')
>>> q
deque(['y', 'a', 'b', 'c', 'x'])
```

deque除了实现list的append()和pop()外，还支持appendleft()和popleft()，这样就可以非常高效地往头部添加或删除元素。

**我们也可以限制这个列表的大小**，当超出你设定的限制时，数据会从对队列另一端被挤出去(pop)。

最好的解释是给出一个例子：

```
d = deque(maxlen=30)
```

现在当你插入30条数据时，最左边一端的数据将从队列中删除。

你还可以从任一端扩展这个队列中的数据：

```
d = deque([1,2,3,4,5])
d.extendleft([0])
d.extend([6,7,8])
print(d)

## 输出: deque([0, 1, 2, 3, 4, 5, 6, 7, 8])
```

# 3 defaultdict - 防dict报错

使用dict时，如果引用的Key不存在，就会抛出KeyError。
- 如果希望key不存在时，返回一个默认值，就可以用defaultdict；
- 如果希望key不存在，但是想赋值成功也可以使用defaultdict。

## 情况一：报错返回默认值

```
>>> from collections import defaultdict
>>> dd = defaultdict(lambda: 'N/A')
>>> dd['key1'] = 'abc'
>>> dd['key1'] # key1存在
'abc'
>>> dd['key2'] # key2不存在，返回默认值
'N/A'
```

注意默认值是调用函数返回的，而函数在创建defaultdict对象时传入。

除了在Key不存在时返回默认值，defaultdict的其他行为跟dict是完全一样的。

## 情况二：未定义key，一键定义

新添加内容：

```
from collections import defaultdict

colours = (
    ('Yasoob', 'Yellow'),
    ('Ali', 'Blue'),
    ('Arham', 'Green'),
    ('Ali', 'Black'),
    ('Yasoob', 'Red'),
    ('Ahmed', 'Silver'),
)

favourite_colours = defaultdict(list)

for name, colour in colours:
    favourite_colours[name].append(colour)

print(favourite_colours)
```

当你在一个字典中对一个键进行嵌套赋值时，如果这个键不存在，会触发keyError异常。 defaultdict允许我们用一个聪明的方式绕过这个问题。 首先我分享一个使用dict触发KeyError的例子，然后提供一个使用defaultdict的解决方案。

如果是dict，会报错：

```
some_dict = {}
some_dict['colours']['favourite'] = "yellow"

## 异常输出：KeyError: 'colours'
```

如果是defaultdict，会如常运行：

```
import collections
tree = lambda: collections.defaultdict(tree)
some_dict = tree()
some_dict['colours']['favourite'] = "yellow"

## 运行正常
```

# 4 OrderedDict - 有序的dict

使用dict时，Key是无序的。在对dict做迭代时，我们无法确定Key的顺序。

如果要保持Key的顺序，可以用OrderedDict：

```
>>> from collections import OrderedDict
>>> d = dict([('a', 1), ('b', 2), ('c', 3)])
>>> d # dict的Key是无序的
{'a': 1, 'c': 3, 'b': 2}
>>> od = OrderedDict([('a', 1), ('b', 2), ('c', 3)])
>>> od # OrderedDict的Key是有序的
OrderedDict([('a', 1), ('b', 2), ('c', 3)])
```

再来一个案例：

```
>>> p = collections.OrderedDict()
>>> p["a"]=1
>>> p["b"]=2
>>> p["c"]=3
>>> print(p)
OrderedDict([('a', 1), ('b', 2), ('c', 3)])
```

循环新建：

```
>>> # regular unsorted dictionary
>>> d = {'banana': 3, 'apple': 4, 'pear': 1, 'orange': 2}

>>> # dictionary sorted by key
>>> OrderedDict(sorted(d.items(), key=lambda t: t[0]))
OrderedDict([('apple', 4), ('banana', 3), ('orange', 2), ('pear', 1)])

>>> # dictionary sorted by value
>>> OrderedDict(sorted(d.items(), key=lambda t: t[1]))
OrderedDict([('pear', 1), ('orange', 2), ('banana', 3), ('apple', 4)])

>>> # dictionary sorted by length of the key string
>>> OrderedDict(sorted(d.items(), key=lambda t: len(t[0])))
OrderedDict([('pear', 1), ('apple', 4), ('orange', 2), ('banana', 3)])
```

OrderedDict提供了下面的一些api。

```
>>> p.
p.clear(        p.fromkeys(     p.items(        p.move_to_end(  p.popitem(      p.update(
p.copy(         p.get(          p.keys(         p.pop(          p.setdefault(   p.values(
```

注意，OrderedDict的Key会按照插入的顺序排列，不是Key本身排序：

```
>>> od = OrderedDict()
>>> od['z'] = 1
>>> od['y'] = 2
>>> od['x'] = 3
>>> od.keys() # 按照插入的Key的顺序返回
['z', 'y', 'x']
```

单地试一下update，pop，move_to_end和clear

```
>>> keys=["apple", "banana", "cat"] 
>>> value=[4, 5, 6] 

# update 
>>> p.update(zip(keys,value)) 
>>> p 
OrderedDict([('a', 1), ('b', 2), ('c', 3), ('apple', 4), ('banana', 5), ('cat', 6)])

# pop 
>>> p.pop('a') 
1 
>>> p 
OrderedDict([('b', 2), ('c', 3), ('apple', 4), ('banana', 5), ('cat', 6)]) 

# popitem(last=True)
>>> d = OrderedDict({'banana': 3, 'apple': 4, 'pear': 1, 'orange': 2})
>>> d
OrderedDict([('banana', 3), ('apple', 4), ('pear', 1), ('orange', 2)])
>>> d.popitem()
('orange', 2)
>>> d
OrderedDict([('banana', 3), ('apple', 4), ('pear', 1)])
>>> d.popitem(last=False)
('banana', 3)
>>> d
OrderedDict([('apple', 4), ('pear', 1)])

# move_to_end 
>>> p.move_to_end('b') 
>>> p 
OrderedDict([('c', 3), ('apple', 4), ('banana', 5), ('cat', 6), ('b', 2)]) 

# del 
>>> del(p['c'])
>>> p 
OrderedDict([('apple', 4), ('banana', 5), ('cat', 6), ('b', 2)]) 

# clear 
>>> p.clear() 
>>> p 
OrderedDict()
```

# 5 ChainMap - dict合并

ChainMap 用来成合并多个字典，但和 update 不同，它不会改变原对象，而且效率更高。

ChainMap用来将多个dict(字典)组成一个list(只是比喻).

```
>>> from collections import ChainMap
>>> a = {'a': 'A', 'c': 'C'}
>>> b = {'b': 'B', 'c': 'D'}
>>> m = ChainMap(a, b)
# 构造一个ChainMap对象
>>> m
ChainMap({'a': 'A', 'c': 'C'}, {'b': 'B', 'c': 'D'})
>>> m['a']
'A'
>>> m['c'] # 获取的是第一个字典中的值
'C'
# 将m变成一个list
>>> m.maps
[{'a': 'A', 'c': 'C'}, {'b': 'B', 'c': 'D'}]
```

不过，如果合并的dict之中，有同一个key，那么会优先以第一个出现的为主。

同时，如何获得一个dict的`keys`：

```
>>> chain.keys()
KeysView(ChainMap({'a': 1, 'b': 2}, {'b': 3, 'c': 4}))
>>> list(chain.keys())
['a', 'b', 'c']
>>> print(chainMap.items())
ItemsView(ChainMap({'user': 'guest', 'color': 'red'}, {'age': '18', 'name': 'drfish'}))
```

chainmap的更新：

```
# 更新a中的值也会对ChainMap对象造成影响
>>> a['c'] = 'E'
>>> m['c']
'E'
# 从m复制一个ChainMap对象，更新这个复制的对象并不会对m造成影响
>>> m2 = m.new_child()
>>> m2['c'] = 'f'
>>> m['c']
'E'
>>> a['c']
'E'
# parents属性
>>> m2.parents
ChainMap({'a': 'A', 'c': 'C'}, {'b': 'B', 'c': 'D'})
# 添加新字典
>>> dict3 = { 'h' : 5 }
>>> new_chain = chain.new_child(dict3)
```

# 6 Counter - 计数

参考：[Python标准库——collections模块的Counter类](http://www.pythoner.com/205.html)

**创建的几种方式：**

```
>>> c = Counter()  # 创建一个空的Counter类
>>> c = Counter('gallahad')  # 从一个可iterable对象（list、tuple、dict、字符串等）创建
>>> c = Counter({'a': 4, 'b': 2})  # 从一个字典对象创建
>>> c = Counter(a=4, b=2)  # 从一组键值对创建
```

**使用的方式**

Counter是一个简单的计数器，例如，统计字符出现的个数：

```
>>> from collections import Counter
>>> c = Counter()
>>> for ch in 'programming':
...     c[ch] = c[ch] + 1
...
>>> c
Counter({'g': 2, 'm': 2, 'r': 2, 'a': 1, 'i': 1, 'o': 1, 'n': 1, 'p': 1})
#键的删除
del c["g"]
```

Counter实际上也是dict的一个子类，上面的结果可以看出，字符’g’、‘m’、'r’各出现了两次，其他字符各出现了一次。

使用elements()方法按照元素的出现次数返回一个iterator(迭代器)，元素以任意的顺序返回，如果元素的计数小于1，将忽略它。

```
>>> c = Counter(a=4, b=2, c=0, d=-2)
>>> c
Counter({'a': 4, 'b': 2, 'c': 0, 'd': -2})
>>> c.elements()
<itertools.chain object at 0x7fb0a069ccf8>
>>> next(c)
'a'
# 排序
>>> sorted(c.elements())
['a', 'a', 'a', 'a', 'b', 'b']
```

使用most_common(n)返回一个list, list中包含Counter对象中出现最多前n个元素。

```
>>> c = Counter('abracadabra')
>>> c
Counter({'a': 5, 'b': 2, 'r': 2, 'd': 1, 'c': 1})
# 获取出现频率最高的3个字符
>>> c.most_common(3)  
[('a', 5), ('b', 2), ('r', 2)]
```

当所访问的键不存在时，返回0，而不是KeyError；否则返回它的计数。

**计数器的更新（update和subtract）**

```
>>> c = Counter('which')
>>> c.update('witch')  # 使用另一个iterable对象更新
>>> c['h']
3
>>> d = Counter('watch')
>>> c.update(d)  # 使用另一个Counter对象更新
>>> c['h']
4
```

**键的删除**

```
>>> c = Counter("abcdcba")
>>> c
Counter({'a': 2, 'c': 2, 'b': 2, 'd': 1})
>>> c["b"] = 0
>>> c
Counter({'a': 2, 'c': 2, 'd': 1, 'b': 0})
>>> del c["a"]
>>> c
Counter({'c': 2, 'b': 2, 'd': 1})
```

**算术和集合操作**

+、-、&、|操作也可以用于Counter。其中&和|操作分别返回两个Counter对象各元素的最小值和最大值。需要注意的是，得到的Counter对象将删除小于1的元素。

```
>>> c = Counter(a=3, b=1)
>>> d = Counter(a=1, b=2)
>>> c + d  # c[x] + d[x]
Counter({'a': 4, 'b': 3})
>>> c - d  # subtract（只保留正数计数的元素）
Counter({'a': 2})
>>> c & d  # 交集:  min(c[x], d[x])
Counter({'a': 1, 'b': 1})
>>> c | d  # 并集:  max(c[x], d[x])
Counter({'a': 3, 'b': 2})
```

其他常见的操作：

```
sum(c.values())  # 所有计数的总数
c.clear()  # 重置Counter对象，注意不是删除
list(c)  # 将c中的键转为列表
set(c)  # 将c中的键转为set
dict(c)  # 将c中的键值对转为字典
c.items()  # 转为(elem, cnt)格式的列表
Counter(dict(list_of_pairs))  # 从(elem, cnt)格式的列表转换为Counter类对象
c.most_common()[:-n:-1]  # 取出计数最少的n-1个元素
c += Counter()  # 移除0和负值
本文内容遵从CC3.0版权协议，转载请注明：转自Pythoner
```

# 参考：

[廖雪峰- collections](https://www.liaoxuefeng.com/wiki/001374738125095c955c1e6d8bb493182103fac9270762a000/001411031239400f7181f65f33a4623bc42276a605debf6000)
[Python标准库之collections使用教程](http://python.jobbole.com/87201/)
[Python collections使用](https://www.jianshu.com/p/f2a429aa5963)
[eastlakeside.gitbooks.io - 容器(Collections)](https://eastlakeside.gitbooks.io/interpy-zh/content/collections/collections.html)
[Python 模块简介 – collections](http://kuanghy.github.io/2017/03/03/python-collections)

官方：[python - collections](https://docs.python.org/3.6/library/collections.html)




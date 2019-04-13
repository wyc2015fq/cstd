
# Python 学习入门（11）—— 排序 - 阳光岛主 - CSDN博客

2013年11月21日 23:15:32[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：4172所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



Python的内置dictionary数据类型是无序的，通过key来获取对应的value。可是有时我们需要对dictionary中 的item进行排序输出，可能根据key，也可能根据value来排。

**示例：**

```python
﻿#/usr/bin/python
#
# it-homer in 2013

def test_sort():
  d = {"a":"it", "b":"homer", "c":"sunboy", "d":2050}
  print(d)              # {'a': 'it', 'c': 'sunboy', 'b': 'homer', 'd': 2050}
  d['h'] = "ithomer"
  print(d)              # {'a': 'it', 'h': 'ithomer', 'c': 'sunboy', 'b': 'homer', 'd': 2050}
  del(d['h'])
  print(d)              # {'a': 'it', 'c': 'sunboy', 'b': 'homer', 'd': 2050}

  # 字典的遍历
  for k in d:
    print "dict[%s] = " % k, d[k]
  #
  for k in d:
    print("dict[%s] = %s" % (k, d[k]))
  # items() 遍历
  for k,v in d.items():
    print("dict[%s] = %s" % (k, v))
  # iteritems() 遍历
  for k,v in d.iteritems():
    print("dict[%s] = %s" % (k, v))
  # zip() 遍历
  for k,v in zip(d.iterkeys(), d.itervalues()):
    print("dict[%s] = %s" % (k, v))
  # d.keys() 遍历
  for k in d.keys():
    print("dict[%s] = %s" % (k, d[k]))
  '''
  dict[a] = it
  dict[c] = sunboy
  dict[b] = homer
  dict[d] = 2050
  '''

def test_sort2():
  d = {"a":("it","homer"), "b":{"e":"blog", "f":"forum"}, "c":["sun", "boy", 2050]}
  print(d)              # {'a': ('it', 'homer'), 'c': ['sun', 'boy', 2050], 'b': {'e': 'blog', 'f': 'forum'}}
  print(d['a'])         # ('it', 'homer')
  print(d['a'][0])      # it
  print(d['b'])         # {'e': 'blog', 'f': 'forum'}
  print(d['b']['e'])    # blog
  print(d['c'])         # ['sun', 'boy', 2050]
  print(d['c'][1])      # boy
  d2 = d['b']
  for k in d2:
    print("%s : %s" % (k, d2[k]))       # e : blog  "\n" f : forum

  # 字典的更新
  d = {"a":"it", "b":"homer"}
  f = {"a":"blog", "c":"forum"}
  d.update(f)
  print d               # {'a': 'blog', 'c': 'forum', 'b': 'homer'}

  d = {}
  d.setdefault("a")
  print d               # {'a': None}
  d.setdefault("b", 0)
  print d               # {'a': None, 'b': 0}

import copy
def test_sort3():
  d = {"a":"it", "b":"homer", "c":"blog", "d":"forum"}
  # 字典的浅拷贝
  f = d.copy()
  print f               # {'a': 'it', 'c': 'blog', 'b': 'homer', 'd': 'forum'}
  
  # 字典的浅拷贝
  d = {"a":"it", "b":"homer", "x":{"c":"blog", "d":"forum"}}
  print d               # {'a': 'it', 'x': {'c': 'blog', 'd': 'forum'}, 'b': 'homer'}
  f = d.copy()
  print f               # {'a': 'it', 'x': {'c': 'blog', 'd': 'forum'}, 'b': 'homer'}
  d['x']['c'] = 'change'
  print d               # {'a': 'it', 'x': {'c': 'change', 'd': 'forum'}, 'b': 'homer'}
  print f               # {'a': 'it', 'x': {'c': 'change', 'd': 'forum'}, 'b': 'homer'}
  # 字典的深拷贝
  d = {"a":"it", "b":"homer", "x":{"c":"blog", "d":"forum"}}
  print d               # {'a': 'it', 'x': {'c': 'blog', 'd': 'forum'}, 'b': 'homer'}
  f = copy.deepcopy(d)
  print f               # {'a': 'it', 'x': {'c': 'blog', 'd': 'forum'}, 'b': 'homer'}
  d['x']['c'] = 'change'
  print d               # {'a': 'it', 'x': {'c': 'change', 'd': 'forum'}, 'b': 'homer'}
  print f               # {'a': 'it', 'x': {'c': 'blog', 'd': 'forum'}, 'b': 'homer'}

def test_sort4():
  d = {"a":1, "c":7, "e":5, "d":9, "b":3}
  print d               # {'a': 1, 'c': 7, 'b': 3, 'e': 5, 'd': 9}
  # 按照key排序
  for k, v in sorted(d.items(), key=lambda x:x[0]):
    print(k,v)          # ('a', 1) ('b', 3) ('c', 7) ('d', 9) ('e', 5)
  # 按照key排序
  for k in sorted(d.keys()):
    print(k,d[k])       # ('a', 1) ('b', 3) ('c', 7) ('d', 9) ('e', 5)
  # 按照key排序
  l = list(d.keys())
  l.sort()
  for k in l:
    print(k,d[k])       # ('a', 1) ('b', 3) ('c', 7) ('d', 9) ('e', 5)
  # 按照value排序
  for k, v in sorted(d.items(), key=lambda x:x[1]):
    print(k,v)          # ('a', 1) ('b', 3) ('e', 5) ('c', 7) ('d', 9)
  # 按照value 逆序排序
  for k, v in sorted(d.items(), key=lambda x:x[1], reverse = True):
    print(k,v)          # ('d', 9) ('c', 7) ('e', 5) ('b', 3) ('a', 1)
  # 按照value 逆序排序
  for k, v in sorted(d.items(), lambda x, y: cmp(x[1], y[1]), reverse = True):
    print(k,v)          # ('d', 9) ('c', 7) ('e', 5) ('b', 3) ('a', 1)

if __name__ == "__main__":
  test_sort()
  test_sort2()
  test_sort3()
  test_sort4()
```

python内置sorted函数的帮助文档：
sorted(...)
sorted(iterable, cmp=None, key=None, reverse=False) --> new sorted list
看了上面这么多种对dictionary排序的方法，其实它们的核心思想都一样，即把dictionary中的元素分离出来放到一个list中，对list排序，从而间接实现对dictionary的排序。这个“元素”可以是key，value或者item。

**List 字段排序**
python的排序函数sort,sorted在列表排序和字典排序中的应用详解和举例
python 列表list中内置了一个十分有用的排序函数sort,sorted，它可以用于列表的排序，以下是例子。

```python
a = [5,2,1,9,6]        
 
>>> sorted(a)                  #将a从小到大排序,不影响a本身结构 
[1, 2, 5, 6, 9] 
 
>>> sorted(a,reverse = True)   #将a从大到小排序,不影响a本身结构 
[9, 6, 5, 2, 1] 
 
>>> a.sort()                   #将a从小到大排序,影响a本身结构 
>>> a 
[1, 2, 5, 6, 9] 
 
>>> a.sort(reverse = True)     #将a从大到小排序,影响a本身结构 
>>> a 
[9, 6, 5, 2, 1] 
 
注意，a.sort() 已改变其结构，b = a.sort() 是错误的写法! 
>>> b = ['aa','BB','bb','zz','CC'] 
>>> sorted(b) 
['BB', 'CC', 'aa', 'bb', 'zz']    #按列表中元素每个字母的ascii码从小到大排序,如果要从大到小,请用sorted(b,reverse=True)下同 
 
>>> c =['CCC', 'bb', 'ffff', 'z']  
>>> sorted(c,key=len)             #按列表的元素的长度排序 
['z', 'bb', 'CCC', 'ffff'] 
 
>>> d =['CCC', 'bb', 'ffff', 'z'] 
>>> sorted(d,key = str.lower )    #将列表中的每个元素变为小写，再按每个元素中的每个字母的ascii码从小到大排序 
['bb', 'CCC', 'ffff', 'z'] 
 
>>> def lastchar(s): 
       return s[-1] 
>>> e = ['abc','b','AAz','ef'] 
>>> sorted(e,key = lastchar)      #自定义函数排序,lastchar为函数名，这个函数返回列表e中每个元素的最后一个字母 
['b', 'abc', 'ef', 'AAz']         #sorted(e,key=lastchar)作用就是 按列表e中每个元素的最后一个字母的ascii码从小到大排序 
 
>>> f = [{'name':'abc','age':20},{'name':'def','age':30},{'name':'ghi','age':25}]     #列表中的元素为字典 
>>> def age(s): 
       return s['age'] 
>>> ff = sorted(f,key = age)      #自定义函数按列表f中字典的age从小到大排序  
 
[{'age': 20, 'name': 'abc'}, {'age': 25, 'name': 'ghi'}, {'age': 30, 'name': 'def'}] 
 
>>> f2 = sorted(f,key = lambda x:x['age'])    #如果觉得上面定义一个函数代码不美观，可以用lambda的形式来定义函数,效果同上
```
最后一行，对 数组的某一字段排序

**参考推荐：**
[python dict sorted 排序](http://www.cnblogs.com/linyawen/archive/2012/03/15/2398292.html)
[Python中dict详解](http://blog.csdn.net/tianmohust/article/details/7621424)




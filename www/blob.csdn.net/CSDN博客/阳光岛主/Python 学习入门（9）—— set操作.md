
# Python 学习入门（9）—— set操作 - 阳光岛主 - CSDN博客

2013年11月19日 23:16:41[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：4129所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



**[python](http://www.python.org/)**的set和其他语言类似，是一个无序、不重复元素集，基本功能包括关系测试和消除重复元素. 集合对象还支持union(联合)，intersection(交)，difference(差)，sysmmetric_difference(对称差集)等数学运算.
sets 支持 x in set，len(set)，for x in set，作为一个无序的集合，sets不记录元素位置或者插入点。因此sets不支持 indexing, slicing, 或其它类序列（sequence-like）的操作。

```python
﻿#!/usr/bin/env python
# it-homer in 2013
import sys
reload(sys)
sys.setdefaultencoding('utf-8')
def test_set():
  x = set('ithomer')
  y = set(['i', 't', 'h', 's', 'n'])
  print("%s, %s" % (x, y))      # set(['e', 'i', 'h', 'm', 'o', 'r', 't']), set(['i', 'h', 's', 't', 'n'])
  # 交集
  print("%s" % (x&y))           # set(['i', 'h', 't'])
  f = x.intersection(y)
  print(f)                      # set(['i', 'h', 't'])
  # 并集
  print("%s" % (x|y))           # set(['e', 'i', 'h', 'm', 'o', 'n', 's', 'r', 't'])
  f = x.union(y)
  print(f)                      # set(['e', 'i', 'h', 'm', 'o', 'n', 's', 'r', 't'])
  # 差集
  print("%s" % (x-y))           # set(['m', 'r', 'e', 'o'])
  f = x.difference(y)
  print(f)                      # set(['m', 'r', 'e', 'o'])
  print("%s" % (y-x))           # set(['s', 'n'])
  f = y.difference(x)
  print(f)                      # set(['s', 'n'])
  # 对称差集（项在x或y中，但不会同时出现在二者中） 
  print("%s" %(x^y))            # set(['e', 'm', 'o', 'n', 's', 'r'])
  print("%s" %(y^x))            # set(['s', 'r', 'e', 'm', 'o', 'n'])
  f = x.symmetric_difference(y) # set(['e', 'm', 'o', 'n', 's', 'r'])
  print(f)
  f = y.symmetric_difference(x) # set(['s', 'r', 'e', 'm', 'o', 'n'])
  print(f)

  a = [11, 22, 33, 44, 11, 33, 22]
  b = set(a)
  print(b)              # set([33, 11, 44, 22])
  for i in b:
    print i,            # 不换行输出 : 33 11 44 22

def test_set2():
  s = set([1, 3, 5, 7, 9])
  t = set("ithomer")
  
  print(t)              # set(['e', 'i', 'h', 'm', 'o', 'r', 't'])
  # 并集  
  print("%s" % (s|t))   # set([1, 3, 'e', 5, 7, 'i', 9, 'm', 'o', 'r', 't', 'h'])
  # 交集
  print("%s" % (t&s))   # set([])
  # 差集
  print("%s" % (t-s))   # set(['e', 'i', 'h', 'm', 'o', 'r', 't'])
  print("%s" % (s-t))   # set([7, 1, 3, 5, 9])
  # 对称差集
  print("%s" % (t^s))   # set([1, 3, 'e', 5, 7, 'i', 9, 'm', 'o', 'r', 't', 'h'])

  print(t)              # set(['e', 'i', 'h', 'm', 'o', 'r', 't'])
  t.add('y')
  t.add('g')
  print(t)              # set(['e', 'g', 'i', 'h', 'm', 'o', 'r', 't', 'y'])

  try:
    print(len(t))       # 9
    t.remove('Y')
  except Exception, e:
    print e             # 'Y'
  print(len(t))         # 9
  t.remove('y')
  print(len(t))         # 8

def test_set3():
  s = set('ithomer')
  t = set('home')
  # 测试 x 是否是 s 的成员
  f = 'i' in s
  print(f)              # True
  # 测试 x 是否不是 s 的成员
  f = 'i' not in s
  print(f)              # False
  # 测试是否 t 中的每一个元素都在 s 中 
  f = t.issubset(s)
  print(f)              # True
  # 测试是否 t 中的每一个元素都在 s 中
  f = s.issuperset(t)
  print(f)              # True
  # 浅复制  
  print(s)              # set(['e', 'i', 'h', 'm', 'o', 'r', 't'])
  f = s.copy()
  print(f)              # set(['r', 'e', 't', 'i', 'h', 'm', 'o'])
  
  f = hash('i')
  print(f)

def test_set4():
  s = set('ithomer')
  t = set('home123')
  # s |= t
  s.update(t)       
  print(s)              # set(['e', 'i', 'h', 'm', 'o', '1', '3', 'r', 't', '2'])

  s = set('ithomer')
  # s &= t
  s.intersection_update(t)
  print(s)              # set(['h', 'e', 'm', 'o'])
  
  s = set('ithomer')
  # s -= t
  s.difference_update(t)
  print(s)              # set(['i', 'r', 't'])

  s = set('ithomer')
  # s ^= t
  s.symmetric_difference_update(t)
  print(s)              # set(['i', '1', '3', 'r', 't', '2'])

  s = set('ithomer')
  print(s)              # set(['e', 'i', 'h', 'm', 'o', 'r', 't'])
  s.add('I')
  print(s)              # set(['e', 'i', 'h', 'm', 'o', 'I', 'r', 't'])
  s.remove('i')          
  print(s)              # set(['e', 'h', 'm', 'o', 'I', 'r', 't'])
  s.discard('I')
  print(s)              # set(['e', 'h', 'm', 'o', 'r', 't'])
  s.pop()
  print(s)              # set(['h', 'm', 'o', 'r', 't'])
  s.clear()
  print(s)              # set([])

if __name__ == "__main__":
#  test_set()
#  test_set2()
  test_set3()
#  test_set4()
```



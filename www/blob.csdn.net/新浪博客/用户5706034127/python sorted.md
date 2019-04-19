# python sorted_用户5706034127_新浪博客
|||
我们需要对List进行排序，Python提供了两个方法
对给定的List L进行排序，
方法1.用List的成员函数sort进行排序
方法2.用built-in函数sorted进行排序（从2.4开始）
--------------------------------sorted---------------------------------------
>>> help(sorted)
Help on built-in function sorted in module
__builtin__:
sorted(...)
sorted(iterable, cmp=None, key=None, reverse=False) --> new
sorted list
---------------------------------sort----------------------------------------
>>> help(list.sort)
Help on method_descriptor:
sort(...)
L.sort(cmp=None, key=None, reverse=False) -- stable sort *IN
PLACE*;
 cmp(x, y)
-> -1, 0, 1
-----------------------------------------------------------------------------
iterable：是可迭代类型;
cmp：用于比较的函数，比较什么由key决定,有默认值，迭代集合中的一项;
key：用列表元素的某个属性和函数进行作为关键字，有默认值，迭代集合中的一项;
reverse：排序规则. reverse = True 或者 reverse =
False，有默认值。
返回值：是一个经过排序的可迭代类型，与iterable一样。
注；一般来说，cmp和key可以使用lambda表达式。
sort()与sorted()的不同在于，sort是在原位重新排列列表，而sorted()是产生一个新的列表。
## Sorting basic:
>>> print sorted([5, 2, 3, 1, 4])
[1, 2, 3, 4, 5]
>>> L = [5, 2, 3, 1, 4]
>>> L.sort()
>>> print L
[1, 2, 3, 4, 5]
## Sorting  cmp:
>>>L = [('b',2),('a',1),('c',3),('d',4)]
>>>print sorted(L,
cmp=lambda x,y:cmp(x[1],y[1]))
[('a', 1), ('b', 2), ('c', 3), ('d', 4)]
## Sorting  keys:
>>>L = [('b',2),('a',1),('c',3),('d',4)]
>>>print sorted(L,
key=lambda x:x[1]))
[('a', 1), ('b', 2), ('c', 3), ('d', 4)]
## Sorting  reverse:
>>> print sorted([5, 2, 3, 1, 4], reverse=True)
[5, 4, 3, 2, 1]
>>> print sorted([5, 2, 3, 1, 4], reverse=False)
[1, 2, 3, 4, 5]
注：效率key>cmp(key比cmp快)
在Sorting Keys中：我们看到，此时排序过的L是仅仅按照第二个关键字来排的，如果我们想用第二个关键字
排过序后再用第一个关键字进行排序呢?
>>> L = [('d',2),('a',4),('b',3),('c',2)]
>>> print sorted(L, key=lambda x:(x[1],x[0]))
>>>[('c', 2), ('d', 2), ('b', 3), ('a', 4)]

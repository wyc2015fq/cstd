# 你可能不知道的python




1.如何循环获得下标，使用 enumerate

```python
ints = ['a','b','c','d','e','f']

for idx, val in enumerate(ints):
    print idx, val
```

结果：

0 a
1 b
2 c
3 d
4 e
5 f

------------------------------------------------------------------------------------------

2.python  中的switch 替代语法，使用字典

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```python
xdef test(par):
    print par

def test1():
    print 1000

def test3(par):
    print par

def f(x):
    return {
        'a': test,
        'b': test1,
        }.get(x, test3)

print f('a')(100)
print f(4)(100)
```



结果：

100

100
------------------------------------------------------------------------------------------

3.如何在非创建全局变量的时候使用全局变量，使用global关键字

```python
globvar = 0

def test4():
    global globvar    
    globvar = 10

def test5():
    print globvar     

test4()
test5() 
```

结果为10

------------------------------------------------------------------------------------------

4.为什么是string.join(list)而不是list.join(string)

```python
my_list = ["Hello", "world"]
print "-".join(my_list)
#为什么不是 my_list.join("-") 。。。。这个....
```

因为所有可迭代对象都可以被连接，而不只是列表，但是连接者总是字符串

------------------------------------------------------------------------------------------

5.如何反向输出一个字符串？

```python
'test'[::-1]
```

------------------------------------------------------------------------------------------

6.如何随机生成大写字母和数字组成的字符串？

```python
import string, random
''.join(random.choice(string.ascii_uppercase + string.digits) for x in range(N))
```

------------------------------------------------------------------------------------------

7.如何将一个字符串转成字典？

使用内建模块 ast.literal_eval

```python
import ast
print ast.literal_eval("{'muffin' : 'lolz', 'foo' : 'kitty'}")
```

结果为：{'muffin': 'lolz', 'foo': 'kitty'}

------------------------------------------------------------------------------------------

8.如何填充字符串

```python
n = '4'
print n.zfill(3)
n = 4
print '%03d' % n
print "{0:03d}".format(4) # python >= 2.6
```

结果：

004
004
004

------------------------------------------------------------------------------------------

9.如何判断一个列表为空？

```python
if not a:
    print "List is empty"
#不要用len(a)来判断
```

------------------------------------------------------------------------------------------

10.列表append 和extend 的区别？

```python
>>> x = [1, 2]
>>> x.append(3)
>>> x
[1, 2, 3]
>>> x.append([4,5])
>>> x
[1, 2, 3, [4, 5]]
>>>
>>> x = [1, 2, 3]
>>> x.extend([4, 5])
>>> x
[1, 2, 3, 4, 5]
```

------------------------------------------------------------------------------------------

11. 如何随机的从列表中获得变量？

```python
foo = ['a', 'b', 'c', 'd', 'e']
from random import choice
print choice(foo)
```

------------------------------------------------------------------------------------------

12. 如何将列表切成长度相同的序列？

```python
def chunks(l, n):
    return [l[i:i+n] for i in range(0, len(l), n)]
```

------------------------------------------------------------------------------------------

13. 列表如何去重？

```python
def f7(seq):
　　seen = set()
　　seen_add = seen.add
　　return [ x for x in seq if x not in seen and not seen_add(x)]

print  f7([1,2,3,4,3,2,4,5,6,5,4,3])
```

结果：[1, 2, 3, 4, 5, 6]

------------------------------------------------------------------------------------------

14.遍历list的时候删除某些元素？

```python
#在原有对象上进行修改
somelist[:] = [x for x in somelist if determine(x)]
```

------------------------------------------------------------------------------------------

15.如何扁平一个二维数组？

```python
l = [[1,2,3],[4,5,6], [7], [8,9]]
变为[1, 2, 3, 4, 5, 6, 4, 5, 6, 7, 8, 9]
1.[item for sublist in l for item in sublist]


2.import itertools
list2d = [[1,2,3],[4,5,6], [7], [8,9]]
merged = list(itertools.chain(*list2d))
print merged

# python >= 2.6
import itertools
list2d = [[1,2,3],[4,5,6], [7], [8,9]]
merged = list(itertools.chain.from_iterable(list2d))

print merged

3.print sum(list2d,[])
```
------------------------------------------------------------------------------------------

16.如何获得list元素的下标？

```python
["foo","bar","baz"].index('bar')
1
```

------------------------------------------------------------------------------------------

17.如何将一个字典换成object

```python
def dict2obj(args):
    '把字典递归转化为类'
    class obj(object):
        def __init__(self, d):
            for a, b in d.items():
                if isinstance(b, (list, tuple)):
                    setattr(self, a, [obj(x) if isinstance(x, dict) else x for x in b])
                else:
                    setattr(self, a, obj(b) if isinstance(b, dict) else b)
    return obj(args)

d = {'a': 1, 'b': {'c': 2}, 'd': ["hi", {'foo': "bar"}]}

x = dict2obj(d)

print x.a
```

------------------------------------------------------------------------------------------

18.如何合并2字典？

```python
>>> x = {'a':1, 'b': 2}
>>> y = {'b':10, 'c': 11}
>>> z = x.update(y)
>>> print z
None
>>> x
{'a': 1, 'b': 10, 'c': 11}
```

我想要最终合并结果在z中，不是x，我要怎么做？

回答

这种情况下，可以使用

```python
z = dict(x.items() + y.items())
```

------------------------------------------------------------------------------------------

19.如何映射2列表成一个字典？

两个列表

```python
keys = ('name', 'age', 'food')
values = ('Monty', 42, 'spam')
```

如何得到

```python
dict = {'name' : 'Monty', 'age' : 42, 'food' : 'spam'}
```

使用zip

```python
>>> keys = ['a', 'b', 'c']
>>> values = [1, 2, 3]
>>> dictionary = dict(zip(keys, values))
>>> print dictionary
{'a': 1, 'b': 2, 'c': 3}
```

------------------------------------------------------------------------------------------

20.排序一个列表所有dict，根据dict的值

```python
lists = [{'name':'Homer', 'age':39}, {'name':'Bart', 'age':10},{'name':'Art', 'age':40}]

from operator import itemgetter
newlist = sorted(lists, key=itemgetter('name'))

print newlist
```

结果为：[{'age': 40, 'name': 'Art'}, {'age': 10, 'name': 'Bart'}, {'age': 39, 'name': 'Homer'}]

------------------------------------------------------------------------------------------

 
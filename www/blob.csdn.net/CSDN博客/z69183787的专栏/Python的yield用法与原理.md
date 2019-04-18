# Python的yield用法与原理 - z69183787的专栏 - CSDN博客
2016年08月16日 17:02:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1877
翻了一篇workflow上关于yield的用法，翻的有点烂，在这里贻笑大方了，慢慢来，总是期待着一点一点的进步。
为了理解yield的机制，我们需要理解什么是生成器。在此之前先介绍迭代器iterables。
## Iterables
当你创建一个list,你可以一个一个的获取，这种列表就称为迭代：
```
>>> mylist = [1, 2, 3]
>>> for i in mylist:
...    print(i)
1
2
3
```
Mylist 是一个迭代器. 当你理解它为一个list，它便是可迭代的:
```
>>> mylist = [x*x for x in range(3)]
>>> for i in mylist:
...    print(i)
0
1
4
```
任何可以用 for in 来迭代读取的都是迭代容器，例如lists,strings,files.这些迭代器非常的便利，因为你可以想取多少便取多少，但是你得存储所有的值，其中很多值都完全没有必要每次都保持在内存中。
## Generators
Generators(生成器)也是可迭代的，但是你每次只能迭代它们一次，因为不是所有的迭代器都被一直存储在内存中的，他们临时产生这些值：
```
>>> mygenerator = (x*x for x in range(3))
>>> for i in mygenerator:
...    print(i)
0
1
4
```
生成器几乎和迭代器是相同的，除了符号[]变为()。但是你无法用两次，因为他们只生成一次：他们生成0然后丢弃，继续统计1，接着是4，一个接着一个。
## Yield
Yield的用法有点像return,除了它返回的是一个生成器，例如：
```
>>> def createGenerator():
...    mylist = range(3)
...    for i in mylist:
...        yield i*i
...
>>> mygenerator = createGenerator() # create a generator
>>> print(mygenerator) # mygenerator is an object!
<generator object createGenerator at 0xb7555c34>
>>> for i in mygenerator:
...     print(i)
0
1
4
```
上面的例子几乎非常积累，但是它很好的阐释了yield的用法，我们可以知道createGenerator()生成的是一个生成器。
为了掌握yield的精髓，你一定要理解它的要点：当你调用这个函数的时候，你写在这个函数中的代码并没有真正的运行。这个函数仅仅只是返回一个生成器对象。有点过于奇技淫巧:-)
然后，你的代码会在每次for使用生成器的时候run起来。
现在是解释最难的地方：
当你的for第一次调用函数的时候，它生成一个生成器，并且在你的函数中运行该循环，知道它生成第一个值。然后每次调用都会运行循环并且返回下一个值，知道没有值返回为止。该生成器背认为是空的一旦该函数运行但是不再刀刀yield。之所以如此是因为该循环已经到达终点，或者是因为你再也不满足“if/else”的条件。
## Your code explained
例子：
生成器：
```
# 这里你创建一个node对象的一个生成器生成方法Here you create the method of the node object that will return the generator
def node._get_child_candidates(self, distance, min_dist, max_dist):
  # 这里是每次被调用的代码Here is the code that will be called each time you use the generator object:
    
  # 如果还有一个左孩子节点If there is still a child of the node object on its left
  # 并且距离可以，返回下一个孩子节点AND if distance is ok, return the next child
  if self._leftchild and distance - max_dist < self._median:
      yield self._leftchild
  # 如果还有一个右孩子几点If there is still a child of the node object on its right
  # 并且距离可以，返回下一个孩子节点AND if distance is ok, return the next child
  if self._rightchild and distance + max_dist >= self._median:
      yield self._rightchild
  # 如果方法运行到这里，生成器会被认为为空If the function arrives here, the generator will be considered empty
  # there is no more than two values: the left and the right children
```
调用者:
```
# 创建一个空的列表Create an empty list and a list with the current object reference
result, candidates = list(), [self]
# 循环candidates列表,只有一个元素。Loop on candidates (they contain only one element at the beginning)
while candidates:
    # Get the last candidate and remove it from the list
    node = candidates.pop()
    # Get the distance between obj and the candidate
    distance = node._get_dist(obj)
    # If distance is ok, then you can fill the result
    if distance <= max_dist and distance >= min_dist:
        result.extend(node._values)
    # Add the children of the candidate in the candidates list
    # so the loop will keep running until it will have looked
    # at all the children of the children of the children, etc. of the candidate
    candidates.extend(node._get_child_candidates(distance, min_dist, max_dist))
return result
```
这段代码包含一些非常机智的部分：
    1. list的循环迭代部分，但是list在循环的同时又在拓展，：）这种方法是一种循环内嵌式的数据的相对简洁的方法，但是又存在着一些风险可能会导致死循环的情况。在这个例子当中，
```
candidates.extend(node._get_child_candidates(distance,
 min_dist, max_dist))
```
 耗尽所有的的生成器的值，但是当保持生成新的生成器对象，并且依据之前生成器产生许多不同的值，由于它产生于不同的节点。
    2. extend()方法是一个list 对象方法，它产生一个迭代器并且添加它的值到list当中去。
通常我们
```
>>> a = [1, 2]
>>> b = [3, 4]
>>> a.extend(b)
>>> print(a)
[1, 2, 3, 4]
```
但是代码中获得一个生成器，这种方式比较好的原因如下：
首先是你无须读取该值两次。
然后你不需要把所有的值都放在内存中。
与此同时，它能够owrk的原因是[Python](http://lib.csdn.net/base/11)不关心一个方法的参数石佛是一个list.期待是一个迭代器所以它能够适用于strings,lists,tuples以及生成器。这被称为动态类型或者鸭子类型（duck
 typing）是python 如此酷的一大原因。鸭子类型又是另外一个问题了，blablabla。
现在让我们来看看一些高级的用法：
控制生成器资源消耗：
```
>>> class Bank(): # let's create a bank, building ATMs
...    crisis = False
...    def create_atm(self):
...        while not self.crisis:
...            yield "$100"
>>> hsbc = Bank() # when everything's ok the ATM gives you as much as you want
>>> corner_street_atm = hsbc.create_atm()
>>> print(corner_street_atm.next())
$100
>>> print(corner_street_atm.next())
$100
>>> print([corner_street_atm.next() for cash in range(5)])
['$100', '$100', '$100', '$100', '$100']
>>> hsbc.crisis = True # crisis is coming, no more money!
>>> print(corner_street_atm.next())
<type 'exceptions.StopIteration'>
>>> wall_street_atm = hsbc.create_atm() # it's even true for new ATMs
>>> print(wall_street_atm.next())
<type 'exceptions.StopIteration'>
>>> hsbc.crisis = False # trouble is, even post-crisis the ATM remains empty
>>> print(corner_street_atm.next())
<type 'exceptions.StopIteration'>
>>> brand_new_atm = hsbc.create_atm() # build a new one to get back in business
>>> for cash in brand_new_atm:
...    print cash
$100
$100
$100
$100
$100
$100
$100
$100
$100
...
```
这一个非常的有用，特别是类似的资源访问控制。
## Itertools模块
Itertools模块包含一些特别的函数去执行迭代器。有没有想过去复制一个生成器 或者链接两个生成器?等等。
引入itertools就好了，import itertools.
下面举个例子.看看四匹马到达先后顺序的例子：
```
>>> horses = [1, 2, 3, 4]
>>> races = itertools.permutations(horses)
>>> print(races)
<itertools.permutations object at 0xb754f1dc>
>>> print(list(itertools.permutations(horses)))
[(1, 2, 3, 4),
 (1, 2, 4, 3),
 (1, 3, 2, 4),
 (1, 3, 4, 2),
 (1, 4, 2, 3),
 (1, 4, 3, 2),
 (2, 1, 3, 4),
 (2, 1, 4, 3),
 (2, 3, 1, 4),
 (2, 3, 4, 1),
 (2, 4, 1, 3),
 (2, 4, 3, 1),
 (3, 1, 2, 4),
 (3, 1, 4, 2),
 (3, 2, 1, 4),
 (3, 2, 4, 1),
 (3, 4, 1, 2),
 (3, 4, 2, 1),
 (4, 1, 2, 3),
 (4, 1, 3, 2),
 (4, 2, 1, 3),
 (4, 2, 3, 1),
 (4, 3, 1, 2),
 (4, 3, 2, 1)]
```
## 最后是理解迭代器的内部机制：
Iteration is a process implying iterables (implementing the `__iter__()` method)
 and iterators (implementing the `__next__()` method).
 Iterables are any objects you can get an iterator from. Iterators are objects that let you iterate on iterables.
更多的相关内容可以阅读 [循环如何工作](http://effbot.org/zone/python-for-statement.htm).

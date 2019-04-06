#  			[Python进阶内容（四）--- 迭代器(Iterator)与生成器(Generator)](https://www.cnblogs.com/echo1937/p/7614989.html) 		



## 迭代器

我们已经知道，可以直接作用于`for`循环的数据类型有以下几种：

- 一类是集合数据类型，如`list`、`tuple`、`dict`、`set`、`str`等；
- 一类是`generator`，包括生成器和带`yield`的generator function。

这些可以直接作用于`for`循环的对象统称为**可迭代对象：Iterable**。可以被`next()`函数调用并不断返回下一个值的对象称为**迭代器：Iterator**。

> 注意：在Python3中，next（Iterator）实际上调用的是Iterator.__next__()，在表述上可以认为两者等价。next()是内建函数，__next__()是Iterator的方法。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
# 可以使用isinstance()判断一个对象是否是Iterable对象：

>>> from collections import Iterable
>>> isinstance([], Iterable)
True
>>> isinstance({}, Iterable)
True
>>> isinstance('abc', Iterable)
True
>>> isinstance((x for x in range(10)), Iterable)
True
>>> isinstance(100, Iterable)
False

# 可以使用isinstance()判断一个对象是否是Iterator对象：

>>> from collections import Iterator
>>> isinstance((x for x in range(10)), Iterator)
True
>>> isinstance([], Iterator)
False
>>> isinstance({}, Iterator)
False
>>> isinstance('abc', Iterator)
False

# 把list、dict、str等Iterable变成Iterator可以使用iter()函数：

>>> isinstance(iter([]), Iterator)
True
>>> isinstance(iter('abc'), Iterator)
True
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

> Q：为什么`list`、tuple、`dict`、set、`str`等数据类型不是`Iterator`？
>
> A：Python的`Iterator`对象表示的是一个数据流，Iterator对象可以被`next()`函数调用并不断返回下一个数据，直到没有数据时抛出`StopIteration异常`。非要类比的话，赌场发牌的荷官算是一个比较接近的例子。本来你需要自己去处理一堆牌（list、tuple、dict等iterable），现在你有了这个对象，只要不断问他要“下一张”，他要是有自然会给你，没有就结束（StopIteration）。

### iter 函数

Python提供了一个`iter`函数用来生成迭代器。这个方法有两个参数，当只传入一个参数的时候：

- 若这个参数是一个`容器`，则返回这个容器的`迭代器对象`，
- 若这个参数本身就是一个`迭代器`，则返回其自身，

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
In [1]: alist = [1, 2, 3, 4]
In [2]: it = iter(alist)

In [3]: it
Out[3]: <list_iterator at 0x102496e10>

In [4]: it2 = iter(it)

In [5]: id(it) == id(it2)
Out[5]: True
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

### iterator 的特点

迭代器都有一个__`next__`方法，调用了__next__方法之后，迭代指针会指向下一个元素的位置，若下一个元素没有了，则会抛出`StopIteration`异常。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
In [09]: alist = [1, 2, 3, 4]
In [10]: it = iter(alist)

In [11]: it.__next__()
Out[11]: 1

In [12]: it.__next__()
Out[12]: 2

In [13]: next(it)  # 也可以使用内建函数next(iterator)完成这一功能
Out[13]: 3

In [14]: next(it)
Out[14]: 4

In [15]: next(it)
---------------------------------------------------------------------------
StopIteration                             Traceback (most recent call last)
<ipython-input-15-2cdb14c0d4d6> in <module>()
----> 1 next(it)

StopIteration: 

In [16]: 
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

了解了这些情况以后，我们就能使用迭代器进行遍历了。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
it = iter([1, 2, 3, 4])
try:
    while True:
        val = it.__next__()
        print(val)
except StopIteration:
    pass

# 实际上由于迭代操作如此普遍，Python专门将关键字for用作了迭代器的语法糖。
# 在for循环中，Python将自动调用工厂函数iter()获得迭代器，自动调用__next__()获取元素，还完成了检查StopIteration异常的工作。
# 上述代码可以写成如下的形式，你一定非常熟悉：
for val in [1, 2, 3, 4]:
    print(val)

# 首先Python将对关键字in后的对象调用iter函数获取迭代器，然后调用迭代器的next方法获取元素，直到抛出StopIteration异常。
# 对迭代器调用iter函数时将返回迭代器自身，所以迭代器也可以用于for语句中，不需要特殊处理。
# 常用的几个内建数据结构tuple、list、set、dict都支持迭代器，字符串string也可以使用迭代操作。
for val in iter([1, 2, 3, 4]):
    print(val)
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

使用迭代器的循环可以避开索引，但有时候我们还是需要索引来进行一些操作的。这时候内建函数enumerate就派上用场，用起来就像这样：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
for idx in enumerate(['a','b','c','d']):
    print(idx)

# 输出结果    
(0, 'a')
(1, 'b')
(2, 'c')
(3, 'd')
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

### iterator 的定义

对于上面的 it 这个迭代器，是通过 iter方法实现的，那么iter函数到底做了什么呢？**简而言之，实现了迭代器协议的对象就是迭代器**。什么是迭代器协议呢？再简而言之，满足下面两个条件即可：

- 实现 `__iter__()方法`，返回一个迭代对象，
- 实现 __`next__()` 方法，返回当前的元素，并指向下一个元素的位置，当前位置已经没有元素的时候，抛出`StopIteration`异常。

> 注意：内建函数iter和__iter__方法两者的关系，类似于内建函数next和__next__方法之间的关系。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
# 下面我们实现一个迭代器对象，要求可以进行逆序迭代。

class ReverseList:
    def __init__(self, item):
        self.list = item

    def __iter__(self):
        return self

    def __next__(self):
        try:
            return self.list.pop()
        except IndexError:
            raise StopIteration


if __name__ == "__main__":

    it = ReverseList([1, 2, 3, 4])
    for i in it:
        print(i)

# 输出结果
4
3
2
1
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

下面的例子用到了 迭代器迭代后销毁元素 这一性质，

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
# 以下代码演示了如何利用zip函数和迭代器对列表中的元素进行分组

def group_adjacent(x, k):
    result = [iter(x)] * k  # result = [<list_iterator at 0x10efc7be0>, <list_iterator at 0x10efc7be0>, <list_iterator at 0x10efc7be0>]
    return zip(*result)     # 解包 包含三个迭代器的列表，并利用zip函数进行归类，从而返回一个可迭代对象zip


alist = ["foo", 2, "bar", 4, "far", 6]
for inx in group_adjacent(alist, 3):
    print(inx)

# 输出结果
('foo', 2, 'bar')
(4, 'far', 6)
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

### 迭代器(Iterator)小结

- 凡是可作用于`for...in`循环的对象都是`Iterable`类型；
- 凡是可作用于__`next__()`函数的对象都是`Iterator`类型，表示一个惰性计算的序列；
- Python中`for...in`循环本质上是通过不断调用__`next__()`函数实现的，可以视作迭代器的语法糖
- 集合数据类型如`list`、`dict`、`str`等是`Iterable`但不是`Iterator`，可以通过`iter()`函数获得一个`Iterator`对象。

迭代器不要求你事先准备好整个迭代过程中所有的元素。迭代器仅仅在迭代至某个元素时才计算该元素，而在这之前或之后，元素可以不存在或者被销毁。这个特点使得它特别适合用于遍历一些巨大的或是无限的集合，比如几个G的文件，或是斐波那契数列等等。这个特点被称为**延迟计算**或**惰性求值(Lazy evaluation)**。

## 生成器

**生成器本质上就是一个迭代器**，它有和迭代器一样的特性，唯一的区别在于实现方式上不一样，生成器更加简洁。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
# 一种方式是 生成器表达式(Generator expression)

l = [x * x for x in range(10)]        # 列表解析(List Comprehension)
g = (x * x for x in range(10))        # 生成器表达式(Generator expression)

print(l)                              # 输出：[0, 1, 4, 9, 16, 25, 36, 49, 64, 81]
print(g)                              # 输出：<generator object <genexpr> at 0x10e7a0a40>

In [20]: g.__next__()
Out[20]: 0

In [21]: g.__next__()
Out[21]: 1

In [22]: g.__next__()
Out[22]: 4

In [23]: g.__next__()
Out[23]: 9

In [24]: g.__next__()
Out[24]: 16

In [25]: l.__next__()
---------------------------------------------------------------------------
AttributeError Traceback (most recent call last)
<ipython-input-25-f891a9c694f5> in <module>()
----> 1 l.__next__()

AttributeError: 'list' object has no attribute '__next__'


# 另一种方式是generator function，如果一个函数定义中包含yield关键字，那么这个函数就不再是一个普通函数，而是一个generator。
# 普通函数是顺序执行，遇到return语句，或者执行完所有函数块内的语句就返回。
# generator function是在每次调用__next__()的时候执行，遇到yield语句返回，再次执行时从上次返回的yield语句处继续执行。

In [27]: def get_square(n):
    ...:     for x in range(n):
    ...:         yield x ** 2      # 每次迭代时返回 x ** 2，然后挂起
    ...:

In [28]: f = get_square(10)

In [29]: f
Out[29]: <generator object get_square at 0x10ea477d8>

In [30]: f.__next__()
Out[30]: 0

In [31]: f.__next__()
Out[31]: 1

In [32]: f.__next__()
Out[32]: 4
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

我们来看一个例子，使用生成器返回自然数的平方（注意返回的是多个值），

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
# 使用普通函数
def gensquares(N):
    res = []
    for i in range(N):
        res.append(i*i)
    return res

for item in gensquares(5):
    print(item)


# 使用生成器函数
def gensquares(N):
    for i in range(N):
        yield i ** 2

for item in gensquares(5):
    print(item)

# 通过对比发现，使用生成器代码更少，惰性计算也使得程序性能更好
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

## 生成器小结

Python有两种不同的方式提供生成器:

- 生成器表达式：类似于列表解析，但是它们返回按需产生结果的一个对象，而不是构建一个结果列表
- 生成器函数：编写为常规的def语句，但是使用yield语句一次返回一个结果，在每个结果之间挂起和继续它们的状态

生成器在协程(coroutine)中发挥了重要作用，协程(coroutine)一般来说是指这样的函数：

- 彼此间有不同的局部变量、指令指针，但仍共享全局变量；
- 可以方便地挂起、恢复，并且有多个入口点和出口点；
- 多个协同程序间表现为协作运行，如A的运行过程中需要B的结果才能继续执行。

我们将在后面的内容中继续详细介绍yield和协程的内容.

## 额外补充：

Python内置了一个模块itertools，包含了很多函数用于creating iterators for efficient looping（创建更有效率的循环迭代器），详见官方文档。

 

 

 

 

 

 

 

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
class Pipe:
    def __init__(self, func):
        self.func = func

    def __ror__(self, other):
        print(other)

        def generator():
            for obj in other:
                if obj is not None:
                    yield self.func(obj)

        return generator()


@Pipe
def even_filter(num):
    return num if num % 2 == 0 else None


@Pipe
def multiply_by_three(num):
    return num * 3


@Pipe
def convert_to_string(num):
    return 'The Number: %s' % num


@Pipe
def echo(item):
    print(item)
    return item


def force(sqs):
    for item in sqs: pass


nums = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

if __name__ == "__main__":
    force(nums | even_filter | multiply_by_three | convert_to_string | echo)
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

推荐阅读：

- [函数式编程](https://coolshell.cn/articles/10822.html)

 
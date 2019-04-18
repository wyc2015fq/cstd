# python容器类、迭代器和生成器 - Orisun - 博客园







# [python容器类、迭代器和生成器](https://www.cnblogs.com/zhangchaoyang/articles/4967794.html)





__getitem__   __setitem__

```
# coding:utf-8

class Father(object):

    def __init__(self):
        self.name = "father"
        self.children = ['child1', 'child2', 'child3']

    def __getitem__(self, i):
        assert i < len(self.children)
        return self.children[i]

    def __setitem__(self, i, obj):
        assert i < len(self.children)
        self.children[i] = obj

if __name__ == '__main__':
    f=Father()
    print f[1]
    f[1]='orisun'
    print f[1]
```

输出：

```
child2
orisun
```

当类中有一个属性是序列时，如果该类中定义了__getitem__和__setitem__，那么就可以通过obj[index]的方式来访问和设置序列中的元素。

## __iter__   迭代器

如果一个类想被用于`for ... in ... `循环，类似list或tuple那样，就必须实现一个`__iter__()`方法，该方法返回一个迭代对象，然后，Python的for循环就会不断调用该迭代对象的`next()`方法拿到循环的下一个值，直到遇到StopIteration错误时退出循环。

```
# coding:utf-8


class Iterable(object):
    num = 0

    def next(self):
        self.num += 1
        if self.num > 5:
            raise StopIteration()
        return self.num


class MyCollection(object):

    def __init__(self):
        self.iter = Iterable()

    def __iter__(self):
        return self.iter

if __name__ == '__main__':
    for ele in MyCollection():
        print ele

    print '*'*5

    itr=iter(MyCollection())
    while True:
        try:
            print itr.next()
        except StopIteration:
            pass
```

输出：

```
1
2
3
4
5
*****
1
2
3
4
5
```

把一个对象传入iter()就等价于调用这个对象的__iter__方法，接着我们就查以调用__iter__返回的对象的next()方法。跟使用for ...  in ... 不同，我们显示调用next()方法时，需要自己处理StopIteration异常。

__iter__()返回的对象就是迭代器，或者说迭代器实质上就是一个有next()方法的对象。

```
iter(func, sentinel)
```

如果是传递两个参数给 iter() , 它会重复地调用 func , 直到迭代器的下个值等于sentinel 。

除iter()可以返回迭代器外，内建函数reversed() 、 enumerate() 也可以返回迭代器，其中reversed()允许反向访问序列。另外python的dict类有3个生成迭代器的函数：iteritems()、iterkeys()、itervalues()。

我们可以让__iter__()返回对象本身，此时该对象必须实现next()方法，比如下面的这个斐波那契类。

```
# coding:utf-8


class Fib(object):

    def __init__(self):
        self.a, self.b = 0, 1  # 初始化两个计数器a，b

    def __iter__(self):
        return self  # 实例本身就是迭代对象，故返回自己

    def next(self):
        self.a, self.b = self.b, self.a + self.b  # 计算下一个值
        if self.a > 30:  # 退出循环的条件
            raise StopIteration()
        return self.a  # 返回下一个值

if __name__ == '__main__':
    for ele in Fib():
        print ele
```

输出：

```
1
1
2
3
5
8
13
21
```

##  生成器

与迭代器具有相同功能的是生成器，生成器代码写起来更加简洁。一个函数能能够连续yield一个对象它就是生成器。

```
# coding:utf-8

def fib():
    a, b = 0, 1
    while True:
        a, b = b, a + b
        yield a

if __name__ == '__main__':
    for ele in fib():
        if ele > 30:
            break
        else:
            print ele
```

输出：

```
1
1
2
3
5
8
13
21
```

每次执行完yield时生成器会停下来，当for ... in ...想获取下一个元素时生成器才会接着yield继续执行，换句话说yield是“延迟生产”或“按需生产”。

当生成器退出或抛出StopIteration时，for ... in ...循环退出。













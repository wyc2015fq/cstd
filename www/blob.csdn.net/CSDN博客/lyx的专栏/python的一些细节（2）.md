# python的一些细节（2） - lyx的专栏 - CSDN博客





2017年05月27日 17:17:01[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：374
所属专栏：[和python在一起的日子](https://blog.csdn.net/column/details/16842.html)









想想自己写了这么久的python，其实基础的东西还是不扎实，重新学习一下廖雪峰老师的教程，有很多之前未知或者有疑惑的东西得到了解答。

1.字符编码问题

ASCII编码是1个字节，而Unicode编码通常是2个字节，utf-8则是1-6个字节。同时utf-8中对英文字母的编码就是ASCII码。
python中u"XXX"代表的是这个字符串是Unicode编码的，而"XXX"则是utf-8编码。所以，对于中文，出现编码问题的时候，往往是没有用好utf-8与unicode之间的转换。u"XXX".encode("utf-8"),将unicode转成utf-8

"XXX".decode("utf_8")，将utf-8编码成unicode。


2.dict类型如何同时遍历键值对





 d = {'x': 'A', 'y': 'B', 'z': 'C' }

for k, v in d.iteritems():

...  print k, '=', v

dict的iteritems方法可以同时遍历key和value



3.生成器的两种方法

之前，我们可以用列表生成的方法简化很多工作，譬如[str(s) for s in list_temp]，这样返回的就是一个list。如果我们把外面的[]改成()，就会生成生成器，生成器和list的最大的不同就是前者是一边计算一边出结果，而后者是一下子把结果算好，显然会占用较大的空间

还有一种方法就是用函数来生成迭代器，而这个时候，函数就是迭代器的名称，可以有next方法。具体就是把return改成yield，这样，每次next就会从上一个yield处开始继续往下运行。



```python
def fib(max):
    n, a, b = 0, 0, 1
    while n < max:
        yield b
        a, b = b, a + b
        n = n + 1
```
4.redue方法

map方法以前就用过，其实和list生成也没什么特别大的区别，但是reduce方法更加神奇。reduce把一个函数作用在一个序列[x1, x2, x3...]上，这个函数必须接收两个参数，reduce把结果继续和序列的下一个元素做累积计算，其效果就是

reduce(f, [x1, x2, x3, x4]) = f(f(f(x1, x2), x3), x4)

reduce中的第一个函数必须是两个参数。

5.filter

和map()类似，filter()也接收一个函数和一个序列。和map()不同的时，filter()把传入的函数依次作用于每个元素，然后根据返回值是True还是False决定保留还是丢弃该元素。



```python
def is_odd(n):
    return n % 2 == 1


filter(is_odd, [1, 2, 4, 5, 6, 9, 10, 15])
```
这样返回的list就会只剩下奇数。很好用的方法，以后过滤list再也不用遍历了。

6.sorted

与filter一样，自己定制化一个排序函数：

通常规定，对于两个元素x和y，如果认为x < y，则返回-1，如果认为x == y，则返回0，如果认为x > y，则返回1，这样，排序算法就不用关心具体的比较过程，而是根据比较结果直接排序。


编写好这样的函数之后，就可以使用了。其实说白了就是自定义了一种比较大小的方式。

7.闭包

当一个函数返回了一个函数后，其内部的局部变量还被新函数引用，所以，闭包用起来简单，实现起来可不容易。

8.装饰器

就是把函数作为一个参数传入到@的语法糖后面，然后@后面的这个function是用来返回一个function的，最后调用这个返回的function

9.frozen

其实就是参数冻结的技术，之前似乎在R还是scikitlearn里面见到过。

所以，简单总结functools.partial的作用就是，把一个函数的某些参数给固定住（也就是设置默认值），返回一个新的函数，调用这个新函数会更简单。、


10.python3

在Python 3.x中，所有的除法都是精确除法，地板除用//表示

11.isinstance

isinstance('a', (str, unicode))，可以判断前面这个字符串类型的或者。


12.@property

我们在对实例属性操作的时候，就知道该属性很可能不是直接暴露的，而是通过getter和setter方法来实现的。






```python
class Student(object):

    @property
    def birth(self):
        return self._birth

    @birth.setter
    def birth(self, value):
        self._birth = value

    @property
    def age(self):
        return 2014 - self._birth
```
而你用property来用也是可以的，而且，这个时候，获得属性和修改属性，之间像访问成员变量一样就可以了，而不是访问方法。



13.print

print其实调用了一个对象的__str__ ()方法，在窗口中直接回车则调用__repr__()方法，而在for in这样的循环中，其实调用的是__iter__()方法，该方法返回一个迭代对象，然后，Python的for循环就会不断调用该迭代对象的next()方法拿到循环的下一个值，直到遇到StopIteration错误时退出循环。

14.函数和类

函数和类的鉴定其实是可以很模糊的。只要类实现了__call__()方法，就可以像函数一样来调用。







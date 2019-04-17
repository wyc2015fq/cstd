# python的这10道面试题，70%的会出错 - zhusongziye的博客 - CSDN博客





2018年01月15日 21:29:55[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：469









第一题：下面哪个单词不是Python3里面的关键字

```
a) eval
b) assert
c) nonlocal
d) pass
```


这道题选a，所谓关键字就是 Python 预先保留的标识符，每个关键字都有特殊的含义，不能作为变量名，通过 keyword 可以查看所有关键字，eval 是Python中的一个内建函数。

```
>>> import keyword
>>> keyword.kwlist
['False', 'None', 'True', ...]
```

### 第二题：表达式`3*1**3`的值是多少

```
a) 27
b) 9
c) 3
d) 1
```


这道题选c，考察的是运算符优先级，指数运算具有最高优先级,下图运算符优先级从高到低

![](https://img-blog.csdn.net/20180115212429064?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


### 第三题：下面两个表达式输出内容是什么？

```
>>> a = 1
>>> b = 1
>>> a is b
???
>>> a = 300
>>> b = 300
>>> a is b
???

a) True True
b) True False
c) False False
d) False True
```


这道题选b，考察的是 Python 中两个整数对象的比较，“is” 比较对象内存地址，只有两个对象都指向相同的地址时才返回True，“==” 比较对象的值，值相等的两个对象内存地址不一定相同。值为300的两个整数在内存中是两个不同的对象，介于[-5,256]之间的整数，值相同的整数都会共享同一个对象，所以不管内存中出现了多少个数值为1的对象，都表示同一个对象。

### 第四题：下面这个函数的返回值是什么？

```
def func(a):
    a = a + '2'
    a = a*2
    return a
>>>func("hello")

a) hello
b) 字符串不支持*操作
c) hello2
d) hello2hello2
```


选d，这道题考察是操作符重载的概念，在Python中，操作符支持重载，也就是说不仅数字支持“+”操作，列表也支持这些操作，因为列表对象重载了这两个操作符。自定义类也可以重载这些操作符，例如：

```
class Point(object):
    def __init__(self, x: int, y: int):
        self.x = x
        self.y = y

    def __repr__(self):
        return "Point(x={}, y={})".format(self.x, self.y)

    def __add__(self, other):
        return Point(self.x + other.x, self.y + other.y)

    def __mul__(self, other):
        return Point(self.x * other.x, self.y * other.y)

>>> p1 = Point(1, 3)
>>> p2 = Point(2, 4)
>>> p1 + p2
Point(x=3, y=7)
>>> p1*p2
Point(x=2, y=12)
```

### 第五题：在Python中，表达式 0.1 + 0.2 == 0.3 的返回是？

```
a) True
b) False
c) 不确定
```


选b，在Python中，数值对象都是用二进制来表示的，浮点数也不例外，但不是所有的浮点数都能用二进制精确表示的。一个浮点数转化为二进制就是是不断的乘2，取其中的整数部分，例如：

```
(1) 0.1*2 = 0.2, 整数部分为0，小数部分为0.2 
(2) 0.2*2 = 0.4 , 整数部分为0，小数部分为0.4 
(3) 0.4*2 = 0.8 , 整数部分为0，小数部分为0.8
(4) 0.8*2 = 1.6, 整数部分为1，小数部分为0.6
(5) 0.6*2 = 1.2, 整数部分为1，小数部分为0.2
重复第2步
```


所以0.1的二进制表示就是0.0 0011 0011 0011…，计算机没法精确表示这个浮点数，所有就造成了误差。

### 第六题：表达式 ~~~5 的值是多少？

```
a) +5
b) -11
c) +11
d) -5
```


“~”是按位取反操作符，就是对整数的二进制位进行取反，把1变为0，0变为1，例如5的二进制表示为00000101，按位取反是 111111010，这串二进制在计算机里面以补码的形式存储，我们需要把它转换为原码才知道它代表的真实数值是多少。因为这个补码的最高位为1，所以他是一个负数，负数从补码转换为原码是符号位保留为1，其它各位取反，再加1，所有 111111010 取反是 100000101，加1转换成了原码 100000110，转换为10进制整数就是-6（最高位为1表示负数），取三次反跟取一次是一样的。所以这道题的答案是-6。

### 第七题：表达式 bool(‘False’) 的返回值是：

```
a) True
b) False
c) 0
d) 1
```


选a，这道题考察的是对象的真假值，”False”在这里是一个字符串，只要不是空字符串，bool返回的结果就是True，下面是真假值对照表。

![](https://img-blog.csdn.net/20180115212504273?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


### 第八题：表达式 True==False==False 的返回值是：

```
a) True
b) False
c) 0
d) 1
```


选b，链式比较中，True==False==False 等价于 (True==False) and (False==False)

### 第九题：下面表达式输出结果为：

```
i = 0
while i < 5:
    print(i)
    i += 1
    if i == 3:
        break
else:
    print(0)

a) 0 1 2 0
b) 0 1 2
c) 0 1
d) 0 1 2 3
```


选b，这道题考察的是while … break …else 的执行逻辑，只要执行了break就不会执行else，反之亦然，没有执行break就一定会执行else。

### 第十题：下面表达式输出结果为：

```
x = 12

def f1():
    x = 3
    print(x)

def f2():
    x += 1
    print(x)

f1()
f2()

a) 3 4
b) 3 13
c) 12 13
d) 3 报错
```


选d，考察的是变量的作用域概念，模块中x是全局变量，函数中的x是局部变量




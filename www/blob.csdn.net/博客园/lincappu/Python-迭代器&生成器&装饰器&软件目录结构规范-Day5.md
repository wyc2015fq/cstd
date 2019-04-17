# Python-迭代器&生成器&装饰器&软件目录结构规范-Day5 - lincappu - 博客园







# [Python-迭代器&生成器&装饰器&软件目录结构规范-Day5](https://www.cnblogs.com/lincappu/p/8157513.html)





目录
Day-Python-迭代器&生成器 2
1、生成器 2
1.1、生成器引入 2
1.2、生成器作用 3
1.3、创建生成器的方法 3
1.4、用函数来实现复杂的生成器 5
1.5、把函数变成生成器通过yield来实现 6
1.6、函数定义中包含yield关键字==generator生成器 8
1.7、通过yield实现单线程并发运算效果 9
2、迭代器 10
2.1、迭代器的定义 10
2.2、inter()函数-将列表、字典、字符串转换成迭代器 11
2.3、小结 12
3、装饰器 12
3.1、装饰器-最初需求 12
3.2、装饰器-添加认证需求： 13
3.3、开发规范 14
3.4、高阶函数引出 14
3.4、添加认证后代码 15
3.5、以上的添加会更改其他用户的调用方式 15
3.6、解决触发认证-定义一层函数 16
3.7、对于新增功能-添加参数 17
3.8、装饰器小结 18
4、软件目录结构规范 18
4.1、为什么要设计好目录结构? 18
4.2、项目目录举例 19
4.3、关于README的内容 19
4.4、setup.py环境部署 20
4.5、requirements.txt环境依赖 20
4.6、关于配置文件的使用方法 21

## 1、生成器

### 1.1、生成器引入

通过列表生成式，我们可以直接创建一个列表。但是，受到内存限制，列表容量肯定是有限的。而且，创建一个包含100万个元素的列表，不仅占用很大的存储空间，如果我们仅仅需要访问前面几个元素，那后面绝大多数元素占用的空间都白白浪费了。

所以，如果列表元素可以按照某种算法推算出来，那我们是否可以在循环的过程中不断推算出后续的元素呢？这样就不必创建完整的list，从而节省大量的空间。在Python中，这种一边循环一边计算的机制，称为生成器（Generator）。

> 
我现在有个需求，看列表[0, 1, 2, 3, 4, 5, 6, 7, 8, 9],我要求你把列表里的每个值加1，你怎么实现？你可能会想到2种方式


![](http://i.imgur.com/fmPBrCk.png)

列表生成式

```
>>> a
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
>>> b = []
>>> for i in a:b.append(i+1)
... 
>>> b
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>> a = b
>>> a
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

>>> a
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>> a = map(lambda x:x+1, a)
>>> a
<map object at 0x101d2c630>
>>> for i in a:print(i)
... 
2
3
4
5
6
7
8
9
10
11
```

其实还有一种写法，如下

```python
>>> a = [i+1 for i in range(10)]
>>> a
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
```

这就叫做列表生成

### 1.2、生成器作用

作用：边运算边生成

通过列表生成式，我们可以直接创建一个列表。但是，受到内存限制，列表容量肯定是有限的。而且，创建一个包含100万个元素的列表，不仅占用很大的存储空间，如果我们仅仅需要访问前面几个元素，那后面绝大多数元素占用的空间都白白浪费了。

所以，如果列表元素可以按照某种算法推算出来，那我们是否可以在循环的过程中不断推算出后续的元素呢？这样就不必创建完整的list，从而节省大量的空间。在Python中，这种一边循环一边计算的机制，称为生成器：generator。

### 1.3、创建生成器的方法

要创建一个generator，有很多种方法。第一种方法很简单，只要把一个列表生成式的[]改成()，就创建了一个generator：

```python
>>> L = [x * x for x in range(10)]
>>> L
[0, 1, 4, 9, 16, 25, 36, 49, 64, 81]
>>> g = (x * x for x in range(10))
>>> g
<generator object <genexpr> at 0x1022ef630>
```

创建L和g的区别仅在于最外层的[]和()，L是一个list，而g是一个generator。

我们可以直接打印出list的每一个元素，但我们怎么打印出generator的每一个元素呢？

如果要一个一个打印出来，可以通过next()函数获得generator的下一个返回值：

```
>>> next(g)
0
>>> next(g)
1
>>> next(g)
4
>>> next(g)
9
>>> next(g)
16
>>> next(g)
25
>>> next(g)
36
>>> next(g)
49
>>> next(g)
64
>>> next(g)
81
>>> next(g)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
StopIteration
```

generator保存的是算法，每次调用next(g)，就计算出g的下一个元素的值，直到计算到最后一个元素，没有更多的元素时，抛出StopIteration的错误。

当然，上面这种不断调用next(g)实在是太变态了，正确的方法是使用for循环，因为generator也是可迭代对象：

```python
>>> g = (x * x for x in range(10))
>>> for n in g:
...     print(n)
...
0
1
4
9
16
25
36
49
64
81
```

所以，我们创建了一个generator后，基本上永远不会调用next()，而是通过for循环来迭代它，并且不需要关心StopIteration的错误。

### 1.4、用函数来实现复杂的生成器

generator非常强大。如果推算的算法比较复杂，用类似列表生成式的for循环无法实现的时候，还可以用函数来实现。

比如，著名的斐波拉契数列（Fibonacci），除第一个和第二个数外，任意一个数都可由前两个数相加得到：
`1, 1, 2, 3, 5, 8, 13, 21, 34, ...`
斐波拉契数列用列表生成式写不出来，但是，用函数把它打印出来却很容易：

```python
def fib(max):
    n, a, b = 0, 0, 1
    while n < max:
        print(b)
        a, b = b, a + b
        n = n + 1
    return 'done'

注意，赋值语句：
    
a, b = b, a + b

相当于：
    
t = (b, a + b) # t是一个tuple
a = t[0]
b = t[1]
```

但不必显式写出临时变量t就可以赋值。

上面的函数可以输出斐波那契数列的前N个数：

```
>>> fib(10)
1
1
2
3
5
8
13
21
34
55
done
```

仔细观察，可以看出，fib函数实际上是定义了斐波拉契数列的推算规则，可以从第一个元素开始，推算出后续任意的元素，这种逻辑其实非常类似generator。

### 1.5、把函数变成生成器通过yield来实现

函数和generator仅一步之遥。要把fib函数变成generator，只需要把print(b)改为yield b就可以了：

```python
def fib(max):
    n,a,b = 0,0,1

    while n < max:
        #print(b)
        yield  b 
        a,b = b,a+b

        n += 1

    return 'done'
```

![](http://i.imgur.com/XicBV1Y.jpg)

> 
yield a#返回a,同事挂起当前这个函数，a返回给了通过__next__()调用当前函数通过yiel就实现了函数的中断，并且保存了函数的中间状态
send()可以传递yield表达式的值进去，而next()不能传递特定的值，只能传递None进去


更简单的写法

```
>>> a= 3
>>> b =6
>>> a,b=b,a
>>> a
6
>>> b
3
>>> a =3
>>> b = 6
>>> a,b=b,b+a
>>> a
6
>>> b
9
>>>
```

### 1.6、函数定义中包含yield关键字==generator生成器

这就是定义generator的另一种方法。如果一个函数定义中包含yield关键字，那么这个函数就不再是一个普通函数，而是一个generator：

```
>>> f = fib(6)
>>> f
<generator object fib at 0x104feaaa0>
```

这里，最难理解的就是generator和函数的执行流程不一样。函数是顺序执行，遇到return语句或者最后一行函数语句就返回。而变成generator的函数，在每次调用next()的时候执行，遇到yield语句返回，再次执行时从上次返回的yield语句处继续执行。

```
data = fib(10)
print(data)

print(data.__next__())
print(data.__next__())
print("干点别的事")
print(data.__next__())
print(data.__next__())
print(data.__next__())
print(data.__next__())
print(data.__next__())
```

输出

```
<generator object fib at 0x101be02b0>
1
1
干点别的事
2
3
5
8
13
```

在上面fib的例子，我们在循环过程中不断调用yield，就会不断中断。当然要给循环设置一个条件来退出循环，不然就会产生一个无限数列出来。

同样的，把函数改成generator后，我们基本上从来不会用next()来获取下一个返回值，而是直接使用for循环来迭代：

```python
>>> for n in fib(6):
...     print(n)
...
1
1
2
3
5
8
```

但是用for循环调用generator时，发现拿不到generator的return语句的返回值。如果想要拿到返回值，必须捕获StopIteration错误，返回值包含在StopIteration的value中：

```python
>>> g = fib(6)
>>> while True:
...     try:
...         x = next(g)
...         print('g:', x)
...     except StopIteration as e:
...         print('Generator return value:', e.value)
...         break
...
g: 1
g: 1
g: 2
g: 3
g: 5
g: 8
Generator return value: done
```

### 1.7、通过yield实现单线程并发运算效果

还可通过yield实现在单线程的情况下实现并发运算的效果

通过生成器实现协程并行运算　

```python
#_*_coding:utf-8_*_
__author__ = 'Alex Li'

import time
def consumer(name):
    print("%s 准备吃包子啦!" %name)
    while True:
       baozi = yield

       print("包子[%s]来了,被[%s]吃了!" %(baozi,name))


def producer(name):
    c = consumer('A')
    c2 = consumer('B')
    c.__next__()
    c2.__next__()
    print("老子开始准备做包子啦!")
    for i in range(10):
        time.sleep(1)
        print("做了2个包子!")
        c.send(i)
        c2.send(i)

producer("alex")
```

## 2、迭代器

### 2.1、迭代器的定义

可以直接作用于for循环的数据类型有以下几种：

一类是集合数据类型，如list、tuple、dict、set、str等；

一类是generator，包括生成器和带yield的generator function。

这些可以直接作用于for循环的对象统称为可迭代对象：Iterable。

可以使用isinstance()判断一个对象是否是Iterable对象：

```python
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
```

而生成器不但可以作用于for循环，还可以被next()函数不断调用并返回下一个值，直到最后抛出StopIteration错误表示无法继续返回下一个值了。

#### 可以被next()函数调用并不断返回下一个值的对象称为迭代器：Iterator。

#### 生成器就是迭代器，但是迭代器不一定都是生成器

可以使用isinstance()判断一个对象是否是Iterator对象：

```python
>>> from collections import Iterator
>>> isinstance((x for x in range(10)), Iterator)
True
>>> isinstance([], Iterator)
False
>>> isinstance({}, Iterator)
False
>>> isinstance('abc', Iterator)
False
```

生成器都是Iterator对象，但list、dict、str虽然是Iterable，却不是Iterator。

### 2.2、inter()函数-将列表、字典、字符串转换成迭代器

把list、dict、str等Iterable变成Iterator可以使用iter()函数：

```
>>> isinstance(iter([]), Iterator)
True
>>> isinstance(iter('abc'), Iterator)
True
```

你可能会问，为什么list、dict、str等数据类型不是Iterator？

这是因为Python的Iterator对象表示的是一个数据流，Iterator对象可以被next()函数调用并不断返回下一个数据，直到没有数据时抛出StopIteration错误。可以把这个数据流看做是一个有序序列，但我们却不能提前知道序列的长度，只能不断通过next()函数实现按需计算下一个数据，所以Iterator的计算是惰性的，只有在需要返回下一个数据时它才会计算。

Iterator甚至可以表示一个无限大的数据流，例如全体自然数。而使用list是永远不可能存储全体自然数的。

### 2.3、小结
- 
凡是可作用于for循环的对象都是Iterable类型；

- 
凡是可作用于next()函数的对象都是Iterator类型，它们表示一个惰性计算的序列；

- 
集合数据类型如list、dict、str等是Iterable但不是Iterator，不过可以通过iter()函数获得一个Iterator对象。


Python的for循环本质上就是通过不断调用next()函数实现的，例如：
for x in [1, 2, 3, 4, 5]:
pass

实际上完全等价于：

```python
# 首先获得Iterator对象:
it = iter([1, 2, 3, 4, 5])
# 循环:
while True:
    try:
        # 获得下一个值:
        x = next(it)
    except StopIteration:
        # 遇到StopIteration就退出循环
        break
```



## 3、装饰器

装饰器是一个很著名的设计模式，经常被用于有切面需求的场景，较为经典的有插入日志、性能测试、事务处理等。装饰器是解决这类问题的绝佳设计，有了装饰器，我们就可以抽离出大量函数中与函数功能本身无关的雷同代码并继续重用。概括的讲，装饰器的作用就是为已经存在的对象添加额外的功能。

### 3.1、装饰器-最初需求

你是一家视频网站的后端开发工程师，你们网站有以下几个版块

```python
def home():
    print("---首页----")
 
def america():
    print("----欧美专区----")
 
def japan():
    print("----日韩专区----")
 
def henan():
    print("----河南专区----")
```

### 3.2、装饰器-添加认证需求：

```python
#_*_coding:utf-8_*_
 
 
user_status = False #用户登录了就把这个改成True
 
def login():
    _username = "alex" #假装这是DB里存的用户信息
    _password = "abc!23" #假装这是DB里存的用户信息
    global user_status
 
    if user_status == False:
        username = input("user:")
        password = input("pasword:")
 
        if username == _username and password == _password:
            print("welcome login....")
            user_status = True
        else:
            print("wrong username or password!")
    else:
        print("用户已登录，验证通过...")
 
def home():
    print("---首页----")
 
def america():
    login() #执行前加上验证
    print("----欧美专区----")
 
def japan():
    print("----日韩专区----")
 
def henan():
    login() #执行前加上验证
    print("----河南专区----")
 
 
 
home()
america()
henan()
```

### 3.3、开发规范

```
封闭：已实现的功能代码块
开放：对扩展开发
```

### 3.4、高阶函数引出

高阶函数，就是把一个函数当做一个参数传给另外一个函数，直接 把这个功能 的函数名当做一个参数 传给 我的验证模块就行了

```python
#_*_coding:utf-8_*_
 
 
user_status = False #用户登录了就把这个改成True
 
def login(func): #把要执行的模块从这里传进来
    _username = "alex" #假装这是DB里存的用户信息
    _password = "abc!23" #假装这是DB里存的用户信息
    global user_status
 
    if user_status == False:
        username = input("user:")
        password = input("pasword:")
 
        if username == _username and password == _password:
            print("welcome login....")
            user_status = True
        else:
            print("wrong username or password!")
 
    if user_status == True:
        func() # 看这里看这里，只要验证通过了，就调用相应功能
 
def home():
    print("---首页----")
 
def america():
    #login() #执行前加上验证
    print("----欧美专区----")
 
def japan():
    print("----日韩专区----")
 
def henan():
    #login() #执行前加上验证
    print("----河南专区----")
 
home()
login(america) #需要验证就调用 login，把需要验证的功能 当做一个参数传给login
# home()
# america()
login(henan)
```

### 3.5、添加认证后代码

```cpp
home()
login(america) #需要验证就调用 login，把需要验证的功能 当做一个参数传给login
# home()
# america()
login(henan)
```

### 3.6、以上的添加会更改其他用户的调用方式

你之所改变了调用方式，是因为用户每次调用时需要执行login(henan)，类似的。其实稍一改就可以了呀

home()
america = login(america)
henan = login(henan)

这样你，其它人调用henan时，其实相当于调用了login(henan), 通过login里的验证后，就会自动调用henan功能。

home()
america = login(america) #你在这里相当于把america这个函数替换了
henan = login(henan)

那用户调用时依然写
america()
但是还是有问题，引用会直接触发认证。

### 3.7、解决触发认证-定义一层函数

开始你写的america = login(america)不触发你函数的执行，只需要在这个login里面再定义一层函数，第一次调用america = login(america)只调用到外层login，这个login虽然会执行，但不会触发认证了，因为认证的所有代码被封装在login里层的新定义 的函数里了，login只返回 里层函数的函数名，这样下次再执行america()时， 就会调用里层函数啦。。。

def login(func): #把要执行的模块从这里传进来

```python
def inner():#再定义一层函数
    _username = "alex" #假装这是DB里存的用户信息
    _password = "abc!23" #假装这是DB里存的用户信息
    global user_status

    if user_status == False:
        username = input("user:")
        password = input("pasword:")

        if username == _username and password == _password:
            print("welcome login....")
            user_status = True
        else:
            print("wrong username or password!")

    if user_status == True:
        func() # 看这里看这里，只要验证通过了，就调用相应功能

return inner #用户调用login时，只会返回inner的内存地址，下次再调用时加上()才会执行inner函数
```

america = login(america) #你在这里相当于把america这个函数替换了

只在你要装饰的函数上面加上下面代码

```python
@login  
def america():  
    #login() #执行前加上验证  
    print("----欧美专区----")
 
def japan():  
    print("----日韩专区----")  
 
@login    
def henan():  
    #login() #执行前加上验证  
    print("----河南专区----")
```

效果是一样的。

### 3.8、对于新增功能-添加参数

[对新增加的@login](mailto:%E5%AF%B9%E6%96%B0%E5%A2%9E%E5%8A%A0%E7%9A%84@login)()还可以添加参数*args,**kwargs...

![](http://i.imgur.com/Gge5PNa.jpg)

### 3.9、装饰器小结

装饰器：
完全符合 开放-封闭原则，
不改变原有功能代码，不改变原有调用方式
实现扩展新功能

## 4、软件目录结构规范

### 4.1、为什么要设计好目录结构?

> - 可读性高: 不熟悉这个项目的代码的人，一眼就能看懂目录结构，知道程序启动脚本是哪个，测试目录在哪儿，配置文件在哪儿等等。从而非常快速的了解这个项目。

- 可维护性高: 定义好组织规则后，维护者就能很明确地知道，新增的哪个文件和代码应该放在什么目录之下。这个好处是，随着时间的推移，代码/配置的规模增加，项目结构不会混乱，仍然能够组织良好。

### 4.2、项目目录举例

假设你的项目名为foo, 我比较建议的最方便快捷目录结构这样就足够了:

```
Foo/
|-- bin/
|   |-- foo
|
|-- foo/
|   |-- tests/
|   |   |-- __init__.py
|   |   |-- test_main.py
|   |
|   |-- __init__.py
|   |-- main.py
|
|-- docs/
|   |-- conf.py
|   |-- abc.rst
|
|-- setup.py
|-- requirements.txt
|-- README
```

简要解释一下:

```
bin/: 存放项目的一些可执行文件，当然你可以起名script/之类的也行。
foo/: 存放项目的所有源代码。
(1) 源代码中的所有模块、包都应该放在此目录。不要置于顶层目录。
(2) 其子目录tests/存放单元测试代码； 
(3) 程序的入口最好命名为main.py。
docs/: 存放一些文档。
setup.py: 安装、部署、打包的脚本。
requirements.txt: 存放软件依赖的外部Python包列表。
README: 项目说明文件。
```

### 4.3、关于README的内容

这个我觉得是每个项目都应该有的一个文件，目的是能简要描述该项目的信息，让读者快速了解这个项目。

它需要说明以下几个事项:

```
软件定位，软件的基本功能。
运行代码的方法: 安装环境、启动命令等。
简要的使用说明。
代码目录结构说明，更详细点可以说明软件的基本原理。
常见问题说明。
```

我觉得有以上几点是比较好的一个README。在软件开发初期，由于开发过程中以上内容可能不明确或者发生变化，并不是一定要在一开始就将所有信息都补全。但是在项目完结的时候，是需要撰写这样的一个文档的。

可以参考Redis源码中Readme的写法，这里面简洁但是清晰的描述了Redis功能和源码结构。
关于requirements.txt和setup.py

### 4.4、setup.py环境部署

一般来说，用setup.py来管理代码的打包、安装、部署问题。业界标准的写法是用Python流行的打包工具setuptools来管理这些事情。这种方式普遍应用于开源项目中。不过这里的核心思想不是用标准化的工具来解决这些问题，而是说，一个项目一定要有一个安装部署工具，能快速便捷的在一台新机器上将环境装好、代码部署好和将程序运行起来。

setup.py可以将这些事情自动化起来，提高效率、减少出错的概率。"复杂的东西自动化，能自动化的东西一定要自动化。"是一个非常好的习惯。

setuptools的文档比较庞大，刚接触的话，可能不太好找到切入点。学习技术的方式就是看他人是怎么用的，可以参考一下Python的一个Web框架，flask是如何写的: setup.py

当然，简单点自己写个安装脚本（deploy.sh）替代setup.py也未尝不可。

### 4.5、requirements.txt环境依赖

这个文件存在的目的是:

```
方便开发者维护软件的包依赖。
将开发过程中新增的包添加进这个列表中，
避免在setup.py安装依赖时漏掉软件包。
方便读者明确项目使用了哪些Python包。
```

这个文件的格式是每一行包含一个包依赖的说明，通常是flask>=0.10这种格式，要求是这个格式能被pip识别，这样就可以简单的通过 pip install -r requirements.txt来把所有Python包依赖都装好了。

### 4.6、关于配置文件的使用方法

注意，在上面的目录结构中，没有将conf.py放在源码目录下，而是放在docs/目录下。

配置的使用，更好的方式是，

```
模块的配置都是可以灵活配置的，不受外部配置文件的影响。
程序的配置也是可以灵活控制的。
```

能够佐证这个思想的是，用过nginx和mysql的同学都知道，nginx、mysql这些程序都可以自由的指定用户配置。

所以，不应当在代码中直接import conf来使用配置文件。上面目录结构中的conf.py，是给出的一个配置样例，不是在写死在程序中直接引用的配置文件。可以通过给main.py启动参数指定配置路径的方式来让程序读取配置内容。当然，这里的conf.py你可以换个类似的名字，比如settings.py。或者你也可以使用其他格式的内容来编写配置文件，比如settings.yaml之类的。













# python学习之函数 - 刘炫320的博客 - CSDN博客
2017年11月30日 14:32:45[刘炫320](https://me.csdn.net/qq_35082030)阅读数：138标签：[python																[函数																[lambda表达式																[递归																[汉诺塔](https://so.csdn.net/so/search/s.do?q=汉诺塔&t=blog)](https://so.csdn.net/so/search/s.do?q=递归&t=blog)](https://so.csdn.net/so/search/s.do?q=lambda表达式&t=blog)](https://so.csdn.net/so/search/s.do?q=函数&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[python](https://blog.csdn.net/qq_35082030/article/category/6780197)
对于函数的了解还是不够透彻，毕竟有很多小细节需要注意。最简单的部分我们不再赘述，主要讲一下需要注意的几点。
# 1.函数参数
## 1.1函数参数种类
函数参数主要分为3种：正常参数，默认参数，可变长参数。 
所谓正常参数，就是我们常规意义上的参数，就和f(x)一样，没什么区别。例如：
```python
def power(x, n):
    s = 1
    while n > 0:
        n = n - 1
        s = s * x
    return s
```
而默认参数则是指具有默认值的参数。例如：
```python
def power(x, n=2):
    s = 1
    while n > 0:
        n = n - 1
        s = s * x
    return s
```
可以看到和正常参数不同的是，默认参数n有了初试值2，那么再进行调用的时候，就可以这样调用：
```python
>>> power(5)
25
>>> power(5, 2)
25
```
这两种效果是等同的。但是在默认参数传递时，需要注意2点： 
1. 默认参数必须是在参数的最后，不然的话，计算机并不知道你接下来的参数是默认参数还是正常参数。 
2. 默认参数一定不能够在函数中发生改变，否则的话会非常奇怪。例如：
```python
def add_end(L=[]):
    L.append('END')
    return L
>>> add_end()
['END']
>>> add_end()
['END', 'END']
>>> add_end()
['END', 'END', 'END']
```
Python函数在定义的时候，默认参数L的值就被计算出来了，即[]，因为默认参数L也是一个变量，它指向对象[]，每次调用该函数，如果改变了L的内容，则下次调用时，默认参数的内容就变了，不再是函数定义时的[]了。
如果一定要避免这个情况的话，必须这么写（虽然这真的是为了这么写而这么写，不建议）：
```python
def add_end(L=None):
    if L is None:
        L = []
    L.append('END')
    return L
```
第三个可变长的参数分为2种，一种是`*t`，另一种是`**d`。
`*t`参数是元组参数，接受任意多个实参并将其放在一个元组中。 
例如：
```python
def myvar1(*t):
    print(t)
myvar1(1,2,3)
myvar1(1,2,3,4,5)
#程序输出结果如下：
#(1, 2, 3)
#(1, 2, 3, 4, 5)
```
`**d`参数则是字典参数，可以接受任意多个实参。实参的形式为：关键字=实参值。在字典可变长度参数中，关键字参数和实参值参数被放入一个字典，分别作为字典的关键字和字典的值。
```python
def myvar2(**d):
    print(t)
myvar2(x=1,y=2,z=3)
myvar2(name='bren',age=25)
#程序输出结果如下：
#{'y': 2, 'x': 1, 'z': 3}
#{'age': 25, 'name': 'bren'}
```
## 1.2另外一个需要注意的地方就是参数传递。
参数传递也分为2种，一种是简单类型，一种是简单类型不影响函数外的变量，而集合类型（列表、字典）是会受影响的，就和Java等引用传递是一样的。
例如：
```python
#第一种传递方式
def exchange(num1,num2):
    num1,num2=num2,num1
    print(num1, num2)
num1=5
num2=7              #运行结果
print(num1,num2)    #5 7
exchange(num1,num2) #7 5
print(num1,num2)    #5 7
#第二种传递方式
def change(lst,dict):
    lst[0]=10
    dict['a']=10
    print('Inside lst = {}, dict = {}'.format(lst,dict))
dict={'a':1,'b':2,'c':3}
lst=[1,2,3,4,5]
print('Before lst = {}, dict = {}'.format(lst,dict))
change(lst,dict)
print('After lst = {}, dict = {}'.format(lst,dict))
#程序运行结果如下：
#Before lst = [1, 2, 3, 4, 5], dict = {'c': 3, 'a': 1, 'b': 2}
#Inside lst = [10, 2, 3, 4, 5], dict = {'c': 3, 'a': 10, 'b': 2}
#After lst = [10, 2, 3, 4, 5], dict = {'c': 3, 'a': 10, 'b': 2}
```
# 2.lambda表达式与函数对象
lambda表达式作为在任何一种高级编程语言中必须出现的高级编程方法，其神秘感一直围绕着它。它简而言之就是一个匿名函数。一般作为参数传递给一些特殊的方法，比如排序。而它的样子基本上也是大同小异，从定义上看`lambda [参数1[,参数2,……,参数n]]:表达式`，例如：
```python
lambda x,y:x+y
```
当然python是一个函数为基础的面向函数编程的语言，因此它可以更加任性：
```python
f=lambda x,y:x+y
f(5,10) #15
```
同样的匿名函数也可以作为序列或字典的元素：
```python
f=[lambda x,y:x+y,lambda x,y:x-y]
print(f[0](3,5),f[1](3,5))#8 -2
f={'a':lambda x,y:x+y,'b':lambda x,y:x-y}
f['a'](3,4)#7
f['b'](3,4)#-1
```
这正是它神奇的地方。
# 3. 递归
递归分为直接递归和间接递归，直接递归就是函数A调用了函数A，而间接递归就是函数A调用了函数B，但是函数B又重新调用了函数A。无论是直接调用还是间接调用，都需要在A和B中设立终止条件，也就是说递归总要有结束的时候。
使用递归函数需要注意防止栈溢出。在计算机中，函数调用是通过栈（stack）这种数据结构实现的，每当进入一个函数调用，栈就会加一层栈帧，每当函数返回，栈就会减一层栈帧。由于栈的大小不是无限的，所以，递归调用的次数过多，会导致栈溢出。
解决递归调用栈溢出的方法是通过尾递归优化，事实上尾递归和循环的效果是一样的，所以，把循环看成是一种特殊的尾递归函数也是可以的。
尾递归是指，在函数返回的时候，调用自身本身，并且，return语句不能包含表达式。这样，编译器或者解释器就可以把尾递归做优化，使递归本身无论调用多少次，都只占用一个栈帧，不会出现栈溢出的情况。
然而可惜的是，大多数编程语言没有针对尾递归做优化，Python解释器也没有做优化，所以，即使把上面的fact(n)函数改成尾递归方式，也会导致栈溢出。
（想进行优化，还是老实的递归转循环吧！）
最后我们使用[经典的汉诺塔问题](http://dmego.me/2016/10/16/hanoi.html)来理解一下递归：
```python
def move(n,a='a',b='b',c='c'):
    if n==1:
        print('move',a,'>>',c)#从A把1个物体移动到C
    else:
        move(n-1,a,c,b)#把这n-1个物体借助C转到B上
        move(1,a,b,c)#把A的最后一个转到C上。
        move(n-1,b,a,c)#把B上的n-1个物体通过A转到C上。
#执行
move(3, 'A', 'B', 'C')
```
# 4. 装饰器
装饰器函数在python里有着完美的支持，比装饰器模式（设计模式）要高级很多，它真的是在函数上再装饰一下。装饰器本质上是一个Python函数，它可以让其他函数在不需要做任何代码变动的前提下增加额外功能，装饰器的返回值也是一个函数对象。它经常用于有切面需求的场景，比如：插入日志、性能测试、事务处理、缓存、权限校验等场景。装饰器是解决这类问题的绝佳设计，有了装饰器，我们就可以抽离出大量与函数功能本身无关的雷同代码并继续重用它。最基本的样子是如下：
```python
def debug(func):
    def wrapper():
        print "[DEBUG]: enter {}()".format(func.__name__)
        return func()
    return wrapper
@debug
def say_hello():
    print "hello!"
```
这里有一个@语法糖，[关于语法糖的那些事](https://www.zhihu.com/question/57470958)我们这里就不一一介绍，大家可以自己看。我们这里只介绍我们这个@debug。
这个是什么意思呢？其实这句话相当于debug(say_hello())，然后可以看到在函数内部还有一个`return func()`这样就起到了装饰的作用。
而如果是使用有参数的装饰器呢？只需要指定装饰器函数wrapper接受和原函数一样的参数就可以了。
```python
def debug(func):
    def wrapper(something):  # 指定一毛一样的参数
        print "[DEBUG]: enter {}()".format(func.__name__)
        return func(something)
    return wrapper  # 返回包装过函数
@debug
def say(something):
    print "hello {}!".format(something)
```
但如果更通用的话，可能需要用到我们刚刚讲的可变参数了。
```python
def debug(func):
    def wrapper(*args, **kwargs):  # 指定宇宙无敌参数
        print "[DEBUG]: enter {}()".format(func.__name__)
        print 'Prepare and say...',
        return func(*args, **kwargs)
    return wrapper  # 返回
@debug
def say(something):
    print "hello {}!".format(something)
```
当然这些只是装饰器最简单的用法，更高的用法，比如带参数的装饰器和基于类实现的装饰器以及内置装饰器等等，则请移步[装饰器全解指南](https://www.cnblogs.com/cicaday/p/python-decorator.html)。
另外装饰器是可以叠加使用的，那么这是就涉及到装饰器调用顺序了。对于Python中的”@”语法糖，装饰器的调用顺序与使用 @ 语法糖声明的顺序相反。
# 5. 全局变量与nonlocal关键字
pyhon中的全局变量如果在函数内部改变的话，就是用global关键字即可，例如：
```python
def f():
global x #说明x为全局变量
x=30
y=40 #定义局部变量y
print("No2:",x,y)
x=10 #定义全局变量x
y=20 #定义全局变量y
print("No1:",x,y)   #No1: 10 20
f()                 #No2: 30 40
print("No3:",x,y)   #No3: 30 20
```
而如果不是一个全局变量怎么办，则需要使用关键字nonlocal了，例如：
```python
def make_counter(): 
    count = 0 
    def counter(): 
        nonlocal count 
        count += 1 
        return count 
    return counter 
def make_counter_test(): 
    mc = make_counter() 
    print(mc())   #1
    print(mc())   #2
    print(mc())   #3
make_counter_test()
```
总而言之global关键字用来在函数或其他局部作用域中使用全局变量。nonlocal关键字用来在函数或其他作用域中使用外层(非全局)变量。这个为以后的闭包做准备，python引用变量的顺序： 当前作用域局部变量->外层作用域变量->当前模块中的全局变量->python内置变量 。
# 6. 函数的类型
在教学过程中，初学者最常见的问题，就是不知道函数使用者是否发生了改变，什么时候应该赋值，什么时候不用赋值。是不是没有返回值的就不用赋值？ 
其实无论是内置函数，还是自定义函数，类型只有两种，一种是原地操作，一种是行为操作。
比如list.sort()则是原地操作，它执行后的结果是改变了list本身。而sorted(X)函数则是通过原对象X创造出了一个新的对象，保留X不变。
具体而言：sorted 和list.sort 都接受key, reverse定制。但是区别是 
1. list.sort()是列表中的方法，只能用于列表；sorted可以用于任何可迭代的对象。 
2. list.sort()是在原序列上进行修改，不会产生新的序列；sorted() 会返回一个新的序列，旧的对象依然存在。 
具体不同的用法为：[sorted()和list.sort()的区别](https://www.cnblogs.com/freemao/p/3869994.html)。
今天有关函数的部分就讲到这里，其实还有很多没有讲到，我们将会在高阶函数中继续讲解。

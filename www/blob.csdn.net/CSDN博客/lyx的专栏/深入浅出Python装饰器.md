# 深入浅出Python装饰器 - lyx的专栏 - CSDN博客





2016年05月24日 22:30:18[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：434标签：[python																[修饰器](https://so.csdn.net/so/search/s.do?q=修饰器&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[Python](https://blog.csdn.net/qtlyx/article/category/5969093)

所属专栏：[和python在一起的日子](https://blog.csdn.net/column/details/16842.html)










python有一种机制，叫做装饰器。顾名思义，就是在你写的函数运行之前做一点装饰。

先来看第一个，内部函数。

1.内部函数



```python
def fun():
    def inter_fun():
        x=9
        return "x is big"
    return inter_fun

f= fun()
```
这是一个很简单的例子，想想我们调用函数fun（）的时候会发生什么？对，就是会返回内部的inter_fun函数，这个函数作用是什么呢？就是返回“x is big”这个字符串。所以，f()这个函数的输出就是inter_fun这个函数的输出。

2.制造函数的函数

上面的内部函数，其实就是造出来了一个函数，当然上面的例子是没有意义的。但是试想，如果我们对某个函数要做先前的一些设置，而又不希望这些甚至的参数都作为一个函数的参数。因为那么多参数，势必是的核心函数有很多判断语句，维护起来很麻烦。所以我们就用一个制造函数的方法，来制造出想要的函数。上面的例子，输出的是“x is big”，如果我们和5比，确实x大，但是和10比较呢，所以我们要有一个标准，这个标准可以作为一个函数的参数。



```python
def funGenerator(benchmark):
    def inter_fun(x):
        if x >benchmark:
            return "x ix big"
        else:
            return "x is small"
    return inter_fun

f= funGenerator(10)
```


看一这个函数，funGenerator()。这个函数有一个参数，为基准（benchmark），返回值是一个函数。返回的函数f，如果传入f（5）则会返回x is small，因为这里benchmark为10.也就是说，我们构造了一个函数，并且用这个函数构造了一个基准为10的函数。

可以说是构造函数的函数，也可以说，我们对这个函数做了一个修饰，修饰的结果就是装上了benchmark。

3.关于变量生存周期

其实这里有一个问题，如果我们仔细考虑一下编程语言中的变量作用域的问题。

函数调用上面的inter_fun()结束之后，按照道理会释放空间，同理，调用funGenerator也是一样，但是我们看到，在后面调用f的时候，当初的benchmark值是被保留下来的。python中的内部函数就有这样的特性，当内部函数调用上一级的变量，而且该内部函数被自身作为返回值返回后，会形成“闭包”。所谓闭包，就是之前的那个上级变量，这里是benchmark被保留在函数对象的__closure__中。可以自己print f.__closure__看一下哦，发现就是一个地址，这个地址存的就是之前benchmark局部变量的地址。由于还有引用，所以块局部区域不会被释放掉。

4.装饰器



```python
def deco(fun):
    def inter_deco(x):
        if  isinstance(x,int):
            return fun(x)
        else:
            return "input value error"
    return inter_deco
@deco
def fun(x):
    if x >20:
        return "big"
    else:
        return "small"

print fun(90)
```


先抛开上面的代码，话说，我有一个函数，用来判断x是不是大于20，但是有一个问题，我怎么才能知道，我传入的参数有没有错呢？万一不是int怎么办呢？

根据制造函数的函数，那么我们就用一个函数来制造一个当传入不是int的时候返回错误输入的函数。




```python
def funGenerator(fun):
    def inter_fun(x):
        if isinstance(x,int):
            return fun(x)
        else:
            return "input error"
    return inter_fun

def fun(x):
    if x >20:
        return "big"
    else:
        return "small"
```


这里呢，f=funGenerator(fun)，把fun作为funGenerator的参数，然后，利用返回值f就可以了。仔细想一想，其实我们这里是对fun函数做了一种修饰是不是，所以python中特地简化出来了，上面的代码等价于一开始的用@deco的代码，当编译器发现@deco后，会去调用deco函数，然后把他的返回值作为被修饰函数。需要好好体会一下这里的逻辑关系。

假设你是编译器，发现fun函数上面有修饰器的代码，所以就去调用修饰器函数，返回inter_deco之后，把函数句柄交给小面的fun。这个时候的fun其实就是内部函数。当然，当内部函数调用fun函数的时候，使用的就是其原本的内部逻辑。











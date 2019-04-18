# python装饰器–原来如此简单 - Nicole的博客 - CSDN博客
2018年04月17日 11:18:41[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：3657
# python装饰器–原来如此简单
今天整理装饰器，内嵌的装饰器、让装饰器带参数等多种形式，非常复杂，让人头疼不已。但是突然间发现了装饰器的奥秘，原来如此简单。。。。
```python
# -*- coding:gbk -*-
'''示例1: 使用语法糖@来装饰函数，相当于“myfunc = deco(myfunc)”
但发现新函数只在第一次被调用，且原函数多调用了一次'''
def deco(func):
    print("before myfunc() called.")
    func()
    print("  after myfunc() called.")
    return func
@deco
def myfunc():
    print(" myfunc() called.")
myfunc()
myfunc()
```
这是一个最简单的装饰器的例子，但是这里有一个问题，就是当我们两次调用myfunc()的时候，发现装饰器函数只被调用了一次。为什么会这样呢？要解释这个就要给出破解装饰器的关键钥匙了。 
**这里@deco这一句，和myfunc = deco(myfunc)其实是完全等价的，只不过是换了一种写法而已**
一定要记住上面这句！！！！ 
好了，从现在开始，只需要做替换操作就可以了。 
将@deco 替换为 myfunc = deco(myfunc) 
程序首先调用deco(myfunc)，得到的返回结果赋值给了myfunc （注意：在Python中函数名只是个指向函数首地址的函数指针而已） 
而deco(myfunc)的返回值就是函数myfunc()的地址 
这样其实myfunc 没有变化，也就是说，最后的两次myfunc()函数调用，其实都没有执行到deco()。 
有同学就问了，明明打印了deco()函数里面的内容啊，怎么说没有调用到呢。这位同学一看就是没有注意听讲，那一次打印是在@deco 这一句被执行的。大家亲自动手试一下就会发现” myfunc() called.”  这句打印输出了三次。多的那次就是@deco这里输出的，因为@deco 等价于myfunc = deco(myfunc)，这里已经调用了deco()函数了。
## 第二步 ：确保装饰器被调用
怎么解决装饰器没有被调用的问题呢
```python
# -*- coding:gbk -*-
'''示例2: 使用内嵌包装函数来确保每次新函数都被调用，
内嵌包装函数的形参和返回值与原函数相同，装饰函数返回内嵌包装函数对象'''
def deco(func):
    def _deco():
        print("before myfunc() called.")
        func()
        print("  after myfunc() called.")
        # 不需要返回func，实际上应返回原函数的返回值
    return _deco
@deco
def myfunc():
    print(" myfunc() called.")
    return 'ok'
myfunc()
myfunc()
```
这里其实不需要我解释了，还是按照第一步中的方法做替换就可以了。还是啰嗦几句吧。。 
@deco 替换为 myfunc = deco(myfunc) 
程序首先调用deco(myfunc)，得到的返回结果赋值给了myfunc ，这样myfunc 就变成了指向函数_deco()的指针 
以后的myfunc()，其实是调用_deco()
## 第三步 ：对带参数的函数进行装饰
破案过程和第一步、第二步完全一致，不再重复了
```python
# -*- coding:gbk -*-
'''示例5: 对带参数的函数进行装饰，
内嵌包装函数的形参和返回值与原函数相同，装饰函数返回内嵌包装函数对象'''
def deco(func):
    def _deco(a, b):
        print("before myfunc() called.")
        ret = func(a, b)
        print("  after myfunc() called. result: %s" % ret)
        return ret
    return _deco
@deco
def myfunc(a, b):
    print(" myfunc(%s,%s) called." % (a, b))
    return a + b
myfunc(1, 2)
myfunc(3, 4)
```
## 第四步 ：让装饰器带参数
```python
# -*- coding:gbk -*-
'''示例7: 在示例4的基础上，让装饰器带参数，
和上一示例相比在外层多了一层包装。
装饰函数名实际上应更有意义些'''
def deco(arg):
    def _deco(func):
        def __deco():
            print("before %s called [%s]." % (func.__name__, arg))
            func()
            print("  after %s called [%s]." % (func.__name__, arg))
        return __deco
    return _deco
@deco("mymodule")
def myfunc():
    print(" myfunc() called.")
@deco("module2")
def myfunc2():
    print(" myfunc2() called.")
myfunc()
myfunc2()
```
这种带参数的装饰器怎么解释呢。其实是一样的，还是我们的替换操作 
@deco(“mymodule”)替换为myfunc = deco(“mymodule”)(myfunc ) 
注意啊，这里deco后面跟了两个括号。 
有同学要问了，这是什么意思？ 
其实很简单，先执行deco(“mymodule”)，返回结果为_deco 
再执行_deco(myfunc)，得到的返回结果为__deco 
所以myfunc = __deco
破案！
本文是转载文章，原文章地址：https://blog.csdn.net/u013858731/article/details/54971762


# Python 学习入门（19）—— 异常 - 阳光岛主 - CSDN博客

2013年12月06日 23:52:24[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：4360所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



在项目开发中，异常处理是不可或缺的。异常处理帮助人们debug，通过更加丰富的信息，让人们更容易找到bug的所在。异常处理还可以提高程序的容错性。
我们之前在讲循环对象的时候，曾提到一个StopIteration的异常，该异常是在循环对象穷尽所有元素时的报错。
我们以它为例，来说明基本的异常处理。
一个包含异常的程序:
re = iter(range(5))foriinrange(100):printre.next()print'HaHaHaHa'
首先，我们定义了一个循环对象re，该循环对象将进行5次循环，每次使用序列的一个元素。
在随后的for循环中，我们手工调用next()函数。当循环进行到第6次的时候，re.next()不会再返回元素，而是抛出(raise)StopIteration的异常。整个程序将会中断。
我们可以修改以上异常程序，直到完美的没有bug。但另一方面，如果我们在写程序的时候，知道这里可能犯错以及可能的犯错类型，我们可以针对该异常类型定义好”应急预案“。
re = iter(range(5))
try:foriinrange(100):printre.next()exceptStopIteration:print'here is end',iprint'HaHaHaHa'
在try程序段中，我们放入容易犯错的部分。我们可以跟上except，来说明如果在try部分的语句发生StopIteration时，程序该做的事情。如果没有发生异常，则except部分被跳过。
随后，程序将继续运行，而不是彻底中断。
完整的语法结构如下：
try:
...exceptexception1:
    ...exceptexception2:
...
except:
    ...else:
    ...finally:
    ...
如果try中有异常发生时，将执行异常的归属，执行except。异常层层比较，看是否是exception1, exception2...，直到找到其归属，执行相应的except中的语句。如果except后面没有任何参数，那么表示所有的exception都交给这段程序处理。比如:
try:print(a*2)exceptTypeError:print("TypeError")except:print("Not Type Error & Error noted")
由于a没有定义，所以是NameError。异常最终被except:部分的程序捕捉。
如果无法将异常交给合适的对象，异常将继续向上层抛出，直到被捕捉或者造成主程序报错。比如下面的程序
deftest_func():
try:
        m= 1/0exceptNameError:print("Catch NameError in the sub-function")try:
    test_func()exceptZeroDivisionError:print("Catch error in the main program")
子程序的try...except...结构无法处理相应的除以0的错误，所以错误被抛给上层的主程序。
如果try中没有异常，那么except部分将跳过，执行else中的语句。
finally是无论是否有异常，最后都要做的一些事情。
流程如下，
try->异常->except->finally
try->无异常->else->finally
我们也可以自己写一个抛出异常的例子:
print'Lalala'raiseStopIterationprint'Hahaha'
(注意，这个例子不具备任何实际意义。读者可探索更多有意义的例子。)
StopIteration是一个类。当我们raise它的时候，有一个中间环节，就是Python利用StopIteration生成一个该类的一个对象。Python实际上举出的，是这一个对象。当然，也可以直接写成:
raiseStopIteration()

示例：

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
@author: homer
@see: ithomer.net
'''
# 普通异常
def test_1():
    try:
        file("hello.txt", "r")
        print("read file...")
    except IOError:
        print("no file")        # no file
    except:
        print("except error")
# 嵌套异常
def test_2():
    try:
        s = "hello"
        try:
            print s[0] + s[1]   # he
            print s[0] - s[1]   # error
        except TypeError:
            print("str not support subtraction operator")       # str not support subtraction operator
    except:
        print("except error")
# finally子句都会被执行
def test_3():
    try:
        f = open("hello.txt", "r")
        try:
            print(f.read(6))            # hello.  of hello.txt
        except:
            print("read file except")
        finally:
            print("release resource")
            f.close()
    except IOError:
        print("file not exist")         # file not exist
        
# raise显示地引发异常，raise后面的语句将不能执行
def test_4():
    try:
        s = None
        if s is None:
            print("s is none")
            raise NameError         # 如果引发NameError异常，后面的代码将不能执行
        print(len(s))
    except TypeError:
        print("None is no lenght")
# 自定义异常
class CustomException(Exception):
    def __init__(self, x, y):
        Exception.__init__(self, x, y)
        self.x = x
        self.y = y
        
def test_5():
    try:
        x = 3
        y = 2
        if x % y > 0:
            print x/y       # 1
            raise CustomException(x, y)                             # 显示指定异常
    except CustomException, div:
        print("CustomException: x/y = %.2f" % (div.x/div.y))        # CustomException: x/y = 1.00

if __name__ == "__main__":
    test_1()
    test_2()
    test_3()
    test_4()
    test_5()
```

**Assert 语句**
assert语句用于检测某个条件表达式是否为真。assert语句又称为断言语句，即assert认为检测的表达式永远为真，if语句中的条件判断都可以使用assert语句检测。
assert isinstance(my_dict, dict)
assert isinstance(my_list, list)

**总结:**
try: ... except exception: ... else: ... finally: ...
raise exception

[python异常处理](http://www.cnblogs.com/dkblog/archive/2011/06/24/2089026.html)
[python中的异常处理](http://blog.chinaunix.net/uid-22920230-id-3074716.html)



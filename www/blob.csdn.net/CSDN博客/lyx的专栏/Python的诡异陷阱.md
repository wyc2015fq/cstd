# Python的诡异陷阱 - lyx的专栏 - CSDN博客





2017年07月29日 22:40:52[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：901
所属专栏：[和python在一起的日子](https://blog.csdn.net/column/details/16842.html)









        编程的人，特别是学过c语言，使用过很长时间c的人，都会觉得，python这种语言跟matlab一样，没什么内涵，很easy。一开始也是这么想的，那是慢慢的，越来越觉得，人生苦短，我用python的理念其实不对。python完成一些小制作是很easy的事情，但是真正要成为一种工具，其实还是要考虑很多事情。近期实习的过程中，这种感觉越来越强烈了。

        python看起来简单，其实内涵很复杂。有人说c很难，有指针；c++很难，要自己管理内存。其实，这种能看到的东西就不算难，掌握了就好，难的是隐藏起来的东西。就像风险，内被预计到的，都不是风险。

# 1.函数内部引用的错误



```python
va = 100
def df_f():
    print va
    va = 9
    print va
df_f()
```
        没有接触过的同学，恐怕觉得上面这段代码是没有问题的，因为python会自动去引用全局变量。


        首先，我们要清楚一点，python变量查找的顺序是
        Python的作用域解析是基于LEGB规则，分别是Local、Enclosing、Global、Built-in。也就是说，python会先查找函数内部的局部变量。那么问题来了，第一个print va的时候，寻找局部变量的时候能找到吗？答案是能！


这是因为，python在函数定义的时候，所有的变量初始化已经完成了，换句话说，我们定义函数，而没有调用的时候，下面的va = 9这句话已经定义了一个va变量。当我们调用函数的时候，第一个print va会去寻找va局部变量，幸运的是，他找到了va这个局部变量，但是，当时这个变量还没有赋值，只是被预留了，所以，最后就是出现了错误：

UnboundLocalError: local variable 'va' referenced before assignment


再来看一段：



```python
va = 100
def df_f():
    va = 20
    print va
    va = 9
    print va
df_f()
print va
```



我们看到结果是这样的：

D:\Anaconda2\python.exe E:/academic/funny/python_one_one_code/cl_4/hh.py
20
9
100


Process finished with exit code 0





        和我们的想法一下，va在函数里面是局部变量，并不会改变外面这个值。

        当我们真的想一开始就用外面这个va怎么办？这个时候就要用global申明。



```python
va = 100
def df_f():
    global va
    print va
    va = 9
    print va
df_f()
print va
```
D:\Anaconda2\python.exe E:/academic/funny/python_one_one_code/cl_4/hh.py
100
9
9


Process finished with exit code 0
        可以看到，全局变量va被我们在函数内部改变了。


        还有更加夸张的例子:



```python
import random
 
def func(ok):
    if ok:
        a = random.random()
    else:
        import random
        a = random.randint(1, 10)
    return a
 
func(True)
```
大家可以自己运行、思考一下。





# 2.把上面那句话：函数在定义的时候就开辟空间了延伸一下



```python
def f(lst = []):
    lst.append(1)
    return lst
print f()
print f()
```
我们按照上面的逻辑，在函数定义的时候，变量们都被开辟了空间，那么，这里的lst = [ ]其实也是被开辟空间了，所以，上面的函数运行的时候，append其实是同一个list，所以，结果是这样的：


D:\Anaconda2\python.exe E:/academic/funny/python_one_one_code/cl_4/hh.py
[1]
[1, 1]


Process finished with exit code 0
那么我们怎么避免这样的情况呢？



```python
def f(lst=None):
    if lst == None:
        lst = []
    lst.append(1)
    return lst
print f()
print f()
```
D:\Anaconda2\python.exe E:/academic/funny/python_one_one_code/cl_4/hh.py
[1]
[1]


Process finished with exit code 0
这个时候就和我们想要的结果一样了。


所以，尽可能不要在定义函数初始化变量的时候，直接默认定义为mutable这样的变量，当然，个人更加喜欢说，是reference这样的变量。

# 3. 以为的等价，并不等价

还是list这种reference带来的问题:



```python
>>> x=[1];print id(x);x=x+[2];print id(x)

4357132800

4357132728

>>> x=[1];print id(x);x+=[2];print id(x)

4357132800

4357132800
```
x = x + [2] 会改变x的引用，我们可以认为，是重新计算了一个值，然后放到了一个新的内存空间。而后者，则是放在原始的空间中。当然，这种对于value类型其实是一样的。


# 4.一个元素的tuple

(3),你以为这是一个tuple？不，他就是一个int，(3,)这样才是一个tuple。



```python
print type((1))
print type((1,))
```
D:\Anaconda2\python.exe E:/academic/funny/python_one_one_code/cl_4/hh.py
<type 'int'>
<type 'tuple'>


Process finished with exit code 0















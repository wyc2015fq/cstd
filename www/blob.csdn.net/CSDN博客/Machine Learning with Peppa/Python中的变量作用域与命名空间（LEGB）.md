# Python中的变量作用域与命名空间（LEGB） - Machine Learning with Peppa - CSDN博客





2018年04月16日 22:27:39[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：121








                Python 使⽤LEGB的顺序来查找⼀个符号对应的对象

locals    ->   enclosing    function    ->   globals    ->    builtins
局部变量———闭包空间———全局变量———内建模块


```python
a  =  1     #  全局变量 globals

def fun():

    a  =  2      #  闭包变量 enclosing

def  inner_fun():

    a  =  3     #   局部变量    locals   
                             
    print("a=%d"  %  a)    # 内建模块 builtins       

    return  inner_fun

f  =  fun()
f()
```



- locals，当前所在命名空间（如函数、模块），函数的参数也属于命名空间内的变量 enclosing，外部嵌套函数的命名空间（闭包中常见）
- enclosing, 介于local和global之间的命名空间
- global, 全局变量的空间
- builtins，内建模块的命名空间。

Python   在启动的时候会自动为我们载⼊很多内建的函数、类，⽐如   dict，list，type，print，这些都位于    __builtin__    模块中可以使⽤ dir(__builtin__) 来查看。        

这也是为什么我们在没有 import任何模块的情况下，就能使⽤丰富的函数和功能了。在Python中，有⼀个内建模块，在该模块中有⼀些常⽤函数;在Python启动后，且没有执⾏程序员所写的任何代码前，Python会⾸先加载该内建模块到内存。        
另外，该内建模块中的功能可以直接使⽤，不⽤在其前添加内建模块前缀，其原因是对函数、变量、类等标识符的查找是按LEGB法则，其中B即代表内建模块⽐如：内建模块中有⼀个abs()函数，其功能求绝对值，如abs(-20)将返回20。            



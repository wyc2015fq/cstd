# Python中定义“私有”成员变量和成员函数 - 宇宙浪子的专栏 - CSDN博客
2014年12月08日 21:41:54[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：2129
# [Python中定义“私有”成员变量和成员函数](http://blog.csdn.net/doufei_ccst/article/details/9152085)
参考连接：[http://docs.python.org/2/tutorial/classes.html](http://docs.python.org/2/tutorial/classes.html)[http://www.pythonclub.org/python-class/private](http://www.pythonclub.org/python-class/private)[http://blog.csdn.net/mafuli007/article/details/7777641](http://blog.csdn.net/mafuli007/article/details/7777641)
在学习Python的过程中发下，它把类（class）中所有的成员函数和成员变量都看做是"Public"的，作为C++出身的程序员们可能就不习惯了。
Python的官方教程中如是说：““Private” instance variables that cannot be accessed except from inside an object don’t exist in Python.”。也就是说，在Python中我们不能够像C++或者Java那样有专门的private和public关键字来指定某些成员是公有的，哪些成员是私有的。
然而，Python教程中又说了：“However, there is a convention that is followed by most Python code: a name prefixed with an underscore (e.g. _spam)
 should be treated as a non-public part of the API (whether it is a function, a method or a data member). It should be considered an implementation detail and subject to change without notice.”。
OK，原来在Python中大家都是通过在一个变量或者函数之前加上下划线来表示私有变量的，例如__spam(这里是两个下划线)就是私有的。
同时，Python为了保证不能再class之外访问该变量，“Any identifier of the form __spam (at
 least two leading underscores, at most one trailing underscore) is textually replaced with _classname__spam,
 where classname is
 the current class name with leading underscore(s) stripped.”，意思就是说，Python会在类的内部自动的把你定义的__spam变量的名字替换成为 _classname__spam(注意，classname前面是一个下划线，spam前是两个下划线)，Python把这种技术叫做“name mangling”。因此，用户在外部访问__spam的时候就会提示找不到相应的变量。
下面给出一段简单的代码：
**[python]**[view
 plain](http://blog.csdn.net/doufei_ccst/article/details/9152085#)[copy](http://blog.csdn.net/doufei_ccst/article/details/9152085#)
- #!/usr/bin/python
- #Filename: classPrivate.py
- 
- class testPrivate:  
- def __init__(self):  
- self.__data = []  
- 
- def add(self,item):  
- self.__data.append(item)  
- def printData(self):  
- print (self.__data)  
- 
- t = testPrivate()  
- t.add('dancingrain')  
- t.add('hello')  
- t.printData()  
- print(t.__data)  
输出结果是：
![](https://img-blog.csdn.net/20130622193427984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZG91ZmVpX2Njc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](http://blog.csdn.net/doufei_ccst/article/details/9152085)
可以看出，当执行print(t.__data)的时候提示我们没有该属性，OK！这就是我们想要的结果。
但是，这并不是意味着我们真的就不能够从外部访问这个变量了，上面说Python在类的内部用_classname__spam替换了__spam，
因此，我们可以在类的外面使用_classname__spam来访问__spam。看看下面的代码:
**[python]**[view
 plain](http://blog.csdn.net/doufei_ccst/article/details/9152085#)[copy](http://blog.csdn.net/doufei_ccst/article/details/9152085#)
- #!/usr/bin/python
- #Filename: classPrivate.py
- 
- class testPrivate:  
- def __init__(self):  
- self.__data = []  
- 
- def add(self,item):  
- self.__data.append(item)  
- def printData(self):  
- print (self.__data)  
- 
- t = testPrivate()  
- t.add('dancingrain')  
- t.add('hello')  
- t.printData()  
- print(t._testPrivate__data)  
结果是：
![](https://img-blog.csdn.net/20130622193351875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZG91ZmVpX2Njc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](http://blog.csdn.net/doufei_ccst/article/details/9152085)
OK,我们在外面也能够访问该“私有”变量了，
这一点在调试中是比较有用的！

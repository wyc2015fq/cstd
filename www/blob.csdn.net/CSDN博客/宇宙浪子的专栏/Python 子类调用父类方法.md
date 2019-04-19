# Python 子类调用父类方法 - 宇宙浪子的专栏 - CSDN博客
2014年12月08日 17:09:12[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：964
Python在继承时，如果子类重写了init()函数，则父类的init()不会被调用，这时如果子类只是想要对父类的init()函数进行简单的扩展的话，是很不方便的。那么有没有比较方便的方法来从子类调用父类呢？
        第一种是直接使用父类的类名来直接调用。(Python3.3)
**[python]**[view
 plain](http://blog.csdn.net/u012083681/article/details/17404031#)[copy](http://blog.csdn.net/u012083681/article/details/17404031#)[print](http://blog.csdn.net/u012083681/article/details/17404031#)[?](http://blog.csdn.net/u012083681/article/details/17404031#)
- class Parent:  
- def __init__(self):  
- print('This is parent init.')  
- def sleep(self):  
- print("Parent sleeps.")  
- 
- class Child(Parent):  
- def __init__(self):  
-         Parent.__init__(self)  
- print('This is child init.')  
- def sleep(self):  
- print("Child sleeps.")  
- 
- c=Child()  
        第二种是使用形如super(type, obj).methodName(args)调用，第一个参数表示从哪个类开始找其父类（不包括其自身），第二个参数表示类实例（一般使用self），参数args在父类方法的参数只有self时不写。
**[python]**[view
 plain](http://blog.csdn.net/u012083681/article/details/17404031#)[copy](http://blog.csdn.net/u012083681/article/details/17404031#)[print](http://blog.csdn.net/u012083681/article/details/17404031#)[?](http://blog.csdn.net/u012083681/article/details/17404031#)
- class Parent:  
- def __init__(self):  
- print('This is parent init.')  
- def sleep(self):  
- print("Parent sleeps.")  
- 
- class Child(Parent):  
- def __init__(self):  
- #Parent.__init__(self)
-         super(Child,self).__init__()  
- print('This is child init.')  
- def sleep(self):  
- print("Child sleeps.")  
- 
- c=Child()  
        当在类内部使用时，super(type, obj).methodName(args)中的type, obj可以省略不写。
        另外，在类外面也可以使用这一函数，比如当子类覆盖了父类的某一方法后，想要通过子类的对象调用父类的这一方法：
**[python]**[view
 plain](http://blog.csdn.net/u012083681/article/details/17404031#)[copy](http://blog.csdn.net/u012083681/article/details/17404031#)[print](http://blog.csdn.net/u012083681/article/details/17404031#)[?](http://blog.csdn.net/u012083681/article/details/17404031#)
- class Parent:  
- def __init__(self):  
- print('This is parent init.')  
- def sleep(self):  
- print("Parent sleeps.")  
- 
- class Child(Parent):  
- def __init__(self):  
- #Parent.__init__(self)
-         super().__init__()  
- print('This is child init.')  
- def sleep(self):  
- print("Child sleeps.")  
- 
- c=Child()  
- c.sleep()  
- super(Child,c).sleep()  

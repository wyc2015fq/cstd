# Python 面向对象编程（三）继承与多态 - 宇宙浪子的专栏 - CSDN博客
2014年12月08日 17:18:32[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：420标签：[python																[继承](https://so.csdn.net/so/search/s.do?q=继承&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[Python](https://blog.csdn.net/bluehawksky/article/category/2542577)

# [Python 面向对象编程（三）继承与多态](http://blog.csdn.net/chongtianfeiyu/article/details/21894005)
在前面一篇文章中谈到了类的基本定义和使用方法，这只体现了面向对象编程的三大特点之一：封装。下面就来了解一下另外两大特征：继承和多态。
在Python中，可以让一个类去继承一个类，被继承的类称为父类或者超类、也可以称作基类，继承的类称为子类。并且Python支持多继承，能够让一个子类有多个父类。
一.继承和多继承
**[python]**[view
 plain](http://blog.csdn.net/chongtianfeiyu/article/details/21894005#)[copy](http://blog.csdn.net/chongtianfeiyu/article/details/21894005#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/252779/fork)
- class superclass():  
-           ........  
- 
- class subclass(superclass):  
-           .........  
- 
在定义一个类的时候，可以在类名后面紧跟一对括号，在括号中指定所继承的父类，如果有多个父类，多个父类名之间用逗号隔开。

**[python]**[view
 plain](http://blog.csdn.net/chongtianfeiyu/article/details/21894005#)[copy](http://blog.csdn.net/chongtianfeiyu/article/details/21894005#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/252779/fork)
- class UniversityMember:  
- 
- def __init__(self,name,age):  
- self.name = name  
- self.age = age  
- 
- def getName(self):  
- returnself.name  
- 
- def getAge(self):  
- returnself.age  
- 
- class Student(UniversityMember):  
- 
- def __init__(self,name,age,sno,mark):  
-         UniversityMember.__init__(self,name,age)     #注意要显示调用父类构造方法，并传递参数self
- self.sno = sno  
- self.mark = mark  
- 
- def getSno(self):  
- returnself.sno  
- 
- def getMark(self):  
- returnself.mark  
- 
- 
- 
- class Teacher(UniversityMember):  
- 
- def __init__(self,name,age,tno,salary):  
-         UniversityMember.__init__(self,name,age)  
- self.tno = tno  
- self.salary = salary  
- 
- def getTno(self):  
- returnself.tno  
- 
- def getSalary(self):  
- returnself.salary  
1）在Python中，如果父类和子类都重新定义了构造方法__init( )__，在进行子类实例化的时候，子类的构造方法不会自动调用父类的构造方法，必须在子类中显示调用。
　　2）如果需要在子类中调用父类的方法，需要以   父类名.方法  这种方式调用，以这种方式调用的时候，注意要传递self参数过去。
　　对于继承关系，子类继承了父类所有的公有属性和方法，可以在子类中通过父类名来调用，而对于私有的属性和方法，子类是不进行继承的，因此在子类中是无法通过父类名来访问的。
　　对于多重继承，比如
　　class SubClass(SuperClass1,SuperClass2)
　　此时有一个问题就是如果SubClass没有重新定义构造方法，它会自动调用哪个父类的构造方法？这里记住一点：以第一个父类为中心。如果SubClass重新定义了构造方法，需要显示去调用父类的构造方法，此时调用哪个父类的构造方法由你自己决定；若SubClass没有重新定义构造方法，则只会执行第一个父类的构造方法。并且若SuperClass1和SuperClass2中有同名的方法，通过子类的实例化对象去调用该方法时调用的是第一个父类中的方法。
二.多态
　　多态即多种形态，在运行时确定其状态，在编译阶段无法确定其类型，这就是多态。Python中的多态和Java以及C++中的多态有点不同，Python中的变量是弱类型的，在定义时不用指明其类型，它会根据需要在运行时确定变量的类型（个人觉得这也是多态的一种体现），并且Python本身是一种解释性语言，不进行预编译，因此它就只在运行时确定其状态，故也有人说Python是一种多态语言。在Python中很多地方都可以体现多态的特性，比如 内置函数len(object)，len函数不仅可以计算字符串的长度，还可以计算列表、元组等对象中的数据个数，这里在运行时通过参数类型确定其具体的计算过程，正是多态的一种体现。有些朋友提出Python不支持多态，我是完全不赞同的.
Python以它这种独有的方式体现多态的根本原因我觉得有两点：1）Python是解释性语言；2）Python中变量是弱类型的。所以Python体现多态的方式和Java决然不同，但是不能因为同Java中体现多态的方式不同就认为Python不支持多态，这种想法过于片面。

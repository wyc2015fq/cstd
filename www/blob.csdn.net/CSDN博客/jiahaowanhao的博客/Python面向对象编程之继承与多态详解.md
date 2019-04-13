
# Python面向对象编程之继承与多态详解 - jiahaowanhao的博客 - CSDN博客


2018年01月25日 21:13:31[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：103标签：[Python																](https://so.csdn.net/so/search/s.do?q=Python&t=blog)


[Python面向对象编程之继承与多态详解](http://cda.pinggu.org/view/24561.html)
本文实例讲述了Python面向对象编程之继承与多态。分享给大家供大家参考，具体如下：
Python 类的继承
在OOP（Object Oriented Programming）程序设计中，当我们定义一个class的时候，可以从某个现有的class 继承，新的class称为子类（Subclass），而被继承的class称为基类、父类或超类（Base class、Super class）。
我们先来定义一个class Person，表示人，定义属性变量 name 及 sex （姓名和性别）；
定义一个方法print_title()：当sex是male时，print man；当sex 是female时，print woman。参考如下代码：
class Person(object):
def __init__(self,name,sex):
self.name = name
self.sex = sex
def print_title(self):
if self.sex == "male":
print("man")
elif self.sex == "female":
print("woman")
class Child(Person):              \# Child 继承 Person
pass
May = Child("May","female")
Peter = Person("Peter","male")
print(May.name,May.sex,Peter.name,Peter.sex)  \# 子类继承父类方法及属性
May.print_title()
Peter.print_title()
而我们编写 Child 类，完全可以继承 Person 类（Child 就是 Person）；使用 class subclass_name(baseclass_name) 来表示继承；
继承有什么好处？最大的好处是子类获得了父类的全部属性及功能。如下 Child 类就可以直接使用父类的 print_title() 方法
实例化Child的时候，子类继承了父类的构造函数，就需要提供父类Person要求的两个属性变量 name 及 sex：
在继承关系中，如果一个实例的数据类型是某个子类，那它也可以被看做是父类（May 既是 Child 又是 Person）。但是，反过来就不行（Peter 仅是 Person，而不是Child）。
继承还可以一级一级地继承下来，就好比从爷爷到爸爸、再到儿子这样的关系。而任何类，最终都可以追溯到根类object，这些继承关系看上去就像一颗倒着的树。比如如下的继承树：
![](http://cda.pinggu.org/uploadfile/image/20180125/20180125081815_92347.png)
isinstance()   及  issubclass()
Python 与其他语言不同点在于，当我们定义一个 class 的时候，我们实际上就定义了一种数据类型。我们定义的数据类型和Python自带的数据类型，比如str、list、dict没什么两样。
Python 有两个判断继承的函数：isinstance() 用于检查实例类型；issubclass() 用于检查类继承。参见下方示例：
class Person(object):
pass
class Child(Person):         \# Child 继承 Person
pass
May = Child()
Peter = Person()
print(isinstance(May,Child))     \# True
print(isinstance(May,Person))    \# True
print(isinstance(Peter,Child))    \# False
print(isinstance(Peter,Person))   \# True
print(issubclass(Child,Person))   \# True
Python 类的多态
在说明多态是什么之前，我们在 Child 类中重写 print_title() 方法：若为male，print boy；若为female，print girl
class Person(object):
def __init__(self,name,sex):
self.name = name
self.sex = sex
def print_title(self):
if self.sex == "male":
print("man")
elif self.sex == "female":
print("woman")
class Child(Person):        \# Child 继承 Person
def print_title(self):
if self.sex == "male":
print("boy")
elif self.sex == "female":
print("girl")
May = Child("May","female")
Peter = Person("Peter","male")
print(May.name,May.sex,Peter.name,Peter.sex)
May.print_title()
Peter.print_title()
当子类和父类都存在相同的 print_title()方法时，子类的 print_title() 覆盖了父类的 print_title()，在代码运行时，会调用子类的 print_title()
这样，我们就获得了继承的另一个好处：多态。
多态的好处就是，当我们需要传入更多的子类，例如新增 Teenagers、Grownups 等时，我们只需要继承 Person 类型就可以了，而print_title()方法既可以直不重写（即使用Person的），也可以重写一个特有的。这就是多态的意思。调用方只管调用，不管细节，而当我们新增一种Person的子类时，只要确保新方法编写正确，而不用管原来的代码。这就是著名的“开闭”原则：
对扩展开放（Open for extension）：允许子类重写方法函数
对修改封闭（Closed for modification）：不重写，直接继承父类方法函数
子类重写构造函数
子类可以没有构造函数，表示同父类构造一致；子类也可重写构造函数；现在，我们需要在子类 Child 中新增两个属性变量：mother 和 father，我们可以构造如下（建议子类调用父类的构造方法，参见后续代码）：
class Person(object):
def __init__(self,name,sex):
self.name = name
self.sex = sex
class Child(Person):        \# Child 继承 Person
def __init__(self,name,sex,mother,father):
self.name = name
self.sex = sex
self.mother = mother
self.father = father
May = Child("May","female","April","June")
print(May.name,May.sex,May.mother,May.father)
若父类构造函数包含很多属性，子类仅需新增1、2个，会有不少冗余的代码，这边，子类可对父类的构造方法进行调用，参考如下：
class Person(object):
def __init__(self,name,sex):
self.name = name
self.sex = sex
class Child(Person):             \# Child 继承 Person
def __init__(self,name,sex,mother,father):
Person.__init__(self,name,sex)    \# 子类对父类的构造方法的调用
self.mother = mother
self.father = father
May = Child("May","female","April","June")
print(May.name,May.sex,May.mother,May.father)
多重继承
多重继承的概念应该比较好理解，比如现在需要新建一个类 baby 继承 Child ， 可继承父类及父类上层类的属性及方法，优先使用层类近的方法，代码参考如下：
class Person(object):
def __init__(self,name,sex):
self.name = name
self.sex = sex
def print_title(self):
if self.sex == "male":
print("man")
elif self.sex == "female":
print("woman")
class Child(Person):
pass
class Baby(Child):
pass
May = Baby("May","female")    \# 继承上上层父类的属性
print(May.name,May.sex)
May.print_title()         \# 可使用上上层父类的方法
class Child(Person):
def print_title(self):
if self.sex == "male":
print("boy")
elif self.sex == "female":
print("girl")
class Baby(Child):
pass
May = Baby("May","female")
May.print_title()        \# 优先使用上层类的方法


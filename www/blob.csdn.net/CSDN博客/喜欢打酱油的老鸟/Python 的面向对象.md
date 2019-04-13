
# Python 的面向对象 - 喜欢打酱油的老鸟 - CSDN博客


2019年01月20日 18:08:01[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：42标签：[Python																](https://so.csdn.net/so/search/s.do?q=Python&t=blog)[面向对象																](https://so.csdn.net/so/search/s.do?q=面向对象&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Python&t=blog)个人分类：[人工智能																](https://blog.csdn.net/weixin_42137700/article/category/7820233)


[https://www.toutiao.com/a6647111554762277383/](https://www.toutiao.com/a6647111554762277383/)
2019-01-16 23:07:41
Python是一种面向对象的语言。所以，创建和使用类和对象非常简单。
如果您以前没有任何面向对象（object-oriented（OO））编程的经验，那么您可能需要参考它的入门课程或至少某种类型的教程，以便掌握基本概念。
本文是面向对象编程（OOP）的简单介绍。
OOP术语概述
Class（类） - 类是一种用户定义的数据类型，跟结构类似。类具有自己的成员变量和成员函数，通过它们可以对类自身进行操作。
Class variable（类变量） - 由类的所有实例共享的变量。类变量是在类中定义，但在类的任何方法之外。类中独立于方法之外的变量。
Data member （数据成员） - 包含与类及其对象关联的数据的类变量或实例变量。
Function overloading（重载函数） - 为特定函数分配多个行为。执行的操作因所涉及的对象或参数的类型而不同。
Instance variable（实例变量） - 在方法内定义的变量，仅属于类的当前实例。
Inheritance（继承） - 将类的特征传递给从其派生的其他类。
Instance（实例） - 某个类的单个对象。例如，属于Circle类的对象obj是Circle类的实例。
Instantiation（实例化）实例化 - 类创建对象的过程。
Method（方法）方法 - 是为了达成某个目标所用的方式和办法。
Object（对象） -类在内存中装载的实例，具有相关的成员变量和成员函数。
Operator overloading（运算符重载） - 对已有的运算符重新进行定义，赋予其另一种功能，以适应不同的数据类型。
创建类
在类的语句创建一个新的类定义。该类的名称紧跟在关键类之后，后跟冒号，如下所示 -
```python
class ClassName:
 'Optional class documentation string'
 class_suite
```
![Python 的面向对象](http://p1.pstatp.com/large/pgc-image/b705d675770748b4930e00885c030a25)
该类有一个文档字符串，可以通过ClassName .__ doc__访问。
class_suite由定义类成员，数据属性和函数的所有组件的语句组成。
以下是一个简单的Python类的示例 ：
```python
class Employee:
 'Common base class for all employees'
 empCount = 0
 def __init__(self, name, salary):
 self.name = name
 self.salary = salary
 Employee.empCount += 1
 
 def displayCount(self):
 print "Total Employee %d" % Employee.empCount
 def displayEmployee(self):
 print "Name : ", self.name, ", Salary: ", self.salary
```
![Python 的面向对象](http://p1.pstatp.com/large/pgc-image/a33baf7df5b34329822224cf0e1aadbf)
变量empCount是一个类变量，其值在此类的所有实例之间共享。可以从类内部或类外部以Employee.empCount的形式访问。
第一个方法__init __（）是一个特殊的方法，被称为类构造函数或初始化方法，Python在创建该类的新实例时调用该方法。
除了每个方法的第一个参数是self之外，还可以声明其他类方法，比如普通函数。Python将self参数添加到列表中; 调用方法时不需要包含它。
创建实例对象
要创建类的实例，可以使用类名调用该类，并传入其__init__方法接受的任何参数。
```python
"This would create first object of Employee class"
emp1 = Employee("Zara", 2000)
"This would create second object of Employee class"
emp2 = Employee("Manni", 5000)
```
![Python 的面向对象](http://p3.pstatp.com/large/pgc-image/06d0d3ec55fe416a971068bd7a818f44)
访问属性
可以使用带有对象的点运算符来访问对象的属性。类变量会使用类名访问，如下所示 -
```python
emp1.displayEmployee()
emp2.displayEmployee()
print "Total Employee %d" % Employee.empCount
```
![Python 的面向对象](http://p1.pstatp.com/large/pgc-image/c127399e44194db58f4c8e0dd111b5c6)
现在，将所有概念放在一起 -
```python
#!/usr/bin/python
class Employee:
 'Common base class for all employees'
 empCount = 0
 def __init__(self, name, salary):
 self.name = name
 self.salary = salary
 Employee.empCount += 1
 
 def displayCount(self):
 print "Total Employee %d" % Employee.empCount
 def displayEmployee(self):
 print "Name : ", self.name, ", Salary: ", self.salary
"This would create first object of Employee class"
emp1 = Employee("Zara", 2000)
"This would create second object of Employee class"
emp2 = Employee("Manni", 5000)
emp1.displayEmployee()
emp2.displayEmployee()
print "Total Employee %d" % Employee.empCount
```
![Python 的面向对象](http://p3.pstatp.com/large/pgc-image/7235c27de8994d0ab87be630d66f73b0)
执行上述代码时，会产生以下结果 -
```python
Name : Zara ,Salary: 2000
Name : Manni ,Salary: 5000
Total Employee 2
```
![Python 的面向对象](http://p1.pstatp.com/large/pgc-image/b14bfd33207d4f899278ac0b76ce7296)
可以随时添加，删除或修改类和对象的属性 -
```python
emp1.age = 7 # Add an 'age' attribute.
emp1.age = 8 # Modify 'age' attribute.
del emp1.age # Delete 'age' attribute.
```
![Python 的面向对象](http://p1.pstatp.com/large/pgc-image/41bbb840bcff4813b4d40bc9b900efde)
可以使用以下函数 - 而不是使用常规语句来访问属性 -
getattr（OBJ，名称[默认]） -访问对象的属性。
hasattr（OBJ，名称） -检查是否存在一个属性。
setattr（OBJ，名称，值） -设置属性。如果属性不存在，则会创建它。
delattr（OBJ，名称） -删除一个属性。
```python
hasattr(emp1, 'age') # Returns true if 'age' attribute exists
getattr(emp1, 'age') # Returns value of 'age' attribute
setattr(emp1, 'age', 8) # Set attribute 'age' at 8
delattr(empl, 'age') # Delete attribute 'age'
```
![Python 的面向对象](http://p3.pstatp.com/large/pgc-image/193758c0074240f49480077152c8e6e0)
内置类属性
每个Python类都遵循内置属性，并且可以使用点运算符访问，就像其他属性一样 -
__dict__ - 包含类名称空间的字典。
__doc__ - 类文档字符串或无，如果未定义。
__name__ - 类名称。
__module__ - 定义类的模块名称。此属性在交互模式下为“__main__”。
__bases__ - 一个可能为空的元组，包含基类，按它们在基类列表中出现的顺序排列。
对于上面的类，让我们尝试访问所有这些属性 -
```python
#!/usr/bin/python
class Employee:
 'Common base class for all employees'
 empCount = 0
 def __init__(self, name, salary):
 self.name = name
 self.salary = salary
 Employee.empCount += 1
 
 def displayCount(self):
 print "Total Employee %d" % Employee.empCount
 def displayEmployee(self):
 print "Name : ", self.name, ", Salary: ", self.salary
print "Employee.__doc__:", Employee.__doc__
print "Employee.__name__:", Employee.__name__
print "Employee.__module__:", Employee.__module__
print "Employee.__bases__:", Employee.__bases__
print "Employee.__dict__:", Employee.__dict__
```
![Python 的面向对象](http://p1.pstatp.com/large/pgc-image/393ad44a13da49298488761242d5446f)
执行上述代码时，会产生以下结果 -
```python
Employee.__doc__: Common base class for all employees
Employee.__name__: Employee
Employee.__module__: __main__
Employee.__bases__: ()
Employee.__dict__: {'__module__': '__main__', 'displayCount':
<function displayCount at 0xb7c84994>, 'empCount': 2, 
'displayEmployee': <function displayEmployee at 0xb7c8441c>, 
'__doc__': 'Common base class for all employees', 
'__init__': <function __init__ at 0xb7c846bc>}
```
![Python 的面向对象](http://p3.pstatp.com/large/pgc-image/32d127a31a0140afa885104591c4d41b)
销毁对象（垃圾收集）
Python会自动删除不需要的对象（内置类型或类实例）以释放内存空间。Python周期性的回收不再使用的内存块的过程称为垃圾收集。
Python的垃圾收集器在程序执行期间运行，当对象的引用计数为零时触发。对象的引用计数随着指向它的别名数量的变化而变化。
当对象被分配新名称或放入容器(列表、元组或字典)时会增加。对象的引用计数在使用del删除、重新分配引用或引用超出作用域时减少。当对象的引用计数为零时，Python会自动收集它。
```python
a = 40 # Create object <40>
b = a # Increase ref. count of <40> 
c = [b] # Increase ref. count of <40> 
del a # Decrease ref. count of <40>
b = 100 # Decrease ref. count of <40> 
c[0] = -1 # Decrease ref. count of <40>
```
![Python 的面向对象](http://p1.pstatp.com/large/pgc-image/839d82fb77cb46a89712b6e7a9c5a78e)
通常不会注意到垃圾收集器何时销毁孤立实例并回收其空间。但是类可以实现特殊方法__del __（），称为析构函数，在实例即将被销毁时调用。这个方法可用于清除实例使用的任何非内存资源。
这个__del __（）析构函数输出即将被销毁的实例的类名 -
```python
#!/usr/bin/python
class Point:
 def __init__( self, x=0, y=0):
 self.x = x
 self.y = y
 def __del__(self):
 class_name = self.__class__.__name__
 print class_name, "destroyed"
pt1 = Point()
pt2 = pt1
pt3 = pt1
print id(pt1), id(pt2), id(pt3) # prints the ids of the obejcts
del pt1
del pt2
del pt3
```
![Python 的面向对象](http://p3.pstatp.com/large/pgc-image/af5377f9958e4a4c934b4662be0bd7b3)
执行上面的代码时，会产生以下结果 -
```python
3083401324 3083401324 3083401324
Point destroyed
```
![Python 的面向对象](http://p3.pstatp.com/large/pgc-image/767f5992cce048d689df06675d268de6)
注意 - 理想情况下，您应该在单独的文件中定义类，然后您应该使用import语句将它们导入主程序文件中。
类继承
您可以通过在新类名后面的括号中列出父类，而不是从头开始，通过从预先存在的类派生它来创建类。
子类继承其父类的属性，可以使用这些属性，就好像它们是在子类中定义的一样。子类还可以覆盖父类的数据成员和方法。
```python
class SubClassName (ParentClass1[, ParentClass2, ...]):
 'Optional class documentation string'
 class_suite
```
![Python 的面向对象](http://p3.pstatp.com/large/pgc-image/deaadd70ff0b4fe189fe1f5ecaed5d97)
语法
派生类的声明与其父类非常相似; 但是，在类名后面给出了一个要继承的基类列表 -
```python
#!/usr/bin/python
class Parent: # define parent class
 parentAttr = 100
 def __init__(self):
 print "Calling parent constructor"
 def parentMethod(self):
 print 'Calling parent method'
 def setAttr(self, attr):
 Parent.parentAttr = attr
 def getAttr(self):
 print "Parent attribute :", Parent.parentAttr
class Child(Parent): # define child class
 def __init__(self):
 print "Calling child constructor"
 def childMethod(self):
 print 'Calling child method'
c = Child() # instance of child
c.childMethod() # child calls its method
c.parentMethod() # calls parent's method
c.setAttr(200) # again call parent's method
c.getAttr() # again call parent's method
```
![Python 的面向对象](http://p1.pstatp.com/large/pgc-image/3141208bfc594dd0b971942893eec2a9)
执行上述代码时，会产生以下结果 -
```python
Calling child constructor
Calling child method
Calling parent method
Parent attribute : 200
```
![Python 的面向对象](http://p1.pstatp.com/large/pgc-image/01410c34fdf64a8d8bd8f393f5ee8348)
类似地，您可以从多个父类驱动一个类，如下所示 -
```python
class A: # define your class A
.....
class B: # define your class B
.....
class C(A, B): # subclass of A and B
.....
```
![Python 的面向对象](http://p1.pstatp.com/large/pgc-image/be7e5aae29844a1cbc0a64184cd7679a)
可以使用issubclass()或isinstance()函数检查两个类和实例的关系。
issubclass（sub, sup）如果给定的子类是超类的子类SUP，则布尔函数返回true。
isinstance（obj, Class）如果obj是class的实例，或者是class的子类的实例，则布尔函数返回true。
重载方法
始终可以重载父类方法。重载父类方法的原因是因为您可能需要子类具有特殊或不同的功能。
```python
#!/usr/bin/python
class Parent: # define parent class
 def myMethod(self):
 print 'Calling parent method'
class Child(Parent): # define child class
 def myMethod(self):
 print 'Calling child method'
c = Child() # instance of child
c.myMethod() # child calls overridden method
```
![Python 的面向对象](http://p1.pstatp.com/large/pgc-image/d008bd3e13fe4e718bcd6dbaef8e7950)
执行上述代码时，会产生以下结果 -
```python
Calling child method
```
![Python 的面向对象](http://p1.pstatp.com/large/pgc-image/c670610208ea413ba1f795998796938a)
基础重载方法
下表列出了一些您可以在自己的类中覆盖的通用功能 -
Sr.No.Method，描述和样本Call1
__init__（self [，args ...]）
构造函数（带有任何可选参数）
示例调用：obj = className（args）
2
__del __（self ）
析构函数，删除一个对象
样本调用：del obj
3
__repr __（self ）
可评估的字符串表示
样本电话：repr（obj）
4
__str __（ self ）
可输出的字符串表示
样本调用：str（obj）
5
__cmp __（self，x）
对象比较
样本调用：cmp（obj，x）
重载运算符
假设你已经创建了一个向量类来表示二维向量，当您使用加号运算符添加时，Python会发出错误提示。
但是，您可以在类中定义__add__方法以执行向量添加，然后加号运算符将按预期运行 -
```python
#!/usr/bin/python
class Vector:
 def __init__(self, a, b):
 self.a = a
 self.b = b
 def __str__(self):
 return 'Vector (%d, %d)' % (self.a, self.b)
 
 def __add__(self,other):
 return Vector(self.a + other.a, self.b + other.b)
v1 = Vector(2,10)
v2 = Vector(5,-2)
print v1 + v2
```
![Python 的面向对象](http://p9.pstatp.com/large/pgc-image/80b231b5765a4746aa63a50e586f128b)
执行上述代码时，会产生以下结果 -
```python
Vector(7,8)
```
![Python 的面向对象](http://p1.pstatp.com/large/pgc-image/406f2b52092b4ab088262b4993a84b93)
数据隐藏
对象的属性在类定义之外可能可见也可能不可见。需要使用双下划线前缀命名属性，然后这些属性不会被外人直接看到。
```python
#!/usr/bin/python
class JustCounter:
 __secretCount = 0
 
 def count(self):
 self.__secretCount += 1
 print self.__secretCount
counter = JustCounter()
counter.count()
counter.count()
print counter.__secretCount
```
![Python 的面向对象](http://p1.pstatp.com/large/pgc-image/0fc0222b0c484d51a5c2e0c3c6d4d967)
执行上述代码时，会产生以下结果 -
```python
1
2
Traceback (most recent call last):
 File "test.py", line 12, in <module>
 print counter.__secretCount
AttributeError: JustCounter instance has no attribute '__secretCount'
```
![Python 的面向对象](http://p9.pstatp.com/large/pgc-image/27345c37082249fd9f8c28cebce812bf)
Python通过内部更改名称来包含类名来保护这些成员。您可以访问object._className__attrName等属性。如果您将以下内容替换最后一行， -
```python
.........................
print counter._JustCounter__secretCount
```
![Python 的面向对象](http://p1.pstatp.com/large/pgc-image/502ab3e3d4dc40059bf857ee5cc37fd1)
执行上述代码时，会产生以下结果 -
```python
1
2
2
```
![Python 的面向对象](http://p1.pstatp.com/large/pgc-image/bc07d4f85bc34f1e8b30ea85fa9e0999)


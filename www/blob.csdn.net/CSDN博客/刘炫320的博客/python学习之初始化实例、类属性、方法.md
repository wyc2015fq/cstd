# python学习之初始化实例、类属性、方法 - 刘炫320的博客 - CSDN博客
2017年08月30日 22:16:30[刘炫320](https://me.csdn.net/qq_35082030)阅读数：3120
# 0. 写在前面
对于python的面向对象的编程还是稍显薄弱，在接下来的几篇中，我们将着重讲解python的面向对象的特性。因为这点确实和java不一样。
# 1.初始化实例化属性。
可接受任意关键字参数，并把他们都作为属性赋值给实例。使用**kw，除了可以直接使用self.name = ‘xxx’设置一个属性外，还可以通过setattr(self, ‘name’, ‘xxx’)设置属性。
```python
class Person(object):
    def __init__(self, name, gender, **kw):
        self.name = name
        self.gender = gender
        for k,v in kw.items():
            setattr(self, k, v)
xiaoming = Person('Xiao Ming', 'Male', birth='1990-1-1', job='Student')
print(xiaoming.name)
print(xiaoming.job)
```
# 2.类中的私有成员,双下划线开头的”__score”不能直接被外部访问。
```python
class Person1(object):
    def __init__(self, name, score):
        self.name = name
        self._gender = 'Male'
        self.__score = score
p1 = Person1('Bob', 59)
try:
    print(p1.__score)  #AttributeError: 'Person1' object has no attribute '__score'
except AttributeError:
    print('attrbuteerror')
```
# 3.创建类属性。
实例属性每个实例都各自拥有，互相独立，而类属性有且只有一份。
```python
class Person2(object):
    address = 'Earth'
    def __init__(self, name):
        self.name = name
print(Person2.address)
p2 = Person2('Tom')
print(Person2.address)
#请给Person3类添加一个类属性count每创建一个实例count属性就加1这样就可以统计出一共创建了多少个Person3的实例。
class Person3(object):
    count = 0
    def __init__(self, name):
        self.name = name
        Person3.count += 1
p3 = Person3('Alice')
print(p3.count)
p3_1 = Person3('Tim')
print(p3_1.count)
```
# 4.python中类属性和实例属性名字冲突怎么办？
请把上节的Person3类属性count改为__count，再试试能否从实例和类访问该属性。
```python
class Person4(object):
    __count = 0
    def __init__(self, name):
        self.name = name
        Person4.__count += 1
        print(Person4.__count)
p4 = Person4('Bob')
p4_1 = Person4('Alice')
try:
    print(Person4.__count)  #AttributeError: type object 'Person4' has no attribute '__count'
except AttributeError:
    print('AttributeError')
```
类属性的公开和私有，如果没有双下划线，外部可以调用，如果有，只能在类内部使用。
# 5.一个实例的私有属性就是以__开头的属性，无法被外部访问，那这些属性定义有什么用？
虽然私有属性无法从外部访问，但是，从类的内部是可以访问的。除了可以定义实例的属性外，还可以定义实例的方法。
实例的方法就是在类中定义的函数，它的第一个参数永远是self，指向调用该方法的实例本身，其他参数和一个普通函数是完全一样的：
```python
class Person5(object):
    def __init__(self, name):
        self.__name = name
    def get_name(self):
        return self.__name
p5 = Person5('Bob')
print(p5.get_name())
```
任务:请给Person5类增加一个私有属性__score，表示分数，再增加一个实例方法get_grade()，
能根据__score的值分别返回A-优秀, B-及格, C-不及格三档。
```python
class Person6(object):
    def __init__(self, name, score):
        self.name = name
        self.__score = score
    def get_grade(self):
        if self.__score >= 80:
            return "A-Excellent"
        elif self.__score >= 60:
            return "B-Passed"
        return "C-Failed"
p6_1 = Person6('Bob', 89)
p6_2 = Person6('Alice', 69)
p6_3 = Person6('Tim', 59)
print(p6_1.get_grade())
print(p6_2.get_grade())
print(p6_3.get_grade())
```
# 6.python中方法也是属性,由于属性可以是普通的值对象，如str，int等，也可以是方法，还可以是函数，大家看看下面代码的运行结果，请想一想p6.get_grade为什么是函数而不是方法：
```python
class Person6(object):
    def __init__(self, name, score):
        self.name = name
        self.score = score
        self.get_grade = lambda: 'A'
p6 = Person6('Bob', 90)
print(p6.get_grade)    
#<function Person6.__init__.<locals>.<lambda> at0x000001D1244300D0>
print(p6.get_grade())  #A
```
直接把lambda函数赋值给self.get_grade和绑定方法有所不同，函数调用不需要传入self，但是方法调用需要传入self。
p6.get_grade是属性，只不过这里的属性是一个函数对象.
p6.get_grade()是方法，前面的p6就是调用这个方法的对象，即实例，整句来说就是实例方法

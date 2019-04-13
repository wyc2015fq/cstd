
# Python编程之多态用法实例详解 - jiahaowanhao的博客 - CSDN博客


2018年01月26日 20:35:40[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：145


[Python编程之多态用法实例详解](http://cda.pinggu.org/view/24567.html)
本文实例讲述了Python编程之多态用法。分享给大家供大家参考。具体分析如下：
什么是多态？顾名思义，多态就是多种表现形态的意思。它是一种机制、一种能力，而非某个关键字。它在类的继承中得以实现，在类的方法调用中得以体现。多态意味着变量并不知道引用的对象是什么，根据引用对象的不同表现不同的行为方式。
我们先看一个简单的例子，运算符多态：
a=34
b=57
print(a+b)
a="世界"
b="你好"
print(a+b)
我们不知道+法运算符左右两个变量是什么类型，当我们给的是int类型时，它就进行加法运算。当我们给的是字符串类型时，它就返回的是两个字符串拼接的结果。也就是根据变量类型的不同，表现不能的形态。
我们再来看一个例子，方法多态：
我们首先创建一个名为MyClass.py的文件，代码如下
__author__= 'MXi4oyu'
classPeople:
def say(self):
print("大家好！")
classStudent:
def say(self):
print("老师好！")
我们再创建一个Main.py的文件，代码如下：
__author__= 'MXi4oyu'
fromrandom import choice
importMyClass
p1=MyClass.People()
stu1=MyClass.Student()
\#通过choice方法我们可以随机选择列表中的某一项
obj=choice([p1,stu1])
print(type(obj))
obj.say()
我们创建的临时对象obj是由随机函数取出来的，我们不知道它的具体类型，但是我们可以对它进行相同的操作。即让它调用say方法，然后根据其类型的不同，它所表现的行为不同。这就是多态。


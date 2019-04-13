
# 举例讲解Python中metaclass元类的创建与使用 - jiahaowanhao的博客 - CSDN博客


2018年02月08日 20:53:30[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：143标签：[Python																](https://so.csdn.net/so/search/s.do?q=Python&t=blog)


[举例讲解Python中metaclass元类的创建与使用](http://cda.pinggu.org/view/24685.html)
元类是可以让你定义某些类是如何被创建的。从根本上说，赋予你如何创建类的控制权。
元类也是一个类，是一个type类。
元类一般用于创建类。在执行类定义时，解释器必须要知道这个类的正确的元类，如果此属性没有定义，它会向上查找父类中的__metaclass__属性。如果还没发现，就查找全局变量。
对于传统类来说，它们的元类是types.ClassType。
元类也有构造器，传递三个参数：类名，从基类继承数据的元组，和类属性字典。
下面我们来定义一个元类，要求写类的时候必须给类提供一个__str__()方法，如果没有提供__repr__()方法，
则给你警告。
from warnings import warn
\#元类需要继承type类
class ReqStrSugRepr(type):
def __init__(cls, name, bases, attrd):
\#构造函数需要传递的参数为类名，基类，类属性字典
super(ReqStrSugRepr, cls).__init__(name, bases, attrd)
\# 判断__str__字符串是否在类的属性字典里
if '__str__' not in attrd:
raise TypeError('Class requires overriding of __str__()')
if '__repr__' not in attrd:
warn('Class suggests overriding of __repr__()\n', stacklevel=3)
class Foo(object):
\#给类指定元类
__metaclass__ = ReqStrSugRepr
def foo(self):
pass
\#这一段代码不用创建类来测试，直接运行一下就会报错，可见元类的功力。
![](http://cda.pinggu.org/uploadfile/image/20180208/20180208070119_25615.png)
type
type函数可以查看一个变量的类型, 比如:
\# <type 'int'>
\# <type 'str'>
type(1)
type('mink')
type函数还可以创建一个新的对象
type接受三个参数，name, bases, dict 第一个接受类名，第二个参数接受父类(元组形式),第三个参数接受属性和方法(字典形式)
X = type('X', (object,), dict(a=1))
\# 等于
class X(object):
a = 1
下面是接受函数的方法
def say(self):
print 'hello'
X = type('X', (object,), dict(say=say))
x = X()
\# pirnt hello
x.say()
元类
我们都知道通过类可以创建处实例对象，而元类就是创建出类对象的类。type可以创建出类对象也就是说type就是一个元类。
metaclass 属性
如果想使用元类创建类对象就需要对该对象添加一个__metaclass__属性。当然你首先得有一个元类
class PrivateMetaclass(type):
def __new__(cls, name, parents, attrs):
attrs = dict(('__%s' % k, v) for k, v in attrs.itmes())
return super(PrivateMetaclass, cls).__new__(cls, name, parents, attrs)
class A(object):
__metaclass__ = PrivateMetaclass
a = 1
b = 2
a = A()
\# raise AttributeError
print a.a, a.b
\# print 1, 2
print a.__a, a.__b
这样你就可以通过元类来修改类的一些特性，上面的就是修改变量为私有变量.


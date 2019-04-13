
# Python设计模式之观察者模式实例 - jiahaowanhao的博客 - CSDN博客


2018年01月31日 21:17:18[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：36


[Python设计模式之观察者模式实例](http://cda.pinggu.org/view/24614.html)
关于设计模式中的观察者模式，定义如下（维基百科）：
觀察者模式（有時又被稱為發布/訂閱模式）是軟體設計模式的一種。在此種模式中，一個目標物件管理所有相依於它的觀察者物件，並且在它本身的狀態改變時主動發出通知。這通常透過呼叫各觀察者所提供的方法來實現。此種模式通常被用來實作事件處理系統。
简单来说，一个被观察者有很多观察者，被观察者的状态的改变会引起所有观察者的响应操作。
那么我们用Python2.7来实现观察者模式。
Python中的集合set
集合（set），类似于列表（list），但是它没有重复的元素，它的doc内容如下：
代码如下:
>>> print set.__doc__
set() -> new empty set object
set(iterable) -> new set objectBuild an unordered collection of unique elements.
下面是在ipython中进行的几个简单的集合操作。
代码如下:
In [1]: myset = set()In [2]: myset.add(1)
In [3]: myset.add(2)
In [4]: myset.add('s')
In [5]: print myset
set([1, 2, 's'])
In [6]: myset.add('s')
In [7]: print myset
set([1, 2, 's'])
In [8]: myset.remove(3)
---------------------------------------------------------------------------
KeyError                                  Traceback (most recent call last)
<ipython-input-8-a93073f8a2af> in <module>()
----> 1 myset.remove(3)
KeyError: 3
In [9]: myset.remove(1)
In [10]: print myset
set([2, 's'])
通过内置的set()可以产生一个空的集合对象，也可以在set中传入一些参数，例如一个列表：
代码如下:
>>> print set([1,2,3,3])
set([1, 2, 3])
最常用的方法就是add和remove了，更多内容可以参考http://docs.python.org/2/library/stdtypes.html\#set。
一个简单的观察者模式的实现
代码如下:
class Observer(object):
def __init__(self, s):
self.s = s
def update(self):
print self.sif __name__ == '__main__':
foo01 = Observer("hi, i am foo01")
foo02 = Observer("hi, i am foo02")
observers = set()
observers.add(foo01)
observers.add(foo01)
observers.add(foo02)
print observers
for ob in observers:
ob.update()
下面是运行结果：
代码如下:
set([<__main__.Observer object at 0xb74627cc>, <__main__.Observer object at 0xb74627ec>])
hi, i am foo01
hi, i am foo02
运行结果中第一行是集合observers的内容，其包含了两个Observer实例，这些实例所处的内存地址在每次运行时可能有不同。而
代码如下:
for ob in observers:
ob.update()
就可以看成多个观察者产生响应。
当然，这种实现并不好——被观察者也应该是一个实例。
更加完善的观察者模式实现
代码如下:
class ObserverInterface(object):
def __init__(self):
pass
def update(self):
passclass SubjectInterface(object):
def __init__(self):
self.observers = set()
def addObserver(self, ob):
self.observers.add(ob)
def delObserver(self, ob):
self.observers.remove(ob)
def notifyObservers(self):
for ob in self.observers:
ob.update()
class Observer01(ObserverInterface):
def __init__(self, s):
self.s = s
def update(self):
print self.s
class Observer02(ObserverInterface):
def __init__(self, num1, num2):
self.num1 = num1
self.num2 = num2
def update(self):
print self.num1 + self.num2
class Subject01(SubjectInterface):
def __init__(self):
SubjectInterface.__init__(self)
if __name__ == '__main__':
ob01 = Observer01("hi, i am ob01")
ob02 = Observer02("hello,","i am ob02")
observers = set()
sb01 = Subject01()
sb01.addObserver(ob01)
sb01.addObserver(ob02)
sb01.notifyObservers()
运行结果如下：
代码如下:
hi, i am ob01
hello,i am ob02


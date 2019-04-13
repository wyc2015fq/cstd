
# Python的操作符重载 - saltriver的专栏 - CSDN博客


2016年08月12日 20:07:43[saltriver](https://me.csdn.net/saltriver)阅读数：533个人分类：[Python																](https://blog.csdn.net/saltriver/article/category/6363186)



在日常工作中，我们经常需要对自定义的类对象进行操作或比较，其实重载我们一直在用，__init__初始化函数就是最常用使用的重载函数。设我们定义了如下一个类，用来表示平面上的一个点。
classpoint:
def__init__(self,x,y):
self.x = x
self.y = y
其中__init__函数就是一个最常用的重载函数，用来对类对象进行初始化。
如果我们需要对不同的point进行+，-或==判断的算术或逻辑操作，可以重载如下函数：
classpoint:
def__init__(self,x,y):
self.x = x
self.y = y
def__add__(self,other):
returnpoint(self.x + other.x,self.y + other.y)
def__sub__(self,other):
returnpoint(self.x - other.x,self.y - other.y)
def__eq__(self,o):
returnabs(self.x-o.x) <0.0001andabs(self.y-o.y) <0.0001p1= point(4,5)
p2 = point(5,4)
p3 =p1+ p2
p4 =p1- p2
print(p3.x,p3.y)
print(p4.x,p4.y)
print(p1== p2)输出结果：
9 9
-1 1
False
如果需要比较大小，可以重载__gt__,__lt__等函数。
def__gt__(self,other):
returnself.x > other.x
def__lt__(self,other):
returnself.x < other.x
除了算术操作符（+-*/，abs，取负）以及比较操作（>,>=,<,<=,==）外，还有逻辑操作以及序列操作等，详细的可以参见Python的operator模块。




# 举例区分Python中的浅复制与深复制 - jiahaowanhao的博客 - CSDN博客


2018年07月14日 13:55:44[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：27标签：[Python																](https://so.csdn.net/so/search/s.do?q=Python&t=blog)


[举例区分Python中的浅复制与深复制](http://cda.pinggu.org/view/26089.html)
这篇文章主要介绍了举例区分Python中的浅复制与深复制,是Python入门学习中的重要知识,需要的朋友可以参考下
copy模块用于对象的拷贝操作。该模块非常简单，只提供了两个主要的方法： copy.copy 与 copy.deepcopy ，分别表示浅复制与深复制。什么是浅复制，什么是深复制，网上有一卡车一卡车的资料，这里不作详细介绍。复制操作只对复合对象有效。用简单的例子来分别介绍这两个方法。
浅复制只复制对象本身，没有复制该对象所引用的对象。
\#coding=gbk
import copy
l1 = [1, 2, [3, 4]]
l2 = copy.copy(l1)
print l1
print l2
l2[2][0] = 50
print l1
print l2
\#---- 结果 ----
[1, 2, [3, 4]]
[1, 2, [3, 4]]
[1, 2, [50, 4]]
[1, 2, [50, 4]]
同样的代码，使用深复制，结果就不一样：
import copy
l1 = [1, 2, [3, 4]]
l2 = copy.deepcopy(l1)
print l1
print l2
l2[2][0] = 50
print l1
print l2
\#---- 结果 ----
[1, 2, [3, 4]]
[1, 2, [3, 4]]
[1, 2, [3, 4]]
[1, 2, [50, 4]]
改变copy的默认行为
在定义类的时候，通过定义__copy__和__deepcopy__方法，可以改变copy的默认行为。下面是一个简单的例子:
class CopyObj(object):
def __repr__(self):
return "CopyObj"
def __copy__(self):
return "Hello"
obj = CopyObj()
obj1 = copy.copy(obj)
print obj
print obj1
\#---- 结果 ----
CopyObj
Hello


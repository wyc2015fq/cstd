# 【python】super()和__init__()中的用法 - zkq_1986的博客 - CSDN博客





2018年11月15日 18:47:38[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：386
个人分类：[程序设计语言](https://blog.csdn.net/zkq_1986/article/category/6534515)









### 单继承时super()和__init__()实现的功能是类似的

class Base(object):
    def __init__(self):
        print 'Base create'
class childA(Base):
    def __init__(self):
        print 'creat A ',
        Base.__init__(self)
class childB(Base):
    def __init__(self):
        print 'creat B ',
        super(childB, self).__init__()
base = Base()
a = childA()
b = childB()
输出结果：

Base create
creat A  Base create
creat B  Base create
区别是使用super()继承时不用显式引用基类。



### super()只能用于新式类中



把基类改为旧式类，即不继承任何基类

class Base():
    def __init__(self):
        print 'Base create'
执行时，在初始化b时就会报错：

super(childB, self).__init__()
TypeError: must be type, not classobj


class A(object):
    def __init__(self):
        print('base')


class B(A):
    def __init__(self,a):
        super(B,self).__init__()
        print('b:a',a)

if __name__ == '__main__':
    b = B(3)

输出：

base

b:a 3

转载自：[https://www.imooc.com/article/50836](https://www.imooc.com/article/50836)




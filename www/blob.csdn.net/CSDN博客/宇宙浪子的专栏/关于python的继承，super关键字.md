# 关于python的继承，super关键字 - 宇宙浪子的专栏 - CSDN博客
2014年12月08日 17:15:44[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：875

# [关于python的继承，super关键字](http://blog.csdn.net/zwan0518/article/details/8709616)
在写排序算法的时候，想到使用继承来定义一个顶层父类，来为各种排序算法定义一个算法框架，然后让各种排序算法继承基类，覆盖算法规则，实现自己的排序。所以稍微学习了python的继承实现机制
首先是顶层的父类：
**[python]**[view
 plain](http://blog.csdn.net/zwan0518/article/details/8709616#)[copy](http://blog.csdn.net/zwan0518/article/details/8709616#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/333934/fork)
- # -*- coding:utf-8 -*-
- #所有排序方法的基类
- import random  
- class MySort(object):  
- def __init__(self, name, initial_len):  
- self.name = name  
- self.initial_len = initial_len  
- print"init..."
- def init(self):  
- self.data = []  
-         i = self.initial_len  
- while (i > 0):  
-             i = i - 1
-             r = random.randint(0, 99)  
- self.data.append(r)  
- returnself.data  
- 
- def sort(self):  
- pass
- 
- def outName(self):  
- printself.name  
- 
- def outData(self):  
- printself.data  
然后是具体的子类归并排序，非递归实现：
**[python]**[view
 plain](http://blog.csdn.net/zwan0518/article/details/8709616#)[copy](http://blog.csdn.net/zwan0518/article/details/8709616#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/333934/fork)
- #-*- coding:utf-8 -*-
- import MySort  
- class MergeSort(MySort.MySort):  
- def __init__(self):  
-         super(MergeSort, self).__init__("mergeSort", 11)  
- self.pre()  
- 
- def pre(self):  
- self.data = self.init()  
- printself.data  
- 
- def sort(self):  
-         data = self.data  
-         tmp = [0for i in range(11)]  
-         i = 1#子序列长度
-         size = len(data)  
- while i < size:  
- self.mergeSort(data, tmp, i, size)  
-             i = i * 2
- self.mergeSort(tmp, data, i, size)  
-             i = i * 2
- 
- def mergeSort(self, data, tmp, s, e):  
-         i = 0
- while i + 2 * s < e:  
- self.ms(data, tmp, i, i + s, i + 2 * s)  
-             i = i + 2 * s  
- if i + s < e:  
- self.ms(data, tmp, i, i + s, e)  
- else:  
- while i < e:  
-                 tmp[i] = data[i]  
-                 i = i + 1
- 
- def ms(self, data, tmp, s, m, e):  
-         k = s  
-         len = m  
- while (len - s) > 0and (e - m) > 0:  
- if data[s] < data[m]:  
-                 tmp[k] = data[s]  
-                 s = s + 1
- else:  
-                 tmp[k] = data[m]  
-                 m = m + 1
-             k = k + 1
- if (len - s) > 0:  
- while s < len:  
-                 tmp[k] = data[s]  
-                 k = k + 1
-                 s = s + 1
- else:  
- 
- while m < e:  
-                 tmp[k] = data[m]  
-                 k = k + 1
-                 m = m + 1
- def outName(self):  
-         super(MergeSort,self).outName()  
- 
- def outData(self):  
-         super(MergeSort,self).outData()  
- 
- if __name__ == "__main__":  
-     s = MergeSort()  
-     s.outName()  
-     s.sort()  
-     s.outData()   
其中对于继承来说，如果父类有__init__方法，那么子类定义了__init__方法的时候必须通过super关键字显式调用父类的init的方法，用以对父类进行初始化。对于super，其是python的一个内置方法，可以直接调用。python的所有内置函数可以查看[https://docs.python.org/3.4/library/functions.html](https://docs.python.org/3.4/library/functions.html)。在旧的类风格中调用就是FatherClass.__init__()方法。
当想通过super调用父类初始化方法的时候，必须使用在新的类风格中。新的类风格从python2.2引入，而旧的类风格在python3中将彻底去除。新的类是所有的类都默认继承一个顶层父类object类，这一点类似与java的设计思想。在python3之前，通过原始的创建类的方式是采用的默认的旧风格的类，即通过class ClassName方式创建；要想使用新的类风格，则必须指定一个集成的父类(如果没有，则继承object)，即class ClassName(object)。
这里调用父类的初始化方法就像java中子类中定义了非默认构造函数则必须调用父类的构造函数是一个概念。但是需要注意的是，编译器在你不主动调用的时候，它是不会给出错误警告。
使用super的好处之一，当继承的父类更改之后，不用在每个子类都去修改父类名字。就如本例，MergeSort继承MySort，如果不使用 super(MergeSort,self).outName()来调用顶层方法，那就必须使用MySort.outName()。如果父类改名之后，那必须在每个子类中都修改此处调用才可以。但是使用super的话就不必如此麻烦。
对于一个旧的类风格，当通过子类访问一个属性的时候，比如subClass.value，它首先会在本类中进行查找，查找到则返回；否则去父类查找，如果多个父类，则是从左到有依次进行查找。其实可以从此看出python和java的集成在很多情况下都是差不多的。但是相对与新的类风格，通过子类访问属性的时候就比较复杂。
附录：
一篇比较好的介绍super用法的文章：[http://rhettinger.wordpress.com/2011/05/26/super-considered-super/](http://rhettinger.wordpress.com/2011/05/26/super-considered-super/)

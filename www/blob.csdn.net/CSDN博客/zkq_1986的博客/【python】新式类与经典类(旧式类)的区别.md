# 【python】新式类与经典类(旧式类)的区别 - zkq_1986的博客 - CSDN博客





2018年01月03日 09:44:43[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：308








➤Python
 2.x中默认都是经典类，只有显式继承了object才是新式类


➤Python 3.x中默认都是新式类,经典类被移除，不必显式的继承object

       新式类跟经典类的差别主要是以下几点:


（1） 类和类型合并。类就是类型，类型就是类。





[python][view
 plain](http://blog.csdn.net/u010066807/article/details/46896835#)[copy](http://blog.csdn.net/u010066807/article/details/46896835#)



- # -*- coding:utf-8 -*-  
- 
- class E:    
- #经典类
- pass
- 
- class E1(object):    
- #新式类
- pass
- 
- e = E()  
- print"经典类"
- print e  
- print type(e)  
- print e.__class__  
- 
- print"新式类"
- e1 = E1()  
- print e1  
- print e1.__class__  
- print type(e1)  




[python][view
 plain](http://blog.csdn.net/u010066807/article/details/46896835#)[copy](http://blog.csdn.net/u010066807/article/details/46896835#)



- 经典类  
- <__main__.E instance at 0x0000000002250B08>  
- <type 'instance'>  
- __main__.E  
- 
- 新式类  
- <__main__.E1 object at 0x0000000002248710>  
- <class'__main__.E1'>  
- <class'__main__.E1'>  





我使用的是python 2.7。

（2）新式类的MRO(method
 resolution order 基类搜索顺序)算法采用C3算法广度优先搜索，而旧式类的MRO算法是采用深度优先搜索




## Python2.x中：







[python][view
 plain](http://blog.csdn.net/zimou5581/article/details/53053775#)[copy](http://blog.csdn.net/zimou5581/article/details/53053775#)



- class A:  
- pass
- class B:  
- pass
- class C(B):  
- pass
- class D(C,A):  
- pass

执行顺序为：D->C->B,->A








[python][view
 plain](http://blog.csdn.net/zimou5581/article/details/53053775#)[copy](http://blog.csdn.net/zimou5581/article/details/53053775#)



- class A(object):  
- pass
- class B(object):  
- pass
- class C(object):   
- pass
- class D(A,B,C):   
- pass

执行顺序为： D->A->B->C->Object

（3）新增了一些内置函数和内置属性。比如__slot()__，__getattribute__。







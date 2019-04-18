# python中的接口 - Nicole的博客 - CSDN博客
2018年04月09日 09:05:37[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：95标签：[接口																[python](https://so.csdn.net/so/search/s.do?q=python&t=blog)](https://so.csdn.net/so/search/s.do?q=接口&t=blog)
个人分类：[python 数据处理](https://blog.csdn.net/weixin_39541558/article/category/7563702)
什么是接口 ？
接口只是定义了一些方法，而没有去实现，多用于程序设计时，只是设计需要有什么样的功能，但是并没有实现任何功能，这些功能需要被另一个类（B）继承后，由 类B去实现其中的某个功能或全部功能。
个人的理解，多用于协作开发时，有不同的人在不同的类中实现接口中的各个方法。
在python中接口由抽象类和抽象方法去实现，接口是不能被实例化的，只能被别的类继承去实现相应的功能。
个人觉得接口在python中并没有那么重要，因为如果要继承接口，需要把其中的每个方法全部实现，否则会报编译错误，还不如直接定义一个class，其中的方法实现全部为pass，让子类重写这些函数。
当然如果有强制要求，必须所有的实现类都必须按照接口中的定义写的话，就必须要用接口。
方法一：用抽象类和抽象函数实现方法
```python
#抽象类加抽象方法就等于面向对象编程中的接口  
from abc import ABCMeta,abstractmethod  
  
class interface(object):  
    __metaclass__ = ABCMeta #指定这是一个抽象类  
    @abstractmethod  #抽象方法  
    def Lee(self):  
        pass  
      
    def Marlon(self):  
        pass  
  
  
class RelalizeInterfaceLee(interface):#必须实现interface中的所有函数，否则会编译错误  
    def __init__(self):      
        print '这是接口interface的实现'  
    def Lee(self):  
        print '实现Lee功能'          
    def Marlon(self):  
        pass     
   
  
class RelalizeInterfaceMarlon(interface): #必须实现interface中的所有函数，否则会编译错误  
    def __init__(self):      
        print '这是接口interface的实现'  
    def Lee(self):  
        pass        
    def Marlon(self):  
        print "实现Marlon功能"
```
方法二：用普通类定义接口，
```python
class interface(object): #假设这就是一个接口，接口名可以随意定义，所有的子类不需要实现在这个类中的函数  
        def Lee(self):，  
            pass  
          
        def Marlon(self):  
            pass  
       
    class Realaize_interface(interface):  
        def __init__(self):  
            pass  
        def Lee(self):  
            print "实现接口中的Lee函数"  
              
              
    class Realaize_interface2(interface):  
        def __init__(self):  
            pass  
        def Marlon(self):  
            print "实现接口中的Marlon函数"  
           
    obj=Realaize_interface()  
    obj.Lee()  
      
      
    obj=Realaize_interface2()  
    obj.Marlon()
```

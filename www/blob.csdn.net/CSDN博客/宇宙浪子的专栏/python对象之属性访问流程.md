# python对象之属性访问流程 - 宇宙浪子的专栏 - CSDN博客
2018年01月08日 15:40:16[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：532
转自：[http://blog.csdn.net/xuriwuyun/article/details/12773547](http://blog.csdn.net/xuriwuyun/article/details/12773547)
为什么题目中是python中对象（object）属点击打开链接性，而不是类（class）属性呢？
       这涉及到object与class之间的关系。在C++和Java中，object等同于instance，而与class区分开来。但是在python中一切皆object，function、int、float、class、instance等等都是object。所以这里的‘object’泛指instance、class、以及python自定义的type（如object、int、str等等）。
       据说这些object，在python运行时，都已经加载到内存中。使用id()，查看一下，都返回了一个整数。该整数是对应object在内存中线性地址的十进制表达，所以每一个object都有一个唯一的id。
既然python中一切都是object，那为什么还要有class和instance之分呢？
       虽然python中一切都是object，但是根据object本身的用法不同，可将object分为三类：instance object、type object、metaclass object[1]。
> 
    instance object就是我们一般使用class实例化得到的数据对象，代表具体的数据实例。如：
[python][view
 plain](http://blog.csdn.net/xuriwuyun/article/details/12773547#)[copy](http://blog.csdn.net/xuriwuyun/article/details/12773547#)
- a=int(1)  
- c=1.2
- b=Myclass()  
    type object故名思意就是用作数据抽象的对象，如python自带的int、float、str等和我们自己定义的一般class。这里的type跟我们所说的class很接近，但同样包含python自定义的数据类型。所以也可以称为class object：
[python][view
 plain](http://blog.csdn.net/xuriwuyun/article/details/12773547#)[copy](http://blog.csdn.net/xuriwuyun/article/details/12773547#)
- class Myclass():  
-     class_attr = "class attribute"
- 
- def func(self, a):  
- self.instance_attr=a  
- printself.class_attr  
    metaclass object是一种比较高极，但基本不会用到的对象，包括python自带的type，以及所有继承type的class。该类对象的作用是用来构造class(type object)。可以认为metaclass是一个可调用对象，给它传入特定的参数就可以生成对应的class[3]。使用metaclass来生成class：
[python][view
 plain](http://blog.csdn.net/xuriwuyun/article/details/12773547#)[copy](http://blog.csdn.net/xuriwuyun/article/details/12773547#)
- def func(self, a):  
- self.instance_attr = a  
- printself.class_attr  
- 
- Myclass = type("Myclass", (), {"class_attr": "class attribute", "func": func})  

     在python中，生成一个对象有两种方法：
> 
1 继承（subclass），使用class关键字来定义。通过这个方法可以生成metaclass和type object
2 实例化（initialization），使用type object进行初始化，如a=int(1)
#### 对象属性的访问流程
       上面对python的object做了一个简要的介绍，了解下object与object之间的关系。当通过a=object.attribute去访问对象属性时，返回是object中属性值，还是它所属class的属性值，或者是它所属class的基类的属性值？       python的对象属性访问，可以分为如下几个步骤。无论该对象是class还是instance，都遵守如下流程：
> 
首先，搜索object中除__dict__外的所有属性，如果有匹配的则直接返回该属性（这里应该都是python自己定义的一些属性，如__class__、__doc__）；
其次，搜索object.__class__.__dict__，如果存在且该属性为data descriptor，则返回该descriptor的执行结果。没有则继续搜索object.__class__.__bases__中所有基类的__dict__，依次递归；
再次，搜索object.__dict__，如果该属性存在并为descriptor，且object为class，则执行该descriptor的执行结果，否则直接返回该属性值。如果object为class且__dict__中不存在该属性，则将继续搜索其__bases__；
第四，搜索object.__class__.__dict__，如果属性存在，且为non-data descriptor（注意由于第二步对data descriptor做过处理，所以这里不可能为data descriptor），则返回该descriptor的执行结果，否则直接返回该属性值；
最后，上面都没有找到，则会抛出AttributeError异常。
#### 对象属性的设置流程
       当通过object.attribute=a来设置对象属性时，流程如下：
> 
首先，搜索object.__class.__dict__是否存在，且该属性为data descriptor，如果存在，则执行该descriptor的__set__()，如不存在依次寻找其基类，依次递归；
其次，如果上面没有找到，则直接向object.__dict__赋值结果。如果object为class且该属性为data descriptor，则调用该descriptor的__set__函数。如果object为class，且它的__dict__及基类的__dict__都不包含该属性的data descriptor，则直接赋值于object.__dict__中，而不管基类的__dict__是否也存在该属性。存在一种特殊情况，当object.__class__有属性__slots__（一个list）时，会判断"attribute"是否在__slots__中，如果不在，则无法在object.__dict__中插入结果。
对象属性的删除流程与设置流程基本一致。
#### 类继承与super的使用
       在类继承中，子类可能会重新实现父类的某些方法，并在实现中调用父类的方法，如下所示
[python][view
 plain](http://blog.csdn.net/xuriwuyun/article/details/12773547#)[copy](http://blog.csdn.net/xuriwuyun/article/details/12773547#)
- class A1(object):  
- def func(self,str)  
- print'class A1',str  
- 
- class B1(A1):  
- def func(self, str)  
-         A.func(self, str) #注意此处调用的func是A的 unbound mothod，需要传递self给它
- print'class B1', str  
- 
- b = B1()  
- b.func('hello')  
运行该脚本，输出如下：
[python][view
 plain](http://blog.csdn.net/xuriwuyun/article/details/12773547#)[copy](http://blog.csdn.net/xuriwuyun/article/details/12773547#)
- class A1 hello  
- class B1 hello  
      这种继承由于每一个子类只有唯一的一个父类，所以通过A.func(self, str)调用父类方法不会存在什么问题。但是如果存在下面情况呢：
[python][view
 plain](http://blog.csdn.net/xuriwuyun/article/details/12773547#)[copy](http://blog.csdn.net/xuriwuyun/article/details/12773547#)
- class C1(A1):                                                                       
- def func(self, str):                                                          
-         A1.func(self,str)                                                         
- print'class C1', str                                                       
- 
- class D1(B1, C1):                                                                   
- def func(self, str):                                                            
-         B1.func(self,str)                                                           
-         C1.func(self,str)                                                           
- print'class D1', str   
- d1 = D1()  
- d1.func()  
输出结果如下：
[python][view
 plain](http://blog.csdn.net/xuriwuyun/article/details/12773547#)[copy](http://blog.csdn.net/xuriwuyun/article/details/12773547#)
- class A1 hello  
- class B1 hello  
- class A1 hello  
- class C1 hello  
- class D1 hello  
      从上面可以看出A1的func被调用了两次，但这不是我们想要的，有时这很可能导致出现问题。使用super来调用父类的方法，可以避免出现上述问题。代码如下：
[python][view
 plain](http://blog.csdn.net/xuriwuyun/article/details/12773547#)[copy](http://blog.csdn.net/xuriwuyun/article/details/12773547#)
- class A(object):                                                                  
- def func(self, str):                                                          
- print'class A', str                                                      
- 
- 
- class B(A):                                                                       
- def func(self,str):                                                           
-         super(B, self).func(str)                                                  
- print'class B', str                                                      
- 
- 
- class C(A):                                                                       
- def func(self, str):                                                          
-         super(C, self).func(str)                                                  
- print'class C', str                                                      
- 
- 
- class D(B,C):                                                                     
- def func(self,str):                                                           
-         super(D, self).func(str)                                                  
- print'class D', str                                                      
- 
- d=D()                                                                             
- d.func('hello')  
输出结果如下：
[python][view
 plain](http://blog.csdn.net/xuriwuyun/article/details/12773547#)[copy](http://blog.csdn.net/xuriwuyun/article/details/12773547#)
- class A hello  
- class C hello  
- class B hello  
- class D hello  
注意在D中，只调用了一条super(D,self).func(str)，并没有一一去调用父类的func。但能调用它所有父类的func。而且保证每一父类的函数只执行一遍。
python在每一个class中都添加一个__mro__属性，这个属性存放了该类和所有基类，并按继承顺序存放。如下：
[python][view
 plain](http://blog.csdn.net/xuriwuyun/article/details/12773547#)[copy](http://blog.csdn.net/xuriwuyun/article/details/12773547#)
- A.__mro__  
- (A, object)  
- B.__mro__  
- (B, A, object)  
- C.__mro__  
- (C, A, object)  
- D.__mro__  
- (D, B, C, A, object)  
       当我们执行super(D, self).func(str)时，super首先获取self.__class__.__mro__的值，然后在列表中寻找元素D后面的元素（B），然后调用B.func(self, str)。在B.func中又会执行super(B,self),func(str)。这样它会在__mro__中寻找元素B后面的元素C，接着调用C.func(self,str)。按如此顺序调用所有基类的func函数，不会出现基类函数重复调用的问题。所以在编写代码时，需尽量使用super，而不能使用基类进行直接调用。
参考文档：
[1][http://www.cafepy.com/article/python_types_and_objects/python_types_and_objects.html](http://www.cafepy.com/article/python_types_and_objects/python_types_and_objects.html)
[2][http://www.cafepy.com/article/python_attributes_and_methods/python_attributes_and_methods.html](http://www.cafepy.com/article/python_attributes_and_methods/python_attributes_and_methods.html)
[3][http://stackoverflow.com/questions/100003/what-is-a-metaclass-in-python](http://stackoverflow.com/questions/100003/what-is-a-metaclass-in-python)
[4][http://python-history.blogspot.ru/2010/06/method-resolution-order.html](http://python-history.blogspot.ru/2010/06/method-resolution-order.html)

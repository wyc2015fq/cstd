# Python类中的 私有变量和私有方法 - Nicole的博客 - CSDN博客
2018年04月09日 14:07:38[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：338
版权声明：转载请标注出处。 原文章地址：[点击打开链接。https://blog.csdn.net/sxingming/article/details/52875125](https://blog.csdn.net/sxingming/article/details/52875125)
默认情况下，Python中的成员函数和成员变量都是公开的(public),在python中没有类似public,private等关键词来修饰成员函数和成员变量。
在python中定义私有变量只需要在变量名或函数名前加上 ”__“两个下划线，那么这个函数或变量就是私有的了。
在内部，python使用一种 name mangling 技术，将 __membername替换成 _classname__membername，也就是说，类的内部定义中,
所有以双下划线开始的名字都被"翻译"成前面加上单下划线和类名的形式。
例如：为了保证不能在class之外访问私有变量，Python会在类的内部自动的把我们定义的__spam私有变量的名字替换成为
_classname__spam(注意，classname前面是一个下划线，spam前是两个下划线)，因此，用户在外部访问__spam的时候就会
提示找不到相应的变量。   python中的私有变量和私有方法仍然是可以访问的；访问方法如下：
私有变量:实例._类名__变量名
私有方法:实例._类名__方法名()
其实，Python并没有真正的私有化支持，但可用下划线得到伪私有。   尽量避免定义以下划线开头的变量！
（1）_xxx      "单下划线 " 开始的成员变量叫做保护变量，意思是只有类实例和子类实例能访问到这些变量，
需通过类提供的接口进行访问；不能用'from module import *'导入
（2）__xxx    类中的私有变量/方法名 （Python的函数也是对象，所以成员方法称为成员变量也行得通。）,
" 双下划线 " 开始的是私有成员，意思是只有类对象自己能访问，连子类对象也不能访问到这个数据。
（3）__xxx__ 系统定义名字，前后均有一个“双下划线” 代表python里特殊方法专用的标识，如 __init__（）代表类的构造函数。
下面我们看几个例子：
【1】
```python
class A(object):  
        def __init__(self):  
            self.__data=[]  #翻译成 self._A__data=[]  
        def add(self,item):  
            self.__data.append(item) #翻译成 self._A__data.append(item)  
        def printData(self):  
            print self.__data  #翻译成 self._A__data  
         
    a=A()  
    a.add('hello')  
    a.add('python')  
    a.printData()  
    #print a.__data  #外界不能访问私有变量 AttributeError: 'A' object has no attribute '__data'  
    print a._A__data #通过这种方式，在外面也能够访问“私有”变量；这一点在调试中是比较有用的！
```
运行结果是：
['hello', 'python']
['hello', 'python']
【2】
获取实例的所有属性        print a.__dict__
获取实例的所有属性和方法  print dir(a)
```python
class A():  
        def __init__(self):  
            self.__name='python' #私有变量，翻译成 self._A__name='python'  
        def __say(self): #私有方法,翻译成 def _A__say(self)  
            print self.__name #翻译成 self._A__name  
          
    a=A()  
    #print a.__name #访问私有属性,报错!AttributeError: A instance has no attribute '__name'  
    print a.__dict__ #查询出实例a的属性的集合  
    print a._A__name #这样，就可以访问私有变量了  
    #a.__say()#调用私有方法，报错。AttributeError: A instance has no attribute '__say'  
    print dir(a)#获取实例的所有属性和方法  
    a._A__say() #这样，就可以调用私有方法了
```
运行结果：
{'_A__name': 'python'}
python
['_A__name', '_A__say', '__doc__', '__init__', '__module__']
python
从上面看来,python还是非常的灵活，它的oop没有做到真正的不能访问，只是一种约定让大家去遵守，
比如大家都用self来代表类里的当前对象，其实，我们也可以用其它的，只是大家习惯了用self  。
【3】小漏洞：派生类和基类取相同的名字就可以使用基类的私有变量
```python
class A():  
        def __init__(self):  
            self.__name='python' #翻译成self._A__name='python'  
          
    class B(A):  
        def func(self):  
            print self.__name #翻译成print self._B__name  
      
    instance=B()  
    #instance.func()#报错：AttributeError: B instance has no attribute '_B__name'  
    print instance.__dict__  
    print instance._A__name
```
运行结果：
{'_A__name': 'python'}
```python
class A():  
        def __init__(self):  
            self.__name='python' #翻译成self._A__name='python'  
          
    class A(A): #派生类和基类取相同的名字就可以使用基类的私有变量。  
        def func(self):  
            print self.__name #翻译成print self._A__name  
      
    instance=A()  
    instance.func()
```
运行结果：
python
（完）

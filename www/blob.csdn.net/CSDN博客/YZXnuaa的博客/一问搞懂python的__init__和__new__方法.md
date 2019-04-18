# 一问搞懂python的__init__和__new__方法 - YZXnuaa的博客 - CSDN博客
2019年04月14日 00:34:16[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：8
一、文字概述
__new__是在实例创建之前被调用的，因为它的任务就是创建实例然后返回该实例，是个静态方法。
__init__是当实例对象创建完成后被调用的，然后设置对象属性的一些初始值。
故而“ 本质上 ”来说，__new__（）方法负责创建实例，而__init__()仅仅是负责实例属性相关的初始化而已，执行顺序是，先new后init。
二、new和init的执行顺序
1、当定义类的时候，不定义__new__()方法，这也是我们平时定义类的时候常见的方式。代码如下：
class Student(object):
    def __init__(self,name,age):
        self.name=name
        self.age=age
        print('我是init')
    def study(self):
        print('我爱学习！')
if __name__=='__main__':
    s=Student('张三',25)
    print(s.name)
    print(s.age)
    s.study()
运行结果为：
我是init
张三
25
我爱学习！
-------------------------------------------------------------------------------------------------------------------------------------------------------------
2、当定义类的时候自己定义了__new__()方法，
代码如下：
class Student(object):
    def __new__(cls,*args,**kwargs):
        print('我是new函数！')   #这是为了追踪new的执行过程
        print(type(cls))        #这是为了追踪new的执行过程
        return object.__new__(cls)  #调用父类的（object）的new方法，返回一个Student实例，这个实例传递给init的self参数
    def __init__(self,name,age):
        self.name=name
        self.age=age
        print('我是init')
    def study(self):
        print('我爱学习！')
if __name__=='__main__':
    s=Student('张三',25)
    print(s.name)
    print(s.age)
    s.study()
运行结果为：
我是new函数！
<class 'type'>
我是init
张三
25
我爱学习！
由此可见的确是先执行new，再执行init。
3、从自定义的类继承
前面的Student类是从object继承的，现在从自定义继承，代码如下：
class Parent:
    def __new__(cls,*args,**kwargs):
        print('我是父亲类的new')
        print(type(cls))
        return object.__new__(cls)
    def __init__(self):
        print('我是父亲类的init')
class Children(Parent):
    def __init__(self,name,age):
        self.name=name
        self.age=age
        print('我是子类的init')
if __name__=='__main__':
    child=Children('李四',30)
    print(child.name)
    print(child.age)
运行结果为：
我是父亲类的new
<class 'type'>
我是子类的init
李四
30
从上面可以看出，如果某个类自己未定义new，则执行的是父类的new函数。
三、__new__()一定要有返回值
前面说到，new函数实际上就是创建当前类的实例的，该实例然后传递给init的self参数，进行实例的初始化，故而如果new函数不返回值，则无法构建实例，如下：
class Student(object):
    def __new__(cls,*args,**kwargs):
        print('我是new函数！')   #这是为了追踪new的执行过程
        print(type(cls))        #这是为了追踪new的执行过程
    def __init__(self,name,age):
        self.name=name
        self.age=age
        print('我是init')
    def study(self):
        print('我爱学习！')
if __name__=='__main__':
    s=Student('张三',25)
    print(s.name)
    print(s.age)
    s.study()
运行结果为：
我是new函数！
<class 'type'>
AttributeError: 'NoneType' object has no attribute 'name'
从上面可见，虽然在创建s实例的过程中，调用了new函数，打印出了相关的值，但是因为没有返回值，故而没能够创建实例s，那么在初始化init的时候自然会出错，这就是为什么会显示没有name属性的原因了，因为根本没创建成功！
四、new函数到底定义在哪里？
       实际上，new函数并不是定义在object中，它定义在 元类 type里面，可以通过查看 help(type) 或者是help（type.__new__)
进行查看，__new__是type的成员，而python中所有的类都是type的实例，包括object，故而通过object，Student.__new__()的形式初始化就在正常不过了，这不就是“ 实例.方法 ”吗？
      new的定义如下：
__new__(*args, **kwargs) method of builtins.type instance
    Create and return a new object.  See help(type) for accurate signature. 
这就是为什么很多人在自定义new的时候写成，如下形式，也就不足为怪了：
def __new__(cls,*args,**kwargs):
        print('我是new函数！')   #这是为了追踪new的执行过程
        print(type(cls))        #这是为了追踪new的执行过程
        return object.__new__(cls)  #调用父类的（object）的new方法，返回一个Student实例，这个实例传递给init的self参数
因为这不就是和定义一样吗？虽然多了一个cls，实际上因为是*args的关系，这并不会有影响，这个参数实际上就是要创建的那个类的实例所属的类型，如Student。
五、new的主要应用
   1、__new__方法主要是当你继承一些不可变的class时(比如int, str, tuple)， 提供给你一个自定义这些类的实例化过程的途径
   2、还有就是实现自定义的metaclass
   3、用__new__来实现单例
详细使用会在后面详细说明！
5.1 __new__函数的本质
注意事项：
（1）__new__方法是定义在元类type里面的，作用就是专门创建实例的。
（2）__new__的本质上是一个“类方法”，故而第一个参数为cls，但是因为系统知道它是类方法，所以有不需要显式添加@classmethod
（3）__new__必须具有返回值，否则无法创建对象，因为__init__函数需要这个返回值
（4）自己在定义__new__的时候，参数要与__init__函数的参数匹配，我可以不用到这些参数，但一定要匹配。或者可以使用*arg和**args的形式。
例子1：
class Parent:
    def __new__(cls,*arg):   #这里是正确的，因为*arg可以匹配任意多个未知参数
        return object.__new__(cls)
    def __init__(self,age,name):
        self.age=age
        self.name=name
p=Parent(23,'feifei')
print(p.age)
print(p.name)
例子2：
class Parent:
    def __new__(cls,age,name):   #正确，虽然没用age和name，但也要匹配
        return object.__new__(cls)
    def __init__(self,age,name):
        self.age=age
        self.name=name
p=Parent(23,'feifei')
print(p.age)
print(p.name)
 上面的两个例子产生的结果为
23
feifei
例子3：
class Parent:
    def __new__(cls):   #这是不行的，因为参数不匹配
        return object.__new__(cls)
    def __init__(self,age,name):
        self.age=age
        self.name=name
p=Parent(23,'feifei')
print(p.age)
print(p.name)
下面详细解释创建对象p的过程，
第一步：python会调用Parent.__new__(Parent，23，‘name') 函数，它会返回一个对象，即object.__new__(Parent) 对象，这里的age和name都没有使用。为什么呢？因为__new__方法是定义在type元类中的，而object本身又是type元类的一个对象，（不明白这里的可以看我的文章，关于python元类）这不就是一个简单的   对象.方法(参数)  的简单调用吗？
第二步：再调用__init__函数进行初始化。
六、总结
  1、创建实例的时候，先new后init
  2、new定义在type元类中，必须具有返回值，
  3、new的作用就是创建实例，然后将创建的实例传递给init进行初始化

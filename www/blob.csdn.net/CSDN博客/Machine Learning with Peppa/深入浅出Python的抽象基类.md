# 深入浅出Python的抽象基类 - Machine Learning with Peppa - CSDN博客





2018年04月05日 20:11:23[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：831








### 抽象基类的定义

不同于Java或是C++，python中并没有直接提供ABC，Abstract Base Class（抽象基类）与抽象方法，但是提供了内置模块abc(abstract base class)来模拟实现抽象类。可以通过abc将基类声明为抽象类的方式，然后注册具体类作为这个基类的实现。

抽象基本类的几大特点：
    1：要定义但是并不完整的实现所有方法
    2：基本的意思是作为父类
    3：父类需要明确表示出那些方法的特征，这样在写子类时更加简单明白

用抽象基本类的地方：
    1：用作父类
    2：用作检验实例类型
    3：用作抛出异常说明




### 抽象基类的作用

抽象基类的作用类似于JAVA中的接口。在接口中定义各种方法，然后继承接口的各种类进行具体方法的实现。抽象基类就是定义各种方法而不做具体实现的类，任何继承自抽象基类的类必须实现这些方法，否则无法实例化。
那么抽象基类这样实现的目的是什么呢？ 假设我们在写一个关于动物的代码。涉及到的动物有鸟，狗，牛。首先鸟，狗，牛都是属于动物的。既然是动物那么肯定需要吃饭，发出声音。但是具体到鸟，狗，牛来说吃饭和声音肯定是不同的。
需要具体去实现鸟，狗，牛吃饭和声音的代码。概括一下抽象基类的作用：定义一些共同事物的规则和行为。


来看下具体的代码实现，定义一个抽象基类的简单方法如下： 首先在Dog,Bird,Cow都继承自Animal。 在Animal中定义了eat和voice两个方法






### 抽象基类的使用
    1：直接继承
        直接继承抽象基类的子类就没有这么灵活，抽象基类中可以声明”抽象方法“和“抽象属性”，只有完全覆写（实现）了抽象基类中的“抽象”内容后，才能被实例化，而虚拟子类则不受此影响。
    2：虚拟子类
    将其他的类”注册“到抽象基类下当虚拟子类（调用register方法），虚拟子类的好处是你实现的第三方子类不需要直接继承自基类，可以实现抽象基类中的部分API接口，也可以根本不实现，但是issubclass(), issubinstance()进行判断时仍然返回真值。
    Python 对于ABC的支持模块是abc模块，定义了一个特殊的metaclass：ABCMeta 还有一些装饰器：@abstractmethod 和 @abstarctproperty 。abc.ABCMeta 用于在Python程序中创建抽象基类。而抽象基类如果想要声明“抽象方法”，可以使用 @abstractmethod ，如果想声明“抽象属性”，可以使用 @abstractproperty 。
任何从Animal中继承的子类都必须实现eat和voice方法。否则调用的时候会报错class Animal(object):

```python
def eat(self):
        raise NotImplementedError
    def voice(self):
        raise NotImplementedError

class Dog(Animal):
    def voice(self): 
        print 'wow....'

class Bird(Animal):
    def voice(self):
        print 'jiji....'


class Cow(Animal):
    def voice(self):
        print 'Oh.....'


if __name__ == "__main__":

    d=Dog()
    d.voice()
    d.Eat()
```
执行结果如下， voice可以正常执行，但是eat却报错了

```python
E:\python2.7.11\python.exe E:/py_prj/fluent_python/chapter11.py
wow....
Traceback (most recent call last):
  File "E:/py_prj/fluent_python/chapter11.py", line 54, in <module>
    d.eat()
  File "E:/py_prj/fluent_python/chapter11.py", line 33, in eat
    raise NotImplementedError
NotImplementedError
```
这样实现有个缺点，就是只有子类调用eat方法的时候才会报错。子类是可以正常实例化的。但是你能够想象鸟，狗，牛不会吃饭么？ 如果不会吃饭那肯定不算是动物了。所以正常的实现应该是如果没有实现eat方法，实例化就应该是失败的。那么这里就要用到抽象基类的一般使用方法.代码修改如下：

除了继承，还有一种注册的方法可以将类和抽象基类关联起来：Animal.register(Cat)

```python
Import abc
class Animal(object):
    __metaclass__ = abc.ABCMeta
    @abc.abstractmethod
    def eat(self):
        return
    @abc.abstractmethod
    def voice(self):
        return 
if __name__ == "__main__":
    d=Dog()

结果如下，代码无法实例化，提示没有实现eat方法。这样就完美的达到了我们的目的。
E:\python2.7.11\python.exe E:/py_prj/fluent_python/chapter11.py
Traceback (most recent call last):
  File "E:/py_prj/fluent_python/chapter11.py", line 56, in <module>
    d=Dog()
TypeError: Can't instantiate abstract class Dog with abstract methods eat
 
完整代码修改如下class Animal(object):
    __metaclass__ = abc.ABCMeta

    @abc.abstractmethod
    def eat(self):
        return

    @abc.abstractmethod
    def voice(self):
        return

class Dog(Animal):
    def voice(self):
        print 'wow....

    def eat(self):
        print 'Dog eat....'

class Bird(Animal):
    def voice(self):
        print 'jiji....'
    def eat(self):
        print 'Bird eat....'

class Cow(Animal):
    def voice(self):
        print 'Oh.....'
    def eat(self):
        print 'Cow eat....'

if __name__ == "__main__":
    d=Dog()
    b=Bird()
    c=Cow()
    d.voice()
    d.eat()
    b.voice()
    b.eat()
    c.voice()


    c.eat()
E:\python2.7.11\python.exe E:/py_prj/fluent_python/chapter11.py
wow....
Dog eat....
jiji....
Bird eat....
Oh.....
Cow eat....
```





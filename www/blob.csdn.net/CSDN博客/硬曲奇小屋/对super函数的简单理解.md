# 对super函数的简单理解 - 硬曲奇小屋 - CSDN博客





2017年08月30日 17:23:57[HardCookies](https://me.csdn.net/james_616)阅读数：155








在看Magnus Lie Hetland的《Python基础教程》，虽然进度很慢但终究还是有些收获。在第9章讲构造方法的时候，介绍到super函数能够使子类调用超类的构造方法，以获得其基本的初始化。 

举个例子：

```python
class Bird:
    def __init__(self):
        self.hungry = True
    def eat(self):
        if self.hungry:
            print 'Aaaah...'
            self.hungry = False
        else:
            print 'No, thanks!'
class SongBird(Bird):
    def __init__(self):
        self.sound = 'Squawk!'
    def sing(self):
        print self.sound
```

定义了一个类Bird，初始化的饥饿参数是True。再定义其子类SongBird，它多了一个功能是唱歌，初始化其声音参数是’Squawk!’。虽然SongBird继承了Bird的eat方法，但是没有hungry的特性，这是因为SongBird的构造方法被重写了，没有初始化饥饿参数。

使用super函数可以很好的解决这个问题，但前提是要使用新式类。运行如下代码，可以得到正确结果。

```python
__metaclass__ = type
# Bird should succed from object
class Bird:
    def __init__(self):
        self.hungry = True
    def eat(self):
        if self.hungry:
            print 'Aaaah...'
            self.hungry = False
        else:
            print 'No, thanks!'
class SongBird(Bird):
    def __init__(self):
        super(SongBird, self).__init__()
        self.sound = 'Squawk!'
    def sing(self):
        print self.sound

sb = SongBird()
sb.sing()
sb.eat()
sb.eat()
```

运行结果：

> 
Squawk! 

  Aaaah… 

  No, thanks!
SongBird既能唱歌又能吃东西，能够完成超类的初始化构造方法。 

在使用新式类的时候也可以不加第一行代码，`__metaclass__ = type`，而直接让Bird继承object类，既类的定义为`class Bird(object):`

在运行这段代码的时候也出现了一个小的插曲，本想写这篇博客来说书上给的代码运行有问题，继承object才行。一写代码发现，原来`__metaclass__ = type`的下划线是两道，我在编辑器中两道连在一起没有看出来也是醉了。 

super函数还是很有用的哦！






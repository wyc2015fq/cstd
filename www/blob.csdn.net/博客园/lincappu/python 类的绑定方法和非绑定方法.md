# python 类的绑定方法和非绑定方法 - lincappu - 博客园







# [python 类的绑定方法和非绑定方法](https://www.cnblogs.com/lincappu/p/8303591.html)





## 一、绑定方法

### 　　1.对象的绑定方法

　　首先我们明确一个知识点，凡是类中的方法或函数，默认情况下都是绑定给对象使用的。下面，我们通过实例，来慢慢解析绑定方法的应用。

```
class People:
    def __init__(self,name,age):
        self.name = name
        self.age = age
    def talk(self):
        pass
    
p = People('xiaohua',18)
print(p.talk)

输出结果：
<bound method People.talk of <__main__.People object at 0x000000F802C69358>>
```

　　从上面的输出结果来看，talk()這个类中的方法，是绑定给对象使用的。下面，我在看看另外一种情况。

```
class People:
    def __init__(self,name,age):
        self.name = name
        self.age = age
    def talk():
        pass

p = People('xiaohua',18)
print(p.talk)

输出结果：
<bound method People.talk of <__main__.People object at 0x000000FF68F39358>>
```

　　现在，我们将talk()函数的参数去掉，结果显示与上面是一样。这说明，不管是类中的方法，还是类中函数，默认情况下都是绑定给对象使用的。绑定给对象使用有一种好处，那就是不用手动将对象传入。对象是自动传到类中。如果你不信，我们来看看下面的例子：

```
class People:
    def __init__(self,name,age):
        self.name = name
        self.age = age
    def talk():
        pass

p = People('xiaohua',18)
print(People.talk)
print(p.talk)

输出结果：
<function People.talk at 0x000000C54E3D0A60>  类来调用仅仅是当作函数使用
<bound method People.talk of <__main__.People object at 0x000000C54E249358>>  而对象来调用则为绑定方法
```

　　上面很好说明了，如果类来调用类中的方法，那么这个方法仅仅只是一个函数，那么既然是函数，就不会有自动传值这一功能。来看看下面代码：

```
class People:
    def __init__(self,name,age):
        self.name = name
        self.age = age
    def talk(self):
        pass

p = People('xiaohua',18)
People.talk() 1
p.talk() 2

#代码1处报错
talk() missing 1 required positional argument: 'self'
#代码2处正常
```

　　从上面输出结果来看，当类调用类中的方法时候i，是不会进行自动传值的，也就是说，函数有几个参数，我们就得传递进去几个参数。如果想结果正常运行，那么在类名调用talk()的时候，将参数一一都传递进去。即：

```
People.talk(312312)
```

　　這个参数可以是任意的，但是，必须传递进去。而，当对象调用类中方法时候，则不用传递，如上面的2正常执行。既然知道了区别，那么，我们来看看下面代码：

```
class People:
    def __init__(self,name,age):
        self.name = name
        self.age = age
    def talk():
        pass

p = People('xiaohua',18)
People.talk() 1
p.talk() 2

# 1处正常执行
# 2 处报错
talk() takes 0 positional arguments but 1 was given
```

　　从输出结果来看，People来调用talk()方法时候，并不需要传递参数；而当对象来调用talk()的时候，由于对象调用自己的绑定方法，会自动将对象当作第一个参数传递进去，所以，当类中talk()方法没有带参数时，而你又给它传递了一个，显然是会报错的。

**综上所述，我们可以得出以下结论：　**

**1.凡是类中的方法和函数，都是绑定给对象使用的；**

**　　　　2.绑定方法都有自动传值的功能。传递进去的值，就是对象本身。**

**　　　　3.如果类想调用绑定方法，就必须遵循函数的参数规则，有几个参数，就必须传递几个参数。**

　　聪明的你，可能会问，既然类中的方法都是绑定给对象使用的，那么有没有方法是绑定给类使用的呢？

　　答案是，当然有！

### 　　2.类的绑定方法

　　　既然类中的方法，默认都是绑定给对象使用，那么，我们要采取一点措施，将类中的绑定方法解除对象绑定关系，进而绑定到类上。

　　　在python中，引入了@classmethod方法，将类中的方法绑定到类身上。下面看看代码：

```
class People:
    @classmethod
    def talk(cls):
        pass

p = People()
print(People.talk)

#输出结果
<bound method People.talk of <class '__main__.People'>>
```

　　从上述结果可以看出，我们加上了一个装饰器，将类中绑定给对象的方法，绑定到类身上了。我们之前分析过，如果一个方法绑定到谁身上，那么在调用该函数的时候，将自动将该调用者当作第一个参数传递到函数中。但是，绑定到类的方法与绑定到对象方法有一点点不同：

```
class People:
    def __init__(self,name):
        self.name = name

    @classmethod
    def talk(cls):
        pass

p = People('xiaohua')
print(People.talk)
print(p.talk)

#输出结果
<bound method People.talk of <class '__main__.People'>>
<bound method People.talk of <class '__main__.People'>>
```

　　也就是说，当对象在调用类的绑定方法时，也会默认把类当作参数传递进去！所以下面执行正常，并不会因为這个方法绑定到类身上，而对象调用没有传递参数，报错！

```
class People:
    @classmethod
    def talk(cls):
        pass

p = People()
People.talk()
p.talk()
```

　　但是，如果talk()没有参数，则下面代码均会报错。

```
class People:
    @classmethod
    def talk():
        pass

p = People()
People.talk()
p.talk()
#报错结果
talk() takes 0 positional arguments but 1 was given
```

　　两者报错结果一致，这就说明了，当对象来调用类的绑定方法时，也是自动将类传递进去，并不需遵循函数参数传递的规则。

　　对于类中的绑定方法，也基本上就这两种，不管怎么变化，只要记住以下规则，遇到这种情况，都不会再错。

　　类中方法默认都是绑定给对象使用，当对象调用绑定方法时，会自动将对象作为第一个参数传递进去；而类来调用，则必须遵循函数参数一一对应的规则，有几个参数，就必须传递几个参数。如果一个方法是用了@classmethod装饰器，那么這个方法绑定到类身上，不管是对象来调用还是类调用，都会将类作为第一个参数传递进去。

## 二、非绑定方法

　　上面说了，类中的方法要么是绑定给对象使用，要么是绑定给类使用，那么有没有不绑定给两者使用的函数？

　　答案：当然有，python给我们提供了@staticmethod，可以解除绑定关系，将一个类中的方法，变为一个普通函数。

　　下面，我们来看看代码示例：

```
import hashlib
import time
class MySQL:
    def __init__(self,host,port):
        self.id=self.create_id()
        self.host=host
        self.port=port
    @staticmethod
    def create_id(): #就是一个普通工具
        m=hashlib.md5(str(time.clock()).encode('utf-8'))
        return m.hexdigest()


print(MySQL.create_id) #<function MySQL.create_id at 0x0000000001E6B9D8> #查看结果为普通函数
conn=MySQL('127.0.0.1',3306)
print(conn.create_id) #<function MySQL.create_id at 0x00000000026FB9D8> #查看结果为普通函数
```

　　从上面的输出结果，我们可以看出，使用了@staticmethod装饰了一个函数，那么这个函数跟普通函数没有什么区别。既然是普通函数，那么就遵从函数参数传递规则，有几个参数就传递几个参数。













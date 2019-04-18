# Python入门--20--类、对象 - 默槑 - 博客园







# [Python入门--20--类、对象](https://www.cnblogs.com/modaidai/p/7291029.html)





OO=Object Oriented 面向对象

python是面向对象的编程语言

OO的特征：

　　　　1、封装：把一堆东西都扔到一起，变为一个类

　　　　2、继承：假如一个类里面

　　　　3、多态：不同的类有相同名称的函数，如果你分别调用，也可以。。。好像没什么卵用



1、python的self

你就这样用就好，在class的def的参数中：

class B:

　　def setname(self,name):

　　　　self.name=name

　　def kick(self):

　　　　print('我叫%s,该死的，谁踢我....'%self.name)

你就这样用就好 ，写def的时候第一个参数是self就好

调用的话要这样：b=B('土豆')

　　　　　　　　b=kick()　　　　　　#会有输出



2、python的__init__(self)



class B:

　　def __init__(self,name):

　　　　self.name=name

　　def kick(self):

　　　　print('我叫%s,该死的，谁踢我....'%self.name)

调用的话要这样：b=B('土豆')

　　　　　　　　b=kick()　　　　　　#会有输出

 __init__替代了setname这个函数，以后会详细讲__init__





3、python的私有变量

class Person:

　　__name='zai zhe li'

你现在输出：p=Person()

　　　　　　print(p.__name)          #是会报错的

想输出就要像这样：p=Person()

　　　　　　　　　print(p._Person__name)          #_类名__变量名称





python是面向对象的编程，所以他是有类，也是有对象的，不像单身狗一样，连对象都没有

用class定义一个类，类名的第一个字母必须大写：

class CC：

　　def setxy(self,x,y):

　　　　self.x=x

　　　　self.y=y

　　def printxy(self):

　　　　print(self.x,self.y)



然后：dd=CC()  定义一个CC类的对象，也就类是一个模板，dd是成品，一个模板可以制造出很多个成品。

使用del CC可以删除这个类，蛋是删除后，这个对象依旧还可以用



可以使用

dd.setxy(4,5)，对dd这个对象中的x、y赋值，

dd.__dict__    查看函数赋值情况













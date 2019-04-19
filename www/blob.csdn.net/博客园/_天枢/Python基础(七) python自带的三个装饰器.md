# Python基础(七) python自带的三个装饰器 - _天枢 - 博客园
## [Python基础(七) python自带的三个装饰器](https://www.cnblogs.com/yhleng/p/7777843.html)
2017-11-03 13:49 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7777843)
**说到装饰器，就不得不说python自带的三个装饰器：**
**1、@property   将某函数，做为属性使用**
 @property 修饰，就是将方法，变成一个属性来使用。
```
class A():
    @property
    def pfunc(self):
        return self.value
    @pfunc.setter
    def pfunc(self,value):
        self.value = value
    @property
    def pfunc1(self):
        print('this is property')
if __name__=="__main__":
    A.pfunc = 9
    print A.pfunc
    A.pfunc1
```
**2、@classmethod  修饰类的方式**
**带修饰类方法：cls做为方法的第一个参数，隐式的将类做为对象，传递给方法，调用时无须实例化。**
**普通函数方法：self做为第一个参数，隐式的将类实例传递给方法，调用方法时，类必须实例化。**
```
class A():
    def func(self,x,y):
        return x * y
    @classmethod
    def cfunc(cls,x,y):
        return x * y
```
```
if __name__=="__main__":
    print A().func(5,5)
    print A.cfunc(4,5)
```
**3、@staticmethod  修饰类的方式**
**1)是把函数嵌入到类中的一种方式，函数就属于类，同时表明函数不需要访问这个类**
** 2)使用修饰服，修饰方法，不需要实例化**
```
class A():
    def func(self,x,y):
        return x * y
    @staticmethod
    def sfunc(x,y):
        return x * y
if __name__=="__main__":
    print A.sfunc(6,5)
```
**Linux and python学习交流1,2群已满.**
**Linux and python学习交流3群新开,欢迎加入,一起学习.qq 3群:563227894**
不前进,不倒退,停止的状态是没有的.
一起进步,与君共勉,

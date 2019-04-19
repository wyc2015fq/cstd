# Python基础(十一) 类继承 - _天枢 - 博客园
## [Python基础(十一) 类继承](https://www.cnblogs.com/yhleng/p/7799439.html)
2017-11-07 16:04 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7799439)
**类继承：**
**继承的想法在于，充份利用已有类的功能，在其基础上来扩展来定义新的类。**
**Parent Class(父类) 与 Child Class(子类):**
**被继承的类称为父类，继承的类称为子类，一个父类，可以有多个子类；**
**子类，一旦继承父类，就拥有了父类的属性与方法，根据需要可以进行增删改。**
**这种做法的主要好处之一就是代码重用。**
**示例代码1：**
```
#*_*coding:utf-8*_*
class perent_class(object):
    def __init__(self,name):
        self.name = name
    def printName(self):
        print('你的名字是：'+self.name)
class child_class(perent_class):
    def __init__(self,name,age):
        super(child_class,self).__init__(name) #初始化类super()函数返回类对象
        self.age = age
if __name__=="__main__":
    child_class('xzdylyh','25').printName()
```
输出
```
C:\Python27\python.exe D:/weixin/temp/temp.py
你的名字是：xzdylyh
Process finished with exit code 0
```
**当子类与父类下方法相同时，子类会覆盖父类的方法内容；其实这即是一种重写，也是一种多态。**

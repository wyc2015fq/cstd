# python之单例模式 - _天枢 - 博客园
## [python之单例模式](https://www.cnblogs.com/yhleng/p/10579387.html)
2019-03-22 16:58 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=10579387)
**#单例模式**：有时需要写出高性能的类，那么会采用单例模式。通俗的解释就是类只创建一次实例，贯穿整个生命周期，实现了高性能。
**#1.模块化单例**
#所谓的模块化就是一个单独的.py文件来存储类，这样就是单例模式。
#其原理就是模块被导入时会产生一个.pyc文件，其它文件在导入的时候不会在去调用。直接读取pyc
**#2.装饰器单例模式**
#原理也很简单，用装饰器装饰类。
#装饰器本身实现，传入类对象，进行判读是不是一个实例，如果是直接返回该实例（需要注意是直接返回，不再创建）。
#如果不是一个实例，那么创建该实例，并把该实例保存在一个变量中，用来标记实例已被创建。
**#实现单例模式装饰器**
```
def sing(cls):
    save_instance = {}
    def _wrap(*args, **kwargs):
        #如果cls没在save_instance中说明，cls是第一次创建实例
        if not 'cls' in save_instance:
            save_instance['cls'] = cls(*args, **kwargs) #注意这里的括号，实例化一个类要带括号否则就是类对象
        return save_instance['cls'] #把实例返回。
    return _wrap
```
**示例1：**
```
class Sing_Cls_Demo():
    def __init__(self, x, y):
        self.x = x
        self.y = y
a1 = Sing_Cls_Demo(1, 2)
a2 = Sing_Cls_Demo(3, 4)
print(a1)
print(a2)
```
**#示例1结果：从以下结果可以看出，不是同一个实例对象**
```
# <__main__.Sing_Cls_Demo object at 0x036692B0>
# <__main__.Sing_Cls_Demo object at 0x03669730>
```
**示例2：**
```
@sing
class Sing_Cls_Demo():
    def __init__(self, x, y):
        self.x = x
        self.y = y
a1 = Sing_Cls_Demo(1, 2)
a2 = Sing_Cls_Demo(3, 4)
print(a1)
print(a2)
```
**#示例2结果：给类带上装饰器，从结果看出，两个类初始化，只创建了一个实例。**
```
# <__main__.Sing_Cls_Demo object at 0x034D9050>
# <__main__.Sing_Cls_Demo object at 0x034D9050>
```
**#3类模式的单例**
#原理其实一样，我们先在类中埋一个属性，或者一个变量也可以。之后判断这个是否存在预设标记，存在原样返回。不存在创建返回。
#以下代码只是类实现的一种方例。
```
class Sing():
    def __init__(self, x, y):
        self.x = x
        self.y = y
    #这里我们通过方法来给类做标记。  实例化时也调用该方法。
    @classmethod
    def instance(cls, *args, **kwargs):
        if not hasattr(Sing,'instance_flag'): #判断如果没有这个属性，说明类没被实例化过。
            Sing.instance_flag = Sing(*args, **kwargs)
        return Sing.instance_flag
a1 = Sing.instance(1, 2)
a2 = Sing.instance(3, 4)
print(a1)
print(a2)
```
#输出结果，可以看出是同一实例对象。
```
# <__main__.Sing object at 0x03479730>
# <__main__.Sing object at 0x03479730>
```
#以上存在的问题。如果是多线程下有概率的会出现问题。可以加上线程锁来解决这问题。
#4__new__模式
原理是一样的，不在重复。  在python中类被实例化的时候最先执行的是__new__
文章就是从__new__开始。在这里做判断，第一次实例一下返回，不是第一次原样返回。

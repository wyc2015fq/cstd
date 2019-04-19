# Python基础(十) __init__与__new__区别 - _天枢 - 博客园
## [Python基础(十) __init__与__new__区别](https://www.cnblogs.com/yhleng/p/7779267.html)
2017-11-03 17:28 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7779267)
**__init__与__new__区别：**
**__init__在python，其实是，在实例化之后执行的，用来初始化一些属性，相当于构造函数，但是又不一样**
**细心一些，通过参数会有所发现，其实__init__(self)  self隐式的将，实例传过来。**
**__new__在python中其实是，在实例化之前执行的，这个通过参数一样可以看出**
**__new__(cls)，cls是隐式的传递的类对象，并不是实例。因为__new__的任务就是，创建类实例并返回实例。**
```
class temp(object):
    def __init__(self,txt):
        self.txt = txt
        print '__init__'
    def __new__(cls,txt):
        print '__new__'
        print txt
        return super(temp,cls).__new__(cls)
temp('what?')
```
**结果：**
```
C:\Python27\python.exe D:/weixin/temp/abc.py
__new__
what?
__init__
Process finished with exit code 0
```
**Linux and python学习交流1,2群已满.**
**Linux and python学习交流3群新开,欢迎加入,一起学习.qq 3群:563227894**
不前进,不倒退,停止的状态是没有的.
一起进步,与君共勉,

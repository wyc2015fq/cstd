# Python基础(十二) 类私有成员和保护成员 - _天枢 - 博客园
## [Python基础(十二) 类私有成员和保护成员](https://www.cnblogs.com/yhleng/p/7800088.html)
2017-11-07 17:34 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7800088)
## **[py](http://www.cnblogs.com/yhleng/p/7799439.html)thon中的protected和private**
**python中用**
**_var ；变量名前一个下划线来定义，此变量为保护成员protected，只有类及其子类可以访问。此变量不能通过from XXX import xxx 导入**
**__var;变量名前两个下划线来定义，此变量为私有private，只允许类本身访问，连子类都不可以访问。**
```
class perent_class(object):
    def __init__(self,name,protected,private):
        self.name = name
        self._protected = protected
        self.__private = private
```
**Linux and python学习交流1,2群已满.**
**Linux and python学习交流3群新开,欢迎加入,一起学习.qq 3群:563227894**
不前进,不倒退,停止的状态是没有的.
一起进步,与君共勉,

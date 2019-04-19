# python装饰器扩展之functools.wraps - _天枢 - 博客园
## [python装饰器扩展之functools.wraps](https://www.cnblogs.com/yhleng/p/9804113.html)
2018-10-17 14:50 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9804113)
我们知道函数被装饰器，装饰后，所有的属性，以及内置函数就失效了。
原因是函数类型变成了warpper类型
示例1：不带wraps装饰器示例
```
def warfunc(func):
    def warpper(*args,**kwargs):
        print('warpper demo')
        ret = func(*args,**kwargs)
        return ret
    return warpper
@warfunc
def demofunc():
    print('func demo')
demofunc()
print(demofunc)
```
输出结果：
```
D:\python36\python3.exe D:/untitled4/temp7.py
warpper demo
func demo
<function warfunc.<locals>**.warpper at 0x035432B8>**
```
**从输出结果标记红色部分可以看出，这个函数类型是一个warpper装饰器。那么内置函数也失效了。如help(demofunc())
示例2：带wraps装饰器的
```
from functools import wraps
def warfunc(func):
   ** @wraps(func)    **def warpper(*args,**kwargs):
        print('warpper demo')
        ret = func(*args,**kwargs)
        return ret
    return warpper
@warfunc
def demofunc():
    print('func demo')
demofunc()
print(demofunc)
```
输出结果：
```
D:\python36\python3.exe D:/untitled4/temp7.py
warpper demo
func demo
<function **demofunc at 0x038632B8**>
```
**从输出结果标记红色部分，可以看出，这个类型是一个funciton；需要注意的引用包from functools import wraps

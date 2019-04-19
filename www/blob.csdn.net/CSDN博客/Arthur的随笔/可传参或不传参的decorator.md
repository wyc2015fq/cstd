# 可传参或不传参的decorator - Arthur的随笔 - CSDN博客
2012年12月18日 21:00:40[largetalk](https://me.csdn.net/largetalk)阅读数：843
个人分类：[python](https://blog.csdn.net/largetalk/article/category/715661)
项目中有个需求， 使用了postgres的多个schema，所以需要来回切换，希望有这样一个decorator:
```python
@useSchema #使用默认schema
def operator_db():
    pass
@useSchema(schema) #使用指定schema
def operator_db():
    pass
```
看了下django.db.transaction的commit_manually的实现，自己写了个：
```python
from functools import wraps
def useSchema(using=None):
    if callable(using):
        @wraps(using)
        def inner(*args):
            print 'in callable', using
            return using(*args)
        return inner
    else:
        def _wrap(func):
            @wraps(func)
            def inner(*args):
                print 'not in callable', using
                return func(*args)
            return inner
        return _wrap
@useSchema
def xxx(a, b):
    print a, b
@useSchema('using')
def yyy(a, b):
    print a, b
print xxx.__name__
print yyy.__name__
xxx(1,2)
yyy(1,2)
```
这个有重复代码，改进后版本：
```python
def useSchema(using=None):
    class Inner(object):
        def __init__(self, func, using):
            self.func = func
            self._using = using
            self.__dict__['__name__'] = func.__name__
        def __call__(self, *args):
            print self._using
            return self.func(*args)
    if callable(using):
        return Inner(using, 'default')
    else:
        return lambda x: Inner(x, using)
@useSchema
def xxx(a, b):
    print a, b
@useSchema('using')
def yyy(a, b):
    print a, b
print xxx.__name__
print yyy.__name__
xxx(1,2)
yyy(1,2)
```
但觉得这还不是很完美，如有更好方法请告诉我
            

# python __str__ 和 __repr__ - Orisun - 博客园







# [python __str__ 和 __repr__](https://www.cnblogs.com/zhangchaoyang/articles/4986891.html)





__str__用于打印方便人阅读的信息。__repr__得到的字符串通常用来重新获得实例，即通常情况下inst=eval(repr(inst))是成立的。在IPython中直接输入实例名然后回车，将会调用__repr__，而“print obj”将调用obj的__str__方法。

具体说来有4种情况会调用__str__，另有4种情况会调用__repr__，看下面代码。

```
class Worker(object):
    def __str__(self):
        return '__str__'
    def __repr__(self):
        return '__repr__'

if __name__ == '__main__':
    d=Worker()
    print d 
    print str(d)
    print '%s' % d
    print '{}'.format(d)
    print
    print `d`
    print repr(d)
    print '%r' % d
    print '{!r}'.format(d)
```

输出

```
__str__
__str__
__str__
__str__

__repr__
__repr__
__repr__
__repr__
```















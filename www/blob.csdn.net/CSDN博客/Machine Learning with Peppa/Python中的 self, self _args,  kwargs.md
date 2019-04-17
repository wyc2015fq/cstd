# Python中的*self,*self._args, **kwargs - Machine Learning with Peppa - CSDN博客





2018年04月30日 20:34:47[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：1797








在python中，有些常见方法参数是：*self._args, **kwargs，如：self._target(*self._args, **self._kwargs)。经过查找一些资料，可以归纳为以下两种类型：
- *self._args 表示接受元组类参数；
- **kwargs     表示接受字典类参数；
例子：


```python
def foo(*args, **kwargs):
    print 'args = ', args
    print 'kwargs = ', kwargs
    print '---------------------------------------'
if __name__ == '__main__':
    foo(1,2,3,4)
    foo(a=1,b=2,c=3)
    foo(1,2,3,4, a=1,b=2,c=3)
    foo('a', 1, None, a=1, b='2', c=3)
```

输出结果如下：

```python
args =  (1, 2, 3, 4) 
kwargs =  {} 
--------------------------------------- 
args =  () 
kwargs =  {'a': 1, 'c': 3, 'b': 2} 
--------------------------------------- 
args =  (1, 2, 3, 4) 
kwargs =  {'a': 1, 'c': 3, 'b': 2} 
--------------------------------------- 
args =  ('a', 1, None) 
kwargs =  {'a': 1, 'c': 3, 'b': '2'} 
---------------------------------------
```


可以看到，这两个是python中的可变参数。*args表示任何多个无名参数，它是一个tuple；**kwargs表示关键字参数，它是一个dict。并且同时使用*args和**kwargs时，必须*args参数列要在**kwargs前，像foo(a=1, b='2', c=3, a', 1, None, )这样调用的话，会提示语法错误“SyntaxError: non-keyword arg after keyword arg”

此外，在Python类定义的方法中，有这样一种写法：

```python
class Test_(object):
    def __init__(self, x, y):
        self.__x = x
        self.__y = y

    def __repr__(self):
        class_name = type(self).__name__
        return '{}:({!r}, {!r})'.format(class_name, *self)
        
    def __iter__(self):
        return (i for i in (self.__x, self.__y))
    def test(*self):
        print(self)

    def test2(self):
        print(self)
```

输出如下：

```python
>>>dd.test()
(Test_:(2, 3),)

>>>dd.test2()
Test_:(2, 3)

>>>dd
Test_:(2, 3)
```

从上面的例子可以看出，在方法中传入(*self)，执行打印操作时，返回值是序列（这里是单元素元组），而传入self参数时，会返回我们期待的repr格式




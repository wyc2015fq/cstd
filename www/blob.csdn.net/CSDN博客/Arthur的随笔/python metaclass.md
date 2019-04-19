# python metaclass - Arthur的随笔 - CSDN博客
2011年09月05日 13:56:30[largetalk](https://me.csdn.net/largetalk)阅读数：1940标签：[python																[class																[object																[django																[basic																[oop](https://so.csdn.net/so/search/s.do?q=oop&t=blog)](https://so.csdn.net/so/search/s.do?q=basic&t=blog)](https://so.csdn.net/so/search/s.do?q=django&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[python](https://blog.csdn.net/largetalk/article/category/715661)
学习python这么久对python的metaclass还是一知半解，网上找了找，发现stackoverflow太强大了： 
http://stackoverflow.com/questions/100003/what-is-a-metaclass-in-python
这个回答很详细，太厉害了，这里自己总结一下，免的忘记。
1. 类也是对象（class are object), 类在内存中也占有一段空间，所以
```python
class Foo(object):pass
id(Foo) #有值
Foo.bar = 'bar'  #给类添加一个属性
```
2. 可以动态的创建类
```python
type(class_name, class_parents, class_dict), 比如
def echo_bar(self): print self.bar
Foo = type('Foo', (object, ), {'bar':'bar', 'echo_bar', echo_bar})
```
3. metaclass是创建类的'stuff'，一般也可以理解成对象是类创建的， 类是metaclass创建的。
type is the metaclass Python uses to create all classes behind the scenes.
Everything, and I mean everything, is an object in Python. That includes ints, strings, functions and classes. All of them are objects. And all of them have been created from a class.
You can call it a 'class factory' if you wish.
4. __metaclass__属性
可以给类添加__metaclass__属性，在创建类时， python会查找(在自身，子类，模块中)是否定义了__metaclass__,有则用之来创建类， 否则用type来创建类
所以可以通过定义__metaclass__来改变类的创建方式，__metaclass__可以是任何可执行的东西，所以不一定需要是类。 这里就直接引用他的例子了。
```python
# the metaclass will automatically get passed the same argument
# that you usually pass to `type`
def upper_attr(future_class_name, future_class_parents, future_class_attr):
  """
    Return a class object, with the list of its attribute turned 
    into uppercase.
  """
  # pick up any attribute that doesn't start with '__'
  attrs = ((name, value) for name, value in future_class_attr.items() if not name.startswith('__'))
  # turn them into uppercase
  uppercase_attr = dict((name.upper(), value) for name, value in attrs)
  # let `type` do the class creation
  return type(future_class_name, future_class_parents, uppercase_attr)
__metaclass__ = upper_attr # this will affect all classes in the module
class Foo(object): 
  # we can define __metaclass__ here instead to affect only this class
  bar = 'bip'
print hasattr(Foo, 'bar')
# Out: False
print hasattr(Foo, 'BAR')
# Out: True
f = Foo()
print f.BAR
# Out: 'bip'
```
```python
# remember that `type` is actually a class like `str` and `int`
# so you can inherit from it
class UpperAttrMetaclass(type): 
    # __new__ is the method called before __init__
    # it's the method that creates the object and returns it
    # while __init__ just initializes the object passed as parameter
    # you rarely use __new__, except when you want to control how the object
    # is created.
    # here the created object is the class, and we want to customize it
    # so we override __new__
    # you can do some stuff in __init__ too if you wish
    # some advanced use involves overriding __call__ as well, but we won't
    # see this
    def __new__(upperattr_metaclass, future_class_name, 
                future_class_parents, future_class_attr):
        attrs = ((name, value) for name, value in future_class_attr.items() if not name.startswith('__'))
        uppercase_attr = dict((name.upper(), value) for name, value in attrs)
        return type(future_class_name, future_class_parents, uppercase_attr)
```
```python
class UpperAttrMetaclass(type): 
    def __new__(upperattr_metaclass, future_class_name, 
                future_class_parents, future_class_attr):
        attrs = ((name, value) for name, value in future_class_attr.items() if not name.startswith('__'))
        uppercase_attr = dict((name.upper(), value) for name, value in attrs)
        # reuse the type.__new__ method
        # this is basic OOP, nothing magic in there
        return type.__new__(upperattr_metaclass, future_class_name, 
                            future_class_parents, uppercase_attr)
```
Django中Model使用__metaclass__的例子
```python
class Model(object):
     __metaclass__ = ModelBase
     _deferred = False
.....
```


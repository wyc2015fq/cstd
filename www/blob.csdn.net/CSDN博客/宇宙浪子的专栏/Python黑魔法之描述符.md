# Python黑魔法之描述符 - 宇宙浪子的专栏 - CSDN博客
2018年01月08日 16:47:45[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：100
转自：[http://www.open-open.com/lib/view/open1485737871117.html](http://www.open-open.com/lib/view/open1485737871117.html)
Descriptors(描述符)是Python语言中一个深奥但很重要的一个黑魔法，它被广泛应用于Python语言的内核，熟练掌握描述符将会为Python程序员的工具箱添加一个额外的技巧。本文我将讲述一下描述符的定义以及一些常见的场景，并且在文末会补充一下 __getattr ， __getattribute__ , __getitem__ 这三个同样涉及到属性访问的魔术方法。
## 描述符的定义
```python
descr__get__(self, obj, objtype=None) --> value
descr.__set__(self, obj, value) --> None
descr.__delete__(self, obj) --> None
```
只要一个 object attribute (对象属性)定义了上面三个方法中的任意一个，那么这个类就可以被称为描述符类。
## 描述符基础
下面这个例子中我们创建了一个 RevealAcess 类，并且实现了 __get__ 方法，现在这个类可以被称为一个描述符类。
```python
classRevealAccess(object):
    def__get__(self, obj, objtype):
        print('self in RevealAccess: {}'.format(self))
        print('self: {}\nobj: {}\nobjtype: {}'.format(self, obj, objtype))
classMyClass(object):
    x = RevealAccess()
    deftest(self):
        print('self in MyClass: {}'.format(self))
```
EX1实例属性
接下来我们来看一下 __get__ 方法的各个参数的含义，在下面这个例子中， self 即RevealAccess类的实例x， obj 即MyClass类的实例m， objtype 顾名思义就是MyClass类自身。从输出语句可以看出， m.x 访问描述符 x 会调用 __get__ 方法。
```python
>>> m = MyClass()
>>> m.test()
self in MyClass: <__main__.MyClass object at 0x7f19d4e42160>
>>> m.x
self in RevealAccess: <__main__.RevealAccess object at 0x7f19d4e420f0>
self: <__main__.RevealAccess object at 0x7f19d4e420f0>
obj: <__main__.MyClass object at 0x7f19d4e42160>
objtype: <class'__main__.MyClass'>
```
EX2类属性
如果通过类直接访问属性 x ，那么 obj 接直接为None，这还是比较好理解，因为不存在MyClass的实例。
```python
>>> MyClass.x
self in RevealAccess: <__main__.RevealAccess object at 0x7f53651070f0>
self: <__main__.RevealAccess object at 0x7f53651070f0>
obj: None
objtype: <class'__main__.MyClass'>
```
## 描述符的原理
### 描述符触发
上面这个例子中，我们分别从实例属性和类属性的角度列举了描述符的用法，下面我们来仔细分析一下内部的原理：
- 
如果是对 实例属性 进行访问，实际上调用了基类object的__getattribute__方法，在这个方法中将obj.d转译成了 type(obj).__dict__['d'].__get__(obj, type(obj)) 。
- 
如果是对 类属性 进行访问，相当于调用了元类type的__getattribute__方法，它将cls.d转译成 cls.__dict__['d'].__get__(None, cls) ，这里__get__()的obj为的None，因为不存在实例。
简单讲一下 __getattribute__ 魔术方法，这个方法在我们访问一个对象的属性的时候会被无条件调用，详细的细节比如和 __getattr , __getitem__ 的区别我会在文章的末尾做一个额外的补充，我们暂时并不深究。
### 描述符优先级
首先，描述符分为两种:
- 
如果一个对象同时定义了__get__()和__set__()方法，则这个描述符被称为 data descriptor 。
- 
如果一个对象只定义了__get__()方法，则这个描述符被称为 non-data descriptor 。
我们对属性进行访问的时候存在下面四种情况：
- data descriptor
- instance dict
- non-data descriptor
- __getattr__()
它们的优先级大小是：
```python
data descriptor > instance dict > non-data descriptor > __getattr__()
```
这是什么意思呢？就是说如果实例对象obj中出现了同名的 data descriptor->d 和 instance attribute->d ， obj.d 对属性 d 进行访问的时候，由于data descriptor具有更高的优先级，Python便会调用 type(obj).__dict__['d'].__get__(obj, type(obj)) 而不是调用obj.__dict__[‘d’]。但是如果描述符是个non-data descriptor，Python则会调用 obj.__dict__['d']
 。
## Property
每次使用描述符的时候都定义一个描述符类，这样看起来非常繁琐。Python提供了一种简洁的方式用来向属性添加数据描述符。
```python
property(fget=None, fset=None, fdel=None, doc=None) -> property attribute
```
fget、fset和fdel分别是类的getter、setter和deleter方法。我们通过下面的一个示例来说明如何使用Property：
```python
classAccount(object):
    def__init__(self):
        self._acct_num = None
    defget_acct_num(self):
        return self._acct_num
    defset_acct_num(self, value):
        self._acct_num = value
    defdel_acct_num(self):
        del self._acct_num
    acct_num = property(get_acct_num, set_acct_num, del_acct_num, '_acct_num property.')
```
如果acct是Account的一个实例，acct.acct_num将会调用getter，acct.acct_num = value将调用setter，del acct_num.acct_num将调用deleter。
```python
>>> acct = Account()
>>> acct.acct_num = 1000
>>> acct.acct_num
1000
```
Python也提供了 @property 装饰器，对于简单的应用场景可以使用它来创建属性。一个属性对象拥有getter,setter和deleter装饰器方法，可以使用它们通过对应的被装饰函数的accessor函数创建属性的拷贝。
```python
classAccount(object):
    def__init__(self):
        self._acct_num = None
 @property
     # the _acct_num property. the decorator creates a read-only property
    defacct_num(self):
        return self._acct_num
 @acct_num.setter
    # the _acct_num property setter makes the property writeable
    defset_acct_num(self, value):
        self._acct_num = value
 @acct_num.deleter
    defdel_acct_num(self):
        del self._acct_num
```
如果想让属性只读，只需要去掉setter方法。
## 在运行时创建描述符
我们可以在运行时添加property属性：
```python
classPerson(object):
    defaddProperty(self, attribute):
        # create local setter and getter with a particular attribute name
        getter = lambda self: self._getProperty(attribute)
        setter = lambda self, value: self._setProperty(attribute, value)
        # construct property attribute and add it to the class
        setattr(self.__class__, attribute, property(fget=getter, \
                                                    fset=setter, \
                                                    doc="Auto-generated method"))
    def_setProperty(self, attribute, value):
        print("Setting: {} = {}".format(attribute, value))
        setattr(self, '_' + attribute, value.title())
    def_getProperty(self, attribute):
        print("Getting: {}".format(attribute))
        return getattr(self, '_' + attribute)
```
```python
>>> user = Person()
>>> user.addProperty('name')
>>> user.addProperty('phone')
>>> user.name = 'john smith'
Setting: name = john smith
>>> user.phone = '12345'
Setting: phone = 12345
>>> user.name
Getting: name
'John Smith'
>>> user.__dict__
{'_phone': '12345', '_name': 'John Smith'}
```
## 静态方法和类方法
我们可以使用描述符来模拟Python中的 @staticmethod 和 @classmethod 的实现。我们首先来浏览一下下面这张表：
|Transformation|Called from an Object|Called from a Class|
|----|----|----|
|function|f(obj, *args)|f(*args)|
|staticmethod|f(*args)|f(*args)|
|classmethod|f(type(obj), *args)|f(klass, *args)|
### 静态方法
对于静态方法 f 。 c.f 和 C.f 是等价的，都是直接查询 object.__getattribute__(c, ‘f’) 或者 object.__getattribute__(C, ’f‘) 。静态方法一个明显的特征就是没有 self 变量。
静态方法有什么用呢？假设有一个处理专门数据的容器类，它提供了一些方法来求平均数，中位数等统计数据方式，这些方法都是要依赖于相应的数据的。但是类中可能还有一些方法，并不依赖这些数据，这个时候我们可以将这些方法声明为静态方法，同时这也可以提高代码的可读性。
使用非数据描述符来模拟一下静态方法的实现：
```python
classStaticMethod(object):
    def__init__(self, f):
        self.f = f
    def__get__(self, obj, objtype=None):
        return self.f
```
我们来应用一下：
```python
classMyClass(object):
 @StaticMethod
    defget_x(x):
        return x
print(MyClass.get_x(100))  # output: 100
```
### 类方法
Python的 @classmethod 和 @staticmethod 的用法有些类似，但是还是有些不同，当某些方法只需要得到 类的引用 而不关心类中的相应的数据的时候就需要使用classmethod了。
使用非数据描述符来模拟一下类方法的实现：
```python
classClassMethod(object):
    def__init__(self, f):
        self.f = f
    def__get__(self, obj, klass=None):
        if klass is None:
            klass = type(obj)
        defnewfunc(*args):
            return self.f(klass, *args)
        return newfunc
```
## 其他的魔术方法
首次接触Python魔术方法的时候，我也被 __get__ , __getattribute__ , __getattr__ , __getitem__ 之间的区别困扰到了，它们都是和属性访问相关的魔术方法，其中重写 __getattr__ ， __getitem__ 来构造一个自己的集合类非常的常用，下面我们就通过一些例子来看一下它们的应用。
### __getattr__
Python默认访问类/实例的某个属性都是通过 __getattribute__ 来调用的， __getattribute__ 会被无条件调用，没有找到的话就会调用 __getattr__ 。如果我们要定制某个类，通常情况下我们不应该重写 __getattribute__ ，而是应该重写 __getattr__ ，很少看见重写 __getattribute__ 的情况。
从下面的输出可以看出，当一个属性通过 __getattribute__ 无法找到的时候会调用 __getattr__ 。
```python
In [1]: classTest(object):
    ...:     def__getattribute__(self, item):
    ...:         print('call __getattribute__')
    ...:         return super(Test, self).__getattribute__(item)
    ...:     def__getattr__(self, item):
    ...:         return 'call __getattr__'
    ...:
In [2]: Test().a
call __getattribute__
Out[2]: 'call __getattr__'
```
应用
对于默认的字典，Python只支持以 obj['foo'] 形式来访问，不支持 obj.foo 的形式，我们可以通过重写 __getattr__ 让字典也支持 obj['foo'] 的访问形式，这是一个非常经典常用的用法：
```python
classStorage(dict):
    """
    A Storage object is like a dictionary except `obj.foo` can be used
    in addition to `obj['foo']`.
    """
    def__getattr__(self, key):
        try:
            return self[key]
        except KeyError as k:
            raise AttributeError(k)
    def__setattr__(self, key, value):
        self[key] = value
    def__delattr__(self, key):
        try:
            del self[key]
        except KeyError as k:
            raise AttributeError(k)
    def__repr__(self):
        return '<Storage ' + dict.__repr__(self) + '>'
```
我们来使用一下我们自定义的加强版字典：
```python
>>> s = Storage(a=1)
>>> s['a']
1
>>> s.a
1
>>> s.a = 2
>>> s['a']
2
>>> del s.a
>>> s.a
...
AttributeError: 'a'
```
### __getitem__
getitem用于通过下标 [] 的形式来获取对象中的元素，下面我们通过重写 __getitem__ 来实现一个自己的list。
```python
classMyList(object):
    def__init__(self, *args):
        self.numbers = args
    def__getitem__(self, item):
        return self.numbers[item]
my_list = MyList(1, 2, 3, 4, 6, 5, 3)
print my_list[2]
```
这个实现非常的简陋，不支持slice和step等功能，请读者自行改进，这里我就不重复了。
应用
下面是参考 [requests库](https://github.com/kennethreitz/requests/blob/master/requests/structures.py) 中对于 __getitem__ 的一个使用，我们定制了一个忽略属性大小写的字典类。
程序有些复杂，我稍微解释一下：由于这里比较简单，没有使用描述符的需求，所以使用了 @property 装饰器来代替， lower_keys 的功能是将 实例字典 中的键全部转换成小写并且存储在字典 self._lower_keys 中。重写了 __getitem__ 方法，以后我们访问某个属性首先会将键转换为小写的方式，然后并不会直接访问实例字典，而是会访问字典 self._lower_keys 去查找。赋值/删除操作的时候由于实例字典会进行变更，为了保持 self._lower_keys 和实例字典同步，首先清除
 self._lower_keys 的内容，以后我们重新查找键的时候再调用 __getitem__ 的时候会重新新建一个 self._lower_keys 。
```python
classCaseInsensitiveDict(dict):
 @property
    deflower_keys(self):
        if not hasattr(self, '_lower_keys') or not self._lower_keys:
            self._lower_keys = dict((k.lower(), k) for k in self.keys())
        return self._lower_keys
    def_clear_lower_keys(self):
        if hasattr(self, '_lower_keys'):
            self._lower_keys.clear()
    def__contains__(self, key):
        return key.lower() in self.lower_keys
    def__getitem__(self, key):
        if key in self:
            return dict.__getitem__(self, self.lower_keys[key.lower()])
    def__setitem__(self, key, value):
        dict.__setitem__(self, key, value)
        self._clear_lower_keys()
    def__delitem__(self, key):
        dict.__delitem__(self, key)
        self._lower_keys.clear()
    defget(self, key, default=None):
        if key in self:
            return self[key]
        else:
            return default
```
我们来调用一下这个类：
```python
>>> d = CaseInsensitiveDict()
>>> d['ziwenxie'] = 'ziwenxie'
>>> d['ZiWenXie'] = 'ZiWenXie'
>>> print(d)
{'ZiWenXie': 'ziwenxie', 'ziwenxie': 'ziwenxie'}
>>> print(d['ziwenxie'])
ziwenxie
# d['ZiWenXie'] => d['ziwenxie']
>>> print(d['ZiWenXie'])
ziwenxie
```
来自：http://www.ziwenxie.site/2017/01/29/python-descriptors/
## 扩展阅读
[python进阶必读汇总](http://www.open-open.com/lib/view/open1447488470163.html)
[python之POP详解](http://www.open-open.com/lib/view/open1335967152733.html)
[Python之SMTP详解](http://www.open-open.com/lib/view/open1335967112015.html)
[Python实战之KNN实现](http://www.open-open.com/lib/view/open1449667580457.html)
[Python之PyChart画图方法](http://www.open-open.com/lib/view/open1345769232704.html)
## 为您推荐
[如何在Python中使用ZeroMQ和Docker构建微服务架构](http://www.open-open.com/lib/view/open1456657314000.html)
[Python编程规范及性能优化](http://www.open-open.com/lib/view/open1358402790652.html)
[机器学习框架、库和软件集合：awesome-machine-learning](http://www.open-open.com/lib/view/open1413446193575.html)
[机器学习开源项目、类库、软件集合](http://www.open-open.com/lib/view/open1412995833374.html)
[awesome-machine-learning
 - 非常棒的机器学习框架，库和软件集合](http://www.open-open.com/lib/view/open1454732802308.html)

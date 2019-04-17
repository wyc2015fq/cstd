# python class 一点总结 - Keith - CSDN博客





2017年04月13日 14:00:12[ke1th](https://me.csdn.net/u012436149)阅读数：1546








# Python class 总结

## 细数class中的 `__**__`
- `__init__(self, *values)`

对象的初始化函数,初始化类的实例时,会调用这个方法
- `__str__(self)`

返回 print对象时要打印的东西,pirnt(obj)时会调用这个方法
- `__iter__(self)` 与 `__next__(self)`

将对象 变为可迭代对象,`__iter__()`用于`iter()`,`__next__`用于`next()`
- `__getitem__(self, key)`

使得对象可以向类表一样使用,obj[2], obj[2:4]
- `__setitem__(self, key, value)`

可以使对象的key被赋值

```python
obj['hello'] = 'world'
```
- `__getattr__(self, attr)`

如果对象没有所调用的属性的时候,就会把属性名送进这个方法,看看这个方法返回什么
- `__getattribute__(self, item)`

所有的属性访问都会经过这个接口

```python
class People(object):
    def __init__(self):
        self.name = "h"

    def __getattribute__(self, item):
        print("get->",item)
        return object.__getattribute__(self,item)

    def __getattr__(self, item):
        print("hello")

    def m(self):
        self.name = 'a'

p = People()
p.m()
print(p.name)
# get-> m
# get-> name
# a
```
- `__setattr__(self, name, value)`

当每次给一个属性赋值的时候,就会调用这个方法

```python
class People(object):
    def __init__(self):
        self.name = "h"

    def __setattr__(self, key, value):
        super(People, self).__setattr__(key,value)
        #self.__dict__[key] = value
        print("hello")
    def m(self):
        self.name = 'a'

p = People()
p.m()
print(p.name)
# hello
# hello
# a
```
- 
`__call__(self, *value)`

使得类的实例可以像函数一样被调用

- 
`__len__(self)`

当执行`len(obj)`时,被调用

- 
`__slots__`

这个和前几个不太一样,它是个类的属性,不是方法,这个用来限制类的实例所能动态添加的属性


## 细数用在class中的装饰器
- `@property`

是一个装饰器,将一个方法当作属性调用
- `@staticmethod`

将方法定义成静态方法,不需要传入`self`了
- `@classmethod`

将一个方法定义成类方法,传入`cls`而不是`self`
- `@abstraction`

将一个方法声明成抽象方法

## 参考资料

[http://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000/00143186739713011a09b63dcbd42cc87f907a778b3ac73000](http://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000/00143186739713011a09b63dcbd42cc87f907a778b3ac73000)
[http://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000/00143186781871161bc8d6497004764b398401a401d4cce000](http://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000/00143186781871161bc8d6497004764b398401a401d4cce000)
[http://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000/0014319098638265527beb24f7840aa97de564ccc7f20f6000](http://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000/0014319098638265527beb24f7840aa97de564ccc7f20f6000)
[https://jeffknupp.com/blog/2014/06/18/improve-your-python-python-classes-and-object-oriented-programming/](https://jeffknupp.com/blog/2014/06/18/improve-your-python-python-classes-and-object-oriented-programming/)
[http://stackoverflow.com/questions/100003/what-is-a-metaclass-in-python](http://stackoverflow.com/questions/100003/what-is-a-metaclass-in-python)
[https://docs.python.org/3/library/functions.html#setattr](https://docs.python.org/3/library/functions.html#setattr)




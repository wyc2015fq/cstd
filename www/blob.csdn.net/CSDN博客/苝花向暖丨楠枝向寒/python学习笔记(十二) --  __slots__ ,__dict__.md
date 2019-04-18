# python学习笔记(十二)  --   __slots__  ,__dict__ - 苝花向暖丨楠枝向寒 - CSDN博客

2018年08月16日 21:56:31[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：120
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



## __dict__

python中默认情况下，每一个类都会有 __dict__这个属性，我们可以通过dir 来查看类结构

```python
>>> class T(object):
	pass

>>> dir(T)
['__class__', '__delattr__', '__dict__', '__dir__', '__doc__', '__eq__', '__format__', '__ge__', '__getattribute__',
 '__gt__', '__hash__', '__init__', '__init_subclass__', '__le__', '__lt__', '__module__', '__ne__', '__new__', 
'__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__']
```

第一行的第三个 就是 __dict__，这个实际上是一个 dict类型的数据，用来存放每一个实例属性的名字以及属性的值

```python
>>> b = T()
>>> b.__dict__    #此时实例中没有绑定属性
{} 
>>> b.name = 1    #绑定属性
>>> b.__dict__    #再次查看
{'name': 1}
```

python会为每一个实例分配一个 __dict__属性，所以这是非常占用内存的，如果说我们在定义类的时候能够确认每个实例的属性，我们可以使用  __slots__。

## __slots__

在类中定义  __slots__(xxx,yyy) 可以限制 实例对属性的动态绑定，例如我括号里 写了 xxx 和 yyy，那么 我动态绑定的时候就只能添加这两个属性，且如果使用__slots__，那么就可以预先开辟出固定的空间，来存放实例的属性，这样相对于__dict__极大的节省了内存空间

```python
>> class T(object):
	__slots__=("name")
	age = 16

	
>>> t = T()
>>> t.age
16
>>> t.name = 'zzh'
>>> 
>>> t.name
'zzh'
>>> 
>>> t.score = 99   #score 不在 __slots__ 设定的范围内
Traceback (most recent call last):
  File "<pyshell#87>", line 1, in <module>
    t.score = 99
AttributeError: 'T' object has no attribute 'score'
>>>
```

我们再通过dir命令来查看一下 该类的结构

```python
>>> dir(T)
['__class__', '__delattr__', '__dir__', '__doc__', '__eq__', '__format__', '__ge__', '__getattribute__', '__gt__', 
'__hash__', '__init__', '__init_subclass__', '__le__', '__lt__', '__module__', '__ne__', '__new__', '__reduce__',
 '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__slots__', '__str__', '__subclasshook__', 'age', 'name']
```

我们可以在最后一行，倒数第五个 看到 __slots__，还有最后两个 'age' 和 'name'，并且此时__dict__没了。__slots__和__dict__只会存在一个。

```python
>>> t.__slots__  # __slots__ 是一个元组，我们定义的时候也可以看得出来
'name'
```

## 注意：一旦使用了__slots__，那么类属性名一定不能与__slots__括号中 属性名相同，否则该属性会变成只读，且实例属性值会受到影响

```python
>>> class T(object):
	__slots__=("name")
	age = 16

	
>>> t = T()
>>> t.name = 'zzh'
>>> t.name
'zzh'
>>> T.name = 'aaa'  #绑定类属性，与__slots__定义的属性名 重名
>>> T.name  
'aaa'
>>> t.name  #绑定的实例属性的值 受到影响，从 zzh 变成了 aaa
'aaa'
>>> t.name = 'bbb'  #再次更改实例属性值，报错，提示只读
Traceback (most recent call last):
  File "<pyshell#98>", line 1, in <module>
    t.name = 'bbb'
AttributeError: 'T' object attribute 'name' is read-only
>>> T.name = 'bbb'  #通过这种方式 确实是可以修改的
>>> T.name
'bbb'
>>> t.name
'bbb'
```

## 所以我们应避免 __slots__ 定义的属性名 与 类属性 重名


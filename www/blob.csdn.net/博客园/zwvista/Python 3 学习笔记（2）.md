# Python 3 学习笔记（2） - zwvista - 博客园

## [Python 3 学习笔记（2）](https://www.cnblogs.com/zwvista/p/7969184.html)

### 输入输出
- str() 函数人类可读，repr() 解释器可读
- rjust() 靠右，ljust() 靠左，center() 居中。zfill() 填0。

### 名字空间
- 名字到对象的映射关系被称为名字空间。
- 名字空间采用字典来实现。
- 不同名字空间中相同的名字各不相关。
- 存在以下名字空间（按照搜索顺序）：
- 最内层函数的局部名字
- 外层函数的非全局非局部名字
- 模块的全局名字
- 语言的内置名字

```
def scope_test():
    def do_local():
        spam = "local spam"

    def do_nonlocal():
        nonlocal spam
        spam = "nonlocal spam"

    def do_global():
        global spam
        spam = "global spam"

    spam = "test spam"
    do_local()
    print("After local assignment:", spam)
    do_nonlocal()
    print("After nonlocal assignment:", spam)
    do_global()
    print("After global assignment:", spam)

scope_test()
print("In global scope:", spam)
```

```
After local assignment: test spam
After nonlocal assignment: nonlocal spam
After global assignment: nonlocal spam
In global scope: global spam
```

### 类（Classes）
- 类的成员缺省为public，方法在子类中均可覆盖（重写）。
- 类名自身是一个对象。
- 类的方法的第一个参数通常是指向类实例自身的 self 参数。
- 方法名本身是一个对象。
- 存在类变量和实例变量。
- 至少两个下划线打头的变量为private变量。
- 内置类型也可以作为基类。
- 存在多重继承。

```
>>> class MyClass:
    """A simple example class"""
    i = 12345
    
    def __init__(self):
        self.data = []
        
    def f(self):
        return 'hello world'

>>> MyClass.i
12345
>>> MyClass.f(None)
'hello world'
>>> MyClass.__doc__
'A simple example class'
>>> x = MyClass()
>>> x.i
12345
>>> x.f()
'hello world'
>>> xf = x.f
>>> print(xf())
hello world
>>> class Complex:
    def __init__(self, realpart, imagpart):
        self.r = realpart
        self.i = imagpart

>>> x = Complex(3.0, -4.5)
>>> x.r, x.i
(3.0, -4.5)
```

### 数据属性（data attributes）和方法（methods）
- 类成员包括数据属性和方法。

上面的例子中

MyClass 是类，MyClass.i 是类对象的数据属性，MyClass.f 是函数对象（function object）。

x 是类的实例，x.data 是实例的数据属性，x.f 是方法对象（method object）。
- 方法的第一个（隐藏）参数是类的实例对象。

x.f() 实际相当于 f(x)。
- 实例对象的属性和方法为实例对象所有，而类对象的属性何方法为类的所有实例对象所共有。

```
>>> class Dog:

    kind = 'canine'         # class variable shared by all instances

    def __init__(self, name):
        self.name = name    # instance variable unique to each instance

>>> d = Dog('Fido')
>>> e = Dog('Buddy')
>>> d.kind                  # shared by all dogs
'canine'
>>> e.kind                  # shared by all dogs
'canine'
>>> d.name                  # unique to d
'Fido'
>>> e.name                  # unique to e
'Buddy'
>>> class Dog:

    def __init__(self, name):
        self.name = name
        self.tricks = []    # creates a new empty list for each dog

    def add_trick(self, trick):
        self.tricks.append(trick)

>>> d = Dog('Fido')
>>> e = Dog('Buddy')
>>> d.add_trick('roll over')
>>> e.add_trick('play dead')
>>> d.tricks
['roll over']
>>> e.tricks
['play dead']
```
- 属性名和方法名冲突时，属性名优先。
- 实例对象可以在类之外添加自己独有的属性。
- 方法可以在类之外定义。

```
# Function defined outside the class
def f1(self, x, y):
    return min(x, x+y)

class C:
    f = f1

    def g(self):
        return 'hello world'

    h = g
```
- 方法可以调用其他方法。

```
class Bag:
    def __init__(self):
        self.data = []

    def add(self, x):
        self.data.append(x)

    def addtwice(self, x):
        self.add(x)
        self.add(x)
```
- 实例对象可以引用自己的类对象 object.__class__
- 空的类定义可以用来模拟其他语言中的记录和结构。

```
class Employee:
    pass

john = Employee()  # Create an empty employee record

# Fill the fields of the record
john.name = 'John Doe'
john.dept = 'computer lab'
john.salary = 1000
```

### 继承
- 继承的语法为 `class DerivedClassName(modname.BaseClassName):`
- 基类中的方法可以在子类中被覆盖（重写）。
- 子类可以通过`BaseClassName.methodname(self, arguments)`调用基类中的（同名）方法。
- isinstance() 检查是否类的实例

如果 obj 是 int 类型或其子类的实例的话，那么 isinstance(obj, int) == True 成立。
- issubclass() 检查是否类的子类

issubclass(bool, int) == True

issubclass(float, int) == False
- 多重继承的语法为`class DerivedClassName(Base1, Base2, Base3):`

### 迭代器

迭代器用例

```
>>> for element in [1, 2, 3]:
    print(element)

1
2
3
>>> for element in (1, 2, 3):
    print(element)

1
2
3
>>> for key in {'one':1, 'two':2}:
    print(key)

one
two
>>> for char in "123":
    print(char)

1
2
3
>>> for line in open("myfile.txt"):
    print(line, end='')
```
- for 循环能够迭代容器 container，是因为容器 container 实现了迭代器的接口。
- for 循环首先调用 iter(container) 得到迭代器对象 iterator。
- for 循环反复调用 next(iterator) 得到容器 container 中的元素。
- 当容器中的元素被迭代完毕时，next(iterator) 抛出 StopIteration 异常，for 循环终止。

迭代器细节

```
>>> s = 'abc'
>>> it = iter(s)
>>> it
<str_iterator object at 0x02C95530>
>>> next(it)
'a'
>>> next(it)
'b'
>>> next(it)
'c'
>>> next(it)
Traceback (most recent call last):
  File "<pyshell#14>", line 1, in <module>
    next(it)
StopIteration
```
- iter(container) 调用 container 的 __iter__ 方法。

该方法返回 iterator 对象。
- next(iterator) 调用 iterator 的 __next__ 方法。

该方法返回下一个元素或者抛出 StopIteration 异常。
- 通常 container 和 iterator 是同一个对象。

迭代器示例

```
>>> class Reverse:
    """Iterator for looping over a sequence backwards."""
    def __init__(self, data):
        self.data = data
        self.index = len(data)

    def __iter__(self):
        return self

    def __next__(self):
        if self.index == 0:
            raise StopIteration
        self.index = self.index - 1
        return self.data[self.index]

>>> rev = Reverse('spam')
>>> iter(rev)
<__main__.Reverse object at 0x02C95670>
>>> for char in rev:
    print(char)

m
a
p
s
```

### 生成器

生成器函数使用 yield 自动实现迭代器接口。

```
>>> def reverse(data):
    for index in range(len(data)-1, -1, -1):
        yield data[index]

>>> for char in reverse('golf'):
    print(char)

f
l
o
g
```

### 生成器表达式

生成器表达式使用小括号加解析表达式自动生成迭代器接口。

```
>>> sum(i*i for i in range(10))                 # sum of squares
285

>>> xvec = [10, 20, 30]
>>> yvec = [7, 5, 3]
>>> sum(x*y for x,y in zip(xvec, yvec))         # dot product
260

>>> from math import pi, sin
>>> sine_table = {x: sin(x*pi/180) for x in range(0, 91)}

>>> unique_words = set(word  for line in page  for word in line.split())

>>> valedictorian = max((student.gpa, student.name) for student in graduates)

>>> data = 'golf'
>>> list(data[i] for i in range(len(data)-1, -1, -1))
['f', 'l', 'o', 'g']
```



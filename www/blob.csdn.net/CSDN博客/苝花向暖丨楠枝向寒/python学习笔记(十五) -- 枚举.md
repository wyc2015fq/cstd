# python学习笔记(十五) -- 枚举 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年08月19日 21:40:56[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：46
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



## 枚举

枚举的作用就是定义一组 数据形式为 dict 但却又不可变的常量

比如我们定义一个类，或者定义一个dict ，类中的成员变量可以被更改，而且类又可以被实例化，dict 里面的值也可以被更改。

如下：

```python
WEEKDAY = {
    'MON': 1,
    'TUS': 2,
    'WEN': 3,
    'THU': 4,
    'FRI': 5
}
class Color:
    RED   = 0
    GREEN = 1
    BLUE  = 2
```

所以就衍生出了枚举，用来声明多组键值对，且一旦声明，其值不允许被修改。

### 声明方式一：

最简单的声明枚举的方式如下：

```python
>>> from enum import Enum
>>> Month = Enum('Month', ('Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'))
```

解释一下 :

首先 枚举类里面存放的是 多组  key = value 形式的 键值对， 每一组  key = value 被称为 一个成员，key称为成员的名字，value称为成员的值。

Enum() 创建一个枚举对象，然后传了两个参数，第一个参数为我们枚举类的类名，第二个参数是一个元组，元组中的每一个元素就是 成员的名字，上面的这种创建枚举对象的方式 每一个成员的值 会被自动赋予，从1开始，然后逐个递增1.

我们通过for循环遍历输出一下

```python
>>> for member in Month:
	print(member)

	
Month.Jan
Month.Feb
Month.Mar
Month.Apr
Month.May
Month.Jun
Month.Jul
Month.Aug
Month.Sep
Month.Oct
Month.Nov
Month.Dec
```

通过上面的输出我们无法看到成员的值，其实我们可以通过member获取到

```python
>>> for member in Month:
	print(member,member.name,member.value)

	
Month.Jan Jan 1
Month.Feb Feb 2
Month.Mar Mar 3
Month.Apr Apr 4
Month.May May 5
Month.Jun Jun 6
Month.Jul Jul 7
Month.Aug Aug 8
Month.Sep Sep 9
Month.Oct Oct 10
Month.Nov Nov 11
Month.Dec Dec 12
```

通过这种方式声明枚举对象不允许出现相同的 成员名

```python
>>> Month = Enum('Month', ('Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec','Dec'))
Traceback (most recent call last):
  File "<pyshell#68>", line 1, in <module>
    Month = Enum('Month', ('Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec','Dec'))
  File "D:\工作软件\python\lib\enum.py", line 309, in __call__
    return cls._create_(value, names, module=module, qualname=qualname, type=type, start=start)
  File "D:\工作软件\python\lib\enum.py", line 420, in _create_
    classdict[member_name] = member_value
  File "D:\工作软件\python\lib\enum.py", line 100, in __setitem__
    raise TypeError('Attempted to reuse key: %r' % key)
TypeError: Attempted to reuse key: 'Dec'
```

 再说一下枚举对象中的值获取方式

1: 枚举对象名(成员值)  获取成员

```python
>>> print(Weekday(6))
Weekday.Sat
```

2：枚举对象名['成员名']  获取成员

```
>>> print(Weekday['Sun'])
Weekday.Sun
```

3：通过 枚举对象名.成员名 直接获取成员

```python
>>> print(Weekday.Sat)
Weekday.Sat
```

4：通过成员.name 和 成员.value 分别获取 成员名 和 成员值 

```python
>>> print(Weekday.Sat.name)
Sat
>>> print(Weekday.Sat.value)
6
```

### 声明方式二：  可以自己设定成员的值

```python
>>> class Weekday(Enum):
	Sun = 0  #默认是从 1开始的，我们自己可以设定从 0 开始
	Mon = 1
	Tue = 2
	Wed = 3
	Thi = 4
	Fri = 5
	Sat = 6
```

同样 也不允许成员名相同

```python
class Weekday(Enum):
	Sun = 0
	Mon = 1
	Tue = 2
	Wed = 3
	Thi = 4
	Fri = 5
	Sat = 6
	Sat = 7

	
Traceback (most recent call last):
  File "<pyshell#71>", line 1, in <module>
    class Weekday(Enum):
  File "<pyshell#71>", line 9, in Weekday
    Sat = 7
  File "D:\工作软件\python\lib\enum.py", line 100, in __setitem__
    raise TypeError('Attempted to reuse key: %r' % key)
TypeError: Attempted to reuse key: 'Sat'
```

但是，因为这种是自己设置成员值，所以可能会出现成员值 相同的情况

```python
class Weekday(Enum):
	Sun = 0
	Mon = 1
	Tue = 2
	Wed = 3
	Thi = 4
	Fri = 5
	Sat = 6
	Sats = 6
```

如果值相同的话，我们通过成员值 去获取 成员的话，只能获取到 第一个成员

```python
>>> print(Weekday(6).name)
Sat
```

通过for 循环迭代的话，也获取不到Sats

```python
>>> for member in Weekday:
	print(member,member.name,member.value)

	
Weekday.Sun Sun 0
Weekday.Mon Mon 1
Weekday.Tue Tue 2
Weekday.Wed Wed 3
Weekday.Thi Thi 4
Weekday.Fri Fri 5
Weekday.Sat Sat 6
```

但是枚举对象中有一个属性  __members__，我们可以通过它获取到值相同的成员。

```python
>>> for name in Weekday.__members__:
	print(name)

	
Sun
Mon
Tue
Wed
Thi
Fri
Sat
Sats
```

上面返回的成员名

Weekday.__members__对象还可以调用 iterms()函数，这样成员所有的信息都可以获取到

```python
>>> for name,member in Weekday.__members__.items():
	print(member,name,member.value)

	
Weekday.Sun Sun 0
Weekday.Mon Mon 1
Weekday.Tue Tue 2
Weekday.Wed Wed 3
Weekday.Thi Thi 4
Weekday.Fri Fri 5
Weekday.Sat Sat 6
Weekday.Sat Sats 6
```

如果想对成员值进行限制，不允许出现重复的成员值，我们需要用到@`unique`装饰器

```python
>>> @unique
class Weekday(Enum):
    Sun = 0 
    Mon = 1
    Tue = 2
    Wed = 3
    Thu = 4
    Fri = 5
    Sat = 6
    Sats = 6

    
Traceback (most recent call last):
  File "<pyshell#102>", line 2, in <module>
    class Weekday(Enum):
  File "D:\工作软件\python\lib\enum.py", line 860, in unique
    (enumeration, alias_details))
ValueError: duplicate values found in <enum 'Weekday'>: Sats -> Sat
```


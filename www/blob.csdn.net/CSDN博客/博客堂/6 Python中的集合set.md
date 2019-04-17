# 6 Python中的集合set - 博客堂 - CSDN博客





2017年03月08日 22:21:26[最小森林](https://me.csdn.net/u012052268)阅读数：1553标签：[python																[set																[集合](https://so.csdn.net/so/search/s.do?q=集合&t=blog)
个人分类：[Python](https://blog.csdn.net/u012052268/article/category/6625160)





## 6 Python中的集合set



- - [Python中的集合set](#6-python中的集合set)

- [6-6 Python中什么是set](#6-6-python中什么是set)
- [6-7 Python之 访问set](#6-7-python之-访问set)
- [6-8 Python之 set的特点](#6-8-python之-set的特点)
- [6-9 Python之 遍历set](#6-9-python之-遍历set)
- [6-10 Python之 更新set](#6-10-python之-更新set)




# 6-6 Python中什么是set

Python中什么是set

set就是**集合**

**dict**的作用是建立一组 key 和一组 value 的映射关系，dict的key是不能重复的。

有的时候，我们只想要 dict 的 key，不关心 key 对应的 value，目的就是保证这个集合的元素不会重复，这时，**set**就派上用场了。

**set** 持有一系列元素，这一点和 list 很像，但是set的元素没有重复，而且是无序的，这点和 dict 的 key很像。

**创建 set 的方式**是调用 set() 并传入一个 list，list的元素将作为set的元素：

```bash
>>> s = set(['A', 'B', 'C'])
```

可以查看 set 的内容：

```python
>>> print s
set(['A', 'C', 'B'])
```

请注意，上述打印的形式类似 list， 但它不是 list，仔细看还可以发现，打印的顺序和原始 list 的顺序有可能是不同的，因为set内部存储的元素是**无序**的。

**因为set不能包含重复的元素**，所以，当我们传入包含重复元素的 list 会怎么样呢？

```python
>>> s = set(['A', 'B', 'C', 'C'])
>>> print s
set(['A', 'C', 'B'])
>>> len(s)
3
```

结果显示，set会自动去掉重复的元素，原来的list有4个元素，但set只有3个元素。

> 
set可以用数学中集合的三要素来记忆：确定性，唯一性，无序性。


**任务**

请用set表示班里的4位同学：

Adam, Lisa, Bart, Paul

**解**：

```bash
s = set(['Adam','Lisa','Bart','Paul'])
```

# 6-7 Python之 访问set

Python之 访问set

由于**set存储的是无序集合**，所以我们没法通过索引来访问。

访问 set中的某个元素实际上就是判断一个元素是否在set中。

例如，存储了班里同学名字的set：

```bash
>>> s = set(['Adam', 'Lisa', 'Bart', 'Paul'])
```

我们可以用 in 操作符判断：

Bart是该班的同学吗？

```python
>>> 'Bart' in s
True
```

Bill是该班的同学吗？

```python
>>> 'Bill' in s
False
```

bart是该班的同学吗？

```python
>>> 'bart' in s
False
```

看来大小写很重要，’Bart’ 和 ‘bart’被认为是两个不同的元素。

**任务**

由于上述set不能识别小写的名字，请改进set，使得 ‘adam’ 和 ‘bart’都能返回True。

**答案**：

```
L=['Adam', 'Lisa', 'Bart', 'Paul']
M=[]
for x in L:
    y=x.lower()
    M.append(y)
s = set(M)
print 'adam' in s
print 'bart' in s
```

# 6-8 Python之 set的特点

Python之 set的特点

**set的内部结构和dict很像，唯一区别是不存储value**，因此，判断一个元素是否在set中速度很快。

**set存储的元素和dict的key类似，必须是不变对象**，因此，任何可变对象是不能放入set中的。

最后，**set存储的元素也是没有顺序的**。

set的这些特点，可以应用在哪些地方呢？

星期一到星期日可以用字符串’MON’, ‘TUE’, … ‘SUN’表示。

假设我们让用户输入星期一至星期日的某天，如何判断用户的输入是否是一个有效的星期呢？

可以用 **if** 语句判断，但这样做非常繁琐：

```
x = '???' # 用户输入的字符串
if x!= 'MON' and x!= 'TUE' and x!= 'WED' ... and x!= 'SUN':
    print 'input error'
else:
    print 'input ok'
```

**注意**：if 语句中的…表示没有列出的其它星期名称，测试时，请输入完整。

如果事先创建好一个set，包含’MON’ ~ ‘SUN’：

```bash
weekdays = set(['MON', 'TUE', 'WED', 'THU', 'FRI', 'SAT', 'SUN'])
```

再判断输入是否有效，只需要判断该字符串是否在set中：

```python
x = '???' # 用户输入的字符串
if x in weekdays:
    print 'input ok'
else:
    print 'input error'
```

这样一来，代码就简单多了。

**任务**

月份也可以用set表示，请设计一个set并判断用户输入的月份是否有效。

月份可以用字符串’Jan’, ‘Feb’, …表示。

**答案**：

```
months = set(['Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec'])
x1 = 'Feb'
x2 = 'Sun'

if x1 in months:
    print 'x1: ok'
else:
    print 'x1: error'

if x2 in months:
    print 'x2: ok'
else:
    print 'x2: error'
```

# 6-9 Python之 遍历set

ython之 遍历set

由于 set 也是一个集合，所以，遍历 set 和遍历 list 类似，都可以通过 for 循环实现。

直接使用 for 循环可以遍历 set 的元素：

```
>>> s = set(['Adam', 'Lisa', 'Bart'])
>>> for name in s:
...     print name
... 
Lisa
Adam
Bart
```

**注意**: 观察 for 循环在遍历set时，元素的顺序和list的顺序很可能是不同的，而且不同的机器上运行的结果也可能不同。

**任务**

请用 for 循环遍历如下的set，打印出 name: score 来。

```bash
s = set([('Adam', 95), ('Lisa', 85), ('Bart', 59)])
```

**答案**

```
#-*- coding:utf-8 -*-
s = set([('Adam', 95), ('Lisa', 85), ('Bart', 59)])
for x in s:
    print x[0],':',x[1]
# 由于 set 里面的每一个元素都是 tuple 类型数据，所以可以对每个 set 里面的元素使用tuple 元素访问方式访问并读取
```

# 6-10 Python之 更新set

Python之 更新set

由于set存储的是一组不重复的无序元素，因此，更新set主要做两件事：

一是把新的元素添加到set中，二是把已有元素从set中删除。

添加元素时，用set的add()方法：

```python
>>> s = set([1, 2, 3])
>>> s.add(4)
>>> print s
set([1, 2, 3, 4])
```

如果添加的元素已经存在于set中，add()不会报错，但是不会加进去了：

```python
>>> s = set([1, 2, 3])
>>> s.add(3)
>>> print s
set([1, 2, 3])
```

删除set中的元素时，用set的remove()方法：

```python
>>> s = set([1, 2, 3, 4])
>>> s.remove(4)
>>> print s
set([1, 2, 3])
```

如果删除的元素不存在set中，remove()会报错：

```
>>> s = set([1, 2, 3])
>>> s.remove(4)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
KeyError: 4
```

所以用add()可以直接添加，而remove()前需要判断。

**任务**

针对下面的set，给定一个list，对list中的每一个元素，如果在set中，就将其删除，如果不在set中，就添加进去。

```bash
s = set(['Adam', 'Paul'])
L = ['Adam', 'Lisa', 'Bart', 'Paul']
```

**答案**：

```
s = set(['Adam', 'Lisa', 'Paul'])
L = ['Adam', 'Lisa', 'Bart', 'Paul']
for x in L:
    if x in s :
        s.remove(x)
    else:
        s.add(x)
print (s)
```](https://so.csdn.net/so/search/s.do?q=set&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)





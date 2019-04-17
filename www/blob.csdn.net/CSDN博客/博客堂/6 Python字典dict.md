# 6 Python字典dict - 博客堂 - CSDN博客





2017年03月08日 22:18:58[最小森林](https://me.csdn.net/u012052268)阅读数：835








## 6 Python字典dict



- - [Python字典dict](#6-python字典dict)

- [6-1 Python之什么是dict](#6-1-python之什么是dict)
- [6-2 Python之访问dict](#6-2-python之访问dict)
- [6-3 Python中dict的特点](#6-3-python中dict的特点)
- [6-4 Python更新dict](#6-4-python更新dict)
- [6-5 Python之 遍历dict](#6-5-python之-遍历dict)




# 6-1 Python之什么是dict

Python之什么是dict 

我们已经知道，list 和 tuple 可以用来表示顺序集合，例如，班里同学的名字：
`['Adam', 'Lisa', 'Bart']`
或者考试的成绩列表：
`[95, 85, 59]`
但是，要根据名字找到对应的成绩，用两个 list 表示就不方便。

如果把名字和分数关联起来，组成类似的查找表：

```php
'Adam' ==> 95
'Lisa' ==> 85
'Bart' ==> 59
```

给定一个名字，就可以直接查到分数。

Python的 dict 就是专门干这件事的。用 dict 表示“名字”-“成绩”的查找表如下：

```bash
d = {
    'Adam': 95,
    'Lisa': 85,
    'Bart': 59
}
```

我们把名字称为**key**，对应的成绩称为**value**，dict就是通过 key 来查找 value。

花括号 {} 表示这是一个**dict**，然后按照 key: value, 写出来即可。最后一个 key: value 的逗号可以省略。

由于dict也是集合，**len()** 函数可以计算任意集合的大小：

```python
>>> len(d)
3
```

注意: 一个 key-value 算一个，因此，dict大小为3。

**任务**

新来的Paul同学成绩是 75 分，请编写一个dict，把Paul同学的成绩也加进去。

```bash
d = {
    'Adam': 95,
    'Lisa': 85,
    'Bart': 59
}
```

**答案**

```
d = {
    'Adam':95,
    'Lisa':85,
    'Paul':75,
    'Bart':59,
}
print(d)
```

# 6-2 Python之访问dict

Python之访问dict

我们已经能创建一个dict，用于表示名字和成绩的对应关系：

```bash
d = {
    'Adam': 95,
    'Lisa': 85,
    'Bart': 59
}
```

那么，如何根据名字来查找对应的成绩呢？

可以简单地使用 **d[key]** 的形式来查找对应的 value，这和 list 很像，不同之处是，**list 必须使用索引返回对应的元素，而dict使用key：**

```python
>>> print d['Adam']
95
>>> print d['Paul']
Traceback (most recent call last):
  File "index.py", line 11, in <module>
    print d['Paul']
KeyError: 'Paul'
```

**注意**: 通过 key 访问 dict 的value，只要 key 存在，dict就返回对应的value。如果key不存在，会直接报错：KeyError。

要避免 KeyError 发生，有两个办法：

**一是先判断一下 key 是否存在，用 in 操作符：**

```
if 'Paul' in d:
    print d['Paul']
```

如果 ‘Paul’ 不存在，if语句判断为False，自然不会执行 print d[‘Paul’] ，从而避免了错误。

**二是使用dict本身提供的一个 get 方法，在Key不存在的时候，返回None：**

```python
>>> print d.get('Bart')
59
>>> print d.get('Paul')
None
```

**任务**

根据如下dict：

```bash
d = {
    'Adam': 95,
    'Lisa': 85,
    'Bart': 59
}
```

请打印出：

```
Adam: 95
Lisa: 85
Bart: 59
```

**答案**：

```
d = {

    'Adam': 95,

    'Lisa': 85,

    'Bart': 59

}

print 'Adam:' + str(d.get('Adam'))

print 'Lisa:' + str(d.get('Lisa'))

print 'Bart:' + str(d.get('Bart'))
```

# 6-3 Python中dict的特点

Python中dict的特点
- dict的第一个特点是**查找速度快**，无论dict有10个元素还是10万个元素，**查找速度都一样**。而list的查找速度随着元素增加而逐渐下降。

不过dict的查找速度快不是没有代价的，dict的缺点是**占用内存大**，还会浪费很多内容，list正好相反，占用内存小，但是查找速度慢。

由于dict是按 key 查找，所以，在一个dict中，**key不能重复**。
- dict的第二个特点就是存储的key-value序对是**没有顺序**的！这和list不一样：

```bash
d = {
    'Adam': 95,
    'Lisa': 85,
    'Bart': 59
}
```

当我们试图打印这个dict时：

```python
>>> print d
{'Lisa': 85, 'Adam': 95, 'Bart': 59}
```

打印的顺序不一定是我们创建时的顺序，而且，不同的机器打印的顺序都可能不同，这说明dict内部是**无序**的，不能用dict存储有序的集合。
- dict的第三个特点是作为 key 的元素必须**不可变**，Python的基本类型如字符串、整数、浮点数都是不可变的，都可以作为 key。但是list是可变的，就不能作为 key。

可以试试用list作为key时会报什么样的错误。

不可变这个限制仅作用于key，value是否可变无所谓：

```
{
    '123': [1, 2, 3],  # key 是 str，value是list
    123: '123',  # key 是 int，value 是 str
    ('a', 'b'): True  # key 是 tuple，并且tuple的每个元素都是不可变对象，value是 boolean
}
```

最常用的key还是字符串，因为用起来最方便。

**任务**

请设计一个dict，可以根据分数来查找名字，已知成绩如下：

```
Adam: 95,

Lisa: 85,

Bart: 59.
```

答案:

```
# -*- coding: utf-8 -*-
d = {
    95:'Adam',
    85:'Lisa',
    59:'Bart'
}
```

# 6-4 Python更新dict

Python更新dict

dict是可变的，也就是说，我们可以随时往dict中添加新的 key-value。比如已有dict：

```bash
d = {
    'Adam': 95,
    'Lisa': 85,
    'Bart': 59
}
```

要把新同学’Paul’的成绩 72 加进去，用赋值语句：

```python
>>> d['Paul'] = 72
```

再看看dict的内容：

```python
>>> print d
{'Lisa': 85, 'Paul': 72, 'Adam': 95, 'Bart': 59}
```

如果 key 已经存在，则赋值会用新的 value 替换掉原来的 value：

```python
>>> d['Bart'] = 60
>>> print d
{'Lisa': 85, 'Paul': 72, 'Adam': 95, 'Bart': 60}
```

**任务**

请根据Paul的成绩 72 更新下面的dict：

```bash
d = {

    95: 'Adam',

    85: 'Lisa',

    59: 'Bart'

}
```

**答案**：

```
d = {
    95: 'Adam',
    85: 'Lisa',
    59: 'Bart'
}
d[72] = 'Paul'
print(d)
```

# 6-5 Python之 遍历dict

Python之 遍历dict

由于dict也是一个集合，所以，遍历dict和遍历list类似，都可以通过 for 循环实现。

直接使用for循环可以遍历 dict 的 key：

```
>>> d = { 'Adam': 95, 'Lisa': 85, 'Bart': 59 }
>>> for key in d:
...     print key
... 
Lisa
Adam
Bart
```

由于通过 key 可以获取对应的 value，因此，在循环体内，可以获取到value的值。

**任务**

请用 for 循环遍历如下的dict，打印出 name: score 来。

```bash
d = {
    'Adam': 95,
    'Lisa': 85,
    'Bart': 59
}
```

**答案**

```
d = {
    'Adam': 95,
    'Lisa': 85,
    'Bart': 59
}
for key in d:
    print(key+':'+str(d[key]))
    #   或者：  print(key,':', d[key])
```




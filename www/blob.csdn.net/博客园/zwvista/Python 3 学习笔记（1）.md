# Python 3 学习笔记（1） - zwvista - 博客园

## [Python 3 学习笔记（1）](https://www.cnblogs.com/zwvista/p/7965931.html)

### Python 3.6

### 运算符
- + - * / 四则运算 % 求余 **乘方
- // 用于整除

### 字符串
- 字符串用单引号或双引号括起来。
- 三引号（单引号或双引号均可）表示多行字符串，行末加反斜杠表示换行不算在字符串之内。
- 字符串加上前缀 r 表示原生字符串。
- + 用于连接字符串。
- * 用于重复字符串。
- 相邻的字符串字面量会自动连接。
- 字符串存在片段（slice）语法（使用下标获取子串），但字符串本身不可变。
- 片段语法中下标 0 指向第一个元素，下标 -1 指向最后一个元素。

```
>>> 'a' * 3
'aaa'
>>> 'b' 'a'
'ba'
>>> 'b' + "a"
'ba'
>>> print(r'C:\some\name')
C:\some\name
>>> print("""\
a
b
c\
""")
a
b
c
>>> "abcde"[1:]
'bcde'
>>> "abcde"[:4]
'abcd'
>>> "abcde"[:]
'abcde'
>>> "abcde"[-3:]
'cde'
```

### 列表（Lists）
- 列表：一种成员的类型相同但个数不限的数据结构。
- 列表采用中括号加逗号的特殊语法。
- + 连接两个列表。
- 列表存在片段（slice）语法（使用下标获取子列表），列表片段可变（可以通过赋值被替换）。
- 片段语法中下标 0 指向第一个元素，下标 -1 指向最后一个元素。

```
>>> [1,2,3][1:]
[2, 3]
>>> [1,2,3][-2:]
[2, 3]
>>> a = [1,2,3,4]
>>> a[2:] = [4,5,6]
>>> a
[1, 2, 4, 5, 6]
>>> a[2:] = []
>>> a
[1, 2]
```
- append 方法添加元素。
- extend 方法连接另一个列表。
- insert 方法插入元素。
- remove 方法移除指定元素。
- pop 方法弹出最后面的元素。
- clear 方法清空列表。
- index 方法查找指定元素的下标。
- count 方法返回列表的长度。
- reverse 方法翻转列表。
- copy 方法复制列表。

```
>>> fruits = ['orange', 'apple', 'pear', 'banana', 'kiwi', 'apple', 'banana']
>>> fruits.count('tangerine')
0
>>> fruits.index('banana')
3
>>> fruits.reverse()
>>> fruits
['banana', 'apple', 'kiwi', 'banana', 'pear', 'apple', 'orange']
>>> fruits.sort()
>>> fruits
['apple', 'apple', 'banana', 'banana', 'kiwi', 'orange', 'pear']
```
- 列表解析（List Comprehensions）

```
>>> [x**2 for x in range(10)]
[0, 1, 4, 9, 16, 25, 36, 49, 64, 81]
>>> [(x, y) for x in [1,2,3] for y in [3,1,4] if x != y]
[(1, 3), (1, 4), (2, 3), (2, 1), (2, 4), (3, 1), (3, 4)]
```

### 元组（Tuples）
- 元组：一种成员数量和次序固定但种类不尽相同的数据结构。
- 元组采用小括号加逗号的特殊语法。（最外层元组的小括号可省略）
- 元组是不可变的。
- 元组存在下标语法。
- 存在没有任何成员的元组 () 以及只有一个成员的元组 a, （元素后跟一个逗号表示元组）

```
>>> t = 123,"123"
>>> t[0]
123
>>> t[1]
'123'
>>> u = t, 456
>>> u
((123, '123'), 456)
>>> v = 1,
>>> v
(1,)
```

### 解构语法

```
>>> a,b = [1,2]
>>> a,b
(1, 2)
>>> a,b = (1,2)
>>> a,b
(1, 2)
>>> a,b = b,a
>>> a,b
(2, 1)
```

### 集合（Sets）
- 集合：一种不存在重复元素的哈希表数据结构。
- 集合采用大括号加逗号的特殊语法。
- 常用操作包括求差集，并集，交集，对称差集。
- 存在集合的解析表达式。（set comprehensions）

```
>>> basket = {'apple', 'orange', 'apple', 'pear', 'orange', 'banana'}
>>> basket
{'pear', 'orange', 'banana', 'apple'}
>>> a = set('abracadabra')
>>> b = set('alacazam')
>>> a
{'c', 'a', 'r', 'd', 'b'}
>>> b
{'l', 'c', 'a', 'm', 'z'}
>>> a - b # 差集
{'b', 'd', 'r'}
>>> a | b # 并集
{'m', 'l', 'c', 'a', 'r', 'd', 'b', 'z'}
>>> a & b # 交集
{'c', 'a'}
>>> a ^ b # 对称差集
{'d', 'b', 'l', 'm', 'z', 'r'}
>>> {x for x in 'abracadabra' if x not in 'abc'}
{'d', 'r'}
```

### 字典（Dictionaries）
- 字典：一种存放键值对的哈希表数据结构。
- 字典采用大括号加冒号和逗号的特殊语法。
- 存在字典的解析表达式。（dictionary comprehensions）

```
>>> tel = {'jack': 4098, 'sape': 4139}
>>> tel
{'jack': 4098, 'sape': 4139}
>>> tel['jack']
4098
>>> tel['guido'] = 4127
>>> tel
{'jack': 4098, 'sape': 4139, 'guido': 4127}
>>> {x: x**2 for x in (2, 4, 6)}
{2: 4, 4: 16, 6: 36}
```

### range 函数

```
>>> list(range(5,10))
[5, 6, 7, 8, 9]
>>> list(range(0,10,3))
[0, 3, 6, 9]
>>> list(range(-10,-100,-30))
[-10, -40, -70]
```

### 语句
- while 语句
- for 语句（实质上是 foreach）
- if ... elif ... else 语句
- 代码块采用冒号加缩进来表示
- pass 语句（空语句）
- break continue 语句
- 循环 + else语句（表示循环正常结束，没有被 break）

```
>>> for n in range(2, 10):
    for x in range(2, n):
        if n % x == 0:
            print(n, 'equals', x, '*', n//x)
            break
    else:
        print(n, 'is a prime number')

        
2 is a prime number
3 is a prime number
4 equals 2 * 2
5 is a prime number
6 equals 2 * 3
7 is a prime number
8 equals 2 * 4
9 equals 3 * 3
```

### 函数
- 定义函数使用 def。
- 形参的特殊形式有：带缺省值的形参 arg=value，元组形参 *args，字典形参 **args。
- 实参的特殊形式有：带关键字的实参 kwarg=value，列表/元组实参 *args，字典实参 **args。
- 函数可以嵌套。
- 函数名本身是一个对象。

```
>>> def f(a, b=2): # 带缺省值的形参
    print(a,b)
    
>>> f(3)
3 2
>>> f(a=4) # 带关键字的实参
4 2
>>> d = {"a":3,"b":5}
>>> f(**d) # 字典实参，调用时展开
3 5
>>> def f(k, *args, **kws): # 元组形参，字典形参
    print(k)
    for arg in args:
        print(arg)
    for kw in kws:
        print(kw, ":", kws[kw])

>>> f("a", 1,2,3, c=4,d="abc") # 调用时 1,2,3 打包成元组，c=4,d="abc" 打包成字典
a
1
2
3
c : 4
d : abc
>>> args = [3,6]
>>> list(range(*args)) # 列表实参，调用时展开
[3, 4, 5]
>>> args = (3,6)
>>> list(range(*args)) # 元组实参，调用时展开
[3, 4, 5]
```

### lambda 表达式

```
>>> def make_incrementor(n):
    return lambda x: x + n

>>> f = make_incrementor(42)
>>> f(0)
42
>>> f(1)
43
>>> pairs = [(1, 'one'), (2, 'two'), (3, 'three'), (4, 'four')]
>>> pairs.sort(key=lambda pair: pair[1])
>>> pairs
[(4, 'four'), (1, 'one'), (3, 'three'), (2, 'two')]
```



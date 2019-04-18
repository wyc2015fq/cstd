# python内置的数据结构 - xcw0754 - 博客园
# [python内置的数据结构](https://www.cnblogs.com/xcw0754/p/5061816.html)
## 详解列表List
这里是列表对象方法的清单：
- list.**append**(x)
　　添加一个元素到列表的末尾。相当于a[len(a):] = [x]。
- list.**extend**(L)
　　将给定列表L中的所有元素附加到原列表a的末尾。相当于a[len(a):] = L。
- list.**insert**(i, x)
　　在给定位置插入一个元素。第一个参数是准备插入到其前面的那个元素的索引，所以 a.insert(0, x) 在列表的最前面插入，a.insert(len(a), x) 相当于a.append(x)。
- list.**remove**(x)
　　删除列表中第一个值为 x 的元素。如果没有这样的元素将会报错。
- list.**pop**([i])
　　删除列表中给定位置的元素并返回它。如果未指定索引，a.pop() 删除并返回列表中的最后一个元素。（i 两边的方括号表示这个参数是可选的，而不是要你输入方括号。你会在 Python 参考库中经常看到这种表示法)。
- list.**clear**()
　　删除列表中所有的元素。相当于del a[:]。
- list.**index**(x)
　　返回列表中第一个值为 x 的元素的索引。如果没有这样的元素将会报错。
- list.**count**(x)
　　返回列表中 x 出现的次数。
- list.**sort**(cmp=None, key=None, reverse=False)
　　原地排序列表中的元素。
- list.**reverse**()
　　原地反转列表中的元素。
- list.**copy**()
　　返回列表的一个浅拷贝，等同于a[:]，注意是“浅拷贝”。

### 将列表当作队列使用
　　将列表当作队列使用，此时最先进入的元素第一个取出（FIFO，先进先出）。但是列表用作此目的效率不高，原因是在列表的末尾添加和弹出元素非常快，但是在列表的开头插入或弹出元素却很慢 (因为所有的其他元素必须向后移一位，顺序容器的通病)。
　　如果要实现一个队列，可以使用 [collections.deque](http://python.usyiyi.cn/python_343/library/collections.html#collections.deque)，它设计的目的就是在两端都能够快速添加和弹出元素。例如：
```
1 >>> from collections import deque
 2 >>> queue = deque(["Eric", "John", "Michael"])
 3 >>> queue.append("Terry")           # 加入一个试试
 4 >>> queue.append("Graham")          # 再加一个
 5 >>> queue.popleft()                 # 删除队头
 6 'Eric'
 7 >>> queue.popleft()                 # 再删队头
 8 'John'
 9 >>> queue                           # 看看剩多少个（剩下后3个）
10 deque(['Michael', 'Terry', 'Graham'])
```
### 列表解析
　　列表解析提供了一个生成列表的简洁方法。应用程序通常会从一个序列的每个元素的操作结果生成新的列表，或者生成满足特定条件的元素的子序列。
　　例如，假设我们想要创建一个列表 s： 
```
1 s=[]
2 for x in range(10):
3     s.append(x**2)
4 print(x)
5 print(s)
6 ---下面是打印---
7 9
8 [0, 1, 4, 9, 16, 25, 36, 49, 64, 81]
```
　　注意这个for循环中的被创建(或被重写)的名为x的变量在循环完毕后依然存在。使用如下方法，我们可以计算s的值而不会产生任何的副作用：
```
s = list(map(lambda x: x**2, range(10)))
```
　　或者：
```
s = [x**2 for x in range(10)]
```
　　列表解析由括号[]括起来，括号里面包含一个表达式，表达式后面跟着一个[for](http://python.usyiyi.cn/python_343/reference/compound_stmts.html#for)语句，后面还可以接零个或更多的  [for](http://python.usyiyi.cn/python_343/reference/compound_stmts.html#for) 或 [if](http://python.usyiyi.cn/python_343/reference/compound_stmts.html#if) 语句。结果是一个新的列表，由表达式依据其后面的 [for](http://python.usyiyi.cn/python_343/reference/compound_stmts.html#for) 和 [if](http://python.usyiyi.cn/python_343/reference/compound_stmts.html#if) 字句上下文计算而来的结果构成。例如，下面的 listcomp 组合两个列表中不相等的元素：
```
1 >>> [(x, y)   for x in [1,2,3] for y in [3,1,4]    if x != y]
2 [(1, 3), (1, 4), (2, 3), (2, 1), (2, 4), (3, 1), (3, 4)]
```
　　这与如下代码是等效的：
```
1 >>> combs = []
2 >>> for x in [1,2,3]:
3 ...     for y in [3,1,4]:
4 ...         if x != y:
5 ...             combs.append((x, y))
6 ...
7 >>> combs
8 [(1, 3), (1, 4), (2, 3), (2, 1), (2, 4), (3, 1), (3, 4)]
```
　　有个更高级的应用，考虑下面由三个长度为 4 的列表组成的 3x4 矩阵：
```
1 >>> matrix = [
2 ...     [1, 2, 3, 4],
3 ...     [5, 6, 7, 8],
4 ...     [9, 10, 11, 12],
5 ... ]
```
　　下面的列表解析可以将矩阵进行转置：
```
1 >>> [ [row[i]  for row in matrix]  for i in range(4)]
2 [[1, 5, 9], [2, 6, 10], [3, 7, 11], [4, 8, 12]]
```
　　它是从最外层开始进行解析的，即上面代码的功能等同于如下代码：
```
1 >>> transposed = []
2 >>> for i in range(4):
3 ...     transposed.append([row[i] for row in matrix])
4 ...
5 >>> transposed
6 [[1, 5, 9], [2, 6, 10], [3, 7, 11], [4, 8, 12]]
```
有个方法可以从列表中按索引来删除元素： [del](http://python.usyiyi.cn/python_343/reference/simple_stmts.html#del) 语句。这不同于有返回值的 pop() 方法。[del](http://python.usyiyi.cn/python_343/reference/simple_stmts.html#del) 语句还可以用于从列表中删除切片或清除整个列表 （我们是将空列表赋值给切片）。例如：
```
1 >>> a = [-1, 1, 66.25, 333, 333, 1234.5]
 2 >>> del a[0]
 3 >>> a
 4 [1, 66.25, 333, 333, 1234.5]
 5 >>> del a[2:4]
 6 >>> a
 7 [1, 66.25, 1234.5]
 8 >>> del a[:]
 9 >>> a
10 []
```
　　可以看到，del语句其实是直接对list对象进行操作的，而类似sorted等函数是直接返回一个对象拷贝的，所以并不会改变到原来的list。[del](http://python.usyiyi.cn/python_343/reference/simple_stmts.html#del) 也可以用于删除整个变量：
```
>>> del a
```
　　此后再引用名称 a 将会报错（直到有另一个值被赋给它）。
## 元组和序列
**元组tuple**看起来类似于列表，它们经常用于不同的场景和不同的目的。元组是[不可变的](http://python.usyiyi.cn/python_343/glossary.html#term-immutable)，而列表是 [可变](http://python.usyiyi.cn/python_343/glossary.html#term-mutable) 的，它们的元素通常是通过迭代访问。
　　元组在输出时总是有括号的，以便于正确表达嵌套结构。但在输入时括号可有可无，不过括号经常都是必须的（如果元组是一个子元素）。不能给元组中单独的一个元素赋值（即修改），但可以创建包含可变对象（如列表list）的元组，当然，list的内部是可以变的。
```
1 >>> t = 12345, 54321, 'hello!'    #不带括号也能创建tuple
 2 >>> t
 3 (12345, 54321, 'hello!')
 4 >>> empty = ()    #创建空tuple
 5 >>> singleton = 'hello',    # 包含单个元素的tuple，末尾的逗号
 6 >>> len(empty)
 7 0
 8 >>> len(singleton)
 9 1
10 >>> singleton
11 ('hello',)
```
　　语句t = 12345, 54321, 'hello!' 是一个元组封装的例子： 值12345, 54321 和 'hello!' 被一起放入一个元组。其逆操作也是可以的：
```
>>> x, y, z = t
```
　　但是必须注意，左边的参数个数必须与右边tuple的大小对应，绝不允许多1个或者少1个。否则会产生歧义，比如仅有2个元素的tuple，x=t是代表什么呢？x指向t吗？还是说x取t的首元素？
## 集合
　　Python 还包含了一个数据类型 **集合set**。集合中的元素**不会重复**且**没有顺序**。集合对象还支持并集、 交集、 差和对称差等数学运算。
　　花大括号{}或 [set()](http://python.usyiyi.cn/python_343/library/stdtypes.html#set) 函数可以用于创建集合。注意： 若要创建一个空集必须使用set()，而不能用{}，因为后者将创建一个空的字典dict。以下是一些普通的操作：
```
1 >>> basket = {'apple', 'orange', 'apple', 'pear', 'orange', 'banana'}    #创建一个set
 2 >>> print(basket) 
 3 {'orange', 'banana', 'pear', 'apple'}
 4 >>> 'orange' in basket                 #测试元素是否存在
 5 True
 6 >>> a = set('abracadabra')    #创建两个集合
 7 >>> b = set('alacazam')
 8 >>> a  
 9 {'a', 'r', 'b', 'c', 'd'}
10 >>> a - b    #集合作差
11 {'r', 'd', 'b'}
12 >>> a | b    #求并集
13 {'a', 'c', 'r', 'd', 'b', 'm', 'z', 'l'}
14 >>> a & b   #求交集
15 {'a', 'c'}
16 >>> a ^ b   # 求对称差：将同时存在于a和b中的元素去除
17 {'r', 'd', 'b', 'm', 'z', 'l'}
```
　　和 [列表解析](http://python.usyiyi.cn/python_343/tutorial/datastructures.html#tut-listcomps) 类似，Python 也支持集合解析：
```
1 >>> a = {x for x in 'abracadabra' if x not in 'abc'}
2 >>> a
3 {'r', 'd'}
```
## 字典
　　字典dict是无序的{键:值}对集合，要求是键key必须是唯一的（在同一个字典内）。一对大括号可以创建一个空的字典：{}。大括号中由逗号分隔的 ‘键:值’ 对将成为字典的初始值，打印字典时也是按照这种方式输出。
与序列不同，序列由数字做索引，字典由key做索引。key可以是任意不可变类型，比如字符串和数字永远可以拿来做key。如果元组只包含字符串、 数字或元组，也可以用作key，而如果元组直接或间接地包含任何可变对象，不能用作key。不能用列表作为key，是因为列表可以通过索引、切片或者 append() 和extend() 等多种方法修改自身。
　　dict的主要操作是依据键来存取值，也可以通过del删除 ‘键:值’ 对。如果用一个已经存在的key存储值，以前为该关键字分配的值就会被替代。从一个不存在的key中读取值将会导致错误，抛出异常。
　　list( d.keys() )返回由dict中所有key组成的列表，其中key的顺序是随机的（如果你想它是有序的，只需使用sorted(d.keys())代替）。[](http://python.usyiyi.cn/python_343/tutorial/datastructures.html#id4)要检查某个键是否在字典中，可以使用 [in](http://python.usyiyi.cn/python_343/reference/expressions.html#in) 关键字。下面是一些常见的操作：
```
1 >>> tel = {'jack': 4098, 'sape': 4139}    #创建一个dict
 2 >>> tel['guido'] = 4127    #直接用[]进行索引出key对应的值
 3 >>> tel
 4 {'sape': 4139, 'guido': 4127, 'jack': 4098}
 5 >>> tel['jack']
 6 4098
 7 >>> del tel['sape']    #删除一个元素对
 8 >>> tel['irv'] = 4127    #新增一个元素对
 9 >>> tel
10 {'guido': 4127, 'irv': 4127, 'jack': 4098}
11 >>> list(tel.keys())    #列出所有key
12 ['irv', 'guido', 'jack']
13 >>> 'guido' in tel    #判断key是否已经存在
14 True
15 >>> 'jack' not in tel
16 False
```
　　dict的构造也可以有一些方便的操作，比如构造函数可以直接从‘键-值’ 对的序列中创建字典：
```
1 >>> dict( [('sape', 4139), ('guido', 4127), ('jack', 4098)] )
2 {'sape': 4139, 'jack': 4098, 'guido': 4127}
```
　　注意上面是用一个序列来初始化dict的，list或者tuple皆可，但是其每个元素必须是一个包含'key,value'的序列，同样也可以是tuple或者list。此外，字典解析可以用于从任意键和值表达式创建字典：
```
1 >>> {x: x**2 for x in (2, 4, 6)}
2 {2: 4, 4: 16, 6: 36}
```
　　如果键都是简单的字符串，有时通过关键字参数指定 键-值 对更为方便，但是也仅仅适用于此：
```
1 >>> dict(sape=4139, guido=4127, jack=4098)
2 {'sape': 4139, 'jack': 4098, 'guido': 4127}
```


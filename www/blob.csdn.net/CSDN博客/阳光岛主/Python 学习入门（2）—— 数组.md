
# Python 学习入门（2）—— 数组 - 阳光岛主 - CSDN博客

2013年10月26日 00:00:04[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：17849所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



**[Python](http://baike.baidu.com/link?url=LnVcAQWbjp19Q-BhV82-IdhVXmKt4U9skHxIkp6uje3nqLdPzJdfgPquRlNGgGeT)**，内置丰富的数据类型。与Java、C++相比，这些数据类型有效地减少代码的长度。下面这个列表简要地描述了Python内置数据类型(适用于Python 3.x)：
|类型
|描述
|例子
|备注
|
|str
|一个由字符组成的不可更改的有串行。
|'Wikipedia'
|"Wikipedia"
|"""Spanning
|multiple
|lines"""
|在Python 3.x里，字符串由Unicode字符组成
|
|bytes
|一个由字节组成的不可更改的有串行。
|b'Some ASCII'
|b"Some ASCII"
|
|list
|可以包含多种类型的可改变的有串行
|[4.0, 'string', True]
|
|tuple
|可以包含多种类型的不可改变的有串行
|(4.0, 'string', True)
|
|set, frozenset
|与数学中集合的概念类似。无序的、每个元素唯一。
|{4.0, 'string', True}
|frozenset([4.0, 'string', True])
|
|dict
|一个可改变的由键值对组成的无串行。
|{'key1': 1.0, 3: False}
|
|int
|精度不限的整数
|42
|
|float
|浮点数。精度与系统相关。
|3.1415927
|
|complex
|复数
|3+2.7j
|
|bool
|逻辑值。只有两个值：真、假
|True
|False
|
除了各种数据类型，Python语言还用类型来表示函数、模块、类型本身、对象的方法、编译后的Python代码、运行时信息等等。因此，Python具备很强的动态性。

**1、Python的数组分三种类型：**
**(1) list**
普通的链表，初始化后可以通过特定方法动态增加元素。
定义方式：arr = [元素]
**(2) Tuple**
固定的数组，一旦定义后，其元素个数是不能再改变的。
定义方式：arr = (元素)
**(3) Dictionary**
词典类型， 即是Hash数组。
定义方式：arr = {元素k:v}
**2、下面具体说明这些数组的使用方法和技巧：**
**(1) list**
a、定义时初始化
a = [1,2,[1,2,3]]
b、定义时不初始化
一维数组：
arr = []
多维数组：
arr = [i for i in range(10), 1,[]] \#注意， i for in xx 这个必须放在第一个位置，否则要先定义i，
如:
arr = [i for i in range(5), j for j in range(5), []]
这是错误的
i = 0
j = 0
arr = [i for i in range(5), j for j in range(5), []]
这是正确的
c、del 语句 和 : 的用法
可以用 start : end 表示数组里的一个区间 ( i >= start and i < end)
del 删除数组里的指定元素
如： del arr[0]
del arr[0, 2]
newarr = arr[0, 2]
d、遍历数组：
for k, v in enumerate(arr):
print k, v
e、增加元素：
一维
arr.append('aaa')
二维
arr[0].append('aaa')
如果要在任意位置插入用 arr.insert(n, 值)
此外还有一种特殊的用法是：
arr += [数组元素]
在不指定下标的情况下，是允许用 += 增加数组元素的。
**(2) Tuple**
Tuple 是不可变 list，一旦创建了一个 tuple 就不能以任何方式改变它。
下面拿具体示例说明：
>>> t = ("a", "b", "c", "d", "e") \#[1] 用小括号包围来定义
>>> t
('a', 'b', 'c', 'd', 'e')
>>> t[0] \#[2] 直接列出某下标的元素
'a'
>>> t[-1] \#[3] 负数表示，从后面倒数的索引 -1 为倒数第一个， 0是顺数第一个
'example'
>>> t[1:3] \#[4] 这里 1:3 是 i>=1 and i<3 的区间
('b', 'mpilgrim')
Tuple 没有的方法：
[1] 不能向 tuple 增加元素，没有 append 、 extend 、insert 等方法。
[2] 不能从 tuple 删除元素，没有 remove 或 pop 方法。
[3] 不能在 tuple 中查找元素，没有 index 方法（index是查找而不是索引，索引直接用下标即可，如：t[0]）。
使用 tuple 的好处：
* Tuple 比 list 操作速度快。如果您定义了一个值的常量集, 并且唯一要用它做的是不断地遍历它, 请使用 tuple 代替 list。
* 如果对不需要修改的数据进行 “写保护”, 可以使代码更安全。使用 tuple 而不是 list 如同拥有一个隐含的 assert 语句, 说明这一数据是常量。如果必须要改变这些值, 则需要执行 tuple 到 list 的转换 (需要使用一个特殊的函数)。
* 还记得我说过 dictionary keys 可以是字符串, 整数和 “其它几种类型”吗? Tuples 就是这些类型之一。 Tuples 可以在 dictionary 中被用做 key, 但是 list 不行。实际上, 事情要比这更复杂。Dictionary key 必须是不可变的。Tuple 本身是不可改变的, 但是如果您有一个 list 的 tuple, 那就认为是可变的了， 用做 dictionary key 就是不安全的。只有字符串, 整数或其它对 dictionary 安全的 tuple 才可以用作 dictionary key。
Tuple 可以转换成 list， 反之亦然。
转换方式为：
t = list( t )
反之：
arr = tuple( arr )
**(3) Dictionary**
\#Dictionary 的用法比较简单，它可以存储任意值，并允许是不同类型的值，下面实例来说明：
\#下面例子中 a 是整数， b 是字符串, c 是数组，这个例子充分说明哈希数组的适用性。
dict_arr = {'a': 100, 'b':'boy', 'c':['o', 'p', 'q']}
\#可以直接增加一个元素，如果同名，则会改变原来的key的元素的值
dict_arr['d'] = 'dog'
\#输出所有的key
print dict_arr.keys()
\#输出所有的value
print dict_arr.values()
\#遍历数组
import types
for k in dict_arr:
v = dict_arr.get(k)
if type(v) is types.ListType: \#如果数据是list类型，继续遍历
print k, '---'
for kk, vv in enumerate(v):
print kk, vv
print '---'
else:
print dict_arr.get(k)


**python字符串截取子串**

python中没有类似sub()或者subString()的方法，但是字符串的截取操作却是更加简单，只需要把字符串看作是一个字符数组，截取子串非常方便。
示例：
str = ’0123456789′
print str[0:3] \#截取第一位到第三位的字符
print str[:] \#截取字符串的全部字符
print str[6:] \#截取第七个字符到结尾
print str[:-3] \#截取从头开始到倒数第三个字符之前
print str[2] \#截取第三个字符
print str[-1] \#截取倒数第一个字符
print str[::-1] \#创造一个与原字符串顺序相反的字符串
print str[-3:-1] \#截取倒数第三位与倒数第一位之前的字符
print str[-3:] \#截取倒数第三位到结尾
print str[:-5:-3] \#逆序截取，具体啥意思没搞明白？
对应的输出结果为：
012
0123456789
6789
0123456
2
9
9876543210
78
789
96

[python数组的使用](http://blog.sina.com.cn/s/blog_6b783cbd0100q2ba.html)
[python中数组的使用](http://blog.csdn.net/leo115/article/details/8550998)
[python字符串截取子串](http://flyash.itcao.com/post_1165.html)




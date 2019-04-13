
# Python的浅拷贝和深拷贝 - saltriver的专栏 - CSDN博客


2016年11月20日 10:58:59[saltriver](https://me.csdn.net/saltriver)阅读数：730标签：[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)[浅拷贝																](https://so.csdn.net/so/search/s.do?q=浅拷贝&t=blog)[深拷贝																](https://so.csdn.net/so/search/s.do?q=深拷贝&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=浅拷贝&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=python&t=blog)个人分类：[Python																](https://blog.csdn.net/saltriver/article/category/6363186)
[
																								](https://so.csdn.net/so/search/s.do?q=python&t=blog)


相对于其他传统编程语言，Python有一个比较奇怪的特性，即在复制对象时，有浅拷贝（shallow copy）和深拷贝（deep copy）两种方式。
浅拷贝和深拷贝只和复合对象相关。复合对象指的是包含对象的对象，如列表（list）、类实例(class instance)等。简单类型的对象（int、float、string等）不存在浅拷贝和深拷贝的说法。
看下面的实例：
```python
colours1 = [
```
```python
"red"
```
```python
,
```
```python
"blue"
```
```python
]
colours2 = colours1
```
```python
print
```
```python
(colours1)
```
```python
print
```
```python
(colours2)
```
```python
print
```
```python
(
```
```python
id
```
```python
(colours1),
```
```python
id
```
```python
(colours2))
```
输出结果：
```python
['red', 'blue']
['red', 'blue']
```
```python
563841065096
```
```python
563841065096
```
在上面的例子中，列表colours1被赋值给colours2。Colours1这样的列表一般被称为浅列表或普通列表，因为它只包含一些简单数据类型，不包含嵌套结构，即不是嵌套列表。id()函数的值相同表明colours2和colours1这2个列表指向同一个对象，说明colours1被赋值给colours2时，并没有分配新的内存地址，而是将colours2指向了colours1的内存地址。下图给出了相关的数据结构说明。
![图1](https://img-blog.csdn.net/20161120103317994)[  ](https://img-blog.csdn.net/20161120103317994)
现在我们看看分配一个新的列表对象给colours2，会发生什么？
[

](https://img-blog.csdn.net/20161120103317994)
```python
colours2 = [
```
```python
"rouge"
```
```python
,
```
```python
"vert"
```
```python
]
```
```python
print
```
```python
(colours1)
```
```python
print
```
```python
(colours2)
```
```python
print
```
```python
(
```
```python
id
```
```python
(colours1),
```
```python
id
```
```python
(colours2))
```
[
](https://img-blog.csdn.net/20161120103317994)输出结果：
[

](https://img-blog.csdn.net/20161120103317994)
```python
['red', 'blue']
['rouge', 'vert']
```
```python
357368848712
```
```python
357368848456
```
[
](https://img-blog.csdn.net/20161120103317994)跟我们期望的一样，colours1的值保持不变，一个新的内存地址被分配给了colours2。
下面我们再看看colours2不是重新分配对象，而是改变其中一个元素的值，结果会有什么变化？
```python
colours1 = [
```
```python
"red"
```
```python
,
```
```python
"blue"
```
```python
]
colours2 = colours1
```
```python
print
```
```python
(id(colours1), id(colours2))
colours2[
```
```python
1
```
```python
] =
```
```python
"green"
```
```python
print
```
```python
(id(colours1), id(colours2))
```
```python
print
```
```python
(colours1)
```
```python
print
```
```python
(colours2)
```
输出结果：
```python
1026592727752
```
```python
1026592727752
```
```python
1026592727752
```
```python
1026592727752
```
```python
['red', 'green']
['red', 'green']
```
可以看到，当我们将colours2中的第二个元素重新赋值时，colours1中的值也被自动改变了，很多初学者在这里都非常迷惑。事实上我们并没有分配一个新的对象给colours2。colours1和colours2仍然指向同一个列表对象。即我们没有两个列表，仍然只有1个，只不过有2个名字。
![图2](https://img-blog.csdn.net/20161120103400287)[  ](https://img-blog.csdn.net/20161120103400287)
那对于简单列表，有没有完全拷贝的方案呢，有！那就是使用[切片方法](http://blog.csdn.net/saltriver/article/details/52194906)。因为切片方法是重新生成了一个新对象。
[

](https://img-blog.csdn.net/20161120103400287)
```python
list1 = [
```
```python
'a'
```
```python
,
```
```python
'b'
```
```python
,
```
```python
'c'
```
```python
,
```
```python
'd'
```
```python
]
list2 = list1[:]
list2[
```
```python
1
```
```python
] =
```
```python
'x'
```
```python
print
```
```python
(list2)
```
```python
print
```
```python
(list1)
```
[
](https://img-blog.csdn.net/20161120103400287)输出结果：
[

](https://img-blog.csdn.net/20161120103400287)
```python
['a', 'x', 'c', 'd']
['a', 'b', 'c', 'd']
```
[
](https://img-blog.csdn.net/20161120103400287)但是，如果是像下面这样的嵌套列表，就又会遇到新的困难和问题。因为切片操作本质上仍然是浅拷贝。当遇到嵌套列表时，切片方法只复制子列表的地址，而不是其全部内容。
[

](https://img-blog.csdn.net/20161120103400287)
```python
lst1 = [
```
```python
'a'
```
```python
,
```
```python
'b'
```
```python
, [
```
```python
'ab'
```
```python
,
```
```python
'ba'
```
```python
]]
lst2 = lst1[:]
```
[
](https://img-blog.csdn.net/20161120103400287)下面的图给出了lst1和lst2的数据结构描述，lst2虽然是一个新建对象，但其中的子列表[‘ab’,’ba’]与lst1中的指的是同一个对象。
![图3](https://img-blog.csdn.net/20161120103427740)[ ](https://img-blog.csdn.net/20161120103427740)
如果对lst1和lst2中的第一个元素或第二个元素进行赋值，并没有什么副作用（side effect）
[

](https://img-blog.csdn.net/20161120103427740)
```python
lst1 = [
```
```python
'a'
```
```python
,
```
```python
'b'
```
```python
, [
```
```python
'ab'
```
```python
,
```
```python
'ba'
```
```python
]]
lst2 = lst1[:]
lst2[
```
```python
0
```
```python
] =
```
```python
'c'
```
```python
print
```
```python
(lst1)
```
```python
print
```
```python
(lst2)
```
[
](https://img-blog.csdn.net/20161120103427740)输出结果：
[

](https://img-blog.csdn.net/20161120103427740)
```python
['a', 'b', ['ab', 'ba']]
['c', 'b', ['ab', 'ba']]
```
[
](https://img-blog.csdn.net/20161120103427740)![图4](https://img-blog.csdn.net/20161120103451589)[ ](https://img-blog.csdn.net/20161120103451589)
但是，如果改变的是嵌套子列表中的值，那么情况就发生了变化。
[

](https://img-blog.csdn.net/20161120103451589)
```python
lst2[
```
```python
2
```
```python
][
```
```python
1
```
```python
] =
```
```python
'd'
```
```python
print(lst1)
print(lst2)
```
[
](https://img-blog.csdn.net/20161120103451589)输出结果：
[

](https://img-blog.csdn.net/20161120103451589)
```python
['a', 'b', ['ab', 'd']]
['c', 'b', ['ab', 'd']]
```
[
](https://img-blog.csdn.net/20161120103451589)下面的图给出了为什么lst1中的嵌套子列表会跟随lst2发生变化的原因，因为lst1和lst2的嵌套子列表指向同一个对象。
![图5](https://img-blog.csdn.net/20161120103603591)[ ](https://img-blog.csdn.net/20161120103603591)
一个解决方案是使用标准库的copy模块。如果我们需要让一个对象发生改变时不对原对象产生副作用，就需要一份这个对象的深度拷贝。深拷贝不仅仅拷贝了原始对象自身，也对其包含的值进行拷贝，它会递归的查找对象中包含的其他对象的引用，来完成更深层次拷贝。因此，深拷贝产生的副本可以随意修改而不需要担心会引起源对象的改变。
[
](https://img-blog.csdn.net/20161120103603591)对先前的例子使用深拷贝：
[

](https://img-blog.csdn.net/20161120103603591)
```python
from
```
```python
copy
```
```python
import
```
```python
deepcopy
lst1 = [
```
```python
'a'
```
```python
,
```
```python
'b'
```
```python
, [
```
```python
'ab'
```
```python
,
```
```python
'ba'
```
```python
]]
lst2 = deepcopy(lst1)
print(lst1)
print(lst2)
print(id(lst1))
print(id(lst2))
print(id(lst1[
```
```python
0
```
```python
]))
print(id(lst2[
```
```python
0
```
```python
]))
print(id(lst1[
```
```python
2
```
```python
]))
print(id(lst2[
```
```python
2
```
```python
]))
```
[
](https://img-blog.csdn.net/20161120103603591)输出结果：
[

](https://img-blog.csdn.net/20161120103603591)
```python
['a', 'b', ['ab', 'ba']]
['a', 'b', ['ab', 'ba']]
```
```python
537508176136
```
```python
537508176200
```
```python
537506121184
```
```python
537506121184
```
```python
537508176712
```
```python
537508176776
```
[
](https://img-blog.csdn.net/20161120103603591)可以看出lst1和lst2的嵌套子列表的内存地址不一样了。下面的图给出了deepcopy后的数据结构情形：
![图6](https://img-blog.csdn.net/20161120103621882)[ ](https://img-blog.csdn.net/20161120103621882)
看看改变lst2中相关值的结果：
[

](https://img-blog.csdn.net/20161120103621882)
```python
lst2[
```
```python
2
```
```python
][
```
```python
1
```
```python
] =
```
```python
"d"
```
```python
lst2[
```
```python
0
```
```python
] =
```
```python
"c"
```
```python
print(lst1)
print(lst2)
```
[
](https://img-blog.csdn.net/20161120103621882)输出结果：
[

](https://img-blog.csdn.net/20161120103621882)
```python
['a', 'b', ['ab', 'ba']]
['c', 'b', ['ab', 'd']]
```
[
](https://img-blog.csdn.net/20161120103621882)上面的代码最后将数据结构变成了这样：
![图7](https://img-blog.csdn.net/20161120103638897)[  ](https://img-blog.csdn.net/20161120103638897)
至于Python中为什么要有浅拷贝和深拷贝的区别，主要是出于效率方面的考虑。
参考文献：[http://www.python-course.eu/python3_deep_copy.php](http://www.python-course.eu/python3_deep_copy.php)

[
](https://img-blog.csdn.net/20161120103621882)

# python中的extend和append的区别 - 数据之美的博客 - CSDN博客
2017年03月20日 16:11:55[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：311
实例：
>>> a.extend([1,2])
>>> print a
[1, 2, '3', '1', 1, 2]
>>> a.append([1,2])
>>> print a
[1, 2, '3', '1', 1, 2, [1, 2]]
append和extend都仅只可以接收一个参数，
append 任意，甚至是tuple
extend 只能是一个列表
### [python list之append和extend的区别](http://blog.csdn.net/cain/article/details/6573085)
分类： [python](http://blog.csdn.net/Cain/article/category/836547)2011-06-28
 16:532909人阅读[评论](http://blog.csdn.net/cain/article/details/6573085#comments)(1)[收藏](http://blog.csdn.net/jfkidear/article/details/8277128)[举报](http://blog.csdn.net/cain/article/details/6573085#report)
1. 列表可包含任何数据类型的元素，单个列表中的元素无须全为同一类型。
2. `append()` 方法向列表的尾部添加一个新的元素。
3. 列表是以类的形式实现的。“创建”列表实际上是将一个类实例化。因此，列表有多种方法可以操作。`extend()`方法只接受一个列表作为参数，并将该参数的每个元素都添加到原有的列表中。
extend的解释没看太明白，琢磨了一下
>>> myList = [1,2.0,'a']
>>> myList
[1, 2.0, 'a']
>>> myList.append('APP')
>>> myList
[1, 2.0, 'a', 'APP']
>>> myList.extend([123,'abc'])
>>> myList
[1, 2.0, 'a', 'APP', 123, 'abc']
>>> myList.append(1,2)
Traceback (most recent call last):
  File "<pyshell#69>", line 1, in <module>
    myList.append(1,2)
TypeError: append() takes exactly one argument (2 given)
>>> myList.extend([1],[2])
Traceback (most recent call last):
  File "<pyshell#70>", line 1, in <module>
    myList.extend([1],[2])
TypeError: extend() takes exactly one argument (2 given)
>>>
结果如下：
append和extend都仅只可以接收一个参数，
append 任意，甚至是tuple
extend 只能是一个列表，其实上面已经说清楚了，是自己没看明白。
实践是检验真理的唯一标准！

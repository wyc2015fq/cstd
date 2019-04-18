# Python list.append list.extend 区别 - YZXnuaa的博客 - CSDN博客
2018年01月31日 10:09:55[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：108
一、
1. 列表可包含任何数据类型的元素，单个列表中的元素无须全为同一类型。
2. `append()` 方法向列表的尾部添加一个新的元素。
3. 列表是以类的形式实现的。“创建”列表实际上是将一个类实例化。因此，列表有多种方法可以操作。`extend()` 方法只接受一个列表作为参数，并将该参数的每个元素都添加到原有的列表中。
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
extend 只能是一个列表.
二、个人测试
![](https://img-blog.csdn.net/20180131100939842?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWVpYbnVhYQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

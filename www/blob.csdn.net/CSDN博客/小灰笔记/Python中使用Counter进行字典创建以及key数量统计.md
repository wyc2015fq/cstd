# Python中使用Counter进行字典创建以及key数量统计 - 小灰笔记 - CSDN博客





2017年03月26日 23:05:16[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：11453
个人分类：[Python](https://blog.csdn.net/grey_csdn/article/category/6639459)









       这里的Counter是指collections中的Counter，通过Counter可以实现字典的创建以及字典key出现频次的统计。然而，使用的时候还是有一点需要注意的小事项。使用Counter创建字典通常有4种方式。其中，第一种方式是不带任何参数创建一个空的字典。剩下的三种分别在下面通过简单的代码进行演示。

       创建方法2示范代码：

need python.'

cell1 =(2,2,3,5,5,4,3,2,1,1,2,3,3,2,2)

list1 =[2,2,3,5,5,4,3,2,1,1,2,3,3,2,2]



c1 = Counter(str1)

c2 =Counter(cell1)

c3 =Counter(list1)



print('c1is:'),c1.items()

print('c2is:'),c2.items()

print('c3is:'),c3.items()

       运行结果如下：

E:\WorkSpace\05_数据分析\01_利用Python进行数据分析\第02章_引言>pythoncounter.py

c1 is: [(' ', 5),('e', 3), ('d', 1), ('f', 1), ('i', 2), ('h', 2), ('l', 1), ('o', 3), (',', 1),('p', 1), ('s', 2), ('r', 1), ('u', 1), ('t', 2), ('.', 1), ('y', 2), ('n', 2)]

c2 is: [(1, 2),(2, 6), (3, 4), (4, 1), (5, 2)]

c3 is: [(1, 2),(2, 6), (3, 4), (4, 1), (5, 2)]

       这三种创建方法都属于一类，只要是传入的对象是一个可迭代的对象都能够通过Counter构建出一个字典。

       构建方法3示范代码：

from collectionsimport Counter



d1 ={'apple':5,'pear':2,'peach':3}

c1 = Counter(d1)

print(c1.items())

       程序的运行结果如下：

E:\WorkSpace\05_数据分析\01_利用Python进行数据分析\第02章_引言>pythonexp1.py

[('pear', 2),('apple', 5), ('peach', 3)]

       第4中构建方法示范代码如下：

from collectionsimport Counter



c1 = Counter(apple= 7,xiaomi = 5,oppo = 9)

print(c1.items())

       程序运行结果如下：

E:\WorkSpace\05_数据分析\01_利用Python进行数据分析\第02章_引言>pythonexp2.py

[('xiaomi', 5),('oppo', 9), ('apple', 7)]

       其实在一定程度上，第三种方式跟一般的字典也就没太大差异了，那么这个Counter构造的对象又有什么不同呢？其实，这里面多了一个统计的通能。举例用的简化代码如下：

from collectionsimport Counter



str1 = 'Life isshort, you need python.'

c1 = Counter(str1)

print(c1.items())

print(c1['i'])

print(c1['e'])

print(c1.most_common(5))

       运行结果如下：

E:\WorkSpace\05_数据分析\01_利用Python进行数据分析\第02章_引言>pythonexp3.py

[(' ', 5), ('e',3), ('d', 1), ('f', 1), ('i', 2), ('h', 2), ('L', 1), ('o', 3), (',', 1), ('p',1), ('s', 2), ('r', 1), ('u', 1), ('t', 2), ('.', 1), ('y', 2), ('n', 2)]

2

3

[(' ', 5), ('e',3), ('o', 3), ('i', 2), ('h', 2)]

     从以上结果可以看出，通过这种方法构建的对象不仅能够具有字典的属性，同时还可以统计key的数目并且通过相应的方法输出最高几项的清单。

       除此之外，还可以对生成的对象进行修改，比如修改其value。如果key不存在的时候统计数为0，但是统计数为0并不意味着没有这个key。也就是说不能够通过赋值为0的方式删除其中的元素。具体的演示代码如下：

from collectionsimport Counter



str1 = 'Life isshort, you need python.'

c1 = Counter(str1)

print(c1.items())

print(c1['i'])

c1['i'] = 0

print(c1['i'])

print(c1.items())

del c1['i']

print(c1.items()

       程序运行结果：

E:\WorkSpace\05_数据分析\01_利用Python进行数据分析\第02章_引言>pythonexp3.py

[(' ', 5), ('e',3), ('d', 1), ('f', 1), ('i', 2), ('h', 2), ('L', 1), ('o', 3), (',', 1), ('p',1), ('s', 2), ('r', 1), ('u', 1), ('t', 2), ('.', 1), ('y', 2), ('n', 2)]

2

0

[(' ', 5), ('e',3), ('d', 1), ('f', 1), ('i', 0), ('h', 2), ('L', 1), ('o', 3), (',', 1), ('p',1), ('s', 2), ('r', 1), ('u', 1), ('t', 2), ('.', 1), ('y', 2), ('n', 2)]

[(' ', 5), ('e',3), ('d', 1), ('f', 1), ('h', 2), ('L', 1), ('o', 3), (',', 1), ('p', 1), ('s',2), ('r', 1), ('u', 1), ('t', 2), ('.', 1), ('y', 2), ('n', 2)]




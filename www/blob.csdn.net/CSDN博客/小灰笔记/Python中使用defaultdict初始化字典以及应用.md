# Python中使用defaultdict初始化字典以及应用 - 小灰笔记 - CSDN博客





2017年03月26日 22:12:07[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：5124








       Python中可以使用collections中的defaultdict类实现创建进行统一初始化的字典。这里总结两种常用一点的初始化方式，分别是初始化为list和int。

       初始化为list示范代码：

#!/usr/bin/python



from collectionsimport defaultdict



s = [('yellow',1), ('blue', 2), ('yellow', 3), ('blue', 4), ('red', 1)]

d =defaultdict(list)

for k,v in s:

       d[k].append(v)

print(d.items()



       运行结果如下：

E:\WorkSpace\05_数据分析\01_利用Python进行数据分析\第02章_引言>pythondict_init.py

[('blue', [2, 4]),('red', [1]), ('yellow', [1, 3])]

       从上面的结果可以看出，这种初始化功能非常适合用于统计几个key出现过那几个value。而第二种初始化为int的方式则与此不同，它适合实现的不是统计key的所有value，而是统计一个key出现过多少次。示范代码如下：

from collectionsimport defaultdict



s = 'mississippi'

d =defaultdict(int)

for k in s:

       d[k] += 1

print(d.items())

       运行结果如下：

E:\WorkSpace\05_数据分析\01_利用Python进行数据分析\第02章_引言>pythondict_int_int.py

[('i', 4), ('p',2), ('s', 4), ('m', 1)]

       如上结果中，每个字典的key对应的value都是其出现的次数。




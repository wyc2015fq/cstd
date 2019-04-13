
# Python的zip - saltriver的专栏 - CSDN博客


2016年08月12日 20:07:26[saltriver](https://me.csdn.net/saltriver)阅读数：689个人分类：[Python																](https://blog.csdn.net/saltriver/article/category/6363186)




zip是Python的一个内置函数，主要用于对多个序列中相对应的元素进行配对，并合并这些序列。
**（1）基本用法**
看下面的例子：
a= [1,2,3]
b = [4,5,6]
c = [7,8,9]
print(list(zip(a,b,c)))输出结果：
[(1, 4, 7), (2, 5, 8), (3, 6, 9)]
zip分别将序列a,b,c中相对应的第1个、第2个、第3个元素进行了配对，并糅合成一个序列。注意：
1）zip配对的元素**形成的是元组（tuple）**
2）Python3中zip、map、filter等内置函数出于效率的考虑，返回的都是对象，可以通过list()转换成列表
**（2）自动截取**
当传入序列的长度不同时，zip能自动以**最短序列长度**为准进行截取。看示例：
a= [1,2,3]
b = [4,5,6,21,23]
c = [7,8,9,17]
print(list(zip(a,b,c)))输出结果：
[(1, 4, 7), (2, 5, 8), (3, 6, 9)]
序列a，b，c的长度分别为3，5，4。zip自动截取长度为3。
如果要取**最长序列长度**为准，需要使用itertools包下的zip_longest，zip_longest将较短序列通过fillvalue为默认填充值进行填充，统一到与最长序列长度一样，示例：

importitertoolsa = [1,2,3]
b = [4,5,6,21,23]
c = [7,8,9,17]
print(list(itertools.zip_longest(a,b,c,fillvalue=0)))输出结果：
[(1, 4, 7), (2, 5, 8), (3, 6, 9), (0, 21, 17), (0, 23, 0)]
序列a，b，c的长度分别为3，5，4。zip_longest自动截取长度为5，长度不满5的使用fillvalue=0进行填充。
**（3）常用于列表推导式**
zip常用于列表推导式或字典推导式，用于糅合多个序列。
names= ['Rose','Lisa','Michael','Jack','Tony']
scores = [4.5,4.2,4.0,5.0,3.5]
d = {n: sforn,sinzip(names,scores)}
print(d)输出结果：
{'Michael': 4.0, 'Tony': 3.5, 'Jack': 5.0, 'Lisa': 4.2, 'Rose': 4.5}
**（4）使用*号操作符**
通过使用*号操作符，能够将zip进行反向操作。示例：
a= [1,2,3]
b = [4,5,6]
c = [7,8,9]
d =list(zip(a,b,c))
print('d=',d)
e =list(zip(*d))
print('e=',e)
输出结果：
d= [(1, 4, 7), (2, 5, 8), (3, 6, 9)]
e= [(1, 2, 3), (4, 5, 6), (7, 8, 9)]





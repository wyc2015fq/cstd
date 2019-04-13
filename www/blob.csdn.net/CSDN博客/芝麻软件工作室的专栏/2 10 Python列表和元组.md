
# 2.10 Python列表和元组 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月30日 10:06:43[seven-soft](https://me.csdn.net/softn)阅读数：212个人分类：[Python																](https://blog.csdn.net/softn/article/category/6290759)



可以将列表和元组当成普通的“数组”，它能保存任意数量任意类型的Python 对象。和数组一样，通过从0 开始的数字索引访问元素，但是列表和元组可以存储不同类型的对象。
列表和元组有几处重要的区别。列表元素用中括号（[ ]）包裹，元素的个数及元素的值可以改变。元组元素用小括号（( )）包裹，不可以更改（尽管他们的内容可以）。元组可以看成是只读的列表。通过切片运算（[ ] 和 [ : ]）可以得到子集，这一点与字符串的使用方法一样。
>>> aList = [1, 2, 3, 4]
>>> aList
[1, 2, 3, 4]
>>> aList[0]
1
>>> aList[2:]
[3, 4]
>>> aList[:3]
[1, 2, 3]
>>> aList[1] = 5
>>> aList
[1, 5, 3, 4]
元组也可以进行切片运算，得到的结果也是元组（不能被修改）：
>>> aTuple = ('robots', 77, 93, 'try')
>>> aTuple
('robots', 77, 93, 'try')
>>> aTuple[:3]
('robots', 77, 93)
>>> aTuple[1] = 5
Traceback (innermost last):
File "<stdin>", line 1, in ?
TypeError: object doesn't support item assignment
关于列表和元组，这里只做简单介绍，后续章节将会进行深入探讨。


# python set集合运算（交集，并集，差集，对称差集） - Nicole的博客 - CSDN博客
2018年04月09日 15:24:30[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：200
版权声明：本文为转载文章，原文地址：https://blog.csdn.net/sxingming/article/details/51922776[点击打开链接](https://blog.csdn.net/sxingming/article/details/51922776)
1》交集
>>> x={1,2,3,4}
>>> y={3,4,5,6}
>>> x
set([1, 2, 3, 4])
>>> y
set([3, 4, 5, 6])
>>> x&y
set([3, 4])
>>> x.intersection(y)
set([3, 4])
2》并集
>>> x | y #集合并集
set([1, 2, 3, 4, 5, 6])
>>> x.union(y)
set([1, 2, 3, 4, 5, 6])
3》差集
>>> x-y # x与y的差集
set([1, 2])
>>> x.difference(y)# x与y的差集
set([1, 2])
>>> y-x # y与x的差集
set([5, 6])
>>> y.difference(x)# y与x的差集
set([5, 6])
4》对称差集
>>> x^y
set([1, 2, 5, 6])
>>> y^x
set([1, 2, 5, 6])
>>> x.symmetric_difference(y)
set([1, 2, 5, 6])
>>> y.symmetric_difference(x)
set([1, 2, 5, 6])
5》集合的子集和超集
>>> x
set([1, 2, 3, 4])
>>> z
set([1, 2, 3])
>>> z.issubset(x)#z是x的子集
True
>>> x.issuperset(z)#x是z的超集
True
下面的图片形象地展示了set集合的各种运算：
![](https://img-blog.csdn.net/20160716093949894)
（完）

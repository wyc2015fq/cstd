# numpy中向量式三目运算符 - 小灰笔记 - CSDN博客





2017年04月07日 23:57:15[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：720







如果用到数据筛选功能，可以使用x if condition else y的逻辑实现。如果使用的是纯Python，可以使用不断迭代的方式对每一组元素组合进行相应的判断筛选。不过，如果使用numpy中的向量化计可以大大加速运算的过程。
在numpy中有一个这个三目运算的向量版本numpy.where。where方法可以接收三个参数，第一个参数为条件向量，而第二、第三个参数可以是矩阵也可以是标量。接下来做一下相应功能的纯Python功能实现以及向量方式实现。

记录如下：

In [76]: xarr = np.array([1.1,1.2,1.3,1.4,1.5])



In [77]: yarr = xarr + 1



In [78]: xarr

Out[78]: array([ 1.1,  1.2,  1.3,  1.4,  1.5])



In [79]: yarr

Out[79]: array([ 2.1,  2.2,  2.3,  2.4,  2.5])



In [80]: cond = np.array([True,False,True,True,False])



In [81]: cond

Out[81]: array([ True, False,  True,  True, False], dtype=bool)



In [82]: result1 = [(x if c else y) for x,y,c in zip(xarr,yarr,cond)]



In [83]: result1

Out[83]: [1.1000000000000001, 2.2000000000000002, 1.3, 1.3999999999999999, 2.5]



In [84]: result2 = np.where(cond,xarr,yarr)



In [85]: result2

Out[85]: array([ 1.1,  2.2,  1.3,  1.4,  2.5])
从浮点表示上，两者有一点点小小的差异，在小数点后多位，通常在数值表示上可以忽略。不过，这里还是要进行一下两个结果的一致性判断，因为之前也看到过Python在浮点表达上因为机器而产生的差异。测试的结果如下：

In [87]: result1 == result2

Out[87]: array([ True,  True,  True,  True,  True], dtype=bool)
从上面的结果可以看出，两个计算结果是一致的。
            



# Python中的NumPy函数库 - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 17:23:03[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：3860








NumPy函数库是python中一个重要的数值计算数据库，在用python实现一些机器学习算法的时候，也经常用到NumPy函数库。
NumPy函数库中存在两种不同的数据类型（矩阵matrix和数组array），都可以用于处理行列表示的数组元素。




罗列一些NumPy函数库中常用到的函数：

- mat()函数可以将数组转化为矩阵；




- .I操作符实现矩阵求逆；
- 矩阵相乘 *；
- tile函数，实现对一个数，数组，矩阵的复制。类似于MATLAB中的repmat函数；（注意：调用的时候需要用numpy.tile）



- shape函数返回矩阵的大小,shape[0]就是行，shape[1]就是列。注意：shape的对象必须是一个数组array，需要用numpy.array()；
- a**b就是a^b，a的b次方；
- sum函数，注意sum的对象一定要是list，而且默认的是axis=0，如果是sum(a, axis=1)则是：




- argsort函数返回数组值从小到大的索引值；




- get函数。在字典中，dict.get(key, v)就是在dict中取出key对应的value，如果key还没有建立，不存在，则赋值指定的v值；
- operator.itemgetter函数可以获取一个对象指定序号的数据，operator.itemgetter获取的不是值，还是定了一个函数，通过该函数作用到对象上才能获取值。一般用在sorted函数中。还需要注意需要import operator模块；




- min()和max()函数得到最值。使用dataSet.min(0)和dataSet.max(0)得到每一列的最值，结果返回的是行向量。
- 在NumPy库中，矩阵除法需要使用函数linalg.solve(matA, matB)；
- 







































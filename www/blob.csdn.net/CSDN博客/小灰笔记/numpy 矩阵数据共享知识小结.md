# numpy 矩阵数据共享知识小结 - 小灰笔记 - CSDN博客





2017年02月09日 22:11:51[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：287








今天看了一段numpy的手册文档，看了一点基础的使用。看到了一段关于数据共享描述，类似的功能在学习Python基础的时候在列表基础的时候看到过。好奇想看看MATLAB的矩阵是否也有数据共享问题，看看numpy与MATLAB是否有基础性的大差异。




命令窗口做一下试探如下，试一下数组的创建以及数组对象属性的查看：




>>> import numpy as np

>>> x = np.array([[1,2,3],[4,5,6]],np.int32)

>>> x

array([[1, 2, 3],

       [4, 5, 6]], dtype=int32)

>>> type(x)

<type 'numpy.ndarray'>

>>> x.shape

(2, 3)

>>> x.dtype

dtype('int32')

>>> x[1,2]

6







再做一下矩阵的切片处理，生成一个新的矩阵y，同时修改y的元素数值。通过查看，可以看到x与y的元素数值全都发生了变化：

>>> y = x[:,1]

>>> y

array([2, 5], dtype=int32)

>>> y[0]

2

>>> y[0] = 123

>>> y

array([123,   5], dtype=int32)

>>> x

array([[  1, 123,   3],

       [  4,   5,   6]], dtype=int32)










看完了numpy，在测试一下MATLAB。命令窗口做一下测试，记录如下：

>> x = [1 2 3;4 5 6]




x =




     1     2     3

     4     5     6




>> y = x(:,1)




y =




     1

     4




>> y = x(:,2)




y =




     2

     5






>> y(2,1)




ans =




     5




>> y(2,1) = 12




y =




     2

    12




>> x




x =




     1     2     3

     4     5     6




从上面的测试记录可以看得出，MATLAB与Python的基础知识甚至是操作都差不多。不过，默认情况下，Python的index从0开始，而MATLAB的index则从1开始。印象中，在MATLAB的simulink中，这个起始索引值是可以修改的，不知道命令窗口中的矩阵相关参数是否有相应的配置方式。

numpy与MATLAB的矩阵切片操作都是相似的，不过通过测试中发现MATLAB倒没有numpy中类似的数据共享问题。




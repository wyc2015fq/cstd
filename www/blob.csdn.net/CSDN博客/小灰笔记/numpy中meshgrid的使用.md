# numpy中meshgrid的使用 - 小灰笔记 - CSDN博客





2017年04月07日 23:22:49[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：30615








       meshgrid函数通常在数据的矢量化上使用，但是使用的方法我暂时还不是很明确。而meshgrid的作用适用于生成网格型数据，可以接受两个一维数组生成两个二维矩阵，对应两个数组中所有的(x,y)对。接下来通过简单的shell交互来演示一下这个功能的使用，并做一下小结。

       交互显示：

In [65]: xnums =np.arange(4)



In [66]: ynums =np.arange(5)



In [67]: xnums

Out[67]: array([0,1, 2, 3])



In [68]: ynums

Out[68]: array([0,1, 2, 3, 4])



In [69]: data_list= np.meshgrid(xnums,ynums)



In [70]: data_list

Out[70]:

[array([[0, 1, 2,3],

        [0, 1, 2, 3],

        [0, 1, 2, 3],

        [0, 1, 2, 3],

        [0, 1, 2, 3]]), array([[0, 0, 0, 0],

        [1, 1, 1, 1],

        [2, 2, 2, 2],

        [3, 3, 3, 3],

        [4, 4, 4, 4]])]



In [71]: x,y =data_list

In [72]: x.shape

Out[72]: (5L, 4L)



In [73]: y.shape

Out[73]: (5L, 4L)



In [74]: x

Out[74]:

array([[0, 1, 2,3],

       [0, 1, 2, 3],

       [0, 1, 2, 3],

       [0, 1, 2, 3],

       [0, 1, 2, 3]])



In [75]: y

Out[75]:

array([[0, 0, 0,0],

       [1, 1, 1, 1],

       [2, 2, 2, 2],

       [3, 3, 3, 3],

       [4, 4, 4, 4]])

       由上面的交互可以看出，meshgrid的作用是根据传入的两个一维数组参数生成两个数组元素的列表。如果第一个参数是xarray，维度是xdimesion，第二个参数是yarray，维度是ydimesion。那么生成的第一个二维数组是以xarray为行，ydimesion行的向量；而第二个二维数组是以yarray的转置为列，xdimesion列的向量。




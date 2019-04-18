# numpy通用函数之四则运算 - 小灰笔记 - CSDN博客





2017年04月07日 21:49:39[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：854







Numpy中的通用函数是实现对数组进行元素级别的运算的函数，比较常用的应该是四则运算以及常见的乘开方和三角函数等计算。最常用的应该就是四则运算了，做简单的使用示范如下：

In [1]: import numpy as np



In [2]: data1 = np.random.randn(3,4)



In [3]: data1

Out[3]:

array([[ 2.08583807,  1.75601778,  0.22426082, -0.30394806],

       [-0.42278652,  1.91341897, -0.27521325,  0.45140919],

       [ 0.21544337,  0.40533004, -0.3852908 ,  0.9754954 ]])



In [4]: data2 = np.random.randn(3,4)



In [5]: data2

Out[5]:

array([[-0.01303592,  0.29044574,  1.21258517,  0.6905317 ],

       [-1.27014631, -1.09704283, -1.91458272, -1.37944753],

       [ 1.64076068, -0.20121267, -0.34006219,  0.17254129]])



In [6]: np.add(data1,data2)

Out[6]:

array([[ 2.07280215,  2.04646352,  1.43684599,  0.38658364],

       [-1.69293282,  0.81637615, -2.18979597, -0.92803834],

       [ 1.85620405,  0.20411738, -0.72535299,  1.14803669]])



In [7]: np.subtract(data1,data2)

Out[7]:

array([[ 2.09887399,  1.46557204, -0.98832435, -0.99447975],

       [ 0.84735979,  3.0104618 ,  1.63936947,  1.83085672],

       [-1.42531732,  0.60654271, -0.04522861,  0.8029541 ]])



In [8]: np.multiply(data1,data2)

Out[8]:

array([[-0.02719082,  0.51002788,  0.27193535, -0.20988577],

       [ 0.53700073, -2.09910256,  0.52691853, -0.62269529],

       [ 0.35349101, -0.08155754,  0.13102283,  0.16831324]])



In [9]: np.divide(data1,data2)

Out[9]:

array([[ -1.60006958e+02,   6.04594089e+00,   1.84944387e-01,

         -4.40165251e-01],

       [  3.32864423e-01,  -1.74416069e+00,   1.43745813e-01,

         -3.27239116e-01],

       [  1.31307003e-01,  -2.01443602e+00,   1.13300099e+00,

          5.65369237e+00]])
向量模式的计算跟普通模式的计算相比在计算速度上有很大的优势，可以省去很多循环的操作。
其实，numpy中四则运算可以使用numpy中的方法也可以直接使用四则运算符号。而且两种操作方式的结果一致，做验证如下：

In [10]: add1 = np.add(data1,data2)



In [11]: add2 = data1 + data2



In [12]: add1 == add2

Out[12]:

array([[ True,  True,  True,  True],

       [ True,  True,  True,  True],

       [ True,  True,  True,  True]], dtype=bool)



In [13]: sub1 = np.subtract(data1,data2)



In [14]: sub2 = data1 - data2



In [15]: sub1 == sub2

Out[15]:

array([[ True,  True,  True,  True],

       [ True,  True,  True,  True],

       [ True,  True,  True,  True]], dtype=bool)



In [16]: mul1 = np.multiply(data1,data2)



In [17]: mul2 = data1 * data2



In [18]: mul1 == mul2

Out[18]:

array([[ True,  True,  True,  True],

       [ True,  True,  True,  True],

       [ True,  True,  True,  True]], dtype=bool)



In [19]: div1 = np.divide(data1,data2)



In [20]: div2 = data1/data2



In [21]: div1 == div2

Out[21]:

array([[ True,  True,  True,  True],

       [ True,  True,  True,  True],

       [ True,  True,  True,  True]], dtype=bool)
从上面的结果中可以看出，两种运算操作的结果是相同的。
            



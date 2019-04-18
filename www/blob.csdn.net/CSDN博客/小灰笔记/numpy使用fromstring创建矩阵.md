# numpy使用fromstring创建矩阵 - 小灰笔记 - CSDN博客





2017年02月07日 23:38:05[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：9864








使用字符串创建矩阵是一个很实用的功能，之前自己尝试了很多次的小功能使用这个方法就能够简单实现。







创建长度为16的字符串，是为了方便能够在各种数据类型之间转换。

>>> s = "mytestfromstring"

>>> len(s)

16







这个功能其实是比较让我兴奋的一个小功能，因为这个简单的转换实现了ASCII码的转换

>>> np.fromstring(s,dtype=np.int8)

array([109, 121, 116, 101, 115, 116, 102, 114, 111, 109, 115, 116, 114,

       105, 110, 103], dtype=int8)










>>> np.fromstring(s,dtype=np.int16)

array([31085, 25972, 29811, 29286, 28015, 29811, 26994, 26478], dtype=int16)

>>> np.fromstring(s,dtype=np.int32)

array([1702132077, 1919317107, 1953721711, 1735289202], dtype=int32)

>>> np.fromstring(s,dtype=np.int64)

array([8243404206920464749, 7453010373645659503])

>>> np.fromstring(s,dtype=np.single)

array([  7.21560833e+22,   4.56462917e+30,   7.71452462e+31,

         1.12586835e+24], dtype=float32)

>>> np.fromstring(s,dtype=np.float32)

array([  7.21560833e+22,   4.56462917e+30,   7.71452462e+31,

         1.12586835e+24], dtype=float32)

>>> np.fromstring(s,dtype=np.float64)

array([  1.19783602e+243,   1.69375610e+190])










float默认的数据宽度是64，我使用的是Mac，本身是64位的，倒不知是不是跟操作系统有关？

>>> np.fromstring(s,dtype=np.float)

array([  1.19783602e+243,   1.69375610e+190])







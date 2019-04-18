# python中array和matrix的区别 - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 17:25:23[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：4597
个人分类：[Python](https://blog.csdn.net/puqutogether/article/category/2595125)










二者的区别主要在于在做乘法运算的时候，一个是矩阵乘，一个是数组乘，这里和MATLAb很相似。

调用的时候需要用numpy.array







Numpy matrices必须是2维的,但是numpy arrays (ndarrays) 可以是多维的（1D，2D，3D····ND）. Matrix是Array的一个小的分支，包含于Array。所以matrix 拥有array的所有特性。

在numpy中matrix的主要优势是：相对简单的乘法运算符号。例如，a和b是两个matrices，那么a*b，就是[矩阵](http://cpro.baidu.com/cpro/ui/uijs.php?rs=1&amp;amp;amp;amp;u=http%3A%2F%2Fwww.aichengxu.com%2Fview%2F12902&amp;amp;amp;amp;p=baidu&amp;amp;amp;amp;c=news&amp;amp;amp;amp;n=10&amp;amp;amp;amp;t=tpclicked3_hc&amp;amp;amp;amp;q=50099048_cpr&amp;amp;amp;amp;k=%BE%D8%D5%F3&amp;amp;amp;amp;k0=%BE%D8%D5%F3&amp;amp;amp;amp;kdi0=1&amp;amp;amp;amp;k1=matrix&amp;amp;amp;amp;kdi1=1&amp;amp;amp;amp;k2=rise&amp;amp;amp;amp;kdi2=8&amp;amp;amp;amp;k3=matlab&amp;amp;amp;amp;kdi3=8&amp;amp;amp;amp;sid=810775df3a4acad9&amp;amp;amp;amp;ch=0&amp;amp;amp;amp;tu=u1703034&amp;amp;amp;amp;jk=ed1258aa82f348a2&amp;amp;amp;amp;cf=29&amp;amp;amp;amp;fv=14&amp;amp;amp;amp;stid=9&amp;amp;amp;amp;urlid=0&amp;amp;amp;amp;luki=1&amp;amp;amp;amp;seller_id=1&amp;amp;amp;amp;di=128)积。

```
import numpy as np

a=np.mat('4 3; 2 1')
b=np.mat('1 2; 3 4')print(a)# [[4 3]#  [2 1]]print(b)# [[1 2]#  [3 4]]print(a*b)# [[13 20]#  [ 5  8]]
```

matrix 和 array 都可以通过在have`.T`to return the transpose, but matrix objects also have`.H`for the conjugate transpose,
 and`.I`for the inverse.

In contrast, numpy arrays consistently abide by the rule that operations are applied element-wise. Thus, if a and b are numpy arrays,
 then a*b is the array formed by multiplying the components element-wise:

```
c=np.array([[4, 3], [2, 1]])
d=np.array([[1, 2], [3, 4]])print(c*d)# [[4 6]#  [6 4]]
```

To obtain the result of matrix multiplication, you use np.dot :
`print(np.dot(c,d))# [[13 20]#  [ 5  8]]`
The`**`operator also behaves differently:
`print(a**2)# [[22 15]#  [10  7]]print(c**2)# [[16  9]#  [ 4  1]]`
Since`a`is a matrix,`a**2`returns the matrix product`a*a`. Since`c`is an ndarray,`c**2`returns
 an ndarray with each component squared element-wise.

There are other technical differences between matrix objects and ndarrays (having to do with np.ravel, item selection and sequence
 behavior).

The main advantage of numpy arrays is that they are more general than 2-dimensional matrices. What happens when you want a 3-dimensional
 array? Then you have to use an ndarray, not a matrix object. Thus, learning to use matrix objects is more work -- you have to learn matrix object operations, and ndarray operations.

Writing a program that uses both matrices and arrays makes your life difficult because you have to keep track of what type of object
 your variables are, lest multiplication return something you don't expect.

In contrast, if you stick solely with ndarrays, then you can do everything matrix objects can do, and more, except with slightly
 different functions/notation.

If you are willing to give up the visual appeal of numpy [matrix](http://cpro.baidu.com/cpro/ui/uijs.php?rs=1&amp;amp;amp;amp;u=http%3A%2F%2Fwww.aichengxu.com%2Fview%2F12902&amp;amp;amp;amp;p=baidu&amp;amp;amp;amp;c=news&amp;amp;amp;amp;n=10&amp;amp;amp;amp;t=tpclicked3_hc&amp;amp;amp;amp;q=50099048_cpr&amp;amp;amp;amp;k=matrix&amp;amp;amp;amp;k0=%BE%D8%D5%F3&amp;amp;amp;amp;kdi0=1&amp;amp;amp;amp;k1=matrix&amp;amp;amp;amp;kdi1=1&amp;amp;amp;amp;k2=rise&amp;amp;amp;amp;kdi2=8&amp;amp;amp;amp;k3=matlab&amp;amp;amp;amp;kdi3=8&amp;amp;amp;amp;sid=810775df3a4acad9&amp;amp;amp;amp;ch=0&amp;amp;amp;amp;tu=u1703034&amp;amp;amp;amp;jk=ed1258aa82f348a2&amp;amp;amp;amp;cf=29&amp;amp;amp;amp;fv=14&amp;amp;amp;amp;stid=9&amp;amp;amp;amp;urlid=0&amp;amp;amp;amp;luki=2&amp;amp;amp;amp;seller_id=1&amp;amp;amp;amp;di=128) product
 notation, then I think numpy arrays are definitely the way to go.

PS. Of course, you really don't have to choose one at the expense of the other, since`np.asmatrix`and`np.asarray`allow
 you to convert one to the other (as long as the array is 2-dimensional).



One of the biggest practical differences for me of numpy ndarrays compared to numpy matrices or matrix languages like [matlab](http://cpro.baidu.com/cpro/ui/uijs.php?rs=1&amp;amp;amp;amp;u=http%3A%2F%2Fwww.aichengxu.com%2Fview%2F12902&amp;amp;amp;amp;p=baidu&amp;amp;amp;amp;c=news&amp;amp;amp;amp;n=10&amp;amp;amp;amp;t=tpclicked3_hc&amp;amp;amp;amp;q=50099048_cpr&amp;amp;amp;amp;k=matlab&amp;amp;amp;amp;k0=%BE%D8%D5%F3&amp;amp;amp;amp;kdi0=1&amp;amp;amp;amp;k1=matrix&amp;amp;amp;amp;kdi1=1&amp;amp;amp;amp;k2=rise&amp;amp;amp;amp;kdi2=8&amp;amp;amp;amp;k3=matlab&amp;amp;amp;amp;kdi3=8&amp;amp;amp;amp;sid=810775df3a4acad9&amp;amp;amp;amp;ch=0&amp;amp;amp;amp;tu=u1703034&amp;amp;amp;amp;jk=ed1258aa82f348a2&amp;amp;amp;amp;cf=29&amp;amp;amp;amp;fv=14&amp;amp;amp;amp;stid=9&amp;amp;amp;amp;urlid=0&amp;amp;amp;amp;luki=4&amp;amp;amp;amp;seller_id=1&amp;amp;amp;amp;di=128),
 is that the dimension is not preserved in reduce operations. Matrices are always 2d, while the mean of an array, for example, has one dimension less.

For example demean rows of a matrix or array:

with [matrix](http://cpro.baidu.com/cpro/ui/uijs.php?rs=1&amp;amp;amp;amp;u=http%3A%2F%2Fwww.aichengxu.com%2Fview%2F12902&amp;amp;amp;amp;p=baidu&amp;amp;amp;amp;c=news&amp;amp;amp;amp;n=10&amp;amp;amp;amp;t=tpclicked3_hc&amp;amp;amp;amp;q=50099048_cpr&amp;amp;amp;amp;k=matrix&amp;amp;amp;amp;k0=%BE%D8%D5%F3&amp;amp;amp;amp;kdi0=1&amp;amp;amp;amp;k1=matrix&amp;amp;amp;amp;kdi1=1&amp;amp;amp;amp;k2=rise&amp;amp;amp;amp;kdi2=8&amp;amp;amp;amp;k3=matlab&amp;amp;amp;amp;kdi3=8&amp;amp;amp;amp;sid=810775df3a4acad9&amp;amp;amp;amp;ch=0&amp;amp;amp;amp;tu=u1703034&amp;amp;amp;amp;jk=ed1258aa82f348a2&amp;amp;amp;amp;cf=29&amp;amp;amp;amp;fv=14&amp;amp;amp;amp;stid=9&amp;amp;amp;amp;urlid=0&amp;amp;amp;amp;luki=2&amp;amp;amp;amp;seller_id=1&amp;amp;amp;amp;di=128)

```
>>> m = np.mat([[1,2],[2,3]])>>> m
matrix([[1, 2],
        [2, 3]])>>> mm = m.mean(1)>>> mm
matrix([[ 1.5],
        [ 2.5]])>>> mm.shape
(2, 1)>>> m - mm
[matrix](http://cpro.baidu.com/cpro/ui/uijs.php?rs=1&amp;amp;amp;amp;u=http%3A%2F%2Fwww.aichengxu.com%2Fview%2F12902&amp;amp;amp;amp;p=baidu&amp;amp;amp;amp;c=news&amp;amp;amp;amp;n=10&amp;amp;amp;amp;t=tpclicked3_hc&amp;amp;amp;amp;q=50099048_cpr&amp;amp;amp;amp;k=matrix&amp;amp;amp;amp;k0=%BE%D8%D5%F3&amp;amp;amp;amp;kdi0=1&amp;amp;amp;amp;k1=matrix&amp;amp;amp;amp;kdi1=1&amp;amp;amp;amp;k2=rise&amp;amp;amp;amp;kdi2=8&amp;amp;amp;amp;k3=matlab&amp;amp;amp;amp;kdi3=8&amp;amp;amp;amp;sid=810775df3a4acad9&amp;amp;amp;amp;ch=0&amp;amp;amp;amp;tu=u1703034&amp;amp;amp;amp;jk=ed1258aa82f348a2&amp;amp;amp;amp;cf=29&amp;amp;amp;amp;fv=14&amp;amp;amp;amp;stid=9&amp;amp;amp;amp;urlid=0&amp;amp;amp;amp;luki=2&amp;amp;amp;amp;seller_id=1&amp;amp;amp;amp;di=128)([[-0.5,  0.5],
        [-0.5,  0.5]])
```

with array

```
>>> a = np.array([[1,2],[2,3]])>>> a
array([[1, 2],
       [2, 3]])>>> am = a.mean(1)>>> am.shape
(2,)>>> am
array([ 1.5,  2.5])>>> a - am #wrong
array([[-0.5, -0.5],
       [ 0.5,  0.5]])>>> a - am[:, np.newaxis]  #right
array([[-0.5,  0.5],
       [-0.5,  0.5]])
```

I also think that mixing arrays and matrices gives [rise](http://cpro.baidu.com/cpro/ui/uijs.php?rs=1&amp;amp;amp;amp;u=http%3A%2F%2Fwww.aichengxu.com%2Fview%2F12902&amp;amp;amp;amp;p=baidu&amp;amp;amp;amp;c=news&amp;amp;amp;amp;n=10&amp;amp;amp;amp;t=tpclicked3_hc&amp;amp;amp;amp;q=50099048_cpr&amp;amp;amp;amp;k=rise&amp;amp;amp;amp;k0=%BE%D8%D5%F3&amp;amp;amp;amp;kdi0=1&amp;amp;amp;amp;k1=matrix&amp;amp;amp;amp;kdi1=1&amp;amp;amp;amp;k2=rise&amp;amp;amp;amp;kdi2=8&amp;amp;amp;amp;k3=matlab&amp;amp;amp;amp;kdi3=8&amp;amp;amp;amp;sid=810775df3a4acad9&amp;amp;amp;amp;ch=0&amp;amp;amp;amp;tu=u1703034&amp;amp;amp;amp;jk=ed1258aa82f348a2&amp;amp;amp;amp;cf=29&amp;amp;amp;amp;fv=14&amp;amp;amp;amp;stid=9&amp;amp;amp;amp;urlid=0&amp;amp;amp;amp;luki=3&amp;amp;amp;amp;seller_id=1&amp;amp;amp;amp;di=128) to
 many "happy" debugging hours. However, scipy.sparse matrices are always matrices in terms of operators like multiplication.





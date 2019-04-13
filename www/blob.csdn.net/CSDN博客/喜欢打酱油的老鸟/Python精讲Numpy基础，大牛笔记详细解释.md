
# Python精讲Numpy基础，大牛笔记详细解释 - 喜欢打酱油的老鸟 - CSDN博客


2019年03月07日 12:57:12[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：61个人分类：[人工智能																](https://blog.csdn.net/weixin_42137700/article/category/7820233)


[https://www.toutiao.com/a6664936105076326920/](https://www.toutiao.com/a6664936105076326920/)
![Python精讲Numpy基础，大牛笔记详细解释](http://p9.pstatp.com/large/dfic-imagehandler/b5c30a62-1aaa-42b5-b960-2761f20606b9)
**总认为Numpy是渣渣，直到深入接触以后才知道功能这么强大。堪比Matlab啊。果然是人生苦短，我用Python。所以本文作为一个记录&笔记，文章内容大多数取自网络以&官网快速入门等，希望可以帮助大家快速入门Numpy。如果你有Matlab基础，那么你能很快看懂本文！！！（本文长期更新！！！）**
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/ce8530028a14495aa3023c3975c8e67c)

# 一个栗子
![Python精讲Numpy基础，大牛笔记详细解释](http://p3.pstatp.com/large/pgc-image/362bffebb62140ae81e1e33452d4fc0e)
Python精讲Numpy基础，大牛笔记详细解释
# 创建矩阵
对于Python中的numpy模块，一般用其提供的ndarray对象。 创建一个ndarray对象很简单，只要将一个list作为参数即可。 例如:
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/8b01e106e7e345468f07ade5c7035065)
Python精讲Numpy基础，大牛笔记详细解释
# 矩阵行数列数
![Python精讲Numpy基础，大牛笔记详细解释](http://p3.pstatp.com/large/pgc-image/83a840192c614b99807302a158b6eab7)
Python精讲Numpy基础，大牛笔记详细解释
# 矩阵按行列选取
矩阵的截取和list相同，可以通过[]（方括号）来截取
![Python精讲Numpy基础，大牛笔记详细解释](http://p9.pstatp.com/large/pgc-image/652a8cf97ef74f4e8b74f4ee740de56b)
Python精讲Numpy基础，大牛笔记详细解释
# 矩阵按条件截取
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/1c23d511776a4be1a666bc67c3bcf800)
Python精讲Numpy基础，大牛笔记详细解释
**按条件截取应用较多的是对矩阵中满足一定条件的元素变成特定的值。 例如将矩阵中大于6的元素变成0**
![Python精讲Numpy基础，大牛笔记详细解释](http://p9.pstatp.com/large/pgc-image/640fdff2154846b389aaeb09d92bc531)
Python精讲Numpy基础，大牛笔记详细解释
# Stacking together different arrays
**矩阵的合并可以通过numpy中的hstack方法和vstack方法实现:**
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/720c0d5b40c34fc2bd69a285afd95302)
Python精讲Numpy基础，大牛笔记详细解释
矩阵的合并也可以通过concatenatef方法。
np.concatenate( (a1,a2), axis=0 ) 等价于 np.vstack( (a1,a2) )
np.concatenate( (a1,a2), axis=1 ) 等价于 np.hstack( (a1,a2) )
# 通过函数创建矩阵
**arange**
![Python精讲Numpy基础，大牛笔记详细解释](http://p3.pstatp.com/large/pgc-image/6dc148a8de6d4c41b74df6afbbea257b)
Python精讲Numpy基础，大牛笔记详细解释
**linspace/ logspace**
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/cc6525c0bff54f64ba3751c55d9ec1eb)
Python精讲Numpy基础，大牛笔记详细解释
**ones、zeros、eye、empty**
ones创建全1矩阵 ，zeros创建全0矩阵 ，eye创建单位矩阵 ，empty创建空矩阵（实际有值）
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/d8943a63d8bd4e62adb9f2d74af4c3fb)
Python精讲Numpy基础，大牛笔记详细解释
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/c392fe95373a4b6a88dc435bc0e031a9)
**fromstring**
fromstring()方法可以将字符串转化成ndarray对象，需要将字符串数字化时这个方法比较有用，可以获得字符串的ascii码序列。
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/d29e22fb500c4f0c97c7c5497f0bde02)
**random**
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/3623192e2dca4394a46a0fba42c75867)
**fromfunction**
fromfunction()方法可以根据矩阵的行号列号生成矩阵的元素。 例如创建一个矩阵，矩阵中的每个元素都为行号和列号的和。
![Python精讲Numpy基础，大牛笔记详细解释](http://p3.pstatp.com/large/pgc-image/9378a66908d4420fb82e9e2da28d4fa3)

# 矩阵的运算
**常用矩阵运算符**
Numpy中的ndarray对象重载了许多运算符，使用这些运算符可以完成矩阵间对应元素的运算。
**运算符说明**+矩阵对应元素相加-矩阵对应元素相减*矩阵对应元素相乘/矩阵对应元素相除，如果都是整数则取商%矩阵对应元素相除后取余数矩阵每个元素都取n次方，如**2：每个元素都取平方
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/ddf661392e1745a78174f190b53373c4)
**常用矩阵函数**
同样地，numpy中也定义了许多函数，使用这些函数可以将函数作用于矩阵中的每个元素。 表格中默认导入了numpy模块，即 import numpy as np 。a为ndarray对象。
**常用矩阵函数说明**np.sin(a)对矩阵a中每个元素取正弦,sin(x)np.cos(a)对矩阵a中每个元素取余弦,cos(x)np.tan(a)对矩阵a中每个元素取正切,tan(x)np.arcsin(a)对矩阵a中每个元素取反正弦,arcsin(x)np.arccos(a)对矩阵a中每个元素取反余弦,arccos(x)np.arctan(a)对矩阵a中每个元素取反正切,arctan(x)np.exp(a)对矩阵a中每个元素取指数函数,exnp.sqrt(a)对矩阵a中每个元素开根号
当矩阵中的元素不在函数定义域范围内，会产生RuntimeWarning，结果为nan(not a number)
**矩阵乘法（点乘）**
矩阵乘法必须满足矩阵乘法的条件，即第一个矩阵的列数等于第二个矩阵的行数。 矩阵乘法的函数为 dot 。
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/26dca58fd34a4edda4be7bb4909989a3)
**矩阵的转置 a.T**
![Python精讲Numpy基础，大牛笔记详细解释](http://p3.pstatp.com/large/pgc-image/b0c8b10cab4d4c4b81563600177aa440)
矩阵的转置还有更简单的方法，就是a.T。
![Python精讲Numpy基础，大牛笔记详细解释](http://p9.pstatp.com/large/pgc-image/4b85fb2c589041d09dc47710a35b4232)

# 矩阵的逆
设A是数域上的一个n阶方阵，若在相同数域上存在另一个n阶矩阵B，使得： AB=BA=E。 则我们称B是A的逆矩阵，而A则被称为可逆矩阵。
求矩阵的逆需要先导入numpy.linalg，用linalg的inv函数来求逆。矩阵求逆的条件是矩阵应该是方阵。
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/4de775828968403792ce2e77790fc704)

# 矩阵信息获取（如均值等）
**最值**
获得矩阵中元素最大最小值的函数分别是max和min，可以获得整个矩阵、行或列的最大最小值。
![Python精讲Numpy基础，大牛笔记详细解释](http://p3.pstatp.com/large/pgc-image/6b015cfd15d444e88ee24c53f7317a30)
**平均值**
获得矩阵中元素的平均值可以通过函数mean()。同样地，可以获得整个矩阵、行或列的平均值。
![Python精讲Numpy基础，大牛笔记详细解释](http://p3.pstatp.com/large/pgc-image/a0e2845b9e844378a03d44cadd564c68)
Python精讲Numpy基础，大牛笔记详细解释
**方差**
方差的函数为var(),方差函数var()相当于函数mean(abs(x - x.mean())**2),其中x为矩阵。
![Python精讲Numpy基础，大牛笔记详细解释](http://p3.pstatp.com/large/pgc-image/0c6a31815ec64384b0a1ab0275d8e839)
**标准差**
标准差的函数为std()。 std()相当于sqrt(mean(abs(x - x.mean())**2))，或相当于sqrt(x.var())。
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/ad2c5415dba648d1998f9843cfbdf594)
**中值**
中值指的是将序列按大小顺序排列后，排在中间的那个值，如果有偶数个数，则是排在中间两个数的平均值。中值的函数是median()，调用方法为numpy.median(x,[axis])，axis可指定轴方向，默认axis=None，对所有数取中值。
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/aa8c109203454c2fb279e9ddc54f108a)
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/66f4237deab24347a443ee607f6569c0)
**求和**
矩阵求和的函数是sum()，可以对行，列，或整个矩阵求和
![Python精讲Numpy基础，大牛笔记详细解释](http://p9.pstatp.com/large/pgc-image/2398e263fb9e416d9f7f99a86625a57e)
Python精讲Numpy基础，大牛笔记详细解释
**累积和**
某位置累积和指的是该位置之前(包括该位置)所有元素的和。例如序列[1,2,3,4,5]，其累计和为[1,3,6,10,15]，即第一个元素为1，第二个元素为1+2=3，……，第五个元素为1+2+3+4+5=15。矩阵求累积和的函数是cumsum()，可以对行，列，或整个矩阵求累积和。
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/8d8036ecfd0e4422a4a5c35e9a9b852c)
Python精讲Numpy基础，大牛笔记详细解释
**极差**
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/875c2e4315914687856b298b536abaeb)
Python精讲Numpy基础，大牛笔记详细解释
**百分位数**
![Python精讲Numpy基础，大牛笔记详细解释](http://p3.pstatp.com/large/pgc-image/b7731622b4ed49f99affb7696d92248c)
Python精讲Numpy基础，大牛笔记详细解释
序号参数及描述1.a 输入数组2.q 要计算的百分位数，在 0 ~ 100 之间3.axis 沿着它计算百分位数的轴
**加权平均值**
![Python精讲Numpy基础，大牛笔记详细解释](http://p3.pstatp.com/large/pgc-image/d95a119e85fb487b9a98c28d53a123c8)
Python精讲Numpy基础，大牛笔记详细解释
# Shape Manipulation
**Changing the shape of an array**
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/8ab16594978a4e169e52f562e8ee2281)
Python精讲Numpy基础，大牛笔记详细解释
数组的形状可以用以下方式改变。Note that the following three commands all return a modified array, but do not change the original array:
![Python精讲Numpy基础，大牛笔记详细解释](http://p3.pstatp.com/large/pgc-image/b54622e1a1804b6eace189c681bd75be)
Python精讲Numpy基础，大牛笔记详细解释
The reshape function returns its argument with a modified shape, whereas the ndarray.resize method modifies the array itself:
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/bfcbe0f418d74ecab066dcdadc720432)
Python精讲Numpy基础，大牛笔记详细解释
If a dimension is given as -1 in a reshaping operation, the other dimensions are automatically calculated:
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/8f1e980fc9114ac7b31dcee09641df7b)
Python精讲Numpy基础，大牛笔记详细解释
**Splitting one array into several smaller ones**
Using hsplit, you can split an array along its horizontal axis, either by specifying the number of equally shaped arrays to return, or by specifying the columns after which the division should occur:
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/06ce821933be40baae765b5d658b2544)
Python精讲Numpy基础，大牛笔记详细解释
**Copies and Views**
When operating and manipulating arrays, their data is sometimes copied into a new array and sometimes not. This is often a source of confusion for beginners. There are three cases:
**No Copy At All**
a = b,改变b就相当于改变a,或者相反。
![Python精讲Numpy基础，大牛笔记详细解释](http://p9.pstatp.com/large/pgc-image/d63565008ac941bab7c07f93ba81faab)
Python精讲Numpy基础，大牛笔记详细解释
**View or Shallow Copy**
Different array objects can share the same data. The view method creates a new array object that looks at the same data.
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/230b82a60504463fbf9648a98d76ef46)
Python精讲Numpy基础，大牛笔记详细解释
Slicing an array returns a view of it:
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/a60039d0cb4349c8a5127e4f3bea3ed8)
Python精讲Numpy基础，大牛笔记详细解释
![Python精讲Numpy基础，大牛笔记详细解释](http://p3.pstatp.com/large/pgc-image/a1b2cd63cebb4632b8ade248cce45f91)
Python精讲Numpy基础，大牛笔记详细解释
**Deep Copy**
The copy method makes a complete copy of the array and its data.
![Python精讲Numpy基础，大牛笔记详细解释](http://p3.pstatp.com/large/pgc-image/203a78ab08e241dca3a59d924f3a53ec)
Python精讲Numpy基础，大牛笔记详细解释
**曼德勃罗**
![Python精讲Numpy基础，大牛笔记详细解释](http://p1.pstatp.com/large/pgc-image/84cc061922e941ad885085a75eb341d3)


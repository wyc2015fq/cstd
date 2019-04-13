
# Numpy中的通用函数和聚合 - 喜欢打酱油的老鸟 - CSDN博客


2019年04月05日 12:11:43[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：17标签：[Numpy																](https://so.csdn.net/so/search/s.do?q=Numpy&t=blog)[通用函数																](https://so.csdn.net/so/search/s.do?q=通用函数&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Numpy&t=blog)个人分类：[人工智能																](https://blog.csdn.net/weixin_42137700/article/category/7820233)


[https://www.toutiao.com/a6675559636919124483/](https://www.toutiao.com/a6675559636919124483/)
![Numpy中的通用函数和聚合](http://p1.pstatp.com/large/pgc-image/c6c7744860064a628f95fe178b96b375)
概述
在Numpy中存在着通用函数和聚合去对数据进行处理,numpy通过向量进行对数据数组的计算,而这些向量主要依靠一些通用函数,而聚合是numpy对面对大量数据时获取描述性统计信息的方法。
numpy的通用函数
1、为什么用numpy的通用函数而不用Python的循环做逐个元素的计算
NumPy 提供了一个简单灵活的接口来优化数据数组的计算,使得numpy成为python数据科学中极其重要的一部分。Numpy主要是通过向量进行操作的，而这些操作主要依靠他的一些通用函数实现，接下来，我们就去学习这些通用函数，以方便我们提高元素重复计算的效率。
学习numpy通用函数，我们首先举个例子：
假设我们想要计算一个数组元素的倒数：
方法1：用循环迭代执行
方法2：用Numpy的通用函数进行计算
1.1用Python的循环语句实现
![Numpy中的通用函数和聚合](http://p1.pstatp.com/large/pgc-image/1b3a3ff77dde473a8e4f872f190c3ab8)
1.2 用numpy的通用函数实现
![Numpy中的通用函数和聚合](http://p1.pstatp.com/large/pgc-image/0ee1f7d206774820ae008f5923fae74f)
1.3对比一下两者运行的时间
![Numpy中的通用函数和聚合](http://p1.pstatp.com/large/pgc-image/bc6e627835394a09997e017e62bc8636)
这时候就会发现循环的时间复杂度很高,直接用数组通用函数进行计算,速度是python代码循环进行计算的10倍左右。
2、常用通用函数
2.1数组的运算
NumPy 通用函数的使用方式非常自然，因为它用到了 Python 原生的算术运算符，标准的 加、减、乘、除都可以使用：
![Numpy中的通用函数和聚合](http://p3.pstatp.com/large/pgc-image/e94d55b9dcf64ef5aaaec3e91e6f1cef)
加法,直接用+号或者使用np.add()
![Numpy中的通用函数和聚合](http://p3.pstatp.com/large/pgc-image/7b0333cbd5d04a1d82ebab2b45425e64)
除法,使用/号或者np.divide()
![Numpy中的通用函数和聚合](http://p3.pstatp.com/large/pgc-image/273166276838477b9a7fbb6f2761048f)
平方,使用**或者np.power()
![Numpy中的通用函数和聚合](http://p3.pstatp.com/large/pgc-image/902a85a793ac43f2a7c4e5a4eefcc373)
乘法,使用*号或者np.multiply()
![Numpy中的通用函数和聚合](http://p3.pstatp.com/large/pgc-image/9503a64e874c4013b05af74bf40c5f8d)
剩余的通用函数操作:
运算符
对应的通用函数
描述
+
Np.add
加法运算(即 1 + 1 = 2)
-
Np.subtract
减法运算(即 3 - 2 = 1)
-
Np.negative
负数运算(即 -2)
*
Np.multiply
乘法运算(即 2 * 3 = 6)
/
Np.divide
除法运算(即 3 / 2 = 1.5)
//
Np.floor_divide
地板除法运算(floor division,即3//2 = 1)
**
Np.power
指数运算(即 2 ** 3 = 8)
%
Np.mod
模 / 余数(即 9 % 4 = 1)
2.2绝对值
正如NumPy 能理解 Python 内置的运算操作， NumPy 也可以理解 Python 内置的绝对值 函数：
![Numpy中的通用函数和聚合](http://p1.pstatp.com/large/pgc-image/7fe800d26cbe49c79da49237edfc798a)
Numpy的计算速度还是快一些的。
2.3三角函数
NumPy 提供了大量好用的通用函数，其中对于数据科学家最有用的就是三角函数。首先定义一个角度数组：
![Numpy中的通用函数和聚合](http://p3.pstatp.com/large/pgc-image/2b11a5b6be8a44c9afa8e0737346e1ca)
然后进行三角函数运算
![Numpy中的通用函数和聚合](http://p3.pstatp.com/large/pgc-image/bc61bc6266bf46848e75700bb2f752b0)
当然反三角函数也是支持的
![Numpy中的通用函数和聚合](http://p1.pstatp.com/large/pgc-image/842e930805924bdb8d1ae3223d8a3bba)
2.4指数和对数
Numpy中也提供了对指数和对数进行运算的方法
指数运算
![Numpy中的通用函数和聚合](http://p3.pstatp.com/large/pgc-image/531e90a56f084a16aff247b637a7a66c)
以e为底的对数运算
![Numpy中的通用函数和聚合](http://p3.pstatp.com/large/pgc-image/8950c3c979ab4517ac937e58180ea52f)
以2为底的对数运算
![Numpy中的通用函数和聚合](http://p3.pstatp.com/large/pgc-image/213649ea5b8644f9a15c0ab8d3398855)
以10为底的对数运算
![Numpy中的通用函数和聚合](http://p1.pstatp.com/large/pgc-image/f5012e9f24ba40779ad530d0c05c55e5)
通用函数特性
1.指定输出
在进行大量运算时，有时候指定一个用于存放运算结果的数组是非常有用的。不同于创建 临时数组，你可以用这个特性将计算结果直接写入到你期望的存储位置。所有的通用函数 都可以通过out参数来指定计算结果的存放位置：
这个特性也可以被用作数组视图，例如可以将计算结果写入指定数组的每隔一个元素的位置：
![Numpy中的通用函数和聚合](http://p9.pstatp.com/large/pgc-image/5fdf3266d3b0450d86e41ed198a4e3ae)
2.聚合
对通用函数调用reduce方法会返回数组中所有元素的和:
![Numpy中的通用函数和聚合](http://p1.pstatp.com/large/pgc-image/0c5ffdd6365c4eb7a8ba6b8c8944398f)
对内积调用reduce也同样会返回数组的内积
对通用函数调用accumulate,会返回每次计算的结果
![Numpy中的通用函数和聚合](http://p3.pstatp.com/large/pgc-image/601e4fb9dd38492b85920ba20835d114)
3.外积
任何通用函数都可以用 outer 方法获得两个不同输入数组所有元素对的函数运算结果。这意味着你可以用一行代码实现一个乘法表：
![Numpy中的通用函数和聚合](http://p3.pstatp.com/large/pgc-image/fd560a031f52495782ca7d09c8ebd642)
三、聚合:最大值、最小值和其他值
当我们面对大量的数据时，首先想到的就是去获取他们的一些描述性统计信息，比如中值、中位数、均值、最大值、最小值、和、乘积等。Numpy内置的一些方法，在求取这些信息时，会非常的便利和高效。下面我们就去介绍一些比较常用的：
1.数组值求和
![Numpy中的通用函数和聚合](http://p1.pstatp.com/large/pgc-image/02e43b120bec4fc1ba28c1c07f82c3fe)
![Numpy中的通用函数和聚合](http://p9.pstatp.com/large/pgc-image/066bee0a762b4e8390ca925ec2e4d30d)
由于Numpy的sum函数是在编译的过程中进行计算的，因此速度会比python的sum更快一些。
2、最大值和最小值
同样，Python也有内置的min函数和max函数,分别被用于获取给定数组的最小值和最大值：
![Numpy中的通用函数和聚合](http://p1.pstatp.com/large/pgc-image/bb1dbf382eed472aafc8270b287a7078)
对于 min、 max、 sum 和其他 NumPy 聚合，一种更简洁的语法形式是数组对象直接调用这些方法：
![Numpy中的通用函数和聚合](http://p3.pstatp.com/large/pgc-image/5e4f93ef41ea446086e355ecbb3d3656)
3、多维度聚合
一种常用的聚合操作是沿着一行或者一列聚合。例如，有着一个二维数组，我们想要统计他的一些聚合信息：
![Numpy中的通用函数和聚合](http://p3.pstatp.com/large/pgc-image/c9b0c23cb99f4e5591b9b8fc669be553)
默认情况下，每一个Numpy聚合函数都会返回对整个数组的聚合结果：
![Numpy中的通用函数和聚合](http://p1.pstatp.com/large/pgc-image/8a574ff7071e4dc4957149cde8e5343f)
聚合函数还有一个参数，用于指定沿着哪个轴的方向进行聚合。例如，可以通过指定 axis=0 找到每一列的最小值：
![Numpy中的通用函数和聚合](http://p1.pstatp.com/large/pgc-image/bf7a9dfda2034a62935497f21828e8d6)
找到行的最大值类似的操作，让axis = 1 就可以实现
![Numpy中的通用函数和聚合](http://p1.pstatp.com/large/pgc-image/25597d8181a740d6a55ce28356e808ee)
axis 关键字指定的是数组将会被折叠的维度，而不是将要返回的维度。因此指定 axis=0 意味着第一个轴将要被折叠——对于二维数组，这意味着每一列的值都将被聚合。
4、其他聚合函数
Python中其他可用的聚合函数:
函数名称
NaN安全版本
描述
np.sum
np.nansum
计算元素的和
np.prod
np.nanprod
计算元素的积
np.mean
np.nanmean
计算元素的平均值
np.std
np.nanstd
计算元素的标准差
np.var
np.nanvar
计算元素的方差
np.min
np.nanmin
找出最小值
np.max
np.nanmax
找出最大值
np.argmin
np.nanargmin
找出最小值的索引
np.argmax
np.nanargmax
找出最大值的索引
np.median
np.nanmedian
计算元素的中位数
np.percentile
np.nanpercentile
计算基于元素排序的统计值
np.any
N/A
验证任何一个元素是否为真
np.all
N/A
验证所有元素是否为真


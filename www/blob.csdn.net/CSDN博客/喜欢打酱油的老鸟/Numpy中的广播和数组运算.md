
# Numpy中的广播和数组运算 - 喜欢打酱油的老鸟 - CSDN博客


2019年04月09日 08:40:05[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：32标签：[NumPy																](https://so.csdn.net/so/search/s.do?q=NumPy&t=blog)个人分类：[人工智能																](https://blog.csdn.net/weixin_42137700/article/category/7820233)


[https://www.toutiao.com/a6677441250955624973/](https://www.toutiao.com/a6677441250955624973/)
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/c9325f7feb8640efba58614e4c464a86)
一、概述
在Numpy中当数组进行运算时,如果两个数组的形状相同,那么两个数组相乘就是两个数组的对应位相乘,这是要求维数相乘,并且各维度的长度相同,但是当运算中两个数组的形状不同使时,numpy将会自动触发广播机制,所以我们要了解numpy的广播机制,才能更好的进行数组的运算。
二、Numpy中的广播
1、什么是广播
我们都知道，Numpy中的基本运算（加、减、乘、除、求余等等）都是元素级别的，但是这仅仅局限于两个数组的形状相同的情况下。
可是大家又会发现，如果让一个数组加1的话，结果时整个数组的结果都会加1，这是什么情况呢？
![Numpy中的广播和数组运算](http://p3.pstatp.com/large/pgc-image/899cb5e85d384bc6a7c40a331721bffb)
![Numpy中的广播和数组运算](http://p9.pstatp.com/large/pgc-image/dcf61bfeb3d94c629295ffcc2a3c528e)
![Numpy中的广播和数组运算](http://p9.pstatp.com/large/pgc-image/6561b8915d05471caf835a647b5bad18)
![Numpy中的广播和数组运算](http://p9.pstatp.com/large/pgc-image/ca07fda3fb1948e5b6af17584c9af367)
![Numpy中的广播和数组运算](http://p9.pstatp.com/large/pgc-image/2b950259b93241f5b83e41b7e9ecb9c9)
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/c827c1b40849424282d78595e1421b77)
其实这就是广播机制：Numpy 可以转换这些形状不同的数组，使它们都具有相同的大小，然后再对它们进行运算。下面是广播示意图：
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/997e71ca5cfb455bb272466176069298)
术语广播是指 NumPy 在算术运算期间处理不同形状的数组的能力。对数组的算术运算通常在 相应的元素上进行。如果两个阵列具有完全相同的形状，则这些操作被无缝执行。
如果两个数组的维数不相同，则元素到元素的操作是不可能的。然而，在 NumPy 中仍然可以对形状不相似的数组进行操作，因为它拥有广播功能。较小的数组会广播到较大数组的大小，以便使它们的形状可兼容。
如果满足以下条件之一，那么数组被称为可广播的。
数组拥有相同形状。数组拥有相同的维数，且某一个或多个维度长度为 1 。数组拥有极少的维度，可以在其前面追加长度为 1 的维度，使上述条件成立
广播的规则:
规则 1：如果两个数组的维度数不相同，那么小维度数组的形状将会在最左边补 1。规则 2：如果两个数组的形状在任何一个维度上都不匹配，那么数组的形状会沿着维度 为 1 的维度扩展以匹配另外一个数组的形状。规则 3：如果两个数组的形状在任何一个维度上都不匹配并且没有任何一个维度等于 1， 那么会引发异常。
1.1、形状相同的广播
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/b0c0a392b64046ba8de030fb704fb736)
1.2、相同维度，但其中某一个或多个维度长度为 1 的广播:
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/9adc65aee64547d29ed4efaa52bd0176)
1.3、较少的维度，默认在其前面追加长度为 1 的维度：
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/1ee42931fc314414b3245e089f32b5e6)
1.4、如果是标量的话，会广播整个数组上：
![Numpy中的广播和数组运算](http://p9.pstatp.com/large/pgc-image/312853f4d0614a61b347096659228e65)
2、广播示例
2.1、将数组赋值给a并查看数组的形状
![Numpy中的广播和数组运算](http://p9.pstatp.com/large/pgc-image/17098e4644ae46d39930f06f3fee3851)
2.2、将数组赋值给b并查看数组的形状
![Numpy中的广播和数组运算](http://p9.pstatp.com/large/pgc-image/45fd309fe228471681ebe4b6e3d9bf08)
2.3、将数组a转换为4行1列
![Numpy中的广播和数组运算](http://p9.pstatp.com/large/pgc-image/84ce163fa1744674b9d2602c5dd2ab33)
2.4、数组a+b,并查看形状
![Numpy中的广播和数组运算](http://p9.pstatp.com/large/pgc-image/8203f9d894c440be8dd1373b75d967f0)
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/e4c84d7f65be48c2a85a7cd16cb9ba63)
明显可以看出，相加前 a 的形状为 (4, 1), b 的形状为 (3, )， a+b 的结果的形状为（4, 3）。计算时，变换结果与上图类似，这里来详细介绍下：
![Numpy中的广播和数组运算](http://p9.pstatp.com/large/pgc-image/1388f40eab6b4bc5b804a4c061a88860)
2.5、现在尝试模拟一个广播的方式, 把a中的每个元素拉长三倍
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/53ca817f3e2849f5b94da6588a4182c7)
2.6、b 中的元素拉长4倍
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/a00ef215e6ad45a883d8361a59b15ce5)
2.7、aa+bb
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/e5b71f38998f4c8d9a1878b865d17950)
2.8、a+b
![Numpy中的广播和数组运算](http://p3.pstatp.com/large/pgc-image/f2a41facd5644f2d97e7bb7e7e918a3f)
这里我们就能看出,我们将数组扩展到较大的维度再进行通用函数操作,和我们直接进行广播操作结果是一样的。
三、Numpy中的运算
1、算术运算
Numpy中用于执行算术运算（如 add() ，subtract() ，multiply() 和 divide() ）的输入数组必须具 有相同的形状或符合数组广播规则。
常用数学运算函数：
数学运算函数
add(x1，x2 )
按元素添加参数，等效于 x1 + x2
subtract(x1，x2)
按元素方式减去参数，等效于x1 - x2
multiply(x1，x2)
逐元素乘法参数，等效于x1 * x2
divide(x1，x2)
逐元素除以参数，等效于x1 / x2
exp(x)
计算e的x次方。
exp2(x)
计算2的x次方。
power(x1,x2)
计算x1的x2次幂。
mod(x)
返回输入数组中相应元素的除法余数.
log(x)
自然对数，逐元素。
log2(x)
x的基础2对数。
log10(x)
以元素为单位返回输入数组的基数10的对数。
expm1(x)
对数组中的所有元素计算exp（x） - 1
log1p(x)
返回一个加自然对数的输入数组。
sqrt(x)
按元素方式返回数组的正平方根。
square(x)
返回输入的元素平方。
sin(x)
三角正弦。
cos(x)
元素余弦。
tan(x)
逐元素计算切线。
1.1、生成一个3行3列元素类型为浮点型的数组和一个1行3列的数组
![Numpy中的广播和数组运算](http://p3.pstatp.com/large/pgc-image/265c05e2940844f0a2770beee499ef11)
1.2、执行数组加法
![Numpy中的广播和数组运算](http://p3.pstatp.com/large/pgc-image/d90810ff8c224d2fb7dd234919a6cec3)
1.3、执行数组减法
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/368abc76b17b4addb039e080f73d494d)
1.4、执行数组的乘法
![Numpy中的广播和数组运算](http://p3.pstatp.com/large/pgc-image/aa8f76a9747c4456bff3db823db93638)
1.5、执行数组的除法
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/86aff7b9ac40409c8a90378234565417)
1.6、numpy.reciprocal()
此函数返回参数逐元素的倒数，由于 Python 处理整数除法的方式，对于绝对值大于 1 的整数 元素，结果始终为 0，对于整数 0，则发出溢出警告。
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/95194a75218b4697a73e2761f20d7060)
1.7、进行指数运算
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/9362b456ffbc4b53ac95cafb9387da5f)
1.8、执行幂运算
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/55529458bef748a8ab6aefc9ae14307c)
1.9、numpy.mod()
此函数返回输入数组中相应元素的除法余数。函数 numpy.remainder() 也产生相同的结果。
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/d612aae2726e49e9802b4bb74f1b14f7)
1.10、对含有复数的数组进行执行的函数
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/4582b823c1784fac93badf1884fe3fc7)
numpy.real() 返回复数类型参数的实部。numpy.imag() 返回复数类型参数的虚部。
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/fbaa561b279a4c17afb893418f8c9312)
numpy.conj() 返回通过改变虚部的符号而获得的共轭复数。
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/d9912c44e1654006954be8128b381aba)
numpy.angle() 返回复数参数的角度。函数的参数是 degree 。如果为 true，返回的角度以角度制来表示，否则为以弧度制来表示。
![Numpy中的广播和数组运算](http://p3.pstatp.com/large/pgc-image/46ae4169512e426fb5c5da59ec948cd9)
1.11、执行三角函数
![Numpy中的广播和数组运算](http://p3.pstatp.com/large/pgc-image/9d697b534755472da3a5310251427a56)
补充numpy.around()
这个函数返回四舍五入到所需精度的值。decimals 表示要舍入的小数位数。默认值为 0。如果 为负，整数将四舍五入到小数点左侧的位置
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/5a2ae9342b70432e9b7f5ac40296d4db)
同pyhon内置一样,在进行舍入的时候需要注意二进制小数的精度问题
![Numpy中的广播和数组运算](http://p3.pstatp.com/large/pgc-image/0069aae435f049c5889d0e1ce96d03d9)
numpy.floor() 向下取整
此函数返回不大于输入参数的最大整数。即标量 x 的下限是最大的整数 i ，使得 i <= x。注意 在 Python 中，向下取整总是从 0 舍入。
![Numpy中的广播和数组运算](http://p3.pstatp.com/large/pgc-image/6a7496851bfd42a19ff0bf781bc75050)
numpy.ceil()向上取整
本函数返回输入值的上限，即，标量 x 的上限是最小的整数 i ，使得 i> = x。
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/de349b45b2fe47029cf19dbef91f6ae3)
2、字符串函数
以下函数用于对 dtype 为 numpy.string 或 numpy.unicode 的数组执行向量化字符串操作。 它们基于 Python 内置库中的标准字符串函数。
2.1、numpy.char.add()
函数执行按元素的字符串连接：
![Numpy中的广播和数组运算](http://p9.pstatp.com/large/pgc-image/807cb174fd294cfba2da0207712d12f0)
2.2、numpy.char.multiply()
这个函数执行多重连接：
![Numpy中的广播和数组运算](http://p9.pstatp.com/large/pgc-image/12d4e706e67b4a9993bc670f59a1d86b)
2.3、numpy.char.center()
此函数返回所需宽度的数组，以便输入字符串位于中心，并使用 fillchar 在左侧和右侧进行填充：
![Numpy中的广播和数组运算](http://p9.pstatp.com/large/pgc-image/068840ede4e34802bfc39bd7cf3b3cf3)
2.4、numpy.char.capitalize()
函数返回字符串的副本，其中第一个字母大写：
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/e46fdc3c3f8d47a4a6dcd9173de89c9b)
2.5、numpy.char.title()
返回输入字符串的按元素标题转换版本，其中每个单词的首字母都大写：
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/1166c542055e46efa30dd33b63de9925)
2.6、numpy.char.lower()
函数返回一个数组，其元素转换为小写。它对每个元素调用 str.lower：
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/0cc6f9d0a6ac430daa3acdceadb9e5f7)
2.7、numpy.char.upper()
函数返回一个数组，其元素转换为大写。它对每个元素调用 str.upper：
![Numpy中的广播和数组运算](http://p9.pstatp.com/large/pgc-image/f4fe09dd32204b539e1ab0f5126480c4)
2.8、numpy.char.split()
此函数返回输入字符串中的单词列表。默认情况下，空格用作分隔符。否则，指定的分隔符字符用于分割字符串：
![Numpy中的广播和数组运算](http://p3.pstatp.com/large/pgc-image/a938989925d54dd7bedc65d217a05611)
2.9、numpy.char.splitlines()
函数返回数组中元素的单词列表，以换行符分割：' '，' '，' ' 都被当做换行符处理。
![Numpy中的广播和数组运算](http://p9.pstatp.com/large/pgc-image/2d64e904a262471c864b78fa5023270a)
2.10、numpy.char.strip()
函数返回数组的副本，其中元素移除了开头或结尾处的特定字符：
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/8c4301025500434a80b23859dad25596)
2.11、numpy.char.join()
这个函数返回一个字符串，其中单个字符由特定的分隔符连接：
![Numpy中的广播和数组运算](http://p3.pstatp.com/large/pgc-image/66d8d3401d7f40748427d2abc4ee4cfe)
2.12、numpy.char.replace()
这个函数返回字符串副本，其中所有字符序列的出现位置都被另一个给定的字符序列取代：
![Numpy中的广播和数组运算](http://p3.pstatp.com/large/pgc-image/02d8dc3335cd45448b4320fa0ead8262)
3、统计函数
NumPy有很多有用的统计函数，用于从数组中给定的元素中查找最小，最大，百分标准差和方差等。
常用统计函数
numpy.amin()
从给定数组中的元素沿指定轴返回最小值
numpy.amax()
从给定数组中的元素沿指定轴返回最大值
numpy.ptp()
返回沿轴的值的极差（最大值 - 最小值）
numpy.percentile()
返回特定轴的百分位数
numpy.median()
返回数组中值
numpy.mean()
返回数组的算术平均值
numpy.average()
返回数组的加权平均值
numpy.std()
返回数组的标准差
numpy.var()
返回数组的方差
3.1、numpy.amin() 和 numpy.amax()
这些函数从给定数组中的元素沿指定轴返回最小值和最大值。
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/1e4ca0a4c5ac45468bdd8927fede7f25)
3.3、numpy.ptp()
本函数返回沿轴的值的极差（最大值 - 最小值）。
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/91e003f5cabc41d2a9434469d0200fec)
3.4、numpy.percentile()
百分位数是统计中使用的度量，表示小于这个值得观察值占某个百分比。函数numpy.percentile() 接受以下参数。
numpy.percentile(a, q, axis):
a 输入数组q 要计算的百分位数，在 0 ~ 100 之间axis 沿着它计算百分位数的轴
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/27df8abdd4074fd3af0b9b32ecc2cce7)
3.5、numpy.median()
中值定义为将数据样本的上半部分与下半部分分开的值。
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/9a057835ae0d4523b9b65323cfc99337)
3.6、numpy.mean()
算术平均值是沿轴的元素的总和除以元素的数量。函数返回数组中元素的算术平均值。如果提供了轴，则沿其计算。
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/536b71d6876d426580e442e938a1d816)
3.7、numpy.average()
加权平均值是由每个分量乘以反映其重要性的因子得到的平均值。函数根据在另一个数组中给出的各自的权重计算数组中元素的加权平均值。该函数可以接受一个轴参数。如果没有指定轴，则数组会被展开。
考虑数组 [1,2,3,4] 和相应的权重 [4,3,2,1] ，通过将相应元素的乘积相加，并将和除以权重的和，来计算加权平均值。
加权平均值 = (1*4+2*3+3*2+4*1)/(4+3+2+1)
![Numpy中的广播和数组运算](http://p9.pstatp.com/large/pgc-image/66f4b8ef762940d9842cdfdd086ced46)
不指定权重时相当于mean函数
![Numpy中的广播和数组运算](http://p3.pstatp.com/large/pgc-image/ab8a0a8bcc8440d2a7f13ee8a10c7b3f)
![Numpy中的广播和数组运算](http://p3.pstatp.com/large/pgc-image/45ef4d7aa4f64944b10eacd81a4e72bb)
如果returned参数设为true,则返回权重的和
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/ae2f9d8076e940dfbe1e2d9888650bd7)
3.8、numpy.std()
标准差是与均值的偏差的平方的平均值的平方根。标准差公式如下： std = sqrt(mean((x - x.mean())**2))
![Numpy中的广播和数组运算](http://p3.pstatp.com/large/pgc-image/10192c88bffb43559bf1a260327b8c75)
方差是偏差的平方的平均值，即mean((x - x.mean())** 2)。换句话说，标准差是方差的平方根。
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/d234a750e69c41618b8f885f17a5d251)
4、排序、搜索和计数函数
4.1、numpy.sort()函数返回输入数组的排序副本。
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/2b0cc717f3f340bb9617573e30bb38c8)
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/48dfe42a506e4bd5a2c4f5cc2d069468)
4.2、numpy.argsort()
函数对输入数组沿给定轴执行间接排序，并使用指定排序类型返回数据的索引数组。这个索引 数组用于构造排序后的数组。
![Numpy中的广播和数组运算](http://p9.pstatp.com/large/pgc-image/f0d64ec96f144fff958a3752d4a2179e)
4.3、numpy.lexsort()
函数使用键序列执行间接排序。键可以看作是电子表格中的一列。该函数返回一个索引数组，使用它可以获得排序数据。注意，最后一个键恰好是 sort 的主键。
![Numpy中的广播和数组运算](http://p3.pstatp.com/large/pgc-image/37b1d1131d124c36a9d6ca6868b7504b)
4.4、numpy.argmax() 和 numpy.argmin()
这两个函数分别沿给定轴返回最大和最小元素的索引
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/86d5c1bc42334c8988a16040ab4f73d4)
4.5、numpy.nonzero()
函数返回输入数组中非零元素的索引。
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/5fd193831ce94a61bbaf137cab85c178)
4.6、numpy.where()
函数返回输入数组中满足给定条件的元素的索引。
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/e1c9936c811c4efe95b293d9640d21ab)
4.7、numpy.extract()
函数返回满足任何条件的元素。
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/aedb7cf44c2a411fbea69e06292d6668)
5、转置
Numpy 的转置可以按照你的需要对数组的轴进行转换。
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/3697e70c140a429682d8be36067423fc)
需要注意的是，转置只能发生在二维及以上的维度的数组上生效，一维的数组只有一个维度是 不可以转置的。
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/fb3e513b8c5c494dbfc860ecdd540516)
6、唯一化和集合逻辑
np.unique() 去重函数np.in1d() 验证元素是否在给定序列中np.intersect1d() 求交集np.union1d() 求并集np.setdiff1d() 求差集
6.1去重
字符串去重
![Numpy中的广播和数组运算](http://p9.pstatp.com/large/pgc-image/2ca0cfd37cc040d7b84a5343c2b333ae)
整数去重
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/8d03c7bb413a49e3b96afda58a63b017)
布尔值去重
![Numpy中的广播和数组运算](http://p9.pstatp.com/large/pgc-image/d8491bfc6d924a3b9d03439810943c40)
布尔值的any()和all()函数
![Numpy中的广播和数组运算](http://p9.pstatp.com/large/pgc-image/197d5ca001654fcca921840a0e0092be)
6.2、检验元素
![Numpy中的广播和数组运算](http://p9.pstatp.com/large/pgc-image/607d5d0fac064a9aa7b52b38c5983ccd)
6.3、求交集
![Numpy中的广播和数组运算](http://p3.pstatp.com/large/pgc-image/feaac1d8560247f995aadb6652067c86)
6.4、求并集
![Numpy中的广播和数组运算](http://p3.pstatp.com/large/pgc-image/7fb87ec3fabe41328c73dc891307c5c8)
6.5、求差集
![Numpy中的广播和数组运算](http://p1.pstatp.com/large/pgc-image/365186af62524206bb885a02da8233ef)
[了解更多](https://i.snssdk.com/pgcui/extern_redirect/?item_id=6677441250955624973)


# Python: NumPy中的多维数组ndarray - zj360202的专栏 - CSDN博客





2017年11月22日 16:57:22[zj360202](https://me.csdn.net/zj360202)阅读数：1083








1 Python中的数组

- 用list和tuple等数据结构表示数组


 - 一维数组 List1 = [1, 2, 3]



 - 二维数组 Tuple1 = ([1, 2, 3], [4, 5, 6], [7, 8, 9])


但是：列表是动态指针数组，它保存的是对象的指针，其元素可以是任意类型的对象。比如要保存上述的List1，需要3个指针和3个整数对象，浪费内存和计算时间。


- array模块（非内置模块）


 - 通过array函数创建数组 array.array()


 - 提供append、insert和read等方法


但是：array模块中的array函数不支持多维数组，且函数功能不丰富。




2 NumPy中的N维数组ndarray



- NumPy中基本的数据结构


- 所有元素是同一种类型


- 别名array（数组）


- 节省内存，提高CPU计算时间


- 有丰富的函数

注：NumPy的思维模式是面向数组。





3 ndarray数组属性


- 下标从0开始。



- 一个ndarray数组中的所有元素的类型必须相同。



- 轴（axis）：每一个线性的数组称为是一个轴，也就是维度（dimensions）。比如，二维数组相当于是两个一维数组，其中第一个一维数组中每个元素又是一个一维数组，所以一维数组就是ndarray中的轴，第一个轴（也就是第0轴）相当于是底层数组，第二个轴（也就是第1轴）是底层数组里的数组。


很多时候可以声明axis。axis=0，表示沿着第0轴进行操作，即对每一列进行操作；axis=1，表示沿着第1轴进行操作，即对每一行进行操作。



- 秩（rank）：维数，一维数组的秩为1，二维数组的秩为2，以此类推。即轴的个数。


- 基本属性


ndarray1.ndim - 秩



ndarray1.shape - 维度 # 是一个元组，表示数组在每个维度上的大小。比如，一个二维数组，其维度表示“行数”和“列数”。该元组的长度即为秩。



ndarray1.size - 元素总个数# 等于shape属性中元组元素的乘积。


ndarray1.dtype - 元素类型



对于科学计算来说，Python中自带的整型、浮点型和复数类型远远不够，因此NumPy中添加了许多数据类型。如下：


![](http://image.sciencenet.cn/album/201707/04/155315mqlamm9zgjwxgaaq.png)



ndarray1.itemsize - 元素字节大小# 即元素所占内存空间大小，例如，元素类型为float64的数组的itemsiz属性值为8(=64/8)，元素类型为complex32的数组的itemsize属性为4(=32/8)。



ndarray1.data - 包含实际数组元素的缓冲区，由于一般通过数组的索引获取元素，所以通常不需要使用这个属性。



例如：以下5x5的数组，可以看成由5个一维数组构成，每个一维数组包含5个元素；第一维被称为第0轴（列），第二维被称为第1轴（行）；秩为2，维度为（5，5），元素总个数为25。


![](http://image.sciencenet.cn/album/201707/01/1825553jym9ty1p98s9e2s.png)

![](http://image.sciencenet.cn/album/201707/04/041756gpc2nnr2v8llm7z8.png)


4 ndarray的创建


常见的创建函数如下：


![](http://image.sciencenet.cn/album/201707/03/160112t8m8t1xmidm8s8lq.png)


4.1 从已有数据中创建多维数组


4.1.1 从list, tuple对象中创建 - array()

array() - 创建多维数组。


np.array(object, dtype=None, copy=True, order='K', subok=False, ndmin=0)


object — list或tuple对象。强制参数。


dtype — 数据类型。可选参数。


copy — 默认为True，对象被复制。可选参数。


order — 数组按一定的顺序排列。C - 按行；F - 按列；A - 如果输入为F则按列排列，否则按行排列；K - 保留按行和列排列。默认值为K。可选参数。


subok — 默认为False，返回的数组被强制为基类数组。如果为True，则返回子类。可选参数。


ndmin — 最小维数。可选参数。



注：array函数的参数必须是由方括号括起来的列表，而不能使用多个数值作为参数调用array。

![](http://image.sciencenet.cn/album/201707/04/184540hqovh1681tg1d2oo.png)

4.1.2 从字符串中读取 - fromstring()

fromstring() - 从字符串中读取数据，并将其转换为一维数组。


np.fromstring(string, dtype=float, count=-1, sep='')


string — 包含数据的字符串。强制参数。


dtype — 数据类型，默认为浮点型。可选参数。


count — 从左到右读取数据的个数。默认为-1，表示读取所有数据。可选参数。


sep — 分隔符。若不指定分隔符，或指定为空，则字符串包含的数据被解译为二进制数据，否则为带有小数的ASCII文本。可选参数。


![](http://image.sciencenet.cn/album/201707/04/192726ee33wkme9er33ax3.png)

4.1.3 从可迭代对象中读取 - fromiter()

fromiter() - 从可迭代对象中读取数据，并将其转换为一维数组。


np.fromiter(iterable, dtype, count=-1)


iterable — 可迭代对象。不能有嵌套。强制参数。


dtype — 数据类型。强制参数。


count — 表示从可迭代对象中读取的项目数。默认为-1，表示读取所有数据。可选参数。

![](http://image.sciencenet.cn/album/201707/04/194450n267tk67v2g40zgv.png)


4.1.4 以函数式创建 - fromfunction()


fromfunction() - 在每个坐标轴上执行函数表达式，用得到的数据创建数组。



np.fromfunction(function, shape, dtype)



function — 可调用的函数。必选参数。


shape — 要创建的数组的维度。其长度与函数参数的个数一致。必选参数。



dtype — 数据类型，默认为浮点型。可选参数。


![](http://image.sciencenet.cn/album/201707/07/213828sdeicfhqpksoqoxx.png)


4.2 创建特定形状的多维数组


4.2.1 创建‘全1’数组- ones(), ones_like()


ones() - 创建给定形状的ndarray，并将数组中所有元素填充为1。


np.ones(shape, dtype=None, order='C')


shape — 维度。强制参数。


dtype — 数据类型。默认为np.float64。可选参数。


order — 数组按一定的顺序排列。C - 按行；F - 按列。默认值为C。可选参数。


ones_like() -  创建与参数数组形状和类型相同的ndarray，并将数组中所有元素填充为1。


np.ones_like(a, dtype=None, order='K', subok=True)


a — 给定的数组。其维度和数据类型决定着要创建的数组的属性。强制参数。



dtype — 数据类型。默认的“None”意为“a.dtype”。可选参数。


order — 数组按一定的顺序排列。C - 按行；F - 按列；A - 如果输入为F则按列排列，否则按行排列；K - 保留按行和列排列。默认值为K。可选参数。


subok — 默认为True，返回的数组被强制为子类数组。如果为False，则返回基类数组。可选参数。

![](http://image.sciencenet.cn/album/201707/04/2026393zfdoabyuohdnhyk.png)


4.2.2 创建‘全0’数组 - zeros(), zeros_like()


zeros() - 创建给定形状的ndarray，并将数组中所有元素填充为0。


np.zeros(shape, dtype=float, order='C')


shape — 维度。强制参数。


dtype — 数据类型。默认为np.float64。可选参数。


order — 数组按一定的顺序排列。C - 按行；F - 按列。默认值为C。可选参数。


zeros_like() -  创建与参数数组形状和类型相同的ndarray，并将数组中所有元素填充为0。


np.zeros_like(a, dtype=None, order='K', subok=True)


a — 给定的数组。其维度和数据类型决定着要创建的数组的属性。强制参数。



dtype — 数据类型。默认的“None”意为“a.dtype”。可选参数。


order — 数组按一定的顺序排列。C - 按行；F - 按列；A - 如果输入为F则按列排列，否则按行排列；K - 保留按行和列排列。默认值为K。可选参数。


subok — 默认为True，返回的数组被强制为子类数组。如果为False，则返回基类数组。可选参数。

![](http://image.sciencenet.cn/album/201707/04/202904bwziseg4nv64z4ks.png)


4.2.3 创建空数组 - empty(), empty_like()

empty() - 返回一个没有初始化内存的数组，该数组是空的。



np.empty(shape, dtype=float, order='C')


shape — 维度。强制参数。


dtype — 数据类型。默认为np.float64。可选参数。


order — 数组按一定的顺序排列。C - 按行；F - 按列。默认值为C。可选参数。


empty_like() - 返回一个与给定数组的维度和数据类型相同的新数组。



np.empty_like(a, dtype=None, order='K', subok=True)


a — 给定的数组。其维度和数据类型决定着要创建的数组的属性。强制参数。



dtype — 数据类型。默认的“None”意为“a.dtype”。可选参数。


order — 数组按一定的顺序排列。C - 按行；F - 按列；A - 如果输入为F则按列排列，否则按行排列；K - 保留按行和列排列。默认值为K。可选参数。


subok — 默认为True，返回的数组被强制为子类数组。如果为False，则返回基类数组。可选参数。

![](http://image.sciencenet.cn/album/201707/04/2032100z85fhuiw8uj5f8w.png)


4.2.4 自主填充数组 - full(), full_like()

full() - 返回一个指定维度和数据类型的新数组，并用指定的值填充。



np.full(shape, fill_value, dtype=None, order='C')


shape — 维度。强制参数。


fill_value — 填充值。强制参数。



dtype — 数据类型。默认的“None”意为“np.array(fill_value).dtype”。可选参数。


order — 数组按一定的顺序排列。C - 按行；F - 按列。默认值为C。可选参数。

full_like() - 返回一个与给定数组的维度和数据类型相同的新填充数组。



np.full_like(a, fill_value, dtype=None, order='K', subok=True)


a — 给定的数组。其维度和数据类型决定着要创建的数组的属性。强制参数。


fill_value — 填充值。强制参数。



dtype — 数据类型。默认的“None”意为“np.array(fill_value).dtype”。可选参数。


order — 数组按一定的顺序排列。C - 按行；F - 按列；A - 如果输入为F则按列排列，否则按行排列；K - 保留按行和列排列。默认值为K。可选参数。


subok — 默认为True，返回的数组被强制为子类数组。如果为False，则返回基类数组。可选参数。

![](http://image.sciencenet.cn/album/201707/06/043356k8cylyipu5ckkk8z.png)


4.2.5 创建对角线矩阵 - eye()

eye() - 返回一个矩阵（即二维数组），其对角线上均为1，其余位置均为0。


eye(N, M=None, k=0, dtype=<class 'float'>)


N — 行数。强制参数。


M — 列数。默认值为None，意为“同N”。可选参数。


k — 对角线索引。k=0，指主对角线；k为正整数，指从第k个值开始的右上对角线；k为负整数，指从第-k个值开始的左下对角线。可选参数。



dtype — 数据类型。默认为浮点数。可选参数。

![](http://image.sciencenet.cn/album/201707/07/164420t6r21zy3tvll1yv1.png)


4.2.6 创建单位矩阵 - identity()

identity() - 创建单位矩阵，即主对角线上为1而其余位置为0的方阵。



identity(n, dtype=None)


n — 行数和列数。即生成的是nxn的方阵。强制参数。


dtype — 数据类型。默认为浮点数。可选参数。

![](http://image.sciencenet.cn/album/201707/07/164856m2juc2m0bjzb59v2.png)

4.2.7 创建数组的浅复制 - copy()

copy() - 创建已知数组的浅复制。


copy(a, order='K')



a — 已知数组。强制参数。


order — 数组按一定的顺序排列。C - 按行；F - 按列；A - 如果输入为F则按列排列，否则按行排列；K - 与数组a的排列尽可能接近。默认值为K。可选参数。

![](http://image.sciencenet.cn/album/201707/07/1744556rpc6p6rzeb5v7bd.png)



4.2.8 改变数组的形状 - reshape()

reshape() - 改变数组的形状。通过reshape生成的新数组和原始数组共用一个内存，也就是说，若更改其中数组的元素，另一个数组也将发生改变。


reshape(a, newshape, order='C')


ndarray1.reshape(newshape)



a — 已知数组。强制参数。


newshape — 新的维度，为整数或由整数组成的元组。强制参数。



order — 数组按一定的顺序排列。C - 按行；F - 按列；A - 如果输入为F则按列排列，否则按行排列；K - 与数组a的排列尽可能接近。默认值为C。可选参数。

![](http://image.sciencenet.cn/album/201709/15/031034zr7pgg2z1dc7drwp.png)


4.3 从numerical range 创建多维数组


4.3.1 创建等差数列数组 - arange()


arange() - 创建一个一维的等差数列数组，与Python中的range()函数类似。区别在于，np.arange()返回的是一个numpy数组，而Python中的range()函数返回的是一个列表。



np.arange(start, stop, step, dtype=None)


start — 起始值（取得到）。默认值为0。可选参数。


stop — 终止值（取不到）。强制参数。


step — 步长。默认值为1。若指定step，则start值必须给出。可选参数。



dtype — 数据类型。若不指定数据类型，则通过其他参数（如start, stop, step）判断。可选参数。

![](http://image.sciencenet.cn/album/201707/07/221011p1wp6ntqaooont6v.png)


4.3.2 创建等差数列数组 - linspace()


linspace()- 创建一个一维数组，在给定的区间上num等分。



np.linspace(start, stop, num=50, endpoint=True, retstep=False, dtype=None)


start — 区间起始值。强制参数。


stop — 区间终止值（是否取得到，需要设定参数endpoint）。强制参数。


num — 等分的个数。默认值为50。可选参数。


endpoint — 若为True（默认），则可以取到区间终止值；否则取不到。可选参数。


retstep — 若为True，则返回由生成的数组和步长构成的元组；若为False（默认），则只返回生成的数组。可选参数。



dtype — 数据类型。若不指定数据类型，则通过其他参数判断。可选参数。

![](http://image.sciencenet.cn/album/201707/07/22325070agg9946ko9kttb.png)


4.3.3 创建对数等差数列数组 - logspace()


logspace() - 创建对数等差数列数组，底数为base。



np.logspace(start, stop, num=50, endpoint=True, base=10.0, dtype=None)


start — 区间起始值为base的start次方。强制参数。


stop — 区间终止值为base的stop次方（是否取得到，需要设定参数endpoint）。强制参数。


num — 等分的个数。按照对数，即start和stop值进行等分。默认值为50。可选参数。


endpoint — 若为True（默认），则可以取到区间终止值；否则取不到。可选参数。


base — 公比。



dtype — 数据类型。若不指定数据类型，则通过其他参数判断。可选参数。

![](http://image.sciencenet.cn/album/201707/07/234251fnfd8ofktxrxe999.png)





4.3.4 创建指数等差数列数组 - geomspace()


geomspace() - 创建指数等差数列数组。



np.geomspace(start, stop, num=50, endpoint=True, dtype=None)


start — 区间起始值。强制参数。


stop — 区间终止值,（是否取得到，需要设定参数endpoint）。强制参数。


num — 等分的个数，按照指数等分。默认值为50。可选参数。


endpoint — 若为True（默认），则可以取到区间终止值；否则取不到。可选参数。


dtype — 数据类型。若不指定数据类型，则通过其他参数判断。可选参数。

![](http://image.sciencenet.cn/album/201707/07/234018vnnrb8b88mni84aa.png)


4.3.5 创建坐标矩阵 - meshgrid()


meshgrid()- 根据坐标轴向量创建坐标轴矩阵。



meshgrid(*xi, indexing='xy', sparse=False, copy=True)






x1, x2,...,xn — 构成网格的坐标轴向量。强制参数。


indexing — 坐标矩阵索引方法。默认为笛卡尔坐标系索引‘xy’；矩阵索引‘ij’。可选参数。


sparse — 若为True，则返回稀疏网格以节省内存；默认为False。可选参数。


copy — 若为True（默认），。。。可选参数。





4.3.6 mgrid()


4.3.7 ogrid()





5 ndarray的切片和索引


Numpy 中多维数组的切片操作与 Python 中 list 的切片操作一样，由 start, stop, step 三个部分组成。



一维数组：


![](http://image.sciencenet.cn/album/201709/15/164521a1lfkfz7frx7ndtk.png)


二维数组：

![](http://image.sciencenet.cn/album/201709/15/1645358v3qzkq41nq1kkvf.png)


三维数组：


![](http://image.sciencenet.cn/album/201709/15/164541pfbpempybjyaqqpp.png)

用数组来索引

![](http://image.sciencenet.cn/album/201709/15/175109531icirtt2v9c9tg.png)

布尔型索引

![](http://image.sciencenet.cn/album/201709/15/175718swrnu2r6yjqjuqrn.png)

列出ndarray所有的行，列和元素

![](http://image.sciencenet.cn/album/201709/15/1803455wh75ktrwldw88nw.png)

注：flatten是一个展开性质的函数，将多维的矩阵进行展开成1行的数列。而flat是一个迭代器，本身是一个object属性。


最值索引



默认情况下，索引为数组展开后的索引，即"By default, the index is into the flattened array"。



最小值的索引：np.argmin(a, axis=None, out=None)


                        np.nanargmin(a, axis=None, out=None)



最大值的索引：np.argmax(a, axis=None, out=None)


np.nanargmax(a, axis=None, out=None)


![](http://blog.sciencenet.cn/home.php?mod=attachment&filename=1.png&id=102821)


6 ndarray的基本运算


加减乘除、点乘、乘方、整余数


![](http://blog.sciencenet.cn/home.php?mod=attachment&filename=1.png&id=102813)

![](http://blog.sciencenet.cn/home.php?mod=attachment&filename=2.png&id=102814)








转载本文请联系原作者获取授权，同时请注明本文来自刘洋洋科学网博客。

链接地址：[http://blog.sciencenet.cn/blog-3031432-1064033.html](http://blog.sciencenet.cn/blog-3031432-1064033.html)



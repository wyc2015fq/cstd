# VC数字图像处理编程讲座之十一 - 深之JohnChen的专栏 - CSDN博客

2006年04月05日 13:28:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：5550


**VC数字图像处理编程讲座之十一    **

                                                                                                                cndg

       这一章主要介绍图像变换。
数学领域内有多种变换，如傅立叶变换、拉普拉斯变换、卫变换等等。但各种数学变换的用途只有一个，即利用某种变换使所遇到的问题更容易更方便地得到解决。当然，每一种具体的数学变换的应用场合和侧重解决的问题不同。数字图像处理技术是一门应用学科，和其它应用学科一样，它是建立在一定的数学理论基础之上的科学，离开数学，这门学科将不复存在。因此，在解决数字图像处理的具体问题时，作为解决技巧，就必须要用到各种数学变换，在此称其为图像变换。
    对于遥感图像的变换处理，在以下两方面有着十分重要的作用：第一，由于图像在变换域进行增强处理要比在空间域进行增强处理简单易行，因而可以通过图像变换简单而有效地实现增强处理，当然以增强为目的的变换处理，其结果还需变换回空间域；第二，通过图像变换可以对图像进行特征抽取。例如利用图像的功率谱特征来分析提取图像中的信理信息。

1、**傅立叶变换的定义及基本概念**

![](http://course.cug.edu.cn/rs/COURSE/images/bluedrop.gif) 傅立叶变换

    傅立叶变换是一种正交变换，它广泛地应用于很多领域，取得了良好的效果。由于它不仅能把空间域中复杂的卷积运算转化为频率域中的乘积运算，还能在频率域中简单而有效地实现增强处理和进行特征抽取，故而在图像处理中也得到了广泛的应用。

    定义：令 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image002.gif)为实变量 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image005.gif)的连续函数，如果![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image002.gif) 满足下面的狄里赫莱条件：
   （1）有有限个间断点；
   （2）有有限个极值点；
   （3）绝对可积；
则有以下二式成立：

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image006.gif)

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image008.gif)

式中![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image004.gif) 为时域变量，![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image010.gif) 为频率变量，![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image012.gif) 为虚数单位，![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image014.gif) 。通常把上述两式称为傅立叶变换对。
    函数 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image002.gif)的傅立叶变换通常是一个复数，它可表示如下，![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image016.gif) ，式中：![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image018.gif) 和![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image020.gif) 分别是![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image018.gif) 的实部和虚部。如果表示成指数形式则为：![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image022.gif) ，式中![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image024.gif) 和 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image026.gif)。幅度函数![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image028.gif) 被称为![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image002.gif) 的傅立叶谱，而 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image030.gif)为相角。傅立叶谱的平方：![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image032.gif) ，一般称为![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image002.gif) 的能量谱。
    实际情况表明，傅立叶变换的条件几乎总是可以满足的，而且连续非周期函数的傅立叶谱是连续的非周期函数，连续的周期函数的傅立叶谱是离散的非周期函数。
傅立叶变换可以容易地推广到二维函数![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image034.gif) 。

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image036.gif)

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image038.gif)

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image040.gif)

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image042.gif)

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image044.gif)


这一章主要**离散的傅立叶变换**

    由于遥感图像是由灰度值组成的二维离散数据矩阵，对它进行傅立叶变换就必须知道离散的傅立叶变换。离散傅立叶变换使数学方法与计算机技术建立了联系，这就使傅立叶变换这样的数学工具在实用中开辟了一条宽阔的道路。
    一维离散变换为：

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image046.gif)

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image048.gif)

式中，![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image050.gif) 。
二维离散变换为：

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image052.gif)

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image054.gif)

    因为在离散的情况下，![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image056.gif) 和![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image058.gif) 两者总是存在的，所以和连续的情况不同，我们不必考虑关系离散傅立叶变换的存在性。

二维离散傅立叶变换的性质

    此处约定离散函数的大小为N×N

![](http://course.cug.edu.cn/rs/COURSE/images/bluedrop.gif) （一） 平均值 

傅开变换域原点的频谱分量 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image060.gif)是空间域的平均值的N倍，即由右边（1）式：

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image062.gif)

![](http://course.cug.edu.cn/rs/COURSE/images/bluedrop.gif) （二） 变换域的周期性

设m，n为整数，m，n=0，±1，±2，…，将u+mN和v+nN代入右（1）式中，有：

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image064.gif)![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image066.gif)

上式中，右边第二个指数项![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image068.gif) 为单位值，因此傅立叶变换是周期性的，即：![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image070.gif)

![](http://course.cug.edu.cn/rs/COURSE/images/bluedrop.gif) （三） 对称共轭性 

由离散傅立叶变换定义可方便地证明，傅立叶变换满足：

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image072.gif)

![](http://course.cug.edu.cn/rs/COURSE/images/bluedrop.gif) （四） 平移性 

如用 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image074.gif)表示傅立叶变换对，则平移性是指：

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image076.gif)

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image078.gif)

由于 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image080.gif)，因而说明f(x,y) 的移动并不影响它的傅立叶变换的幅度。 

![](http://course.cug.edu.cn/rs/COURSE/images/bluedrop.gif) （五） 分配性和比例性 

设![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image083.gif) ，若![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image085.gif) 和 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image087.gif)分别是 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image089.gif)的傅氏变换，则根据定义可知其分配性，即（线性特性）：

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image091.gif)

 同时，也容易证明其比例性，即： 

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image093.gif)

![](http://course.cug.edu.cn/rs/COURSE/images/bluedrop.gif) （六） 可分离性 

可分离性的主要优点是可以通过两次一维变换来实现一个二维变换（或反变换）。可分为两步。第一步，先沿y 轴对f(x,y) 求一维离散傅立叶变换，得中间结果![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image097.gif) ，即：

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image099.gif)

 第二步，再沿x 轴对 F1(x,v)求一维离散傅氏变换，得最后结果 F(u,v)，即：

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image105.gif)

![](http://course.cug.edu.cn/rs/COURSE/images/bluedrop.gif) （七） 旋转性质 

由于在极坐标下表示二维函数图形的旋转特性非常方便，所以将坐标进行转换。空间域坐标变换为：

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image107.gif)， ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image109.gif)

频率域坐标变换为：

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image111.gif)， ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image113.gif)

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image115.gif)便是极坐标中的傅氏变换对。可以证明二维离散傅氏变换具有如下旋转性质： 

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image117.gif)

![](http://course.cug.edu.cn/rs/COURSE/images/bluedrop.gif) （八） 微分性质 

傅立叶变换的微分性质可表示为：

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image119.gif)

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image121.gif)

 作为特例，定义拉普拉斯（Laplace）算式为：（在图像增强中会用到）

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image123.gif)

 则由微分性质可知laplace算子的傅氏变换为 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image125.gif)，即：![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image127.gif) 便是在模式识别技术中经常用到的laplace算子。 

快速傅立叶变换（FFT）

    傅立叶变换除了具有前面介绍过的许多独特性质之外，还有一个突出的优点，即它的快速算法（FFT）。FFT的存在使计算机节省了大量的计算时间。目前，快速傅立叶变换有好几种，最常用的是由Cooley和Tukey于1965年首先提出来的一种FFT快速算法。利用这种算法在实现f (x) (x =0,1,…, N－1)的离散傅立叶变换时，需要完成的复数乘法和加法的次灵敏分别为1/2Nlog2N和N log2N。而直接用离散傅立叶变换的原始式（3-25）式计算f (x)的傅立叶变换时，需要进行的复数乘法和加法次数分别为N2和N（N－1）≈N2（后者称为直接傅立叶变换（DFT））。通过表3-1比较N取不同的值时N2与Nlog2N的差异，可明显看出FFT在计算速度上的巨大优势。
    对于一幅M×N的数字图像，如果用DFT法计算其傅立叶变换，需要完成的复数乘法次数复数加法次数分别为M2N2和MN（M－1）（N－1）。对于目前常用的512×512图像处理系统，用DFT法计算这样一幅图像的傅立叶变换，复数加法和乘法次数均将达到700亿次，可以想象其运算量的巨大。因而FFT法的产生可以说是数字图像处理领域的一次革命。
    下面开始讨论Cooley和Tukey的逐次加倍法FFT的基本原理，它要求N为2的整数次幂，即
N=2*n* （n为正整数）
因为二维离散傅立叶变换可通过两次一维离散傅立叶变换来实现，所以只讨论一维FFT即可。
    由一维离散傅立叶变换的表达式，

即    ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image206.gif)
设    ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image208.gif)
则    ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image210.gif)（4-15）
因为N为偶数，所以（4-15）式右边共有偶数个项目相加，可将其分为两部分之和，即N/2个偶数项和N/2个奇数项之和，则（4-15）式可写为
![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image212.gif)
因为 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image214.gif)
所以，有
![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image216.gif)（4-16）
我们知道F(u)(u=0,1,2,...,N-1) 共有N个离散值，如果把前N/2个离散值与后N/2个离散值分成两个式子来表示，则有
![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image220.gif)![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image222.gif)（4-17）

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image224.gif)![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image222.gif)（4-18）
这里（4-17）式表示了F(u)的前N/2个离散值，（4-18）式表示了F(u)的后N /2个离散值。（4-18）式中，因为
![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image226.gif)

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image228.gif)

则（4-18）式可简化为：
![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image230.gif)（4-19）
令
![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image232.gif)![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image222.gif)（4-20）
![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image234.gif)![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image222.gif)（4-21）
则（4-17）式得（4-19）式可写为
![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image236.gif)（4-22）
由（4-20）式、（4-21）式和（4-22）式可以看出，计算一维离散傅立叶变换，可以分两步进行。第一步，先按定义求出 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image238.gif)和 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image240.gif)，按（4-22）式计算出傅立叶变换F (u)的前N/2个点和后N/2个点。按这个方式求出全部N个点的F (u)，需要的复数乘法次数为N (N+1)/2，复数加法次数为N (N/2－1)。可以算出，这种算法的速度约是DFT算法的两倍。
    FFT的速度优势并不局限于此。因为N =2n，N是偶数，当n≥2时，N/2仍然是偶数。因此，![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image238.gif) 和![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image240.gif)![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image222.gif)的计算仍然可以按同样方法分解为偶、奇两个子集，得出如下结果

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image244.gif)（4-23）
![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image246.gif)（4-24）
式中 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image248.gif)![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image250.gif)（4-25）

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image252.gif)![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image250.gif)（4-26）

对比（4-23）式（4-24）与（4-22）式可知，计算 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image238.gif)和 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image240.gif)![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image222.gif)相当于计算N/2个点离散函数的傅立叶变换。如果按这种方法一直进行下去，直到求出两点离散函数的傅立叶变换时为止。这就是快速傅立叶变换的原理。
    图4-2给出了当N=8（N=3）时的快速傅立叶变换的流程图。可以证明，这种FFT算法所要进行的复数乘法和加法次数分别为 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image256.gif)和 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image258.gif)。
从图4-2可以看出，f (x)的8个样本值中，f (0)与f (4)、f (2)与f (6)、f (1)与f (5)、f (3)与f (7)两两结合计算两点变换、 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image260.gif)、![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image262.gif) 、![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image264.gif) 和 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image266.gif)，最后结果f (u)是按自然顺序排列的。那么当N>8且N =2*n*时，为使f (u)（u = 0,1,2,…, N－1）按自然顺序排列。f (x)（x = 0,1,2,…, N－1）应按怎样的次序出现来首先完成两点变换呢？解决这个问题的办法称为“比特倒置”整序处理。“比特倒置”的原则是把f (x)的自然顺序号0,1,2,…, N－1改写成二进制数，然后把这些二进制数作比特位倒置，再将倒置后的二进制数写成对应的十进制数，这十进制的值就是整序处理后f (x)的输入序号。表4-1给出了N=8时的整序处理过程。

表4-1  *N*=8时的“比特倒置”整序过程

|自然顺序|二进制码|比特倒置|整序后的顺序|
|----|----|----|----|
|0|000|000|0|
|1|001|100|4|
|2|010|010|2|
|3|011|110|6|
|4|100|001|1|
|5|101|101|5|
|6|110|011|3|
|7|111|111|7|

    现在考虑离散傅立叶反变换的快速算法。根据离散傅立叶反变换的表达式
![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image268.gif)![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image270.gif)
两边取正轭，且乘以1/N，则有
![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image272.gif)![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image270.gif)
此式形式上与（4-15）式相同，应用上面所讨论的FFT的方法可求出F*(u) 的傅立叶变换 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image274.gif)，由此式可得出f (x)。

2、 **K-L变换**

    K-L（Karhunen-Loeve）变换也叫做主成分分析或主分量分析，是在统计特征基础上的多维（如多波段）正交线性变换，它也是遥感数字图像处理中最常用也是最有用的一种变换算法。
    由于遥感图像的不同波段之间往往存在着很高的相关性，从直观上看，就是不同波段的图像很相似。因而从最有用信息的角度考虑，有相当大一部分数据是多余和重复的。K-L变换的目的就是把原来多波段图像中的有用信息集中到数目尽可能少的新的主成分图像中，并使这些主成分图像之间互不相关，也就是说各个主成分包含的信息内容是不重叠的，从而大大减少总的数据量并使图像信息得到增加。
有矩阵：
![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image276.gif)
其中，m和n分别为波段数（或称变量数）和每幅图像中的像元数；矩阵中每一行矢量表示一个波段的图像。
    对于一般的线性变换Y=TX，如果变换矩阵T是正交矩阵，并且它是由原始图像数据矩阵X的斜方差矩阵S的特征向量所组成，则此式的变换称为K-L变换。
    K-L变换的具体过程如下：
    第一步，根据原始图像数据矩阵X，求出它的协方差矩阵S，X的协方差矩阵为：
![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image278.gif)
式中：![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image280.gif) ；![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image282.gif)（即为第i个波段的均值）；
![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image284.gif)；S是一个实对称矩阵。
    第二步，求S矩阵的特征值λ和特征向量，并且成变换矩阵T。考虑特征方程：
![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image286.gif)）
式中，I为单位矩阵，U为特征向量。
    解上述的特征方程即可求出协方差矩阵S的各个特征值![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image288.gif) ，将其按![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image290.gif) 排列，求得各特征值对应的单位特征向量（经归一化）Uj：
![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image292.gif)
若以各特征方量为列构成矩阵，即
![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image294.gif)
U矩阵满足：UTU=UUT=I（单位矩阵），则U矩阵是正交矩阵。
U矩阵的转置矩阵即为所求的K-L变换的变换矩阵T。
有了变换矩阵T，将其代入Y=TX，则：
![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image296.gif)
式中Y矩阵的行向量![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image298.gif) 为第j主成分。
经过K-L变换后，得到一组（m个）新的变量（即Y的各个行向量），它们依次被称为第一主成分、第二主成分、…第m主成分。这时若将Y矩阵的各行恢复为二维图像时，即可以得到m个主成分图像。
**K-L变换的性质和特点**

    K-L变换是一种线性变换，而且是当取Y的前p（p<m）个主成分经反变换而恢复的图像 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image300.gif)和原图像X在均方误差最小意义上的最佳正交变换。它具有以下性质和特点：
   （1）由于K-L变换是正交线性变换，所以变换前后的方差总和不变，变换只是把原来的方差不等量的再分配到新的主成分图像中。
   （2）第一主成分包含了总方差的绝大部分（一般在80%以上），其余各主成分的方差依次减小。
   （3）可以证明，变换后各主成分之间的相关系数为零，也就是说各主成分间的内容是不同的，是“垂直”的。
   （4）第一主成分相当于原来各波段的加权和，而且每个波段的加权值与该波段的方差大小成正比（方差大说明信息量大）。其余各主成分相当于不同波段组合的加权差值图像。
   （5）K-L变换的第一主成分还降低了噪声，有利于细部特征的增强和分析，适用于进行高通滤波，线性特征增强和提取以及密度分割等处理。
   （6）K-L变换是一种数据压缩和去相关技术，第一成分虽信息量大，但有时对于特定的专题信息，第五、第六主成分也有重要的意义。
   （7）可以在图像中局部地区或者选取训练区的统计特征基础上作整个图像的K-L变换，则所选部分图像的地物类型就会更突出。
   （8）可以将所有波段分组进行K-L变换，再选主成分进行假彩色合成或其它处理。
   （9）K-L变换在几何意义上相当于进行空间坐标的旋转，第一主成分取波谱空间中数据散布最大的方向；第二主成分则取与第一主成分正交且数据散布次大的方向，其余依此类推。   

3、**定向变换**

　  定向变换是利用图像之间的相关性，在图像特征之间进行转轴变换，以便分离和消除干扰信息达到提取专题信息的目的。

4、**典型成分变换**

　  在遥感图像分类过程中，为了在减少各类的类内亮度（灰度）方差的同时，增大类间亮度方差以提高分类精度，需要选择恰当的投明方向（即变换），使同类的像元应尽可能集中，不同类的像元尽可能分开，这样处理后的分类，有利于分类精度的提高，这就是典型成分的基本思想。

5、**缨帽变换**

　   缨帽变换是R J Kauth和G S Thomas通过分析陆地卫星MSS图像反映农作物和植被生长过程的数据结构后提出的一种经验性的多波段图像的正交线性变换，又称K-T变换。

6、**其它离散图像变换**

　    图像变换除了应用广泛的傅立叶变换之外，还存在其它的图像变换，如本节要介绍的沃尔什变换、哈达玛变换等。每一种变换都有自己的特殊用途。随着数字图像处理应用技术的不断发展，有可能出现更多种类的图像变换。 离散图象变换的一般表达式
离散沃尔什变换
离散哈达玛变换
离散余弦变换
[](http://course.cug.edu.cn/rs/COURSE/5-5-a.htm#)

**6.1 离散图像变换的一般表达式**

![](http://course.cug.edu.cn/rs/COURSE/images/bluedrop.gif) 离散图像变换的代数表达式 

对二维离散函数![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image302.gif) 变换对的代数表达式，一般可写为

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image304.gif)![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image306.gif) （4-27） 

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image308.gif)![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image310.gif)（4-28） 

其中称![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image312.gif) 为正变换核，称![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image314.gif) 为反变换核。 如果![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image312.gif) = ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image316.gif)则称变换核 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image312.gif)是可分离的。 

    同样，如果![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image318.gif) 则称反变换核 是可分离的。变换核可分离的离散图像变换可表示为 

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image321.gif)![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image306.gif)（4-29）

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image323.gif)![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image310.gif)（4-30） 

如前面介绍的离散傅立叶变换，其正反变换核都是可分离的。由前面可知，二维离散傅立叶变换可以用两次一维变换来实现。这个性质对变换核（或反变换核）可分离的变换（或反变换）都适用。 

![](http://course.cug.edu.cn/rs/COURSE/images/bluedrop.gif) 离散图像变换的矩阵表达式 

    在二维离散傅立叶变换表示式中，如果令

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image325.gif)

则二维离散傅立叶变换用矩阵式表示，为 

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image327.gif)![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image329.gif)![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image331.gif)![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image333.gif)（4-31）

 或简单地表示为 

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image335.gif)（4-32） 

上式两边左乘矩阵![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image337.gif) 并右乘矩阵![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image339.gif) ，则有 

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image341.gif)（4-33） 

这就是离散傅立叶反变换的矩阵表示形式。通过（4-33）式可以看出

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image343.gif)

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image345.gif)

对比矩阵![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image347.gif) 与 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image337.gif)，![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image349.gif) 与 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image339.gif)可以看出

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image351.gif) ，![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image353.gif)

 所以矩阵![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image347.gif) 和![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image349.gif) 是酉矩阵，而对应的变换即离散傅立叶变换是一种酉变换。

    一般的，如果一个变换（及反变换）的变换核是可分离的，都可写成如下矩阵形式。 

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image355.gif)

对 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image357.gif)，矩阵A为M×M方阵，矩阵B为N×N方阵。如果一个变换的变换核不可分离，则此变换的矩阵表示形式为 

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image359.gif)

其中：A和 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image361.gif)均为M×M方阵。变换核不可分离的变换，则无法通过两次一维变换来实现二维变换。 

**6.2 离散沃尔什变换**

    前面介绍的离散傅立叶变换，其变换核由正弦和余弦函数所组成。本节要介绍的离散沃尔什变换，其变换核由+1和－1所组成。因此在变换过程中只有加法和减法运算，因而计算比较简单，易于硬件实现。

![](http://course.cug.edu.cn/rs/COURSE/images/bluedrop.gif) 一维离散沃尔什变换 

    若N=2*n*，则离散 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image363.gif)的沃尔什变换对为

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image365.gif)（ ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image367.gif)） （4-34）

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image369.gif)（![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image367.gif) ） （4-35） 

式中变换核为 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image371.gif)

反变换核为h(x,u) = g(x,u) 

这里*bi*(z)为z的二进制数的第i+1位的值（即0或1）。如N=8时的变换核和反变换作用矩阵形式表示为： 

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image373.gif)

![](http://course.cug.edu.cn/rs/COURSE/images/bluedrop.gif) 二维离散沃尔什变换 

    二维离散沃尔什变换的正反变换核相同，均为

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image375.gif)（4-36）

则二维离散沃尔什变换对为 

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image377.gif)（![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image379.gif) ）

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image381.gif)（ ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image379.gif)） 

**6.3 离散哈达玛变换**

![](http://course.cug.edu.cn/rs/COURSE/images/bluedrop.gif) 一维离散哈达玛变换 

    当N=2n，一维哈达玛正变换核与反变换核相同，为

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image383.gif)        （4-37） 

因此，一维哈达玛变换对可表示为 

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image385.gif)（u=0,1,2,...,N-1 ）

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image387.gif) （ ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image389.gif)） 

哈达玛变换核除了 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image391.gif)因子之外，由一系列的+1和－1组成。如N=8时的哈达玛变换核用矩阵表示为： 

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image393.gif)

由此矩阵可得出一个非常有用的结论，即 2N 阶的哈达玛变换矩阵可由N阶的变换矩阵按下述规律形成 

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image397.gif)

而最低阶（N=2）的哈达玛变换矩阵为 

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image399.gif)

利用这个性质求N阶（N=2n）的哈达玛变换矩阵要比直接用（4-37）式来求此矩阵速度快的多，此结论提供了一种快速哈达玛变换（FHT）。 

    除了上面介绍的哈达玛变换之外，常和的中外一种哈达玛变换称为定序的哈达玛变换。定序的哈达玛变换是由前面介绍的哈达玛变换演变而得到。 

    在哈达玛变换矩阵中，通常把沿某列符号改变的次数称为这个列的列率。则前面给出的N=8时的变换矩阵的8个列的列率分别为0,7,3,4,1,6,2,5。而下面要介绍的定序哈达玛变换的变换矩阵的列率是随u的增加而递增。如N=8时定序哈达玛变换矩阵的列率从第1列到第8列分别为0,1,2,3,4,5,6,7。 

    N=2n时定序哈达玛正变换核和反变换核相同，为 

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image383.gif)

其中pi(u)存在以下递推关系

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image401.gif)

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image403.gif)

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image405.gif)

 ……………… 

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image407.gif)

如N=8时的定序哈达玛变换核矩阵为 

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image409.gif)

定序哈达玛变换对显然为 

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image385.gif) （u=0,1,2,...,N-1 ）

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image387.gif) （ ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image389.gif)） 

![](http://course.cug.edu.cn/rs/COURSE/images/bluedrop.gif) 二维离散哈达玛变换 

    二维离散哈达玛变换的正变换核和反变换核相同，为

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image411.gif)

 这里M=2m，N=2n。则对应的二维哈达变换对可表示为： 

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image413.gif)  （ u=0,1,2,...,M-1;v=0,1,2,...,N-1） 

和 ![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image415.gif)（x=0,1,2,...,M-1;v=0,1,2,...,N-1 ） 

可以看出，二维离散哈达玛变换的正反变换核具有可分离性，因此可以通过两次一维变换来实现一个二维变换。 

**6.4 离散余弦变换**

![](http://course.cug.edu.cn/rs/COURSE/images/bluedrop.gif) 一维离散余弦变换 

一维离散余弦变换（简称）DCT的正变换核由下述关系式给出，即

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image419.gif)

 对应的离散余弦变换可表示为 

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image421.gif)

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image423.gif)

式中C(u) (u=0,1,2,…, N-1)为f(x) (x =0,1,2,…, N -1)的离散余弦变换（DCT）。 

    若取反变换核与正变换核相同，则可定义离散余弦反变换为 

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image425.gif)

![](http://course.cug.edu.cn/rs/COURSE/images/bluedrop.gif) 二维离散余弦变换 

二维离散余弦变换的正反变换相同，即

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image427.gif)  ( u=0,1,2,...,M-1;v=0,1,2,...,N-1) 

则对应离散余弦变换对为：

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image429.gif)

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image431.gif)![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image433.gif)

![](http://course.cug.edu.cn/rs/COURSE/pic/4pic/image435.gif)(x=0,1,2,...,M-1;y=0,1,2,...,N-1)

不难看出，二维离散余弦变换的变换核是可分离的，因而可通过两次一维变换实现一个二维变换。 




# dp进阶之FFT加速+数据结构优化+不等式优化 - 紫芝的博客 - CSDN博客





2018年08月13日 19:34:30[紫芝](https://me.csdn.net/qq_40507857)阅读数：269








# 快速傅里叶变换

**快速傅里叶变换**（英语：**Fast Fourier Transform, FFT**），是快速计算序列的[离散傅里叶变换](https://zh.wikipedia.org/wiki/%E7%A6%BB%E6%95%A3%E5%82%85%E9%87%8C%E5%8F%B6%E5%8F%98%E6%8D%A2)（DFT）或其逆变换的方法。[傅里叶分析](https://zh.wikipedia.org/wiki/%E5%82%85%E9%87%8C%E5%8F%B6%E5%88%86%E6%9E%90)将信号从原始域（通常是时间或空间）转换到[频域](https://zh.wikipedia.org/wiki/%E9%A0%BB%E5%9F%9F)的表示或者逆过来转换。FFT会通过把[DFT矩阵](https://zh.wikipedia.org/wiki/%E9%9B%A2%E6%95%A3%E5%82%85%E9%87%8C%E8%91%89%E8%AE%8A%E6%8F%9B%E7%9F%A9%E9%99%A3)[分解](https://zh.wikipedia.org/wiki/%E7%9F%A9%E9%98%B5%E5%88%86%E8%A7%A3)为[稀疏](https://zh.wikipedia.org/wiki/%E7%A8%80%E7%96%8F%E7%9F%A9%E9%98%B5)（大多为零）因子之积来快速计算此类变换。 因此，它能够将计算DFT的[复杂度](https://zh.wikipedia.org/wiki/%E8%A8%88%E7%AE%97%E8%A4%87%E9%9B%9C%E6%80%A7%E7%90%86%E8%AB%96)从只用DFT定义计算需要的  ![O(n^{2})](https://wikimedia.org/api/rest_v1/media/math/render/svg/6cd9594a16cb898b8f2a2dff9227a385ec183392)，降低到  ![O(n\log n)](https://wikimedia.org/api/rest_v1/media/math/render/svg/9d2320768fb54880ca4356e61f60eb02a3f9d9f1)，其中  ![n](https://wikimedia.org/api/rest_v1/media/math/render/svg/a601995d55609f2d9f5e233e36fbe9ea26011b3b) 为数据大小。

快速傅里叶变换广泛的应用于工程、科学和数学领域。这里的基本思想在1965年才得到普及，但早在1805年就已推导出来。 1994年美国数学家[吉尔伯特·斯特朗](https://zh.wikipedia.org/w/index.php?title=%E5%90%89%E5%B0%94%E4%BC%AF%E7%89%B9%C2%B7%E6%96%AF%E7%89%B9%E6%9C%97&action=edit&redlink=1)把FFT描述为“我们一生中最重要的[数值算法](https://zh.wikipedia.org/wiki/%E6%95%B0%E5%80%BC%E5%88%86%E6%9E%90)”，它还被IEEE科学与工程计算期刊列入20世纪十大算法

**库利－图基快速傅里叶变换算法**（**Cooley-Tukey算法**）是最常见的[快速傅里叶变换](https://zh.wikipedia.org/wiki/%E5%BF%AB%E9%80%9F%E5%82%85%E9%87%8C%E5%8F%B6%E5%8F%98%E6%8D%A2)算法。这一方法以[分治法](https://zh.wikipedia.org/wiki/%E5%88%86%E6%B2%BB%E6%B3%95)为策略[递归](https://zh.wikipedia.org/wiki/%E9%80%92%E5%BD%92)地将长度为*N* = *N*1*N*2的DFT分解为长度分别为*N*1和*N*2的两个较短序列的DFT，以及与旋转因子的复数乘法。这种方法以及FFT的基本思路在1965年J. W. Cooley和J. W. Tukey合作发表An algorithm for the machine calculation of complex Fourier series之后开始为人所知。但后来发现，实际上这两位作者只是重新发明了高斯在1805年就已经提出的算法（此算法在历史上数次以各种形式被再次提出）。

库利－图基算法最有名的应用，是将序列长为N的DFT分区为两个长为N/2的子序列的DFT，因此这一应用只适用于序列长度为2的幂的DFT计算，即基2-FFT。实际上，如同高斯和库利与图基都指出的那样，库利－图基算法也可以用于序列长度N为任意因数分解形式的DFT，即混合基FFT，而且还可以应用于其他诸如分裂基FFT等变种。尽管库利－图基算法的基本思路是采用递归的方法进行计算，大多数传统的算法实现都将显示的递归算法改写为非递归的形式。另外，因为库利－图基算法是将DFT分解为较小长度的多个DFT，因此它可以同任一种其他的DFT算法联合使用。

**快速傅里叶变换**是一个很有工程价值的算法，广泛地应用于音频、图像等数字信号处理软件中。傅里叶变换本身的理论很深，这里仅以快速多项式乘法为例介绍它在算法竞赛中的应用。快速傅里叶变换（FFT）是用来计算离散傅里叶变换（DFT)及其逆变换（IDFT）的快速算法。**DFT把时域信号变换为频域信号。时域和频域只是两种信号分析的方法，并不是指两种不同类别的信号。**DFT有一个很有意思的性质：时域卷积，频域乘积；频域卷积，时域乘积；如果你不知道什么是卷积（convolution),也没关系，请记住：多项式乘法实际上是多项式系数向量的卷积。

多项式乘法。给定两个单变量多项式A(x),B(x),次数均不超过n，如何快速计算二者的乘积呢？最简单的方法就是把系数两两相乘，再相加。这样计算的时间复杂度是 O（n^2),当n很大时，速度会很慢。注意，高精度整数的乘法是多项式乘法的特殊情况，所以**多项式乘法的快速乘法也可以用来做高精度乘法**。

上述算法之所以慢是因为表示方法不好。虽然”系数序列“是最自然的表示方法，但却不适合用来计算乘法。在多项式快速乘法中，需要用点值法来表示一个多项式，点值表示一个”点—值“对的序列{(x0,y0),(x1,y1).......(x(n-1),y(n-1))}，它代表一个多项式，点值法非常适合做乘法：只要两个多项式的点集{ xi }相同，则只需要把对应的值乘起来就可以了，只需要O(n)时间。但问题在于输入输出的时候仍需要采用传统的系数表示法，因此需要快速地在系数表示和点值表示之间转换。还记得上面那句话吗？时域卷积，频域乘积；也就是说，**多项式的系数表示法对应于时域，而点值表示法对应于频域。**因此，只需要用FFT计算出一个DFT，就可以完成转换。

**单位根。**这样算出来的点值表示法，对应的求值点是那些？答案是2n次单位根。所谓”n次单位根“，是指满足x^n=1的复数。在复数域，1恰好有n个单位根e^(2k*PI*i / n),其中i是虚数单位，e^(iu)=cos u+i*sin u.单位根非常特殊，因此FFT才可以在更短时间内求出多项式在这些点的取值。

利用FFT进行快速多项式乘法的步骤：

**1.补0：**在两个多项式的最前面补0，得到两个2n次多项式，设系数向量分别是V1和V2

**2.求值：**用FFT计算 f1=DFT(v1)和 f2=DFT(v2).这里的f1和f2分别是两个输入多项式在2n次单位根处的各个取值（即点值表示）

**3.乘法：**把两个向量f1和f2的每一维对应相乘，得到向量f。它对应多项式乘积的点值表示

**4.插值：**用FFT计算v=IDFT(f),其中n就是乘积的系数向量

**多项式求值算法**

给定多项式：A(x)=a0+a1*x+a2*x^2+a3*x^3+......+a(n-1)*x^(n-1)

设x为1的2n次方根，对所有的x计算A(x)的值

算法1：对每个x做下述运算，依次计算每个项 ai*x^i,(i=0,1,2,3....n-1),然后对 ai*x^i(i=0,1,2,3....n-1)求和

每一项都需要重新计算，时间复杂度是O(n^3)

算法2：依次对x做下述运算，减少不必要的重新计算，A1(x)=a(n-1),

# 数据结构优化

**有时尽管状态找好了，转移方程的想好了，但时间复杂度比较大，需要用数据结构进行优化。常见的优化有二进制优化、单调队列优化、斜率优化、四边形不等式优化等。**

**1、二进制优化**

主要是优化背包问题，背包九讲里面有介绍，比较简单，这里只附上几道题目。

[hdu 1059 Diving](http://acm.hdu.edu.cn/showproblem.php?pid=1059)

[hdu 1171 Big Event in Hdu](http://acm.hdu.edu.cn/showproblem.php?pid=1059)

[poj 1048 Follow My Magic](http://poj.org/problem?id=1048)

**2、单调队列优化**

推荐论文：[http://wenku.baidu.com/view/4d23b4d128ea81c758f578ae.html](http://wenku.baidu.com/view/4d23b4d128ea81c758f578ae.html)

推荐博客：[http://www.cnblogs.com/neverforget/archive/2011/10/13/ll.html](http://www.cnblogs.com/neverforget/archive/2011/10/13/ll.html)

[hdu 3401 Trade](http://blog.csdn.net/cc_again/article/details/9328243)

[poj 3245 Sequece Partitioning](http://blog.csdn.net/cc_again/article/details/9335795) 二分+单调队列优化

**3、斜率优化**

推荐论文：[用单调性优化动态规划](http://wenku.baidu.com/view/ef259400bed5b9f3f90f1c3a.html)

推荐博客：[http://www.cnblogs.com/ronaflx/archive/2011/02/05/1949278.html](http://www.cnblogs.com/ronaflx/archive/2011/02/05/1949278.html)

[hdu 3507 Print Article](http://acm.hdu.edu.cn/showproblem.php?pid=3507)

[poj 1260 Pearls](http://poj.org/problem?id=1260)

[hdu 2829 Lawrence](http://acm.hdu.edu.cn/showproblem.php?pid=2829)

[hdu 2993 Max Average Problem](http://acm.hdu.edu.cn/showproblem.php?pid=2993)

**4、四边形不等式优化**

推荐博客：[http://www.cnblogs.com/ronaflx/archive/2011/03/30/1999764.html](http://www.cnblogs.com/ronaflx/archive/2011/03/30/1999764.html)

推荐博客：[http://www.cnblogs.com/zxndgv/archive/2011/08/02/2125242.html](http://www.cnblogs.com/zxndgv/archive/2011/08/02/2125242.html)

[hdu 2952 Counting Sheep](http://acm.hdu.edu.cn/showproblem.php?pid=2952)

[poj 1160 Post Office](http://poj.org/problem?id=1160)

[hdu 3480 Division](http://acm.hdu.edu.cn/showproblem.php?pid=3480)

[hdu 3516 Tree Construction](http://acm.hdu.edu.cn/showproblem.php?pid=3516)

[hdu 2829 Lawrence](http://acm.hdu.edu.cn/showproblem.php?pid=2829)

# 四边形不等式优化

**今天第一次学习四边形不等式优化dp，感觉优化效果十分给力，不过数学味道比较浓重，证明比较复杂。因此这里删繁就简，给出关于四边形不等式优化必须要明白的地方，以后直接套用条件即可。**

### 四边形不等式优化条件

**1.在动态规划中，经常遇到如下式的状态转移方程：**

**m(i,j) = min{ m(i,k-1),m(k,j) } + w(i,j)  (i<=k<=j)** (min也可以改为max）

m(i,j) 表示在【i，j】区间上的某个最优值，w(i,j)表示转移时需要付出的代价，该方程的时间复杂度为O(n^3).

**2.下面我们用四边形不等式来优化上述方程：**

**（1）区间包含的单调性**

如果对于 **i <= i' < j <= j'**，有**w(i' , j) <= w(i , j') **,那么说明 W 具有区间包含的单调性。

（形象的理解为如果小区间包含于大区间，那么小区间的w值不超过大区间的w值）

**（2）四边形不等式**

如果对于 **i <= i' < j <= j' **,有 **w( i , j ) + w( i' , j' ) <= w ( i , j' )+w( i' , j' ) **,我们称函数w满足四边形不等式

（形象的理解为两个交错区间的和不超过小区间与大区间的和）

**两个定理：**

**定理一：如果W函数同时满足区间包含的单调性和四边形不等式，那么函数m也满足四边形不等式**

我们再定义S(i,j)表示m(i,j)取得最优值时对应的下标，（即 i<= k <= j 时，k处的w值最大，则s(i,j)=k )

此时有如下定理：

**定理二：假如 m(i,j)满足四边形不等式，那么s(i,j)单调，即 s(i,j) <= s(i,j+1) <= s(i+1,j+1)**

有了上述两个定理后，如果w函数满足区间包含单调性和四边形不等式性质，那么有**s(i,j-1) <= s(i,j) <= s(i+1,j)**

原来的状态转移方程可以改写为下式：

**m(i,j)=min{m(i,k-1),m(k,j)}+w(i,j)   (min也可以改为max）**

由于这个方程枚举的是区间长度L=j-i，而s(i,j-1)和s(i-1,j)的区间长度为L-1，是之前已经计算过的，可以直接调用。不仅如此，区间的长度最多有n个，对于固定的长度L，不同的状态也有n个，故时间复杂度为O(n^2),而原来的复杂度为O(n^3),实现了四边形不等式的优化。今后只需要根据方程的形式，以及w函数是否满足上述两条性质，即可考虑使用四边形不等式来优化。






# 主成分分析（PCA）与Kernel PCA - 白马负金羁 - CSDN博客





2018年03月01日 03:21:13[白马负金羁](https://me.csdn.net/baimafujinji)阅读数：23336
所属专栏：[图像处理中的数学原理详解](https://blog.csdn.net/column/details/math-imageprocess.html)









本博客在之前的文章【1】中曾经介绍过PCA在图像压缩中的应用。其基本思想就是设法提取数据的主成分（或者说是主要信息），然后摒弃冗余信息（或次要信息），从而达到压缩的目的。本文将从更深的层次上讨论PCA的原理，以及Kernel化的PCA。




首先我们来考察一下，这里的信息冗余是如何体现的。如下图中的左图所示，我们有一组二维数据点，从图上不难发现这组数据的两个维度之间具有很高的相关性。因为这种相关性，我们就可以认为其实有一个维度是冗余的，因为当已知其中一个维度时，便可以据此大致推断出另外一个维度的情况。


![](https://img-blog.csdn.net/20180226135605968)![](https://img-blog.csdn.net/20180226135623589)


为了剔除信息冗余，我们设想把这些数据转换到另外一个坐标系下（或者说是把原坐标系进行旋转），例如像上图中的右图所示之情况，当然这里通过平移设法把原数据的均值变成了零。下图中的左图是经过坐标系旋转之后的数据点分布情况。你可以看出，原数据点的两个维度之间的相关性已经被大大削弱（就这个例子而言几乎已经被彻底抹消）。

![](https://img-blog.csdn.net/20180226135631446)![](https://img-blog.csdn.net/20180226135639419)


同时你也会发现在新坐标系中，横轴这个维度x相比于纵轴那个维度y所表现出来的重要性更高，因为从横轴这个维度上更大程度地反应出了数据分布的特点。也就是说，本来需要用两个维度来描述的数据，现在也能够在很大程度地保留数据分布特点的情况下通过一个维度来表达。如果我们仅保留x这个维度，而舍弃y那个维度，其实就起到了数据压缩的效果。而且，舍弃y这个维度之后，我们再把数据集恢复到原坐标系上，你会发现关于数据分布情况的信息确实在很大程度上得以保留了（如上图中的右图所示）。




上面所描述的也就是PCA要达到的目的。但是，如何用数学的语言来描述这个目的呢？或者说，我们要找到一个变换使得坐标系旋转的效果能够实现削弱相关性或将主要信息集中在少数几个维度上这一任务，应该如何确定所需之变换（或者坐标系旋转的角度）呢？我们还是来看一个例子，假设现在有如下图所示的一些数据，它们形成了一个椭圆形状的点阵，那么这个椭圆有一个长轴和一个短轴。在短轴方向上，数据变化很少；相反，长轴的方向数据分散得更开，对数据点分布情况的解释力也就更强。

![](https://img-blog.csdn.net/20180226152431750)


那么数学上如何定义数据“分散得更开”这一概念呢？没错，这里就需要用到方差这个概念。如下图所示，现在有5个点，假设有两个坐标轴w和v，它们的原点都位于O。然后，我们分别把这5个点向w和v做投影，投影后的点再计算相对于原点的方差，可知在v轴上的方差要大于w轴上的方差，所以如果把原坐标轴旋转到v轴的方向上，相比于旋转到w轴的方向上，数据点会分散得更开！


![](https://img-blog.csdn.net/20180226152110512)


或者来看动图：

![](https://img-blog.csdn.net/20180228145615041)


Suppose ![](https://img-blog.csdn.net/20180226154236634) are training samples with zero mean. The goal of PCA is to find a set of ![](https://img-blog.csdn.net/20180226154313553) vectors in space ![](https://img-blog.csdn.net/20180226154351389)containing the max amount of variance in the data.





上图中的一点 xj 向 v 轴做投影，所得之投影向量为

![](https://img-blog.csdn.net/20180226160051580)


其中θ是向量Oxj与v的夹角。如果这里的向量v是单位向量，则有


![](https://img-blog.csdn.net/20180226160117281)


这同时表明其系数其实就是内积


![](https://img-blog.csdn.net/20180226160124501)


The projections of the all pixels xjonto this normalized direction v are

![](https://img-blog.csdn.net/20180226161803097)


The variance of the projections is 


![](https://img-blog.csdn.net/20180226161831526)


其中，C是协方差矩阵，关于协方差矩阵（Covariance matrix）可以参考【3】。





因为vTCv 就是 variance，而我们的目标是最大化variance，因此 the first principal vector can be found by the following equation:

![](https://img-blog.csdn.net/20180228144911139)


鉴于是带等式约束的优化问题，遂采用拉格朗日乘数法，写出拉格朗日乘数式如下：


![](https://img-blog.csdn.net/20180228145040923)


然后将上式对*v*和*λ*求导，并令导数等于0，则有


![](https://img-blog.csdn.net/20180228145053089)


于是我们知道，原来的argmax式子就等价于find the largest eigenvalue of the following eigenvalue problem:


![](https://img-blog.csdn.net/20180228145845342)


注意我们前面的argmax式子要算的是使得vTCv达到最大的v，而v可以由上式解出，据此再来计算vTCv，则有


![](https://img-blog.csdn.net/20180228145409064)


也就是说我们要做的就是求解Cv=*λ*v，从而得到一个最大的*λ*，而这个*λ*对应特征向量v所指示的就是使得variance最大的方向。Projections of the data on the principal axes are called 主成分, also known as PC scores。注意因为v是单位向量，所以点 xj 向 v 轴做投影所得之 PC score 就是 vT·xj。而且这也是最大的主成分方向。如果要再多加的一个方向的话，则继续求一个次大的*λ*，而这个*λ*对应特征向量v所指示的就是使得variance第二大的方向，并以此类推。




更进一步，因为C是Covariance matrix，所以它是对称的，对于一个对称矩阵而言，如果它有N个不同的特征值，那么这些特征值对应的特征向量就会彼此正交。如果把Cv=*λ*v，中的向量写成矩阵的形式，也就是采用矩阵对角化（特征值分解）的形式，则有C=VΛVT，where V is a matrix of eigenvectors (each column is an eigenvector) andΛ is a diagonal matrix with eigenvalues *λ***iin the decreasing order on the diagonal. The eigenvectors are called principal axes or principal directions of the data. 





注意到Covariance matrix（这里使用了前面给定的零均值假设）


![](https://img-blog.csdn.net/20180228153947743)


如果令![](https://img-blog.csdn.net/20180228154009530)，其中xi 表示一个列向量，则有


![](https://img-blog.csdn.net/20180228154211673)


Projections of the data on the principal axes are called principal components, also known as PC scores; these can be seen as new, transformed, variables. The j-th principal component is given by j-th column of XV. The coordinates of the i-th data point in the new PC space are given by the i-th row of XV。





**Kernel PCA**





下面，我们来推导引入Kernel版的PCA，关于Kernel的一些内容请参考文献【5】。其实，Kernel版的PCA思想是比较简单的，我们同样需要求出Covariance matrix C，但不同的是这一次我们要再目标空间中来求，而非原空间。


![](https://img-blog.csdn.net/20180228155231767)


如果令![](https://img-blog.csdn.net/20180228155240606)，则Φ(**i)表示**i被映射到目标空间后的一个列向量，于是同样有


![](https://img-blog.csdn.net/20180228154211673)


C和XTX具有相同的特征向量。但现在的问题是Φ是隐式的，我们并不知道。所以，我们需要设法借助核函数K来求解XTX。




因为核函数K是已知的，所以如下所示XXT是可以算得的。


![](https://img-blog.csdn.net/20180228161931270)


注意到XTX并不等于XXT，但二者之间肯定存在某种关系。所以我们设想是否可以用K来计算XTX。





The eigenvalue problem of K=XXT is (XXT)u = u。现在我们需要的是XTX，所以把上述式子的左右两边同时乘以一个XT，从而构造出我们想要的，于是有

XT(XXT)u = XTu


即

(XTX)(XTu)=(XTu)


这就意味着XTu就是XTX的特征向量！尽管，此处特征向量的norm并不一定为1。为了保证eigenvector 的 norm为1，我们用eigenvector除以其自身的长度。注意K=XXT，而Ku=u，即


![](https://img-blog.csdn.net/20180228163532614)


上式中![](https://img-blog.csdn.net/20180228155240606)仍然是未知的，所以v仍然未知。但是也不要紧，因为我们可以直接设法求投影，因为我们的最终目的仍然是计算所以点 Φ(xj) 向 v 轴做投影所得之 PC score 就是 vT·Φ(xj)。如下


![](https://img-blog.csdn.net/20180301030953949)


综上，便得到了核化版的PCA的计算方法。我们再做扼要总结如下：

Solve the following eigenvalue problem:

![](https://img-blog.csdn.net/20180301031000976)


The projection of the test sample Φ(xj) on the i-th eigenvector can be computed by


![](https://img-blog.csdn.net/20180301031747247)


所得之vTi·Φ(xj)即为特征空间（Feature space）中沿着vi方向的坐标。




最后我们给出的是一个KPCA的例子。其中左图是用传统PCA画出的投影。右图是在高维空间中找到投影点后又转换回原空间的效果。可见，加了核函数之后的PCA变得更加强大了。


![](https://img-blog.csdn.net/20180228164538889)














参考文献与推荐阅读材料

【1】http://blog.csdn.net/baimafujinji/article/details/50373143


【2】部分图片来自李政軒博士的在线授课视频

【3】http://blog.csdn.net/baimafujinji/article/details/50372906

【4】https://stats.stackexchange.com/questions/134282/relationship-between-svd-and-pca-how-to-use-svd-to-perform-pca

【5】http://blog.csdn.net/baimafujinji/article/details/79372911





# 线性降维之LDA与PCA - 刘炫320的博客 - CSDN博客
2017年07月20日 22:26:27[刘炫320](https://me.csdn.net/qq_35082030)阅读数：1093标签：[机器学习																[LDA																[PCA																[线性降维																[主成分分析法](https://so.csdn.net/so/search/s.do?q=主成分分析法&t=blog)](https://so.csdn.net/so/search/s.do?q=线性降维&t=blog)](https://so.csdn.net/so/search/s.do?q=PCA&t=blog)](https://so.csdn.net/so/search/s.do?q=LDA&t=blog)](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)
个人分类：[特征工程](https://blog.csdn.net/qq_35082030/article/category/7042982)
# 1.前言
如果我只说LDA，那么对于自然语言处理的人来讲，可能更熟悉的是LDA主题模型。但是一旦我跟上PCA了，那么这两个兄弟，我相信从事机器学习相关的同学就会知道，这是在讲——线性降维。
# 2. 摘要
我们这里首先给出LDA和PCA之间的区别与联系，然后我们再详细讲解两个线性降维方法。因为实际上，scikit-learning机器学习工具包中就已经包含了[LDA](http://scikit-learn.org/stable/modules/generated/sklearn.discriminant_analysis.LinearDiscriminantAnalysis.html#sklearn.discriminant_analysis.LinearDiscriminantAnalysis)和[PCA](http://scikit-learn.org/stable/modules/generated/sklearn.decomposition.PCA.html)降维的实现，因此我们如果在应用时，可以直接拿来用。细节部分我们稍后介绍。
LDA和PCA都是线性降维处理的常用方法，但是他们的目标基本上是相反的，下表给出了LDA与PCA之间的区别。
|类型|LDA|PCA|
|----|----|----|
|学习用处|可独立作为线性分类器|只能当做预处理|
|学习方式|有监督学习|无监督学习|
|学习目标|保证样本在空间中有最佳的可分离性|保证样本在空间中保持原来变量信息|
|学习过程|保证类别内距离越近越好，类别间距离越远越好|保留样本拥有最大方差|
下图给出了对于同一个样例，两种降维方法得到的不同的结果，很直观的，对吧。 
![这里写图片描述](https://img-blog.csdn.net/20170720202016724?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzUwODIwMzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 3. LDA
LDA(Linear Discriminant Analysis)称为是线性判别分析，是一种常用的线性降维方法，而线性降维的原理就是通过一个函数H，把原空间X映射到空间Y，使得|Y|<|X|即达到降维目的，但是降维的好坏要看映射后空间Y对于原数据的表现能力是否优于原空间X。
LDA是线性分类器，因此它的模样其实长得很大众化： 
$y_k(x)=w^T_kx+w_{k0}$
这个东西在神经网络中实在是再熟悉不过了。但事实上，每一个线性分类器只能二元分类（包括SVM），因此想K类分类的话，需要有K个线性函数。
回顾我们上面讲的，LDA的目标是保证映射后的样本类别内距离越近越好，类别间距离越远越好。可是这个目标比较宽泛，我们如何根据他来确定最终的线性分类器呢。
首先需要定义这么几个值; 
类别i的原始中心点（均值）为： 
$m_i=\frac{1}{n_i}\sum_{x\in D_i}x $
类别i投影后的中心点为： 
$\tilde{m_i}=w^Tm_i$
衡量类别i投影后，类别点之间的分散程度（方差）为： 
$\tilde{s_i}=\sum_{y \in \gamma_i }(y-\tilde{m_i}^2)$
最终一个二分类问题就可以得到下面的目标优化函数： 
$J(w)=\frac{|\tilde{m_1}-\tilde{m_2}|^2}{\tilde{s_1}^2-\tilde{s_2}^2}$
我们定义一个投影前的各类别分散程度的矩阵，其意思是，如果某一个分类的输入点集$D_i$里面的点距离这个分类的中心点$m_i$越近，则$s_i$里面的元素的值越小。带入$s_i$,将J（w）的分母化为： 
$S_i=\sum_{x \in D_i}(x-m_i)(x-m_i)^T$
$\tilde{s_i}=\sum_{x \in D_i}(w^Tx-w^Tm_i)^2=sum_{x \in D_i}(x-m_i)(x-m_i)^Tw=w^TS_iw$
这样，二分类的分母就可以是： 
$\tilde{s_1}^2+\tilde{s_2}^2=w^T(S_1+S_2)w=w^TS_ww$
同样，分子就变成了： 
$|\tilde{m_1}-\tilde{m_2}|^2=w^T(m_1-m_2)(m_1-m_2)^Tw=w^TS_Bw$
因此目标函数就变成了： 
$J(w)=\frac{w^TS_Bw}{w^TS_ww}$
这样就可以使用拉格朗日乘子法了，不过为了限制无穷节，这里使用一个小技巧，那就是将分母限制长度为1，并作为拉格朗日乘子法的限制条件，带入就可以得到： 
$c(w)=w^TS_Bw-\lambda(w^TS_ww-1)$
$\frac{dc}{dw}=2S_Bw-2\lambda S_ww=0$
$S_Bw=\lambda S_ww$
这样式子就是一个求广义特征值的问题了，如果$S_w$可逆，那么求到后的结果两边同乘以$S_w^{-1}$得到： 
$S_w^{-1}S_Bw=\lambda w$
w为矩阵的特征向量，这个公式就被称为Fisher linear discrimination。
让我们观察一下： 
$(m_1-m_2)(m_1-m_2)^T=S_B$
那么： 
$S_Bw=(m_1-m_2)(m_1-m_2)^Tw=(m_1-m_2)\lambda^{'}=\lambda w$
由于对w扩大缩小任何倍不影响结果，因此可以约去两边的未知常数$\lambda和\lambda^{'}$,因此得到了： 
$w=S_w^{-1}(m_1-m_2)$
至此，我们只需要求出原始样本的均值和方差就可以求出最佳的方向w。 
对于N（N>2）分类问题，我们可以直接写出以下结论： 
$S_w=\sum_{i=1}^cS_i$
$S_B=\sum_{i=1}^c{n_i}(m_i-m)(m_i-m)^T$
$S_Bw_i=\lambda S_wW_i$
这同样是一个求广义特征值的问题，求出的第i大的特征向量，即为对应的$w_i$。
最后，计算映射向量w就是求最大特征向量，也可以是前几个最大特征向量组成矩阵$A=[w_1,w_2,….w_k]$之后，就可以对新来的点进行降维了：$y = A·X$（线性的一个好处就是计算方便！）
可以发现，LDA最后也是转化成为一个求矩阵特征向量的问题，和PCA很像，事实上很多其他的算法也是归结于这一类，一般称之为谱（spectral）方法。
更多的可以参考《[LDA 线性判别分析](http://blog.csdn.net/porly/article/details/8020696)》和《[LDA算法的中心思想](https://www.zhihu.com/question/34305879)》及《[LDA线性判别分析详解](http://www.cnblogs.com/engineerLF/p/5393119.html)》和《[线性判别分析LDA与主成分分析PCA](https://wenku.baidu.com/view/37055bd4a300a6c30d229f30.html)》。
# 4. PCA
PCA（Principal Component Analysis）被称为是主成分分析法。它的目标是通过某种线性投影，将高维的数据映射到低维的空间中表示，并期望在所投影的维度上数据的方差最大，以此使用较少的数据维度，同时保留住较多的原数据点的特性。
通俗的理解，如果把所有的点都映射到一起，那么几乎所有的信息（如点和点之间的距离关系）都丢失了，而如果映射后方差尽可能的大，那么数据点则会分散开来，以此来保留更多的信息。可以证明，PCA是丢失原始数据信息最少的一种线性降维方式。（实际上就是最接近原始数据，但是PCA并不试图去探索数据内在结构）
说实话，如果把原来的数据变为一个矩阵，那么PCA就是通过线性变换，成为一个线性无关的矩阵。但是这种方式有很多，该如何选取一个合适的变换呢？
如果还记得线性代数里的协方差，那么就好办多了。
如果将选取的第一个线性组合即第一个综合变量记为$F_1$，自然希望它尽可能多地反映原来变量的信息，这里“信息”用方差来测量，即希望$Var(F_1)$越大，表示$F_1$包含的信息越多。因此在所有的线性组合 
中所选取的$F_1$应该是方差最大的，故称$F_1$为第一主成分。如果第一主成分不足以代表原来p个变量的信息，再考虑选取$F_2$即第二个线性组合，为了有效地反映原来信息，$F_1$己有的信息就不需要再出现在$F_2$中，用数学语言表达就是要求$cov（F_1，F_2）＝0$。，称$F_1$为第二主成分，依此类推可以构造出第三、四…第p个主成分。
为什么要用方差大的作为主信息呢，而这一切，都要从最大方差理论说起：在信号处理中认为信号具有较大的方差，噪声有较小的方差，信噪比就是信号与噪声的方差比，越大越好。因此我们认为，最好的k维特征是将n维样本点转换为k维后，每一维上的样本方差都很大。
因此这个理论认为方差最大的才是有效信息。我们要求的是最佳的直线，是的投影后样本点方差最大。设投影后样本点为$\bar{x},\bar{x}=\frac{1}{N}\displaystyle\sum_{n=1}^Nx_n$,因此方差为： 
$\frac{1}{N}\displaystyle\sum_{n=1}^N{(u_1^Tx_n-u_1^T\bar{x})^2}=u_1^TSu_1$
其中： 
$S=\frac{1}{N}\sum_{i=1}^N(x_i-\bar{x})(x_i-\bar{x})^T$
S就是样本特征的协方差矩阵，令u为单位向量，即u^Tu=1,用拉格朗日乘子法可得： 
$f(u)=u^TSu+\lambda(1-u^Tu)$
将上式求导，使之为0，得到$Su=\lambda u$这是一个标准的特征值表达式了，$\lambda对应的特征值，u对应的特征向量。由此var=u^TSu=\lambda$。
var取得最大值的条件就是入最大，也就是取得最大的特征值的时候。假设我们是要将一个D维的数据空间投影到M维的数据空间中$（M<D)$，那我们取前M个特征向量构成的投影矩阵就是能够使得方差最大的矩阵了。同时，由于u是实对称矩阵的特征向量，因此特征向量之间正交，投影得到的综合变量彼此独立，协方差为0。
因此，我们只需要对协方差矩阵进行特征值分解，得到的前k大特征值对应的特征向量就是最佳的k维新特征，而且这k维新特征是正交的。
更多的细节可以参照《[PCAPPT](http://www.csd.uwo.ca/~olga/Courses//CS434a_541a//Lecture8.pdf)》和《[PCA的数学原理](http://blog.csdn.net/xiaojidan2011/article/details/11595869)》以及《[主元分析(PCA)理论分析及应用](http://blog.csdn.net/ayw_hehe/article/details/5736659)》。
# 5. 小结
今天我们主要介绍了2个线性降维的方法，而且有很多公式推导，比较烧脑。后面可以慢慢体会。

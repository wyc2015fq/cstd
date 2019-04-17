# LDA 线性判别分析/Fisher线性判别 - 李鑫o_O - CSDN博客





2016年05月10日 14:16:58[hustlx](https://me.csdn.net/HUSTLX)阅读数：1306









1. LDA是什么


线性判别式分析（Linear Discriminant Analysis），简称为LDA。也称为Fisher线性判别（Fisher Linear Discriminant，FLD），是模式识别的经典算法，在1996年由Belhumeur引入模式识别和[人工智能](http://lib.csdn.net/base/2)领域。

基本思想是将高维的模式样本投影到最佳鉴别矢量空间，以达到抽取分类信息和压缩特征空间维数的效果，投影后保证模式样本在新的子空间有**最大的类间距离**和**最小的类内距离**，即模式在该空间中有最佳的可分离性。

LDA与前面介绍过的PCA都是常用的降维技术。**PCA主要是从特征的协方差角度，去找到比较好的投影方式**。**LDA更多的是考虑了标注**，**即希望投影后不同类别之间数据点的距离更大，同一类别的数据点更紧凑**。


下面给出一个例子，说明LDA的目标。

![](http://pic002.cnblogs.com/images/2011/79762/2011102010103238.png)![](http://pic002.cnblogs.com/images/2011/79762/2011102010120697.png)


       可以看到两个类别，一个绿色类别，一个红色类别。左图是两个类别的原始数据，现在要求将数据从二维降维到一维。直接投影到x1轴或者x2轴，不同类别之间会有重复，导致分类效果下降。右图映射到的直线就是用LDA方法计算得到的，可以看到，红色类别和绿色类别在映射之后之间的距离是最大的，而且每个类别内部点的离散程度是最小的（或者说聚集程度是最大的）。


这里还有一个LDA的例子：

![](http://pic002.cnblogs.com/images/2011/79762/2011102014250451.png)![](http://pic002.cnblogs.com/images/2011/79762/2011102014251376.png)




下图是一个LDA和PCA区别的例子：

![](http://pic002.cnblogs.com/images/2011/79762/2011102010174967.png)


       两个类别，class1的点都是圆圈，class2的点都是十字。图中有两条直线，斜率在1左右的这条直线是PCA选择的映射直线，斜率在 -1左右的这条直线是LDA选择的映射直线。其余不在这两条直线上的点是原始数据点。可以看到由于LDA考虑了“类别”这个信息（即标注），映射后，可以很好的将class1和class2的点区分开。


2. LDA的一些说明


   第一，降维后的维度是多少？


     PCA降维是直接和数据维度相关的，比如原始数据是n维的，那么PCA后，可以任意选取1维、2维，一直到n维都行（当然是对应特征值大的那些）。LDA 降维是直接和类别的个数相关的，与数据本身的维度没关系，比如原始数据是n维的，一共有C个类别，那么LDA降维之后，一般就是1维，2维到C-1维进行选择（当然对应的特征值也是最大的一些），举个例子，假设图象分类，两个类别正例反例，每个图象10000维特征，那么LDA之后，就只有1维特征，并且这维特征的分类能力最好。
PS：对于很多两类分类的情况，LDA之后就剩下1维，找到分类效果最好的一个阈值貌似就可以了。


   第二，投影的坐标系是否正交


    PCA投影的坐标系都是正交的，而LDA根据类别的标注，关注分类能力，因此不保证投影到的坐标系是正交的（一般都不正交）。


3. LDA计算过程（两个类别）


这一节，主要讨论只有两个类别的数据，LDA的计算。现在要找到一个向量w，将数据x投影到w上去之后，得到新的数据y。第一，为了实现投影后的两个类别的距离较远，用映射后两个类别的均值差的绝对值来度量。第二，为了实现投影后，每个类内部数据点比较聚集，用投影后每个类别的方差来度量。


类别i的均值：


![](http://pic002.cnblogs.com/images/2011/79762/2011102014454997.png)



    类别i投影后的均值（实际上等于mi的投影）：


![](http://pic002.cnblogs.com/images/2011/79762/2011102014472583.png)



    投影后均值差的绝对值：


![](http://pic002.cnblogs.com/images/2011/79762/2011102014492130.png)



    投影后的方差（这里的y是类别i中数据投影后的数据，即y=w_t * x）：


![](http://pic002.cnblogs.com/images/2011/79762/2011102014503457.png)



    目标优化函数为：


![](http://pic002.cnblogs.com/images/2011/79762/2011102014524146.png)



    下面通过展开m'和s'，定义S_B和S_W：



![](http://pic002.cnblogs.com/images/2011/79762/2011102111540310.png)



![](http://pic002.cnblogs.com/images/2011/79762/2011102111555950.png)



![](http://pic002.cnblogs.com/images/2011/79762/2011102111563767.png)


优化目标J(w)改写如下，这样是为了方便推导出计算w的方法。


![](http://pic002.cnblogs.com/images/2011/79762/2011102014561448.png)



    推导过程忽略了，最后推导结果如下：


![](http://pic002.cnblogs.com/images/2011/79762/2011102112004569.png)




假设数据是n维特征，m个数据，分类个数为2。那么Sw实际上是每个类别的协方差矩阵之和，每个类别的协方差矩阵都是n*n的，因此Sw是n*n的，m1-m2是n*1的。计算得到的w是n*1的，即w将维特征映射为1维。


PS：这里不用纠结与Sw的协方差矩阵形式，实际上这是拆拆开来w和w_t后的结果，实际上w*Sw*w_t后，仍旧是一个数值，即表示两个类的投影后方差之和。


4. LDA计算过程（多个类别）


     对于S_w，就是“两个类别的协方差矩阵之和”变成了“多个类别的协方差矩阵之和”。


![](http://pic002.cnblogs.com/images/2011/79762/2011102015035933.png)



      对于S_b，以前是“两个类别的均值之差的绝对值”，现在多个类别了，怎么计算呢？计算任意两个类别之间的均值之差的绝对值之和？这样对于N个类别，需要 C(N,2)次计算，这可能是一个方法，不过LDA使用的方法，计算每个类别的均值与所有类别均值的差异，并且对每个类别的数据量进行加权处理。下式中，m为所有类别的均值，m_i为类别i的均值，n_i为类别i的数据量。


![](http://pic002.cnblogs.com/images/2011/79762/2011102015133280.png)




        对于n维特征、C个类别、m个样本的数据，是将n维数据映射为C-1维。即所求的w是一个n*(c-1)的矩阵。S_w是一个n*n的矩阵（没有除以样本数的协方差矩阵之和），S_B是一个C*C的矩阵，实际上S_B矩阵的的秩最多是C-1，这是因为n_i*(m_i-m)这c向量实际上是线性相关的，因为他们的和是均值m的常数倍。这导致后面求解w，实际上是求出C-1个向量一个构成的一个w。


具体计算忽略。


下面就是一个三个类别的例子： 
![](http://pic002.cnblogs.com/images/2011/79762/2011102112445054.png)


4. LDA其他变种


如果原始数据投影后，仍旧不能很好的分开，那么Kernel LDA是一个解决方法。


LDA在计算量上与数据的维度有关，2DLDA能够大大减小LDA计算量。


5. LDA的一些问题


第一，LDA最多投影出C-1维特征，如果需要更多特征，则要引入其他方法。


第二，LDA假设数据服从单峰高斯分布，比如下面的复杂数据结构。
![](http://pic002.cnblogs.com/images/2011/79762/2011102112430329.png)


5. 参考

[Introduction to LDA](http://www.cmlab.csie.ntu.edu.tw/~cyy/learning/tutorials/LDA.pdf)
[Linear Discriminant Analysis - A Brief Tutorial](http://www.isip.piconepress.com/publications/reports/isip_internal/1998/linear_discrim_analysis/lda_theory.pdf)
[http://www.aiaccess.net/English/Glossaries/GlosMod/e_gm_fisher_discriminant.htm](http://www.aiaccess.net/English/Glossaries/GlosMod/e_gm_fisher_discriminant.htm)
[线性判别分析(Linear Discriminant Analysis, LDA）算法分析](http://blog.csdn.net/warmyellow/article/details/5454943)




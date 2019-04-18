# ［机器学习］学习笔记：PCA（主成分分析）的直观理解 - 知乎
# 

PCA是一种基本的数据降维技术。在知乎或者百度搜索PCA，会找到很多关于PCA的介绍。

我喜欢在直观上理解数学知识。因此我打算从直观的角度把我对PCA的理解写出来。毕竟是个人见解，如有不妥之处欢迎指正。

以下内容假定读者稍微了解过PCA。

我以最简单的模型为例：如何把二维数据压缩至一维。

正如最近很火的一句话：“戏说不是胡说，改编不是乱编”。数据压缩也不是随心所欲地压缩。我们的目标是：让新数据的方差尽可能地大。这样的标准能使得新数据尽可能地不丢失原有数据的信息，因为方差越大，数据间的差异越大。

如下图所示：有六个点，每个点有两个特征，分别对应x轴和y轴。我们需要把他们压缩成一维的数据，即每个点只有一个特征。

因此要寻一条直线，让所有点投影到该直线上。该直线上的刻度即为新数据的值。
![](https://pic2.zhimg.com/v2-a18763438621f12b09ec72d247d70931_b.jpg)
首先我们进行中心化处理。中心化的好处在于，我们寻求的直线必定经过原点。如下图所示。我们只需从所有经过原点的直线中，找一条直线，使得各个数据的方差最大。
![](https://pic2.zhimg.com/v2-8c682ad8e7c825750c523ef40dfeffe1_b.jpg)
如下图所示，设P是样本的一个数据。注意到这样一个性质：由于OP的距离是定的，因此过原点作任何一条直线，记Q是P在该直线上的投影点，都有 ![OQ^{2}+PQ^{2}](https://www.zhihu.com/equation?tex=OQ%5E%7B2%7D%2BPQ%5E%7B2%7D) =定值。
![](https://pic4.zhimg.com/v2-46a18fcb77070635b2df6d3efc67838f_b.jpg)

显然知道 ![OQ](https://www.zhihu.com/equation?tex=OQ) （带正负号的长度）是数据 ![P(x_{p},y_{p})](https://www.zhihu.com/equation?tex=P%28x_%7Bp%7D%2Cy_%7Bp%7D%29) 压缩后的值。

那么如下图所示：对于 ![A_{1}](https://www.zhihu.com/equation?tex=A_%7B1%7D) 到 ![A_{6}](https://www.zhihu.com/equation?tex=A_%7B6%7D) 六个点，分别作直线的垂线，交直线于 ![B_{1}](https://www.zhihu.com/equation?tex=B_%7B1%7D) 到 ![B_{6}](https://www.zhihu.com/equation?tex=B_%7B6%7D) 六个点。直线可以看做数轴，坐标系的原点就是数轴的原点。记 ![B_{1}](https://www.zhihu.com/equation?tex=B_%7B1%7D) 到 ![B_{6}](https://www.zhihu.com/equation?tex=B_%7B6%7D) 六个点的刻度分别为 ![b_{1},…,b_{6}](https://www.zhihu.com/equation?tex=b_%7B1%7D%2C%E2%80%A6%2Cb_%7B6%7D) 。那么 ![b_{1},…,b_{6}](https://www.zhihu.com/equation?tex=b_%7B1%7D%2C%E2%80%A6%2Cb_%7B6%7D) 就是降维后的数据。
![](https://pic2.zhimg.com/v2-a86f315e6a724236d4d18f92691a0d49_b.jpg)
由于数据已经做了中心化处理，所以 ![b_{1}+b_{2}+…+b_{6}=0](https://www.zhihu.com/equation?tex=b_%7B1%7D%2Bb_%7B2%7D%2B%E2%80%A6%2Bb_%7B6%7D%3D0) 。一般性的具体证明如下（电脑上编辑公式太麻烦了，学习时间紧张，暂时就用手写代替吧）：
![](https://pic1.zhimg.com/v2-00297f1a68eb37d10eb8ab64ffbe3758_b.jpg)
也就说明 ![b](https://www.zhihu.com/equation?tex=b) 的均值为0。那么 ![b](https://www.zhihu.com/equation?tex=b) 的方差为： ![\frac{b_{1}^{2}+b_{2}^{2}+b_{3}^{2}+b_{4}^{2}+b_{5}^{2}+b_{6}^{2}}{n}](https://www.zhihu.com/equation?tex=%5Cfrac%7Bb_%7B1%7D%5E%7B2%7D%2Bb_%7B2%7D%5E%7B2%7D%2Bb_%7B3%7D%5E%7B2%7D%2Bb_%7B4%7D%5E%7B2%7D%2Bb_%7B5%7D%5E%7B2%7D%2Bb_%7B6%7D%5E%7B2%7D%7D%7Bn%7D) .

回想我们的目标是让压缩后的数据方差最大，即让 ![\sum_{i=1}^{6}{b_{i}^{2}}](https://www.zhihu.com/equation?tex=%5Csum_%7Bi%3D1%7D%5E%7B6%7D%7Bb_%7Bi%7D%5E%7B2%7D%7D) 最大。

记点 ![A_{i}](https://www.zhihu.com/equation?tex=A_%7Bi%7D) 到直线的距离为 ![d_{i}](https://www.zhihu.com/equation?tex=d_%7Bi%7D) ，那么有 ![d_{i}^{2}](https://www.zhihu.com/equation?tex=d_%7Bi%7D%5E%7B2%7D) + ![b_{i}^{2}](https://www.zhihu.com/equation?tex=b_%7Bi%7D%5E%7B2%7D) =定值。

于是 ![\sum_{i=1}^{6}{b_{i}^{2}}+\sum_{i=1}^{6}{d_{i}^{2}}](https://www.zhihu.com/equation?tex=%5Csum_%7Bi%3D1%7D%5E%7B6%7D%7Bb_%7Bi%7D%5E%7B2%7D%7D%2B%5Csum_%7Bi%3D1%7D%5E%7B6%7D%7Bd_%7Bi%7D%5E%7B2%7D%7D) =定值。

使 ![\sum_{i=1}^{6}{b_{i}^{2}}](https://www.zhihu.com/equation?tex=%5Csum_%7Bi%3D1%7D%5E%7B6%7D%7Bb_%7Bi%7D%5E%7B2%7D%7D) 最大，那么等价于 ![\sum_{i=1}^{6}{d_{i}^{2}}](https://www.zhihu.com/equation?tex=%5Csum_%7Bi%3D1%7D%5E%7B6%7D%7Bd_%7Bi%7D%5E%7B2%7D%7D) 最小。

到这里，问题就差不多解决了。后面的步骤和网上其他方法是一样的。

这个结果是很美妙的，我忍不住和一元线性回归作比较。如下图所示，一共有九个点。我们要进行一元线性回归，所采取的策略是让平方损失最小，通俗地来说，就是让九条绿线的平方和最小。
![](https://pic3.zhimg.com/v2-196c332b5294aaf08bc2493569c0eb0e_b.jpg)
如果是采用PCA，如下图所示，我们采取的策略是让九条红线的平方和最小。
![](https://pic3.zhimg.com/v2-6f4681424ddd9f4231653cccce3270d6_b.jpg)
事实上，一元线性回归也能采取“使点到直线距离的平方和最小”的策略。两种策略各有各的优势与缺陷。只不过“平方误差最小原则”已经被广泛地接受。

类似地，对于n维数据，投影到更小的k维空间也能同样地进行类比。有时间的话我再更新。


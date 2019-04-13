
# scikit-learn 朴素贝叶斯类库使用小结 - 刘建平Pinard - 博客园






# [scikit-learn 朴素贝叶斯类库使用小结](https://www.cnblogs.com/pinard/p/6074222.html)
之前在[朴素贝叶斯算法原理小结](http://www.cnblogs.com/pinard/p/6069267.html)这篇文章中，对朴素贝叶斯分类算法的原理做了一个总结。这里我们就从实战的角度来看朴素贝叶斯类库。重点讲述scikit-learn 朴素贝叶斯类库的使用要点和参数选择。
# 1. scikit-learn 朴素贝叶斯类库概述
朴素贝叶斯是一类比较简单的算法，scikit-learn中朴素贝叶斯类库的使用也比较简单。相对于决策树，KNN之类的算法，朴素贝叶斯需要关注的参数是比较少的，这样也比较容易掌握。在scikit-learn中，一共有3个朴素贝叶斯的分类算法类。分别是GaussianNB，MultinomialNB和BernoulliNB。其中GaussianNB就是先验为高斯分布的朴素贝叶斯，MultinomialNB就是先验为多项式分布的朴素贝叶斯，而BernoulliNB就是先验为伯努利分布的朴素贝叶斯。
这三个类适用的分类场景各不相同，一般来说，如果样本特征的分布大部分是连续值，使用GaussianNB会比较好。如果如果样本特征的分大部分是多元离散值，使用MultinomialNB比较合适。而如果样本特征是二元离散值或者很稀疏的多元离散值，应该使用BernoulliNB。
# 2. GaussianNB类使用总结
GaussianNB假设特征的先验概率为正态分布，即如下式：
$$
P(X_j=x_j|Y=C_k) = \frac{1}{\sqrt{2\pi\sigma_k^2}}exp\Bigg{(}-\frac{(x_j - \mu_k)^2}{2\sigma_k^2}\Bigg{)}
$$
其中$C_k$为Y的第k类类别。$\mu_k和\sigma_k^2$为需要从训练集估计的值。
GaussianNB会根据训练集求出$\mu_k和\sigma_k^2$。 $\mu_k$为在样本类别$C_k$中，所有$X_j$的平均值。$\sigma_k^2$为在样本类别$C_k$中，所有$X_j$的方差。
GaussianNB类的主要参数仅有一个，即先验概率priors ，对应Y的各个类别的先验概率$P(Y=C_k)$。这个值默认不给出，如果不给出此时$P(Y=C_k) = m_k/m$。其中m为训练集样本总数量，$m_k$为输出为第k类别的训练集样本数。如果给出的话就以priors 为准。
在使用GaussianNB的fit方法拟合数据后，我们可以进行预测。此时预测有三种方法，包括predict，predict_log_proba和predict_proba。
predict方法就是我们最常用的预测方法，直接给出测试集的预测类别输出。
predict_proba则不同，它会给出测试集样本在各个类别上预测的概率。容易理解，predict_proba预测出的各个类别概率里的最大值对应的类别，也就是predict方法得到类别。
predict_log_proba和predict_proba类似，它会给出测试集样本在各个类别上预测的概率的一个对数转化。转化后predict_log_proba预测出的各个类别对数概率里的最大值对应的类别，也就是predict方法得到类别。
下面给一个具体的例子，代码如下,亦可见[我的github](https://github.com/ljpzzz/machinelearning/blob/master/classic-machine-learning/native_bayes.ipynb)：
importnumpy as np
X= np.array([[-1, -1], [-2, -1], [-3, -2], [1, 1], [2, 1], [3, 2]])
Y= np.array([1, 1, 1, 2, 2, 2])fromsklearn.naive_bayesimportGaussianNB
clf=GaussianNB()\#拟合数据clf.fit(X, Y)print"==Predict result by predict=="print(clf.predict([[-0.8, -1]]))print"==Predict result by predict_proba=="print(clf.predict_proba([[-0.8, -1]]))print"==Predict result by predict_log_proba=="print(clf.predict_log_proba([[-0.8, -1]]))
结果如下：
==Predict result by predict==
[1]
==Predict result by predict_proba==
[[  9.99999949e-01   5.05653254e-08]]
==Predict result by predict_log_proba==
[[ -5.05653266e-08  -1.67999998e+01]]从上面的结果可以看出，测试样本[-0.8,-1]的类别预测为类别1。具体的测试样本[-0.8,-1]被预测为1的概率为9.99999949e-01 ，远远大于预测为2的概率5.05653254e-08。这也是为什么最终的预测结果为1的原因了。
此外，GaussianNB一个重要的功能是有 partial_fit方法，这个方法的一般用在如果训练集数据量非常大，一次不能全部载入内存的时候。这时我们可以把训练集分成若干等分，重复调用partial_fit来一步步的学习训练集，非常方便。后面讲到的MultinomialNB和BernoulliNB也有类似的功能。
# 3. MultinomialNB类使用总结
MultinomialNB假设特征的先验概率为多项式分布，即如下式：
$$
P(X_j=x_{jl}|Y=C_k) = \frac{x_{jl} + \lambda}{m_k + n\lambda}
$$
其中，$P(X_j=x_{jl}|Y=C_k)$是第k个类别的第j维特征的第l个个取值条件概率。$m_k$是训练集中输出为第k类的样本个数。$\lambda$ 为一个大于0的常数，常常取为1，即拉普拉斯平滑。也可以取其他值。
MultinomialNB参数比GaussianNB多，但是一共也只有仅仅3个。其中，参数alpha即为上面的常数$\lambda$，如果你没有特别的需要，用默认的1即可。如果发现拟合的不好，需要调优时，可以选择稍大于1或者稍小于1的数。布尔参数fit_prior表示是否要考虑先验概率，如果是false,则所有的样本类别输出都有相同的类别先验概率。否则可以自己用第三个参数class_prior输入先验概率，或者不输入第三个参数class_prior让MultinomialNB自己从训练集样本来计算先验概率，此时的先验概率为$P(Y=C_k) = m_k/m$。其中m为训练集样本总数量，$m_k$为输出为第k类别的训练集样本数。总结如下：
|fit_prior|class_prior|最终先验概率|
|false|填或者不填没有意义|$P(Y=C_k) = 1/k$|
|true|不填|$P(Y=C_k) = m_k/m$|
|true|填|$P(Y=C_k) = $class_prior|
在使用MultinomialNB的fit方法或者partial_fit方法拟合数据后，我们可以进行预测。此时预测有三种方法，包括predict，predict_log_proba和predict_proba。由于方法和GaussianNB完全一样，这里就不累述了。
# 4. BernoulliNB类使用总结
BernoulliNB假设特征的先验概率为二元伯努利分布，即如下式：
$$
P(X_j=x_{jl}|Y=C_k) = P(j|Y=C_k)x_{jl} + (1 - P(j|Y=C_k)(1-x_{jl}) 
$$
此时$l$只有两种取值。$x_{jl}$只能取值0或者1。
BernoulliNB一共有4个参数，其中3个参数的名字和意义和MultinomialNB完全相同。唯一增加的一个参数是binarize。这个参数主要是用来帮BernoulliNB处理二项分布的，可以是数值或者不输入。如果不输入，则BernoulliNB认为每个数据特征都已经是二元的。否则的话，小于binarize的会归为一类，大于binarize的会归为另外一类。
在使用BernoulliNB的fit或者partial_fit方法拟合数据后，我们可以进行预测。此时预测有三种方法，包括predict，predict_log_proba和predict_proba。由于方法和GaussianNB完全一样，这里就不累述了。
以上就是scikit-learn 朴素贝叶斯类库的使用的经验总结。希望可以帮到朋友们。
（欢迎转载，转载请注明出处。欢迎沟通交流： liujianping-ok@163.com）






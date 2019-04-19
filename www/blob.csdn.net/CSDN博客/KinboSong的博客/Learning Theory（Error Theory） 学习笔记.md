# Learning Theory（Error Theory） 学习笔记 - KinboSong的博客 - CSDN博客
2017年03月08日 15:54:24[songjinbo3](https://me.csdn.net/KinboSong)阅读数：1606
**1、样本复杂度、模型复杂度（model complexity）、泛化（generalization）能力**
定义：
样本复杂度 = 样本的数量
模型复杂度：一般用未知参数的个数来表征
泛化能力：代表模型的预测能力，表征模型的好坏，用泛化误差来表示。
**2、误差（error）、偏差（bias）、方差（variance）**
定义：假设有很多组采样点，每组采样点都拟合一次模型，得到若干组模型。选定某个特征值（非训练集），根据训练出的多个模型会产生多个预测值。这些预测值的平均值和真实值之间的差值代表模型的偏差（bias）；预测值的方差（variance）代表模型的方差。
用图表表示如下：
![](https://img-blog.csdn.net/20170309093643810?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
误差（error）定义：代表模型的预测能力，表征模型的好坏。
误差 = 泛化误差
误差 = 偏差+方差
模型复杂度与泛化误差、偏差、方差之间的关系如下图所示：
![](https://img-blog.csdn.net/20170309093658834?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**参考：《Understanding the Bias-Variance Tradeoff》 http://scott.fortmann-roe.com/docs/BiasVariance.html#fnref:1**
**《Accurately Measuring Model Prediction Error》 http://scott.fortmann-roe.com/docs/MeasuringError.html**
**3、泛化误差（generalization error）、期望误差（expected error）、结构化风险（structural risk）、训练误差（training error）、经验误差（empirical error）、经验风险（empirical risk）、置信风险（incredible risk）、Traning Optimism**
个人理解：训练误差 =  经验误差 = 经验风险
定义如下：
![](http://latex.codecogs.com/gif.latex?%5Chat%7B%5Cvarepsilon%7D(h)=%5Cfrac%7B1%7D%7Bm%7D%5Cunderset%7Bi=1%7D%7B%5Coverset%7Bm%7D%7B%5Csum%7D%7D%5C&hash;%5C%7Bh(x%5E%7B(i)%7D)%5Cneq&space;y%5E%7B(i)%7D%5C%7D)
训练误差为模型在训练样本中的错分类的比例，如果我们要强调![](http://latex.codecogs.com/gif.latex?%5Chat&space;%7B%5Cvarepsilon%7D(h))是依赖训练集的
泛化误差 = 期望误差 = 结构化风险
![](http://latex.codecogs.com/gif.latex?%5Cvarepsilon(h)=P_%7B(x,y)%5Csim&space;D%7D(h(x)%5Cneq&space;y))
这里得到的是一个概率，表示通过特定的分布D生成的样本（x，y）中的y与通过预测函数h(x)生成的结果不同的概率。
置信风险 = Training Optimism
    泛化误差 = 训练误差 + Traning Optimism
结构化风险 = 经验风险 + 置信风险
模型复杂度与泛化误差、训练误差、Optimism（置信误差）之间的关系如下图所示：
吴恩达的机器学习课程中曾经证明：
（1）“随着样本量的增加，训练误差接近泛化误差的概率随之增大”。
（2）“如果需要确保训练误差和泛化误差的差值在一个给定的范围内，并且发生的概率不低于![](http://latex.codecogs.com/gif.latex?1-%5Cdelta)，需要的样本数量和假设集的VC维大小呈线性相关。”
参考：http://blog.csdn.net/myarrow/article/details/50610411
**4、过拟合、欠拟合**
通过以上两张图片可知，泛化误差在模型复杂度取中间的一个最优值的时候可以得到一个最小值。当模型复杂度小于这个最优值时，偏差、训练误差对泛化能力影响较大；当模型复杂度大于这个最优值时，方差、置信风险对泛化能力影响较大。
**5、模型比较**
参考:《Accurately Measuring Model Prediction Error》 http://scott.fortmann-roe.com/docs/MeasuringError.html
比如“交叉验证”
**6、提高泛化能力的方法**
（1）增加样本数量。样本数量越多，越不容易过拟合。
（2）减少特征数量。特征越少，越不容易过拟合。比如，吴恩达在《机器学习》第10课中讲的“特征选择”。
（3）添加正则项。利用结构风险最小化，抑制过拟合。比如：SVM中得soft margin
（4）贝叶斯规范与正则化。原理也是利用结构风险最小化，抑制过拟合。
**7、贝叶斯统计与正则化（Bayesian statistics and regularization）**
（1）默认theta是随机分布的，而不是一个固定的值，下面求出theta的后验分布
![](https://img-blog.csdn.net/20170309091616000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**上式中第4个等号的原因是：假设x的采集与theta无关。**
（2）根据第一步中求得的theta的分布P(theta|S),求出预测样本点的概率：
（3）MAP
theta的后验分布p(theta|S)很难求，所以改成求p(theta|S)得最大值theta，将theta视为固定值。这种方法叫做最大后验概率估计（Maximum a posteriori）方法，公式如下：
所以说，MAP只是对原始方法的一个改进。
参考博客：[http://http://blog.csdn.net/qrlhl/article/details/48135873](http://http//blog.csdn.net/qrlhl/article/details/48135873)
**8、需要完善的地方：**
（1）没有说明偏差与训练误差、方差与置信风险之间的关系
（2）没有说明样本复杂度是如何影响偏差和方差、训练误差和置信风险的

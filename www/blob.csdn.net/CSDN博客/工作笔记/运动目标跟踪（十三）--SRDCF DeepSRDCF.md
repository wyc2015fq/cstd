# 运动目标跟踪（十三）--SRDCF/DeepSRDCF - 工作笔记 - CSDN博客





2016年08月17日 13:04:32[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：10967
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









**原文：**

**http://m.blog.csdn.net/article/details?id=51325716**

**介绍DeepSRDCF之前，首先介绍SRDCF。**

#### **今天对SRDCF算法做一些笔记[paper:Learning Spatially Regularized Correlation Filters for Visual Tracking]**

这篇文章同样是目前比较好的，在VOT2015年的排名第四。他是KCF的一种变形[KCF-SRDCF-DeepSRDCF].下面详细介绍paper.速度4fps.本文主要解决的问题是kcf算法的boundary effects(由循环矩阵导致的)，在loss function中引入惩罚项，期望抑制离中心较远的特征的影响。


转载请注明：http://write.blog.csdn.net/postedit/51325716

- **paper 特点**

> - 在KCF/DCF基础上改善了boundary effects，加入惩罚项[spatial regularization function that penalizes(惩罚) filter coefficients residing outside the target region].
- 不同的scale搜索，处理尺度的问题（暴力的分为指定个尺度，依次匹配）。
- 在求解corelation filters时，利用iterative Gauss-Seidel method在线学习。

作者的最重要的贡献在与第一点。

- **details**

> - 目的： 从一组train samples中学习一个corralation filter[DCF算法，paper中利用的是circular struct+FFT加速运算]。
- 由于标准的DCF有许多缺点[boundary effects,容易over-fitting]所以本文在此有所改进，那么标准的DCF是怎样的呢？标准的DCF就是一个监督学习，学习一个linear classifier&linear regressor,可是SVM等也是同样的思想，两者最大的不同在于DCF利用的时circular correlation 来training and detection.circular correlation filters的两大优势为：第一，训练样本多样化(有各种样本的平移【KCF】);第二，FFT加速运算，适用于online-tracking.


> 
标准的DCF原理如下：


![](https://img-blog.csdn.net/20160505203004962?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20160505203009629?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

> 
首先式（1）中，S为响应值，X为search region中的特征，**f**为学习的filters系数，‘*’表示循环卷积[可以参见KCF原理]，式（2）为loss function,然后利用循环矩阵来求解[详细参见KCF算法]， 缺点循环卷积带来boundary effects:


![](https://img-blog.csdn.net/20160505203603507?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


> 
 提出SRDCF算法，在DCF的基础上添加正则化项：空间权重函数(惩罚项,w)，将式（2）的式子修改如下：


![](https://img-blog.csdn.net/20160505204028696?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

> 
其中惩罚权重w满足(负)Gauss分布，目的是超过边界的w更大，表示惩罚越大[注意不是label中的高斯分布，不要弄混淆了]，所以归一化后，loss function如下：


![](https://img-blog.csdn.net/20160505204647921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

> 
对式（5），利用FFT和循环矩阵的性质进行计算，这样学习到的滤波系数可视化对比如下：


![](https://img-blog.csdn.net/20160505205100162?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


> 
最终求解等价于求解线性方程组 



![](https://img-blog.csdn.net/20160505205926986?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


> 
由于采用circular 算法，某些等式可以简化，所以提出Gauss-seidel method来简化【略】。


有了上面的求解，那么在跟踪的时候，根据第一帧的ground-truth,就可以进行训练，迭代等式如下（第一帧的信息可以求迭代需要的初始化的值）：


![](https://img-blog.csdn.net/20160505205931720?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
- 
#### test部分


> - 训练好了后，用于detection,the location of the target in a new frame t is estimated by applying the filter f ˆ t−1 that has been updated in the previous frame,首先，we apply the filter at multiple resolutions to estimate changes in the target size.则选择一定的候选框，并做放缩处理，最后crop为同样的size[作为循环矩阵的输入].
- 搜索采用Sub-grid策略，即用t-1的滤波系数，滑动step>1pixel,这样根据score，大致定位（可以考虑金字塔策略），再在频域，对每一个尺度，利用如下公式迭代求解最佳匹配的位置：


![](https://img-blog.csdn.net/20160505211531132?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

> 
文章最后利用了多种特征。In addition to the HOG features used in [12], the submitted variant of SRDCF also employs Colour Names and greyscale features. These features are averaged over the 4 × 4 HOG cells and then concatenated, giving a 42 dimensional feature vector
 at each cell






#### **DeepSRDCF[Convolutional Features for Correlation Filter Based Visual Tracking]**

他是在SRDCF的基础上，将hand-crafted的特征换为CNN的特征，其他和SRDCF一模一样，这篇文章对不同的特征进行了实验，说明了CNN特征在解决跟踪的问题采取底层的特征效果会比较好，说明了跟踪问题并不需要太高的语义信息。作者并没有公布源代码，是基于Matconvnet来做的。
- **paper特点**
- -  探索了不同conv的特征的不同影响，得出利用第一层conv的特征效果最好。
- 利用PCA将第一层的特征降为40-D: the DeepSRDCF employs convolutional features from a pre-trained network.A Principal Component Analysis is used to reduce the feature dimensionality of the extracted activations。





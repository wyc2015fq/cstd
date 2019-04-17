# 文本分类 - 样本不平衡的解决思路与交叉验证CV的有效性 - 素质云笔记/Recorder... - CSDN博客





2018年11月01日 14:24:26[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：1476








现实情况中，很多机器学习训练集会遇到样本不均衡的情况，应对的方案也有很多种。

笔者把看到的一些内容进行简单罗列，此处还想分享的是交叉验证对不平衡数据训练极为重要。




### 文章目录
- [1 样本不平衡的解决思路](#1__10)
- [1.2 将不平衡样本当作离群点](#12__17)
- [1.2 欠采样/过采样](#12__30)
- [**观点：为什么over-sampling在这种情况下工作得不好**](#oversampling_53)
- [**观点：两则的缺陷**](#_58)
- [**观点：解决**](#_63)
- [**观点：下采样的情况下的三个解决方案**](#_66)
- [1.3 训练策略的优化](#13__76)
- [1.3.1 Focal_Loss](#131_Focal_Loss_79)
- [1.3.2 class_weight](#132_class_weight_92)
- [1.4 不平衡评价指标：不要ROC，用Precision/Recall](#14_ROCPrecisionRecall_100)
- [1.5 使用相关模型 或调整预测概率](#15___118)
- [2 交叉验证CV的有效性](#2_CV_128)




# 1 样本不平衡的解决思路

有好几篇原创知乎内容都很赞，不做赘述，参考：
- [严重数据倾斜文本分类，比如正反比1:20～100，适合什么model，查准一般要做到多少可以上线？](https://www.zhihu.com/question/59236897)
- [如何处理数据中的「类别不平衡」？](https://zhuanlan.zhihu.com/p/32940093)

## 1.2 将不平衡样本当作离群点

具体问题具体分析，依据不平衡的比例，如果一些问题是极其不平衡的1：100+，该任务就可以当作寻找离群点。一分类就是左图中一大堆点的区域，当有超过这一堆的就会分到另外一类。
![在这里插入图片描述](https://img-blog.csdn.net/20170803192449963?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
常规的可以使用一些聚类 或 OneClassSVM（[无监督︱异常、离群点检测 一分类——OneClassSVM](https://blog.csdn.net/sinat_26917383/article/details/76647272)）

常规的可参考文献 ：
- [微调：数据挖掘中常见的『异常检测』算法有哪些？](https://www.zhihu.com/question/280696035/answer/417091151)
- [「异常检测」开源工具库推荐](https://zhuanlan.zhihu.com/p/37132428)

## 1.2 欠采样/过采样

这个方式应该是最多被提及的，对于样本比较多的分类进行欠采样，样本比较少的进行过采样。

过采样，就相当于文本增强，常见的文本增强有：
- 
随机打乱句子

- 
加入噪声：在正例中加入一些噪声词，或随机剔除一些词

- 
裁剪掉长文本的某一句（或开头句，或结尾句）

- 
复制（最常见）

- 
paraphrasing，用序列到序列的方式去生成，在问答系统有一个领域叫做问题复述，根据原始问题生成格式更好的问题，相当于修正不规范的问题，将新问题代替旧问题输入到问答系统中，我觉得的也算是一种数据增强方法了吧（[文本多分类踩过的坑](https://zhuanlan.zhihu.com/p/35586145)）。

- 
TTA ，kaggle比赛转用，中文->英文->中文


```
英语 - 德语 - 英语 
EN -> DE -> EN 
EN -> FR -> EN 
EN -> ES -> EN
```

欠采样：删除掉一些文本（一些不重要文本进行剔除）

### **观点：为什么over-sampling在这种情况下工作得不好**

以smote为例，我们希望从样本及其最近邻的点的连线上选一个随机点将其作为新的样本来合成。但是文本数据（无论是用n-gram feature还是distributed representation）都是很高维度的。在高维空间的一个事实就是数据倾向于接近互相正交，故而两两不相近，所以采用NN的思想来做up-sampling，效果是不会太好的。

正样本的数据，是否能够公正地代表正样本的分布？是否有明显的属于正样本但是没有包括进来的例子？很大可能，需要补数据。如果真的是缺乏正样本数据的话，用什么办法都很难了。

### **观点：两则的缺陷**

过拟合的缺陷：过拟合风险

欠拟合的缺陷：缺失样本，偏差较大

### **观点：解决**

过采样（或SMOTE）+强正则模型（如XGBoost）可能比较适合不平衡的数据。

### **观点：下采样的情况下的三个解决方案**

（[干货｜如何解决机器学习中数据不平衡问题](https://mp.weixin.qq.com/s?__biz=MzA4NzE1NzYyMw==&mid=2247492055&idx=3&sn=76e4216a997199a6b2b76daa403ef000&chksm=903f1fcfa74896d92218c41814a7423c79b184fdff462129b446f39d2b3565ed7db0ee7419c9&mpshare=1&scene=1&srcid=0701s3q4k9QFeMkdYAiJcRs0#rd)）

因为下采样会丢失信息，如何减少信息的损失呢？
- 第一种方法叫做EasyEnsemble，利用模型融合的方法（Ensemble）：多次下采样（放回采样，这样产生的训练集才相互独立）产生多个不同的训练集，进而训练多个不同的分类器，通过组合多个分类器的结果得到最终的结果。
- 第二种方法叫做BalanceCascade，利用增量训练的思想（Boosting）：先通过一次下采样产生训练集，训练一个分类器，对于那些分类正确的大众样本不放回，然后对这个更小的大众样本下采样产生训练集，训练第二个分类器，以此类推，最终组合所有分类器的结果得到最终结果。
- 第三种方法是利用KNN试图挑选那些最具代表性的大众样本，叫做NearMiss，这类方法计算量很大，感兴趣的可以参考“Learning from Imbalanced Data”这篇综述的3.2.1节。

## 1.3 训练策略的优化

在模型训练的时候有一些策略，比较常见的是sklearn的class_weight：

### 1.3.1 Focal_Loss

举一个keras的例子，[focal_loss](https://github.com/mkocabas/focal-loss-keras/blob/master/focal_loss.py)：

```
# new focal loss
from keras import backend as K
def new_focal_loss(gamma=2., alpha=.25):
    def focal_loss_fixed(y_true, y_pred):
            pt_1 = tf.where(tf.equal(y_true, 1), y_pred, tf.ones_like(y_pred))
            pt_0 = tf.where(tf.equal(y_true, 0), y_pred, tf.zeros_like(y_pred))
            return -K.sum(alpha * K.pow(1. - pt_1, gamma) * K.log(pt_1))-K.sum((1-alpha) * K.pow( pt_0, gamma) * K.log(1. - pt_0))
    return focal_loss_fixed
```

### 1.3.2 class_weight

在随机选择mini batch的时候，每个batch中正负样本配平。示例：

```
from sklearn.utils import class_weight
   imbalance_label = class_weight.compute_class_weight('balanced',np.unique(y_train), y_train)
```

## 1.4 不平衡评价指标：不要ROC，用Precision/Recall

[Credit Card Fraud: Handling highly imbalance classes and why Receiver Operating Characteristics Curve (ROC Curve) should not be used, and Precision/Recall curve should be preferred in highly imbalanced situations](https://www.kaggle.com/lct14558/imbalanced-data-why-you-should-not-use-roc-curve)

来看一下这篇kaggle文献的观点，样本不均衡的情况下不适用ROC，而应该考量的是Precision/Recall这两条曲线。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181101114141337.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)

作者的表述如下，笔者就不翻译啦：

For a PR curve, a good classifer aims for the upper right corner of the chart but upper left for the ROC curve.

While PR and ROC curves use the same data, i.e. the real class labels and predicted probability for the class lables, you can see that the two charts tell very different stories, with some weights seem to perform better in ROC than in the PR curve.

While the blue, w=1, line performed poorly in both charts, the black, w=10000, line performed “well” in the ROC but poorly in the PR curve. This is due to the high class imbalance in our data. ROC curve is not a good visual illustration for highly imbalanced data, because the False Positive Rate ( False Positives / Total Real Negatives ) does not drop drastically when the Total Real Negatives is huge.

Whereas Precision ( True Positives / (True Positives + False Positives) ) is highly sensitive to False Positives and is not impacted by a large total real negative denominator.

The biggest difference among the models are at around 0.8 recall rate. Seems like a lower weight, i.e. 5 and 10, out performs other weights significantly at 0.8 recall. This means that with those specific weights, our model can detect frauds fairly well (catching 80% of fraud) while not annoying a bunch of customers with false positives with an equally high precision of 80%.

## 1.5 使用相关模型 或调整预测概率

不对数据进行过采样和欠采样，但使用现有的集成学习模型，如随机森林，输出随机森林的预测概率，调整阈值得到最终结果

# 2 交叉验证CV的有效性

但是如果你处于比赛阶段，如果是分类单一还可以，如果分类较多比较复杂的分类体系的话，过采与欠采就非常困难。欠采影响分布之后，会对指标造成非常明显的打击，不能轻易动大簇类别。不过，小类别过采样，这个还是可以接受的，直接复制的效果也很明显。一般情况下，为了高效训练以及模型融合，一般情况下对不平衡不会做太大的采样操作。

对于分类体系较为复杂的文本分类任务，交叉验证的结果受不平衡数据，效果也有很大差异。正因为很大差异，CV对于不平衡来说是一种非常有效的训练手段。

以笔者看到的kaggle Toxic Comment Classification中该篇文献：[Things you need to be aware of before stacking](https://www.kaggle.com/ogrellier/things-you-need-to-be-aware-of-before-stacking#)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181101140938245.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)
可以看到不同分类，单个模型的OOF 预测结果差异很大，对于Full OOF,一些fold的效果差异比较大。

有差异，有效的融合就显得很有必要，单纯的bagging在一起并不合理。






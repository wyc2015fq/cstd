# 声学特征变换 LDA - xmdxcsj的专栏 - CSDN博客





2017年11月12日 16:31:18[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1293








## 含义

[Linear Discriminant Analysis 线性判别式分析](https://zh.wikipedia.org/wiki/%E7%B7%9A%E6%80%A7%E5%88%A4%E5%88%A5%E5%88%86%E6%9E%90)是一种降维算法，特征经过映射以后，在新的空间有最大的类间距离和最小的类内距离；LDA降维的维度跟类别的个数有关 

相关公式推导可以参考[这篇博客](http://www.cnblogs.com/jerrylead/archive/2011/04/21/2024384.html)

## kaldi实现

### 特征降维

特征做完splice以后进行降维

```
steps/train_lda_mllt.sh
acc-lda #使用pdf-id作为类别，获得统计量
est-lda #获得lda转换矩阵
```

### 数据归一化

![这里写图片描述](https://img-blog.csdn.net/20171112163106592?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

为了加快网络训练，一般需要对输入特征进行处理[1]： 

- 零均值 

- 去相关 

- 方差规整
在方差处理的时候，如果已知某些维度比较重要，可以增大它们的方差，有益于网络训练。 

这里做LDA并不降维，求得到的转换矩阵是一些较大的特征值对应的特征向量（特征值的解释参考[知乎](https://www.zhihu.com/question/20507061)）组成的，可以挑选出重要的特征维度。
`steps/libs/nnet3/train/chain_objf/acoustic_model.py: compute_preconditioning_matrix`




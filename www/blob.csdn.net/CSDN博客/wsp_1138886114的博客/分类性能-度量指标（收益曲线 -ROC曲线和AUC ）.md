# 分类性能-度量指标（收益曲线 -ROC曲线和AUC ） - wsp_1138886114的博客 - CSDN博客





2018年07月28日 18:08:08[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：404










- - - [一、误分类矩阵](#一误分类矩阵)- - [1.1 医学图像识别二分类问题](#11-医学图像识别二分类问题)


- [二、ROC曲线和AUC值](#二roc曲线和auc值)- - [2.1 ROC曲线分析](#21-roc曲线分析)
- [2.2 AUC判断分类器（预测模型）](#22-auc判断分类器预测模型)


- [三、sklearn计算ROC](#三sklearn计算roc)






### 一、误分类矩阵

![这里写图片描述](https://img-blog.csdn.net/20180821113643453?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 1.1 医学图像识别二分类问题

```
(1) 真阳性(True Positive，TP)： 检测有结节，且实际有结节；正确肯定的匹配数目；
(2) 假阳性(False Positive，FP)：检测有结节，但实际无结节；误诊，给出的匹配是不正确的；
(3) 真阴性(True Negative，TN)： 检测无结节，且实际无结节；正确拒绝的非匹配数目；
(4) 假阴性(False Negative，FN)：检测无结节，但实际有结节；漏诊，没有正确找到的匹配的数目。
```
- 几个主要参数指标如下：
真阳性率(True Positive Rate，TPR)，灵敏度(Sensitivity)，**召回率**(Recall)： 


$TPR=\frac{TP}{TP+FN}$

假阳性率(False Positice Rate，FPR)，**误诊率**( 1 - 特异度)：  


$TPR=\frac{FP}{FP+TN}$
### 二、ROC曲线和AUC值
- ROC：是“受试者工作特征”（Receiver Operating Characteristic）。
ROC曲线的面积就是AUC（Area Under the Curve）。 

ROC曲线和AUC值常被用来评价一个二值分类器 (binary classifier) 的优劣。- 使用ROC曲线有个很好的特性：
当测试集中的正负样本的分布变化的时候，ROC曲线能够保持不变。  

在实际的数据集中经常会出现类不平衡(class imbalance)现象，即负样本比正样本多很多(或者相反)， 而且测试数据中的正负样本的分布也可能随着时间变化。 但ROC曲线能够保持不变。- 分类器或分类算法:
**真正率**(True positive rate， TPR)，**预测为正且实际为正**的样本占所有正例样本 的比例。**假正率**(False positive rate， FPR)，**预测为正但实际为负**的样本占所有负例样本 的比例评价指标主要有precision，recall，F1 score等
![这里写图片描述](https://img-blog.csdn.net/20180728165747692?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
ROC曲线是评判一个模型好坏的标准: 
    TPR（真正率）和 FPR（假正率）
    ROC曲线就是以这两个值为坐标轴画的。  

比如逻辑回归得到的结果是概率，那么就要取阈值来划分正负，这时候，每划一个阈值，就会产生一组FPR和TPR的值， 
然后把这组值画成坐标轴上的一个点，这样，当选取多组阈值后，就形成了ROC曲线（每次选取一个不同的阈值， 
我们就可以得到一组FPR和TPR，即ROC曲线上的一点），AUC值就是ROC曲线下方的面积。 
最好的阈值是不能通过这个图知道的，要通过KS曲线得出。

KS曲线的纵轴是表示TPR和FPR的值: 
    就是这两个值可以同时在一个纵轴上体现，横轴就是阈值. 
    然后在两条曲线分隔最开的地方，对应的就是最好的阈值，也是该模型最好的AUC值， 
    就比如是上图的AUC=0.53，上图中，一条曲线是FPR，一条是TPR
```

##### 2.1 ROC曲线分析

![这里写图片描述](https://img-blog.csdn.net/20180728174337899?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 2.2 AUC判断分类器（预测模型）

```
AUC = 1，是完美分类器，采用这个预测模型时，存在至少一个阈值能得出完美预测。绝大多数预测的场合，不存在完美分类器。
0.5 < AUC < 1，优于随机猜测。这个分类器（模型）妥善设定阈值的话，能有预测价值。
AUC = 0.5，跟随机猜测一样（例：丢铜板），模型没有预测价值。
AUC < 0.5，比随机猜测还差；但只要总是反预测而行，就优于随机猜测。
```

![这里写图片描述](https://img-blog.csdn.net/20180728174947691?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 三、sklearn计算ROC

![这里写图片描述](https://img-blog.csdn.net/20180728180349168?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```php
y = np.array([1, 1, 2, 2])
scores = np.array([0.1, 0.4, 0.35, 0.8])
fpr, tpr, thresholds = metrics.roc_curve(y, scores, pos_label=2) 

输出：（TPR, FPR, 截断点）
fpr = array([ 0. ,  0.5,  0.5,  1. ])
tpr = array([ 0.5,  0.5,  1. ,  1. ])
thresholds = array([ 0.8 ,  0.4 ,  0.35,  0.1 ])#截断点
```

鸣谢： 
[https://blog.csdn.net/tanzuozhev/article/details/79109311](https://blog.csdn.net/tanzuozhev/article/details/79109311)
[https://blog.csdn.net/ybdesire/article/details/51999995](https://blog.csdn.net/ybdesire/article/details/51999995)




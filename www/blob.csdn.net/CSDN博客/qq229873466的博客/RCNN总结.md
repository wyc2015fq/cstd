# RCNN总结 - qq229873466的博客 - CSDN博客

2017年05月22日 20:32:04[qq229873466](https://me.csdn.net/qq229873466)阅读数：309


# RCNN算法

## 训练：

ILSVRC 2012数据库，训练网络A

VOC图片，通过SS 获得 RP图片（和真值区域的IoU>=0.5为正类，剩下为负类）

RP图片，数据预处理（减均值，裁剪）

RP图片，以A网络权值fine-tuning训练网络B

RP图片，通过网络B获得pool5特征和fc8特征

RP位置和pool5特征，回归得到BB

fc8特征（真值框为正类， IoU<0.3为负类，其余舍弃），HNM训练SVM分类器

## 测试:

图片，通过SS获得 RP图片

RP图片，数据预处理（减均值，裁剪）

网络B，提取pool5特征和fc8特征

fc8特征，通过SVM得出类别分数

pool5特征和类别分数，通过GNMS和BB得到位置

## 术语：

Selective search（SS）

Region proposals（RP）

Hard nagetive mining（HNM）

Bounding box（BB）

Greedy non-maximum suppression（GNMS）

## 关键问题：

RP的类别怎么决定？

IoU大于某个重叠阈值则为正类，反之亦然。阈值的选取由验证集实验结果决定。

为什么fine-tuning和SVM训练时的正负样本数不一样？

存在很多IoU>0.5的非真值框样本，如果加入这些样本，fine-tuning训练的正类数就会增多，可以避免过拟合，但同时也会造成解不是最优，因为这个网络不是训练成精确定位的。

为什么用SVM而不是softmax输出？

经过实验发现用SVM效果比softmax好，原因可能由多种因数组成，包括fine-tuning时定义的正类没有强调精确定位；softmax训练用的负类是随机的，而不像HNM那样。


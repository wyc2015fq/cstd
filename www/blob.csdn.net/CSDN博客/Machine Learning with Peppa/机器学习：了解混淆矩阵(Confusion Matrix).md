# 机器学习：了解混淆矩阵(Confusion Matrix) - Machine Learning with Peppa - CSDN博客





2018年04月18日 20:07:06[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：1217








> 


混淆矩阵是数据科学、数据分析和机器学习中总结分类模型预测结果的情形分析表，以矩阵形式将数据集中的记录按照真实的类别与分类模型作出的分类判断两个标准进行汇总。




#### 1. 什么是混淆矩阵

#### 混淆矩阵的每一列代表了预测类别 ，每一列的总数表示预测为该类别的数据的数目；每一行代表了数据的真实归属类别 ，每一行的数据总数表示该类别的数据实例的数目。

#### 以二元分类问题为例，数据集存在肯定类别和否定类别两类记录，而分类模型对记录分类可能作出阳性判断（判断记录属于肯定类别）或阴性判断（判断记录属于否定类别）两种判断。混淆矩阵是一个2 × 2的情形分析表，显示以下四组记录的数目：作出正确判断的肯定记录（真阳性）、作出错误判断的肯定记录（假阴性）、作出正确判断的否定记录（真阴性）以及作出错误判断的否定记录（假阳性）。表1给出了混淆矩阵的结构。以筛检化验为例，真阳性和假阳性分别是病人和健康人的结果阳性，而真阴性和假阴性分别是健康人和病人的结果阴形。显然，分类模型对在混淆矩阵对角线上的真阳性和真阴性记录组作出了正确的分类，而对反对角线上的假阳性和假阴性记录组发生了误判。

#### ![](https://img-blog.csdn.net/20180418200255704?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzk1MjE1NTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

#### 1. 混淆矩阵的举例






TP = True Postive = 真阳性； FP = False Positive = 假阳性


FN = False Negative = 假阴性； TN = True Negative = 真阴性





比如我们一个模型对15个样本进行预测，然后结果如下。


预测值：1    1    1    1    1    0    0    0    0    0    1    1    1    0    1


真实值：0    1    1    0    1    1    0    0    1    0    1    0    1    0    0







这个就是混淆矩阵。矩阵中的这四个数值，经常被用来定义其他一些度量。




准确度(Accuracy) = (TP+TN) / (TP+TN+FN+TN)


在上面的例子中，准确度 = (5+4) / 15 = 0.6




精度(precision, 或者PPV, positive predictive value) = TP / (TP + FP)


在上面的例子中，精度 = 5 / (5+4) = 0.556




召回(recall, 或者敏感度，sensitivity，真阳性率，TPR，True Positive Rate) = TP / (TP + FN)


在上面的例子中，召回 = 5 / (5+2) = 0.714




特异度(specificity，或者真阴性率，TNR，True Negative Rate) = TN / (TN + FP)


在上面的例子中，特异度 = 4 / (4+2) = 0.667




F1-值(F1-score) = 2*TP / (2*TP+FP+FN) 


在上面的例子中，F1-值 = 2*5 / (2*5+4+2) = 0.625



#### 3. 混淆矩阵延伸出的各个评价指标


从混淆矩阵中，可以衍生出各种评价的指标。如下是截取的wiki上的一个截图（[https://en.wikipedia.org/wiki/Confusion_matrix](https://en.wikipedia.org/wiki/Confusion_matrix)） 
![这里写图片描述](https://img-blog.csdn.net/20161104174912959)






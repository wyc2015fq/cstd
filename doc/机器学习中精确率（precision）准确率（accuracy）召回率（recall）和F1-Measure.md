# 机器学习中精确率（precision）准确率（accuracy）召回率（recall）和F1-Measure

2018年07月13日 12:18:41 [Jeo_dmy](https://me.csdn.net/duyuan6949) 阅读数：433

## 先验知识

我们首先将数据的类别统一分为两类：正类和负类。例如：一个数据集中的数据一共有3类，小学生、中学生、高中生。我们的目标是预测小学生，那么标记为小学生的数据就是正类，标记为其他类型的数据都是负类。

数据有两种状态：测试集数据和预测结果数据。

对一批测试数据进行预测，结果可以分成四种。

TP(True Positive): 原本是正类，预测结果为正类。(正确预测为正类)
FP(False Positive): 原本是负类，预测结果为正类。(错误预测为正类)
TN(True Negative): 原本是负类，预测结果为负类。(正确预测为负类)
FN(False Negative): 原本是正类，预测结果为负类。(错误预测为负类)



## 精确率（precision）

​    precision是表示预测为正样本中，被实际为正样本的比例。可以看出precision是考虑的正样本被预测正确的比例．根据图1-1可得其计算公式为：`P = TP / (TP + FP)`

对应于检索中的**查准率**，`检索出相关文档数/检索出的文档总数`



## 准确率（accuracy）

准确率表示所有的预测样本中，预测正确的比例．其计算公式为：`A = (TP + TN) / (TP + FN + FP + TN)`

## 召回率（recall）

​    召回率是表示实际为正样本中，预测为正样本的比例。可以看出，召回率考虑的是正样本的召回的比例．根据图1-1可得其计算公式为：`P = TP / (TP + FN)`



**总结：精确率（accuracy）和召回率（recall）计算公式的分子都是TP也就是正样本被预测为正样本的数量，可知其为正样本的精确率和正样本的召回率．而准确率（accuracy）主要表征的是整体预测正确的比例．**



​                                                         **![img](https://img-blog.csdn.net/20180713115009517?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1eXVhbjY5NDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**

 **图1-1**

**图1-1参数描述：TP（True Positive）表示实际为正样本被预测为正样本的数量；FN（False Negative）表示实际为正样本却被预测为负样本的数量；FP（False Positive）表示实际为负样本却被预测为正样本的数量；TN（True Negative）表示实际为负样本预测也为负样本的数量。可以得出(TP + FＮ)是全部实际的正样本数，而（TP + FP）是所有预测为正样本的数量。**







对于二分类问题，机器预测的和实际的还是会有所偏差，所以我们引入以下几个概念来评价分类器的优良。

**一、TP、TN、FP、FN概念**

首先有关TP、TN、FP、FN的概念。大体来看，TP与TN都是分对了情况，TP是正类，TN是负类。则推断出，FP是把错的分成了对的，而FN则是把对的分成了错的。（我的记忆方法：首先看第一个字母是T则代表分类正确，反之分类错误；然后看P，在T中则是正类，若在F中则实际为负类分成了正的。）

【举例】一个班里有男女生，我们来进行分类，把女生看成正类，男生看成是负类。我们可以用混淆矩阵来描述TP、TN、FP、FN。

 

|                             | 相关（Relevant），正类                                       | 无关（NonRelevant），负类                                    |
| :-------------------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| 被检索到（Retrieved）       | True Positives（TP，正类判定为正类。即女生是女生）           | False Positives（FP，负类判定为正类，即“存伪”。男生判定为女生） |
| 未被检索到（Not Retrieved） | False Negatives（FN，正类判定为负类，即“去真”。女生判定为男生） | True Negatives（TN，负类判定为负类。即男生判定为男生）       |

 

**二、准确率、精确率（精准率）、召回率、F1值**

**1.准确率（Accuracy）。**顾名思义，就是所有的预测正确（正类负类）的占总的比重。

![img](https://img-blog.csdn.net/20180709094035173?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTMwNjMwOTk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**2.精确率（Precision）**，查准率。即正确预测为正的占全部预测为正的比例。个人理解：真正正确的占所有预测为正的比例。

![img](https://img-blog.csdn.net/20180709094203518?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTMwNjMwOTk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**3.召回率（Recall）**，查全率。即正确预测为正的占全部实际为正的比例。个人理解：真正正确的占所有实际为正的比例。

![img](https://img-blog.csdn.net/2018070909424549?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTMwNjMwOTk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**4.F1值。**F1值为算数平均数除以几何平均数，且越大越好，将Precision和Recall的上述公式带入会发现，当F1值小时，True Positive相对增加，而false相对减少，即Precision和Recall都相对增加，即F1对Precision和Recall都进行了加权。

![img](https://img-blog.csdn.net/20180709093550249?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTMwNjMwOTk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

公式转化之后为：

![img](https://img-blog.csdn.net/20180709093934948?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTMwNjMwOTk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 

**三、ROC曲线、AUC值**

**1.ROC曲线。**接收者操作特征曲线（receiver operating characteristic curve），是反映敏感性和特异性连续变量的综合指标，ROC曲线上每个点反映着对同一信号刺激的感受性。下图是ROC曲线例子。

![img](https://img-blog.csdn.net/20180709095422772?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTMwNjMwOTk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

横坐标：**1-Specificity**，伪正类率(False positive rate，FPR，FPR=FP/(FP+TN))，预测为正但实际为负的样本占所有负例样本的比例；

纵坐标：**Sensitivity**，真正类率(True positive rate，TPR，TPR=TP/(TP+FN))，预测为正且实际为正的样本占所有正例样本的比例。

 

 

在一个二分类模型中，假设采用逻辑回归分类器，其给出针对每个实例为正类的概率，那么通过设定一个阈值如0.6，概率大于等于0.6的为正类，小于0.6的为负类。对应的就可以算出一组(FPR，TPR)，在平面中得到对应坐标点。随着阈值的逐渐减小，越来越多的实例被划分为正类，但是这些正类中同样也掺杂着真正的负实例，即TPR和FPR会同时增大。阈值最大时，对应坐标点为(0,0)，阈值最小时，对应坐标点(1,1)。

**真正的理想情况**，TPR应接近1，FPR接近0，即图中的（0,1）点。**ROC曲线越靠拢（0,1）点，越偏离45度对角线越好****。**

**2.AUC值。**AUC (Area Under Curve) 被定义为ROC曲线下的面积，显然这个面积的数值不会大于1。又由于ROC曲线一般都处于y=x这条直线的上方，所以AUC的取值范围一般在0.5和1之间。使用AUC值作为评价标准是因为很多时候ROC曲线并不能清晰的说明哪个分类器的效果更好，而作为一个数值，对应AUC更大的分类器效果更好。
 

从AUC判断分类器（预测模型）优劣的标准：

- AUC = 1，是完美分类器，采用这个预测模型时，存在至少一个阈值能得出完美预测。绝大多数预测的场合，不存在完美分类器。
- 0.5 < AUC < 1，优于随机猜测。这个分类器（模型）妥善设定阈值的话，能有预测价值。
- AUC = 0.5，跟随机猜测一样（例：丢铜板），模型没有预测价值。
- AUC < 0.5，比随机猜测还差；但只要总是反预测而行，就优于随机猜测。

**一句话来说，AUC值越大的分类器，正确率越高。**

 

**【参考文献】**

https://www.jianshu.com/p/c61ae11cc5f6

https://www.cnblogs.com/sddai/p/5696870.html

https://blog.csdn.net/xyzx043874/article/details/54969239

https://blog.csdn.net/weeeeeida/article/details/78906570




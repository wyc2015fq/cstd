
# R语言︱分类器的性能表现评价（混淆矩阵，准确率，召回率，F1,mAP、ROC曲线） - 素质云笔记-Recorder... - CSDN博客

2016年04月10日 18:38:13[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：20642






**笔者寄语：**分类器算法最后都会有一个预测精度，而预测精度都会写一个混淆矩阵，所有的训练数据都会落入这个矩阵中，而对角线上的数字代表了预测正确的数目，即True Positive+True Nagetive。

——————————————————————————

**相关内容：**
1、[R语言︱ROC曲线——分类器的性能表现评价](http://blog.csdn.net/sinat_26917383/article/details/51114244)
2、[机器学习中的过拟合问题](http://blog.csdn.net/sinat_26917383/article/details/51615223)
3、[R语言︱机器学习模型评估方案（以随机森林算法为例）](http://blog.csdn.net/sinat_26917383/article/details/51307154)

——————————————————————————
笔者觉得在性能评价上有两个分支：
TPR-TNR，后续接AUC值/ROC曲线；
召回率-正确率，接F1值曲线，再接mAP曲线（召回-准确曲线）

本节部分参考：[二分类与多分类评估（混淆矩阵，准确率，召回率，F1,mAP）](http://mp.weixin.qq.com/s?__biz=MzA3NzQ1ODQ1OA==&mid=2653139483&idx=1&sn=32826c0f5e4547ac3f18c79945220e50&chksm=8486af6eb3f1267835d0688c3fafc2de95c2c272d0da986174eec283f43aa9c45c772549b971&mpshare=1&scene=1&srcid=0416Gqz07HhFG8IpUq3ZK22H#rd)

**1、TPR与TNR**

同时可以相应算出TPR（真正率或称为灵敏度）和TNR（真负率或称为特异度）。我们主观上希望这两个指标越大越好，但可惜二者是一个此消彼涨的关系。除了分类器的训练参数，临界点的选择，也会大大的影响TPR和TNR。有时可以根据具体问题和需要，来选择具体的临界点。

![](https://img-blog.csdn.net/20160609173545809)
来看看这个图就一目了然了，ROC曲线需要两个数据集，预测数据+实际数据。

如果我们选择一系列的临界点，就会得到一系列的TPR和TNR，将这些值对应的点连接起来，就构成了ROC曲线。ROC曲线可以帮助我们清楚的了解到这个分类器的性能表现，还能方便比较不同分类器的性能。在绘制ROC曲线的时候，习惯上是使用1-TNR作为横坐标，TPR作为纵坐标。下面来看看如何在R语言中绘制ROC曲线。

2、召回率与准确率
召回率(Recall)      =  TP/P = TPR
准确率(Precision) =  TP/（TP+FP）
召回率recall就是上述说到的TPR

**3、TPR与TNR的延伸——AUC值**

为了更好的衡量ROC所表达结果的好坏，Area Under Curve（AUC）被提了出来，简单来说就是曲线右下角部分占正方形格子的面积比例。该比例代表着分类器预测精度。

4、召回率与准确率的延伸——F1值
准确率和召回率是互相影响的，理想情况下肯定是做到两者都高，但是一般情况下准确率高、召回率就低，召回率低、准确率高，当然如果两者都低，那是什么地方出问题了。一般情况，用不同的阀值，统计出一组不同阀值下的精确率和召回率，如下图：
![](https://img-blog.csdn.net/20170417142655308)
如果是做搜索，那就是保证召回的情况下提升准确率；如果做疾病监测、反垃圾，则是保准确率的条件下，提升召回。
所以，在两者都要求高的情况下，可以用F1来衡量。
`1. F1 = 2 * P * R / (P + R)`
公式基本上就是这样，但是如何算图1中的A、B、C、D呢？这需要人工标注，人工标注数据需要较多时间且枯燥，如果仅仅是做实验可以用用现成的语料。当然，还有一个办法，找个一个比较成熟的**算法**作为基准，用该算法的结果作为样本来进行比照，这个方法也有点问题，如果有现成的很好的算法，就不用再研究了。
5、召回率、准确率、F1的延伸——AP和mAP(mean Average Precision)
mAP是为解决P，R，F-measure的单点值局限性的。为了得到 一个能够反映全局性能的指标，可以看考察下图，其中两条曲线(方块点与圆点)分布对应了两个检索系统的准确率-召回率曲线
![](https://img-blog.csdn.net/20170417142816169)
可以看出，虽然两个系统的性能曲线有所交叠但是以圆点标示的系统的性能在绝大多数情况下要远好于用方块标示的系统。
从中我们可以 发现一点，如果一个系统的性能较好，其曲线应当尽可能的向上突出。
更加具体的，曲线与坐标轴之间的面积应当越大。
最理想的系统， 其包含的面积应当是1，而所有系统的包含的面积都应当大于0。这就是用以评价信息检索系统的最常用性能指标，平均准确率mAP其规范的定义如下:(其中P，R分别为准确率与召回率)
![](https://img-blog.csdn.net/20170417143032789)

**————————————————————————————————————————————————————————**

R语言中ROC曲线的绘制


**参考以下的博客：**转载于：http://www.r-bloggers.com/lang/chinese/1205
该博客示范了三种画出ROC曲线的方法，第一种主动自己运算、第二种ROCR包、第三种pROC包。


1、自己运算

`# 做一个logistic回归，生成概率预测值
model1 <- glm(y~., data=newdata, family='binomial')
pre <- predict(model1,type='response')
# 将预测概率prob和实际结果y放在一个数据框中
data <- data.frame(prob=pre,obs=newdata$y)
# 按预测概率从低到高排序
data <- data[order(data$prob),]
n <- nrow(data)
tpr <- fpr <- rep(0,n)
# 根据不同的临界值threshold来计算TPR和FPR，之后绘制成图
for (i in 1:n) {
    threshold <- data$prob[i]
    tp <- sum(data$prob > threshold & data$obs == 1)
    fp <- sum(data$prob > threshold & data$obs == 0)
    tn <- sum(data$prob < threshold & data$obs == 0)
    fn <- sum(data$prob < threshold & data$obs == 1)
    tpr[i] <- tp/(tp+fn) # 真正率
    fpr[i] <- fp/(tn+fp) # 假正率
}
plot(fpr,tpr,type='l')
abline(a=0,b=1)`

2、ROCR包

R中也有专门用来绘制ROC曲线的包，例如常见的ROCR包，它不仅可以用来画图，还能计算ROC曲线下面积AUC，以评价分类器的综合性能，该数值取0-1之间，越大越好。

`library(ROCR)
pred <- prediction(pre,newdata$y)
performance(pred,'auc')@y.values #AUC值
perf <- performance(pred,'tpr','fpr')
plot(perf)`
注意：其中pre是分类器预测的模型，而newdata$y是实际值。

3、pROC包

ROCR包画图函数功能比较单一，笔者比较偏好使用功能更强大的pROC包。它可以方便比较两个分类器，还能自动标注出最优的临界点，图看起来也比较漂亮

`library(pROC)
modelroc <- roc(newdata$y,pre)
plot(modelroc, print.auc=TRUE, auc.polygon=TRUE, grid=c(0.1, 0.2),
     grid.col=c("green", "red"), max.auc.polygon=TRUE,
     auc.polygon.col="skyblue", print.thres=TRUE)`
——————————————————————————————————————————————
笔者在操作的时候出现以下的问题：
`Error in prediction(tablebank, bank_part$y_n) : 
Number of cross-validation runs must be equal for predictions and labels.`
网上的解决方案有：
在这种情况下预测(预测,标签,标签。 点= NULL)函数类的“预测”和“标签”变量应该列表或矩阵。

本文有两个ROC曲线绘制包，可参考。
转载于：http://www.r-bloggers.com/lang/chinese/1205

**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————



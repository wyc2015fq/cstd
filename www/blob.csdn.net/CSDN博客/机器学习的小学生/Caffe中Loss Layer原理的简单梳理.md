
# Caffe中Loss Layer原理的简单梳理 - 机器学习的小学生 - CSDN博客


2017年04月05日 09:53:27[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：319


转自：http://blog.csdn.net/liumaolincycle/article/details/50822392 [liumaolincycle的博客]
# 1.SoftmaxWithLoss
对一对多的分类任务计算多项逻辑斯蒂损失，并通过softmax传递预测值，来获得各类的概率分布。该层可以分解为SoftmaxLayer+MultinomialLogisticLossLayer，但它的梯度计算在数值上更为稳健。在[测试](http://lib.csdn.net/base/softwaretest)时，该层可用SoftmaxLayer替代。
## 前向传播
**bottom：**1.（N×C×H×W）维的预测得分x，N是batch数，类别总数为K=CHW，目前看到的SoftmaxWithLossLayer的bottom一般是一个InnerProduct层，所以K是写在一起的。该层将这些得分通过softmax函数（多项logistic回归模型）![这里写图片描述](https://img-blog.csdn.net/20160308102130566)[映射为概率分布，n∈[0, 1, …, N-1]，k、k’∈[0, 1, …, K-1]； ](https://img-blog.csdn.net/20160308102130566)
2.（N×1×1×1）维的标签l，![这里写图片描述](https://img-blog.csdn.net/20160308102308678)[，表示这个bacth中各样本的正确标签。 ](https://img-blog.csdn.net/20160308102308678)
**top：**（1×1×1×1）维，对softmax输出类别概率![这里写图片描述](https://img-blog.csdn.net/20160308102658792)[的交叉熵分类损失](https://img-blog.csdn.net/20160308102658792)![这里写图片描述](https://img-blog.csdn.net/20160308102607286)[。](https://img-blog.csdn.net/20160308102607286)
[

](https://img-blog.csdn.net/20160308102658792)
## 反向传播
[
](https://img-blog.csdn.net/20160308102658792)**top：**（1×1×1×1）维，该blob的微分就是loss_weight λ，λ是该层输出![这里写图片描述](https://img-blog.csdn.net/20160308163923086)[ 的系数，整个网络的损失为](https://img-blog.csdn.net/20160308163923086)![这里写图片描述](https://img-blog.csdn.net/20160308163959592)[ ，这样](https://img-blog.csdn.net/20160308163959592)![这里写图片描述](https://img-blog.csdn.net/20160308164107542)[。 ](https://img-blog.csdn.net/20160308164107542)
**bottom：**1.（N×C×H×W）维的预测得分x，反向计算微分![这里写图片描述](https://img-blog.csdn.net/20160308164318109)[； ](https://img-blog.csdn.net/20160308164318109)
2.（N×1×1×1）维的标签，忽略。
另外可参考一篇很好的介绍文章：[http://blog.csdn.net/u012162613/article/details/44239919](http://blog.csdn.net/u012162613/article/details/44239919)
# 2.EuclideanLoss
对**回归**任务计算欧氏距离（L2）损失![这里写图片描述](https://img-blog.csdn.net/20160308143234979)[，可用于最小二乘回归任务。](https://img-blog.csdn.net/20160308143234979)
## 前向传播
**bottom：**1.（N×C×H×W）维的预测，![这里写图片描述](https://img-blog.csdn.net/20160308161523041)[ ](https://img-blog.csdn.net/20160308161523041)
2.（N×C×H×W）维的真实结果，![这里写图片描述](https://img-blog.csdn.net/20160308161548745)[ ](https://img-blog.csdn.net/20160308161548745)
**top：**（1×1×1×1）维的欧氏距离损失：![这里写图片描述](https://img-blog.csdn.net/20160308161717028)
## 反向传播
**top：**（1×1×1×1）维，该blob的微分就是loss_weight λ，λ是该层输出![这里写图片描述](https://img-blog.csdn.net/20160308163923086)[ 的系数，整个网络的损失为](https://img-blog.csdn.net/20160308163923086)![这里写图片描述](https://img-blog.csdn.net/20160308163959592)[ ，这样](https://img-blog.csdn.net/20160308163959592)![这里写图片描述](https://img-blog.csdn.net/20160308164107542)[。 ](https://img-blog.csdn.net/20160308164107542)
**bottom：**1.（N×C×H×W）维的预测![这里写图片描述](https://img-blog.csdn.net/20160308165208291)[，反向计算微分](https://img-blog.csdn.net/20160308165208291)![这里写图片描述](https://img-blog.csdn.net/20160308165954569)[； ](https://img-blog.csdn.net/20160308165954569)
2.（N×C×H×W）维的预测![这里写图片描述](https://img-blog.csdn.net/20160308170045240)[，反向计算微分](https://img-blog.csdn.net/20160308170045240)![这里写图片描述](https://img-blog.csdn.net/20160308170103349)[。](https://img-blog.csdn.net/20160308170103349)
[

](https://img-blog.csdn.net/20160308170045240)
# 3.HingeLoss
[
](https://img-blog.csdn.net/20160308170045240)对一对多的分类任务计算铰链损失。
[

](https://img-blog.csdn.net/20160308170045240)
## 前向传播
[
](https://img-blog.csdn.net/20160308170045240)**bottom：**1.（N×C×H×W）维的预测t，blob值的范围是![这里写图片描述](https://img-blog.csdn.net/20160308172826979)[，表示对K=CHW中每个类的预测得分。在SVM中，](https://img-blog.csdn.net/20160308172826979)![这里写图片描述](https://img-blog.csdn.net/20160308175432887)[是D维特征](https://img-blog.csdn.net/20160308175432887)![这里写图片描述](https://img-blog.csdn.net/20160308173107372)[和超平面参数](https://img-blog.csdn.net/20160308173107372)![这里写图片描述](https://img-blog.csdn.net/20160308173150050)[的内积](https://img-blog.csdn.net/20160308173150050)![这里写图片描述](https://img-blog.csdn.net/20160308173227300)[，因此只有一个InnerProductLayer（num_output = D）提供预测到HingeLossLayer中的网络就相当于一个SVM； ](https://img-blog.csdn.net/20160308173227300)
2.（N×1×1×1）维的真实标签l，![这里写图片描述](https://img-blog.csdn.net/20160308174632689)[。 ](https://img-blog.csdn.net/20160308174632689)
**top：**（1×1×1×1）维的铰链损失：![这里写图片描述](https://img-blog.csdn.net/20160308174904822)[，对应于](https://img-blog.csdn.net/20160308174904822)![这里写图片描述](https://img-blog.csdn.net/20160308175018336)[正规化，默认是L1正规化，也可以用L2正规化，](https://img-blog.csdn.net/20160308175018336)![这里写图片描述](https://img-blog.csdn.net/20160308175318777)[。](https://img-blog.csdn.net/20160308175318777)
[

](https://img-blog.csdn.net/20160308175018336)
## 反向传播
[
](https://img-blog.csdn.net/20160308175018336)**top：**（1×1×1×1）维，该blob的微分就是loss_weight λ，λ是该层输出![这里写图片描述](https://img-blog.csdn.net/20160308163923086)[ 的系数，整个网络的损失为](https://img-blog.csdn.net/20160308163923086)![这里写图片描述](https://img-blog.csdn.net/20160308163959592)[ ，这样](https://img-blog.csdn.net/20160308163959592)![这里写图片描述](https://img-blog.csdn.net/20160308164107542)[。 ](https://img-blog.csdn.net/20160308164107542)
**bottom：**1.（N×C×H×W）维的预测得分t，反向计算微分![这里写图片描述](https://img-blog.csdn.net/20160308181237628)[； ](https://img-blog.csdn.net/20160308181237628)
2.（N×1×1×1）维的标签，忽略。
[

](https://img-blog.csdn.net/20160308181237628)
# 4.SigmoidCrossEntropyLoss
[
](https://img-blog.csdn.net/20160308181237628)计算交叉熵（逻辑斯蒂）损失![这里写图片描述](https://img-blog.csdn.net/20160308182816665)[，通常用于以概率形式预测目标。该层可以分解为SigmoidLayer+CrossEntropyLayer，但它的梯度计算在数值上更为稳健。在测试时，该层可用SigmoidLayer替代。](https://img-blog.csdn.net/20160308182816665)
## 前向传播
**bottom：**1.（N×C×H×W）维的预测得分![这里写图片描述](https://img-blog.csdn.net/20160308183138354)[，是用sigmoid函数](https://img-blog.csdn.net/20160308183138354)![这里写图片描述](https://img-blog.csdn.net/20160308183238561)[的概率预测](https://img-blog.csdn.net/20160308183238561)![这里写图片描述](https://img-blog.csdn.net/20160308183330516)[； ](https://img-blog.csdn.net/20160308183330516)
2.（N×C×H×W）维的真实结果，![这里写图片描述](https://img-blog.csdn.net/20160308183420683)[。 ](https://img-blog.csdn.net/20160308183420683)
**top：**（1×1×1×1）维的交叉熵损失：![这里写图片描述](https://img-blog.csdn.net/20160308183524378)[。](https://img-blog.csdn.net/20160308183524378)
## 反向传播
**top：**（1×1×1×1）维，该blob的微分就是loss_weight λ，λ是该层输出![这里写图片描述](https://img-blog.csdn.net/20160308163923086)[ 的系数，整个网络的损失为](https://img-blog.csdn.net/20160308163923086)![这里写图片描述](https://img-blog.csdn.net/20160308163959592)[ ，这样](https://img-blog.csdn.net/20160308163959592)![这里写图片描述](https://img-blog.csdn.net/20160308164107542)[。 ](https://img-blog.csdn.net/20160308164107542)
**bottom：**1.（N×C×H×W）维的预测得分x，反向计算微分![这里写图片描述](https://img-blog.csdn.net/20160308183726458)[； ](https://img-blog.csdn.net/20160308183726458)
2.（N×1×1×1）维的标签，忽略。
[

](https://img-blog.csdn.net/20160308183726458)
# 5.MultinomialLogisticLossLayer
[
](https://img-blog.csdn.net/20160308183726458)对一对多的分类任务计算多项逻辑斯蒂损失，直接将预测的概率分布作为输入。当预测并不是概率分布时应该用SoftmaxWithLossLayer，因为它在计算多项逻辑斯蒂损失前通过SoftmaxLayer将预测映射为分布。
[
](https://img-blog.csdn.net/20160308183726458)
## 前向传播
[
](https://img-blog.csdn.net/20160308183726458)**bottom：**1.（N×C×H×W）维的预测得分![这里写图片描述](https://img-blog.csdn.net/20160308184229749)[，blob值的范围是[0, 1]，表示对K=CHW个类中每个类的预测概率。每个预测向量](https://img-blog.csdn.net/20160308184229749)![这里写图片描述](https://img-blog.csdn.net/20160308184506688)[之和为1，](https://img-blog.csdn.net/20160308184506688)![这里写图片描述](https://img-blog.csdn.net/20160308184613391)[； ](https://img-blog.csdn.net/20160308184613391)
2.（N×1×1×1）维的标签l，![这里写图片描述](https://img-blog.csdn.net/20160308102308678)[，表示这个bacth中各样本的正确标签。 ](https://img-blog.csdn.net/20160308102308678)
**top：**（1×1×1×1）维的多项逻辑斯蒂损失![这里写图片描述](https://img-blog.csdn.net/20160308184818736)[。](https://img-blog.csdn.net/20160308184818736)
## 反向传播
**top：**（1×1×1×1）维，该blob的微分就是loss_weight λ，λ是该层输出![这里写图片描述](https://img-blog.csdn.net/20160308163923086)[ 的系数，整个网络的损失为](https://img-blog.csdn.net/20160308163923086)![这里写图片描述](https://img-blog.csdn.net/20160308163959592)[ ，这样](https://img-blog.csdn.net/20160308163959592)![这里写图片描述](https://img-blog.csdn.net/20160308164107542)[。 ](https://img-blog.csdn.net/20160308164107542)
**bottom：**1.（N×C×H×W）维的预测得分![这里写图片描述](https://img-blog.csdn.net/20160308220121534)[，反向计算微分](https://img-blog.csdn.net/20160308220121534)![这里写图片描述](https://img-blog.csdn.net/20160308220145050)[； ](https://img-blog.csdn.net/20160308220145050)
2.（N×1×1×1）维的标签，忽略。
[

](https://img-blog.csdn.net/20160308220145050)
# 6.InfogainLoss
[
](https://img-blog.csdn.net/20160308220145050)是MultinomialLogisticLossLayer的泛化，利用“information gain”（infogain）矩阵指定所有**标签对**的“value“，如果infogain矩阵一致则与MultinomialLogisticLossLayer等价。
[

](https://img-blog.csdn.net/20160308220145050)
## 前向传播
[
](https://img-blog.csdn.net/20160308220145050)**bottom：**1.（N×C×H×W）维的预测得分![这里写图片描述](https://img-blog.csdn.net/20160308184229749)[，blob值的范围是[0, 1]，表示对K=CHW个类中每个类的预测概率。每个预测向量](https://img-blog.csdn.net/20160308184229749)![这里写图片描述](https://img-blog.csdn.net/20160308184506688)[之和为1，](https://img-blog.csdn.net/20160308184506688)![这里写图片描述](https://img-blog.csdn.net/20160308184613391)[； ](https://img-blog.csdn.net/20160308184613391)
2.（N×1×1×1）维的标签l，![这里写图片描述](https://img-blog.csdn.net/20160308102308678)[，表示这个bacth中各样本的正确标签； ](https://img-blog.csdn.net/20160308102308678)
3.（1×1×K×K）维的infogain矩阵H（相应的另一个为I），若H=I则该层等价于MultinomialLogisticLossLayer。
**top：**（1×1×1×1）维的infogain多项逻辑斯蒂损失![这里写图片描述](https://img-blog.csdn.net/20160308221243943)[，](https://img-blog.csdn.net/20160308221243943)![这里写图片描述](https://img-blog.csdn.net/20160308221327193)[指的是H的第](https://img-blog.csdn.net/20160308221327193)![这里写图片描述](https://img-blog.csdn.net/20160308221359988)[行。](https://img-blog.csdn.net/20160308221359988)
[

](https://img-blog.csdn.net/20160308221327193)
## 反向传播
[
](https://img-blog.csdn.net/20160308221327193)**top：**（1×1×1×1）维，该blob的微分就是loss_weight λ，λ是该层输出![这里写图片描述](https://img-blog.csdn.net/20160308163923086)[ 的系数，整个网络的损失为](https://img-blog.csdn.net/20160308163923086)![这里写图片描述](https://img-blog.csdn.net/20160308163959592)[ ，这样](https://img-blog.csdn.net/20160308163959592)![这里写图片描述](https://img-blog.csdn.net/20160308164107542)[。 ](https://img-blog.csdn.net/20160308164107542)
**bottom：**1.（N×C×H×W）维的预测得分![这里写图片描述](https://img-blog.csdn.net/20160308220121534)[，反向计算微分](https://img-blog.csdn.net/20160308220121534)![这里写图片描述](https://img-blog.csdn.net/20160308220145050)[； ](https://img-blog.csdn.net/20160308220145050)
2.（N×1×1×1）维的标签，忽略；
3.（1×1×K×K）维的infogain矩阵，忽略。
# 7.ContrastiveLoss
计算对比损失![这里写图片描述](https://img-blog.csdn.net/20160308221617882)[，其中](https://img-blog.csdn.net/20160308221617882)![这里写图片描述](https://img-blog.csdn.net/20160308221639148)[，可用于训练siamese网络。](https://img-blog.csdn.net/20160308221639148)
[

](https://img-blog.csdn.net/20160308221617882)
## 前向传播
[
](https://img-blog.csdn.net/20160308221617882)**bottom：**1.（N×C×1×1）维的特征![这里写图片描述](https://img-blog.csdn.net/20160308221852494)[； ](https://img-blog.csdn.net/20160308221852494)
2.（N×C×1×1）维的特征![这里写图片描述](https://img-blog.csdn.net/20160308221927666)[； ](https://img-blog.csdn.net/20160308221927666)
3.（N×C×1×1）维的二元相似度![这里写图片描述](https://img-blog.csdn.net/20160308222006852)[。 ](https://img-blog.csdn.net/20160308222006852)
**top：**（1×1×K×K）维的对比损失![这里写图片描述](https://img-blog.csdn.net/20160308222104075)[。](https://img-blog.csdn.net/20160308222104075)
## 反向传播
**top：**（1×1×1×1）维，该blob的微分就是loss_weight λ，λ是该层输出![这里写图片描述](https://img-blog.csdn.net/20160308163923086)[ 的系数，整个网络的损失为](https://img-blog.csdn.net/20160308163923086)![这里写图片描述](https://img-blog.csdn.net/20160308163959592)[ ，这样](https://img-blog.csdn.net/20160308163959592)![这里写图片描述](https://img-blog.csdn.net/20160308164107542)[。 ](https://img-blog.csdn.net/20160308164107542)
**bottom：**1.（N×C×1×1）维的特征a；
2.（N×C×1×1）维的特征b。
[

](https://img-blog.csdn.net/20160308164107542)
[            ](https://img-blog.csdn.net/20160308163959592)
[
						](https://img-blog.csdn.net/20160308163923086)
[
	](https://img-blog.csdn.net/20160308163923086)

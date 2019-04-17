# Caffe学习（六）损失层及其参数设置 - BryantLJ学习的地方 - CSDN博客





2016年08月07日 19:29:19[遍地流金](https://me.csdn.net/u012177034)阅读数：15057








机器学习的目的就是通过对训练样本输出与真实值不一致的进行惩罚，得到损失Loss，然后采用一定的优化算法对loss进行最小优化，进而得到合理的网络权值。本文介绍Caffe中含有的常见的LossLayer及其参数设置方法

Caffe的LossLayer主要由6个：


**（1）ContrastiveLossLayer**

对比损失函数主要用来训练siamese网络，改网络为专门处理同时输入两张图像的网络，如用于FaceVerification的DeepID网络。在改网络的最后需要对比两张图像是否为一张人脸，因此需要一个对比损失函数。目标检测暂时不会用到

**(2)EuclideanLossLayer**

适合回归任务，尤其输出值为实数时。

输入blob向量：长度为2，两个（N，C，H，W）维的prediction与groundTruth blob，N张图像，在这里的CHW应该都是1.

输出blob：预测值与真实值的均方误差E，实数

![](http://latex.codecogs.com/gif.latex?E%3D%5Cfrac%7B1%7D%7B2N%7D%5Csum%20%7C%7Cpred-truth%7C%7C_%7B2%7D%5E%7B2%7D)


**(3)HingeLossLayer**

按损失函数，适用于多分类任务。

输入blob向量：长度为2，包括

                      预测的score值：（N，C，H，W）维，代表N个样本每一个样本在C类中的每一类上的得分。其中HW应该都为1

                      真实值：（N，1,1,1），样本的真实Label


输出blob：损失值E

![](http://latex.codecogs.com/gif.latex?E%3D%5Cfrac%7B1%7D%7BN%7D%5Csum_%7Bn%3D1%7D%5E%7BN%7D%5Csum_%7Bk%3D1%7D%5E%7BK%7D%28max%280%2C1-%5Csigma%28label%3D%3Dk%29Score_%7Bnk%7D%29%29%5E%7Bp%7D)


其中![](http://latex.codecogs.com/gif.latex?%5Csigma%28Condition%29%3D%5Cleft%5C%7B%5Cbegin%7Bmatrix%7D%20%5C1%2CCondition%20%3D%3D%20true%20%5C%5C-1%2CCondition%3D%3Dfalse%20%5Cend%7Bmatrix%7D%5Cright.)


其中![](http://latex.codecogs.com/gif.latex?Score_%7Bnk%7D)表示第n个样本在第k类上的得分，p表示L范数的阶，默认为1范数。可取2范数。

由SVM的原理可知一层全连接网络+HingeLossLayer等价于一个线性SVM，2范数Hinge损失的SVM也叫做L2-SVM


**(4)InfogainLossLayer**

信息增益损失。MultinomialLogisticLoss的泛化版本，不仅仅接受预测的每个样本在每类上的概率信息，还接受信息增益矩阵信息。当信息增益矩阵为单位阵的时候两者是等价的。

输入blob：

             预测概率值：（N,C,H,W）维，其中HW应该为1

             真实标签：(N,1,1,1)维

             信息增益矩阵H(可选，默认为单位阵)：（1,1，K,K）维，其中K为类别的个数


输出blob：误差损失E

![](http://latex.codecogs.com/gif.latex?E%3D-%5Cfrac%7B1%7D%7BN%7D%5Csum_%7Bn%3D1%7D%5E%7BN%7DH_%7Blabel_%7Bn%7D%7DlogP_%7Bn%7D%3D-%5Cfrac%7B1%7D%7BN%7D%5Csum_%7Bn%3D1%7D%5E%7BN%7D%5Csum%20_%7Bk%3D1%7D%5E%7BK%7DH_%7Blabel_%7Bn%7D%2Ck%7DlogP_%7Bn%2Ck%7D)


**(5)MultinomialLogisticLossLayer**

多项逻辑损失，主要处理多分类问题。当输出是每一类的概率时用这个损失函数。

输入blob：

            预测概率值：每一个样本在每一类的概率，（N,C,H,W）维，N为样本个数，C为类别个数，HW应该均为1

            真实标签：（N，1,1,1）维

输出blob：误差损失E，其中![](http://latex.codecogs.com/gif.latex?p_%7Bn%2Clabel%7D)表示估计的样本n为其真实标签label的概率

![](http://latex.codecogs.com/gif.latex?E%3D-%5Cfrac%7B1%7D%7BN%7D%5Csum_%7Bn%3D1%7D%5E%7BN%7Dlog%28p_%7Bn%2Clabel%7D%29)


**（5）SoftmaxLossLayer**

等价于softmaxLayer+MultinomialLogisticLossLayer。在训练时用该层，测试时用softmax层。为什么要多添加一个SoftmaxLossLayer，而不是用Softmax+MltinomialLogisticLayer来做？主要原因是直接采用这种SoftmaxLoss计算梯度时在数值计算上更稳定！！！具体原因参考[点击打开链接](http://freemind.pluskid.org/machine-learning/softmax-vs-softmax-loss-numerical-stability/)

输入blob：每个样本在每个类别的得分

输出blob：误差损失E，形式同多项逻辑损失，只不过估计概率P的来源不一样


**（6）SigmoidCrossEntroyLossLayer**


Sigmoid交叉熵损失，主要用于二分类问题。

等价于SigmoidLayer+CrossEntropyLayer，第一层先将score转为概率输入**（为什么不是softmaxLayer转换为概率呢？？？对于二分类问题用Sigmoid函数计算出一个概率即可，多分类问题用softmax函数）**，第二层计算交叉熵损失.采用一个新层而不是由两个已有的层来实现，主要原因也是由于数值稳定性


输入blob：每个样本在每一类上的得分。（N,C,H,W）维，其中HW应该为1。

输出blob：误差损失E，其中Pn表示估计的该类为类1的概率，labeln为样本n的类别（0或者1）。

![](http://latex.codecogs.com/gif.latex?E%3D-%5Cfrac%7B1%7D%7BN%7D%5Csum_%7Bn%3D1%7D%5E%7BN%7D%5Blabel_%7Bn%7Dlog%28p_%7Bn%7D%29&plus;%281-label_%7Bn%7D%29log%281-p_%7Bn%7D%29%5D)


由E的定义可知，**该E其实等价于MultinomialLogisticLoss中的二分类情况**





最后来个提醒，Caffe中,默认每一层都可以作为LossLayer，不过层的名字中不带Loss的层其"Loss_weight"属性默认为0，即对最后的总的Loss贡献为0。名字中带Loss的层可以在网络的任意位置使用（不一定非在网络的输出最后部分），且可以有不止一个LossLayer，最后的训练所使用的Loss为所有LossLayer损失的加权和，即按下式计算：



```
loss := 0
for layer in layers:
  for top, loss_weight in layer.tops, layer.loss_weights:
    loss += loss_weight * sum(top)
```








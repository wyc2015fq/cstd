# 决策树Decision Tree - zkq_1986的博客 - CSDN博客





2016年09月19日 20:32:50[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：246
个人分类：[Machine Learning](https://blog.csdn.net/zkq_1986/article/category/6374102)









## 1 决策树流程

决策树遵循的是“分而治之”的策略流程。 
![这里写图片描述](https://img-blog.csdn.net/20160919202847848)
## 2 信息熵

信息熵(information entropy)，是衡量事物不确定的指标。 


$\displaystyle \mathrm {Ent} (D)=-\sum _{k=1}^{|y|}{\mathrm {p_klog_2p_k} }$

在信息增益度量中，熵值越小越好。
## 3 CART决策树

CART,Classification And Regression Tree，分类回归树，分类和回归都可用。$p_k$表示第k个类个数占所有个数的比例。 
![这里写图片描述](https://img-blog.csdn.net/20160920202750209)

ID3采用信息增益率。
## 4 剪枝

剪枝是为了防止过拟合，提高泛化能力。分为“预剪枝”和“后剪枝”。

### 4.1 预剪枝

预剪枝是指，在构建树的时候判断是否对节点进行分支。判断是否对节点进行分支的准则就是确定其分支是否能够带来泛化性能的提升。泛化性能判断的做法就是用测试集确定。 

在划分之前，所有样例集中在根节点。若不进行划分，则该节点被标记为叶节点，其类别标记为训练样例最多的类别，假设我们 
![这里写图片描述](https://img-blog.csdn.net/20160923100705631)
![这里写图片描述](https://img-blog.csdn.net/20160923100719580)
### 4.2 后剪枝

后剪枝是指，先按训练集构建整棵完整的树，然后从最后一个非叶子节点开始通过测试集验证其剪枝后是否能提高泛化性能，方法类似预剪枝。

## 5 C4.5

在属性值缺失情况下，C4.5通过已知属性值个数占全部样本数的比例这种形式来计算属性的信息增益。信息增益最小的，就选那个属性作为当前树节点。 
![这里写图片描述](https://img-blog.csdn.net/20161008114916802)
![这里写图片描述](https://img-blog.csdn.net/20161008114856036)
![这里写图片描述](https://img-blog.csdn.net/20161008114927427)
![这里写图片描述](https://img-blog.csdn.net/20161008114934978)
![这里写图片描述](https://img-blog.csdn.net/20161008114944509)
![这里写图片描述](https://img-blog.csdn.net/20161008114951744)
![这里写图片描述](https://img-blog.csdn.net/20161008114959978)
![这里写图片描述](https://img-blog.csdn.net/20161008115009900)
## 6 实验

数据集：  Iris，150条记录

============================ 

c4.5 

                TP Rate   FP Rate   Precision   Recall  F-Measure   ROC Area  Class 

                 0.98      0          1         0.98      0.99       0.99     Iris-setosa 

                 0.94      0.03       0.94      0.94      0.94       0.952    Iris-versicolor 

                 0.96      0.03       0.941     0.96      0.95       0.961    Iris-virginica 

Weighted Avg.    0.96      0.02       0.96      0.96      0.96       0.968
======================== 

naive bayes 

                TP Rate   FP Rate   Precision   Recall  F-Measure   ROC Area  Class 

                 1         0          1         1         1          1        Iris-setosa 

                 0.96      0.04       0.923     0.96      0.941      0.992    Iris-versicolor 

                 0.92      0.02       0.958     0.92      0.939      0.992    Iris-virginica 

Weighted Avg.    0.96      0.02       0.96      0.96      0.96       0.994
===================== 

logit regression 

                TP Rate   FP Rate   Precision   Recall  F-Measure   ROC Area  Class 

                 1         0          1         1         1          1        Iris-setosa 

                 0.92      0.02       0.958     0.92      0.939      0.97     Iris-versicolor 

                 0.96      0.04       0.923     0.96      0.941      0.975    Iris-virginica 

Weighted Avg.    0.96      0.02       0.96      0.96      0.96       0.982
================== 

bp 神经网络 

               TP Rate   FP Rate   Precision   Recall  F-Measure   ROC Area  Class 

                 1         0          1         1         1          1        Iris-setosa 

                 0.96      0.02       0.96      0.96      0.96       0.996    Iris-versicolor 

                 0.96      0.02       0.96      0.96      0.96       0.996    Iris-virginica 

Weighted Avg.    0.973     0.013      0.973     0.973     0.973      0.998
bp神经网络效果最好。













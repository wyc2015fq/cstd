# HMM简单理解 - xmdxcsj的专栏 - CSDN博客





2015年10月01日 22:23:54[xmucas](https://me.csdn.net/xmdxcsj)阅读数：3673








## 模型导出

贝叶斯的输出变量只有一个，如果将输出变量y加以扩展成序列标注，也就是每一个Xi对应于一个标记Yi，贝叶斯模型转化为：

![](https://img-blog.csdn.net/20151001222102608?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

考虑到输出y之间的相互依赖性，联合概率可以分解为生成概率和转移概率的乘积。

缺点：虽然考虑了y之间的依赖性，但是仍然假设x之间是相互独立的。

HMM用于标记的时候，隐藏变量就是词序列对应的label标记，观测序列是词序列，所求的就是基于各类标记之间的转移概率，以及某一个标记对应的词的生成概率。

HMM的假设有两点：

1.      齐次马尔科夫性假设：任意时刻的状态只与前一时刻的状态有关，与其他时刻的状态和观测无关，对应于转移概率矩阵

2.      观测独立性假设：任意时刻的观测至于该时刻的状态有关，与其他观测和状态无关，对应于生成概率

## 概率计算问题

主要使用前向和后向算法。

计算出的前向变量和后向变量用于EM算法E步更新统计量。

## 学习问题

1.      监督学习

比如标注问题，已知观测和状态（标注）一一对应，所以属于监督学习。

比较简单，根据转移概率和生成概率的定义直接进行数数相除就可以得到对应的概率分布。

2.      非监督学习

比如语音识别的hmm训练，无法将一帧MFCC特征对应到一个状态，所以就涉及到无监督学习。使用Baum-Welch算法（EM算法）：

E步：

计算统计量，类似于GMM训练需要计算属于某个高斯的概率，此处需要计算t时刻的观察量属于状态i的概率：

![](https://img-blog.csdn.net/20151001222137530?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

t时刻的观察量属于状态i，t+1时刻观察量输入状态j的转移概率：

![](https://img-blog.csdn.net/20151001222157703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

用到了前向变量和后向变量

M步：

使用上面的两个统计量更新模型参数，分别如下：

![](https://img-blog.csdn.net/20151001222218431?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



![](https://img-blog.csdn.net/20151001222225998?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 预测问题

维特比算法

## 语音识别中的hmm

语音识别使用hmm对声学模型建模，观测值对应于一帧特征数据（比如39维的mfcc特征），每一个三音素对应5个状态，每个状态有一个或多个高斯模型混合而成型，所以，hmm需要估计参数包括：状态之间的转移概率、状态对应高斯模型的均值和方差。

Hmm参数估计的方法有两种：

1.      使用类似于kmeans聚类的方法

l  首先，将一条语音的所有帧均分到标注对应的状态上面，然后可以得到每一个状态对应的所有帧，对该状态对应所有帧求平均就可以得到该状态的模型参数-均值和方差。

l  然后，使用维特比解码对所有的语音数据进行解码，对齐训练语音中每一帧对应的状态，然后再次使用上一步的方法更新模型参数。

l  一直循环，直到模型参数不再发生变化。

2.      使用Baum-Welch

E步：估计统计量-t时刻对应状态j的概率Lj(t)，也就是将t时刻的该帧对应分配给j状态

![](https://img-blog.csdn.net/20151001222301209?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

使用的是前向和后向概率进行计算。

M步：更新模型的均值和方差

![](https://img-blog.csdn.net/20151001222313046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## EM、HMM和GMM的关系

EM是一种求解目标函数的算法，属于无监督学习（隐藏变量没有标注），可以用于训练HMM或者GMM。

HMM是一种基于X和Y独立性假设的抽象模型，可以对问题进行建模。基于其状态是否已知，可以分为有监督学习（标注问题）和无监督学习（语音识别）。

GMM是对数据分布的一种描述，HMM的一个状态对应的生成概率可以使用GMM来建模。



## 参考文献

《统计学习方法》李航

ClassicalProbabilistic Models and ConditionalRandom Fields

HTK book




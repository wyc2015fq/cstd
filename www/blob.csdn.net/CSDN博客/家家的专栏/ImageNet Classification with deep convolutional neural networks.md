# ImageNet Classification with deep convolutional neural networks - 家家的专栏 - CSDN博客





2014年05月25日 19:38:35[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：22856








分析paper：ImageNet Classification with deep convolutional neural networks 

主要是**在imagenet上训练一个CNN网络。**Caffe的初始化便是这个我网络设置。

因为想更清楚的了解Caffe里面的网络结构设置，所以来研究这篇paper。。。




基本网络构成：

60millon 个参数，650 thousand 个神经元，

包含5个卷积层，其中有几个层后跟着max-pooling层，

有3个全连接层 和 1000-way softmax.




用到的两个策略“：

为了加速，使用non-saturating neurons + GPU

为了减少全连接层中的overfitting，利用一种最近发现的规则化方法：**dropout**




数据库的说明：

Imagenet数据库中的图像大小是不一致的， 统一把图像变换为固定大小：256*256。减均值。

   （具体来说：统一把图像短的一边，规则化到256，然后剪切出256*256的中间patch）




网络架构及其策略：

8层（5个卷积层+3个全连接层）



**ReLU Nonlinearity :   **instead ![](https://img-blog.csdn.net/20140525184127625)![](https://img-blog.csdn.net/20140525184253390) with ![](https://img-blog.csdn.net/20140525184121484)



**Training on Multiple GPUs**

**Local Response Normalization: ![](https://img-blog.csdn.net/20140525184946046)(**the sum over n kernel maps on the same spatial position.)




**Overlapping pooling: (in general: **summarize the outputs of neighboring groups of neurons in the same kernel map, in this**paper**,**overlapping pooling)**



**网络架构图如下：**

**![](https://img-blog.csdn.net/20140525182821015)**

**![](https://img-blog.csdn.net/20140526110624640)**

**![](https://img-blog.csdn.net/20140808110431875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWloYWl6aGl5YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**

**网络架构解读（Fig.2）：**

网络总共有8层（每一层都带有weight）； 

前五层是卷积层，后三层是全连接层。最后一个全连接层，输入到一个1000-way softmax，产生一个在1000类别上一个分布。

网络的目标:   最大化multinomial logostic regression objective，

                        等价于最大化the **average **across training cases of the log-probability of the correct label under the**prediction distribution**. 




第2 4 5 卷积层的kernels，只与位于同一块GPU上的前一层相连接。

第3个卷积层的kernels，与第2层上的所有的kernel maps相连接。

全连接层中的neurons，与前一层中的所有neurons相连接。

ReLU Nonlinearity :    应用在所有层。



第1 2层采用了response normalization策略。

第1 2 5 层采用了max-pooling策略。





第1层是作用在224*224*3的输入图像上，96个kernels（大小：11*11*3），步长：4 pixels

第2层是 256 kernels（大小：5*5*48）

第3 4 5层没有利用任何intervening的pooling或者normalization



第3层有384kernels（大小：3*3*256），其输入是第二层normalizated and pooled的输出。

第4层是384kernels (大小：3*3*192)

第5层是256 kernel (大小：3*3*192)

全连接层有4096个neurons。 







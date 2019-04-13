
# Reducing BERT Pre-Training Time from 3 Days to 76 Minutes - lpty的博客 - CSDN博客

2019年04月09日 11:24:08[lpty](https://me.csdn.net/sinat_33741547)阅读数：44



# 引言
Bert在工业使用中表现相当优异，但是预训练时间非常的长。Bert的作者公开的记录是用了16块TPU训练了三天。
本论文就是针对训练时间上进行的优化，提出了LAMB优化方式，在不损失训练精度的情况下，将BERT的训练时间缩短到了76分钟，如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190408165242102.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9scHR5LW5scC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
# 背景
从上图可以看到，论文作者提出的方法用了1024块TPU，训练时间为76分钟，F1值相对原始的Bert训练有了微小的提升。简单来看，这个缩短的训练时间刚好是TPU数量线性相关的，差不多是64倍，那么加速模型训练只是简单的叠加硬件吗？
实际上并不是，大批量的训练会导致模型结果变差。作者列举了一些例子：
> For example, by using a batch size of 512, AlexNet (13) is able to achieve 80+% top-5 testing accuracy for ImageNet training.

> A straight forward approach may only get 50% ∼ 60% top-5 accuracy or the divergence results when people scale the batch size beyond 4096.

> 造成上述结果的原因认为是大批量训练会存在泛化误差，模型更容易在局部极小点收敛，而长时间的梯度下降过程有助于模型寻找到一个更优的收敛位置。

> 前面有许多工作针对上述情况进行试验，通过设计超参数比如学习率和动量等方式，在一些数据集上实现了大批量训练的目标，如：

> To the best of our knowledge, the fastest training result for 76+%

> top-1 accuracy is achieved by Ying et al. (20). By using the LARS

> optimizer (21) to scale the batch size to 32K, Ying et al. (20) are

> able to finish the ImageNet training with ResNet-50 in 2.2 minutes on

> a TPUv3 Pod.

> 目前神经网络的训练方式大部分基于SGD，可以参考博文：
> [从Stochastic Gradient Descent到Adaptive Moment Estimation](https://blog.csdn.net/sinat_33741547/article/details/87367996)
> ，可以简单的认为是下述形式：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190408172701310.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9scHR5LW5scC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
> Krizhevsky等介绍了一种在大批量下调整LR的方法，当batch_size的大小提升K倍时，相对的提升LR为
$$
\sqrt k
$$
> 大小，这样能够保持梯度的优化方差在同一个等级，同时也提出了LR的线性增长比平方根增长效果会更好，即将LR提升为
$$
k
$$
> 倍大小。

> 但是上述方式会造成LR比较大，初始数值太大的学习率会导致收敛的不稳定。Goyal等人提出learning rate warm-up的方式:：

> training starts with a smaller LR and then gradually increases the LR to a larger value.

> 通过上述两种方法，能够在8K大小的bath_size上训练ResNet-50而不造成模型效果下降。还有许多的研究人员针对上述问题做了很多工，其中效果比较好有Ying提出的LARS优化器，在一个TPU上用2.2分钟完成了ResNet-50的训练，达到了76+% top-1 accuracy。

> LAMB

> LAMB，即Layer-wise Adaptive Moments optimizer for Batch training，优化算法的具体过程如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190408175110546.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9scHR5LW5scC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
> 可以看到，LAMB基于AdamW改造而来，区别在于学习率的自适应调整方式，作者在论文提到参考了LARS的计算方式：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409110959146.PNG)
> 但是在Adam或者AdamW中上述方式效果可能会下降，因此做了修改：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409111200202.PNG)
> 结合AdamW的权重衰减就是上述LAMB优化器形式了。

> [
](https://img-blog.csdnimg.cn/20190409111200202.PNG)
> 结果

> [
](https://img-blog.csdnimg.cn/20190409111200202.PNG)
> 作者用32k的batch_size，基于LAMB优化器进行了 15625 次迭代，得到了 91.460 的 F1 分数，大约用时 100 分钟，取得了 76.7% 的弱可扩展性效率，即用64倍的资源取得了49.1的提速；

> 为了充分利用硬件资源，作者使用混合批次训练：

> BERT 预训练主要分为两部分：

> 1）前面 9/10 的 Epoch 使用 128 的序列长度；

> 2）最后 1/10 的 Epoch 使用 512 的序列长度进行训练。

> 对于第二阶段而言，因为内存限制，TPUv3 Pod 上最大的批量大小为 32768，因此第二阶段使用的批大小为 32768。

> 对于第一阶段，受限于内存，TPUv3 Pod 上最大的批量大小为 131072。然而，研究者将批大小从 65536 增加到131072 时，并没有发现明显的加速，因此研究者将阶段 1 的批大小定为 65536。

> 最终用时 76 分钟，达到了 101.8% 的弱可扩展性效率。

> 结论

> 提出来一个新型的优化器，在其他数据集上也有一定效果，但具体为什么这个方法能够work，作者没有更多的解释，文章最后提到还在分析，关注后续工作。

> 针对Bert做提速的文章，目前来说是训练速度最快的，但也可以看到，这个训练速度需要的是更多的计算资源，不是像谷歌这样的大公司还真的玩不起。

> 引用

> 1、Reducing BERT Pre-Training Time from 3 Days to 76 Minutes



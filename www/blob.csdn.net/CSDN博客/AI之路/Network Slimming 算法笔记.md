# Network Slimming 算法笔记 - AI之路 - CSDN博客





2018年04月17日 07:52:20[AI之路](https://me.csdn.net/u014380165)阅读数：1385








论文：Learning Efficient Convolutional Networks Through Network Slimming 

链接：[https://arxiv.org/abs/1708.06519](https://arxiv.org/abs/1708.06519)

原代码（Torch实现）：[https://github.com/liuzhuang13/slimming](https://github.com/liuzhuang13/slimming)

第三方代码（PyTorch实现）：[https://github.com/foolwood/pytorch-slimming](https://github.com/foolwood/pytorch-slimming)
这篇是ICCV2017的文章，通过剪枝来做模型压缩和加速。我们知道在剪枝系列算法中，主要的不同点在于如何评价卷积核或者feature map的重要性，一般都是通过额外增加一些对原网络层的计算得到卷积核或feature map的重要性信息，**而这篇文章的剪枝比较有特色，因为其通过网络原有的Batch Normalization层的γ参数进行feature map重要性评估（由BN层γ参数的含义可知feature map的每个channel对应一个γ值，所以这个γ值相当于该channel的重要性），然后将重要性低于某个阈值的feature map剪掉，剪掉feature map相当于生成这些feature map的对应卷积核也被剪掉，这就完成了剪枝的过程**，如Figure1所示。factors那一列就是BN层的γ参数，其中比较小的值对应的feature map就会被剪掉（橙色）。 
![这里写图片描述](https://img-blog.csdn.net/20180417075048732?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
当然这里有两个注意的点：1、如果不对BN层的γ参数做一些限制，也就是正常情况下其值是很分散了，那样剪枝的效果就会很差，可以看Figure4的第一个图，λ=0表示不对γ做限制情况下γ值的分布情况。所以作者在原来的损失函数中增加了一项对网络所有BN层的γ参数的L1范数限制，使得γ更加稀疏，如公式1所示。公式1中加号左边部分是原来网络的损失函数，加号右边是增加的γ的L1范数，也就是向量γ的绝对值和，λ是两个loss之间的权重值。增加了γ的L1范数后的γ值分布就如Figure4中第二个和第三个图所示，比如在Figure4中第三个图是λ=0.0001时的γ参数分布，大部分γ都是0值，因此当把这些0值对应的feature map剪掉对网络结果的影响就很小。 
![这里写图片描述](https://img-blog.csdn.net/20180417075102798?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/201804170751175?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2、剪枝完要做fine tune。如果只是对一个网络做剪枝而不做fine tune，那么结果是比较差的。因此这个剪枝算法的具体过程是这样的（如Figure2所示）：导入一个网络结构；通过公式1设定的损失函数进行训练，训练到一定准确率后开始基于该网络的所有BN层的γ参数和要剪枝的比例计算γ的阈值，然后将低于该阈值的γ对应的feature map剪掉；剪枝完的网络要fine tune一定epoch后才能看到准确率等指标慢慢爬升到理想范围。以上就是整个剪枝的过程，整个过程可以重复执行多次，就像Figure2中的虚线箭头所示。

![这里写图片描述](https://img-blog.csdn.net/20180417075131386?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

举个例子来说明剪枝的过程：比如某个网络的第一层是卷积层（假设卷积核有64个），接下来是BN层，然后在确定好γ的阈值后，假设这个BN层的γ向量（原来γ向量有64个值，和前面卷积核的数量的对应相等的）只有17个值大于这个阈值，那么在构建剪枝后的网络时，第一个卷积层的卷积核参数就设定为17，同时把这17个卷积核的参数赋给剪枝后的网络，同样BN层的γ向量也初始化成长度为17的向量，而且用那17个超过阈值的值来赋值。网络的其他层都是这样按顺序进行赋值，这样就得到了剪枝后的网络了。接下来的fine tune就不细说了，就是常规的网络训练。
**实验结果：**

文章中大部分的实验是在CIFAR-10和CIFAR-100数据集上做的，网络主要还是VGG，而ResNet和DenseNet的实验较少。整体看加速和压缩的效果还是比较明显的，更多实验结果可以参看论文。 
![这里写图片描述](https://img-blog.csdn.net/20180417075147752?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
最后说下文中主要还是对VGG网络做压缩和加速，并没有ResNet和DenseNet网络在ImageNet数据集上的实验结果，因此不知道是否同样有效，毕竟对于ResNet这样存在skip connection和element-wise add操作的网络而言，如何保证剪枝后做element-wise add时feature map的channel数量一致是关键。上面给出的PyTorch代码逻辑比较清晰，如果看不懂源代码的lua代码，可以考虑看PyTorch实现的代码，PyTorch命令式的编程可以很容易对BN层的梯度进行操作，非常直接地对损失函数做了改造。









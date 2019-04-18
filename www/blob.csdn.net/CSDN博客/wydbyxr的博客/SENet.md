# SENet - wydbyxr的博客 - CSDN博客
2018年11月28日 10:01:37[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：456
个人分类：[深度学习论文阅读](https://blog.csdn.net/wydbyxr/article/category/7151097)
# SENet
  ImageNet冠军
  SE模块相当于channel-wise attention。卷积核作为卷积神经网络的核心，通常被看做是在局部感受野上，将空间上（spatial）的信息和特征维度上（channel-wise）的信息进行聚合的信息聚合体。
  最近很多工作被提出来从空间维度层面来提升网络的性能，如Inception结构中嵌入了多尺度信息，聚合多种不同感受野上的特征来获得性能增益；在Inside-Outside网络中考虑了空间中的上下文信息；还有将Attention机制引入到空间维度上等等。
  参考资料：
[https://zhuanlan.zhihu.com/p/39530147](https://zhuanlan.zhihu.com/p/39530147)
[http://www.sohu.com/a/161793789_642762](http://www.sohu.com/a/161793789_642762)
  在SE结构中，Squeeze和Excitation是两个非常关键的操作，所以我们以此来命名。我们的动机是希望显式地建模特征通道之间的相互依赖关系。另外，我们并不打算引入一个新的空间维度来进行特征通道间的融合，而是采用了一种全新的“特征重标定”策略。具体来说，就是通过学习的方式来自动获取到每个特征通道的重要程度，然后依照这个重要程度去提升有用的特征并抑制对当前任务用处不大的特征。
## 网络结构
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181128095825779.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
  给定一个输入x，其特征通道数为c_1，通过一系列卷积等一般变换后得到一个特征通道数为c_2的特征。与传统的CNN不一样的是，接下来我们通过三个操作来重标定前面得到的特征：
  1）首先是Squeeze操作，我们顺着空间维度来进行特征压缩，将每个二维的特征通道变成一个实数，这个实数某种程度上具有全局的感受野，并且输出的维度和输入的特征通道数相匹配。它表征着在特征通道上响应的全局分布，而且使得靠近输入的层也可以获得全局的感受野，这一点在很多任务中都是非常有用的。
  2）其次是Excitation操作，它是一个类似于循环神经网络中门的机制。通过参数 来为每个特征通道生成权重，其中参数 被学习用来显式地建模特征通道间的相关性。
  3）最后是一个Reweight的操作，我们将Excitation的输出的权重看做是进过特征选择后的每个特征通道的重要性，然后通过乘法逐通道加权到先前的特征上，完成在通道维度上的对原始特征的重标定。
## 具体的实例
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181128095915120.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
  1）左图是将SE模块嵌入到Inception结构的一个示例
  使用global average pooling作为Squeeze操作。紧接着两个Fully Connected 层组成一个Bottleneck结构去建模通道间的相关性，并输出和输入特征同样数目的权重。我们首先将特征维度降低到输入的1/16，然后经过ReLu激活后再通过一个Fully Connected 层升回到原来的维度。这样做比直接用一个Fully Connected层的好处在于：a）具有更多的非线性，可以更好地拟合通道间复杂的相关性；b）极大地减少了参数量和计算量。然后通过一个Sigmoid的门获得0~1之间归一化的权重，最后通过一个Scale的操作来将归一化后的权重加权到每个通道的特征上。
  2）右图是将SE嵌入到 ResNet模块中的一个例子，嵌入到含有skip-connections的模块中。
  操作过程基本和SE-Inception一样，只不过是在Addition前对分支上Residual的特征进行了特征重标定。如果对Addition后主支上的特征进行重标定，由于在主干上存在0~1的scale操作，在网络较深BP优化时就会在靠近输入层容易出现梯度消散的情况，导致模型难以优化。
## 实验
  它还在模型和计算复杂度上具有良好的特性。
  拿ResNet-50和SE-ResNet-50对比举例来说，SE-ResNet-50相对于ResNet-50有着10%模型参数的增长。额外的模型参数都存在于Bottleneck设计的两个Fully Connected中，由于ResNet结构中最后一个stage的特征通道数目为2048，导致模型参数有着较大的增长，实验发现移除掉最后一个stage中3个build block上的SE设定，可以将10%参数量的增长减少到2%。此时模型的精度几乎无损失。
  由于在现有的GPU实现中，都没有对global pooling和较小计算量的Fully Connected进行优化，这导致了在GPU上的运行时间SE-ResNet-50相对于ResNet-50有着约10% 的增长。尽管如此，其理论增长的额外计算量仅仅不到1%，这与其在CPU 运行时间上的增长相匹配（~2%）。可以看出，在现有网络架构中嵌入SE模块而导致额外的参数和计算量的增长微乎其微。

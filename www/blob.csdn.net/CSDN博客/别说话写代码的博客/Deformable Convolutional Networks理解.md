# Deformable Convolutional Networks理解 - 别说话写代码的博客 - CSDN博客





2018年12月08日 22:17:27[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：67








## 文章转自知乎：[https://zhuanlan.zhihu.com/p/37578271](https://zhuanlan.zhihu.com/p/37578271)

## Deformable Convolutional Networks

论文链接：[https://arxiv.org/abs/1703.06211](http://link.zhihu.com/?target=https%3A//arxiv.org/abs/1703.06211)

代码链接：[https://github.com/msracver/Deformable-ConvNets](http://link.zhihu.com/?target=https%3A//github.com/msracver/Deformable-ConvNets)（官方实现，但是release的版本是迁移到MXNet上的，速度和performance上有些出入）

MSRA的文章，严格意义上讲，deformable convolutional不仅仅只适用于object detection，作为对常见的卷积的一种改良，它可以广泛应用在各个网络中，和空洞卷积一样，是对传统卷积的改良，而且某种意义上也可以说是空洞卷积的进一步拓展。

## 1. Background

这篇文章的motivation其实比较简单，我们都知道，常规的卷积，包括池化这些操作，不管你怎么累加，基本得到的都是矩形框……这个其实非常不贴合实际，在对不规则的目标建模的时候有非常大的局限性。

作者认为克服建模形变困难主要是两种方法：
- 通过提供变化足够丰富的大量训练样本，让训练器强行记忆拟合
- 设计能够检测到形变的特征和算法，这方面主要还是以hand-crafted的特征为主，这种在对付没有考虑到的形变和过于复杂的形变的时候其实比较无力

	CNN其实不具有旋转不变性和尺度不变性的，所以本质上CNN是用的策略1，缺乏判断形变的策略依据。局限性比较多，比如固定的box对于灵活物体来说非常僵硬，比如**同一层的特征图的不同位置可能对应的是不同形状的物体，但是都和同一个卷积做计算。**(理解这一点比较重要，涉及到后面对deformable卷积的理解)

	作为对上述问题的解决方案，作者提出了一种新的卷积方式deformable convolution，在此基础上提出了一种新的RoI pooling方法deformable RoI pooling。主要思路就是卷积操作不是在规规矩矩的3x3的格子里做了，而是有了种种偏移，如下图所示：

![](https://img-blog.csdnimg.cn/20181220141926234.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)



至于这个位置偏移是怎么做的？看下个部分

## 2. Deformable Convolutional Networks

## 2.1 deformable卷积的计算公式

我们知道，一般的卷积是这种形式的：

![](https://img-blog.csdnimg.cn/20181220141945277.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

留意到p0是中心点，pn是属于R的3x3的kernel的9个位置，因此这是在3x3方格内采样的一个规则卷积，而可形变的卷积就是在x(po+pn)之中再加上一个偏移量：

![](https://img-blog.csdnimg.cn/2018122014200211.png)

在实际当中，因为添加的这个偏移量往往是小数，导致比较难处理，作者采用的办法是双线性差值，根据相邻的点来进行计算具体公式如下，留意到这里的a-b如果差值大于1了，g就是0，所以只有比较接近的点才对线性差值的点有效果：

![](https://img-blog.csdnimg.cn/20181220142017267.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)





(以上是公式化的内容，关于差值内容如何求导可以到附录里面找)

## 2.2 偏置求法

**重点来了，怎么得到这个偏移量？**，可变卷积神经网络的示意图如下图所示：

![](https://img-blog.csdnimg.cn/20181220142040427.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

deformable convolution在**特征图的每个位置都生成一个偏移量**，注意是特征图的每个位置而不是卷积核的每个位置，我之前也经常误解为后者……实际上这里的特征图经过conv得到一个2N的offset field，N就是通道数，field是和feature map相同的，这样HxWxN的feature map就得到了一个HxWx2N的offset filed，正好每个feature map位置有两个偏移量。

然后将偏移量对应到原来feature map的每个位置，就可以在原来的feature map上做deformable卷积了~还不理解的话可以看这篇博客[https://blog.csdn.net/u011974639/article/details/79996353](http://link.zhihu.com/?target=https%3A//blog.csdn.net/u011974639/article/details/79996353)，是对照着代码讲的，很详细，不过这个只是帮助理解原理的，不能直接用，没有重写op，太慢。

还需要注意的一点是，如果input feature map到output feature map用的是空洞卷积之类的特殊结构的话，生成offset field的卷积也要使用相应的结构以保证一致。

## 2.3 Deformable RoI Pooling

为RoI pooling添加deformable机制其实和卷积的非常类似。因为本身RoI pooling的时候和卷积一样，选取的也是方形区域，我们要做的也是增加偏移量。原来的RoI pooling过程可以表示为如下：

![](https://img-blog.csdnimg.cn/20181220142111339.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

那么新的RoI pooling计算公式则如下：

![](https://img-blog.csdnimg.cn/20181220142127750.png)

而它的偏移量的网络结构和计算过程如下图所示：



![](https://img-blog.csdnimg.cn/20181220142150652.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

其中，要注意的有两点：
- feature map此处是用fc来学习的，而且也不是直接学习最终偏移量，而是学习头上带三角号的那个变量……
- 这个变量会和(w, h)做点乘，然后再乘以一个尺度因子，其中w,h是RoI的宽和高，而伽马是一个0.1的常数因子……个人理解，前者是为了让deformable能和RoI的尺度结合起来，更好地估计偏移位置；而后者是为了防止偏移量太大甚至超出RoI和整个图像……之前也看到有人认为，DNN网络最后几层经常会出现感受野不足的情况，所以空洞卷积才会效果比一般卷积好，如果不加约束，deformable的RoI可能无限制地扩大，这个观念我还不是很理解，日后如果有了新发现再更新吧

	另外就是作者还搞出了一个和R-FCN的位置敏感比较类似的另一种RoI方法，同样是考虑deformable因素的，叫Position-Sensitive (PS) RoI Pooling，这里就不详细介绍了

## 2.4 deformable networks

这个部分没什么太多内容要介绍的，因为deformable卷积使用前后feature map是不变化的，因此大致上就是将卷积和Pooling方法直接替换到一些主流方法里就好……一些网络设置就不谈了，说两个我感觉比较重要的点：
- 学习offset的那些参数怎么初始化和学习：作者采用的是0初始化，然后按照网络其它参数学习率的$\beta$倍来学习，这个$\beata$默认是1，但在某些情况下，比如faster rcnn中的fc的那些，是0.01
- 并不是所有的卷积都一股脑地换成可行变卷积就是好的，在提取到一些语义特征后使用形变卷积效果会更好一点，一般来说是网络靠后的几层

## 3. 网络理解和实验结果

## 3.1 对deformable卷积的理解

作者绘制了一些图来方便我们理解deformable卷积，例如常规卷积和可形变卷积的一些对比：

![](https://img-blog.csdnimg.cn/20181220142215362.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

最上面的top feature map的activation units，中间层和最下层是上一层和上上层的卷积，可以看到它们产生的过程。这张图应该会更清晰一点的：

![](https://img-blog.csdnimg.cn/2018122014223773.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

绿色的点是activation units，而红色的点是激活点在3个level的deformable filters采样位置(一共9的三次方700多个点)，左中右分别是背景类，小目标和中等目标的采样点分布，和传统的卷积还是产生了很大差异的。

另外也有关于RoI pooling中的9个Bin和gt的图，可以看到和传统的方法的差别，RoI pooling也会根据物体产生形变：

![](https://img-blog.csdnimg.cn/2018122014225499.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

作者认为可形变卷积的优势还是很大的，包括：
- 对物体的形变和尺度建模的能力比较强
- 感受野比一般卷积大很多，因为有偏移的原因，实际上相关实验已经表明了DNN网络很多时候受感受野不足的条件制约；但是一般的空洞卷积空洞是固定的，对不同的数据集不同情况可能最适合的空洞大小是不同的，但是可形变卷积的偏移是可以根据具体数据的情况进行学习的

	另外值得一提的是，作者发现可形变卷积可以适应物体的尺度，而背景类的尺度一般在medium和large尺寸的物体之间，作者认为这说明large的目标可以更有效帮助我们识别干扰，也算是无心插柳吧。

## 3. 2 实验结果

这是作者给出的最终结果，class-aware RPN是指分类别的RPN，可以视作一个简易版本的SSD，而对DeepLab的方法笔者也不是很了解。

![](https://img-blog.csdnimg.cn/20181220142313604.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

总的来说，这个工作亮点是很多，包括对卷积的改良，知乎上对这个工作有一些讨论包括评论我觉得还是很不错的：
[https://www.zhihu.com/question/57493889/answer/153369805](https://www.zhihu.com/question/57493889/answer/153369805) (包括评论)，卷积改变为类似采样的思路都是很有意义的。但是看完之后总感觉也有一些疑问，比如怀疑是否一层网络真的能有效学习偏置，不同图片的大量不同物体在offset优化上会不会引发竞争什么的，不过都属于很难量化和考察的内容了，最后还是要靠实验结果说话，大概是目前炼丹的通病吧，有兴趣的也可以留言讨论下~




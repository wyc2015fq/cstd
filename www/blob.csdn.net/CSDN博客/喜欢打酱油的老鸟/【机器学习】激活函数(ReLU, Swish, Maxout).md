
# 【机器学习】激活函数(ReLU, Swish, Maxout) - 喜欢打酱油的老鸟 - CSDN博客


2018年08月16日 08:54:02[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：443


[https://blog.csdn.net/ChenVast/article/details/81382939](https://blog.csdn.net/ChenVast/article/details/81382939)
神经网络中使用激活函数来加入非线性因素，提高模型的表达能力。
## ReLU(Rectified Linear Unit,修正线性单元)
形式如下:
![](https://img-blog.csdn.net/20180803103158262?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0NoZW5WYXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**ReLU**公式近似推导::
![](https://img-blog.csdn.net/20180803103227318?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0NoZW5WYXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
下面解释上述公式中的softplus,Noisy ReLU.
**softplus**函数与ReLU函数接近,但比较平滑, 同ReLU一样是单边抑制,有宽广的接受域(0,+inf), 但是由于指数运算,对数运算计算量大的原因,而不太被人使用.并且从一些人的使用经验来看(Glorot et al.(2011a)),效果也并不比ReLU好.
softplus的导数恰好是sigmoid函数.softplus 函数图像:
![softplus](https://images2018.cnblogs.com/blog/606386/201805/606386-20180502160705206-923153087.png)
**Noisy ReLU**1
ReLU可以被扩展以包括高斯噪声(Gaussian noise):
f(x)=max(0,x+Y),Y∼N(0,σ(x))f(x)=max(0,x+Y),Y∼N(0,σ(x))
Noisy ReLU 在受限玻尔兹曼机解决计算机视觉任务中得到应用.
**ReLU上界设置**: ReLU相比sigmoid和tanh的一个缺点是没有对上界设限.在实际使用中,可以设置一个上限,如ReLU6经验函数: f(x)=min(6,max(0,x))f(x)=min(6,max(0,x)). 参考这个上限的来源论文:Convolutional Deep Belief Networks on CIFAR-10. A. Krizhevsky
**ReLU的稀疏性**（摘自这里）：
> 当前，深度学习一个明确的目标是从数据变量中解离出关键因子。原始数据（以自然数据为主）中通常缠绕着高度密集的特征。然而，如果能够解开特征间缠绕的复杂关系，转换为稀疏特征，那么特征就有了鲁棒性（去掉了无关的噪声）。稀疏特征并不需要网络具有很强的处理线性不可分机制。那么在深度网络中，对非线性的依赖程度就可以缩一缩。一旦神经元与神经元之间改为线性激活，网络的
> 非线性部分
> 仅仅来自于神经元部分选择性激活。

> 对比大脑工作的95%稀疏性来看，现有的计算神经网络和生物神经网络还是有很大差距的。庆幸的是，ReLu只有负值才会被稀疏掉，即引入的稀疏性是可以训练调节的，是动态变化的。只要进行梯度训练，网络可以向误差减少的方向，自动调控稀疏比率，保证激活链上存在着合理数量的非零值。

### ReLU 缺点
坏死: ReLU 强制的稀疏处理会减少模型的有效容量（即特征屏蔽太多，导致模型无法学习到有效特征）。由于ReLU在x < 0时梯度为0，这样就导致负的梯度在这个ReLU被置零，而且这个神经元有可能再也不会被任何数据激活，称为神经元“坏死”。
无负值: ReLU和sigmoid的一个相同点是结果是正值，没有负值.

## ReLU变种
### Leaky ReLU
当x<0x<0时,f(x)=αxf(x)=αx,其中αα非常小,这样可以避免在x<0x<0时,不能够学习的情况：
![](https://img-blog.csdn.net/20180803103340337?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0NoZW5WYXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
称为Parametric Rectifier(**PReLU**),将 αα 作为可学习的参数.
当 αα 从高斯分布中随机产生时称为Random Rectifier（**RReLU**）。
当固定为α=0.01α=0.01时,是**Leaky ReLU**。
优点:
不会过拟合(saturate)
计算简单有效
比sigmoid/tanh收敛快

### 指数线性单元ELU
![](https://img-blog.csdn.net/20180803103408797?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0NoZW5WYXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
exponential linear unit， 该激活函数由Djork等人提出,被证实有较高的噪声鲁棒性,同时能够使得使得神经元
的平均激活均值趋近为 0,同时对噪声更具有鲁棒性。由于需要计算指数,计算量较大。
ReLU family:
![relus](https://images2017.cnblogs.com/blog/606386/201711/606386-20171102101255873-2095075312.bmp)
Leaky ReLU αα是固定的;PReLU的αα不是固定的,通过训练得到;RReLU的αα是从一个高斯分布中随机产生,并且在测试时为固定值，与Noisy ReLU类似（但是区间正好相反）。
ReLU系列对比：
![ReLUç³»åå¯¹æ¯](https://images2017.cnblogs.com/blog/606386/201711/606386-20171102101447857-1756364198.png)

### SELU
论文: 自归一化神经网络(Self-Normalizing Neural Networks)中提出只需要把激活函数换成SELU就能使得输入在经过一定层数之后变成固定的分布. 参考对这篇论文的讨论.
SELU是给ELU乘上系数 λλ, 即 SELU(x)=λ⋅ELU(x)
![](https://img-blog.csdn.net/20180803103455206?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0NoZW5WYXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## Swish
paperSearching for Activation functions(Prajit Ramachandran,Google Brain 2017)
![](https://img-blog.csdn.net/20180803103523693?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0NoZW5WYXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
β是个常数或可训练的参数.Swish 具备无上界有下界、平滑、非单调的特性。
Swish 在深层模型上的效果优于 ReLU。例如，仅仅使用 Swish 单元替换 ReLU 就能把 Mobile NASNetA 在 ImageNet 上的 top-1 分类准确率提高 0.9%，Inception-ResNet-v 的分类准确率提高 0.6%。
![swish](https://images2017.cnblogs.com/blog/606386/201711/606386-20171102101521763-698600913.png)
导数:
![swish-derivation](https://images2017.cnblogs.com/blog/606386/201711/606386-20171102101538013-1397340773.png)
当β = 0时,Swish变为线性函数f(x)=x2f(x)=x2.
β → ∞, σ(x)=(1+exp(−x))−1σ(x)=(1+exp⁡(−x))−1为0或1. Swish变为ReLU: f(x)=2max(0,x)
所以Swish函数可以看做是介于线性函数与ReLU函数之间的平滑函数.
**工程实现**:
在TensorFlow框架中只需一行代码:`x * tf.sigmoid(beta * x)`或`tf.nn.swish(x)`.
在Caffe中使用`Scale+Sigmoid+EltWise(PROD)`来实现或者合并成一个层.代码参考.

## Maxout
论文Maxout Networks(Goodfellow,ICML2013)
Maxout可以看做是在深度学习网络中加入一层激活函数层,包含一个参数k.这一层相比ReLU,sigmoid等,其特殊之处在于增加了k个神经元,然后输出激活值最大的值.
我们常见的隐含层节点输出：
![](https://img-blog.csdn.net/2018080310360642?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0NoZW5WYXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
而在Maxout网络中，其隐含层节点的输出表达式为：
![](https://img-blog.csdn.net/20180803103615918?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0NoZW5WYXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
其中
![](https://img-blog.csdn.net/20180803103639116?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0NoZW5WYXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
以如下最简单的多层感知器(MLP)为例:
![maxout-networks-4-1024](https://images2017.cnblogs.com/blog/606386/201710/606386-20171009171758918-776097243.jpg)
图片来源:slides
maxout-networks-4-1024
假设网络第i层有2个神经元x1、x2，第i+1层的神经元个数为1个.原本只有一层参数,将ReLU或sigmoid等激活函数替换掉,引入Maxout,将变成两层参数,参数个数增为k倍.
优点：
Maxout的拟合能力非常强，可以拟合任意的凸函数。
Maxout具有ReLU的所有优点，线性、不饱和性。
同时没有ReLU的一些缺点。如：神经元的死亡。
缺点：
从上面的激活函数公式中可以看出，每个神经元中有两组(w,b)参数，那么参数量就增加了一倍，这就导致了整体参数的数量激增。

### Maxout激活函数
与常规激活函数不同的是,它是一个可学习的分段线性函数.
然而任何一个凸函数，都可以由线性分段函数进行逼近近似。其实我们可以把以前所学到的激活函数：ReLU、abs激活函数，看成是分成两段的线性函数，如下示意图所示：
![maxout-convex-func-approximate!](https://images2017.cnblogs.com/blog/606386/201710/606386-20171009171740277-913264806.png)
maxout-convex-func-approximate!
实验结果表明Maxout与Dropout组合使用可以发挥比较好的效果。
那么,前边的两种ReLU便是两种Maxout,函数图像为两条直线的拼接,
![](https://img-blog.csdn.net/20180803103734329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0NoZW5WYXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## sigmoid & tanh
sigmoid/logistic 激活函数:
![](https://img-blog.csdn.net/20180803103749743?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0NoZW5WYXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
tanh 函数是sigmoid函数的一种变体，以0点为中心。取值范围为 [-1,1] ，而不是sigmoid函数的 [0,1] 。
![](https://img-blog.csdn.net/20180803103801223?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0NoZW5WYXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
tanh 是对 sigmoid 的平移和收缩: tanh(x)=2⋅σ(2x)−1tanh⁡(x)=2⋅σ(2x)−1.
你可能会想平移使得曲线以0点为中心,那么为什么还要收缩呢? 如果不拉伸或收缩得到 f(x)=ex−1ex+1f(x)=ex−1ex+1 不行吗? 我猜想是因为 tanh 更加著名吧。
那么 tanh 这个双曲正切函数与三角函数 tan 之间是什么关系呢?
![](https://img-blog.csdn.net/20180803103841244?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0NoZW5WYXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
hard tanh 限界: g(z) = max(-1, min(1,z))
sigmoid & tanh 函数图像如下:
![èè²sigmoid-çº¢è²tanh](https://images2018.cnblogs.com/blog/606386/201807/606386-20180712202915278-1408388561.png)

### sigmoid作激活函数的优缺点
历史上很流行(Historically popular since they have nice interpretation as a saturating “firing rate” of a neuron),梯度计算较为方便:
![](https://img-blog.csdn.net/20180803103913530?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0NoZW5WYXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**优势**是能够控制数值的幅度,在深层网络中可以保持数据幅度不会出现大的变化;而ReLU不会对数据的幅度做约束.
存在三个**问题**:
饱和的神经元会"杀死"梯度,指离中心点较远的x处的导数接近于0,停止反向传播的学习过程.
sigmoid的输出不是以0为中心,而是0.5,这样在求权重w的梯度时,梯度总是正或负的.
指数计算耗时

### 为什么tanh相比sigmoid收敛更快:
梯度消失问题程度
![](https://img-blog.csdn.net/20180803103941909?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0NoZW5WYXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
可以看出tanh(x)的梯度消失问题比sigmoid要轻.梯度如果过早消失,收敛速度较慢.
以零为中心的影响
如果当前参数(w0,w1)的最佳优化方向是(+d0, -d1),则根据反向传播计算公式,我们希望 x0 和 x1 符号相反。但是如果上一级神经元采用 Sigmoid 函数作为激活函数，sigmoid不以0为中心，输出值恒为正，那么我们无法进行最快的参数更新，而是走 Z 字形逼近最优解。4

## 激活函数的作用
加入非线性因素
充分组合特征
下面说明一下为什么有组合特征的作用.
一般函数都可以通过泰勒展开式来近似计算, 如sigmoid激活函数中的指数项可以通过如下的泰勒展开来近似计算:
![](https://img-blog.csdn.net/20180803104000124?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0NoZW5WYXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
其中有平方项,立方项及更更高项, 而 z=wx+bz=wx+b, 因此可以看作是输入特征 x 的组合. 以前需要由领域专家知识进行特征组合,现在激活函数能起到一种类似特征组合的作用. (思想来源: 微博@算法组)
**为什么ReLU,Maxout等能够提供网络的非线性建模能力？**它们看起来是分段线性函数，然而并不满足完整的线性要求：加法f(x+y)=f(x)+f(y)和乘法f(ax)=a×f(x)或者写作![](https://img-blog.csdn.net/20180803104017883?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0NoZW5WYXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)。非线性意味着得到的输出不可能由输入的线性组合重新得到（重现）。假如网络中不使用非线性激活函数，那么这个网络可以被一个单层感知器代替得到相同的输出，因为线性层加起来后还是线性的，可以被另一个线性函数替代。
## 梯度消失与梯度爆炸
**梯度消失/爆炸原因及解决办法**
原因,浅层的梯度计算需要后面各层的权重及激活函数导数的乘积,因此可能出现前层比后层的学习率小(vanishing gradient)或大(exploding)的问题,所以具有不稳定性.那么如何解决呢?
需要考虑几个方面:
权重初始化
使用合适的方式初始化权重, 如ReLU使用MSRA的初始化方式, tanh使用xavier初始化方式.
激活函数选择
激活函数要选择ReLU等梯度累乘稳定的.
学习率
一种训练优化方式是对输入做白化操作(包括正规化和去相关), 目的是可以选择更大的学习率. 现代深度学习网络中常使用Batch Normalization(包括正规化步骤,但不含去相关). (All you need is a good init. If you can't find the good init, use Batch Normalization.)
由于梯度的公式包含每层激励的导数以及权重的乘积,因此让中间层的乘积约等于1即可.但是sigmoid这种函数的导数值又与权重有关系(最大值1/4,两边对称下降),所以含有sigmoid的神经网络不容易解决,输出层的activation大部分饱和,因此不建议使用sigmoid.
ReLU在自变量大于0时导数为1,小于0时导数为0,因此可以解决上述问题.
**梯度爆炸**
由于sigmoid,ReLU等函数的梯度都在[0,1]以内，所以不会引发梯度爆炸问题。 而梯度爆炸需要采用梯度裁剪、BN、设置较小学习率等方式解决。
## 激活函数选择
首先尝试ReLU,速度快,但要注意训练的状态.
如果ReLU效果欠佳,尝试Leaky ReLU或Maxout等变种。
尝试tanh正切函数(以零点为中心,零点处梯度为1)
sigmoid/tanh在RNN（LSTM、注意力机制等）结构中有所应用，作为门控或者概率值.
在浅层神经网络中，如不超过4层的，可选择使用多种激励函数，没有太大的影响。
著作权归作者所有。商业转载请联系作者获得授权,非商业转载请注明出处。
原文:https://www.cnblogs.com/makefile/p/activation-function.html©康行天下


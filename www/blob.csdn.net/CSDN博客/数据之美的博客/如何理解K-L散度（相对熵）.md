# 如何理解K-L散度（相对熵） - 数据之美的博客 - CSDN博客
2019年03月14日 13:50:05[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：55
转自于：[https://www.jianshu.com/p/43318a3dc715?from=timeline&isappinstalled=0](https://www.jianshu.com/p/43318a3dc715?from=timeline&isappinstalled=0)，如有侵权，请联系我删除。
`Kullback-Leibler Divergence`，即`K-L散度`，是一种**量化两种概率分布P和Q之间差异**的方式，又叫`相对熵`。在概率学和统计学上，我们经常会使用一种`更简单的、近似的分布`来替代`观察数据`或`太复杂的分布`。K-L散度能帮助我们度量使用一个分布来近似另一个分布时所损失的信息。
K-L散度定义见文末附录1。另外在附录5中解释了为什么在深度学习中，训练模型时使用的是`Cross Entropy` 而非`K-L Divergence`。
我们从下面这个问题出发思考K-L散度。
> 
假设我们是一群太空科学家，经过遥远的旅行，来到了一颗新发现的星球。在这个星球上，生存着一种长有牙齿的蠕虫，引起了我们的研究兴趣。我们发现这种蠕虫生有10颗牙齿，但是因为不注意口腔卫生，又喜欢嚼东西，许多蠕虫会掉牙。收集大量样本之后，我们得到关于蠕虫牙齿数量的经验分布，如下图所示
![](https://upload-images.jianshu.io/upload_images/75110-f1e63f2247d8a351.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
牙齿数量分布
![](https://upload-images.jianshu.io/upload_images/75110-a1671534d273e258.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/400/format/webp)
会掉牙的外星蠕虫
这些数据很有价值，但是也有点问题。我们距离地球🌍太远了，把这些概率分布数据发送回地球过于昂贵。还好我们是一群聪明的科学家，用一个只有一两个参数的简单模型来近似原始数据会减小数据传送量。最简单的近似模型是`均分布`，因为蠕虫牙齿不会超过10颗，所以有11个可能值，那蠕虫的牙齿数量概率都为 `1/11`。分布图如下：
![](https://upload-images.jianshu.io/upload_images/75110-6f8844d5047fc1ee.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
uniform distribution
显然我们的原始数据并非均分布的，但也不是我们已知的分布，至少不是常见的分布。作为备选，我们想到的另一种简单模型是`二项式分布binomial distribution`。蠕虫嘴里面共有`n=10`个牙槽，每个牙槽出现牙齿与否为独立事件，且概率均为`p`。则蠕虫牙齿数量即为期望值`E[x]=n*p`，真实期望值即为观察数据的平均值，比如说`5.7`，则`p=0.57`，得到如下图所示的二项式分布：
![](https://upload-images.jianshu.io/upload_images/75110-8ad3782fe0220619.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
binomial
对比一下原始数据，可以看出均分布和二项分布都不能完全描述原始分布。
![](https://upload-images.jianshu.io/upload_images/75110-b049f7fc560bd8d8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
all distributions
可是，我们不禁要问，哪一种分布更加接近原始分布呢？
已经有许多度量误差的方式存在，但是我们所要考虑的是减小发送的信息量。上面讨论的均分布和二项式分布都把问题规约到只需要两个参数，牙齿数量和概率值（均分布只需要牙齿数量即可）。那么哪个分布保留了更多的原始数据分布的信息呢？这个时候就需要K-L散度登场了。
## 数据的熵
K-L散度源于信息论。信息论主要研究如何量化数据中的信息。最重要的信息度量单位是`熵`Entropy，一般用`H`表示。分布的熵的公式如下：
![](https://upload-images.jianshu.io/upload_images/75110-7977551fba322af7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/472/format/webp)
Entropy
上面对数没有确定底数，可以是`2`、`e`或`10`，等等。如果我们使用以`2`为底的对数计算H值的话，可以把这个值看作是编码信息所需要的最少二进制位个数bits。上面空间蠕虫的例子中，信息指的是根据观察所得的经验分布给出的蠕虫牙齿数量。计算可以得到原始数据概率分布的熵值为`3.12 bits`。这个值只是告诉我们编码蠕虫牙齿数量概率的信息需要的二进制位`bit`的位数。
可是熵值并没有给出压缩数据到最小熵值的方法，即如何编码数据才能达到最优（存储空间最优）。优化信息编码是一个非常有意思的主题，但并不是理解K-L散度所必须的。熵的主要作用是告诉我们最优编码信息方案的理论下界（存储空间），以及度量数据的信息量的一种方式。理解了熵，我们就知道有多少信息蕴含在数据之中，现在我们就可以计算当我们用一个带参数的概率分布来近似替代原始数据分布的时候，到底损失了多少信息。请继续看下节内容。↓↓↓
## K-L散度度量信息损失
只需要稍加修改`熵H`的计算公式就能得到`K-L散度`的计算公式。设`p`为观察得到的概率分布，`q`为另一分布来近似`p`，则`p`、`q`的`K-L散度`为：
![](https://upload-images.jianshu.io/upload_images/75110-e94b5412d85e5698.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/752/format/webp)
entropy-p-q
显然，根据上面的公式，K-L散度其实是数据的原始分布p和近似分布q之间的对数差值的期望。如果继续用`2`为底的对数计算，则**K-L散度值表示信息损失的二进制位数**。下面公式以期望表达K-L散度：
![](https://upload-images.jianshu.io/upload_images/75110-5e6109b0e9ec780a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/576/format/webp)
DKL1
一般，K-L散度以下面的书写方式更常见：
![](https://upload-images.jianshu.io/upload_images/75110-d602c1e8d0fe3689.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/534/format/webp)
DKL2
注：`log a - log b = log (a/b)`
OK，现在我们知道当用一个分布来近似另一个分布时如何计算信息损失量了。接下来，让我们重新回到最开始的蠕虫牙齿数量概率分布的问题。
## 对比两种分布
首先是用均分布来近似原始分布的K-L散度：
![](https://upload-images.jianshu.io/upload_images/75110-f682f83675fa42ee.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/571/format/webp)
DKL-uniform
接下来计算用二项式分布近似原始分布的K-L散度：
![](https://upload-images.jianshu.io/upload_images/75110-b7a85dc7ac779750.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/581/format/webp)
DKL-binomial
通过上面的计算可以看出，使用均分布近似原始分布的信息损失要比用二项式分布近似小。所以，如果要从均分布和二项式分布中选择一个的话，均分布更好些。
## 散度并非距离
很自然地，一些同学把K-L散度看作是不同分布之间距离的度量。这是不对的，因为从K-L散度的计算公式就可以看出它不符合对称性（距离度量应该满足对称性）。如果用我们上面观察的数据分布来近似二项式分布，得到如下结果：
![](https://upload-images.jianshu.io/upload_images/75110-c72abbdf1938256e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/558/format/webp)
DKL-notdistance
所以，`Dkl (Observed || Binomial) != Dkl (Binomial || Observed)`。
也就是说，用`p`近似`q`和用`q`近似`p`，二者所损失的信息并不是一样的。
## 使用K-L散度优化模型
前面使用的二项式分布的参数是概率 `p=0.57`，是原始数据的均值。`p`的值域在 [0, 1] 之间，我们要选择一个`p`值，建立二项式分布，目的是最小化近似误差，即K-L散度。那么`0.57`是最优的吗？
下图是原始数据分布和二项式分布的K-L散度变化随二项式分布参数`p`变化情况：
![](https://upload-images.jianshu.io/upload_images/75110-e6ca24b145a5ca8c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
二项分布K-L值变化曲线
通过上面的曲线图可以看出，K-L散度值在圆点处最小，即`p=0.57`。所以我们之前的二项式分布模型已经是最优的二项式模型了。注意，我已经说了，是而像是模型，这里只限定在二项式模型范围内。
前面只考虑了均分布模型和二项式分布模型，接下来我们考虑另外一种模型来近似原始数据。首先把原始数据分成两部分，1）0-5颗牙齿的概率和 2）6-10颗牙齿的概率。概率值如下：
![](https://upload-images.jianshu.io/upload_images/75110-76a03ef0cd285801.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/414/format/webp)
ad hoc model
即，一只蠕虫的牙齿数量`x=i`的概率为`p/5`; `x=j`的概率为`(1-p) / 6`，`i=0,1,2,3,4,5`; `j=6,7,8,9,10`。
Aha，我们自己建立了一个新的（奇怪的）模型来近似原始的分布，模型只有一个参数`p`，像前面那样优化二项式分布的时候所做的一样，让我们画出K-L散度值随`p`变化的情况：
![](https://upload-images.jianshu.io/upload_images/75110-dd374eaec9c5ad74.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
finding an optimal parameter value for our ad hoc model
当`p=0.47`时，K-L值取最小值`0.338`。似曾相识吗？对，这个值和使用均分布的K-L散度值是一样的（这并不能说明什么）！下面我们继续画出这个奇怪模型的概率分布图，看起来确实和均分布的概率分布图相似：
![](https://upload-images.jianshu.io/upload_images/75110-857b751b045a3aa2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
ad hoc model distribution
我们自己都说了，这是个奇怪的模型，在K-L值相同的情况下，更倾向于使用更常见的、更简单的均分布模型。
回头看，我们在这一小节中使用K-L散度作为目标方程，分别找到了二项式分布模型的参数`p=0.57`和上面这个随手建立的模型的参数`p=0.47`。是的，这就是本节的重点：**使用K-L散度作为目标方程来优化模型**。当然，本节中的模型都只有一个参数，也可以拓展到有更多参数的高维模型中。
## 变分自编码器VAEs和变分贝叶斯法
如果你熟悉神经网络，你肯能已经猜到我们接下来要学习的内容。除去神经网络结构的细节信息不谈，整个神经网络模型其实是在构造一个参数数量巨大的函数（百万级，甚至更多），不妨记为`f(x)`，通过设定目标函数，可以训练神经网络逼近非常复杂的真实函数`g(x)`。训练的关键是要设定目标函数，反馈给神经网络当前的表现如何。训练过程就是不断减小目标函数值的过程。
我们已经知道K-L散度用来度量在逼近一个分布时的信息损失量。K-L散度能够赋予神经网络近似表达非常复杂数据分布的能力。变分自编码器（Variational Autoencoders，VAEs）是一种能够学习最佳近似数据集中信息的常用方法，[Tutorial on Variational Autoencoders 2016](https://link.jianshu.com?t=https://arxiv.org/abs/1606.05908)是一篇关于VAEs的非常不错的教程，里面讲述了如何构建VAE的细节。 [What are Variational Autoencoders? A simple explanation](https://link.jianshu.com?t=https://medium.com/@dmonn/what-are-variational-autoencoders-a-simple-explanation-ea7dccafb0e3)简单介绍了VAEs，[Building Autoencoders in Keras](https://link.jianshu.com?t=https://blog.keras.io/building-autoencoders-in-keras.html)介绍了如何利用Keras库实现几种自编码器。
变分贝叶斯方法（Variational Bayesian Methods）是一种更常见的方法。[这篇文章](https://link.jianshu.com?t=https://www.countbayesie.com/blog/2015/3/3/6-amazing-trick-with-monte-carlo-simulations)介绍了强大的蒙特卡洛模拟方法能够解决很多概率问题。蒙特卡洛模拟能够帮助解决许多贝叶斯推理问题中的棘手积分问题，尽管计算开销很大。包括VAE在内的变分贝叶斯方法，都能用K-L散度生成优化的近似分布，这种方法对棘手积分问题能进行更高效的推理。更多变分推理（Variational Inference）的知识可以访问[Edward library for python](https://link.jianshu.com?t=http://edwardlib.org/)。
**因为本人没有学习过VAE和变分推理，所以本节内容质量无法得到保证，我会联系这方面的朋友来改善本节内容，也欢迎大家在评论区给出建议**
译自：[Kullback-Leibler Divergence Explained](https://link.jianshu.com?t=https://www.countbayesie.com/blog/2017/5/9/kullback-leibler-divergence-explained)
作者：Will Kurt
If you enjoyed this post please [subscribe](https://link.jianshu.com?t=http://countbayesie.com/subscribe) to keep up to date and follow [@willkurt](https://link.jianshu.com?t=https://twitter.com/willkurt)!
If you enjoyed this writing and also like programming languages, you might like the [book on Haskell](https://link.jianshu.com?t=https://www.manning.com/books/learn-haskell) I just finished due in print July 2017 (though nearly all the content is available online today).
## 附录
- 
K-L 散度的定义
![](https://upload-images.jianshu.io/upload_images/75110-5d773218b2511d9a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
define K-L divergence
- 
计算K-L的注意事项
![](https://upload-images.jianshu.io/upload_images/75110-f55d663d60503fa4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
notice
- 
遇到`log 0`时怎么办
![](https://upload-images.jianshu.io/upload_images/75110-a7dc83686d7206c1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
example for K-L smoothing
- 
信息熵、交叉熵、相对熵
- 信息熵，即熵，香浓熵。编码方案完美时，最短平均编码长度。
- 交叉熵，cross-entropy。编码方案不一定完美时（由于对概率分布的估计不一定正确），平均编码长度。是神经网络常用的损失函数。
- 相对熵，即K-L散度，relative entropy。编码方案不一定完美时，平均编码长度相对于最小值的增加值。
	更详细对比，见知乎[如何通俗的解释交叉熵与相对熵?](https://link.jianshu.com?t=https://www.zhihu.com/question/41252833)
- 为什么在神经网络中使用交叉熵损失函数，而不是K-L散度？
	K-L散度=熵-交叉熵，即 `DKL( p||q )=H(p)-H(p,q)`。
	在神经网络所涉及到的范围内，`H(p)`不变，则`DKL( p||q )`等价`H(p,q)`。
	更多讨论见[Why do we use Kullback-Leibler divergence rather than cross entropy in the t-SNE objective function?](https://link.jianshu.com?t=https://stats.stackexchange.com/questions/265966/why-do-we-use-kullback-leibler-divergence-rather-than-cross-entropy-in-the-t-sne)和[Why train with cross-entropy instead of KL divergence in classification?](https://link.jianshu.com?t=https://www.reddit.com/r/MachineLearning/comments/4mebvf/why_train_with_crossentropy_instead_of_kl/)
作者：Aspirinrin
链接：https://www.jianshu.com/p/43318a3dc715
来源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。

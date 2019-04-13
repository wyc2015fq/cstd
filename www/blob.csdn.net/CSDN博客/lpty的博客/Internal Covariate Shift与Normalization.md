
# Internal Covariate Shift与Normalization - lpty的博客 - CSDN博客

2019年02月13日 15:02:39[lpty](https://me.csdn.net/sinat_33741547)阅读数：291



# 引言
Batch norm在深度学习中已经是一个常用且有效的方法，用来加速网络收敛及防止梯度消失等神经网络常见的问题，那么batch norm为什么有效呢？
从源头出发，神经网络难以训练的原因是什么？
### Internal Covariate Shift
> 深度神经网络涉及到很多层的叠加，而每一层的参数更新会导致上层的输入数据分布发生变化，通过层层叠加，高层的输入分布变化会非常剧烈，这就使得高层需要不断去重新适应底层的参数更新。为了训好模型，我们需要非常谨慎地去设定学习率、初始化权重、以及尽可能细致的参数更新策略。

> Google 将这一现象总结为 Internal Covariate Shift，简称 ICS.

> 所以ICS是什么呢？将每一层的输入作为一个分布看待，由于底层的参数随着训练更新，导致相同的输入分布得到的输出分布改变了。

> 而机器学习中有个很重要的假设：IID独立同分布假设，就是假设训练数据和测试数据是满足相同分布的，这是通过训练数据获得的模型能够在测试集获得好的效果的一个基本保障。

> 那么，细化到神经网络的每一层间，每轮训练时分布都是不一致，那么相对的训练效果就得不到保障，所以称为层间的covariate shift。

> 大家都知道在统计机器学习中的一个经典假设是“源空间（source domain）和目标空间（target

> domain）的数据分布（distribution）是一致的”。如果不一致，那么就出现了新的机器学习问题，如 transfer

> learning / domain adaptation 等。而 covariate shift

> 就是分布不一致假设之下的一个分支问题，它是指源空间和目标空间的条件概率是一致的，但是其边缘概率不同，即：对所有
$$
x∈X
$$
> ，有：

$$
P_s(Y|x=X)=P_t(Y|x=X)
$$

> 但是，

$$
P_s(X)≠P_t(X)
$$

> 可以看到，随着网络层数的加深，输入分布经过多次线性非线性变换，已经被改变了，但是它对应的标签，如分类，还是一致的，即使条件概率一致，边缘概率不同。

> 因此，每个神经元的输入数据不再是“独立同分布”，导致了以下问题：

> 1、上层网络需要不断适应新的输入数据分布，降低学习速度。

> 2、下层输入的变化可能趋向于变大或者变小，导致上层落入饱和区，使得学习过早停止。

> 3、每层的更新都会影响到其它层，因此每层的参数更新策略需要尽可能的谨慎。

> 解决思想

> 前面说到，出现上述问题的根本原因是神经网络每层之间，无法满足基本假设"独立同分布"，那么思路应该是怎么使得输入分布满足独立同分布。

> 白化（Whitening）

> 白化（Whitening）是机器学习里面常用的一种规范化数据分布的方法，主要是PCA白化与ZCA白化。

> 白化是对输入数据分布进行变换，进而达到以下两个目的：

> 1、使得输入特征分布具有相同的均值与方差，其中PCA白化保证了所有特征分布均值为0，方差为1；而ZCA白化则保证了所有特征分布均值为0，方差相同；

> 2、去除特征之间的相关性。

> 通过白化操作，我们可以减缓ICS的问题，进而固定了每一层网络输入分布，加速网络训练过程的收敛。

> Normalization

> 但是白化计算成本太高，每一轮训练中的每一层都需要做白化操作；同时白化改变了网络每一层的分布，导致网络层中数据的表达能力受限。

> 因此提出了normalization方法，能够简化计算过程；又能够让数据尽可能保留原始的表达能力。

> 但是，实际想想，是不是均值方差一致就是相同的分布了呢？其实不一定，normalization这种方式，实际上并不是直接去解决ICS问题，更多的是面向梯度消失等，去加速网络收敛的。类似covariance shift比较直接的解决思路应该是对样本的Reweight操作，根据前后两个分布进行权重的学习，再对新的分布进行reweight，
> [参考](https://blog.csdn.net/zxyhhjs2017/article/details/79405591)
> 。

> Normalization

> 上面说到，标准化就是将分布变换为均值方差一致的分布，这么做能够加速收敛的本质是什么呢？

> 假设存在
$$
U=Wx+b
$$
> ，
$$
Z=F(U)
$$
> ，其中
$$
x
$$
> 是输入，
$$
F
$$
> 是激活函数，那么：

> 在训练过程中，随着网络加深，分布逐渐发生变动，导致整体分布逐渐往激活函数的饱和区间移动，从而反向传播时底层出现梯度消失，也就是收敛越来越慢的原因。

> 而Normalization则是把分布强行拉回到均值为0方差为1的标准正态分布，使得激活输入值落在非线性函数对输入比较敏感的区域，这样输入的小变化就会导致损失函数较大的变化，避免梯度消失问题产生，加速收敛，如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213101223734.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
> 如上，假设正态分布均值是-2，方差是0.5，如上图绿色曲线，通过标准化操作转换为均值为0，方差为1的正态分布，如上图红色曲线。

> 实际上是将输入x的取值正态分布整体右移2（均值变化），图形曲线更平缓了（方差变化）。那么这么做的好处是什么呢？
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213101808517.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
> 上图为标准正态分布的图形，可以看到，在一个标准差范围，有68%的概率x其值落在[-1,1]的范围内；在两个标准差范围，有95%的概率x其值落在了[-2,2]的范围内，假如这就是需要进行激活的分布，激活函数为sigmoid，如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213101816654.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
> 可以看到，在[-2, 2]的范围内，即是标准正态分布两个标注差范围内，在sigmoid函数中为线性变换区域，微小的变化就能得到很大的改变，也即是梯度比较大。

> 如果不经过变换，存在一个均值为-6，方差为1的分布，对应到激活函数中就是[-8, -4]的区域，这已经是饱和区了，这也就是所谓的梯度消失。

> 所以标准化其实就是把大部分激活的值落入非线性函数的线性区内，其对应的导数远离导数饱和区，这样来加速训练收敛过程。

> 这样看起来很美好，但是实际上呢？

> 如果使用标准化，那就相当于把非线性激活函数替换成线性函数了。那么使用非线性激活的意义在哪里呢，多层线性网络跟一层线性网络是等价的，也就是网络的表达能力下降了。

> 为了保证非线性表达能力，后面又对此打了个补丁，对变换后的满足均值为0方差为1的x进行了scale加上shift操作，形成类似
$$
y=scale*x+shift
$$
> 这种形式，参数通过训练进行学习，把标准正态分布左移或者右移一点，并且长胖一点或者变瘦一点，将分布从线性区往非线性区稍微移动，希望找到一个线性和非线性的较好平衡点，既能享受非线性的较强表达能力，又能够享受线性区较大的下降梯度。

> 因此，通用的标准化公式如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213103936215.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
> 当然这种re-scale的方式看起来有点迷，但标准化实践起来确实work，那也就先用着吧。

> [
](https://img-blog.csdnimg.cn/20190213103936215.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
> Batch Normalization

> [
](https://img-blog.csdnimg.cn/20190213103936215.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
> BN来自Google在2015年的
> [论文](https://arxiv.org/pdf/1502.03167.pdf)
> ，这也是第一篇normalization的论文，标准化针对输入数据的单一维度进行，根据每一个batch计算均值与标准差，由于从形象上是纵向的计算，又称为纵向标准化，如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213105127524.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
> 在原论文中，均值与方差的计算过程如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213105323105.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
> 如上，BN针对每个样本
$$
x
$$
> 的单个维度
$$
x_i
$$
> ，计算大小为m的mini-batch中的m个
$$
x_i
$$
> 的均值与方差，以及后续进行再平移及缩放。

> 但是可以看到，BN针对的是一整个batch进行一阶统计量及二阶统计量的计算，即是隐式的默认了每个batch之间的分布是大体一致的，小范围的不同可以认为是噪音增加模型的鲁棒性，但是如果大范围的变动其实会增加模型的训练难度。

> 同时，bn在计算过程中，需每一层进行标准化，同时还需要保存统计量，相对来说其内存占用较大，同时也不适用于RNN等网络。

> Layer Normalization

> LN来自Jimmy Lei Ba等人在2016年发表的
> [论文](https://arxiv.org/pdf/1607.06450.pdf)
> ，为了解决BN的不足之处提出，相对BN的纵向标准化，LN可以称为横向标准化，
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213111413321.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
> 如上，LN根据单个样本计算该层的平均值及方差，之后用同一个再平移及缩放因子规范各位维度的输入，
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213112332435.png)
> 如上，LN针对单个样本进行计算，避免了batch分布的影响，同时不需要保存每层的统计量，节省了存储空间。但是需要注意的是，相对BN标准化的是单一维度，LN是对所有维度同时进行标准化，假如各个维度表示的特征的纲量不一致（比如颜色和大小），那么会导致模型的表达能力下降。

> [
](https://img-blog.csdnimg.cn/20190213112332435.png)
> Weight Normalization

> [
](https://img-blog.csdnimg.cn/20190213112332435.png)
> WN来自OpenAI在2016年发表的
> [论文](https://arxiv.org/pdf/1602.07868.pdf)
> ，相比BN 和 LN 对输入特征数据
$$
X
$$
> 做标准化 ， WN 则将标准化应用于权重
$$
W
$$
> ，
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213114208771.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
> 如下，WN将权重
$$
W
$$
> 分解为向量方向
$$
v
$$
> 和向量模
$$
g
$$
> 两个部分，
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213135045158.jpg)
> 其中，
$$
v
$$
> 是与
$$
g
$$
> 同维度的向量，
$$
||v||
$$
> 是欧式范数。因此，$ v / ||v||$ 是单位向量，决定了
$$
w
$$
> 的方向；
$$
g
$$
> 是标量，决定了
$$
w
$$
> 的长度，对应到前述的标准化公式，推导如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213135549770.jpg)
> 也就是说，WN虽然在前面是对权重做了手脚，但最后反应到的还是在数据特征上，其中：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213135728424.jpg)
> 只是这里对数据特征进行scale的参数来源于权重的欧式范数，相对BN或者LN，WN不依赖于数据的统计量，避免了前述的batch分布依赖等。

> [
](https://img-blog.csdnimg.cn/20190213135728424.jpg)
> Cosine Normalization

> [
](https://img-blog.csdnimg.cn/20190213135728424.jpg)
> CN来自Luo Chunjie等在2017年发表的
> [论文](https://arxiv.org/pdf/1702.05870.pdf)
> ，前面说到BN、LN都是对输入数据的特征做标准化，WN对权重做标准化，那CN呢？仔细看神经网络中常出现的公式：
$$
f_w(x) = w·x
$$
> ，除了对
$$
x
$$
> 和
$$
w
$$
> 做手脚，那剩下只有中间的操作符
$$
·
$$
> 了，CN正是这么做的。

> [
](https://img-blog.csdnimg.cn/20190213135728424.jpg)
> 前面说到，我们进行标准化是为了让分布处于激活函数的非饱和区，那么为什么分布经过神经元转换后会达到饱和区呢？原因在于点积计算，这个操作的结果是无界的，所以假如能够替换掉这个方法，找一个有界的操作那不就解决了。

> 那么怎么替换点积呢？点积的其实是衡量两个向量相似度的方式，那么用cosine操作不也可以，有界同时也是一个衡量相似度的方式，如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213141221202.jpg)
> 这样，得到的数据都是[-1, 1]范围内的了，相比WN两者存在不少相似之处，CN可以认为是从WN的基础上，再增加了输入向量的的模
$$
||x||
$$
> 对数据特征进行标准化，而WN只使用了权重的模||w||进行标准化。

> 但是原始的点积计算，实际意义上是包含了两个向量的夹角信息及其大小信息，单纯使用cosine只是利用了夹角信息，可能会造成表达能力的下降。

> Why Normalization

> 前面介绍了标准化为什么能加速收敛，及一些不同的标准化方法，这里再思考以下，为什么标准化在实际使用中会有效？

> Weight Scale Invariance

> 权重伸缩不变性，指的是当权重
$$
W
$$
> 按照常量
$$
λ
$$
> 进行伸缩后，标准化的结果保持不变，如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019021314314454.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
> 也就是说，在反向传播的过程中，我们可以更少的去关心权重的初始化值或者其他因素，因为权重过大或者过小导致梯度爆炸或者消失，
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213143429865.jpg)
> 同时，伸缩不变性还对权重起到了正则化的作用，权重越大其梯度就越小，这样就可以使用更大的学习率而无需在调参时花费太大的精力，参数更新稳定同时学习速度加快，
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213143555312.jpg)
> 需要注意的是，这里的权重伸缩不变看起来与数据伸缩不变很类似，原因在于进行标准化操作的是
$$
W·X
$$
> 操作后的输出分布，此时再进行均值与标准差计算时已经隐式包含了上一层的权重信息，所以均值与方差也随之等比例伸缩了。

> [
](https://img-blog.csdnimg.cn/20190213143555312.jpg)
> Data Scale Invariance

> [
](https://img-blog.csdnimg.cn/20190213143555312.jpg)
> 数据伸缩不变性，指的是当数据
$$
X
$$
> 按照常量
$$
λ
$$
> 进行伸缩后，标准化的结果保持不变，即：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190213145808381.jpg)
> 数据伸缩不变性仅对 BN、LN 和 CN 有效，这三个都是对输入数据特征进行标准化。当数据进行常量伸缩时，其均值和方差都等比例缩放，分子分母互相抵消。而 WN 不具有这一性质。

> 而相应的，带来的好处也与上述权重伸缩不变类似。

> 参考

> 1、
> [https://blog.csdn.net/c9Yv2cf9I06K2A9E/article/details/79276708](https://blog.csdn.net/c9Yv2cf9I06K2A9E/article/details/79276708)

> 2、
> [https://www.zhihu.com/question/38102762/answer/85238569](https://www.zhihu.com/question/38102762/answer/85238569)

> 3、
> [https://www.cnblogs.com/guoyaohua/p/8724433.html](https://www.cnblogs.com/guoyaohua/p/8724433.html)


> [
  ](https://img-blog.csdnimg.cn/20190213145808381.jpg)


# 全面解析Inception Score原理及其局限性 - Paper weekly - CSDN博客


2019年01月10日 12:02:10[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：226


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

作者丨尹相楠
学校丨里昂中央理工博士在读
研究方向丨人脸识别、对抗生成网络

本文主要基于这篇文章：*A Note on the Inception Score*，属于读书笔记的性质，为了增加可读性，也便于将来复习，在原文的基础上增加了一些细节。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3Ue25Reup6abYwMsoaMJcenVQrbYJ7l1whV4h3UBOJEjN9s3BexZptibSA/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UeZRynbIsQ9u7hDgvibAMv7gsQZ0F5ockJVfU8znGMuYXnQFrPfBzQIEQ/640?wx_fmt=png)

**很多关于 GAN 生成图片的论文中，作者评价其模型表现的一项重要指标是 Inception Score（下文简称 IS）。**其名字中 Inception 来源于 Google 的 Inception Net，因为计算这个 score 需要用到 Inception Net-V3（第三个版本的 Inception Net）。

Inception Net 是图片分类网络，在 ImageNet 数据库上训练，ImageNet 数据库共有 1.2M 个 RGB 图片，分为 1000 类。Inception Score 只是把 Inception Net-V3 作为一个工具，理解 Inception Score 不需要知道 Inception Net-V3 的细节，各种深度学习框架中都已经包含了预训练好的 Inception Net-V3 了，直接拿来用就好了。

# 基本原理

众所周知，评价一个生成模型，我们需要考验它两方面性能：1. 生成的图片**是否清晰**；2. 生成的图片**是否多样**。生成的图片不够清晰，显然说明生成模型表现欠佳；生成的图片够清晰了，我们还要看是不是能生成足够多样的图片，有些生成模型只能生成有限的几种清晰图片，陷入了所谓 mode collapse，也不是好的模型。

**Inception Score 是这样考虑这两个方面的：**

**1. 清晰度：**把生成的图片 x 输入 Inception V3 中，将输出 1000 维的向量 y ，向量的每个维度的值对应图片属于某类的概率。对于一个清晰的图片，它属于某一类的概率应该非常大，而属于其它类的概率应该很小（这个假设本身是有问题的，有可能有些图片很清晰，但是具体属于哪个类却是模棱两可的）。用专业术语说， p(y|x) 的熵应该很小（熵代表混乱度，均匀分布的混乱度最大，熵最大）。

**2. 多样性：**如果一个模型能生成足够多样的图片，那么它生成的图片在各个类别中的分布应该是平均的，假设生成了 10000 张图片，那么最理想的情况是，1000 类中每类生成了 10 张。转换成术语，就是生成图片在所有类别概率的边缘分布 p(y) 熵很大（均匀分布）。

具体计算时，可以先用生成器生成 N 张图片，然后用公式 (1) 的经验分布来代替：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3Ue1ryWTzqKPOA18ZpQuyJUxldKuia5nmTK9eU9BbBux6aYI6kiaNW917gA/640?wx_fmt=png)

综合上面两方面，Inception Score 的公式为：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UehQtwo3Fzq55rqPYP938uvZyzkic0zVYRNyQ5UDjibMoEugqpl2LVtAPg/640?wx_fmt=png)

exp：仅仅是为了好看，没有具体含义。

x~Pg ：表示从生成器中生图片。

p(y|x) ：把生成的图片 x 输入到 Inception V3，得到一个 1000 维的向量 y ，也就是该图片属于各个类别的概率分布。IS 提出者的假设是，对于清晰的生成图片，这个向量的某个维度值格外大，而其余的维度值格外小（也就是概率密度图十分尖）。

p(y) ：N 个生成的图片（N 通常取 5000），每个生成图片都输入到 Inception V3 中，各自得到一个自己的概率分布向量，把这些向量求一个平均，得到生成器生成的图片全体在所有类别上的边缘分布，见公式 (1)。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UeBUujsAjyQiaibvibHsgiaTnqd97DqtBvrdZ0icQbNuM9j8OAgb5AVI3vRaQ/640?wx_fmt=png)：对p(y|x)和 p(y) 求 KL 散度。KL 散度离散形式的公式如下：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UeAFhXq9ib9A8bZE3BUoYSTLkayj6yKZE5u8lMpucmVpBASeyJ4kpwfGQ/640?wx_fmt=png)

KL 散度用以衡量两个概率分布的距离，它是非负的，值越大说明这两个概率分布越不像。但这个距离不是对称的，观察公式， P(i) 很大 Q(i) 很小的地方对 KL 距离贡献很大，而 P(i) 很小 Q(i) 很大的地方对 KL 距离的贡献很小。

我们预期![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UeWQbDRonB6iaVdHuQiav0RKeru9wNUia6strVM36ot9HxOszZe3JmXLD8A/640?wx_fmt=png)的某个维度值很大，而 p(y) 总体均匀，因此需要把![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UeWQbDRonB6iaVdHuQiav0RKeru9wNUia6strVM36ot9HxOszZe3JmXLD8A/640?wx_fmt=png)放在公式 (2) 中双竖线的前面。放到后面可能会造成![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UeWQbDRonB6iaVdHuQiav0RKeru9wNUia6strVM36ot9HxOszZe3JmXLD8A/640?wx_fmt=png)的极端值被忽略，而正是这个极端值的存在告诉了我们这个生成的图片是否清晰。

综合起来，只要p(y|x)和 p(y) 的距离足够大，就能证明这个生成模型足够好。因为前者是一个很尖锐的分布，后者是一个均匀分布，这俩距离本就应该很大。

公式 (2) 很不直观，在实际操作中可以改成如下形式：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UeZtm60fkia5I95abFTJawtHahYJ6YquG2icGv0XibZN2MickO9F8spib6IwA/640?wx_fmt=png)

实际操作中，先用生成的大量样本代入公式 (1)，求出![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UevyicibzsnoAfnGr4vFSXn7T3icYdWF265x4uh1AGlSE4jUejsoTZxqMvw/640?wx_fmt=png)，然后再对每个样本求出![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UeWQbDRonB6iaVdHuQiav0RKeru9wNUia6strVM36ot9HxOszZe3JmXLD8A/640?wx_fmt=png)，计算它和![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UevyicibzsnoAfnGr4vFSXn7T3icYdWF265x4uh1AGlSE4jUejsoTZxqMvw/640?wx_fmt=png)的 KL 散度，最后求平均，再算一下指数即可。

Inception Score 的 pytorch 版本代码可以参考下面的链接，十分清晰易懂：

https://github.com/sbarratt/inception-score-pytorch

# 深入理解Inception Score

我们对公式 (2) 取对数刨除无用的 exp，并作简单的推导：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UefsiaLVxcTUpWIUAo3vzSHiaFia11yUonOSGkg891nHCoNHg4kA8H7Qia3Q/640?wx_fmt=png)

可以发现，公式 (4) 的结尾正是互信息的定义式，即：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UeCuhoFxSY9dwW7AfuATLtEranKEZXhHKSaeQ4MiaXIEY4CdFCAZMSEOg/640?wx_fmt=png)

注意，互信息是对称的，即 I(y;x)=I(x;y) 。而互信息的 Wikipedia 页面，给出了它和熵之间的关系推导。

# 熵，条件熵，信息增益和互信息

**熵是衡量随机变量不确定性的量**，对随机变量 x，其信息熵的公式为：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UeLiaBibuiaSDqyktA3HH9vQLQialSmLH6P2Dx0Z3DxDp6EgTzl2t8ks2tiaQ/640?wx_fmt=png)

**条件熵是衡量在给定条件下，随机变量不确定性的量。**对随机变量 x，y 条件熵的公式为：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3Ue0hvk0eTBmib8NQmpEkb8eKmlYY1Tgiah8BNcWvqibNbdxgD2hum24ibuqA/640?wx_fmt=png)

关于这两个公式的直观解释，请参阅**数学之美第 6 章**[1]。

而对于互信息，我们可以作如下推导：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UexkjClwuiaRWbuTnlast5pGQS5rPBAwpTvNfvfySrvtfLH8bGNyNkAzw/640?wx_fmt=png)

从公式 (8) 中发现，对于随机变量 y，其本来的不确定性是 H(y)，而给定了条件 x 后，y 的不确定性变为了 H(y|x)。因此，**互信息为：给定某个条件后，随机变量不确定性的减少程度，因此也叫信息增益。**而这个指标同时反映的是**两个变量的相关程度**。

互信息和信息增益是等价的，一个侧重表达两个随机变量的相关程度，一个侧重给定某个条件后，随机变量不确定性的减少程度。

PS：协方差也可以用于表示两个随机变量的相关性，但是只能表示两个变量之间的线性相关性，但协方差的好处是不需要知道变量的概率分布函数，而这对互信息是必须的。具体见**这个链接**[2]。

有了上面的推导，再代回到公式 (5)：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UeA6SORU4gdmJRiauttb9yg2HdC81yNyq50ya8XP939gdkCluEaShZL0w/640?wx_fmt=png)

我们根据公式 (9) 发现，Inception Score 的真实意义是：**生成的所有样本在各类别的分布函数的熵，与每个样本在各类别可能性的分布的熵（的期望）之差。**第一项越大，说明生成的样本在各类上分布越平均，第二项越小，说明生成的样本属于某个类别的可能性越大，说明这个样本越清晰。

# Inception Score的局限性

在*A Note on the Inception Score*中，作者首先举了一个一维的例子，用以说明 Inception Score 并不能反映生成模型的性能。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UecwicdDFfCdIu9yr4hT2ubOfOQePkQc1wp05cOMtDgzyZQ9l8CBUic88g/640?wx_fmt=jpeg)
▲图1

假设真实数据以 1/2 的概率分别从两个正态分布 N(-1, 2) 和 N(1,2) 中采样（如图 1），各自对应的类别分别为 0 和 1。那么最优贝叶斯分类器为：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UeFgahAoPUJygEBDtqwfONOnp12MG9VJWvohaic6l9KZzDn54HZMRWIpw/640?wx_fmt=png)

公式 10 的图像如图 2。即给出一个 x，分别算一下它在左正态分布和右正态分布的概率，代入上式，大于 0.5 表示来自右边的正态分布，小于 0.5 表示来自左边的正态分布。注意在 x=0 时，它来自左右两个分布的概率均为 0.5。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UeCJsTkd4MicHDCVgJdFwTzDTVGozr9EdqLrUkUTK3qezdYXTJ8ldSVJA/640?wx_fmt=jpeg)
▲图2

而根据 Inception score 的计算公式 (9)，我们可以设计这样的生成器：以相同概率随机生成 -∞ 和 +∞。为了方便阅读，这里重新复制一遍公式 (9)：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UepYhQWOM2shicGyjX0yvjM60je1PDnibOxwCsQ1dK7aLoehA5TaI668bQ/640?wx_fmt=png)

根据图 2 中最优判别器的图像，可以知道：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3Uejr1uiczNDGpMoSfu0jG4n0hS0yV36YFpmsF27j5K6ibnc8K5fGlscicvw/640?wx_fmt=png)

(11) 代入 (9) 的第二项，可以得到 H(y|x)=0，而生成-∞ 和 +∞的概率都是 1/2，故 H(y)=log2。因此，Inception Score 可以取到最大值 2。

除此之外，还有很多分布都可以得到很高的 Inception Score，例如均匀分布 U(-100,100)，正态分布 N(0, 20) ，因为它们都可以得到 H(y)=log2（关于坐标原点对称，所以生成两类的概率相同），同时得到一个很小的 H(y|x)=0（最优判别器下，大于 0 的数对应的概率都很大，小于 0 的数，概率都很小）。反而真正的分布，左右两个正态分布对应的 Inception Score 小于前面这些分布。

作者总结，**Inception Score 主要有两个问题：**

Inception Score 自身的问题

错误的使用场景

**Inception Score自身的问题**

**1. Inception Score 对神经网络内部权重十分敏感。**

作者利用 TensorFlow, Torch 和 Keras 等不同框架下预训练的 Inception V2 和 Inception V3 ，计算同一个数据库（50 k CIFAR-10 training images 和 50k ImageNet validation images）的 Inception Score。

发现尽管不同框架预训练的网络达到同样的分类精度，但由于其内部权重微小的不同，导致了 Inception Score 很大的变化，在 CIFAR-10 上 IV3 (inception v3) Torch 和 IV3 Keras 算出的 Inception Score 相差了 11.5% ，在 ImageNet 上，IV3 Torch 和 IV3 Keras 算出的 IS 相差 3.5%，这些差异，足以涵盖某些所谓 state-of-the-art 的模型所作出的提升。

**2. 计算 Inception Score 的方式不对。**

通常计算 Inception Score 时，会生成 50000 个图片，然后把它分成 10 份，每份 5000 个，分别代入公式 (3) 计算 10 次 Inception Score，再计算均值和方差，作为最终的衡量指标（均值+-方差）。

但是 5000 个样本往往不足以得到准确的边缘分布![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UevyicibzsnoAfnGr4vFSXn7T3icYdWF265x4uh1AGlSE4jUejsoTZxqMvw/640?wx_fmt=png)，尤其是像 ImageNet 这种包含 1000 个类的数据集，仅仅 5000 个样本是不够的。作者通过实验证明，把同一个数据集分成不同的份数，会影响最后的 Inception Score。因此，**作者建议把 50000 个样本放到一起，来确定边缘分布**![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UevyicibzsnoAfnGr4vFSXn7T3icYdWF265x4uh1AGlSE4jUejsoTZxqMvw/640?wx_fmt=png)。

同时，**作者建议去掉 Inception Score 提出者基于审美原因加进公式的 exp**，根据上文，去掉后可以直接解释为互信息。改进后的 Inception Score 公式为：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UeT06IeDTCIWcs9Fy03BTzicy56mYg8fD0cjjiaSmvUU3vkSJAfteK8Q2A/640?wx_fmt=png)

按照作者的改进，就不会出现把数据集划分为不同份数，计算的 Inception Score 不一致的现象了。对于方差，可以通过计算每一个生成数据的![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UeKviaianCjnwHqOiao9OhXf0JiclPCb4FC4oQsUiamnSnnM9YuGK9QCKMRBg/640?wx_fmt=png)来计算（相当于 n_split = N）。

**错误的使用场景**

**1. 分类模型和生成模型在不同的数据集上训练。**

由于 Inception V3 是在 ImageNet 上训练的，用 Inception V3 时，应该保证生成模型也在 ImageNet 上训练并生成 ImageNet 相似的图片，而不是把什么生成模型生成的图片（卧室，花，人脸）都往 Inception V3 中套，那种做法没有任何意义。

**Inception Score 基于两个假设：**

Inception V3 可以准确估计 p(y)，即样本在所有类别上的边缘分布；

Inception V3 可以准确估计 p(y|x) ，从而计算出条件熵，用条件熵反映图片的真实程度。

对于假设 1，作者计算了 CIFAR-10 的边缘分布，取了排名前 10 的预测类。把这 10 类和 CIFAR-10 的 10 类相比较，发现，它们并没有对应关系。再一次说明了生成模型的训练数据需要和 Inception Net 或者别的什么分类网络的训练数据一致。**不能在一个数据集上训练分类模型，用来评估另一个数据集上训练的生成模型。**

对于假设 2，Inception 网络通过 p(y|x) 计算出条件熵，条件熵越低表示生成的图片越真实，这也是有问题的。作者计算了 CIFAR 10 训练集的条件熵是 4.664 bit，而在随机噪声图片上，条件熵是 6.512 bit，仅仅比真实训练集图片高了一点点。

而在 ImageNet 的 validation set上，Inception net 算出的条件熵是 1.97 bit，也就是说 CIFAR 训练数据比起 ImageNet 更接近随机噪声，这显然是不科学的，因为 CIFAR 数据再不济也是真实图片，应该和 ImageNet 的条件熵更接近才对。再一次说明了，**不能在一个数据集上训练分类模型，用来评估另一个数据集上训练的生成模型。**

**2. 优化 Inception Score （不直接地&隐式地）。**

Inception Score 只能是粗糙的指导，如果直接优化这个 Inception Score，会导致生成对抗样本（只会刷分，其实并不真实）。但同时也应该注意到，间接地优化这个分数，同样会导致生成对抗样本，例如用这个指标来确定是否停止训练，调整超参数，甚至调整网络架构。

作者在附录中提出了一种**可以****刷 Inception Score 的方法：**把生成样本输入分类模型中，用梯度下降来修改输入的样本，让它在某一类的概率达到非常大，这样，条件熵就降下来了，同时让不同样本优化不同的类，循环遍历所有的类，这样就能保证边缘分布是均匀分布，即生成的图片把每个类都均匀覆盖了。但是，这会导致生成毫无意义的图片。

**3. 没有反映过拟合。**

根据 Inception Score 的计算原理，我们可以发现：如果神经网络记住了所有的训练集图片，然后随机输出，那么它会得到一个很高的 Inception Score。但是这种生成模型是没有意义的。

因此在用 Inception Score 评估生成模型的性能时，应该加上别的指标，证明模型没有过拟合，即模型输出的图片和训练集中任何图片都不同，单纯用 Inception Score 评估性能是不全面的。

# 总结

本文全面讲解了 Inception Score 的原理和它存在的一些问题：

IS大，不一定生成的图片就真实；

分类模型参数的轻微变动将影响 IS；

使用 IS 时，分类模型和生成模型应该在同一个数据集上训练；

通常计算 IS 的方法是有漏洞的：估计![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkhxySIqYdGexoX8Kt3u3UevyicibzsnoAfnGr4vFSXn7T3icYdWF265x4uh1AGlSE4jUejsoTZxqMvw/640?wx_fmt=png)的样本数据量太小，导致同一堆数据，分割的份数不同算出的 IS 不同；

以 IS 为优化目标会导致产生对抗样本；

IS 无法反映生成模型过拟合情况。

综上，IS 是一个浑身硬伤的评价指标，能不用还是不要用了……

经常与 IS 并列的还有 Fréchet Inception Distance，关于它的简介请移步本人的另一篇文章[3]。限于水平，这篇文章会比较水，没能对 Fréchet Distance 做出理论推导或者直观解释，只是引用了一下公式，但从公式中仍然可以看出，它比 IS 更加有道理一点。

# 相关链接

[1] https://github.com/Tongzhenguo/ebooks/blob/master/数学之美.pdf
[2] https://stats.stackexchange.com/questions/81659/mutual-information-versus-correlation
[3] https://zhuanlan.zhihu.com/p/54213305

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)


**点击以下标题查看更多往期内容：**

[自动机器学习（AutoML）最新综述](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492654&idx=1&sn=b9047d5cca7657f02dc7f6685ef04037&chksm=96ea3baea19db2b8dc1c1267801d0c585b3cf072531af86abdeb73c6fb4c07dc3325c2d13d57&scene=21#wechat_redirect)
[图神经网络综述：模型与应用](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493906&idx=1&sn=15c9f18a1ce6baa15dc85ecb52e799f6&chksm=96ea3692a19dbf847c1711e6e194ad60d80d11138daf0938f90489a054d77cfd523bee2dc1d2&scene=21#wechat_redirect)
[近期值得读的10篇GAN进展论文](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493987&idx=1&sn=ce1bcdce28e78f4a307743e389f42b10&chksm=96ea36e3a19dbff5cff7f4f1c9d9fc482bb2144d80566319b3d26bce4d9ab80689d38ab2e427&scene=21#wechat_redirect)

[自然语言处理中的语言模型预训练方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492317&idx=1&sn=e823a75d9463257ed9ea7b3e4677c1ae&chksm=96ea3d5da19db44be0872ff4e29043aa72c7a624a116196bfeeca092a15f9209d7cf8ce46eb5&scene=21#wechat_redirect)
[从傅里叶分析角度解读深度学习的泛化能力](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491082&idx=1&sn=d7c1cb39c3be43154c658ca5a791eb4c&chksm=96e9c18aa19e489c32fe36671e4208ce42bf200e3a7adeda200fa2785462d16f85c58bb455b4&scene=21#wechat_redirect)
[两行代码玩转Google BERT句向量词向量](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493033&idx=1&sn=1ae1cd347126b10d6a857cd9bba7b601&chksm=96ea3a29a19db33f3c07723ed6e5ecbb8d2ff1b1617f1cf0d39cb3cc1e6e9c325cc29147d58d&scene=21#wechat_redirect)
AI Challenger 2018 机器翻译参赛总结
[TensorSpace：超酷炫3D神经网络可视化框架](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492746&idx=1&sn=921d7315a973b85dd4e802cb5fd456fb&chksm=96ea3b0aa19db21c48841ddcee38592a3c086ae8fa1a9893cf46ff974f0f38fb350bcd528265&scene=21#wechat_redirect)
[深度长文：NLP的巨人肩膀（上）](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493520&idx=1&sn=2b04c009ef75291ef3d19e8fe673aa36&chksm=96ea3810a19db10621e7a661974c796e8adeffc31625a769f8db1d87ba803cd58a30d40ad7ce&scene=21#wechat_redirect)
[NLP的巨人肩膀（下）：从CoVe到BERT](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493731&idx=1&sn=51206e4ca3983548436d889590ab5347&chksm=96ea37e3a19dbef5b6db3143eb9df822915126d3d8f61fe73ddb9f8fa329d568ec79a662acb1&scene=21#wechat_redirect)



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****投 稿 通 道****\#**
**让你的论文被更多人看到**

如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？**答案就是：你不认识的人。**

总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。

PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。

📝**来稿标准：**
• 稿件确系个人**原创作品**，来稿需注明作者个人信息（姓名+学校/工作单位+学历/职位+研究方向）
• 如果文章并非首发，请在投稿时提醒并附上所有已发布链接
• PaperWeekly 默认每篇文章都是首发，均会添加“原创”标志

**📬 投稿邮箱：**
• 投稿邮箱：hr@paperweekly.site
• 所有文章配图，请单独在附件中发送
• 请留下即时联系方式（微信或手机），以便我们在编辑发布时和作者沟通



🔍

现在，在**「知乎」**也能找到我们了
进入知乎首页搜索**「PaperWeekly」**
点击**「关注」**订阅我们的专栏吧


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)
▽ 点击 |阅读原文| 获取最新论文推荐



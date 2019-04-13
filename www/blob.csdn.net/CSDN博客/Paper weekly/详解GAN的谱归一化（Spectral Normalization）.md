
# 详解GAN的谱归一化（Spectral Normalization） - Paper weekly - CSDN博客


2019年02月13日 08:52:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：591


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

作者丨尹相楠
学校丨里昂中央理工博士在读
研究方向丨人脸识别、对抗生成网络

本文主要介绍谱归一化这项技术，详细论文参考*Spectral Normalization for Generative Adversarial Networks*。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7hnXQkPdQLlkibjvhXY8RmibzZfbpkTBBIWNZtfUPMX5QKJFavYkAPS2A/640?wx_fmt=png)

本文主要对论文中的基础知识和遗漏的细节做出补充，以便于更好地理解谱归一化。部分内容参考并整合了如下两篇博客。

> http://kaiz.xyz/blog/posts/spectral-norm/


> https://christiancosgrove.com/blog/2018/01/04/spectral-normalization-explained.html


# Lipschitz Continuity

在 GAN 中，假设我们有一个判别器 D: I→R，其中 I 是图像空间。如果判别器是 K-Lipschitz continuous 的，那么对图像空间中的任意 x 和 y，有：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7J0ib5tWdIkJ6hnwial8r20aOgMrSZPpRHCvCKhibJ9VqVEEPdO7a5rQjw/640?wx_fmt=png)

其中 || · || 为 L2 norm，如果 K 取到最小值，那么 K 被称为 Lipschitz constant。

直观地来说，Lipschitz 条件限制了函数变化的剧烈程度，即函数的梯度。在一维空间中，很容易看出 y=sin(x) 是 1-Lipschitz 的，它的最大斜率是 1。

那么，为什么要使判别器函数具有 Lipschitz continuity 呢？Wasserstein GAN 提出了用 wasserstein 距离取代之前的 KL 散度或者 JS 散度，作为 GAN 判别器的损失函数：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7tUpShUGnEXHPPgmSpuFnNe3Oh5fSTsTUaeuqscEZNPCKIZs5vtIYcw/640?wx_fmt=png)

其中 Pr 和 Pg 分别为真实数据和生成数据的分布函数，Wasserstein 距离衡量了这两个分布函数的差异性。

直观地理解，就是根据这两个分布函数分别生成一堆数据 x1, x2, ... , xn 和另一堆数据 y1, y2, ... , yn，然后计算这两堆数据之间的距离。距离的算法是找到一种一一对应的配对方案 γ~∏(Pr, Pg)，把 xi 移动到 yj，求总移动距离的最小值。

由于在 GAN 中， Pr 和 Pg 都没有显式的表达式，只能是从里面不停地采样，所以不可能找到这样的γ，无法直接优化公式 (2) 。W-GAN 的做法是根据 Kantorovich-Rubinstein duality，将公式 (2) 转化成公式 (3)，过程详见以下链接：

https://vincentherrmann.github.io/blog/wasserstein/

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7CY0IJfSZCjeCKkTmGWkJzDfIJ7zoM1bSY65iaZf0N4wh5icOVNgGNMUg/640?wx_fmt=png)

其中 f 即为判别器函数。只有当判别器函数满足 1-Lipschitz 约束时，(2) 才能转化为 (3)。除此之外，正如上文所说，Lipschitz continuous 的函数的梯度上界被限制，因此函数更平滑，在神经网络的优化过程中，参数变化也会更稳定，不容易出现梯度爆炸，因此 Lipschitz continuity 是一个很好的性质。

为了让判别器函数满足 1-Lipschitz continuity，W-GAN 和之后的 W-GAN GP 分别采用了 weight-clipping 和 gradient penalty 来约束判别器参数。这里的谱归一化，则是另一种让函数满足 1-Lipschitz continuity 的方式。

# 矩阵的Lipschitz Continuity

众所周知，矩阵的乘法是线性映射。对线性映射来说，如果它在零点处是 K-Lipschitz 的，那么它在整个定义域上都是 K-Lipschitz 的。

想象一条过零点的直线，它的斜率是固定的，只要它上面任何一点是 K-Lipschitz 的，那么它上面所有点都是 K-Lipschitz 的。因此，对矩阵![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7TQgBUot9kAbxVicLd8vcUpgYeZQAIc3ya3ibvL3QdCLINGKE2BKboN5g/640?wx_fmt=png)来说，它满足 K-Lipschitz 的充要条件是：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7iaBwwzrvZgHpJeNGicXexE3gx2dd1vvEZNUr2kI2ziccschb2ZcP9597w/640?wx_fmt=png)

对其做如下变换：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr78Ztwv80gbpX05Fs67MuGHlTx2xUysicbBicoZLv8nvSO6ic98C5vRicMzQ/640?wx_fmt=png)

其中 <·> 表示两个向量的内积。由于矩阵![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr74tkUvD1M2VzzCOQ4vY4tkyx1tXRDQPbkAfDZbLDxMYMYpiaOqkAgReQ/640?wx_fmt=png)是半正定矩阵，它的所有特征值均为非负。我们假设它的特征向量构成的基底为 v1, v2,..., vn，对应的特征值为 λ1, λ2,..., λn，令 x=x1·v1+x2·v2+ ... +xn·vn。那么，公式 (5) 的左半部分可以转化为：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7iaJCDhUuCGzCSR4gRJcmkNuPFGWKrexa4K6hcMrBdBUxzeicTP9A3lzA/640?wx_fmt=png)

要使公式 (6) 对任意 xi 恒成立，且λi 非负，则必有![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7JKoKPJ0AC6O2PpLJS7UhKPMegy28vGfRwevXW0AN8S6OGz8gwRrpdQ/640?wx_fmt=png)。若λ1 为![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr74tkUvD1M2VzzCOQ4vY4tkyx1tXRDQPbkAfDZbLDxMYMYpiaOqkAgReQ/640?wx_fmt=png)最大的特征值，只需要满足![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7l1HlU7Nhcn8nqztQsnvOKvkoz6KC3hTUeicjRhCmLsC9MDrDP0NqECg/640?wx_fmt=png)，这里![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7R8YCNfoHD5tZpneDNbEsQjoeFlbKyo2icBwtEvzuU0dmvWibukbczIicg/640?wx_fmt=png)即为矩阵 A 的 spectral norm。

综上所述，映射![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7TQgBUot9kAbxVicLd8vcUpgYeZQAIc3ya3ibvL3QdCLINGKE2BKboN5g/640?wx_fmt=png)满足 K-Lipschitz 连续，K 的最小值为![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7R8YCNfoHD5tZpneDNbEsQjoeFlbKyo2icBwtEvzuU0dmvWibukbczIicg/640?wx_fmt=png)。那么，要想让矩阵 A 满足 1-Lipschitz 连续，只需要在 A 的所有元素上同时除以![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7R8YCNfoHD5tZpneDNbEsQjoeFlbKyo2icBwtEvzuU0dmvWibukbczIicg/640?wx_fmt=png)即可（观察公式 (4) 左侧是线性映射）。

通过上面的讨论，我们得出了这样的结论：**矩阵 A 除以它的 spectral norm（**![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr74tkUvD1M2VzzCOQ4vY4tkyx1tXRDQPbkAfDZbLDxMYMYpiaOqkAgReQ/640?wx_fmt=png)**最大特征值的开根号**![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7R8YCNfoHD5tZpneDNbEsQjoeFlbKyo2icBwtEvzuU0dmvWibukbczIicg/640?wx_fmt=png)**）可以使其具有 1-Lipschitz continuity。**

# 矩阵的奇异值分解

上文提到的矩阵的 spectral norm 的另一个称呼是矩阵的**最大奇异值**。回顾矩阵的 SVD 分解：

矩阵![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7TQgBUot9kAbxVicLd8vcUpgYeZQAIc3ya3ibvL3QdCLINGKE2BKboN5g/640?wx_fmt=png)存在这样的一种分解：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7kmFmnpHsic9GTbfIzlvGHjqc64tbIp8HyicKicMw6icA7I1erKu4ZoyNGA/640?wx_fmt=png)

其中：

U 是一个 m × m 的单位正交基矩阵
Σ 是一个 m×n 的对角阵，对角线上的元素为奇异值，非对角线上的元素为 0
V 是一个 n×n 的单位正交基矩阵

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr74miciaz8re2ibs6fyfgMIcCTf7MTvFs0LgtRZmiagPV2YdBgLSZObsrCDg/640?wx_fmt=png)
**▲**SVD分解

由于 U 和 V 都是单位正交基，因此可以把矩阵乘以向量分成三步：旋转，拉伸，旋转。一前一后的两步旋转不改变向量的模长，唯一改变向量模长的是中间的拉伸，即与Σ相乘的那一步。而矩阵的 Lipschitz continuity 关心的正是矩阵对向量模长的改变，不关心旋转。

因此，只需要研究中间的Σ即可。而Σ又是一个对角矩阵，因此，它对向量的模长拉长的最大值，就是对角线上的元素的最大值。也就是说，矩阵的最大奇异值即为它的 spectral norm。

根据公式 (7) ，以及 SVD 分解中 U 和 V 都是单位正交基，单位正交基的转置乘以它本身为单位矩阵，有：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7t2LvUCWLtraAfGJNicSibjkPLVzWDUukV6siawE5aMn3nvmocIVxc2uog/640?wx_fmt=png)

因此，只需要求出![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr74tkUvD1M2VzzCOQ4vY4tkyx1tXRDQPbkAfDZbLDxMYMYpiaOqkAgReQ/640?wx_fmt=png)的最大特征值，再开根号，就求出了矩阵的最大奇异值，也就是矩阵的 spectral norm，和上一小节的推导殊途同归。

# 神经网络的Spectral Norm

对于复合函数，我们有这样的定理：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7Ryn5icicNp7EvvOlotevNWGBnicNLRTbRzNFzmsCX0EiciauB1328G7U9KQ/640?wx_fmt=png)

而多层神经网络，正是多个复合函数嵌套的操作。最常见的嵌套是：一层卷积，一层激活函数，再一层卷积，再一层激活函数，这样层层包裹起来。

激活函数通常选取的 ReLU，Leaky ReLU 都是 1-Lipschitz 的，带入到 (9) 中相乘不影响总体的 Lipschitz constant，我们只需要保证卷积的部分是 1-Lipschitz continuous 的，就可以保证整个神经网络都是 1-Lipschitz continuous 的。

而在图像上每个位置的卷积操作，正好可以看成是一个矩阵乘法。因此，我们只需要约束各层卷积核的参数 W，使它是 1-Lipschitz continuous 的，就可以满足整个神经网络的 1-Lipschitz continuity。

我们已经知道，想让矩阵满足 1-Lipschitz continuous，只需要让它所有元素同时除以它的最大奇异值，或者说是它的 spectural norm。因此，下一步的问题是如何计算 W 的最大奇异值。

对大矩阵做 SVD 分解运算量很大，我们不希望在优化神经网络的过程中，每步都对卷积核矩阵做一次 SVD 分解。一个近似的解决方案是**power iteration 算法**。

# Power Iteration

Power iteration 是用来近似计算矩阵最大的特征值（dominant eigenvalue 主特征值）和其对应的特征向量（主特征向量）的。

假设矩阵 A 是一个 n×n 的满秩的方阵，它的单位特征向量为 v1, v2,..., vn，对应的特征值为λ1,λ2,...,λn。那么对任意向量x=x1·v1+x2·v2+ ... +xn·vn，有：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7ZPotXbw2ds5K0qswNyNypUhNXgGia9eXyaMibyJFwWDcXbK0NicT7a4MQ/640?wx_fmt=png)

我们经过 k 次迭代：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7REFrMeQ1ZJt5102I8tehsxqnC98ExiaS9MVvOszic2Q7IT9S0IF0Zb8A/640?wx_fmt=png)
由于λ1,λ2,...,λn（不考虑两个特征值相等的情况，因为太少见了）。可知，经过 k 次迭代后![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7RrJZNvxia1AclV4ft8UnDIry7vXzXVKw7hia6wdR1X8V5Yo4DiazfOduA/640?wx_fmt=png)。因此：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7GIyP9kHjUxBhrE3OicSfhQ16C4rqC2XTNJTsVwwG9YOL6ciaPMP0Eibrg/640?wx_fmt=png)
也就是说，经过 k 次迭代后，我们将得到矩阵主特征向量的线性放缩，只要把这个向量归一化，就得到了该矩阵的单位主特征向量，进而可以解出矩阵的主特征值。

而我们在神经网络中，想求的是权重矩阵 W 的最大奇异值，根据上面几节的推导，知道这个奇异值正是![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7V7iajELREDebZB0pKnicic6NAwuNZeM8zZGW0D3lqTJVmDCRibrUMJvrog/640?wx_fmt=png)最大特征值的开方![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7R8YCNfoHD5tZpneDNbEsQjoeFlbKyo2icBwtEvzuU0dmvWibukbczIicg/640?wx_fmt=png)。因此，我们可以采用 power iteration 的方式求解![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7V7iajELREDebZB0pKnicic6NAwuNZeM8zZGW0D3lqTJVmDCRibrUMJvrog/640?wx_fmt=png)的单位主特征向量，进而求出最大特征值λ1。论文中给出的算法是这样的：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7ZfkQul83W929Muhic6aLNx44fj9CTz69vRnCEzXVE2m1RV62OfjRZzg/640?wx_fmt=png)

如果单纯看分子，我们发现这两步合起来就是![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7jGLEJgDiauszGKiasPtcUSIRFJYQEFRJNhOkNBPcXuIiajTjrnqIvwUfg/640?wx_fmt=png)，反复迭代 (13) 中上下两个式子 ，即可得到矩阵![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7V7iajELREDebZB0pKnicic6NAwuNZeM8zZGW0D3lqTJVmDCRibrUMJvrog/640?wx_fmt=png)的单位主特征向量![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7uSllbcojtDqVmYkcHg6VSJZFI5omzpBX6O3wAxwP9qLMZBBHqb14ibA/640?wx_fmt=png)。只不过 (13) 的每算“半”步都归一化一次。

其实这种归一化并不影响向量![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7uSllbcojtDqVmYkcHg6VSJZFI5omzpBX6O3wAxwP9qLMZBBHqb14ibA/640?wx_fmt=png)的方向收敛到主特征向量的方向，而只影响特征向量前面的系数。而每步归一化一次的好处是，每步都可以得到单位主特征向量的近似解。

那么，知道![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7V7iajELREDebZB0pKnicic6NAwuNZeM8zZGW0D3lqTJVmDCRibrUMJvrog/640?wx_fmt=png)的单位主特征向量![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7uSllbcojtDqVmYkcHg6VSJZFI5omzpBX6O3wAxwP9qLMZBBHqb14ibA/640?wx_fmt=png)后，如何求出最大特征值λ1呢？

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7E8mle7pXryibMvfz3GogJZUias8A0VEZs83W8VYXXjD1NG18l0EfsRNA/640?wx_fmt=png)

而将公式 (13) 的第二个式子两边同时左乘![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7H5gohjZOZw9Kpz1pia2azRnB4nWficPAflZ8dYJkIkGTNbJiatD5NUnLA/640?wx_fmt=png):

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7lJAlGicYQaicwKguoPiceTibWQu0QXM29M36atSB4RLhnxMOYfjUKP0Tdw/640?wx_fmt=png)

最终，(15) 即为论文中提出的权重矩阵 W 的 spectral norm 公式。

而在具体的代码实现过程中，可以随机初始化一个噪声向量![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7kw9rcTcr4KfyH05JxCgdpIQQ59CiaLLxMadNhhQ3ttbtyxSibXyFepgw/640?wx_fmt=png)代入公式 (13) 。由于每次更新参数的 step size 很小，矩阵 W 的参数变化都很小，矩阵可以长时间维持不变。

因此，可以把参数更新的 step 和求矩阵最大奇异值的 step 融合在一起，即每更新一次权重 W ，更新一次![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7RGbhevAI5ibP4wd71UoB24Pj0bXP5WuYsuGO3L6oWVEVa6Pb0SfoykA/640?wx_fmt=png)和![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAsLuPmn95Erz6nImbKicr7uSllbcojtDqVmYkcHg6VSJZFI5omzpBX6O3wAxwP9qLMZBBHqb14ibA/640?wx_fmt=png)，并将矩阵归一化一次（除以公式 (15) 近似算出来的 spectral norm）。

具体代码见：

https://github.com/christiancosgrove/pytorch-spectral-normalization-gan

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)


**点击以下标题查看更多往期内容：**

[Airbnb实时搜索排序中的Embedding技巧](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494488&idx=1&sn=452ac80c593a9b31252031eac38d0e01&chksm=96ea34d8a19dbdce940ed25bb93507aa6c4d118f84dd0bb965b060f232fe5d41894bbc9edcb6&scene=21#wechat_redirect)
[图神经网络综述：模型与应用](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493906&idx=1&sn=15c9f18a1ce6baa15dc85ecb52e799f6&chksm=96ea3692a19dbf847c1711e6e194ad60d80d11138daf0938f90489a054d77cfd523bee2dc1d2&scene=21#wechat_redirect)
[近期值得读的10篇GAN进展论文](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493987&idx=1&sn=ce1bcdce28e78f4a307743e389f42b10&chksm=96ea36e3a19dbff5cff7f4f1c9d9fc482bb2144d80566319b3d26bce4d9ab80689d38ab2e427&scene=21#wechat_redirect)
[F-Principle：初探理解深度学习不能做什么](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494694&idx=1&sn=7020fb834ce8307f27ce9c072047d37d&chksm=96ea33a6a19dbab0a6585daa00d5b5c65501dd633fa677c80541fad0e170d92baffe379315c3&scene=21#wechat_redirect)

[自然语言处理中的语言模型预训练方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492317&idx=1&sn=e823a75d9463257ed9ea7b3e4677c1ae&chksm=96ea3d5da19db44be0872ff4e29043aa72c7a624a116196bfeeca092a15f9209d7cf8ce46eb5&scene=21#wechat_redirect)
[两行代码玩转Google BERT句向量词向量](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493033&idx=1&sn=1ae1cd347126b10d6a857cd9bba7b601&chksm=96ea3a29a19db33f3c07723ed6e5ecbb8d2ff1b1617f1cf0d39cb3cc1e6e9c325cc29147d58d&scene=21#wechat_redirect)
[AI Challenger 2018 机器翻译参赛总结](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494103&idx=1&sn=fc372862e0369b1f6a943bf997f6fc1b&chksm=96ea3657a19dbf4108bbc4179e779aa04ef05fe84f0013fa6425b0cd7e761e9880917361c4c1&scene=21#wechat_redirect)
[Google BERT应用之红楼梦对话人物提取](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494507&idx=1&sn=3c3cafef0fb51a7e40d9b9bbab53fd5f&chksm=96ea34eba19dbdfd31eaa760bb7cfd5e18f2e967c83c6ea6693ad9a062c55b3009211d824ca3&scene=21#wechat_redirect)
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



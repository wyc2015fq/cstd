# 【NLP】基于GAN的文本生成综述 - zkq_1986的博客 - CSDN博客





2018年09月30日 10:09:44[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：1417








**论文一、**《Generative Adversarial Nets》NIPS 2014

![](https://pic3.zhimg.com/80/v2-25a69d382aadeedcfe4c6d8c5adcae19_hd.jpg)

**1、模型简述**

这篇论文是最早提出 GAN 的文章，作者 Ian J. Goodfellow 提出了一种新的对抗过程来评价生成模型的效果。GAN 主要分为两部分：生成模型和判别模型。生成模型的作用是模拟真实数据的分布，判别模型的作用是判断一个样本是真实的样本还是生成的样本，GAN 的目标是训练一个生成模型完美的拟合真实数据分布使得判别模型无法区分。

生成模型和判别模型之间的交互可以类比为这样一个场景：生成模型是一个生产假币的伪造者，他的任务就是要生成假币然后使用假币而不被发现，判别模型则是一个警察，他的任务则是识别出那些假币，两者之间的较量使得伪造者不断提升制造假币的能力，警察不断提升识别假币的能力，最终警察无法区分伪造者生产的假币和真实的货币。

**2、数学表示**

真实数据分布： ![x\sim p_{data}(x)](https://www.zhihu.com/equation?tex=x%5Csim+p_%7Bdata%7D%28x%29)

生成模型： ![G(z;\theta_g)](https://www.zhihu.com/equation?tex=G%28z%3B%5Ctheta_g%29) ，其中 ![z](https://www.zhihu.com/equation?tex=z) 是噪声输入，满足分布 ![z\sim p_z(z)](https://www.zhihu.com/equation?tex=z%5Csim+p_z%28z%29) ， ![\theta_g](https://www.zhihu.com/equation?tex=%5Ctheta_g) 是参数，最终生成的样本满足分布 ![p_g](https://www.zhihu.com/equation?tex=p_g) ，生成模型的目标是要尽量使得 ![p_g](https://www.zhihu.com/equation?tex=p_g) 拟合 ![p_{data}](https://www.zhihu.com/equation?tex=p_%7Bdata%7D) 。

判别模型： ![D(x;\theta_{d})](https://www.zhihu.com/equation?tex=D%28x%3B%5Ctheta_%7Bd%7D%29) ，其中 ![x](https://www.zhihu.com/equation?tex=x) 是输入的样本，![\theta_d](https://www.zhihu.com/equation?tex=%5Ctheta_d) 是参数，最终输出 ![x](https://www.zhihu.com/equation?tex=x) 来自 ![p_{data}](https://www.zhihu.com/equation?tex=p_%7Bdata%7D) 而不是 ![p_g](https://www.zhihu.com/equation?tex=p_g) 的概率。

最后的训练目标为：

![\min \limits_{G} \max \limits_{D} V(D,G)=\mathbb{E}_{x\sim p_{data}(x)}[\log D(x)] + \mathbb{E}_{z\sim p_z(z)}[\log (1-D(G(z))]](https://www.zhihu.com/equation?tex=%5Cmin+%5Climits_%7BG%7D+%5Cmax+%5Climits_%7BD%7D+V%28D%2CG%29%3D%5Cmathbb%7BE%7D_%7Bx%5Csim+p_%7Bdata%7D%28x%29%7D%5B%5Clog+D%28x%29%5D+%2B+%5Cmathbb%7BE%7D_%7Bz%5Csim+p_z%28z%29%7D%5B%5Clog+%281-D%28G%28z%29%29%5D+)

解释一下：

对于生成模型 ![G](https://www.zhihu.com/equation?tex=G) ，它希望 ![D](https://www.zhihu.com/equation?tex=D) 判别 真实数据![x](https://www.zhihu.com/equation?tex=x) 的概率低，判别生成数据 ![G(z)](https://www.zhihu.com/equation?tex=G%28z%29) 的概率高，所以 ![V(D,G)](https://www.zhihu.com/equation?tex=V%28D%2CG%29) 的值小；

对于判别模型 ![D](https://www.zhihu.com/equation?tex=D) ，它希望判别真实数据 ![x](https://www.zhihu.com/equation?tex=x) 的概率高，判别生成数据 ![G(z)](https://www.zhihu.com/equation?tex=G%28z%29) 的概率低，所以![V(D,G)](https://www.zhihu.com/equation?tex=V%28D%2CG%29) 的值大。

**3、算法**

![](https://pic2.zhimg.com/80/v2-0ea336b69337d62114da7bd785153daf_hd.jpg)

这是训练 GAN 的算法，在训练时，并不是生成模型和判别模型交替训练，而是训练 ![k](https://www.zhihu.com/equation?tex=k) 次判别模型同时训练1次生成模型。在训练早期，判别模型 ![D](https://www.zhihu.com/equation?tex=D) 能够轻松否定 ![G(z)](https://www.zhihu.com/equation?tex=G%28z%29) ， ![D(G(z))](https://www.zhihu.com/equation?tex=D%28G%28z%29%29)值很小，![\log (1-D(G(z)))](https://www.zhihu.com/equation?tex=%5Clog+%281-D%28G%28z%29%29%29) 接近饱和，因此对于 ![G](https://www.zhihu.com/equation?tex=G) 而言，我们不试图优化![\min \limits_G \log(1-D(G(z)))](https://www.zhihu.com/equation?tex=%5Cmin+%5Climits_G+%5Clog%281-D%28G%28z%29%29%29) ，而是 ![\max \limits_G \log(D(G(z)))](https://www.zhihu.com/equation?tex=%5Cmax+%5Climits_G+%5Clog%28D%28G%28z%29%29%29) 。

训练的最后将达到一个全局最优条件： ![p_g=p_{data}=0.5](https://www.zhihu.com/equation?tex=p_g%3Dp_%7Bdata%7D%3D0.5) ，同时若固定 ![G](https://www.zhihu.com/equation?tex=G) ， ![D_G^*(x)](https://www.zhihu.com/equation?tex=D_G%5E%2A%28x%29) 收敛为 ![\frac {p_{data}(x)}{p_{data}(x)+p_g(x)}](https://www.zhihu.com/equation?tex=%5Cfrac+%7Bp_%7Bdata%7D%28x%29%7D%7Bp_%7Bdata%7D%28x%29%2Bp_g%28x%29%7D) 。

> 证明：

若固定 ![G](https://www.zhihu.com/equation?tex=G) ，则训练目标为 ![\max \limits_{D} V(G,D)](https://www.zhihu.com/equation?tex=%5Cmax+%5Climits_%7BD%7D+V%28G%2CD%29) 。
![\begin{align} V(D,G) &= \mathbb{E}_{x\sim p_{data}(x)}[\log D(x)] + \mathbb{E}_{z\sim p_z(z)}[\log (1-D(G(z))] \\ &= \int_{x} p_{data}(x)\log(D(x))dx + \int_{z} p_z(z) \log(1-D(G(z)))dz \\ &= \int_{x} p_{data}(x)\log(D(x))dx + p_g(x) \log(1-D(G(x)))dx \end{align} \\](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+V%28D%2CG%29+%26%3D+%5Cmathbb%7BE%7D_%7Bx%5Csim+p_%7Bdata%7D%28x%29%7D%5B%5Clog+D%28x%29%5D+%2B+%5Cmathbb%7BE%7D_%7Bz%5Csim+p_z%28z%29%7D%5B%5Clog+%281-D%28G%28z%29%29%5D+%5C%5C+%26%3D+%5Cint_%7Bx%7D+p_%7Bdata%7D%28x%29%5Clog%28D%28x%29%29dx+%2B+%5Cint_%7Bz%7D+p_z%28z%29+%5Clog%281-D%28G%28z%29%29%29dz+%5C%5C+%26%3D+%5Cint_%7Bx%7D+p_%7Bdata%7D%28x%29%5Clog%28D%28x%29%29dx+%2B+p_g%28x%29+%5Clog%281-D%28G%28x%29%29%29dx+%5Cend%7Balign%7D+%5C%5C)

对于区间 ![(a,b) \in \mathbb{R}^2 \backslash \{0,0\}](https://www.zhihu.com/equation?tex=%28a%2Cb%29+%5Cin+%5Cmathbb%7BR%7D%5E2+%5Cbackslash+%5C%7B0%2C0%5C%7D) 来说，函数 ![x\rightarrow a\log(x) + b\log(1-x)](https://www.zhihu.com/equation?tex=x%5Crightarrow+a%5Clog%28x%29+%2B+b%5Clog%281-x%29) 的最优解在 ![\frac {a}{a+b}](https://www.zhihu.com/equation?tex=%5Cfrac+%7Ba%7D%7Ba%2Bb%7D)处取得，即 ![D^*_{G}(x)= \frac {p_{data}(x)}{p_{data}(x)+p_g(x)}](https://www.zhihu.com/equation?tex=D%5E%2A_%7BG%7D%28x%29%3D+%5Cfrac+%7Bp_%7Bdata%7D%28x%29%7D%7Bp_%7Bdata%7D%28x%29%2Bp_g%28x%29%7D) 。
![\begin{align} C(G) &= \max \limits_{D}V(G,D) \\ &= \mathbb{E}_{x\sim p_{data}} \log[D_G^*(x)] + \mathbb{E}_{z\sim p_z}\log[1-D_G^*(G(z))] \\ &= \mathbb{E}_{x\sim p_{data}} \log[D_G^*(x)] + \mathbb{E}_{x\sim p_g}\log[1-D_G^*(x)] \\ &= \mathbb{E}_{x\sim p_{data}} \log \frac {p_{data}(x)}{p_{data}(x)+p_g(x)} + \mathbb{E}_{x\sim p_g} \log \frac {p_g(x)}{p_{data}(x)+p_g(x)} \end{align} \\](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+C%28G%29+%26%3D+%5Cmax+%5Climits_%7BD%7DV%28G%2CD%29+%5C%5C+%26%3D+%5Cmathbb%7BE%7D_%7Bx%5Csim+p_%7Bdata%7D%7D+%5Clog%5BD_G%5E%2A%28x%29%5D+%2B+%5Cmathbb%7BE%7D_%7Bz%5Csim+p_z%7D%5Clog%5B1-D_G%5E%2A%28G%28z%29%29%5D+%5C%5C+%26%3D+%5Cmathbb%7BE%7D_%7Bx%5Csim+p_%7Bdata%7D%7D+%5Clog%5BD_G%5E%2A%28x%29%5D+%2B+%5Cmathbb%7BE%7D_%7Bx%5Csim+p_g%7D%5Clog%5B1-D_G%5E%2A%28x%29%5D+%5C%5C+%26%3D+%5Cmathbb%7BE%7D_%7Bx%5Csim+p_%7Bdata%7D%7D+%5Clog+%5Cfrac+%7Bp_%7Bdata%7D%28x%29%7D%7Bp_%7Bdata%7D%28x%29%2Bp_g%28x%29%7D+%2B+%5Cmathbb%7BE%7D_%7Bx%5Csim+p_g%7D+%5Clog+%5Cfrac+%7Bp_g%28x%29%7D%7Bp_%7Bdata%7D%28x%29%2Bp_g%28x%29%7D+%5Cend%7Balign%7D+%5C%5C)

那么最终训练目标是最小化 ![C(G)](https://www.zhihu.com/equation?tex=C%28G%29) ，即 ![\min \limits_{G}C(G)](https://www.zhihu.com/equation?tex=%5Cmin+%5Climits_%7BG%7DC%28G%29) ，下证当且仅当 ![p_g=p_{data}](https://www.zhihu.com/equation?tex=p_g%3Dp_%7Bdata%7D) ，此时最小值 ![C(G)=-\log4](https://www.zhihu.com/equation?tex=C%28G%29%3D-%5Clog4) 。
**KL-散度**：衡量两个概率分布之间的差异情况。 ![D(P||Q)=\sum_{x \in X}{\log \frac {P(x)}{Q(x)}}](https://www.zhihu.com/equation?tex=D%28P%7C%7CQ%29%3D%5Csum_%7Bx+%5Cin+X%7D%7B%5Clog+%5Cfrac+%7BP%28x%29%7D%7BQ%28x%29%7D%7D)

那么
![\begin{align} \mathbb{E}_{x\sim p_{data}} \log \frac {p_{data}(x)}{p_{data}(x)+p_g(x)} &= \int_{x} p_{data}(x) \log \frac {p_{data}(x)}{p_{data}(x)+p_g(x)} \\ &= \int_{x} p_{data}(x) \log [\frac {p_{data}(x)}{\frac {p_{data}(x)+p_g(x)}{2}} * \frac {1}{2}] \\ &= KL(p_{data}||\frac {p_{data}+p_g}{2}) + \int_{x}p_{data}(x) \log \frac {1}{2} \\ &= KL(p_{data}||\frac {p_{data}+p_g}{2}) + \log \frac {1}{2} \end{align} \\](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%5Cmathbb%7BE%7D_%7Bx%5Csim+p_%7Bdata%7D%7D+%5Clog+%5Cfrac+%7Bp_%7Bdata%7D%28x%29%7D%7Bp_%7Bdata%7D%28x%29%2Bp_g%28x%29%7D+%26%3D+%5Cint_%7Bx%7D+p_%7Bdata%7D%28x%29+%5Clog+%5Cfrac+%7Bp_%7Bdata%7D%28x%29%7D%7Bp_%7Bdata%7D%28x%29%2Bp_g%28x%29%7D+%5C%5C+%26%3D+%5Cint_%7Bx%7D+p_%7Bdata%7D%28x%29+%5Clog+%5B%5Cfrac+%7Bp_%7Bdata%7D%28x%29%7D%7B%5Cfrac+%7Bp_%7Bdata%7D%28x%29%2Bp_g%28x%29%7D%7B2%7D%7D+%2A+%5Cfrac+%7B1%7D%7B2%7D%5D+%5C%5C+%26%3D+KL%28p_%7Bdata%7D%7C%7C%5Cfrac+%7Bp_%7Bdata%7D%2Bp_g%7D%7B2%7D%29+%2B+%5Cint_%7Bx%7Dp_%7Bdata%7D%28x%29+%5Clog+%5Cfrac+%7B1%7D%7B2%7D+%5C%5C+%26%3D+KL%28p_%7Bdata%7D%7C%7C%5Cfrac+%7Bp_%7Bdata%7D%2Bp_g%7D%7B2%7D%29+%2B+%5Clog+%5Cfrac+%7B1%7D%7B2%7D+%5Cend%7Balign%7D+%5C%5C)

同理， ![\begin{align} \mathbb{E}_{x\sim p_g} \log \frac {p_g(x)}{p_{data}(x)+p_g(x)} = KL(p_g||\frac {p_{data}+p_g}{2}) + \log \frac {1}{2} \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%5Cmathbb%7BE%7D_%7Bx%5Csim+p_g%7D+%5Clog+%5Cfrac+%7Bp_g%28x%29%7D%7Bp_%7Bdata%7D%28x%29%2Bp_g%28x%29%7D+%3D+KL%28p_g%7C%7C%5Cfrac+%7Bp_%7Bdata%7D%2Bp_g%7D%7B2%7D%29+%2B+%5Clog+%5Cfrac+%7B1%7D%7B2%7D+%5Cend%7Balign%7D)

**JS-散度**：衡量两个概率分布之间的相似度。 ![JS(P||Q)=\frac {1}{2}KL(P||\frac {P+Q}{2})+\frac {1}{2}KL(Q||\frac {P+Q}{2})](https://www.zhihu.com/equation?tex=JS%28P%7C%7CQ%29%3D%5Cfrac+%7B1%7D%7B2%7DKL%28P%7C%7C%5Cfrac+%7BP%2BQ%7D%7B2%7D%29%2B%5Cfrac+%7B1%7D%7B2%7DKL%28Q%7C%7C%5Cfrac+%7BP%2BQ%7D%7B2%7D%29+)


因此
![\begin{align} C(G) &=-\log4 + KL(p_{data}||\frac {p_{data}+p_g}{2})+KL(p_g||\frac {p_{data}+p_g}{2}) \\ &= -\log4+2 \cdot JS(p_{data}||p_g) \end{align} \\](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+C%28G%29+%26%3D-%5Clog4+%2B+KL%28p_%7Bdata%7D%7C%7C%5Cfrac+%7Bp_%7Bdata%7D%2Bp_g%7D%7B2%7D%29%2BKL%28p_g%7C%7C%5Cfrac+%7Bp_%7Bdata%7D%2Bp_g%7D%7B2%7D%29+%5C%5C+%26%3D+-%5Clog4%2B2+%5Ccdot+JS%28p_%7Bdata%7D%7C%7Cp_g%29+%5Cend%7Balign%7D+%5C%5C)

JS-散度总是大于等于0，当且仅当 ![P=Q](https://www.zhihu.com/equation?tex=P%3DQ) 时， ![JS(P||Q)=0](https://www.zhihu.com/equation?tex=JS%28P%7C%7CQ%29%3D0) 。所以
![C(G)-(-\log4)=2 \cdot JS(p_{data}||p_g) \geq 0](https://www.zhihu.com/equation?tex=C%28G%29-%28-%5Clog4%29%3D2+%5Ccdot+JS%28p_%7Bdata%7D%7C%7Cp_g%29+%5Cgeq+0) ， ![C(G)](https://www.zhihu.com/equation?tex=C%28G%29) 最小值是 ![-\log4](https://www.zhihu.com/equation?tex=-%5Clog4) ，当且仅当 ![p_{data}=p_g](https://www.zhihu.com/equation?tex=p_%7Bdata%7D%3Dp_g) 。



**论文二、**《GANs for Sequences of Discrete Elements with the Gumbel-softmax Distribution》 2016

![](https://pic4.zhimg.com/80/v2-8b28ce8c8fda3d78f331dad52b9323be_hd.jpg)

**1、写作动机**

虽然 GAN 在计算机视觉领域取得巨大的成功，但对于离散型数据的序列却捉襟见肘。原因在于，从离散对象的分布中采样这一过程是不可导的，因此参数难以更新。作者认为，这一问题可以通过 Gumbel-softmax 分布来避免，Gumbel-softmax 分布是对基于 softmax 函数的多项式分布的一个连续逼近。

> Gumbel分布是一种极值型分布。举例说明，假设每次测量心率值为一个随机变量（服从某种指数型分布，如正态分布），每天测量10次并取最大的心率作为当天的心率测量值。显然，每天记录的心率值也是一个随机变量，并且它的概率分布即为 Gumbel 分布。

**2、Gumbel-softmax 分布**

对于一个 d 维向量 ![h](https://www.zhihu.com/equation?tex=h) 和一个 d 维 one-hot 向量 ![y](https://www.zhihu.com/equation?tex=y) ，softmax 函数可以参数化表示![y](https://www.zhihu.com/equation?tex=y) 上的多项分布。 ![p](https://www.zhihu.com/equation?tex=p) 是一个 d 维向量，表示 ![y](https://www.zhihu.com/equation?tex=y) 上的多项分布，其中 ![p_i=p(y_i=1),i=1,...,d](https://www.zhihu.com/equation?tex=p_i%3Dp%28y_i%3D1%29%2Ci%3D1%2C...%2Cd) ，所以

![p=\text{softmax}(h) \tag{1} \\](https://www.zhihu.com/equation?tex=p%3D%5Ctext%7Bsoftmax%7D%28h%29+%5Ctag%7B1%7D+%5C%5C)

![[\text{softmax}(h)]_i=\frac {\exp(h_i)}{\sum_{j=1}^{d}{\exp(h_j)}},i=1,...,d \tag{2} \\](https://www.zhihu.com/equation?tex=%5B%5Ctext%7Bsoftmax%7D%28h%29%5D_i%3D%5Cfrac+%7B%5Cexp%28h_i%29%7D%7B%5Csum_%7Bj%3D1%7D%5E%7Bd%7D%7B%5Cexp%28h_j%29%7D%7D%2Ci%3D1%2C...%2Cd+%5Ctag%7B2%7D+%5C%5C)

式（1）按照向量 ![p](https://www.zhihu.com/equation?tex=p) 中的概率对 ![y](https://www.zhihu.com/equation?tex=y) 进行多项分布的采样等价于：

![y=\text{one_hot}(\mathop{\arg\max} \limits_{i}(h_i+g_i)) \tag{3} \\](https://www.zhihu.com/equation?tex=y%3D%5Ctext%7Bone_hot%7D%28%5Cmathop%7B%5Carg%5Cmax%7D+%5Climits_%7Bi%7D%28h_i%2Bg_i%29%29+%5Ctag%7B3%7D+%5C%5C)

这里的 ![g_i](https://www.zhihu.com/equation?tex=g_i) 相互独立且符合 Gumbel 分布，可以证明， ![\mathop{\arg\max}(h_i+g_i)](https://www.zhihu.com/equation?tex=%5Cmathop%7B%5Carg%5Cmax%7D%28h_i%2Bg_i%29) 的分布等于由 softmax 函数计算的概率分布。

但是，式（3）对向量 ![h](https://www.zhihu.com/equation?tex=h) 的梯度等于0，因为 ![\text{one_hot}(\mathop{\arg\max}(\cdot))](https://www.zhihu.com/equation?tex=%5Ctext%7Bone_hot%7D%28%5Cmathop%7B%5Carg%5Cmax%7D%28%5Ccdot%29%29) 操作是不可导的。作者提出了一种基于 softmax 转换的可微函数来逼近式（3）:

![y=\text{softmax}(\frac {1}{\tau (h+g)}) \tag{4} \\](https://www.zhihu.com/equation?tex=y%3D%5Ctext%7Bsoftmax%7D%28%5Cfrac+%7B1%7D%7B%5Ctau+%28h%2Bg%29%7D%29+%5Ctag%7B4%7D+%5C%5C)

这里的 ![\tau](https://www.zhihu.com/equation?tex=%5Ctau) 是一个控制 softmax 函数 soft 程度的参数，当 ![\tau\rightarrow0](https://www.zhihu.com/equation?tex=%5Ctau%5Crightarrow0) ，式（4）和式（3）生成的样本接近一样，当 ![\tau\rightarrow \infty](https://www.zhihu.com/equation?tex=%5Ctau%5Crightarrow+%5Cinfty) ，式（4）生成的样本满足均匀分布，当 ![\tau](https://www.zhihu.com/equation?tex=%5Ctau) 大于0且有限时，式（4）生成的样本比较平滑而且对 ![h](https://www.zhihu.com/equation?tex=h) 可导。因此，这里的式（4）所代表的的概率分布就称为 Gumbel-softmax 分布，GAN 在离散型数据上的训练可通过式（4）进行， ![\tau](https://www.zhihu.com/equation?tex=%5Ctau) 最开始的值较大，慢慢接近于0 。

3、模型算法

![](https://pic2.zhimg.com/80/v2-f526dbf878b9d7205fe00f264d8bcf47_hd.jpg)

![](https://pic2.zhimg.com/80/v2-4a9917560672a423286fbe61bb23d7ef_hd.jpg)

原始的生成模型是对隐藏状态 ![h_i](https://www.zhihu.com/equation?tex=h_i) 做 softmax 操作取最大值对应的单词，但因为从 softmax 分布采样输出这一过程是不可导的，所以训练中对隐藏状态 ![h_i](https://www.zhihu.com/equation?tex=h_i) 使用 Gumbel-softmax 分布进行采样输出，最终，生成模型和判别模型的参数都可以通过基于梯度的算法进行更新。



**论文三、**《Generating Text via Adversarial Training》NIPS 2016 Workshop

![](https://pic1.zhimg.com/80/v2-1d21497dbff5177ca04a42deaf519ba4_hd.jpg)

**1、写作动机**

本篇论文同样是为了解决 GAN 模型中离散输出的问题。作者以 LSTM 作为 GAN 的生成器，以 CNN 作为 GAN 的判别器，并使用光滑近似（smooth approximation）的思想逼近生成器 LSTM 的输出，从而解决离散导致的梯度不可导问题。与原始 GAN 模型的目标函数不同，作者的优化函数采用的是 feature matching 的方法，并使用了多种训练手段来改善 GAN 模型的收敛性。

**2、TextGAN 模型定义**

语料集： ![S=\{s_1, \cdots ,s_n \}](https://www.zhihu.com/equation?tex=S%3D%5C%7Bs_1%2C+%5Ccdots+%2Cs_n+%5C%7D) ， ![s_i](https://www.zhihu.com/equation?tex=s_i) 表示第 ![i](https://www.zhihu.com/equation?tex=i)个句子，![n](https://www.zhihu.com/equation?tex=n) 表示句子个数。

句子： ![s=\{w^t\}_{t=1}^m](https://www.zhihu.com/equation?tex=s%3D%5C%7Bw%5Et%5C%7D_%7Bt%3D1%7D%5Em) ， ![w^t](https://www.zhihu.com/equation?tex=w%5Et) 表示句子的第 ![t](https://www.zhihu.com/equation?tex=t) 个单词。每一个单词 ![w^t](https://www.zhihu.com/equation?tex=w%5Et) 都映射为一个 k 维词向量 ![x_t=W_e[w^t]](https://www.zhihu.com/equation?tex=x_t%3DW_e%5Bw%5Et%5D) ，其中 ![W_e \in \mathbb{R}^{k\times V}](https://www.zhihu.com/equation?tex=W_e+%5Cin+%5Cmathbb%7BR%7D%5E%7Bk%5Ctimes+V%7D) 是在训练过程中逐渐学习的词嵌入矩阵， ![V](https://www.zhihu.com/equation?tex=V) 是词典的大小，符号 ![[v]](https://www.zhihu.com/equation?tex=%5Bv%5D) 表示单词 ![v](https://www.zhihu.com/equation?tex=v) 在词典中对应的索引值。

**CNN discriminator:**

一个长度为 ![T](https://www.zhihu.com/equation?tex=T) （长度不足则补齐）的句子可以表示成一个矩阵 ![X \in \mathbb{R}^{k\times T}](https://www.zhihu.com/equation?tex=X+%5Cin+%5Cmathbb%7BR%7D%5E%7Bk%5Ctimes+T%7D) ，它的每一列由句子中单词的词向量组成，例如第 ![t](https://www.zhihu.com/equation?tex=t) 列是单词 ![x_t](https://www.zhihu.com/equation?tex=x_t) 的词向量。

**抽取句子的一个特征需要对句子进行一次卷积操作和池化操作。**

卷积核 filter： ![W_c\in \mathbb{R}^{k\times h}](https://www.zhihu.com/equation?tex=W_c%5Cin+%5Cmathbb%7BR%7D%5E%7Bk%5Ctimes+h%7D) ，对句子中 h 大小窗口内的连续单词做卷积： ![c=f(X * W_c +b)](https://www.zhihu.com/equation?tex=c%3Df%28X+%2A+W_c+%2Bb%29)，其中 ![f(\cdot)](https://www.zhihu.com/equation?tex=f%28%5Ccdot%29) 是一个非线性激活函数， ![*](https://www.zhihu.com/equation?tex=%2A) 是卷积操作，最终得到一个 feature map ![c](https://www.zhihu.com/equation?tex=c) ，然后对 feature map 做最大池化操作， ![\hat{c}=\max\{c\}](https://www.zhihu.com/equation?tex=%5Chat%7Bc%7D%3D%5Cmax%5C%7Bc%5C%7D) 作为这次操作的最终特征。

最大池化的目的是为了获取最重要的特征（对应重要的单词位置），过滤掉信息量少的单词。

在实验中，作者使用了多个卷积核 filter 以及多种窗口大小 h 。每一个卷积核都可以看做是一个语言特征的提取器，它能够识别提取 h-gram 的特征。假设我们有 m 种窗口大小， d 个卷积核，最终我们可以得到一个 md 维的特征。在这个 m*d 维的特征之上，应用一个 softmax 层映射为输出 ![D(X)\in [0,1]](https://www.zhihu.com/equation?tex=D%28X%29%5Cin+%5B0%2C1%5D) ，表示句子 ![X](https://www.zhihu.com/equation?tex=X) 来自真实数据的概率。

**LSTM generator:**

生成器将一个输入噪声 ![z](https://www.zhihu.com/equation?tex=z) 映射为一个句子 ![\tilde{s}](https://www.zhihu.com/equation?tex=%5Ctilde%7Bs%7D) 。生成概率为：

![p(\tilde{s}|z)=p(w^1|z)\prod_{t=2}^{T}p(w^t|w^{<t},z) \\](https://www.zhihu.com/equation?tex=p%28%5Ctilde%7Bs%7D%7Cz%29%3Dp%28w%5E1%7Cz%29%5Cprod_%7Bt%3D2%7D%5E%7BT%7Dp%28w%5Et%7Cw%5E%7B%3Ct%7D%2Cz%29+%5C%5C)

![p(w^1|z)=\arg\max(Vh_1) \\](https://www.zhihu.com/equation?tex=p%28w%5E1%7Cz%29%3D%5Carg%5Cmax%28Vh_1%29+%5C%5C)

![h_1=\tanh(Cz) \\](https://www.zhihu.com/equation?tex=h_1%3D%5Ctanh%28Cz%29+%5C%5C)

之后的所有单词则按顺序使用 LSTM 计算得出，直至遇到结束标志符。

![p(w^t|w^{<t},z)=\arg\max(Vh_t) \\](https://www.zhihu.com/equation?tex=p%28w%5Et%7Cw%5E%7B%3Ct%7D%2Cz%29%3D%5Carg%5Cmax%28Vh_t%29+%5C%5C)

![h_t=\text{LSTM}(y_{t-1},h_{t-1},z) \\](https://www.zhihu.com/equation?tex=h_t%3D%5Ctext%7BLSTM%7D%28y_%7Bt-1%7D%2Ch_%7Bt-1%7D%2Cz%29+%5C%5C)

![y_{t-1}=W_e[w^{t-1}] \tag{*}\\](https://www.zhihu.com/equation?tex=y_%7Bt-1%7D%3DW_e%5Bw%5E%7Bt-1%7D%5D+%5Ctag%7B%2A%7D%5C%5C)

需要注意的是，噪声 ![z](https://www.zhihu.com/equation?tex=z) 作为额外的 LSTM 输入指导句子的生成。模型框架如下：

![](https://pic1.zhimg.com/80/v2-aac5919eefc20db0a3ef1b5bb6db5057_hd.jpg)

**3、训练方法**

本篇论文的目标函数和原始 GAN 有所不同，文中采用了 feature matching 的方法 。迭代优化过程包含以下两个步骤：

![\begin{align} \text{minimizing:}&\ L_D=-\mathbb{E}_{s\sim S}\log D(s)-\mathbb{E}_{z\sim p_z(z)}\log [1-D(G(z))] \\ \text{minimizing:}&\ L_G=\text{tr}(\Sigma^{-1}_s\Sigma_r+\Sigma^{-1}_r\Sigma_s)+(\mu_s-\mu_r)^{\mathrm{T}}(\Sigma^{-1}_s+\Sigma^{-1}_r)(\mu_s-\mu_r) \\ \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%5Ctext%7Bminimizing%3A%7D%26%5C+L_D%3D-%5Cmathbb%7BE%7D_%7Bs%5Csim+S%7D%5Clog+D%28s%29-%5Cmathbb%7BE%7D_%7Bz%5Csim+p_z%28z%29%7D%5Clog+%5B1-D%28G%28z%29%29%5D+%5C%5C+%5Ctext%7Bminimizing%3A%7D%26%5C+L_G%3D%5Ctext%7Btr%7D%28%5CSigma%5E%7B-1%7D_s%5CSigma_r%2B%5CSigma%5E%7B-1%7D_r%5CSigma_s%29%2B%28%5Cmu_s-%5Cmu_r%29%5E%7B%5Cmathrm%7BT%7D%7D%28%5CSigma%5E%7B-1%7D_s%2B%5CSigma%5E%7B-1%7D_r%29%28%5Cmu_s-%5Cmu_r%29+%5C%5C+%5Cend%7Balign%7D)

其中 ![\Sigma_s](https://www.zhihu.com/equation?tex=%5CSigma_s) 和 ![\Sigma_r](https://www.zhihu.com/equation?tex=%5CSigma_r) 分别表示生成句子特征向量 ![f_s](https://www.zhihu.com/equation?tex=f_s) 和真实句子特征向量 ![f_r](https://www.zhihu.com/equation?tex=f_r) 的协方差矩阵， ![\mu_s](https://www.zhihu.com/equation?tex=%5Cmu_s) 和 ![\mu_r](https://www.zhihu.com/equation?tex=%5Cmu_r) 则分别表示 ![f_s](https://www.zhihu.com/equation?tex=f_s) 和 ![f_r](https://www.zhihu.com/equation?tex=f_r) 的平均值。 ![\Sigma_s](https://www.zhihu.com/equation?tex=%5CSigma_s) , ![\Sigma_r](https://www.zhihu.com/equation?tex=%5CSigma_r) , ![\mu_s](https://www.zhihu.com/equation?tex=%5Cmu_s) 和 ![\mu_r](https://www.zhihu.com/equation?tex=%5Cmu_r) 都是通过经验设置。通过设置 ![\Sigma_s=\Sigma_r=\text{I}](https://www.zhihu.com/equation?tex=%5CSigma_s%3D%5CSigma_r%3D%5Ctext%7BI%7D) ，生成器 ![G](https://www.zhihu.com/equation?tex=G) 的优化则变成了 feature matching 方法。第二个损失 ![L_G](https://www.zhihu.com/equation?tex=L_G) 是两个多元正态分布 ![N(\mu_r,\Sigma_r)](https://www.zhihu.com/equation?tex=N%28%5Cmu_r%2C%5CSigma_r%29) 和 ![N(\mu_s,\Sigma_s)](https://www.zhihu.com/equation?tex=N%28%5Cmu_s%2C%5CSigma_s%29) 的** JS-散度**。论文中详细说明了这种方法好。

因为生成器 ![G](https://www.zhihu.com/equation?tex=G) 包含离散变量，直接应用梯度估计的方法无法使用。基于 score function 的强化学习算法，例如 Monte Carlo 估计，可以得到离散变量梯度的无偏估计值。但是，这种方法的梯度估计值的方差会很大，因此，这里作者采用的是用 soft-argmax 函数来逼近 (*) 式：

![y_{t-1}=W_e\text{softmax}(Vh_{t-1}\odot L) \\](https://www.zhihu.com/equation?tex=y_%7Bt-1%7D%3DW_e%5Ctext%7Bsoftmax%7D%28Vh_%7Bt-1%7D%5Codot+L%29+%5C%5C)

这里的 ![L](https://www.zhihu.com/equation?tex=L) 生成器的损失，当 ![L\rightarrow \infty](https://www.zhihu.com/equation?tex=L%5Crightarrow+%5Cinfty) ，逼近越接近 (*) 式。

梯度下降的优化策略往往会因为在鞍点附近徘徊而难以收敛至纳什均衡。一个好的初始化往往能加快收敛速度。本篇论文的初始化非常有意思，对于生成器，作者预训练了一个标准自编码 LSTM 模型来初始化生成器的 LSTM 参数；对于判别器，作者使用了一种 confusion 训练策略，利用原始的句子和该句子中随机交换两个词的位置后得到的新句子进行判别训练。(在初始化的过程中，运用逐点分类损失函数对判别器进行优化)。之所以进行交换操作而不进行增加/删除操作，是因为作者希望 CNN 学习到句子结构上的特征，而不是某些单词的特征，将两个单词互换位置，输入的数据信息实际上是基本相同的，大多数卷积计算最终会得出完全相同的值。这种方法能够避免生成器通过产生重复的“realistic”单词来试图获取更高分数以欺骗判别器。

在实验中，作者交替地对生成器和判别器进行训练，每训练5次生成器就训练1次判别器，因为 LSTM 的参数比 CNN 更多，更难训练，这与原始的 GAN 训练过程相反。



**论文四、**《Adversarial Feature Matching for Text Generation》ICML 2017

![](https://pic3.zhimg.com/80/v2-e53f958e5320babdadd718e31923d38e_hd.jpg)

**1、写作动机**

这篇论文是论文三的续作，着重叙述对抗训练的过程。训练时的目标函数不是原始 GAN 的目标函数，而是通过 kernelized discrepancy metric 对真实句子和生成句子的隐藏特征表示进行 match 操作。这种方法可以缓解对抗训练中的模式崩溃（mode collapse）问题。

![](https://pic2.zhimg.com/80/v2-33d490f52b4f3abf223bc35388eaa52f_hd.jpg)

> **什么是模式崩溃（mode collapse）？**

真实世界中的数据分布可能会有多个样本集中的“波峰”。例如，假设有一个数据集，其中包含澳大利亚中部爱丽丝泉（通常非常热）和南极南极（通常非常寒冷）的夏日温度读数的混合数据。数据的分布是双峰的 - 两个地区的平均温度存在峰值，两者之间存在差距。如上图。

现在我们想训练一个产生合理温度值的 GAN。直观地说，我们希望生成器学会以几乎相等的概率产生热和冷的温度。然而，通常遇到的问题是模式崩溃，导致生成器仅从单一模式输出样本（例如，仅低温）。为什么会这样？可以考虑以下情形：

1、生成器学习到它可以通过产生接近南极温度的值来欺骗判别器认为它正在输出实际温度。

2、判别器通过了解所有澳大利亚温度都是真实的（不是由生成器产生的）而对生成器进行反驳，并且猜测南极温度是真实的还是虚假的，因为它们无法区分。

3、该生成器切换模式使得产生的值接近澳大利亚的温度来欺骗判别器，放弃南极模式。

4、判别器现在假设澳大利亚所有的温度都是假的，而南极的温度是真实的。

5、返回到第1步。

这种猫捉老鼠的游戏总是重复让人厌烦，生成器也从未被直接激励来覆盖这两种模式。在这种情况下，生成器在生成的样本中将表现出非常差的多样性，这限制了 GAN 的使用。

论文同样使用 LSTM 作为生成器，CNN 作为判别器。作者还考虑了再生核希尔伯特空间（Reproducing Kernel Hilbert Space, RKHS）下基于核的矩匹配模式（kernel-based moment-matching scheme），它能够使得真实句子和生成句子的经验分布在隐藏向量空间上有匹配的矩。这种方法能够很好地缓解原始 GAN 训练中的模式崩溃问题。

**2、模型解读**

原始的 GAN 模型优化目标是：

![L_{GAN}=\mathbb{E}_{x\sim p_x}\log D(x) + \mathbb{E}_{z\sim p_z}\log [1-D(G(z))] \tag{1}\\](https://www.zhihu.com/equation?tex=L_%7BGAN%7D%3D%5Cmathbb%7BE%7D_%7Bx%5Csim+p_x%7D%5Clog+D%28x%29+%2B+%5Cmathbb%7BE%7D_%7Bz%5Csim+p_z%7D%5Clog+%5B1-D%28G%28z%29%29%5D+%5Ctag%7B1%7D%5C%5C)

在论文一中我们知道，当判别器 ![D](https://www.zhihu.com/equation?tex=D) 达到最优解时，这个优化目标就等于最小化 ![p_x](https://www.zhihu.com/equation?tex=p_x) 和 ![p_z](https://www.zhihu.com/equation?tex=p_z) 的JS-散度问题。但是，在大多数情况下，式（1）在鞍点附近没有很好地解决办法，所以交替更新生成器 ![G](https://www.zhihu.com/equation?tex=G) 和判别器 ![D](https://www.zhihu.com/equation?tex=D) 是常用的选择。

给定一个语料集 ![S](https://www.zhihu.com/equation?tex=S) ，与其直接优化式（1），作者采取了一种类似 feature matching 的方法。作者的目标函数是：

![\begin{align} L_{D}&=L_{GAN}-\lambda_rL_{recon}+\lambda_mL_{MMD^2} \tag{2} \\ L_G&=L_{MMD^2} \tag{3} \\ L_{GAN}&=\mathbb{E}_{s\sim S}\log D(s)+\mathbb{E}_{z\sim p_z}\log [1-\log[1-D(G(z))]] \tag{4} \\ L_{recon}&=||\hat{z}-z||^2 \tag{5} \\ \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+L_%7BD%7D%26%3DL_%7BGAN%7D-%5Clambda_rL_%7Brecon%7D%2B%5Clambda_mL_%7BMMD%5E2%7D+%5Ctag%7B2%7D+%5C%5C+L_G%26%3DL_%7BMMD%5E2%7D+%5Ctag%7B3%7D+%5C%5C+L_%7BGAN%7D%26%3D%5Cmathbb%7BE%7D_%7Bs%5Csim+S%7D%5Clog+D%28s%29%2B%5Cmathbb%7BE%7D_%7Bz%5Csim+p_z%7D%5Clog+%5B1-%5Clog%5B1-D%28G%28z%29%29%5D%5D+%5Ctag%7B4%7D+%5C%5C+L_%7Brecon%7D%26%3D%7C%7C%5Chat%7Bz%7D-z%7C%7C%5E2+%5Ctag%7B5%7D+%5C%5C+%5Cend%7Balign%7D+)

![L_D](https://www.zhihu.com/equation?tex=L_D) 和 ![L_G](https://www.zhihu.com/equation?tex=L_G) 分别是对 ![D(\cdot)](https://www.zhihu.com/equation?tex=D%28%5Ccdot%29) 和 ![G(\cdot)](https://www.zhihu.com/equation?tex=G%28%5Ccdot%29) 的最大化和最小化函数。 ![L_{GAN}](https://www.zhihu.com/equation?tex=L_%7BGAN%7D) 是原始 GAN 的目标函数（1）。 ![L_{recon}](https://www.zhihu.com/equation?tex=L_%7Brecon%7D) 表示初始隐藏向量 ![z](https://www.zhihu.com/equation?tex=z) 和重构隐藏向量 ![\hat{z}](https://www.zhihu.com/equation?tex=%5Chat%7Bz%7D) 之间的欧氏距离， ![z](https://www.zhihu.com/equation?tex=z) 来自先验分布 ![p_z](https://www.zhihu.com/equation?tex=p_z)。作者定义生成的句子为 ![\hat{s}](https://www.zhihu.com/equation?tex=%5Chat%7Bs%7D) 且 ![\hat{s}\triangleq G(z)](https://www.zhihu.com/equation?tex=%5Chat%7Bs%7D%5Ctriangleq+G%28z%29) ， ![z\sim p_z(\cdot)](https://www.zhihu.com/equation?tex=z%5Csim+p_z%28%5Ccdot%29) 。 ![L_{MMD^2}](https://www.zhihu.com/equation?tex=L_%7BMMD%5E2%7D) 表示真实句子和生成句子的特征矩阵 ![f](https://www.zhihu.com/equation?tex=f) 和 ![\hat{f}](https://www.zhihu.com/equation?tex=%5Chat%7Bf%7D) 经验分布的 MMD 值。具体如下图所示：

> MMD：maximum mean discrepancy。最大平均差异。最先提出的时候用于双样本的检测（two-sample test）问题，用于判断两个分布p和q是否相同。它的基本假设是：如果对于所有以分布生成的样本空间为输入的函数f，如果两个分布生成的足够多的样本在f上的对应的像的均值都相等，那么那么可以认为这两个分布是同一个分布。现在一般用于度量两个分布之间的相似性。

![](https://pic2.zhimg.com/80/v2-e9c1728309a4fc721b804c0bb4397fcc_hd.jpg)

**优化函数解读：**

对于生成器 ![G](https://www.zhihu.com/equation?tex=G) 来说，考虑式（3），它的目标是使得生成的句子 ![\hat{s}](https://www.zhihu.com/equation?tex=%5Chat%7Bs%7D) 能够尽量模仿真实句子 ![s](https://www.zhihu.com/equation?tex=s) ，这就需要计算特征矩阵 ![\hat{f}](https://www.zhihu.com/equation?tex=%5Chat%7Bf%7D) 和 ![f](https://www.zhihu.com/equation?tex=f) 的经验分布之间的 MMD 值。

对于判别器 ![D](https://www.zhihu.com/equation?tex=D) 来说，考虑式（2），它的目标是希望生成的特征矩阵 ![f](https://www.zhihu.com/equation?tex=f) 和 ![\hat{f}](https://www.zhihu.com/equation?tex=%5Chat%7Bf%7D) 能够具有 discriminative，representative 以及 challenging 三种特性。这分别体现在式（2）的三部分中：i) ![L_{GAN}](https://www.zhihu.com/equation?tex=L_%7BGAN%7D) 保证了真实句子和生成句子的特征矩阵 ![f](https://www.zhihu.com/equation?tex=f) 和 ![\hat{f}](https://www.zhihu.com/equation?tex=%5Chat%7Bf%7D) 具有 discriminative 特性；ii) ![L_{recon}](https://www.zhihu.com/equation?tex=L_%7Brecon%7D) 保证了生成句子的特征矩阵 ![\hat{f}](https://www.zhihu.com/equation?tex=%5Chat%7Bf%7D) 最大程度保留原始信息，即 representative 特性，也就是重构表示 ![\hat{z}](https://www.zhihu.com/equation?tex=%5Chat%7Bz%7D) 和初始输入 ![z](https://www.zhihu.com/equation?tex=z) 的欧氏距离较小；iii) ![L_{MMD^2}](https://www.zhihu.com/equation?tex=L_%7BMMD%5E2%7D) 保证判别器 ![D](https://www.zhihu.com/equation?tex=D) 选择最 challenging 的特征与生成器 ![G](https://www.zhihu.com/equation?tex=G) 进行特征匹配 (feature matching) 。



**论文五、**《SeqGAN: Sequence Generative Adversarial Nets with Policy Gradient》

AAAI 2017

![](https://pic3.zhimg.com/80/v2-d35ac074a7849437881341837f864e86_hd.jpg)

**1、写作动机**

GAN 在生成连续离散序列时会遇到两个问题：一是因为生成器的输出是离散的，梯度更新从判别器传到生成器比较困难；二是判别器只有当序列被完全生成后才能进行判断，但此刻指导用处已不太大，而如果生成器生成序列的同时判别器来判断，如何平衡当前序列的分数和未来序列的分数又是一个难题。在这篇论文中，作者提出了一个序列生成模型——SeqGAN ，来解决上述这两个问题。作者将生成器看作是强化学习中的 stochastic policy，这样 SeqGAN 就可以直接通过 gradient policy update 避免生成器中的可导问题。同时，判别器对整个序列的评分作为强化学习的奖励信号可以通过 Monte Carlo 搜索传递到序列生成的中间时刻。

具体来说，作者将生成器生成序列的过程看做是一个强化学习中的序列决策过程。生成模型被看作一个 agent，目前为止已生成的序列表示当前 state，下一个要生成的单词则是采取的 action，判别模型对序列的评价分数则是返回的 reward。

**2、问题定义**

给定一个真实世界的序列数据集。

1）训练以 ![\theta](https://www.zhihu.com/equation?tex=%5Ctheta) 为参数的生成模型 ![G_{\theta}](https://www.zhihu.com/equation?tex=G_%7B%5Ctheta%7D) ，生成序列 ![Y_{1:T}=(y_1,\cdots ,y_t,\cdots ,y_T),y_t \in V](https://www.zhihu.com/equation?tex=Y_%7B1%3AT%7D%3D%28y_1%2C%5Ccdots+%2Cy_t%2C%5Ccdots+%2Cy_T%29%2Cy_t+%5Cin+V) ， ![V](https://www.zhihu.com/equation?tex=V)是词典。我们将这个问题建立在强化学习基础上。在时间步 t，状态 s 是当前已生成序列 ![(y_1,\cdots ,y_{t-1})](https://www.zhihu.com/equation?tex=%28y_1%2C%5Ccdots+%2Cy_%7Bt-1%7D%29) ，动作 a 是下一个选择的单词 ![y_t](https://www.zhihu.com/equation?tex=y_t) ，策略 policy是生成器 ![G_{\theta}(y_t|Y_{1:t-1})](https://www.zhihu.com/equation?tex=G_%7B%5Ctheta%7D%28y_t%7CY_%7B1%3At-1%7D%29) ，当下一步动作选择之后，状态转换是确定的，即 ![\delta^a_{s,s'}=1](https://www.zhihu.com/equation?tex=%5Cdelta%5Ea_%7Bs%2Cs%27%7D%3D1) 当下一个状态 ![s'=Y_{1:t}](https://www.zhihu.com/equation?tex=s%27%3DY_%7B1%3At%7D) ，当前状态 ![s=Y_{1:t-1}](https://www.zhihu.com/equation?tex=s%3DY_%7B1%3At-1%7D) ，动作 ![a=y_t](https://www.zhihu.com/equation?tex=a%3Dy_t) ；如果下一步是其他状态 ![s''](https://www.zhihu.com/equation?tex=s%27%27) ，则 ![\delta^a_{s,s^{''}}=0](https://www.zhihu.com/equation?tex=%5Cdelta%5Ea_%7Bs%2Cs%5E%7B%27%27%7D%7D%3D0) 。

2）训练以 ![\phi](https://www.zhihu.com/equation?tex=%5Cphi) 为参数的判别模型，指导生成器 ![G](https://www.zhihu.com/equation?tex=G) 的生成。 ![D_{\phi}(Y_{1:T})](https://www.zhihu.com/equation?tex=D_%7B%5Cphi%7D%28Y_%7B1%3AT%7D%29) 表示序列 ![Y_{1:T}](https://www.zhihu.com/equation?tex=Y_%7B1%3AT%7D) 来自真实数据中的概率。

![](https://pic1.zhimg.com/80/v2-8a7092db40f6f7beeb6022d48413163a_hd.jpg)

如上图，左边是判别器的训练，通过输入来自真实数据的正样例和来自生成器生成的负样例从而训练，判别器由 CNN 组成；右边是生成器的训练，通过将判别器判别的概率回传给生成器从而训练，这里使用了 Monte Carlo search 和 policy gradient 方法。

**3、Policy Gradient**

生成模型（policy） ![G_\theta(y_t|Y_{1:t-1})](https://www.zhihu.com/equation?tex=G_%5Ctheta%28y_t%7CY_%7B1%3At-1%7D%29) 的目标是最大化期望奖励：

![J(\theta)=\mathbb{E}[R_T|s_0,\theta]=\sum_{y_1 \in V}^{}{G_\theta(y_1|s_0)\cdot Q^{G_\theta}_{D_\phi}(s_0,y_1)} \tag{1} \\](https://www.zhihu.com/equation?tex=J%28%5Ctheta%29%3D%5Cmathbb%7BE%7D%5BR_T%7Cs_0%2C%5Ctheta%5D%3D%5Csum_%7By_1+%5Cin+V%7D%5E%7B%7D%7BG_%5Ctheta%28y_1%7Cs_0%29%5Ccdot+Q%5E%7BG_%5Ctheta%7D_%7BD_%5Cphi%7D%28s_0%2Cy_1%29%7D+%5Ctag%7B1%7D+%5C%5C)

其中 ![R_T](https://www.zhihu.com/equation?tex=R_T) 是完全生成序列的奖励，来自判别器 ![D_\phi](https://www.zhihu.com/equation?tex=D_%5Cphi) ，![Q^{G_\theta}_{D_\phi}(s,a)](https://www.zhihu.com/equation?tex=Q%5E%7BG_%5Ctheta%7D_%7BD_%5Cphi%7D%28s%2Ca%29) 是整个序列的 action-value function，表示从状态 ![s](https://www.zhihu.com/equation?tex=s) 开始，依据策略 ![G_\theta](https://www.zhihu.com/equation?tex=G_%5Ctheta) 采取动作 ![a](https://www.zhihu.com/equation?tex=a) 直到结束所得到的期望累计奖励。所以，最重要就是如何计算 action-value function。

在这篇论文中，作者将判别器 ![D_\phi(Y^n_{1:T})](https://www.zhihu.com/equation?tex=D_%5Cphi%28Y%5En_%7B1%3AT%7D%29) 判断序列 ![Y^n_{1:T}](https://www.zhihu.com/equation?tex=Y%5En_%7B1%3AT%7D) 为真的概率作为奖励，即：

![Q^{G_\theta}_{D_\phi}(a=y_T,s=Y_{1:T-1})=D_\phi(Y_{1:T}) \tag{2} \\](https://www.zhihu.com/equation?tex=Q%5E%7BG_%5Ctheta%7D_%7BD_%5Cphi%7D%28a%3Dy_T%2Cs%3DY_%7B1%3AT-1%7D%29%3DD_%5Cphi%28Y_%7B1%3AT%7D%29+%5Ctag%7B2%7D+%5C%5C)

但是，前面已经说明，判别器只能当序列被完全生成之后才能返回一个奖励。因为我们在每一个时间步上，真正关心的是长期的收益，所以我们不仅应该考虑已生成序列的效果，也要考虑接下来生成的结果，这就好比在下棋时，我们会适当放弃一些眼前优势从而追求长远的胜利。因此，为了估计中间时间步上的 action-value，我们使用 Monte Carlo 搜索 和 roll-out policy ![G_\beta](https://www.zhihu.com/equation?tex=G_%5Cbeta) 来对剩下的 ![T-t](https://www.zhihu.com/equation?tex=T-t) 个未知单词进行采样。我们将这个 N 次 Monte Carlo 搜索过程表示为：

![\{Y^1_{1:T},\cdots,Y^N_{1:T}\}=\text{MC}^{G_\beta}(Y_{1:t};N) \tag{3}\\](https://www.zhihu.com/equation?tex=%5C%7BY%5E1_%7B1%3AT%7D%2C%5Ccdots%2CY%5EN_%7B1%3AT%7D%5C%7D%3D%5Ctext%7BMC%7D%5E%7BG_%5Cbeta%7D%28Y_%7B1%3At%7D%3BN%29+%5Ctag%7B3%7D%5C%5C)

> roll-out 算法是对于当前状态，从每一个可能的动作开始，之后根据给定的策略进行路径采样，根据多次采样的奖励总和来对当前状态的行动值进行估计。当当前估计基本收敛时，会根据行动值最大的原则选择动作进入下一个状态再重复上述过程。在蒙特卡洛控制中，采样的目的是估计一个完整的，最优价值函数，但是roll-out中的采样目的只是为了计算当前状态的行动值以便进入下一个状态，而且这些估计的行动值并不会被保留。在roll-out中采用的策略往往比较简单被称作 **roll-out 策略 (roll-out policy)**。



> 蒙特卡洛树搜索是上面提到的 roll-out 算法的拓展版，在于它会记录搜索过程中的行动值变化以便更好的采样，完整的步骤有以下四步：

1、选择：从根节点出发，根据**树策略(tree policy)**选择一个叶节点

2、拓展：有一定概率发生，从选择的叶节点中执行一个未执行过的行动来增加一个子节点

3、模拟：从当前叶节点开始，根据 roll-out 策略执行动作直到终止时间

4、回溯：利用本次模拟中得到的奖励和逐层更新所使用到的树内节点

如下图所示。

![](https://pic2.zhimg.com/80/v2-bf88261beb23a8ad71d23a14d026197c_hd.jpg)

进行 N 次Monte Carlo 搜索之后，我们将得到一个 batch 为 N 的输出样例。因此，

![Q^{G_\theta}_{D_\phi}(s=Y_{1:t-1},a=y_t)=\left\{ \begin{array}{rcl} \frac {1}{N}\sum \nolimits_{n=1}^{N}{D_\phi (Y^n_{1:T}),Y^n_{1:T}\in \text{MC}^{G_\beta}(Y_{1:t};N)} & \text{for} & t<T\\ D_\phi(Y_{1:t}) & \text{for} & t=T \tag{4} \end{array} \right .](https://www.zhihu.com/equation?tex=Q%5E%7BG_%5Ctheta%7D_%7BD_%5Cphi%7D%28s%3DY_%7B1%3At-1%7D%2Ca%3Dy_t%29%3D%5Cleft%5C%7B+%5Cbegin%7Barray%7D%7Brcl%7D+%5Cfrac+%7B1%7D%7BN%7D%5Csum+%5Cnolimits_%7Bn%3D1%7D%5E%7BN%7D%7BD_%5Cphi+%28Y%5En_%7B1%3AT%7D%29%2CY%5En_%7B1%3AT%7D%5Cin+%5Ctext%7BMC%7D%5E%7BG_%5Cbeta%7D%28Y_%7B1%3At%7D%3BN%29%7D+%26+%5Ctext%7Bfor%7D+%26+t%3CT%5C%5C+D_%5Cphi%28Y_%7B1%3At%7D%29+%26+%5Ctext%7Bfor%7D+%26+t%3DT+%5Ctag%7B4%7D+%5Cend%7Barray%7D+%5Cright+.)

使用判别器 ![D_\phi](https://www.zhihu.com/equation?tex=D_%5Cphi) 作为奖励函数的好处在于奖励值可以动态地更新从而循环地改进生成模型的效果。论文中指出，每当我们生成许多与真实数据相像的序列，我们就要重新训练一次判别器：

![\min \limits_\phi -\mathbb{E}_{Y\sim p_{data}}(\log D_\phi (Y))-\mathbb{E}_{Y\sim G_\theta}[\log(1-D_\phi(Y))] \tag{5} \\](https://www.zhihu.com/equation?tex=%5Cmin+%5Climits_%5Cphi+-%5Cmathbb%7BE%7D_%7BY%5Csim+p_%7Bdata%7D%7D%28%5Clog+D_%5Cphi+%28Y%29%29-%5Cmathbb%7BE%7D_%7BY%5Csim+G_%5Ctheta%7D%5B%5Clog%281-D_%5Cphi%28Y%29%29%5D+%5Ctag%7B5%7D+%5C%5C)

每当重新训练得到新的判别器之后就可以更新生成器。文中采用的是 policy gradient 的方法，式（1）关于生成器中参数 ![\theta](https://www.zhihu.com/equation?tex=%5Ctheta) 的梯度为：

![\nabla J(\theta)=\sum \nolimits_{t=1}^{T}{\mathbb{E}_{Y_{1:t-1}\sim G_\theta}}[\sum_{y_t \in V}^{}{\nabla_\theta G_\theta (y_t|Y_{1:t-1})\cdot Q^{G_\theta}_{D_\phi}(Y_{1:t-1},y_t)}] \tag{6}\\](https://www.zhihu.com/equation?tex=%5Cnabla+J%28%5Ctheta%29%3D%5Csum+%5Cnolimits_%7Bt%3D1%7D%5E%7BT%7D%7B%5Cmathbb%7BE%7D_%7BY_%7B1%3At-1%7D%5Csim+G_%5Ctheta%7D%7D%5B%5Csum_%7By_t+%5Cin+V%7D%5E%7B%7D%7B%5Cnabla_%5Ctheta+G_%5Ctheta+%28y_t%7CY_%7B1%3At-1%7D%29%5Ccdot+Q%5E%7BG_%5Ctheta%7D_%7BD_%5Cphi%7D%28Y_%7B1%3At-1%7D%2Cy_t%29%7D%5D+%5Ctag%7B6%7D%5C%5C)

之后，更新参数 ![\theta](https://www.zhihu.com/equation?tex=%5Ctheta) ：

![\theta\leftarrow \theta+\alpha_h \nabla_\theta J(\theta) \\](https://www.zhihu.com/equation?tex=%5Ctheta%5Cleftarrow+%5Ctheta%2B%5Calpha_h+%5Cnabla_%5Ctheta+J%28%5Ctheta%29+%5C%5C)

具体推导过程可以参考论文附录。下图是整个算法：

![](https://pic4.zhimg.com/80/v2-152cf96689a87d8b96601d524e7b55c2_hd.jpg)



**论文六、**《MaskGAN: Better Text Generation via Filling in the ____》ICLR 2018

![](https://pic2.zhimg.com/80/v2-ff28a3da1e0606994cc045c7236b1224_hd.jpg)

**1、写作动机**

作者认为，以往的神经文本生成模型，例如 seq2seq，是机器翻译，摘要提取的基本模型，它们通常利用 maximun likelihood 和 teacher forcing 进行训练，生成文本的质量也都通过 validation perplexity 来定义。这些训练方法对于 perplexity 的优化来说可能很合适，但却不能保证生成文本的质量足够好，因为生成文本时，历史生成单词可能在训练时没有出现过，导致误差逐渐累积，也就是常说的 exposure bias 问题。虽然之前已经有 Professor Forcing 和 Scheduled Sampling 来解决这些问题，但它们都没有针对输出明确定义一个损失函数来提高结果质量，这篇论文对此做了改变。另外，离散输出也是导致梯度不能反向传播训练的原因，作者在这篇论文中利用强化学习中的 actor-critic 算法来训练生成器，利用最大似然和随机梯度下降来训练判别器。最后，GAN 模型中的模式崩溃和训练不稳定问题在文本生成任务下也更严重。训练不稳定会随着句子的长度增加而加重。为了避免这两个问题的影响，作者选择了基于上下文环境对缺失单词进行填充的场景。

**2、模型定义**

![(x_t,y_t)](https://www.zhihu.com/equation?tex=%28x_t%2Cy_t%29) 表示输入输出 token 对， ![<m>](https://www.zhihu.com/equation?tex=%3Cm%3E) 表示 token 遮住符号， ![\hat{x}_t](https://www.zhihu.com/equation?tex=%5Chat%7Bx%7D_t) 表示遮住的原始 token， ![\tilde{x}_t](https://www.zhihu.com/equation?tex=%5Ctilde%7Bx%7D_t) 表示生成的 token。

计算被遮住的 token 需要我们的 MaskGAN 模型基于历史和将来的信息。生成器由 seq2seq 模型组成。对一个输入序列 ![x=(x_1,\cdots ,x_T)](https://www.zhihu.com/equation?tex=x%3D%28x_1%2C%5Ccdots+%2Cx_T%29) ，随机或者确定地生成一个二进制向量 ![m=(m_1,\cdots ,m_T)](https://www.zhihu.com/equation?tex=m%3D%28m_1%2C%5Ccdots+%2Cm_T%29) ， ![m_t \in \{0,1\}](https://www.zhihu.com/equation?tex=m_t+%5Cin+%5C%7B0%2C1%5C%7D) 。如果 ![m_t=0](https://www.zhihu.com/equation?tex=m_t%3D0) ，则输入序列中的 ![x_t](https://www.zhihu.com/equation?tex=x_t) 被 ![<m>](https://www.zhihu.com/equation?tex=%3Cm%3E) 替换，否则不变。seq2seq 模型中的 encoder 将这个遮住部分 token 的序列编码为一个表示，记 ![m(x)](https://www.zhihu.com/equation?tex=m%28x%29) 。在 decoder 中的时间步 t，当我们在生成被遮住的 token 时，不仅依赖于 ![m(x)](https://www.zhihu.com/equation?tex=m%28x%29) ，也依赖到目前为止被补充的完整序列 ![(\hat{x}_1,\cdots ,\hat{x}_{t-1})](https://www.zhihu.com/equation?tex=%28%5Chat%7Bx%7D_1%2C%5Ccdots+%2C%5Chat%7Bx%7D_%7Bt-1%7D%29) ，所以

![P(\hat{x}_1,\cdots , \hat{x}_T|m(x))=\prod_{t=1}^{T}P(\hat{x}_t|\hat{x}_1,\cdots ,\hat{x}_{t-1},m(x)) \\](https://www.zhihu.com/equation?tex=P%28%5Chat%7Bx%7D_1%2C%5Ccdots+%2C+%5Chat%7Bx%7D_T%7Cm%28x%29%29%3D%5Cprod_%7Bt%3D1%7D%5E%7BT%7DP%28%5Chat%7Bx%7D_t%7C%5Chat%7Bx%7D_1%2C%5Ccdots+%2C%5Chat%7Bx%7D_%7Bt-1%7D%2Cm%28x%29%29+%5C%5C)

![G(x_t) \equiv P(\hat{x}_t|\hat{x}_1,\cdots ,\hat{x}_{t-1},m(x)) \\](https://www.zhihu.com/equation?tex=G%28x_t%29+%5Cequiv+P%28%5Chat%7Bx%7D_t%7C%5Chat%7Bx%7D_1%2C%5Ccdots+%2C%5Chat%7Bx%7D_%7Bt-1%7D%2Cm%28x%29%29+%5C%5C)

![](https://pic3.zhimg.com/80/v2-2c0a193d38e422e14f3e98dad1006cad_hd.jpg)

判别器的模型同样也是 seq2seq 模型，它的encoder 输入是已经被生成器补充后的序列，同时还要给定被部分遮住的序列 ![m(x)](https://www.zhihu.com/equation?tex=m%28x%29) ，decoder 在每个时间步的输出不再是一个词典上的 token 概率分布，而是一个0到1之间的概率（标量）。所以

![D_\phi (\tilde{x}_t|\tilde{x}_{0:T},m(x))=P(\tilde{x}_t=x^{real}_t|\tilde{x}_{0:T},m(x)) \\](https://www.zhihu.com/equation?tex=D_%5Cphi+%28%5Ctilde%7Bx%7D_t%7C%5Ctilde%7Bx%7D_%7B0%3AT%7D%2Cm%28x%29%29%3DP%28%5Ctilde%7Bx%7D_t%3Dx%5E%7Breal%7D_t%7C%5Ctilde%7Bx%7D_%7B0%3AT%7D%2Cm%28x%29%29+%5C%5C)

最后的奖励函数为：

![r_t \equiv\log D_\phi (\tilde{x}_t|\tilde{x}_{0:T},m(x)) \\](https://www.zhihu.com/equation?tex=r_t+%5Cequiv%5Clog+D_%5Cphi+%28%5Ctilde%7Bx%7D_t%7C%5Ctilde%7Bx%7D_%7B0%3AT%7D%2Cm%28x%29%29+%5C%5C)

最后一个部分则是 critic 网络，critic 网络的作用是对做出的动作估计 value function，这里表示填充后的序列总的折扣收益： ![R_t=\sum_{s=t}^{T}{\gamma^sr_s}](https://www.zhihu.com/equation?tex=R_t%3D%5Csum_%7Bs%3Dt%7D%5E%7BT%7D%7B%5Cgamma%5Esr_s%7D) ， ![\gamma](https://www.zhihu.com/equation?tex=%5Cgamma) 表示每一个位置上的折扣率。

**3、训练目标**

这篇论文同样是利用 policy gradient 方法对生成器的参数 ![\theta](https://www.zhihu.com/equation?tex=%5Ctheta) 进行训练。生成器优化目标则是最大化累积总奖励 ![R=\sum \nolimits_{t=1}^{T}{R_t}](https://www.zhihu.com/equation?tex=R%3D%5Csum+%5Cnolimits_%7Bt%3D1%7D%5E%7BT%7D%7BR_t%7D) 。所以，我们通过对 ![\mathbb{E}_{G(\theta)}[R]](https://www.zhihu.com/equation?tex=%5Cmathbb%7BE%7D_%7BG%28%5Ctheta%29%7D%5BR%5D) 进行梯度上升操作来优化生成器的参数 ![\theta](https://www.zhihu.com/equation?tex=%5Ctheta) 。



论文七、《Long Text Generation via Adversarial Training with Leaked Information》

AAAI 2018

![](https://pic4.zhimg.com/80/v2-8c61deb3bfc275996a9c0ff380966e81_hd.jpg)

**1、写作动机**

作者认为，虽然之前已经有许多研究将 policy gradient 方法运用到 GAN 的文本生成任务中，也取得了一些成功，但这些方法最后返回的奖励只有当生成器完全生成序列之后才能得到，序列生成过程中缺乏一些关于序列结构的中间信息的反馈，而且，这个奖励标量没有太多关于序列语义结构方面的信息量，无法起到特别大的指导作用，这些限制了序列生成的长度（一般不大于20）。在这篇论文中，作者提出了一个 LeakGAN 模型来解决长文本生成的问题。判别器会在中间时间步泄露一些提取的特征给生成器，生成器则利用这个额外信息指导序列的生成。

**2、模型定义**

![](https://pic4.zhimg.com/80/v2-50fe6c97e81f0bb6d92ce2b228ac3940_hd.jpg)

如上图，在生成器中，作者使用的是 hierarchical reinforcement learning 结构，包括 Manager 模块和 Worker 模块。Manager 模块是一个 LSTM 网络，充当中介的一个角色。在每个时间步上，它都会从判别器中接收一个特征表示（例如，CNN 中的 feature map），然后将它作为一个指导信号传递为 Worker 模块。因为判别器的中间信息在原始的 GAN 中是不应该被生成器知道的，所以作者把这个特征表示称为泄露的信息。接收到这个指导信号的 embedding 之后，Worker 模块同样也利用 LSTM 网络编码当前输入 ![x_t](https://www.zhihu.com/equation?tex=x_t) ，然后将 LSTM 的输出和收到的指导信号 embedding 连接在一起共同计算下一步的动作，即选择下一个单词。

**泄露的特征表示：**

传统的 RL 下，模型的 reward functi\ ![\phi=(\phi_f,\phi_l)](https://www.zhihu.com/equation?tex=%5Cphi%3D%28%5Cphi_f%2C%5Cphi_l%29) on 一般是个黑匣子，而在这个对抗文本生成模型中，作者使用判别器 ![D_\phi](https://www.zhihu.com/equation?tex=D_%5Cphi) 作为可学习的 reward function， ![D_\phi](https://www.zhihu.com/equation?tex=D_%5Cphi) 可以分解为一个特征提取器 ![F(\cdot;\phi_f)](https://www.zhihu.com/equation?tex=F%28%5Ccdot%3B%5Cphi_f%29) 和一个权重为 ![\phi_l](https://www.zhihu.com/equation?tex=%5Cphi_l) 的 sigmoid 分类层。数学化表示为，给定一个输入 ![s](https://www.zhihu.com/equation?tex=s) ，有

![D_\phi(s)=\text{sigmoid}(\phi_l^{\text{T}}F(s;\phi_f))=\text{sigmoid}(\phi_l^{\text{T}}f) \tag{1}\\](https://www.zhihu.com/equation?tex=D_%5Cphi%28s%29%3D%5Ctext%7Bsigmoid%7D%28%5Cphi_l%5E%7B%5Ctext%7BT%7D%7DF%28s%3B%5Cphi_f%29%29%3D%5Ctext%7Bsigmoid%7D%28%5Cphi_l%5E%7B%5Ctext%7BT%7D%7Df%29+%5Ctag%7B1%7D%5C%5C)

这里的 ![\phi=(\phi_f,\phi_l)](https://www.zhihu.com/equation?tex=%5Cphi%3D%28%5Cphi_f%2C%5Cphi_l%29) ， ![f=F(s;\phi_f)](https://www.zhihu.com/equation?tex=f%3DF%28s%3B%5Cphi_f%29) 是输入 ![s](https://www.zhihu.com/equation?tex=s) 的特征向量，也就是泄露给生成器的中间信息。从式（1）可以看出，判别器对每一时刻的状态 ![s](https://www.zhihu.com/equation?tex=s) 的奖励值依赖于提取出的特征 ![f](https://www.zhihu.com/equation?tex=f) 。也正因为这样，从判别器 ![D_\phi](https://www.zhihu.com/equation?tex=D_%5Cphi) 中获得一个高奖励值的优化目标可以等价为在特征空间 ![F(S;\phi_f)=\{F(s;\phi)\}_{s\in S}](https://www.zhihu.com/equation?tex=F%28S%3B%5Cphi_f%29%3D%5C%7BF%28s%3B%5Cphi%29%5C%7D_%7Bs%5Cin+S%7D) 中找到一个高奖励值的的区域。相比于之前奖励值为标量的模型，特征向量 ![f](https://www.zhihu.com/equation?tex=f) 可以提供更多的指导信息给生成器。

**生成器层次结构：**

Manager：LSTM 的初始化状态为 ![h^M_0](https://www.zhihu.com/equation?tex=h%5EM_0) ，在每个时间步 ![t](https://www.zhihu.com/equation?tex=t) 上，以泄露的特征向量 ![f_t](https://www.zhihu.com/equation?tex=f_t) 为输入，输出目标向量 ![g_t](https://www.zhihu.com/equation?tex=g_t) ，如下式：

![\hat{g}_t,h^M_t=M(f_t,h^M_{t-1};\theta_m) \\](https://www.zhihu.com/equation?tex=%5Chat%7Bg%7D_t%2Ch%5EM_t%3DM%28f_t%2Ch%5EM_%7Bt-1%7D%3B%5Ctheta_m%29+%5C%5C)

![g_t=\hat{g}_t/||\hat{g}_t|| \\](https://www.zhihu.com/equation?tex=g_t%3D%5Chat%7Bg%7D_t%2F%7C%7C%5Chat%7Bg%7D_t%7C%7C+%5C%5C)

为了结合 Manager 产生的目标向量 ![g_t](https://www.zhihu.com/equation?tex=g_t) ，作者对最新的 c 个目标向量进行求和并进行权重为 ![W_\psi](https://www.zhihu.com/equation?tex=W_%5Cpsi) 的线性变换，得到一个 k 维的目标嵌入 ![w_t](https://www.zhihu.com/equation?tex=w_t) ：

![w_t=\psi(\sum_{i=1}^{c}{g_{t-i}})=W_\psi(\sum_{i=1}^{c}{g_{t-i}}) \\](https://www.zhihu.com/equation?tex=w_t%3D%5Cpsi%28%5Csum_%7Bi%3D1%7D%5E%7Bc%7D%7Bg_%7Bt-i%7D%7D%29%3DW_%5Cpsi%28%5Csum_%7Bi%3D1%7D%5E%7Bc%7D%7Bg_%7Bt-i%7D%7D%29+%5C%5C)

Worker：以当前单词 ![x_t](https://www.zhihu.com/equation?tex=x_t) 为输入，输出一个矩阵 ![O_t](https://www.zhihu.com/equation?tex=O_t) ，它将通过矩阵乘法和目标嵌入 ![w_t](https://www.zhihu.com/equation?tex=w_t) 连接在一起，决定下一步动作的概率分布：

![O_t,h^W_t=W(x_t,h^W_{t-1};\theta_w) \\](https://www.zhihu.com/equation?tex=O_t%2Ch%5EW_t%3DW%28x_t%2Ch%5EW_%7Bt-1%7D%3B%5Ctheta_w%29+%5C%5C)

![G_\theta(\cdot|s_t)=\text{softmax}(O_t\cdot w_t/\alpha) \\](https://www.zhihu.com/equation?tex=G_%5Ctheta%28%5Ccdot%7Cs_t%29%3D%5Ctext%7Bsoftmax%7D%28O_t%5Ccdot+w_t%2F%5Calpha%29+%5C%5C)

**3、训练生成器**

在 LeakGAN 模型中，作者对生成器中的 Manager 模块和 Worker 模块分别训练。Manager 模块的训练目标是要在判别器的特征空间中找到有利的指导方向，Worker 模块的训练目标是遵循这个方向以得到奖励。

Manager 模块的梯度为：

![\nabla^{adv}_{\theta_m}g_t=-Q_{F}(s_t,g_t)\nabla_{\theta_m}d_{cos}(f_{t+c}-f_t,g_t(\theta_m)) \\](https://www.zhihu.com/equation?tex=%5Cnabla%5E%7Badv%7D_%7B%5Ctheta_m%7Dg_t%3D-Q_%7BF%7D%28s_t%2Cg_t%29%5Cnabla_%7B%5Ctheta_m%7Dd_%7Bcos%7D%28f_%7Bt%2Bc%7D-f_t%2Cg_t%28%5Ctheta_m%29%29+%5C%5C)

其中 ![Q_F(s_t,g_t)=Q(F(s_t),g_t)=Q(f_t,g_t)=\mathbb{E}[r_t]](https://www.zhihu.com/equation?tex=Q_F%28s_t%2Cg_t%29%3DQ%28F%28s_t%29%2Cg_t%29%3DQ%28f_t%2Cg_t%29%3D%5Cmathbb%7BE%7D%5Br_t%5D) ，是当前策略下的预期奖励，可以通过 Monte Carlo 搜索近似估计。 ![d_{cos}](https://www.zhihu.com/equation?tex=d_%7Bcos%7D) 表示 c 步之后特征向量的改变 ![f_{t+c}-f_t](https://www.zhihu.com/equation?tex=f_%7Bt%2Bc%7D-f_t) 和目标向量 ![g_t(\theta_m)](https://www.zhihu.com/equation?tex=g_t%28%5Ctheta_m%29) 之间的余弦相似度。损失函数要求目标向量匹配特征空间的转换，同时获得更高奖励。同时，Worker 模块要最大化这个奖励，这可以通过对状态 ![s_{t-1}](https://www.zhihu.com/equation?tex=s_%7Bt-1%7D) 和动作 ![x_t](https://www.zhihu.com/equation?tex=x_t) 进行采样来近似。

![\nabla_{\theta_m}\mathbb{E}_{s_{t-1}\sim G}[\sum_{x_t}^{}{r^I_tW(x_t|s_{t-1};\theta_w)}]=\mathbb{E}_{s_{t-1}\sim G,x_t\sim W(x_t|s_{t-1})}[r^I_t\nabla_{\theta_m}\log W(x_t|s_{t-1};\theta_w)] \\](https://www.zhihu.com/equation?tex=%5Cnabla_%7B%5Ctheta_m%7D%5Cmathbb%7BE%7D_%7Bs_%7Bt-1%7D%5Csim+G%7D%5B%5Csum_%7Bx_t%7D%5E%7B%7D%7Br%5EI_tW%28x_t%7Cs_%7Bt-1%7D%3B%5Ctheta_w%29%7D%5D%3D%5Cmathbb%7BE%7D_%7Bs_%7Bt-1%7D%5Csim+G%2Cx_t%5Csim+W%28x_t%7Cs_%7Bt-1%7D%29%7D%5Br%5EI_t%5Cnabla_%7B%5Ctheta_m%7D%5Clog+W%28x_t%7Cs_%7Bt-1%7D%3B%5Ctheta_w%29%5D+%5C%5C)

当鼓励 Worker 模块遵循 Manager 指示的方向时，Worker 的内在奖励定义为：

![r^I_t=\frac {1}{c}\sum_{i=1}^{c}{d_{cos}(f_t-f_{t-i},g_{t-i})} \\](https://www.zhihu.com/equation?tex=r%5EI_t%3D%5Cfrac+%7B1%7D%7Bc%7D%5Csum_%7Bi%3D1%7D%5E%7Bc%7D%7Bd_%7Bcos%7D%28f_t-f_%7Bt-i%7D%2Cg_%7Bt-i%7D%29%7D+%5C%5C)



转载自：

[https://zhuanlan.zhihu.com/p/36880287?utm_source=wechat_timeline&utm_medium=social&utm_oi=623434717970698240&from=timeline&isappinstalled=0&wechatShare=1](https://zhuanlan.zhihu.com/p/36880287?utm_source=wechat_timeline&utm_medium=social&utm_oi=623434717970698240&from=timeline&isappinstalled=0&wechatShare=1)






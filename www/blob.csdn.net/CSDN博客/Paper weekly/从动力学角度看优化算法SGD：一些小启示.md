# 从动力学角度看优化算法SGD：一些小启示 - Paper weekly - CSDN博客





2018年07月09日 12:01:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：277









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhglryG74dIr2B1019Yibv9PAGsWGGYBiaoSGbK2kzUnbIsicCEiazKMticicR0MPtmr1ynDovFe2kGicSydcg/640)




作者丨苏剑林

单位丨广州火焰信息科技有限公司

研究方向丨NLP，神经网络

个人主页丨kexue.fm




在本文中，我们来关心优化算法 SGD（stochastic gradient descent，随机梯度下降），包括带 Momentum 和 Nesterov 版本的。**对于 SGD，我们通常会关心的几个问题是： **



- 
SGD 为什么有效？ 

- 
SGD 的 batch size 是不是越大越好？ 

- 
SGD 的学习率怎么调？ 

- 
Momentum 是怎么加速的？ 

- 
Nesterov 为什么又比 Momentum 稍好？ 

- 
... 





这里试图从动力学角度分析 SGD，给出上述问题的一些启发性理解。




# 梯度下降




既然要比较谁好谁差，就需要知道最好是什么样的，也就是说我们的终极目标是什么？




**训练目标分析**




假设全部训练样本的集合为 S，损失度量为 L(x;θ)，其中 x 代表单个样本，而 θ 则是优化参数，那么我们可以构建损失函数：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglCFFhpJw2nEURPiayMHa02CMK5Wh8RHErMWnM2AS3qmPtxQD4jd8yvF6qTuH299HFgPfwY2dHnnDg/640)




而**训练的终极目标，则是找到 L(θ) 的一个全局最优点**（这里的最优是“最小”的意思）。




**GD与ODE**




为了完成这个目标，我们可以用梯度下降法（gradient descent，GD）：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglCFFhpJw2nEURPiayMHa02CBvAibZrx8KjMN6OCP4lBPLEqUh8wP9IWickfd9lN2rzm3USsTbX5g3WA/640)




其中 ε>0 称为学习率，这里刚好也是迭代的步长（后面我们会看到步长不一定等于学习率）。梯度下降有多种理解方式，由于一般都有 ε≪1，所以这里我们将它改变为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglCFFhpJw2nEURPiayMHa02CSA6ibk8fcTNeUlcHXG03qicvbeEzVURp9Qlxzs1cYs9FGhJicSOBrRF6A/640)




那么左边就近似于 θ 的导数（假设它是时间 t 的函数），于是我们可以得到 ODE 动力系统：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglCFFhpJw2nEURPiayMHa02CHTMLHPZcOd51taiaIzOpeZYXtkkMzJianhb9q3ciaZ96cRLib2icKk5EJHA/640)




而 (2) 则是 (4) 的一个欧拉解法。这样一来，**梯度下降实际上就是用欧拉解法去求解动力系统 (4)**，由于 (4) 是一个**保守动力系统**，因此它最终可以收敛到一个不动点（让 θ˙=0），并且可以证明稳定的不动点是一个极小值点（但未必是全局最小的）。

# 随机梯度下降




这里表明，随机梯度下降可以用一个随机微分方程来半定性、半定量地分析。 




**从GD到SGD**




(2) 我们一般称为“全量梯度下降”，因为它需要所有样本来计算梯度，这是梯度下降的一个显著缺点：当样本成千上万时，每迭代一次需要的成本太大，甚至可能达到难以进行。于是我们想随机从 S 中随机抽取一个子集 R⊆S，然后只用 R 去计算梯度并完成单次迭代。我们记：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglCFFhpJw2nEURPiayMHa02CMDVwQ9JK5vden6GBFMlqwWxfUvCncCkiboxSzicBlC2qb44icYm3hs8Sw/640)




然后公式 (2) 变为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglCFFhpJw2nEURPiayMHa02CrXLaraSka4BYoewx9hmoJvfye4C7glOR5GcP5dY7lIf9GBibcpksgWg/640)




注意 *L* 的最小值才是我们的目标，而 *LR* 则是一个随机变量，∇*θLR*(*θn*) 只是原来 ∇*θL*(*θn*) 的一个估计。这样做能不能得到合理的结果呢？




**从SGD到SDE**




这里我们假设：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglCFFhpJw2nEURPiayMHa02CZXnSGQF3ibORvVsWHNoW7ibxnCrXEibuiaHXj4FckolgIpj30INSfHBVIQ/640)




服从一个方差为 σ^2 的正态分布，注意这只是一个近似描述，主要是为了半定性、半定量分析。经过这样假设，随机梯度下降相当于在动力系统 (4) 中引入了高斯噪声：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglCFFhpJw2nEURPiayMHa02CP7UToK8vTIndI5icicZeLZ4ibxV4fF6KfWk22KgSAeYepnSmlmZia8jQxA/640)




**原来的动力系统是一个 ODE，现在变成了 SDE（随机微分方程），我们称之为“朗之万方程”**。当然，其实噪声的来源不仅仅是随机子集带来的估算误差，每次迭代的学习率大小也会带来噪声。




在噪声的高斯假设下，这个方程的解是怎样的呢？原来的 ODE 的解是一条确定的轨线，现在由于引入了一个随机噪声，因此解也是随机的，可以解得平衡状态的概率分布为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglCFFhpJw2nEURPiayMHa02CYg9U9Zfenl6Le13SBSkZjpfauiaQVxu6PEan5vzlm4DDvKoJcZCer3w/640)




求解过程可以参考一般的随机动力学教程，我们只用到这个结果就好。




**结果启发**




从 (8) 式中我们可以得到一些有意义的结果。首先我们看到，原则上来说这时候的 *θ* 已经不是一个确定值，而是一个概率分布，而且原来 *L*(*θ*) 的极小值点，刚好现在变成了 *P*(*θ*) 的极大值点。这说明如果我们无限长地执行梯度下降的话，理论上 *θ* 能走遍所有可能的值，并且在 *L*(*θ*) 的各个“坑”中的概率更高。




σ^2 是梯度的方差，显然这个方差是取决于 batch size 的，根据定义 (7)，batch size 越大方差越小。而在 (9) 式中，σ^2 越大，说明 *P*(*θ*) 的图像越平缓，即越接近均匀分布，这时候 *θ* 可能就到处跑；当 σ^2 越小时，原来 *L*(*θ*) 的极小值点的区域就越突出，这时候 *θ* 就可能掉进某个“坑”里不出来了。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglCFFhpJw2nEURPiayMHa02ChHzpDSscOxDtyWwibO6V4olWBVjUEKz6wcLzCQdkBOd9OicQPMa16mgg/640)

▲ L(θ)曲线




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglCFFhpJw2nEURPiayMHa02Chxiav8uIuHB1SbOFBezibVCeBrfCqJ5OQwlLrsYEb9fbyzjjvcpltE5Q/640)

▲ exp(-L(θ))曲线




这样分析的话，**理论上来说，我们一开始要让 batch size 小一些，那么噪声方差 σ^2 就会大一些，越接近均匀分布，算法就会遍历更多的区域，随着迭代次数的增加，慢慢地就会越来越接近最优区域，这时候方差应该要下降，使得极值点更为突出。也就是说，有可能的话，batch size 应该要随着迭代次数而缓慢增加**。这就部分地解释了去年 Google 提出来的结果[《学界 | 取代学习率衰减的新方法：谷歌大脑提出增加Batch Size](https://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650732855&idx=5&sn=a4f9f5cf712ada42652ec97fcc4dc698&chksm=871b3d49b06cb45fea6924f1b584ae01e20b9882992826cb989ba0c1db186032fb42627c9147&mpshare=1&scene=21&srcid=0627rrCLJpeTQU35IpW7nn6Y#wechat_redirect)》，不过 batch size 增加会大幅度增加计算成本，所以我们一般增加到一定量也就不去调整了。




当然，从图中可以看到，**当进入稳定下降区域时，每次迭代的步长 ε（学习率）就不应该超过“坑”的宽度，而 σ^2 越小，坑也就越窄，这也表明学习率应该要随着迭代次数的增加而降低**。另外 ε 越大也部分地带来噪声，因此 σ^2 降低事实上也就意味着我们要降低学习率。




所以分析结果就是： 




> 
条件允许情况下，在使用 SGD 时，开始使用小 batch size 和大学习率，然后让 batch size 慢慢增加，学习率慢慢减小。 





至于增大、减少的策略，就要靠各位的炼丹水平了。




# 动量加速




众所周知，相比 Adam 等自适应学习率算法，纯 SGD 优化是很慢的，而引入动量可以加速 SGD 的迭代。它也有一个漂亮的动力学解释。




**从一阶到二阶**




从前面的文字我们知道，SGD 和 GD 在迭代格式上没有什么差别，所以要寻求 SGD 的加速，我们只需要寻求 GD 的加速，然后将全量梯度换成随机梯度就行了。而 (2) 式到 (4) 式的过程我们可以知道，GD 将求 L(θ) 的最小值问题变成了 ODE 方程![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglCFFhpJw2nEURPiayMHa02Cu8FROuryibZ8xR1LM5ewp2OLvlib1TR9Aw87JUic6ueFiaFPKYTsI54ib7g/640)的迭代求解问题。




那么，为什么一定要是一阶的呢？二阶的![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglCFFhpJw2nEURPiayMHa02Ck085ekPbgsC4sSITCkUEDXxmDAaZJRQNjTnS4nJ4ia5uBo6oHibzLnTw/640)行不？




为此，我们考虑一般的：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglCFFhpJw2nEURPiayMHa02CKj9x1BhnFtZYNB9qCC3scPPucwbeWdBsdKSvMT18gxtV2R6PwOFJgg/640)




这样就真正地对应一个（牛顿）力学系统了，其中 λ>0 引入了类似摩擦力的作用。我们来分析这样的系统跟原来 GD 的一阶 ODE (4) 与 (10) 有什么差别。




首先，从不动点的角度看，(10)最终收敛到的稳定不动点（让![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglCFFhpJw2nEURPiayMHa02CkSC192zeLdXZ2hk2vKIicaTGjlFv1vdRg4dialNK3cJh1PtEFU2o8JJA/640)），确实也是 *L*(*θ*) 的一个极小值点。试想一下，一个小球从山顶滚下来，会自动掉到山谷又爬升，但是由于摩擦阻力的存在，最终就停留在山谷了。注意，除非算法停不了，否则一旦算法停了，那么一定是一个山谷（也有可能是山顶、鞍点，但从概率上来讲则是比较小的），但绝对不可能是半山腰，因为半山腰的话，还存在势能，由能量守恒定律，它可以转化为动能，所以不会停下来。




因此收敛情况跟一阶的 GD 应该是没有差别的，所以只要比较它们俩的收敛速度。




**GD+Momentum**




我们将 (10) 等价地改写为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglCFFhpJw2nEURPiayMHa02CfzN6qhyiaZOsv3Z5FdBj3rY3O7BXhxsUiaa3ricQwgNGiclmHuDKGZLia0A/640)




我们将 θ˙ 离散化为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglCFFhpJw2nEURPiayMHa02CiaL6KjvibVUVMfObFPCkxkvnVvEWNlZARJYBq54iaPIMXpUIJp1LM6MoA/640)




那么 η 要怎么处理呢？ηn？不对不对，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglCFFhpJw2nEURPiayMHa02CFhr38iaFHIMg5Aicukc53ot0ZqqGdpP2TBibox48rIu1OVx6TpCSevAcw/640)作为 n 时刻的导数只有一阶近似（            





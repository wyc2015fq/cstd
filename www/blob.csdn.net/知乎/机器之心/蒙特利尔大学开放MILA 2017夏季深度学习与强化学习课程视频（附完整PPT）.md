# 蒙特利尔大学开放MILA 2017夏季深度学习与强化学习课程视频（附完整PPT） - 知乎
# 



机器之心整理

**参与：黄小天、蒋思源**

> *2016 年，Aaron Courville 和 Yoshua Bengio 组织的 MILA 深度学习夏季课程获得了极大的关注。今年，新一届的 MILA 深度学习与强化学习夏季课程开放了 PPT 和教学视频。机器之心摘选了 Bengio、Goodfellow 和 Sutton 的部分亮点并简要介绍了该课程。*
- 课程首页：[https://mila.umontreal.ca/en/cours/deep-learning-summer-school-2017/](https://link.zhihu.com/?target=https%3A//mila.umontreal.ca/en/cours/deep-learning-summer-school-2017/)
- 课程视频地址：[http://videolectures.net/deeplearning2017_montreal/](https://link.zhihu.com/?target=http%3A//videolectures.net/deeplearning2017_montreal/)
- PDF 下载地址：[https://mila.umontreal.ca/en/cours/deep-learning-summer-school-2017/slides/](https://link.zhihu.com/?target=https%3A//mila.umontreal.ca/en/cours/deep-learning-summer-school-2017/slides/)

## **课程介绍**

本届 MILA 2017 夏季课程包括深度学习夏季课程和强化学习夏季课程，以下是对这两个课程的官方介绍：

## **深度学习夏季课程**

深度神经网络学习在多层抽象网络中呈现数据，这极大地提升了语音识别、目标识别、目标检测、预测药物分子活动和其他多项任务的水平。深度学习通过监督学习、无监督学习和强化学习构建分布式表征（distributed representation），在大型数据集中构建复杂的结构。



![](https://pic4.zhimg.com/v2-a06bd4ec39a7b6ed10d70c78cd794d5b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='899'></svg>)



深度学习夏季课程面向已具备机器学习（也可以是深度学习，但不是必须）领域基础知识并想了解更多的硕士研究生、工程师和研究者。今年的深度学习课程由 Graham Taylor、Aaron Courville 和 Yoshua Bengio 共同组织。

## **强化学习夏季课程**

我们将要举办第一届蒙特利尔强化学习夏季课程。强化学习课程将会覆盖强化学习的基础知识，并且展示其最前沿的研究方向和新发现，还会提供与研究生和业内高级研究人员进行交流的机会。



![](https://pic4.zhimg.com/v2-441d3065927e87fb63ae532e975139df_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='318'></svg>)



本课程主要面向机器学习及其相关领域的研究生。参加者应该有过计算机科学和数学方面的进阶预训练的经验，来自机器和大脑学习 CIFAR 项目的附属研究实验室的学生将会被优先考虑。我们也强烈欢迎非以上典型小组的代表来申请此次课程。

近日，该课程的众多 PDF 已经开放，感兴趣的读者可从以下网址下载。



![](https://pic3.zhimg.com/v2-4f10fac133f1d3c7c3180ca01547691e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='480'></svg>)





![](https://pic2.zhimg.com/v2-4e43a352f67659ae3c0582a4e9c4c73d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='488'></svg>)





![](https://pic3.zhimg.com/v2-adad3b8a7b2b9c36a9f2c8dfb8f148b2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='325'></svg>)



## **课程亮点**

MILA 2017 夏季课程结束之后，机器之心结合公开的课程资料对这些课程进行了介绍，希望能辅助大家学习。

## **1.Yoshua Bengio：循环神经网络**

Bengio 在课程中主要介绍的就是循环，包括了循环神经网络定义、变体、存在的问题、训练技巧、最先进的注意力机制、RNN 架构的设计和一些非常前沿的概率图模型。

## **循环神经网络**

首先 Bengio 当然还是先介绍了循环神经网络的结构，如下图所示，我们可以将循环神经网络展开到详细的连接。RNN 每一隐藏层的值都不仅取决于当前的输入，同时还取决于还取决于上一次隐藏层的值。其中 x 代表输入、s 代表隐藏值、O 代表输出值。我们可以看到，第 t 个隐藏值 x_t 不仅依赖于第 t 个输入 x_t，还依赖于前一次的隐藏值 s_t-1 才能得出第 t 个观察值 O_t。



![](https://pic1.zhimg.com/v2-b3e83120666b3870d74138484ac63e0c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='299'></svg>)





![](https://pic4.zhimg.com/v2-5c04b08905831c5862a686a4a6bed933_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='252'></svg>)



## **梯度消失和爆炸问题**

梯度消失和爆炸问题一直是循环神经网络所面临的巨大挑战。因为当权值过大，前面层比后面层梯度变化更快，会引起梯度爆炸问题，但是如果前面的层比后面的层梯度变化更小，故变化更慢，从而会引起梯度消失问题。我们可以看到，如果雅可比矩阵的谱半径小于 1，那么所传播的梯度就会消失。



![](https://pic4.zhimg.com/v2-04ff1cb5f2fbd8856288e2f6b333c4bf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='363'></svg>)



从下图我们可以看到十分实用的 RNN 训练技巧，包括使用梯度截断（Clipping gradients）防止梯度爆炸问题，使用渗漏集成（Leaky integration）以传播长期依赖性，采用动量（Momentum）加速梯度下降的效率，在大概近似的区域内初始化神经网络以避免陷入局部解或出现梯度消失和爆炸问题。同时，我们还可以使用系数梯度以打破对称性而是训练更加高效，在执行梯度下降时还可以使用正则化器和门控自循环（Gated self-loops，如 LSTM 和 GRU）以避免或降低梯度消失问题。



![](https://pic4.zhimg.com/v2-c43e5283a4e24d511e0b3f104ed10fc7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='404'></svg>)



## **注意力机制**

注意力机制近来在 NLP 领域起到十分重要的作用，它可以减少编码器-解码器模型的缺陷，尤其是在长序列问题上有极其出色的表现。注意力机制可以用于循环神经网路，大大提高了其处理标注问题的能力。注意力机制如下所示通过在低层序列和高层序列间加入隐藏的注意力分配向量而大大加强了性能。



![](https://pic1.zhimg.com/v2-5ecbc7044d57ee4ba8feda0185c92f28_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='424'></svg>)



## **循环神经网络的架构**

如下展示了 RNN 架构的基本概念，循环深度表示由序列长度切分的最大路径长度，前馈深度表示从输入到最近输出的最大深度，而跳过系数则表示由序列长度切分的最短路径长度。这些概念都是设计 RNN 架构所必须了解的，Bengio 也在课程中做了进一步详细说明。



![](https://pic1.zhimg.com/v2-fbd0b0440941be07ae3167527abda564_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='482'></svg>)



## **2.Ian Goodfellow：Generative Models I**



![](https://pic3.zhimg.com/v2-559d3f1ba4cba4758475fff6e123d5c6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='340'></svg>)



Ian Goodfellow 在此次夏季课程的演讲主题是 Generative Models I，Generative Models II 则由 Aaron Courville 接着讲授。Ian Goodfellow 这次讲授的内容主要分为两部分：生成模型的分类（Taxonomy of Generative Models）与应用（What can you do with generative models?）。下面首先介绍第一部分，生成模型的分类。



![](https://pic1.zhimg.com/v2-09a51c4cf7b63ebfa7802322478237a0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='360'></svg>)



有监督机器学习方法可以分为生成方法和判别方法，生成方法学习出的是生成模型，判别方法学习出的是判别模型。生成模型的本质是密度估计（Density Estimation），其基本思想是首先建立样本的概率密度模型，再利用模型进行推理预测。生成模型的处理过程会告诉你关于数据的一些统计信息（p(x|y) 的分布等），更接近于统计学。这种方法一般建立在统计学贝叶斯理论的基础之上。



![](https://pic2.zhimg.com/v2-2f925764bf79bc8ec6cc918df9dcff09_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='357'></svg>)



生成方法由数据学习联合概率分布，然后求出条件概率分布作为预测模型；生成模型是根据已知的样本用基于统计方法来估计整个样本空间的真实分布，它表示了给定输入 X（训练样本）产生输出 Y（模型样本）的生成关系。



![](https://pic1.zhimg.com/v2-09a51c4cf7b63ebfa7802322478237a0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='360'></svg>)



机器学习中，生成模型可以用来直接对数据建模（例如根据某个变量的概率密度函数进行数据采样）。如果观测数据是由生成模型中采样的，那么最大化数据似然概率是一个常见的方法。最大似然估计最早是由罗纳德·费雪在 1912 年至 1922 年间推荐、分析并大范围推广的。在统计学中，最大似然估计是用来估计一个概率模型的参数的一种方法。Ian Goodfellow 据此对生成模型做了一个分类，如下：



![](https://pic3.zhimg.com/v2-696567c96dc9c7d696987429ecc44bd2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='355'></svg>)



一般来讲，典型的生成模型有：朴素贝叶斯法、隐马尔科夫模型、高斯混合模型、AODE、隐狄利克雷分配模型（Latent Dirichlet allocation）、受限波尔兹曼机（Restricted Boltzmann Machine）。Ian Goodfellow 在这次讲授中根据最大似然估计（可再分为显性密度和隐性密度）给出了生成模型的五个分类：GAN、生成随机网络（GSN）、玻尔兹曼机、变分自编码器（VAE）、全可见信念网络（Fully visible belief nets）。



![](https://pic4.zhimg.com/v2-4b67cc5479b378a49e457c981fd7a2fb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='358'></svg>)



显性密度可再分为驯服密度（Tractable density）和近似密度（Approximate density）。根据驯服密度，生成模型可分为全可见信念网络。



![](https://pic3.zhimg.com/v2-047bf179e0542f0ad9a32fc1407dbf66_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='358'></svg>)



全可见信念网络是基于链式规则（Frey et al, 1996）的显性公式。它有一些自身的缺陷：O(n) 非平行样本生成运行时间；生成不受隐藏代码控制。



![](https://pic4.zhimg.com/v2-d2b97b580daf20a455ccea4e5b7c80db_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='356'></svg>)



近似密度可再划分为可变分（Variational）与马尔可夫链（Markov Chain）；根据前者生成模型可分为变分自编码器（VAE），根据后者可分为玻尔兹曼机。



![](https://pic2.zhimg.com/v2-481c712dbf07282c9c41006b9370148d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='361'></svg>)



变分自编码器是一类重要的生成模型，它于 2013 年由 Diederik P.Kingma 和 Max Welling 提出，是一种基于变分贝叶斯推断的生成网络。2016 年 Carl Doersch 写了一篇 VAE 的 tutorial，对 VAE 做了更易懂的详细介绍。



![](https://pic4.zhimg.com/v2-6eeb0146cc148745293447ec405d1d5f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='357'></svg>)



根据隐性密度的马尔可夫链，生成模型可划分为生成随机网络（GSN），它最早由 Bengio 于 2013 年提出。



![](https://pic3.zhimg.com/v2-9fc1f60f1a2eef7ae7c3bb1c0525f272_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='357'></svg>)



在这次课程的后半部分，Ian Goodfellow 讲述了生成模型的实际应用，分为以下几个方面：
- 模拟化环境与训练数据
- 丢失性数据
- 多正确答案
- 逼真化生成任务
- 基于预测的模拟
- 学习有用性嵌入

关于生成模型的后续内容，可参见 Aaron Courville 讲授的 Generative Models II。

## **3.Rich Sutton：Temporal-Difference Learning**



![](https://pic2.zhimg.com/v2-1448952faa4208f579527b39278d5901_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='301'></svg>)



Richard S. Sutton 教授被认为是现代计算的强化学习创立者之一。他为该领域做出了许多重大贡献，包括：时间差分学习（temporal difference learning）、策略梯度方法（policy gradient methods）等。本次课程上，Sutton 教授讲解的就是事件差分学习。

在 Sutton 最开始的课程中，他表明随着计算力的增长，可扩展的能力也需要增长，这样才是人工智能的未来。Sutton 表明，即使是弱一些的一般方法也要比强大的特定方法好。因为监督学习和免模型的强化学习方法在泛化性能上十分欠缺。但是预测学习（Prediction Learning）是可泛化的，因为它是一种无监督学习。虽然预测学习也需要目标，但这些目标并不需要人工标注。因此预测学习才是可扩展的免模型学习。

时间差分学习（Temporal-difference learning）是一种学习预测的方法，它广泛应用于强化学习来预测未来的奖励（Reward）或价值函数。时间差分学习是 Q 学习、深度 Q 网络、时间差分-Gammon、actor-critic 等方法的关键，它还能用于预测任何信号而不只是奖励。时间差分学习主要从其他的已学习的预测学习一个新的预测。



![](https://pic4.zhimg.com/v2-dfdf072e5b95fd8167ec73b4e59d0e0b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='469'></svg>)



正如上图所示，时间差分学习首先从一个随机网络开始，并且通过自我博弈数百万次而从模拟的经验中学习到价值函数。

但是我们一定需要使用时间差分学习吗，我们是不是可以使用其他强大的监督学习方法？如下所示，很多 AlphaGo 和其他 Atari 游戏都是使用的强化学习和深度学习。



![](https://pic2.zhimg.com/v2-6a5151a8b53f1a9e4e5eccf2b0e6886d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='480'></svg>)



时间差分学习只和多步预测问题相关，也就是我们预测的东西在未来会经过多个步骤。而模型会揭示每一步骤的可能包含的信息。这种多步预测十分常见，比如说预测游戏的结果、股票市场一段时间后的走势等等。Sutton 表示我们不能将这些多步预测转换为单步预测而使用强的机器学习方法处理，因为任何一个多步预测都需要由多次模拟，并且只有在完成一步之后才能预测下一步的状态和观察值。

下面 Sutton 详细介绍了时间差分学习（Temporal difference learning）。



![](https://pic4.zhimg.com/v2-d61434dee838886f3194fbc8fdea398b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='429'></svg>)



除此之外，Sutton 还对比了监督学习蒙特卡罗（MC）方法、最简单的时间差分方法和动态规划方法。



![](https://pic1.zhimg.com/v2-9e140d52db92e9b33e1eb6cecc01a274_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='360' height='882'></svg>)



对于会涉及到估计而更新的 Bootstrapping 而言，MC 方法并不适用 Bootstrap，而动态规划和时间差分方法都会使用 Bootstrap。对于不涉及期望而更新的采样，动态规划并不会使用采样，而 MC 方法和时间差分方法都会使用。

下面对比了 MC 方法和时间差分方法的更新策略：



![](https://pic1.zhimg.com/v2-b631f37993cda324ea425ad38487b474_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='416'></svg>)



从上图可以看出，策略评估或者预测问题的更新过程，即给定一个策略π，我们需要计算状态值函数 v_π。在更新状态值函数中，我们可以看到蒙特卡罗方法使用的是时间 t 上的真实反馈目标，即人工标注的目标。而在时间差分方法中，我们更新使用的目标是一个概率估计，所以这种无监督的方法才有更好的扩展能力。

Sutton 在前面介绍了单步扁平（tabular）免模型时间差分方法，并且时间差分方法结合了动态规划和 MC 方法的优点。时间差分方法还是一种节约计算资源的算法，它比 MC 方法更快。并且 MC 方法虽然在过去的数据中误差很低，但在未来的数据中误差较高，它远没有时间差分方法的扩展性能。

最后 Sutton 总结道：



![](https://pic1.zhimg.com/v2-f3ac45304ca06ad220838627db601aa8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='457'></svg>)



本文为机器之心报道，转载请联系本公众号获得授权。




